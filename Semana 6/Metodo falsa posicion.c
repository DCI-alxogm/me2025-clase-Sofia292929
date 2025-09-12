//Metodo de la falsa posicion f(c)=gm/c*(1-e^c/m*t)/v
#include <stdio.h>
#include <math.h>

// Definir la función f(c)
float f(float c) {
    return (9.81f * 68.1f) / c * (1 - expf(-(c / 68.1f) * 10)) - 40.0f;
}

int main() {
    float a = 4, b = 20;  // intervalo inicial
    float k = 0, k_old = 0;
    float err, err_max = 1e-6f;  // tolerancia
    int iter = 0;

    do {
        k_old = k;
        k = (f(b)*a-f(a)*b)/(f(b)-f(a));
        if (f(k) * f(a) < 0)
            b = k;
        else
            a = k;

        err = fabsf(k - k_old);

        iter++;
    } while (err > err_max);

    printf("La raiz aproximada es: %f\n", k);
    printf("f(c_raiz) = %f\n", f(k));

    return 0;
}
