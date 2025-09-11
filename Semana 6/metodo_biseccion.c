#include <stdio.h>
#include <math.h>

// Función matemática a evaluar
double funcion(double x_valor) {
    return sin(10.0 * x_valor) - cos(3.0 * x_valor);
}

int main() {
    // Definición de parámetros iniciales
    double limite_inferior = 3.05;
    double limite_superior = 3.20;
    double tolerancia = 0.0001;
    double punto_medio, valor_funcion;
    int contador_iteraciones = 0;
    
    // Encabezado de ejecución
    printf("Aplicando método de bisección en el intervalo [%.3f, %.3f]\n", 
           limite_inferior, limite_superior);
    printf("Tolerancia establecida: %.6f\n\n", tolerancia);
    
    // Algoritmo principal de bisección
    do {
        contador_iteraciones++;
        
        // Calcular nuevo punto medio
        punto_medio = (limite_inferior + limite_superior) / 2.0;
        valor_funcion = funcion(punto_medio);
        
        // Verificar si encontramos la raíz exacta
        if (fabs(valor_funcion) < 1e-10) {
            break;
        }
        
        // Actualizar intervalo según el teorema de Bolzano
        if (valor_funcion * funcion(limite_inferior) < 0.0) {
            limite_superior = punto_medio;
        } else {
            limite_inferior = punto_medio;
        }
        
    } while (fabs(limite_superior - limite_inferior) > tolerancia);
    
    // Resultados finales
    printf("=== RESULTADOS OBTENIDOS ===\n");
    printf("Aproximación de la raíz: %.8f\n", punto_medio);
    printf("Valor de la función en la raíz: %.10f\n", funcion(punto_medio));
    printf("Total de iteraciones realizadas: %d\n", contador_iteraciones);
    printf("Error absoluto final: %.8f\n", 
           fabs(limite_superior - limite_inferior));
    
    return 0;
}
