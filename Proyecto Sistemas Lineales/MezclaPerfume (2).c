// perfume_sistemas.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX 50
#define TOL 1e-8
#define MAX_ITER 10000

void leerMatriz(float A[MAX][MAX+1], int *N, char *archivo);
void imprimirMatriz(float A[MAX][MAX+1], int N);
void evaluarCondicion(float A[MAX][MAX+1], int N);
void pivoteoParcial(float A[MAX][MAX+1], int N, int k);
void normalizarFila(float A[MAX][MAX+1], int N, int fila);

// Métodos
void gaussSimple(float A[MAX][MAX+1], int N, float x[MAX]);
void gaussJordan(float A[MAX][MAX+1], int N, float x[MAX]);
int gaussSeidel(float A[MAX][MAX+1], int N, float x[MAX]); // returns iterations
void factorizacionLU(float A[MAX][MAX+1], int N, float x[MAX]);

// Helpers
float residual_norm(float Aorig[MAX][MAX+1], int N, float xvec[MAX]);
void guardarResultadosCSV(const char *fname, const char *metodo, int N, float xvec[MAX], float resid_norm, int iter);

int main() {
    int N, metodo;
    float A[MAX][MAX+1], A_orig[MAX][MAX+1], x[MAX];
    char opcion, archivo[128];
    
    printf("=== Resolución de sistemas lineales (Mezcla para perfume) ===\n");
    printf("Métodos disponibles:\n");
    printf("1. Eliminación Gaussiana simple\n");
    printf("2. Gauss-Jordan\n");
    printf("3. Gauss-Seidel\n");
    printf("4. Factorización LU\n");
    printf("5. Ejecutar todos y guardar CSV\n");
    printf("Seleccione método (1-5): ");
    if (scanf("%d", &metodo)!=1) return 0;
    
    printf("\n¿Desea leer la matriz desde archivo? (s/n): ");
    scanf(" %c", &opcion);
    if (opcion == 's' || opcion == 'S') {
        printf("Ingrese el nombre del archivo (ejemplo: datos_perfume4.txt): ");
        scanf(" %127s", archivo);
        leerMatriz(A, &N, archivo);
    } else {
        printf("Ingrese la dimensión del sistema (N): ");
        scanf("%d", &N);
        printf("Ingrese los coeficientes de la matriz aumentada (A|B):\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N + 1; j++) {
                scanf("%f", &A[i][j]);
            }
        }
    }
    
    // Guardar copia original
    for (int i=0;i<N;i++) for (int j=0;j<N+1;j++) A_orig[i][j]=A[i][j];
    
    printf("\nMatriz aumentada original:\n");
    imprimirMatriz(A, N);
    evaluarCondicion(A, N);
    
    // If user chose to run all and save, prepare CSV
    if (metodo == 5) {
        FILE *f = fopen("resultados_perfume_template.csv","w");
        if (f) {
            fprintf(f,"metodo");
            for (int i=0;i<N;i++) fprintf(f,",x%d", i+1);
            fprintf(f,",residuo_norm,iteraciones\n");
            fclose(f);
        }
        
        // Gauss-Jordan
        for (int i=0;i<N;i++) for (int j=0;j<N+1;j++) A[i][j]=A_orig[i][j];
        for (int i=0;i<N;i++) x[i]=0.0f;
        gaussJordan(A, N, x);
        float res_gj = residual_norm(A_orig, N, x);
        printf("\n[Gauss-Jordan] Residuo ||Ax-b||_2 = %.6e\n", res_gj);
        guardarResultadosCSV("resultados_perfume_template.csv","Gauss Jordan",N,x,res_gj,0);
        
        // Gauss-Seidel
        for (int i=0;i<N;i++) for (int j=0;j<N+1;j++) A[i][j]=A_orig[i][j];
        for (int i=0;i<N;i++) x[i]=0.0f;
        int iter_gs = gaussSeidel(A, N, x);
        float res_gs = residual_norm(A_orig, N, x);
        printf("\n[Gauss-Seidel] Residuo ||Ax-b||_2 = %.6e | Iter = %d\n", res_gs, iter_gs);
        guardarResultadosCSV("resultados_perfume_template.csv","Gauss Seidel",N,x,res_gs,iter_gs);
        
        // LU
        for (int i=0;i<N;i++) for (int j=0;j<N+1;j++) A[i][j]=A_orig[i][j];
        for (int i=0;i<N;i++) x[i]=0.0f;
        factorizacionLU(A, N, x);
        float res_lu = residual_norm(A_orig, N, x);
        printf("\n[LU] Residuo ||Ax-b||_2 = %.6e\n", res_lu);
        guardarResultadosCSV("resultados_perfume_template.csv","LU",N,x,res_lu,0);
        
        printf("\nResultados guardados en resultados_perfume_template.csv\n");
        return 0;
    }
    
    // Single-method execution
    switch (metodo) {
        case 1:
            gaussSimple(A, N, x);
            break;
        case 2:
            gaussJordan(A, N, x);
            break;
        case 3:
            gaussSeidel(A, N, x);
            break;
        case 4:
            factorizacionLU(A, N, x);
            break;
        default:
            printf("Método no válido.\n");
            return 0;
    }
    
    printf("\n=== Solución del sistema ===\n");
    for (int i = 0; i < N; i++) {
        printf("x%d = %.8f\n", i + 1, x[i]);
    }
    
    float res = residual_norm(A_orig, N, x);
    printf("\nNorma del residuo ||Ax-b||_2 = %.6e\n", res);
    
    return 0;
}

