#include <stdio.h>
#include <math.h>

// Definir la función f(x,y)
double f(double x, double y) {
    return 2*x*y + 2*x - x*x - 2*y*y;
}

// Derivadas parciales aproximadas con diferencias finitas
double dfdx(double x, double y, double h) {
    return (f(x + h, y) - f(x - h, y)) / (2*h);
}

double dfdy(double x, double y, double h) {
    return (f(x, y + h) - f(x, y - h)) / (2*h);
}

int main() {
    double x = 1.0, y = 1.0;     // valores iniciales
    double alpha = 0.1;          // paso del gradiente
    double tol = 1e-6;           // tolerancia para detener
    double dx, dy;
    double h = 1e-5;
    int iter = 0, max_iter = 1000;

    printf("Metodo del gradiente\n");
    printf("f(x,y) = 2xy + 2x - x^2 - 2y^2\n\n");
    printf("Iter\t x\t\t y\t\t f(x,y)\n");

    while (iter < max_iter) {
        dx = dfdx(x, y, h);
        dy = dfdy(x, y, h);

        double x_new = x + alpha * dx;
        double y_new = y + alpha * dy;

        // imprimir cada paso
        printf("%d\t %.6f\t %.6f\t %.6f\n", iter, x, y, f(x, y));

        // condición de parada
        if (fabs(x_new - x) < tol && fabs(y_new - y) < tol)
            break;

        x = x_new;
        y = y_new;
        iter++;
    }

    printf("\nMaximo aproximado en:\n");
    printf("x = %.6f, y = %.6f\n", x, y);
    printf("f(x,y) = %.6f\n", f(x, y));

    return 0;
}


