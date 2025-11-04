#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double f(double x, double y) {
    return y - x - 2*x*x - 2*x*y - y*y;
}

double randInRange(double min, double max) {
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

int main() {
    double x, y, fx;
    double xmax, ymax, fmax;
    int i, N = 100000;  // número de intentos

    srand(time(NULL));  // inicializa generador aleatorio

    fmax = -1e9;
    xmax = ymax = 0;

    for (i = 0; i < N; i++) {
        x = randInRange(-2, 2);
        y = randInRange(1, 3);
        fx = f(x, y);

        if (fx > fmax) {
            fmax = fx;
            xmax = x;
            ymax = y;
        }
    }

    printf("Máximo encontrado por Búsqueda Aleatoria:\n");
    printf("f(%.4f, %.4f) = %.4f\n", xmax, ymax, fmax);

    return 0;
}
