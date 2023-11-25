// Experimento, parte general.

#include "experimento.h"
#include <random>
using namespace std;
using namespace std::chrono;

#define DEBUG false
#define PRINT true

Point* generarPuntos(int n) {
    Point* puntos = (Point*)malloc(n * sizeof(Point));
    random_device rd;
    mt19937 generator(rd());
    uniform_real_distribution<float> distribution(0.0,1.0);
    for (int i = 0; i < n; i++) {
        puntos[i].x = distribution(generator);
        puntos[i].y = distribution(generator);
    }
    return puntos;
}

ClosestPoint* probarAlgoritmo(FILE *out, Point P[], int n, int k, ClosestPoint& (*algoritmo)(Point[], int, ull&)){
    ClosestPoint* tiempos = (ClosestPoint*)malloc(k * sizeof(ClosestPoint));
    ull comparaciones, comparaciones_promedio = 0;
    double tiempo_promedio = 0;
    for (int i = 0; i < k; i++) {
        comparaciones = 0;
        Point *P_copy = (Point*)malloc(n * sizeof(Point));
        copy(P, P + n, P_copy);
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        ClosestPoint resultado = algoritmo(P_copy, n, comparaciones);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        double tiempo = duration_cast<duration<double>>(t2 - t1).count();
        resultado.tiempo = tiempo;
        tiempos[i] = resultado;
        comparaciones_promedio += comparaciones;
        tiempo_promedio += tiempo;
        free(P_copy);
        #if DEBUG
        cout << "Resultado "<< i+1 << " : " << resultado << endl;
        #endif
        #if PRINT
        fprintf(out, resultado);
        #endif
    }
    #if PRINT
    comparaciones_promedio /= k;
    tiempo_promedio /= k;
    fprintf(out, "Comparaciones promedio: %lld\n", comparaciones_promedio);
    fprintf(out, "Tiempo promedio: %f\n", tiempo_promedio);
    #endif
    return tiempos;
}

ClosestPoint*** experimento(int nMin, int nMax, int nStep, int k, ClosestPoint& (*algoritmo)(Point[], int, ull&), ClosestPoint& (*algoritmo2)(Point[], int, ull&)){
    int n = (nMax - nMin) / nStep + 1;
    ClosestPoint** tiempos = new ClosestPoint*[n];
    ClosestPoint** tiempos2 = new ClosestPoint*[n];
    FILE *out = fopen("resultados_experimento.txt", "w+");
    for (int i = 0; i < n; i++) {
        tiempos[i] = new ClosestPoint[k];
        tiempos2[i] = new ClosestPoint[k];
    }
    for (int i = 0; i < n; i++) {
        int nActual = nMin + i * nStep;
        Point* puntos = generarPuntos(nActual);
        #if PRINT
        fprintf(out, "n = %d\n", nActual);
        fprintf(out, "Algoritmo 1:\n");
        #endif
        ClosestPoint* tiemposActual = probarAlgoritmo(out, puntos, nActual, k, algoritmo);
        #if PRINT
        fprintf(out, "Algoritmo 2:\n");
        #endif
        ClosestPoint* tiemposActual2 = probarAlgoritmo(out, puntos, nActual, k, algoritmo2);
        #if PRINT
        fprintf(out, "\n###################################\n");
        #endif
        for (int j = 0; j < k; j++) {
            tiempos[i][j] = tiemposActual[j];
            tiempos2[i][j] = tiemposActual2[j];
        }
        free(puntos);
        free(tiemposActual);
        free(tiemposActual2);
    }
    ClosestPoint*** tiemposTotales = new ClosestPoint**[2];
    tiemposTotales[0] = tiempos;
    tiemposTotales[1] = tiempos2;
    return tiemposTotales;
}

ClosestPoint** pruebaAlgoritmoRandom(FILE *out, int n, int k, ClosestPoint& (*algoritmo)(Point[], int, ull&), ClosestPoint& (*algoritmo2)(Point[], int, ull&)){
    ClosestPoint* tiempos = new ClosestPoint[k];
    ClosestPoint* tiempos2 = new ClosestPoint[k];
    ull comparaciones, comparaciones2;
    ull comparaciones_promedio = 0, comparaciones_promedio2 = 0;
    double tiempo_promedio = 0, tiempo_promedio2 = 0;
    for (int i = 0; i < k; i++) {
        Point* P = generarPuntos(n);
        Point *P_copy = (Point*)malloc(n * sizeof(Point));
        copy(P, P + n, P_copy);
        comparaciones = 0;
        comparaciones2 = 0;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        ClosestPoint resultado1 = algoritmo(P, n, comparaciones);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        resultado1.tiempo = duration_cast<duration<double>>(t2 - t1).count();
        tiempos[i] = resultado1;
        high_resolution_clock::time_point t3 = high_resolution_clock::now();
        ClosestPoint resultado2 = algoritmo2(P_copy, n, comparaciones2);
        high_resolution_clock::time_point t4 = high_resolution_clock::now();
        resultado2.tiempo = duration_cast<duration<double>>(t4 - t3).count();
        tiempos2[i] = resultado2;
        free(P);
        free(P_copy);
        #if PRINT
        comparaciones_promedio += comparaciones;
        comparaciones_promedio2 += comparaciones2;
        tiempo_promedio += resultado1.tiempo;
        tiempo_promedio2 += resultado2.tiempo;
        fprintf(out, "Resultado %d:\n", i+1);
        fprintf(out, resultado1);
        fprintf(out, resultado2);
        #endif
    }
    ClosestPoint** tiemposTotales = new ClosestPoint*[2];
    tiemposTotales[0] = tiempos;
    tiemposTotales[1] = tiempos2;
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
    return tiemposTotales;
}

