#include <stdio.h>
#include <math.h>

// Función que se desea minimizar: f(x) = x^2 - x
double funcion(double x) {
    return x * x - x;
}

// Método de la Sección Dorada (versión simplificada)
void metodo_seccion_dorada(double a, double b) {
    double phi = (sqrt(5.0) - 1.0) / 2.0;
    double x1, x2, f1, f2;
    int paso = 0;
    int iter_error = -1;  // Guarda el número de iteración cuando el error es <= 0.0001
    
    printf("\nBuscando el mínimo en el intervalo [%.2f , %.2f]\n", a, b);
    printf("\nIter |        a        |        b        |    Error    |   x_min    |  f(x_min) \n");
    printf("-----|-----------------|-----------------|-------------|------------|-----------\n");
    
    while (fabs(b - a) > 1e-6) {
        paso++;
        
        x1 = b - phi * (b - a);
        x2 = a + phi * (b - a);
        f1 = funcion(x1);
        f2 = funcion(x2);
        
        double error = fabs(b - a);
        double x_med = (a + b) / 2.0;
        double f_med = funcion(x_med);
        
        // Imprimir cuando se alcanza por primera vez el error <= 0.0001
        if (error <= 0.0001 && iter_error == -1) {
            iter_error = paso;
            printf("%4d | %13.6f | %13.6f | %11.6f | %10.6f | %10.6f  <-- ERROR <= 0.0001\n",
                   paso, a, b, error, x_med, f_med);
        } else {
            printf("%4d | %13.6f | %13.6f | %11.6f | %10.6f | %10.6f\n",
                   paso, a, b, error, x_med, f_med);
        }

        // Reducir el intervalo según los valores de f1 y f2
        if (f1 < f2)
            b = x2;
        else
            a = x1;
    }

    double xmin = (a + b) / 2.0;

    printf("\n=== RESULTADOS ===\n");
    printf("x_min encontrado: %.6f\n", xmin);
    printf("f(x_min): %.6f\n", funcion(xmin));
    printf("Iteraciones totales: %d\n", paso);

    if (iter_error != -1) {
        printf("El error de 0.0001 se alcanzó en la iteración %d\n", iter_error);
        printf("  Intervalo en esa iteración: [%.6f , %.6f]\n", a, b);
        printf("  Error: 0.0001\n");
    } else {
        printf("No se alcanzó el error de 0.0001 dentro del ciclo.\n");
    }
}

int main() {
    double a, b;

    printf("=== METODO DE LA SECCION DORADA ===\n");
    printf("Funcion analizada: f(x) = x^2 - x\n\n");

    printf("Ingrese los limites del intervalo:\n");
    printf("a = "); scanf("%lf", &a);
    printf("b = "); scanf("%lf", &b);

    if (a < b) {
        metodo_seccion_dorada(a, b);
    } else {
        printf("Error: el valor de a debe ser menor que b.\n");
    }

    return 0;
}
