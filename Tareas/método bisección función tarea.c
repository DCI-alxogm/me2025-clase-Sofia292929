#include <stdio.h>
#include <math.h>

// Definir la función f(x)
float f(float x) {
    return sin(10.0f * x) - cos(3.0f * x);
}

int main() {
    float x_min = 3.0f, x_max = 5.0f;
    float delta = 0.01f;       // paso para buscar cambios de signo
    float a, b, c, c_old;
    float err, Er_max = 0.001f;
    int i, iter;
    
    printf("Iter | a        | b        | c        | f(c)     | Error\n");
    printf("------------------------------------------------------------\n");

    a = x_min;
    while (a < x_max) {
        b = a + delta;
        if (f(a) * f(b) <= 0) {  // cambio de signo, hay raíz
            // Método de bisección
            iter = 0;
            c = (a + b) / 2.0f;
            do {
                c_old = c;
                c = (a + b) / 2.0f;
                if (f(a) * f(c) < 0)
                    b = c;
                else
                    a = c;

                err = fabsf((c - c_old)/c); // error relativo
                iter++;
            } while (err > Er_max);

            float Ea = fabsf(b - a); // longitud del intervalo final
            int n = iter;
            float delta_x = b - a;
            float Ea_formula = delta_x / powf(2.0f, n);

            printf("%3d  | %.6f | %.6f | %.6f | %.6f | %.6f\n",
                   iter, a, b, c, f(c), err);
            printf("Relación Ea=δx/2^n : %.6f\n", Ea_formula);

            // Preparar siguiente búsqueda
            a = b;
        } else {
            a = b;
        }
    }

    return 0;
}
