#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(void) {
    float x1, x2, a, b, c, e, f;
    int d= 1;


    do {
        printf("Ingrese los valores correspondientes para ""a"", ""b"" y ""c"" del polinomio:");
        scanf ("%f, %f, %f", &a,&b,&c);

        if (a != 0){
            e= (b*b)-(4*a*c);

            if (e >= 0) {
                x1 = (-b + sqrt(e))/ (2*a);
                x2 = (-b - sqrt(e))/(2*a);
                printf("x1= %f, x2= %f\n", x1, x2);
                printf("Desea resolver otro binomio (verdadero = 1, falso = 0)");
                scanf("%d", &d);
            } else {
                f = -b/(2*a);
                e = fabs (e);
                x1 = sqrtf(e) / (2*a);
                x2 = -x1;
                printf("x1= %f, x2= %f\n", f + x1, f + x2);
                printf("Desea resolver otro binomio (verdadero = 1, falso = 0)");
                scanf("%d", &d);
            }
        } else {
            if (b != 0) {
                x1 = -c / b;
                printf("Es un monomio con raiz: %f\n", x1);
                printf("Desea resolver otro binomio (verdadero = 1, falso = 0)");
                scanf("%d", &d);
            } else {
                x1 = c;
                printf("Solución trival: %f\n", x1);
                printf("Desea resolver otro binomio (verdadero = 1, falso = 0)");
                scanf("%d", &d);
            }
        }
    } while (d == 1);

    return 0;

}