#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_DATOS 500
#define NUM_PARAMS 4
#define MAX_ITER 100000
#define TOLERANCIA 1e-9
#define MAX_LINE 1024

// Estructura para los datos experimentales
typedef struct {
    double aw[MAX_DATOS];
    double mc[MAX_DATOS];
    int n_puntos;
} DatosExperimentales;

// Estructura para parámetros
typedef struct {
    double params[NUM_PARAMS];
    double chi_cuadrado;
    double rmse;
} Parametros;

// Modelo de Peleg: Xe = b0*aw^b1 + b2*aw^b3
double modelo_peleg(double aw, double *params) {
    if (aw < 0.0 || aw > 1.0) return 0.0;
    return params[0] * pow(aw, params[1]) + params[2] * pow(aw, params[3]);
}

// Modelo DLP: Xe = b0 + b1*x + b2*x^2 + b3*x^3, donde x = ln(-ln(aw))
double modelo_dlp(double aw, double *params) {
    if (aw <= 0.001 || aw >= 0.999) return 0.0;
    
    double x = log(-log(aw));
    return params[0] + params[1]*x + params[2]*x*x + params[3]*x*x*x;
}

// Calcular chi-cuadrado
double calcular_chi2(DatosExperimentales *datos, double *params, int modelo_tipo) {
    double chi2 = 0.0;
    double prediccion, residuo;
    
    for (int i = 0; i < datos->n_puntos; i++) {
        if (modelo_tipo == 1) {
            prediccion = modelo_peleg(datos->aw[i], params);
        } else {
            prediccion = modelo_dlp(datos->aw[i], params);
        }
        
        residuo = datos->mc[i] - prediccion;
        chi2 += residuo * residuo;
    }
    
    return chi2;
}