ClosestPoint*** experimentoRandom(int nMin, int nMax, int nStep, int k, ClosestPoint& (*algoritmo)(Point[], int, ull&), ClosestPoint& (*algoritmo2)(Point[], int, ull&)){
    int n = (nMax - nMin) / nStep + 1;
    ClosestPoint** tiempos = new ClosestPoint*[n];
    ClosestPoint** tiempos2 = new ClosestPoint*[n];
    FILE *out = fopen("resultados_experimento_random.txt", "w+");
    for (int i = 0; i < n; i++) {
        tiempos[i] = new ClosestPoint[k];
        tiempos2[i] = new ClosestPoint[k];
    }
    for (int i = 0; i < n; i++) {
        int nActual = nMin + i * nStep;
        #if PRINT
        fprintf(out, "n = %d\n", nActual);
        fprintf(out, "Algoritmo 1 y 2:\n");
        #endif
        ClosestPoint** tiemposActual = pruebaAlgoritmoRandom(out, nActual, k, algoritmo, algoritmo2);
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
    ClosestPoint*** tiemposTotales = new ClosestPoint**[2];
    tiemposTotales[0] = tiempos;
    tiemposTotales[1] = tiempos2;
    return tiemposTotales;
}

void printArrayPoints(Point P[], int n){
    cout << "Puntos:" << endl;
    for (int i = 0; i < n; i++) {
        cout << P[i] << " ";
    }
    cout << endl;
}

int main(){
    // FILE *out = fopen("resultados.txt", "w+");
    int nMin = 50'000, nMax= 500'000, nStep = 50'000, k = 10;
    // int n = (nMax - nMin) / nStep + 1;
    ClosestPoint ***resultados = experimento(nMin, nMax, nStep, k, closestDivide, closestDivide);
    free(resultados);
    ClosestPoint ***resultados2 = experimentoRandom(nMin, nMax, nStep, k, closestDivide, closestDivide);
    free(resultados2);


    // ClosestPoint **resultadosAlgoritmo1 = resultados[0];
    // ClosestPoint **resultadosAlgoritmo2 = resultados[1];
    // int comp_promedio;
    // double tiempo_promedio;
    // ClosestPoint resultado;
    // for (int i = 0; i < n; i++) {
    //     comp_promedio = 0;
    //     tiempo_promedio = 0;
    //     int n_i = nMin + i*nStep;
    //     cout << "n = " << n_i << endl;
    //     Point* P = generarPuntos(n_i);
    //     // printArrayPoints(P, n_i);
    //     // cout << "Brute force ind:" << *probarAlgoritmo(P, n_i, 1, bruteForce) << "\n";
    //     // cout << "Divide and conquer ind:" << *probarAlgoritmo(P, n_i, 1, closestDivide) << "\n";
    //     // cout << "Brute force:" << endl;
    //     fprintf(out, "Brute force:\n");
    //     for (int j = 0; j < k; j++) {
    //         resultado = resultadosAlgoritmo1[i][j];
    //         // cout << resultado << endl;
    //         fprintf(out, resultado);
    //         comp_promedio += resultado.comparaciones;
    //         tiempo_promedio += resultado.tiempo;
    //     }
    //     // cout << "Comparaciones promedio: " << comp_promedio / k << endl;
    //     // cout << "Tiempo promedio: " << tiempo_promedio / k << endl;
    //     // cout << endl;
    //     fprintf(out, "Comparaciones promedio: %d\n", comp_promedio / k);
    //     fprintf(out, "Tiempo promedio: %f\n", tiempo_promedio / k);
    //     fprintf(out, "\n");
    //     comp_promedio = 0;
    //     tiempo_promedio = 0;
    //     // cout << "Divide and conquer:" << endl;
    //     fprintf(out, "Divide and conquer:\n");
    //     for (int j = 0; j < k; j++) {
    //         resultado = resultadosAlgoritmo2[i][j];
    //         // cout << resultado << endl;
    //         fprintf(out, resultado);
    //         comp_promedio += resultado.comparaciones;
    //         tiempo_promedio += resultado.tiempo;
    //     }
    //     // cout << "Comparaciones promedio: " << comp_promedio / k << endl;
    //     // cout << "Tiempo promedio: " << tiempo_promedio / k << endl;
    //     // cout << endl;
    //     fprintf(out, "Comparaciones promedio: %d\n", comp_promedio / k);
    //     fprintf(out, "Tiempo promedio: %f\n", tiempo_promedio / k);
    //     fprintf(out, "\n");
    //     free(P);
    //     // printf("############################################\n");
    //     fprintf(out, "############################################\n");
    // }
    return 0;
}