#include <stdio.h>
#include <math.h>

// Factorial simple
long double factorial(int n) {
    long double f = 1;
    for(int i=1; i<=n; i++) f *= i;
    return f;
}

int main() {
    int opcion, n = 0;
    long double x, suma = 0, termino, error, real, tolerancia;

    printf("Elige una funcion para aproximar con Serie de Taylor:\n");
    printf("1. sin(x)\n");
    printf("2. cos(x)\n");
    printf("3. exp(x)\n");
    printf("4. ln(1+x)\n");
    printf("5. 1/(1-x)\n");
    printf("Opcion: ");
    scanf("%d", &opcion);

    printf("Ingresa el valor de x: ");
    scanf("%Lf", &x);

    printf("Ingresa el error maximo permitido: ");
    scanf("%Lf", &tolerancia);

    // Valor real para comparar
    switch(opcion) {
        case 1: real = sin(x); break;
        case 2: real = cos(x); break;
        case 3: real = expl(x); break;
        case 4: real = logl(1+x); break;
        case 5: real = 1.0/(1-x); break;
        default: printf("Opcion invalida.\n"); return 0;
    }

    suma = 0;
    n = 0;
    do {
        switch(opcion) {
            case 1: // sin(x) = sum((-1)^n * x^(2n+1) / (2n+1)!)
                termino = powl(-1,n) * powl(x,2*n+1) / factorial(2*n+1);
                break;
            case 2: // cos(x) = sum((-1)^n * x^(2n) / (2n)!)
                termino = powl(-1,n) * powl(x,2*n) / factorial(2*n);
                break;
            case 3: // exp(x) = sum(x^n / n!)
                termino = powl(x,n) / factorial(n);
                break;
            case 4: // ln(1+x) = sum((-1)^(n+1) * x^n / n), n>=1
                if(n==0){ termino=0; } 
                else termino = powl(-1,n+1) * powl(x,n) / n;
                break;
            case 5: // 1/(1-x) = sum(x^n), n>=0
                termino = powl(x,n);
                break;
        }

        suma += termino;
        n++;

        error = fabsl(real - suma);

    } while(error > tolerancia && n < 1000); // evita bucles infinitos

    printf("\nResultado real: %Lf", real);
    printf("\nAproximacion: %Lf", suma);
    printf("\nError: %Lf", error);
    printf("\nNumero de terminos usados: %d\n", n);

    return 0;
}