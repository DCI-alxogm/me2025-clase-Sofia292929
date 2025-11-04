#include <stdio.h>

double f(double x, double y) {
    return y - x - 2*x*x - 2*x*y - y*y;
}

int main() {
    double x, y;
    double fx, fmax;
    double xmax, ymax;

    fmax = -1e9;  // valor inicial muy pequeño
    xmax = ymax = 0;

    // Definimos los pasos de la malla
    double paso = 0.1;

    // Recorremos el rango de x y y
    for (x = -2; x <= 2; x += paso) {
        for (y = 1; y <= 3; y += paso) {
            fx = f(x, y);
            if (fx > fmax) {
                fmax = fx;
                xmax = x;
                ymax = y;
            }
        }
    }

    printf("Máximo encontrado por Malla:\n");
    printf("f(%.2f, %.2f) = %.4f\n", xmax, ymax, fmax);

    return 0;
}
