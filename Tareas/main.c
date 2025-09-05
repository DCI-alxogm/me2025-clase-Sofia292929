#include <stdio.h>
#include <math.h>

// Función integrada sin la constante
double f_integrada_sin_C(double x) {
    return -0.1*pow(x,4) - 0.15*pow(x,3) - 0.5*pow(x,2) - 0.25*x;
}

// Función completa con la constante ya determinada
double f(double x, double C) {
    return f_integrada_sin_C(x) + C;
}

void calcular_constante(double x0, double fx0) {
    double valor_sin_C = f_integrada_sin_C(x0);
    double C = fx0 - valor_sin_C;

    printf("\n====================================\n");
    printf("Punto usado: (x0 = %.3f , f(x0) = %.3f)\n", x0, fx0);
    printf("Constante de integración C = %.6f\n", C);

    printf("La función reconstruida es:\n");
    printf("f(x) = -0.1*x^4 - 0.15*x^3 - 0.5*x^2 - 0.25*x + %.6f\n", C);

    // Validación
    double resultado = f(x0, C);
    printf("Validación: f(%.3f) = %.6f\n", x0, resultado);
}

int main() {
    // Caso 1: punto (0.5 , 0.925)
    calcular_constante(0.5, 0.925);

    // Caso 2: punto (1.25 , 0.925)
    calcular_constante(1.25, 0.925);

    // Caso 3: punto (0.2 , 0.925)
    calcular_constante(0.2, 0.925);

    return 0;
}
