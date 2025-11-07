#include <stdio.h>
#include <math.h>

int main() {
    int m, n, i, j, k;

    printf("CALCULO DE INVERSA GENERALIZADA\n");
    printf("METODO: FACTORIZACION LU PASO A PASO\n\n");

    printf("Ingrese el numero de filas de A: ");
    scanf("%d", &m);
    printf("Ingrese el numero de columnas de A: ");
    scanf("%d", &n);

    double A[m][n], AT[n][m], ATA[n][n];
    double L[n][n], U[n][n], Inv[n][n];
    double I[n][n], Y[n], X[n];

    // Inicializar matrices
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            ATA[i][j] = 0;
            L[i][j] = 0;
            U[i][j] = 0;
            Inv[i][j] = 0;
            I[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    // Leer matriz A
    printf("\nIngrese los valores de la matriz A (%dx%d):\n", m, n);
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            scanf("%lf", &A[i][j]);
        }
    }

    // Mostrar A
    printf("\nMATRIZ A:\n");
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            printf("%8.3f ", A[i][j]);
        }
        printf("\n");
    }

    // Calcular A transpuesta
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            AT[j][i] = A[i][j];
        }
    }

    // Calcular ATA = AT * A
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            for(k = 0; k < m; k++){
                ATA[i][j] += AT[i][k] * A[k][j];
            }
        }
    }

    printf("\nMATRIZ A^T * A:\n");
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("%8.3f ", ATA[i][j]);
        }
        printf("\n");
    }

    // Factorización LU
    for(i = 0; i < n; i++){
        L[i][i] = 1.0;  // diagonal de L
    }

    for(i = 0; i < n; i++){
        // Calcular fila i de U
        for(j = i; j < n; j++){
            double suma = 0;
            for(k = 0; k < i; k++)
                suma += L[i][k] * U[k][j];
            U[i][j] = ATA[i][j] - suma;
        }

        // Calcular columna i de L
        for(j = i+1; j < n; j++){
            double suma = 0;
            for(k = 0; k < i; k++)
                suma += L[j][k] * U[k][i];
            L[j][i] = (ATA[j][i] - suma) / U[i][i];
        }
    }

    printf("\nMATRIZ L:\n");
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("%10.5f ", L[i][j]);
        }
        printf("\n");
    }

    printf("\nMATRIZ U:\n");
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("%10.5f ", U[i][j]);
        }
        printf("\n");
    }

    // Calcular la inversa de ATA
    for(int col = 0; col < n; col++){
        // Resolver L * Y = I(:,col)
        for(i = 0; i < n; i++){
            Y[i] = I[i][col];
            for(j = 0; j < i; j++){
                Y[i] -= L[i][j] * Y[j];
            }
        }

        // Resolver U * X = Y
        for(i = n - 1; i >= 0; i--){
            X[i] = Y[i];
            for(j = i + 1; j < n; j++){
                X[i] -= U[i][j] * X[j];
            }
            X[i] /= U[i][i];
            Inv[i][col] = X[i];
        }
    }

    printf("\nINVERSA DE (A^T * A):\n");
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("%10.5f ", Inv[i][j]);
        }
        printf("\n");
    }

    return 0;
}