// Algoritmo Simplex-Nelder-Mead simplificado
void optimizar_simplex(DatosExperimentales *datos, Parametros *resultado, int modelo_tipo) {
    double simplex[NUM_PARAMS + 1][NUM_PARAMS];
    double valores[NUM_PARAMS + 1];
    double centroide[NUM_PARAMS];
    double reflejado[NUM_PARAMS];
    double expandido[NUM_PARAMS];
    double contraido[NUM_PARAMS];
    
    double alpha = 1.0;   // Reflexión
    double gamma = 2.0;   // Expansión
    double rho = 0.5;     // Contracción
    double sigma = 0.5;   // Reducción
    
    // Inicializar simplex
    if (modelo_tipo == 1) { // Peleg
        simplex[0][0] = 2.5;  simplex[0][1] = 0.15;  simplex[0][2] = 30.0;  simplex[0][3] = 12.0;
        simplex[1][0] = 3.0;  simplex[1][1] = 0.15;  simplex[1][2] = 30.0;  simplex[1][3] = 12.0;
        simplex[2][0] = 2.5;  simplex[2][1] = 0.20;  simplex[2][2] = 30.0;  simplex[2][3] = 12.0;
        simplex[3][0] = 2.5;  simplex[3][1] = 0.15;  simplex[3][2] = 35.0;  simplex[3][3] = 12.0;
        simplex[4][0] = 2.5;  simplex[4][1] = 0.15;  simplex[4][2] = 30.0;  simplex[4][3] = 15.0;
    } else { // DLP
        simplex[0][0] = 2.4;  simplex[0][1] = 0.3;   simplex[0][2] = -0.5;  simplex[0][3] = -0.8;
        simplex[1][0] = 2.6;  simplex[1][1] = 0.3;   simplex[1][2] = -0.5;  simplex[1][3] = -0.8;
        simplex[2][0] = 2.4;  simplex[2][1] = 0.4;   simplex[2][2] = -0.5;  simplex[2][3] = -0.8;
        simplex[3][0] = 2.4;  simplex[3][1] = 0.3;   simplex[3][2] = -0.6;  simplex[3][3] = -0.8;
        simplex[4][0] = 2.4;  simplex[4][1] = 0.3;   simplex[4][2] = -0.5;  simplex[4][3] = -0.9;
    }
    
    printf("Optimizando modelo %s con método Simplex...\n", modelo_tipo == 1 ? "PELEG" : "DLP");
    
    // Calcular valores iniciales
    for (int i = 0; i <= NUM_PARAMS; i++) {
        valores[i] = calcular_chi2(datos, simplex[i], modelo_tipo);
    }
    
    printf("Chi² inicial: %.6f\n", valores[0]);
    
    // Iteraciones principales
    for (int iter = 0; iter < MAX_ITER; iter++) {
        // Ordenar vértices (burbuja simple)
        for (int i = 0; i <= NUM_PARAMS; i++) {
            for (int j = i + 1; j <= NUM_PARAMS; j++) {
                if (valores[i] > valores[j]) {
                    double temp_val = valores[i];
                    valores[i] = valores[j];
                    valores[j] = temp_val;
                    
                    for (int k = 0; k < NUM_PARAMS; k++) {
                        double temp_param = simplex[i][k];
                        simplex[i][k] = simplex[j][k];
                        simplex[j][k] = temp_param;
                    }
                }
            }
        }
        
        // Verificar convergencia
        if (fabs(valores[NUM_PARAMS] - valores[0]) < TOLERANCIA) {
            printf("Convergencia alcanzada en iteración %d\n", iter);
            break;
        }
        
        // Calcular centroide (excluyendo el peor punto)
        for (int i = 0; i < NUM_PARAMS; i++) {
            centroide[i] = 0.0;
            for (int j = 0; j < NUM_PARAMS; j++) {
                centroide[i] += simplex[j][i];
            }
            centroide[i] /= NUM_PARAMS;
        }
        
        // Reflexión
        for (int i = 0; i < NUM_PARAMS; i++) {
            reflejado[i] = centroide[i] + alpha * (centroide[i] - simplex[NUM_PARAMS][i]);
        }
        double valor_reflejado = calcular_chi2(datos, reflejado, modelo_tipo);
        
        if (valor_reflejado < valores[NUM_PARAMS - 1] && valor_reflejado >= valores[0]) {
            for (int i = 0; i < NUM_PARAMS; i++) {
                simplex[NUM_PARAMS][i] = reflejado[i];
            }
            valores[NUM_PARAMS] = valor_reflejado;
            continue;
        }
        
        // Expansión
        if (valor_reflejado < valores[0]) {
            for (int i = 0; i < NUM_PARAMS; i++) {
                expandido[i] = centroide[i] + gamma * (reflejado[i] - centroide[i]);
            }
            double valor_expandido = calcular_chi2(datos, expandido, modelo_tipo);
            
            if (valor_expandido < valor_reflejado) {
                for (int i = 0; i < NUM_PARAMS; i++) {
                    simplex[NUM_PARAMS][i] = expandido[i];
                }
                valores[NUM_PARAMS] = valor_expandido;
            } else {
                for (int i = 0; i < NUM_PARAMS; i++) {
                    simplex[NUM_PARAMS][i] = reflejado[i];
                }
                valores[NUM_PARAMS] = valor_reflejado;
            }
            continue;
        }
        
        // Contracción
        for (int i = 0; i < NUM_PARAMS; i++) {
            contraido[i] = centroide[i] + rho * (simplex[NUM_PARAMS][i] - centroide[i]);
        }
        double valor_contraido = calcular_chi2(datos, contraido, modelo_tipo);
        
        if (valor_contraido < valores[NUM_PARAMS]) {
            for (int i = 0; i < NUM_PARAMS; i++) {
                simplex[NUM_PARAMS][i] = contraido[i];
            }
            valores[NUM_PARAMS] = valor_contraido;
            continue;
        }
        
        // Reducción
        for (int i = 1; i <= NUM_PARAMS; i++) {
            for (int j = 0; j < NUM_PARAMS; j++) {
                simplex[i][j] = simplex[0][j] + sigma * (simplex[i][j] - simplex[0][j]);
            }
            valores[i] = calcular_chi2(datos, simplex[i], modelo_tipo);
        }
        
        // Reporte de progreso
        if (iter % 10000 == 0) {
            printf("  Iteración %d: Chi² = %.6f\n", iter, valores[0]);
        }
    }
    
    // Guardar mejor resultado
    for (int i = 0; i < NUM_PARAMS; i++) {
        resultado->params[i] = simplex[0][i];
    }
    resultado->chi_cuadrado = valores[0];
    resultado->rmse = sqrt(valores[0] / datos->n_puntos);
    
    printf("Chi² final: %.6f\n", resultado->chi_cuadrado);
    printf("RMSE: %.6f\n\n", resultado->rmse);
}

