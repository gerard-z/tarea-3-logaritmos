// Experimento, parte general.

#include "experimento.h"
#include <random>
using namespace std;
using namespace std::chrono;

#define DEBUG true

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

double* probarAlgoritmo(Point P[], int n, int k, ClosestPoint& (*algoritmo)(Point[], int)){
    double* tiempos = (double*)malloc(k * sizeof(double));
    for (int i = 0; i < k; i++) {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        #if DEBUG
        ClosestPoint resultado = algoritmo(P, n);
        cout << "Resultado: " << resultado << endl;
        #else
        algoritmo(P, n);
        #endif
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        tiempos[i] = duration_cast<duration<double>>(t2 - t1).count();
    }
    return tiempos;
}

double*** experimento(int nMin, int nMax, int nStep, int k, ClosestPoint& (*algoritmo)(Point[], int), ClosestPoint& (*algoritmo2)(Point[], int)){
    int n = (nMax - nMin) / nStep + 1;
    double** tiempos = new double*[n];
    double** tiempos2 = new double*[n];
    for (int i = 0; i < n; i++) {
        tiempos[i] = new double[k];
        tiempos2[i] = new double[k];
    }
    for (int i = 0; i < n; i++) {
        int nActual = nMin + i * nStep;
        Point* puntos = generarPuntos(nActual);
        double* tiemposActual = probarAlgoritmo(puntos, nActual, k, algoritmo);
        double* tiemposActual2 = probarAlgoritmo(puntos, nActual, k, algoritmo2);
        for (int j = 0; j < k; j++) {
            tiempos[i][j] = tiemposActual[j];
            tiempos2[i][j] = tiemposActual2[j];
        }
        free(puntos);
        delete[] tiemposActual;
        delete[] tiemposActual2;
    }
    double*** tiemposTotales = new double**[2];
    tiemposTotales[0] = tiempos;
    tiemposTotales[1] = tiempos2;
    return tiemposTotales;
}

double** pruebaAlgoritmoRandom(int n, int k, ClosestPoint& (*algoritmo)(Point[], int), ClosestPoint& (*algoritmo2)(Point[], int)){
    double* tiempos = new double[k];
    double* tiempos2 = new double[k];
    for (int i = 0; i < k; i++) {
        Point* P = generarPuntos(n);
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        algoritmo(P, n);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        tiempos[i] = duration_cast<duration<double>>(t2 - t1).count();
        high_resolution_clock::time_point t3 = high_resolution_clock::now();
        algoritmo2(P, n);
        high_resolution_clock::time_point t4 = high_resolution_clock::now();
        tiempos2[i] = duration_cast<duration<double>>(t4 - t3).count();
    }
    double** tiemposTotales = new double*[2];
    tiemposTotales[0] = tiempos;
    tiemposTotales[1] = tiempos2;
    return tiemposTotales;
}

double*** experimentoRandom(int nMin, int nMax, int nStep, int k, ClosestPoint& (*algoritmo)(Point[], int), ClosestPoint& (*algoritmo2)(Point[], int)){
    int n = (nMax - nMin) / nStep + 1;
    double** tiempos = new double*[n];
    double** tiempos2 = new double*[n];
    for (int i = 0; i < n; i++) {
        tiempos[i] = new double[k];
        tiempos2[i] = new double[k];
    }
    for (int i = 0; i < n; i++) {
        int nActual = nMin + i * nStep;
        double** tiemposActual = pruebaAlgoritmoRandom(nActual, k, algoritmo, algoritmo2);
        for (int j = 0; j < k; j++) {
            tiempos[i][j] = tiemposActual[0][j];
            tiempos2[i][j] = tiemposActual[1][j];
        }
        delete[] tiemposActual[0];
        delete[] tiemposActual[1];
        delete[] tiemposActual;
    }
    double*** tiemposTotales = new double**[2];
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
    int n = 10;
    Point* P = generarPuntos(n);
    printArrayPoints(P, n);
    cout << "Brute force:" << "\n";
    probarAlgoritmo(P, n, 1, bruteForce);
    cout << "Divide and conquer:" << "\n";
    probarAlgoritmo(P, n, 1, closestDivide);
    return 0;
}