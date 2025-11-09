#include <stdio.h>
#include <math.h>

// ----------------------------
// Función a maximizar
// Cambia esta función fácilmente
// ----------------------------
double f(double x, double y) {
    return -(x*x + y*y) + 4*x + 6*y; // Ejemplo: máximo en (2,3)
}

// ----------------------------
// Aproximación de derivadas parciales (diferencias finitas)
// ----------------------------
double dfdx(double x, double y, double h) {
    return (f(x + h, y) - f(x - h, y)) / (2 * h);
}

double dfdy(double x, double y, double h) {
    return (f(x, y + h) - f(x, y - h)) / (2 * h);
}

// ----------------------------
// Método del gradiente
// ----------------------------
void gradiente(double x0, double y0, double alpha, double tol, int max_iter) {
    double x = x0, y = y0;
    double h = 1e-5;
    int iter = 0;

    printf("Iter\t x\t\t y\t\t f(x,y)\n");
    printf("------------------------------------------\n");

    while (iter < max_iter) {
        double grad_x = dfdx(x, y, h);
        double grad_y = dfdy(x, y, h);

        double x_new = x + alpha * grad_x;
        double y_new = y + alpha * grad_y;

        double cambio = sqrt((x_new - x)*(x_new - x) + (y_new - y)*(y_new - y));

        x = x_new;
        y = y_new;

        printf("%d\t %.6f\t %.6f\t %.6f\n", iter, x, y, f(x,y));

        if (cambio < tol)
            break;

        iter++;
    }

    printf("\nMáximo aproximado en: (%.6f , %.6f)\n", x, y);
    printf("Valor máximo aproximado: %.6f\n", f(x,y));
}

// ----------------------------
// Programa principal
// ----------------------------
int main() {
    double x0, y0;      // valores iniciales
    double alpha;       // paso
    double tol;         // tolerancia
    int max_iter;       // número máximo de iteraciones

    printf("Metodo del Gradiente (Ascenso)\n");
    printf("Funcion: f(x,y) = -(x^2 + y^2) + 4x + 6y\n\n");

    printf("Ingresa valor inicial x0: ");
    scanf("%lf", &x0);
    printf("Ingresa valor inicial y0: ");
    scanf("%lf", &y0);

    printf("Ingresa paso (alpha): ");
    scanf("%lf", &alpha);
    printf("Ingresa tolerancia: ");
    scanf("%lf", &tol);
    printf("Ingresa maximo de iteraciones: ");
    scanf("%d", &max_iter);

    printf("\n");
    gradiente(x0, y0, alpha, tol, max_iter);

    return 0;
}
