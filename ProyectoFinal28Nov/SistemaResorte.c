#include <stdio.h>

/*
 Sistema de ecuaciones diferenciales:
 dx/dt = v
 dv/dt = -k*x

 Ejemplo: masa-resorte sin fricción (oscilador armónico)
*/

int main(){
    double h = 0.1;   // paso
    double t = 0.0;   // tiempo

    double k = 1.0;   // constante del resorte

    // Condiciones iniciales
    double x = 1.0;   // posicion inicial
    double v = 0.0;   // velocidad inicial

    int i;

    printf("Sistema de EDO: masa-resorte sin friccion\n");
    printf("dx/dt = v,  dv/dt = -k*x\n\n");

    printf("i   t      x(t)        v(t)\n");
    printf("-------------------------------------\n");

    for(i=0;i<=10;i++){
        printf("%d  %.2f   %.6f   %.6f\n", i, t, x, v);

        // Método de Euler
        double dx = v;          // dx/dt
        double dv = -k * x;     // dv/dt

        x = x + h * dx;
        v = v + h * dv;

        t += h;
    }

    return 0;
}