// Leer datos desde CSV/Excel exportado
int leer_datos_csv(const char *archivo, DatosExperimentales *datos, 
                   int temp_filtro, int replicate_filtro) {
    FILE *fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("Error: No se pudo abrir el archivo %s\n", archivo);
        return 0;
    }
    
    char linea[MAX_LINE];
    datos->n_puntos = 0;
    
    // Saltar encabezado
    fgets(linea, sizeof(linea), fp);
    
    while (fgets(linea, sizeof(linea), fp) != NULL && datos->n_puntos < MAX_DATOS) {
        char type[100];
        int replicate, temperature;
        double water_activity, moisture_wet, moisture_dry;
        
        // Leer línea completa del CSV
        int items = sscanf(linea, "%[^,],%d,%d,%lf,%lf,%lf",
                          type, &replicate, &temperature,
                          &water_activity, &moisture_wet, &moisture_dry);
        
        // Filtrar: solo "Roasted beans", réplica específica, temperatura específica
        if (items == 6 && 
            strstr(type, "Roasted beans") != NULL &&
            replicate == replicate_filtro &&
            temperature == temp_filtro) {
            
            datos->aw[datos->n_puntos] = water_activity;
            datos->mc[datos->n_puntos] = moisture_wet;
            datos->n_puntos++;
        }
    }
    
    fclose(fp);
    printf("Temperatura %d°C: %d puntos leídos\n", temp_filtro, datos->n_puntos);
    return datos->n_puntos > 0;
}

// Combinar datos de múltiples temperaturas
void combinar_datos(DatosExperimentales *datos_total, 
                    DatosExperimentales datos[], int n_conjuntos) {
    datos_total->n_puntos = 0;
    
    for (int conjunto = 0; conjunto < n_conjuntos; conjunto++) {
        for (int i = 0; i < datos[conjunto].n_puntos; i++) {
            if (datos_total->n_puntos < MAX_DATOS) {
                datos_total->aw[datos_total->n_puntos] = datos[conjunto].aw[i];
                datos_total->mc[datos_total->n_puntos] = datos[conjunto].mc[i];
                datos_total->n_puntos++;
            }
        }
    }
}

