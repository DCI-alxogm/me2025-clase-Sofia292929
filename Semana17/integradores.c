/* integradores.c
   Resuelve dos iteraciones (h = 0.1 s) de:
   dv/dt = g - (C/m) * ( v + a*(v/vmax)^b )
   usando: Euler explícito, RK2 (midpoint) y RK4.
   Imprime paso a paso valores intermedios (k1,k2,k3,k4) y f(v).
*/

#include <stdio.h>
#include <math.h>

/* Parámetros (tomados de la pizarra) */
const double C = 12.5;     /* kg/s */
const double m = 68.1;     /* kg */
const double a = 8.3;      /* m/s (factor en el término no lineal) */
const double b = 2.2;      /* exponente */
const double vmax = 46.0;  /* m/s */
const double g = 9.81;     /* m/s^2 */

/* Función f(v) = dv/dt */
double f(double v) {
    double ratio = v / vmax;
    double nonlin = 0.0;
    if (v > 0.0) {
        nonlin = a * pow(ratio, b);  /* a*(v/vmax)^b */
    } else {
        /* pow(0, b) = 0, así que también sirve */
        nonlin = a * pow(ratio, b);
    }
    return g - (C / m) * (v + nonlin);
}

int main(void) {
    double h = 0.1;         /* paso */
    int steps = 2;          /* queremos 2 iteraciones: t=0->0.1->0.2 */
    double t;
    int n;

    /* Valores iniciales */
    double v_euler = 0.0;
    double v_rk2   = 0.0;
    double v_rk4   = 0.0;

    printf("Integracion numérica - 2 iteraciones (h = %.2f s)\n", h);
    printf("Ecuacion: dv/dt = g - (C/m)*(v + a*(v/vmax)^b)\n");
    printf("Parametros: C=%.2f, m=%.2f, a=%.2f, b=%.2f, vmax=%.2f, g=%.2f\n\n",
           C, m, a, b, vmax, g);

    /* Impresión de encabezado de tabla resumen */
    printf("-------------------------------------------------------------\n");
    printf("i   t(s)    Euler_v (m/s)    RK2_v (m/s)    RK4_v (m/s)\n");
    printf("-------------------------------------------------------------\n");

    /* imprimir la condición inicial (i=0) */
    printf("%-3d %-7.2f  %-16.10f  %-13.10f  %-13.10f\n",
           0, 0.0, v_euler, v_rk2, v_rk4);

    /* Bucle de iteraciones */
    t = 0.0;
    for (n = 1; n <= steps; ++n) {
        printf("\n================== Iteracion %d: t = %.2f -> %.2f s ==================\n", n-1, t, t + h);

        /* ------------------ Euler explicito ------------------ */
        printf("\n[ Euler explícito ]\n");
        {
            double fv = f(v_euler);
            printf(" v_n = %.10f\n", v_euler);
            printf(" f(v_n) = f(%.10f) = %.10f\n", v_euler, fv);
            double v_next = v_euler + h * fv;
            printf(" v_{n+1} = v_n + h * f(v_n) = %.10f + %.2f * %.10f = %.10f\n",
                   v_euler, h, fv, v_next);
            v_euler = v_next;
        }

        /* ------------------ RK2 (midpoint) ------------------ */
        printf("\n[ RK2 (midpoint) ]\n");
        {
            double k1 = f(v_rk2);
            double v_half = v_rk2 + 0.5 * h * k1;
            double k2 = f(v_half);
            double v_next = v_rk2 + h * k2;

            printf(" v_n = %.10f\n", v_rk2);
            printf(" k1 = f(v_n) = f(%.10f) = %.10f\n", v_rk2, k1);
            printf(" v_half = v_n + (h/2)*k1 = %.10f\n", v_half);
            printf(" k2 = f(v_half) = f(%.10f) = %.10f\n", v_half, k2);
            printf(" v_{n+1} = v_n + h * k2 = %.10f\n", v_next);

            v_rk2 = v_next;
        }

        /* ------------------ RK4 ------------------ */
        printf("\n[ RK4 ]\n");
        {
            double k1 = f(v_rk4);
            double k2 = f(v_rk4 + 0.5 * h * k1);
            double k3 = f(v_rk4 + 0.5 * h * k2);
            double k4 = f(v_rk4 + h * k3);
            double v_next = v_rk4 + (h / 6.0) * (k1 + 2.0*k2 + 2.0*k3 + k4);

            printf(" v_n = %.10f\n", v_rk4);
            printf(" k1 = f(v_n) = f(%.10f) = %.10f\n", v_rk4, k1);
            printf(" k2 = f(v_n + h/2 * k1) = f(%.10f) = %.10f\n", v_rk4 + 0.5*h*k1, k2);
            printf(" k3 = f(v_n + h/2 * k2) = f(%.10f) = %.10f\n", v_rk4 + 0.5*h*k2, k3);
            printf(" k4 = f(v_n + h * k3) = f(%.10f) = %.10f\n", v_rk4 + h*k3, k4);
            printf(" v_{n+1} = v_n + (h/6)*(k1 + 2*k2 + 2*k3 + k4) = %.10f\n", v_next);

            v_rk4 = v_next;
        }

        /* actualizar tiempo e imprimir fila resumen */
        t += h;
        printf("\nResumen iteracion %d (t = %.2f s):\n", n, t);
        printf(" Euler_v = %.10f, RK2_v = %.10f, RK4_v = %.10f\n",
               v_euler, v_rk2, v_rk4);

        printf("-------------------------------------------------------------\n");
        printf("%-3d %-7.2f  %-16.10f  %-13.10f  %-13.10f\n",
               n, t, v_euler, v_rk2, v_rk4);
    }

    printf("\nFin de las %d iteraciones.\n", steps);
    return 0;
}
