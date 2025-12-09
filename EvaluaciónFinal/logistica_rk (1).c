/* logistica_rk.c
   Solucion de la EDO logistica usando RK2 (Heun) y RK4.
   dC/dt = k * C * (Cmax - C)
*/

#include <stdio.h>
#include <math.h>   // <-- ESTA LÍNEA QUITA LOS WARNINGS

double k = 0.45;
double Cmax = 1.0;

/* dC/dt */
double f(double C) {
    return k * C * (Cmax - C);
}

/* RK4 */
double rk4_step(double C, double h) {
    double k1 = f(C);
    double k2 = f(C + 0.5*h*k1);
    double k3 = f(C + 0.5*h*k2);
    double k4 = f(C + h*k3);
    return C + (h/6.0)*(k1 + 2*k2 + 2*k3 + k4);
}

/* RK2 (Heun) */
double rk2_heun_step(double C, double h) {
    double k1 = f(C);
    double k2 = f(C + h*k1);
    return C + 0.5*h*(k1 + k2);
}

int main() {
    double C0 = 0.02;
    double h = 0.1;
    double T = 25.0;
    int N = (int)(T / h);

    double Ct_rk4 = C0;
    double Ct_rk2 = C0;
    double t = 0.0;

    printf("t(min)\t\tRK4_C(t)\t\tRK2_C(t)\t\t|diff|\n");
    printf("--------------------------------------------------------------\n");

    printf("%.2f\t\t%.9f\t%.9f\t%.9f\n", t, Ct_rk4, Ct_rk2, fabs(Ct_rk4 - Ct_rk2));

    for (int i = 1; i <= N; i++) {
        Ct_rk4 = rk4_step(Ct_rk4, h);
        Ct_rk2 = rk2_heun_step(Ct_rk2, h);
        t = i * h;

        printf("%.2f\t\t%.9f\t%.9f\t%.9f\n",
               t, Ct_rk4, Ct_rk2, fabs(Ct_rk4 - Ct_rk2));
    }

    return 0;
}