// ================= FUNCIONES BÁSICAS =================
void leerMatriz(float A[MAX][MAX+1], int *N, char *archivo) {
    FILE *fp = fopen(archivo, "r");
    if (!fp) {
        printf("Error: no se puede abrir el archivo %s.\n", archivo);
        exit(1);
    }
    
    if (fscanf(fp, "%d", N) != 1) { 
        printf("Formato inválido en archivo.\n");
        fclose(fp); 
        exit(1);
    }
    
    for (int i = 0; i < *N; i++) {
        for (int j = 0; j < *N + 1; j++) {
            if (fscanf(fp, "%f", &A[i][j])!=1) { 
                printf("Error leyendo coeficientes.\n"); 
                fclose(fp); 
                exit(1);
            }
        }
    }
    fclose(fp);
}

void imprimirMatriz(float A[MAX][MAX+1], int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N + 1; j++) {
            printf("%10.6f ", A[i][j]);
        }
        printf("\n");
    }
}

void evaluarCondicion(float A[MAX][MAX+1], int N) {
    for (int i = 0; i < N; i++) {
        if (fabs(A[i][i]) < 1e-8) {
            printf("\nEl sistema podría estar mal condicionado (pivote pequeño\nen fila %d)\n", i + 1);
        }
    }
}

void pivoteoParcial(float A[MAX][MAX+1], int N, int k) {
    int fila_pivote = k;
    float max_val = fabs(A[k][k]);
    
    for (int i = k + 1; i < N; i++) {
        if (fabs(A[i][k]) > max_val) {
            max_val = fabs(A[i][k]);
            fila_pivote = i;
        }
    }
    
    if (fila_pivote != k) {
        for (int j = 0; j < N + 1; j++) {
            float temp = A[k][j];
            A[k][j] = A[fila_pivote][j];
            A[fila_pivote][j] = temp;
        }
        printf("\nPivoteo: fila %d ↔ fila %d\n", k + 1, fila_pivote + 1);
    }
}

void normalizarFila(float A[MAX][MAX+1], int N, int fila) {
    float pivote = A[fila][fila];
    if (fabs(pivote) > 1e-12) {
        for (int j = fila; j < N + 1; j++) {
            A[fila][j] /= pivote;
        }
    }
}

// ================= MÉTODOS =================
// --- Gauss simple --
void gaussSimple(float A[MAX][MAX+1], int N, float x[MAX]) {
    for (int k = 0; k < N-1; k++) {
        if (fabs(A[k][k]) < 1e-12)
            pivoteoParcial(A, N, k);
            
        for (int i = k + 1; i < N; i++) {
            float factor = A[i][k] / A[k][k];
            for (int j = k; j < N + 1; j++) {
                A[i][j]-= factor * A[k][j];
            }
        }
    }
    
    for (int i = N-1; i >= 0; i--) {
        float suma = 0;
        for (int j = i + 1; j < N; j++)
            suma += A[i][j] * x[j];
        x[i] = (A[i][N]-suma) / A[i][i];
    }
}

// --- Gauss-Jordan --
void gaussJordan(float A[MAX][MAX+1], int N, float x[MAX]) {
    for (int i = 0; i < N; i++) {
        if (fabs(A[i][i]) < 1e-12)
            pivoteoParcial(A, N, i);
            
        normalizarFila(A, N, i);
        
        for (int k = 0; k < N; k++) {
            if (k != i) {
                float factor = A[k][i];
                for (int j = 0; j < N + 1; j++) {
                    A[k][j]-= factor * A[i][j];
                }
            }
        }
    }
    
    for (int i = 0; i < N; i++) {
        x[i] = A[i][N];
    }
}

