#include <stdio.h>
#include <math.h>

// Función que se quiere minimizar: f(x) = x² - 4x + 4
double funcion(double x) {
    return x * x - 4 * x + 4;
}

// Método de Interpolación Cuadrática
void metodo_interpolacion_cuadratica(double x0, double x1, double x2) {
    double f0, f1, f2;
    double x3, f3;
    int paso = 0;
    int paso_error = -1;   // Iteración donde se alcanza el error <= 0.0001
    double x_anterior = 1e10;

    printf("=== METODO DE INTERPOLACION CUADRATICA ===\n");
    printf("Funcion: f(x) = x^2 - 4x + 4\n");
    printf("Valores iniciales: x0 = %.2f, x1 = %.2f, x2 = %.2f\n\n", x0, x1, x2);

    printf("Iter |     x0      |     x1      |     x2      |     x3      |   Error    |   f(x3)\n");
    printf("-----|-------------|-------------|-------------|-------------|------------|-----------\n");

    do {
        paso++;

        // Evaluar los tres puntos actuales
        f0 = funcion(x0);
        f1 = funcion(x1);
        f2 = funcion(x2);

        // Cálculo del nuevo punto (x3)
        double num = f0 * (x1 * x1 - x2 * x2) + f1 * (x2 * x2 - x0 * x0) + f2 * (x0 * x0 - x1 * x1);
        double den = 2.0 * (f0 * (x1 - x2) + f1 * (x2 - x0) + f2 * (x0 - x1));

        if (fabs(den) < 1e-15) {
            printf("Advertencia: el denominador es demasiado pequeño. Se detiene el proceso.\n");
            break;
        }

        x3 = num / den;
        f3 = funcion(x3);

        double error = (paso > 1) ? fabs(x3 - x_anterior) : fabs(x3 - x1);
        x_anterior = x3;

        // Mostrar los resultados por iteración
        if (error <= 0.0001 && paso_error == -1) {
            paso_error = paso;
            printf("%4d | %11.6f | %11.6f | %11.6f | %11.6f | %10.6f | %10.6f  <-- ERROR <= 0.0001\n",
                   paso, x0, x1, x2, x3, error, f3);
        } else {
            printf("%4d | %11.6f | %11.6f | %11.6f | %11.6f | %10.6f | %10.6f\n",
                   paso, x0, x1, x2, x3, error, f3);
        }

        // Reemplazar el punto con el valor de función más alto
        if (f0 >= f1 && f0 >= f2)
            x0 = x3;
        else if (f1 >= f0 && f1 >= f2)
            x1 = x3;
        else
            x2 = x3;

        if (error < 1e-10)
            break;

    } while (paso < 20);

    // Buscar el mejor valor final
    double puntos[3] = {x0, x1, x2};
    double mejor_x = x3;
    double mejor_f = f3;

    for (int i = 0; i < 3; i++) {
        double val = funcion(puntos[i]);
        if (val < mejor_f) {
            mejor_f = val;
            mejor_x = puntos[i];
        }
    }

    printf("\n=== RESULTADOS ===\n");
    printf("Mejor valor de x: %.8f\n", mejor_x);
    printf("f(x) minimo: %.8f\n", mejor_f);
    printf("Iteraciones realizadas: %d\n", paso);

    if (paso_error != -1)
        printf("El error <= 0.0001 se alcanzo en la iteracion %d\n", paso_error);
    else
        printf("No se alcanzo el error de 0.0001 dentro del limite de iteraciones.\n");
}

int main() {
    double x0, x1, x2;

    printf("INTERPOLACION CUADRATICA\n");
    printf("Funcion: f(x) = x^2 - 4x + 4\n\n");

    printf("Ingrese tres valores iniciales distintos:\n");
    printf("x0: "); scanf("%lf", &x0);
    printf("x1: "); scanf("%lf", &x1);
    printf("x2: "); scanf("%lf", &x2);

    if (x0 == x1 || x1 == x2 || x0 == x2) {
        printf("Error: Los puntos deben ser diferentes.\n");
        return 1;
    }

    metodo_interpolacion_cuadratica(x0, x1, x2);

    return 0;
}

