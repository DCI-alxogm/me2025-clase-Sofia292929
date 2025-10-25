#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 20
#define TOL 1e-5
#define MAX_ITER 1000

void leerMatriz(float A[MAX][MAX+1], int *N, char *archivo);
void imprimirMatriz(float A[MAX][MAX+1], int N);
void evaluarCondicion(float A[MAX][MAX+1], int N);
void pivoteoParcial(float A[MAX][MAX+1], int N, int k);
void normalizarFila(float A[MAX][MAX+1], int N, int fila);

// Métodos
void gaussSimple(float A[MAX][MAX+1], int N, float x[MAX]);
void gaussJordan(float A[MAX][MAX+1], int N, float x[MAX]);
void gaussSeidel(float A[MAX][MAX+1], int N, float x[MAX]);
void factorizacionLU(float A[MAX][MAX+1], int N, float x[MAX]);

int main() {
    int N, metodo;
    float A[MAX][MAX+1], x[MAX] = {0};
    char opcion, archivo[50];

    printf("=== Resolución de sistemas lineales ===\n");
    printf("Métodos disponibles:\n");
    printf("1. Eliminación Gaussiana simple\n");
    printf("2. Gauss-Jordan\n");
    printf("3. Gauss-Seidel\n");
    printf("4. Factorización LU\n");
    printf("Seleccione método (1-4): ");
    scanf("%d", &metodo);

    printf("\n¿Desea leer la matriz desde archivo? (s/n): ");
    scanf(" %c", &opcion);

    if (opcion == 's' || opcion == 'S') {
        printf("Ingrese el nombre del archivo (ejemplo: datos.txt): ");
        scanf("%s", archivo);
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

    printf("\nMatriz aumentada original:\n");
    imprimirMatriz(A, N);
    evaluarCondicion(A, N);

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
        printf("x%d = %.6f\n", i + 1, x[i]);
    }

    return 0;
}

// ================= FUNCIONES BÁSICAS =================

void leerMatriz(float A[MAX][MAX+1], int *N, char *archivo) {
    FILE *fp = fopen(archivo, "r");
    if (!fp) {
        printf("Error: no se puede abrir el archivo.\n");
        exit(1);
    }

    fscanf(fp, "%d", N);
    for (int i = 0; i < *N; i++) {
        for (int j = 0; j < *N + 1; j++) {
            fscanf(fp, "%f", &A[i][j]);
        }
    }
    fclose(fp);
}

void imprimirMatriz(float A[MAX][MAX+1], int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N + 1; j++) {
            printf("%8.3f ", A[i][j]);
        }
        printf("\n");
    }
}

void evaluarCondicion(float A[MAX][MAX+1], int N) {
    for (int i = 0; i < N; i++) {
        if (fabs(A[i][i]) < 1e-6) {
            printf("\nEl sistema podría estar mal condicionado (pivote pequeño en fila %d)\n", i + 1);
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
    if (fabs(pivote) > 1e-8) {
        for (int j = fila; j < N + 1; j++) {
            A[fila][j] /= pivote;
        }
    }
}

// ================= MÉTODOS =================

// --- Gauss simple ---
void gaussSimple(float A[MAX][MAX+1], int N, float x[MAX]) {
    for (int k = 0; k < N - 1; k++) {
        if (fabs(A[k][k]) < 1e-8)
            pivoteoParcial(A, N, k);
        for (int i = k + 1; i < N; i++) {
            float factor = A[i][k] / A[k][k];
            for (int j = k; j < N + 1; j++) {
                A[i][j] -= factor * A[k][j];
            }
        }
    }

    for (int i = N - 1; i >= 0; i--) {
        float suma = 0;
        for (int j = i + 1; j < N; j++)
            suma += A[i][j] * x[j];
        x[i] = (A[i][N] - suma) / A[i][i];
    }
}

// --- Gauss-Jordan ---
void gaussJordan(float A[MAX][MAX+1], int N, float x[MAX]) {
    for (int i = 0; i < N; i++) {
        if (fabs(A[i][i]) < 1e-8)
            pivoteoParcial(A, N, i);

        normalizarFila(A, N, i);

        for (int k = 0; k < N; k++) {
            if (k != i) {
                float factor = A[k][i];
                for (int j = 0; j < N + 1; j++) {
                    A[k][j] -= factor * A[i][j];
                }
            }
        }
    }

    for (int i = 0; i < N; i++) {
        x[i] = A[i][N];
    }
}

// --- Gauss-Seidel ---
void gaussSeidel(float A[MAX][MAX+1], int N, float x[MAX]) {
    float x_prev[MAX];
    for (int i = 0; i < N; i++) x[i] = 0;

    int iter = 0;
    float error;

    do {
        error = 0.0;
        for (int i = 0; i < N; i++) {
            x_prev[i] = x[i];
            float suma = 0;
            for (int j = 0; j < N; j++) {
                if (j != i) suma += A[i][j] * x[j];
            }
            x[i] = (A[i][N] - suma) / A[i][i];
            error += fabs(x[i] - x_prev[i]);
        }
        iter++;
    } while (error > TOL && iter < MAX_ITER);

    printf("\nIteraciones realizadas: %d\n", iter);
    if (iter >= MAX_ITER)
        printf("No converge dentro del límite de iteraciones.\n");
}

// --- Factorización LU (Doolittle) ---
void factorizacionLU(float A[MAX][MAX+1], int N, float x[MAX]) {
    float L[MAX][MAX] = {0}, U[MAX][MAX] = {0}, b[MAX], y[MAX];

    for (int i = 0; i < N; i++)
        b[i] = A[i][N];

    for (int i = 0; i < N; i++) {
        for (int k = i; k < N; k++) {
            float suma = 0;
            for (int j = 0; j < i; j++)
                suma += L[i][j] * U[j][k];
            U[i][k] = A[i][k] - suma;
        }

        for (int k = i; k < N; k++) {
            if (i == k)
                L[i][i] = 1;
            else {
                float suma = 0;
                for (int j = 0; j < i; j++)
                    suma += L[k][j] * U[j][i];
                L[k][i] = (A[k][i] - suma) / U[i][i];
            }
        }
    }

    // Sustitución hacia adelante (Ly = b)
    for (int i = 0; i < N; i++) {
        float suma = 0;
        for (int j = 0; j < i; j++)
            suma += L[i][j] * y[j];
        y[i] = b[i] - suma;
    }

    // Sustitución hacia atrás (Ux = y)
    for (int i = N - 1; i >= 0; i--) {
        float suma = 0;
        for (int j = i + 1; j < N; j++)
            suma += U[i][j] * x[j];
        x[i] = (y[i] - suma) / U[i][i];
    }

    printf("\nMatriz L:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%8.3f ", L[i][j]);
        printf("\n");
    }

    printf("\nMatriz U:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%8.3f ", U[i][j]);
        printf("\n");
    }
}
