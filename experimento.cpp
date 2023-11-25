// Experimento, parte general.

#include "experimento.h"
#include <random>
using namespace std;
using namespace std::chrono;

#define PRINT true
#define SAVE false

Point* generarPuntos(int n) {
    Point* puntos = new Point[n];
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<float> distribution(0.0,1.0);
    for (int i = 0; i < n; i++) {
        puntos[i].x = distribution(generator);
        puntos[i].y = distribution(generator);
    }
    return puntos;
}
#if SAVE
ClosestPoint** probarAlgoritmo(FILE *out, Point P[], int n, int k, ClosestPoint* (*algoritmo)(Point[], int, ull&)){
    ClosestPoint** tiempos = new ClosestPoint[k];
#else
void probarAlgoritmo(FILE *out, Point P[], int n, int k, ClosestPoint* (*algoritmo)(Point[], int, ull&)){
#endif
    ull comparaciones, comparaciones_promedio = 0;
    double tiempo_promedio = 0;
    for (int i = 0; i < k; i++) {
        comparaciones = 0;
        Point *P_copy = new Point[n];
        memcpy(P_copy, P, n * sizeof(Point));
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        ClosestPoint *resultado = algoritmo(P_copy, n, comparaciones);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        double tiempo = duration_cast<duration<double>>(t2 - t1).count();
        resultado->tiempo = tiempo;
        #if SAVE
        tiempos[i] = resultado;
        #endif
        comparaciones_promedio += comparaciones;
        tiempo_promedio += tiempo;
        delete[] P_copy;
        #if PRINT
        fprintf(out, *resultado);
        #endif
        #if !SAVE
        delete resultado;
        #endif
    }
    #if PRINT
    comparaciones_promedio /= k;
    tiempo_promedio /= k;
    fprintf(out, "Comparaciones promedio: %lld\n", comparaciones_promedio);
    fprintf(out, "Tiempo promedio: %f\n", tiempo_promedio);
    #endif
    #if SAVE
    return tiempos;
    #endif
}
#if SAVE
ClosestPoint**** experimento(int nMin, int nMax, int nStep, int k, ClosestPoint* (*algoritmo)(Point[], int, ull&), ClosestPoint* (*algoritmo2)(Point[], int, ull&)){
    int n = (nMax - nMin) / nStep + 1;
    ClosestPoint*** tiempos = new ClosestPoint**[n];
    ClosestPoint*** tiempos2 = new ClosestPoint**[n];
    FILE *out = fopen("resultados_experimento.txt", "w+");
    for (int i = 0; i < n; i++) {
        tiempos[i] = new ClosestPoint*[k];
        tiempos2[i] = new ClosestPoint*[k];
    }
    for (int i = 0; i < n; i++) {
        int nActual = nMin + i * nStep;
        Point* puntos = generarPuntos(nActual);
        #if PRINT
        fprintf(out, "n = %d\n", nActual);
        fprintf(out, "Algoritmo 1:\n");
        #endif
        ClosestPoint** tiemposActual = probarAlgoritmo(out, puntos, nActual, k, algoritmo);
        #if PRINT
        fprintf(out, "Algoritmo 2:\n");
        #endif
        ClosestPoint** tiemposActual2 = probarAlgoritmo(out, puntos, nActual, k, algoritmo2);
        #if PRINT
        fprintf(out, "\n###################################\n");
        #endif
        delete[] puntos;
        for (int j = 0; j < k; j++) {
            tiempos[i][j] = tiemposActual[j];
            tiempos2[i][j] = tiemposActual2[j];
        }
        delete[] tiemposActual;
        delete[] tiemposActual2;
    }
    fclose(out);
    ClosestPoint**** tiemposTotales = new ClosestPoint***[2];
    tiemposTotales[0] = tiempos;
    tiemposTotales[1] = tiempos2;
    return tiemposTotales;
}
#else
void experimento(int nMin, int nMax, int nStep, int k, ClosestPoint* (*algoritmo)(Point[], int, ull&), ClosestPoint* (*algoritmo2)(Point[], int, ull&)){
    int n = (nMax - nMin) / nStep + 1;
    FILE *out = fopen("resultados_experimento.txt", "w+");
    for (int i = 0; i < n; i++) {
        int nActual = nMin + i * nStep;
        Point* puntos = generarPuntos(nActual);
        #if PRINT
        fprintf(out, "n = %d\n", nActual);
        fprintf(out, "Algoritmo 1:\n");
        #endif
        probarAlgoritmo(out, puntos, nActual, k, algoritmo);
        #if PRINT
        fprintf(out, "Algoritmo 2:\n");
        #endif
        probarAlgoritmo(out, puntos, nActual, k, algoritmo2);
        #if PRINT
        fprintf(out, "\n###################################\n");
        #endif
        delete[] puntos;
    }
    fclose(out);
}
#endif

#if SAVE
ClosestPoint*** pruebaAlgoritmoRandom(FILE *out, int n, int k, ClosestPoint* (*algoritmo)(Point[], int, ull&), ClosestPoint* (*algoritmo2)(Point[], int, ull&)){
    ClosestPoint** tiempos = new ClosestPoint*[k];
    ClosestPoint** tiempos2 = new ClosestPoint*[k];
#else
void pruebaAlgoritmoRandom(FILE *out, int n, int k, ClosestPoint* (*algoritmo)(Point[], int, ull&), ClosestPoint* (*algoritmo2)(Point[], int, ull&)){
#endif
    ull comparaciones, comparaciones2;
    ull comparaciones_promedio = 0, comparaciones_promedio2 = 0;
    double tiempo_promedio = 0, tiempo_promedio2 = 0;
    for (int i = 0; i < k; i++) {
        Point* P = generarPuntos(n);
        Point *P_copy = new Point[n];
        memcpy(P_copy, P, n * sizeof(Point));
        comparaciones = 0;
        comparaciones2 = 0;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        ClosestPoint* resultado1 = algoritmo(P, n, comparaciones);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        resultado1->tiempo = duration_cast<duration<double>>(t2 - t1).count();
        #if SAVE
        tiempos[i] = resultado1;
        #endif
        high_resolution_clock::time_point t3 = high_resolution_clock::now();
        ClosestPoint* resultado2 = algoritmo2(P_copy, n, comparaciones2);
        high_resolution_clock::time_point t4 = high_resolution_clock::now();
        resultado2->tiempo = duration_cast<duration<double>>(t4 - t3).count();
        #if SAVE
        tiempos2[i] = resultado2;
        #endif
        delete[] P;
        delete[] P_copy;
        #if PRINT
        comparaciones_promedio += comparaciones;
        comparaciones_promedio2 += comparaciones2;
        tiempo_promedio += resultado1->tiempo;
        tiempo_promedio2 += resultado2->tiempo;
        fprintf(out, "Resultado %d:\n", i+1);
        fprintf(out, *resultado1);
        fprintf(out, *resultado2);
        #endif
        #if !SAVE
        delete resultado1;
        delete resultado2;
        #endif
    }
    #if PRINT
    comparaciones_promedio /= k;
    comparaciones_promedio2 /= k;
    tiempo_promedio /= k;
    tiempo_promedio2 /= k;
    fprintf(out, "Comparaciones promedio: %lld\n", comparaciones_promedio);
    fprintf(out, "Tiempo promedio: %f\n", tiempo_promedio);
    fprintf(out, "Comparaciones promedio: %lld\n", comparaciones_promedio2);
    fprintf(out, "Tiempo promedio: %f\n", tiempo_promedio2);
    #endif
    #if SAVE
    ClosestPoint*** tiemposTotales = new ClosestPoint**[2];
    tiemposTotales[0] = tiempos;
    tiemposTotales[1] = tiempos2;
    return tiemposTotales;
    #endif
}

#if SAVE
ClosestPoint**** experimentoRandom(int nMin, int nMax, int nStep, int k, ClosestPoint* (*algoritmo)(Point[], int, ull&), ClosestPoint* (*algoritmo2)(Point[], int, ull&)){
    int n = (nMax - nMin) / nStep + 1;
    ClosestPoint*** tiempos = new ClosestPoint**[n];
    ClosestPoint*** tiempos2 = new ClosestPoint**[n];
    FILE *out = fopen("resultados_experimento_random.txt", "w+");
    for (int i = 0; i < n; i++) {
        tiempos[i] = new ClosestPoint*[k];
        tiempos2[i] = new ClosestPoint*[k];
    }
    for (int i = 0; i < n; i++) {
        int nActual = nMin + i * nStep;
        #if PRINT
        fprintf(out, "n = %d\n", nActual);
        fprintf(out, "Algoritmo 1 y 2:\n");
        #endif
        ClosestPoint*** tiemposActual = pruebaAlgoritmoRandom(out, nActual, k, algoritmo, algoritmo2);
        #if PRINT
        fprintf(out, "\n###################################\n");
        #endif
        for (int j = 0; j < k; j++) {
            tiempos[i][j] = tiemposActual[0][j];
            tiempos2[i][j] = tiemposActual[1][j];
        }
        delete[] tiemposActual[0];
        delete[] tiemposActual[1];
        delete[] tiemposActual;
    }
    fclose(out);
    ClosestPoint**** tiemposTotales = new ClosestPoint***[2];
    tiemposTotales[0] = tiempos;
    tiemposTotales[1] = tiempos2;
    return tiemposTotales;
}
#else
void experimentoRandom(int nMin, int nMax, int nStep, int k, ClosestPoint* (*algoritmo)(Point[], int, ull&), ClosestPoint* (*algoritmo2)(Point[], int, ull&)){
    int n = (nMax - nMin) / nStep + 1;
    FILE *out = fopen("resultados_experimento_random.txt", "w+");
    for (int i = 0; i < n; i++) {
        int nActual = nMin + i * nStep;
        #if PRINT
        fprintf(out, "n = %d\n", nActual);
        fprintf(out, "Algoritmo 1 y 2:\n");
        #endif
        pruebaAlgoritmoRandom(out, nActual, k, algoritmo, algoritmo2);
        #if PRINT
        fprintf(out, "\n###################################\n");
        #endif
    }
    fclose(out);
}
#endif

void printArrayPoints(Point P[], int n){
    cout << "Puntos:" << endl;
    for (int i = 0; i < n; i++) {
        cout << P[i] << " ";
    }
    cout << endl;
}

int main(){
    int nMin = 5'000'000, nMax= 50'000'000, nStep = 5'000'000, k = 100;
    // int nMin = 500, nMax= 5000, nStep = 500, k = 10;
    // int n = (nMax - nMin) / nStep + 1;
    #if SAVE
    ClosestPoint ****resultados = experimento(nMin, nMax, nStep, k, bruteForce, closestDivide);
    delete[] resultados;
    ClosestPoint ****resultados2 = experimentoRandom(nMin, nMax, nStep, k, bruteForce, closestDivide);
    delete[] resultados2;
    #else
    experimento(nMin, nMax, nStep, k, closestDivide, closestDivide);
    experimentoRandom(nMin, nMax, nStep, k, closestDivide, closestDivide);
    #endif
    return 0;
}