// Guardar resultados
void guardar_resultados(const char *archivo, Parametros *peleg, Parametros *dlp, int n_datos) {
    FILE *fp = fopen(archivo, "w");
    if (fp == NULL) {
        printf("Error al crear archivo de resultados\n");
        return;
    }
    
    fprintf(fp, "╔════════════════════════════════════════════════════════════╗\n");
    fprintf(fp, "║   RESULTADOS DE OPTIMIZACIÓN - ISOTERMAS DE CAFÉ          ║\n");
    fprintf(fp, "║   Minimización de Chi-cuadrado (χ²)                       ║\n");
    fprintf(fp, "╚════════════════════════════════════════════════════════════╝\n\n");
    
    fprintf(fp, "═══════════════════════════════════════════════════════════\n");
    fprintf(fp, "MODELO DE PELEG: Xe = b0*aw^b1 + b2*aw^b3\n");
    fprintf(fp, "═══════════════════════════════════════════════════════════\n");
    fprintf(fp, "  Parámetros optimizados:\n");
    fprintf(fp, "    b0 = %12.8f\n", peleg->params[0]);
    fprintf(fp, "    b1 = %12.8f\n", peleg->params[1]);
    fprintf(fp, "    b2 = %12.8f\n", peleg->params[2]);
    fprintf(fp, "    b3 = %12.8f\n", peleg->params[3]);
    fprintf(fp, "\n  Métricas de ajuste:\n");
    fprintf(fp, "    Chi-cuadrado (χ²) = %12.8f\n", peleg->chi_cuadrado);
    fprintf(fp, "    RMSE              = %12.8f\n", peleg->rmse);
    fprintf(fp, "    R² ajustado       = %12.8f\n", 1.0 - (peleg->chi_cuadrado / n_datos));
    fprintf(fp, "═══════════════════════════════════════════════════════════\n\n");
    
    fprintf(fp, "═══════════════════════════════════════════════════════════\n");
    fprintf(fp, "MODELO DLP: Xe = b0 + b1*x + b2*x² + b3*x³\n");
    fprintf(fp, "           donde x = ln(-ln(aw))\n");
    fprintf(fp, "═══════════════════════════════════════════════════════════\n");
    fprintf(fp, "  Parámetros optimizados:\n");
    fprintf(fp, "    b0 = %12.8f\n", dlp->params[0]);
    fprintf(fp, "    b1 = %12.8f\n", dlp->params[1]);
    fprintf(fp, "    b2 = %12.8f\n", dlp->params[2]);
    fprintf(fp, "    b3 = %12.8f\n", dlp->params[3]);
    fprintf(fp, "\n  Métricas de ajuste:\n");
    fprintf(fp, "    Chi-cuadrado (χ²) = %12.8f\n", dlp->chi_cuadrado);
    fprintf(fp, "    RMSE              = %12.8f\n", dlp->rmse);
    fprintf(fp, "    R² ajustado       = %12.8f\n", 1.0 - (dlp->chi_cuadrado / n_datos));
    fprintf(fp, "═══════════════════════════════════════════════════════════\n\n");
    
    fprintf(fp, "╔════════════════════════════════════════════════════════════╗\n");
    fprintf(fp, "║                   ANÁLISIS COMPARATIVO                     ║\n");
    fprintf(fp, "╚════════════════════════════════════════════════════════════╝\n\n");
    
    if (peleg->chi_cuadrado < dlp->chi_cuadrado) {
        double mejora = ((dlp->chi_cuadrado - peleg->chi_cuadrado) / 
                         dlp->chi_cuadrado) * 100.0;
        fprintf(fp, "  ✓ MODELO SUPERIOR: PELEG\n");
        fprintf(fp, "  • Reducción de error respecto a DLP: %.2f%%\n", mejora);
        fprintf(fp, "  • Diferencia de χ²: %.6f\n", dlp->chi_cuadrado - peleg->chi_cuadrado);
    } else {
        double mejora = ((peleg->chi_cuadrado - dlp->chi_cuadrado) / 
                         peleg->chi_cuadrado) * 100.0;
        fprintf(fp, "  ✓ MODELO SUPERIOR: DLP\n");
        fprintf(fp, "  • Reducción de error respecto a Peleg: %.2f%%\n", mejora);
        fprintf(fp, "  • Diferencia de χ²: %.6f\n", peleg->chi_cuadrado - dlp->chi_cuadrado);
    }
    
    fprintf(fp, "\n═══════════════════════════════════════════════════════════\n");
    fprintf(fp, "Número total de datos: %d\n", n_datos);
    fprintf(fp, "═══════════════════════════════════════════════════════════\n");
    
    fclose(fp);
    printf("\n✓ Resultados guardados en: %s\n", archivo);
}

// Generar archivo de predicciones
void exportar_predicciones(DatosExperimentales *datos, Parametros *peleg, 
                           Parametros *dlp, const char *archivo) {
    FILE *fp = fopen(archivo, "w");
    if (fp == NULL) return;
    
    fprintf(fp, "aw,mc_observado,mc_peleg,residuo_peleg,mc_dlp,residuo_dlp\n");
    
    for (int i = 0; i < datos->n_puntos; i++) {
        double pred_peleg = modelo_peleg(datos->aw[i], peleg->params);
        double pred_dlp = modelo_dlp(datos->aw[i], dlp->params);
        double res_peleg = datos->mc[i] - pred_peleg;
        double res_dlp = datos->mc[i] - pred_dlp;
        
        fprintf(fp, "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n", 
                datos->aw[i], datos->mc[i], pred_peleg, res_peleg, pred_dlp, res_dlp);
    }
    
    fclose(fp);
    printf("✓ Predicciones exportadas: %s\n", archivo);
}

