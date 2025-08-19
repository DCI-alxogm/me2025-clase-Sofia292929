#include <stadio.h>
#include <math.h>

int main() {
    double x, errorEsperado, termino, suma = 0.0, error = 100.0;
    int n = 0;
    
    printf("Calculo de e^x con serie de Maclaurin\n");
    printf("Ingrese el valor de x: ");
    scanf("%lf", &x);
    printf("Ingrese el error porcentual esperado: ");
    scanf("%lf", &errorEsperado);
