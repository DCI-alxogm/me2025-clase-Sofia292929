 //Metodo de la falsa posicion f(x,10)/1
#include <stdio.h>
#include <math.h>

// Definir la función f(c)
float f(float x) {
     
 return powf (x,10)-1;;
}

int main() {
    float a = 0, b = 1.3;  // intervalo inicial
    float k = 0, k_old = 0;
    float err, err_max = 0.0001f;  // tolerancia
    int iter = 0;

    do {
        k_old = k;
        k = (f(b)*a-f(a)*b)/(f(b)-f(a));
        if (f(k) * f(a) < 0)
            b = k;
        else
            a = k;

        err = fabsf(k - k_old);
        
        printf("%-6d | %-8f |%-8f |%-8f |%-8f \n", iter, k_old, k, err);
        iter++;
    } while (err > err_max);

    printf("La raiz aproximada es: %f\n", k);
    printf("f(c_raiz) = %f\n", f(k));

    return 0;
}
 
 