int main(int argc, char *argv[]) {
    DatosExperimentales datos_25c, datos_35c, datos_45c, datos_completos;
    Parametros resultado_peleg, resultado_dlp;
    
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║     OPTIMIZACIÓN DE ISOTERMAS DE ADSORCIÓN DE CAFÉ        ║\n");
    printf("║          Minimización de Chi-cuadrado en C                 ║\n");
    printf("║                                                        \n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    // Nombre del archivo de entrada
    const char *archivo_entrada = "Experimental water adsorption isotherms.csv";
    if (argc > 1) {
        archivo_entrada = argv[1];
    }
    
    printf("Leyendo datos desde: %s\n\n", archivo_entrada);
    
    // Leer datos filtrados por temperatura
    if (!leer_datos_csv(archivo_entrada, &datos_25c, 25, 1) ||
        !leer_datos_csv(archivo_entrada, &datos_35c, 35, 1) ||
        !leer_datos_csv(archivo_entrada, &datos_45c, 45, 1)) {
        printf("\n⚠ ERROR: No se pudieron leer los datos.\n");
        printf("Asegúrate de que el archivo Excel esté exportado como CSV con este nombre:\n");
        printf("  '%s'\n\n", archivo_entrada);
        printf("Puedes exportar desde Excel: Archivo → Guardar como → CSV UTF-8\n");
        return 1;
    }
    
    // Combinar todos los datos
    DatosExperimentales datos_array[3] = {datos_25c, datos_35c, datos_45c};
    combinar_datos(&datos_completos, datos_array, 3);
    printf("\n✓ Total de datos combinados: %d puntos\n\n", datos_completos.n_puntos);
    
    // Optimizar ambos modelos
    printf("════════════════════════════════════════════════════════════\n");
    optimizar_simplex(&datos_completos, &resultado_peleg, 1);
    
    printf("════════════════════════════════════════════════════════════\n");
    optimizar_simplex(&datos_completos, &resultado_dlp, 2);
    
    // Mostrar resultados en consola
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                   RESULTADOS FINALES                       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
    printf("MODELO PELEG:\n");
    printf("  b0 = %10.6f    b1 = %10.6f\n", resultado_peleg.params[0], resultado_peleg.params[1]);
    printf("  b2 = %10.6f    b3 = %10.6f\n", resultado_peleg.params[2], resultado_peleg.params[3]);
    printf("  Chi² = %.6f      RMSE = %.6f\n\n", resultado_peleg.chi_cuadrado, resultado_peleg.rmse);
    
    printf("MODELO DLP:\n");
    printf("  b0 = %10.6f    b1 = %10.6f\n", resultado_dlp.params[0], resultado_dlp.params[1]);
    printf("  b2 = %10.6f    b3 = %10.6f\n", resultado_dlp.params[2], resultado_dlp.params[3]);
    printf("  Chi² = %.6f      RMSE = %.6f\n\n", resultado_dlp.chi_cuadrado, resultado_dlp.rmse);
    
    printf("════════════════════════════════════════════════════════════\n");
    if (resultado_peleg.chi_cuadrado < resultado_dlp.chi_cuadrado) {
        double mejora = ((resultado_dlp.chi_cuadrado - resultado_peleg.chi_cuadrado) / 
                        resultado_dlp.chi_cuadrado) * 100.0;
        printf("✓ MEJOR MODELO: PELEG (%.2f%% mejor que DLP)\n", mejora);
    } else {
        double mejora = ((resultado_peleg.chi_cuadrado - resultado_dlp.chi_cuadrado) / 
                        resultado_peleg.chi_cuadrado) * 100.0;
        printf("✓ MEJOR MODELO: DLP (%.2f%% mejor que Peleg)\n", mejora);
    }
    printf("════════════════════════════════════════════════════════════\n\n");
    
    // Guardar resultados
    guardar_resultados("resultados_optimizacion_c.txt", &resultado_peleg, 
                      &resultado_dlp, datos_completos.n_puntos);
    exportar_predicciones(&datos_completos, &resultado_peleg, &resultado_dlp, 
                         "predicciones_completas.csv");
    

    return 0;
}