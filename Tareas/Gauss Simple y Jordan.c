#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 20

void leerMatriz(float A[MAX][MAX+1], int *N, char *archivo);
void imprimirMatriz(float A[MAX][MAX+1], int N);
void evaluarCondicion(float A[MAX][MAX+1], int N);
void gaussSimple(float A[MAX][MAX+1], int N, float x[MAX]);
void gaussJordan(float A[MAX][MAX+1], int N, float x[MAX]);
void pivoteoParcial(float A[MAX][MAX+1], int N, int k);

int main() {
    int N, metodo;
    float A[MAX][MAX+1], x[MAX];
    char opcion, archivo[50];

    printf("=== Resolución de sistemas lineales ===\n");
    printf("Métodos disponibles:\n");
    printf("1. Eliminación Gaussiana simple\n");
    printf("2. Gauss-Jordan\n");
    printf("Seleccione método (1-2): ");
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

    if (metodo == 1)
        gaussSimple(A, N, x);
    else if (metodo == 2)
        gaussJordan(A, N, x);
    else {
        printf("Método no válido.\n");
        return 0;
    }

    printf("\n=== Solución del sistema ===\n");
    for (int i = 0; i < N; i++) {
        printf("x%d = %.5f\n", i + 1, x[i]);
    }

    return 0;
}

// ================= FUNCIONES =================

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
        printf("\nSe realizó pivoteo: se intercambió fila %d con fila %d\n", k + 1, fila_pivote + 1);
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

// ---------- MÉTODO 1: GAUSS SIMPLE ----------
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

// ---------- MÉTODO 2: GAUSS-JORDAN ----------
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

