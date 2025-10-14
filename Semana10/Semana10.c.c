#include <stdio.h>
#include <math.h>

#define N 3

void mostrarMatriz(const double M[N][N+1]);
int gaussEliminacion(double M[N][N+1]);
int sustitucionAtras(const double M[N][N+1], double soluciones[N]);

int main() {
    printf("=== Resolución de sistemas lineales por el método de Gauss (sin pivoteo) ===\n\n");
    
    double M[N][N+1] = {0};

    for (int i = 0; i < N; i++) {
        printf("Introduce los coeficientes de la fila %d (a%d1 a%d2 a%d3 b%d):\n", i + 1, i + 1, i + 1, i + 1, i + 1);
        for (int j = 0; j < N + 1; j++) {
            scanf("%lf", &M[i][j]);
        }
    }

    printf("\nMatriz aumentada inicial:\n");
    mostrarMatriz(M);

    if (gaussEliminacion(M) != 0) {
        printf("\nError: pivote nulo encontrado. El sistema no puede resolverse sin pivoteo.\n");
        return -1;
    }

    printf("\nMatriz transformada en forma triangular superior:\n");
    mostrarMatriz(M);

    double x[N] = {0};
    if (sustitucionAtras(M, x) != 0) {
        printf("\nError durante la sustitución hacia atrás.\n");
        return -1;
    }

    printf("\n=== Solución del sistema ===\n");
    for (int i = 0; i < N; i++) {
        printf("x%d = %.6f\n", i + 1, x[i]);
    }

    return 0;
}

int gaussEliminacion(double M[N][N+1]) {
    for (int k = 0; k < N - 1; k++) {
        double pivote = M[k][k];
        if (fabs(pivote) < 1e-12) {
            return -1; // pivote muy pequeño → posible división por cero
        }

        for (int i = k + 1; i < N; i++) {
            double factor = M[i][k] / pivote;
            M[i][k] = 0.0; // limpiar columna

            for (int j = k + 1; j < N + 1; j++) {
                M[i][j] -= factor * M[k][j];
            }
        }
    }
    return 0;
}

int sustitucionAtras(const double M[N][N+1], double soluciones[N]) {
    for (int i = N - 1; i >= 0; i--) {
        double suma = 0.0;
        for (int j = i + 1; j < N; j++) {
            suma += M[i][j] * soluciones[j];
        }

        if (fabs(M[i][i]) < 1e-12) return -1;
        soluciones[i] = (M[i][N] - suma) / M[i][i];
    }
    return 0;
}

void mostrarMatriz(const double M[N][N+1]) {
    for (int i = 0; i < N; i++) {
        printf("[ ");
        for (int j = 0; j < N; j++) {
            printf("%9.4f ", M[i][j]);
        }
        printf("| %9.4f ]\n", M[i][N]);
    }
}