// --- Gauss-Seidel --
int gaussSeidel(float A[MAX][MAX+1], int N, float x[MAX]) {
    float x_prev[MAX];
    int iter = 0;
    float error;

    // 1) Asegurar que no haya pivotes nulos: pivoteo parcial por columna
    for (int k = 0; k < N; k++) {
        if (fabs(A[k][k]) < 1e-12) {
            pivoteoParcial(A, N, k);
            if (fabs(A[k][k]) < 1e-12) {
                printf("No es posible aplicar Gauss-Seidel: pivote nulo en columna %d.\n", k+1);
                return -1;
            }
        }
    }

    // 2) Comprobar (e informar) si la matriz no es diagonalmente dominante
    int dominantes = 0;
    for (int i = 0; i < N; i++) {
        float diag = fabs(A[i][i]);
        float suma = 0.0f;
        for (int j = 0; j < N; j++) if (j != i) suma += fabs(A[i][j]);
        if (diag >= suma) dominantes++;
    }
    if (dominantes < N) {
        printf("Advertencia: la matriz NO es diagonalmente dominante (%d/%d filas dominantes).\n", dominantes, N);
        printf("Gauss-Seidel puede no converger o converger lentamente.\n");
    }

    // 3) Inicializar x (si quieres otra inicialización, cámbiala aquí)
    for (int i = 0; i < N; i++) x[i] = 0.0f;

    // 4) Iteraciones
    do {
        error = 0.0f;
        for (int i = 0; i < N; i++) {
            x_prev[i] = x[i];
            float suma = 0.0f;
            for (int j = 0; j < N; j++) {
                if (j != i) suma += A[i][j] * x[j]; // usando el valor más reciente (Gauss-Seidel)
            }

            if (fabs(A[i][i]) < 1e-14) {
                printf("Error: pivote casi cero en fila %d durante iteración.\n", i+1);
                return -1;
            }

            x[i] = (A[i][N] - suma) / A[i][i];

            if (!isfinite(x[i])) {
                printf("Divergencia detectada: x[%d] = %f\n", i+1, x[i]);
                return -1;
            }

            error += fabs(x[i] - x_prev[i]);
        }

        iter++;
        if (iter > MAX_ITER) {
            printf("No converge dentro del límite de iteraciones (%d).\n", MAX_ITER);
            return iter;
        }

    } while (error > TOL);

    printf("Iteraciones realizadas (Gauss-Seidel): %d\n", iter);
    return iter;
}

// --- Factorización LU (Doolittle) --
void factorizacionLU(float A[MAX][MAX+1], int N, float x[MAX]) {
    float L[MAX][MAX] = {0}, U[MAX][MAX] = {0}, b[MAX], y[MAX];
    
    for (int i = 0; i < N; i++)
        b[i] = A[i][N];
        
    for (int i = 0; i < N; i++) {
        for (int k = i; k < N; k++) {
            float suma = 0.0f;
            for (int j = 0; j < i; j++)
                suma += L[i][j] * U[j][k];
            U[i][k] = A[i][k]-suma;
        }
        
        for (int k = i; k < N; k++) {
            if (i == k)
                L[i][i] = 1.0f;
            else {
                float suma = 0.0f;
                for (int j = 0; j < i; j++)
                    suma += L[k][j] * U[j][i];
                if (fabs(U[i][i]) < 1e-12) { 
                    printf("Pivote U[%d][%d] nulo.\n", i, i); 
                    exit(1); 
                }
                L[k][i] = (A[k][i]-suma) / U[i][i];
            }
        }
    }
    
    // Sustitución hacia adelante (Ly = b)
    for (int i = 0; i < N; i++) {
        float suma = 0.0f;
        for (int j = 0; j < i; j++)
            suma += L[i][j] * y[j];
        y[i] = b[i]-suma;
    }
    
    // Sustitución hacia atrás (Ux = y)
    for (int i = N-1; i >= 0; i--) {
        float suma = 0.0f;
        for (int j = i + 1; j < N; j++)
            suma += U[i][j] * x[j];
        x[i] = (y[i]- suma) / U[i][i];
    }
    
    printf("\nMatriz L:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%10.6f ", L[i][j]);
        printf("\n");
    }
    
    printf("\nMatriz U:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%10.6f ", U[i][j]);
        printf("\n");
    }
}

// ================= Helpers =================
float residual_norm(float Aorig[MAX][MAX+1], int N, float xvec[MAX]) {
    float r, norm2 = 0.0f;
    for (int i = 0; i < N; i++) {
        r = 0.0f;
        for (int j = 0; j < N; j++) r += Aorig[i][j]*xvec[j];
        r-= Aorig[i][N];
        norm2 += r*r;
    }
    return sqrt(norm2);
}

void guardarResultadosCSV(const char *fname, const char *metodo, int N, float xvec[MAX], float resid_norm, int iter) {
    FILE *fp = fopen(fname, "a");
    if (!fp) {
        printf("No se pudo abrir %s para escribir.\n", fname);
        return;
    }
    
    fprintf(fp, "%s", metodo);
    for (int i=0;i<N;i++) fprintf(fp, ",%.8f", xvec[i]);
    fprintf(fp, ",%.6e,%d\n", resid_norm, iter);
    fclose(fp);
}