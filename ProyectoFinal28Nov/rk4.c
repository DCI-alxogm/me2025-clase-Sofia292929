#include <stdio.h>

// ==== EDITA ESTA FUNCIÓN PARA DEFINIR TU EDO ====
// Ejemplo: y' = x + y
double f(double x, double y) {
    return x + y;
}

int main() {
    double x0, y0, h;
    int n;

    printf("=== Metodo Runge Kutta de 4to Orden ===\n");
    printf("Ingresa x0: ");
    scanf("%lf", &x0);

    printf("Ingresa y0: ");
    scanf("%lf", &y0);

    printf("Ingresa tamano de paso h: ");
    scanf("%lf", &h);

    printf("Ingresa numero de iteraciones: ");
    scanf("%d", &n);

    double x = x0;
    double y = y0;

    printf("\nIter\t x\t\t y\n");
    printf("--------------------------------------\n");
    printf("0\t %.6f\t %.6f\n", x, y);

    for (int i = 1; i <= n; i++) {
        double k1 = f(x, y);
        double k2 = f(x + h/2.0, y + (h/2.0)*k1);
        double k3 = f(x + h/2.0, y + (h/2.0)*k2);
        double k4 = f(x + h, y + h*k3);

        y = y + (h/6.0)*(k1 + 2*k2 + 2*k3 + k4);
        x = x + h;

        printf("%d\t %.6f\t %.6f\n", i, x, y);
    }

    return 0;
}
