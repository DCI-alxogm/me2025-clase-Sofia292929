#include <stdio.h>

int main() {
    int numero;
    char opcion;

    do {
        // Pedir número
        printf("Ingrese un numero: ");
        scanf("%d", &numero);

        // Verificar si es par o impar
        if (numero % 2 == 0) {
            printf("El numero %d es PAR.\n", numero);
        } else {
            printf("El numero %d es IMPAR.\n", numero);
        }

        // Preguntar si quiere continuar
        printf("¿Desea analizar otro numero? (s/n): ");
        scanf(" %c", &opcion); // el espacio antes de %c evita problemas con enter

    } while (opcion == 's' || opcion == 'S');

    printf("Programa finalizado.\n");
    return 0;
}
