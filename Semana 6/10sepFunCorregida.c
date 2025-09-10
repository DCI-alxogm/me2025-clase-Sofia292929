#include <stdio.h>
#include <math.h>

// Definir la función f(c)
float f(float c) {
    return (9.81f * 68.1f) / c * (1 - expf(-(c / 68.1f) * 10)) - 40.0f;
}

int main() {
    float a = 14.5f, b = 15.5f;  // intervalo inicial
    float k = 0, k_old = 0;
    float err, err_max = 1e-6f;  // tolerancia
    int iter = 0;

    do {
        k_old = k;
        k = (a + b) / 2.0f;  // punto medio

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
