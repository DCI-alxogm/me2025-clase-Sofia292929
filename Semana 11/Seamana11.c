#include <stdio.h>
#include <math.h>

int main() {
    int i, j, k;
    int N = 3;
    float A[3][4] = {
        {2, 3, 0, 8},
        {4, 6, 7, -3},
        {2, 1, 6, 5}
    };
    float pivote[4], x[3];

    printf("=== Eliminación Gaussiana con pivoteo parcial ===\n\n");
    printf("Matriz original:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N + 1; j++) {
            printf("%8.3f ", A[i][j]);
        }
        printf("\n");
    }

    // Eliminación con pivoteo parcial
    for (i = 0; i < N - 1; i++) {
        // Buscar pivote (mayor valor absoluto en la columna i)
        int fila_pivote = i;
        float max_val = fabs(A[i][i]);
        for (j = i + 1; j < N; j++) {
            if (fabs(A[j][i]) > max_val) {
                max_val = fabs(A[j][i]);
                fila_pivote = j;
            }
        }

        // Intercambiar filas si es necesario
        if (fila_pivote != i) {
            for (k = 0; k < N + 1; k++) {
                pivote[k] = A[i][k];
                A[i][k] = A[fila_pivote][k];
                A[fila_pivote][k] = pivote[k];
            }
        }

        // Evitar división por cero
        if (fabs(A[i][i]) < 1e-8) {
            printf("\nError: pivote nulo en fila %d.\n", i + 1);
            return -1;
        }

        // Eliminación hacia adelante
        for (j = i + 1; j < N; j++) {
            float factor = A[j][i] / A[i][i];
            for (k = i; k < N + 1; k++) {
                A[j][k] -= factor * A[i][k];
            }
        }
    }

    // Sustitución hacia atrás
    for (i = N - 1; i >= 0; i--) {
        float suma = 0.0;
        for (j = i + 1; j < N; j++) {
            suma += A[i][j] * x[j];
        }
        x[i] = (A[i][N] - suma) / A[i][i];
    }

    printf("\nMatriz triangular superior:\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N + 1; j++) {
            printf("%8.3f ", A[i][j]);
        }
        printf("\n");
    }

    printf("\nSolución del sistema:\n");
    for (i = 0; i < N; i++) {
        printf("x%d = %.4f\n", i + 1, x[i]);
    }

    return 0;
}
