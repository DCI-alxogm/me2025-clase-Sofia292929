/*
------------------------------------------------------
 Programa: Métodos de búsqueda de raíces
 Martha Sofia Juárez Vázquez
 Fecha: 30 de Septiembre 2025
 Descripción:
 Este programa encuentra la raíz de una función no polinómica
 f(x) = e^(-x) - x usando los métodos:
   - Bisección
   - Falsa Posición (Regula-Falsi)
   - Newton-Raphson
   - Secante
 Para cada método se muestra el error aproximado en cada iteración.
------------------------------------------------------
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Declaración de la función y su derivada
double f(double x) {
    return exp(-x) - x;
}

double df(double x) {
    return -exp(-x) - 1;
}

// ------------------ MÉTODO DE BISECCIÓN ------------------
void biseccion(double a, double b, double tol, int max_iter) {
    printf("\n--- MÉTODO DE BISECCIÓN ---\n");
    if (f(a) * f(b) >= 0) {
        printf("No se cumple f(a)*f(b) < 0. No se puede aplicar.\n");
        return;
    }

    double c, error = 100;
    int iter = 0;

    printf("Iter\t a\t b\t c\t f(c)\t Error(%%)\n");

    while (error > tol && iter < max_iter) {
        c = (a + b) / 2.0;
        double fc = f(c);

        if (iter > 0) error = fabs((b - a) / b) * 100.0;

        printf("%d\t %.6f\t %.6f\t %.6f\t %.6f\t %.4f\n",
               iter + 1, a, b, c, fc, error);

        if (f(a) * fc < 0)
            b = c;
        else
            a = c;

        iter++;
    }
    printf("Raíz aproximada: %.6f\n", c);
}

// ------------------ MÉTODO DE FALSA POSICIÓN ------------------
void falsa_posicion(double a, double b, double tol, int max_iter) {
    printf("\n--- MÉTODO DE FALSA POSICIÓN ---\n");
    if (f(a) * f(b) >= 0) {
        printf("No se cumple f(a)*f(b) < 0. No se puede aplicar.\n");
        return;
    }

    double c, prev = 0, error = 100;
    int iter = 0;

    printf("Iter\t a\t b\t c\t f(c)\t Error(%%)\n");

    while (error > tol && iter < max_iter) {
        c = b - f(b) * (a - b) / (f(a) - f(b));

        if (iter > 0) error = fabs((c - prev) / c) * 100.0;

        printf("%d\t %.6f\t %.6f\t %.6f\t %.6f\t %.4f\n",
               iter + 1, a, b, c, f(c), error);

        if (f(a) * f(c) < 0)
            b = c;
        else
            a = c;

        prev = c;
        iter++;
    }
    printf("Raíz aproximada: %.6f\n", c);
}

// ------------------ MÉTODO DE NEWTON-RAPHSON ------------------
void newton(double x0, double tol, int max_iter) {
    printf("\n--- MÉTODO DE NEWTON-RAPHSON ---\n");

    double x1, error = 100;
    int iter = 0;

    printf("Iter\t x\t f(x)\t Error(%%)\n");

    while (error > tol && iter < max_iter) {
        double fx = f(x0);
        double dfx = df(x0);

        if (dfx == 0) {
            printf("Derivada nula. No se puede continuar.\n");
            return;
        }

        x1 = x0 - fx / dfx;
        if (iter > 0) error = fabs((x1 - x0) / x1) * 100.0;

        printf("%d\t %.6f\t %.6f\t %.4f\n", iter + 1, x0, fx, error);

        x0 = x1;
        iter++;
    }
    printf("Raíz aproximada: %.6f\n", x1);
}

// ------------------ MÉTODO DE SECANTE ------------------
void secante(double x0, double x1, double tol, int max_iter) {
    printf("\n--- MÉTODO DE SECANTE ---\n");

    double x2, error = 100;
    int iter = 0;

    printf("Iter\t x0\t x1\t f(x1)\t Error(%%)\n");

    while (error > tol && iter < max_iter) {
        double fx0 = f(x0);
        double fx1 = f(x1);

        if (fx1 - fx0 == 0) {
            printf("División por cero. No se puede continuar.\n");
            return;
        }

        x2 = x1 - fx1 * (x0 - x1) / (fx0 - fx1);
        if (iter > 0) error = fabs((x2 - x1) / x2) * 100.0;

        printf("%d\t %.6f\t %.6f\t %.6f\t %.4f\n", iter + 1, x0, x1, fx1, error);

        x0 = x1;
        x1 = x2;
        iter++;
    }
    printf("Raíz aproximada: %.6f\n", x2);
}

// ------------------ FUNCIÓN PRINCIPAL ------------------
int main() {
    double a = 0.0, b = 1.0;  // Intervalo inicial
    double tol = 0.0001;      // Tolerancia (error %)
    int max_iter = 20;        // Iteraciones máximas

    printf("Cálculo de raíces para f(x) = e^(-x) - x\n");
    printf("--------------------------------------------------\n");

    biseccion(a, b, tol, max_iter);
    falsa_posicion(a, b, tol, max_iter);
    newton(0.5, tol, max_iter);
    secante(0.0, 1.0, tol, max_iter);

    return 0;
}
 