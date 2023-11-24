// Experimento, parte general.

#include "experimento.h"
#include <random>
using namespace std;
using namespace std::chrono;

#define DEBUG false

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

ClosestPoint* probarAlgoritmo(Point P[], int n, int k, ClosestPoint& (*algoritmo)(Point[], int, int&)){
    ClosestPoint* tiempos = (ClosestPoint*)malloc(k * sizeof(ClosestPoint));
    int comparaciones;
    for (int i = 0; i < k; i++) {
        comparaciones = 0;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        ClosestPoint resultado = algoritmo(P, n, comparaciones);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        double tiempo = duration_cast<duration<double>>(t2 - t1).count();
        resultado.tiempo = tiempo;
        tiempos[i] = resultado;
        #if DEBUG
        cout << "Resultado "<< i+1 << " : " << resultado << endl;
        #endif
    }
    return tiempos;
}

ClosestPoint*** experimento(int nMin, int nMax, int nStep, int k, ClosestPoint& (*algoritmo)(Point[], int, int&), ClosestPoint& (*algoritmo2)(Point[], int, int&)){
    int n = (nMax - nMin) / nStep + 1;
    ClosestPoint** tiempos = new ClosestPoint*[n];
    ClosestPoint** tiempos2 = new ClosestPoint*[n];
    for (int i = 0; i < n; i++) {
        tiempos[i] = new ClosestPoint[k];
        tiempos2[i] = new ClosestPoint[k];
    }
    for (int i = 0; i < n; i++) {
        int nActual = nMin + i * nStep;
        Point* puntos = generarPuntos(nActual);
        ClosestPoint* tiemposActual = probarAlgoritmo(puntos, nActual, k, algoritmo);
        ClosestPoint* tiemposActual2 = probarAlgoritmo(puntos, nActual, k, algoritmo2);
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

ClosestPoint** pruebaAlgoritmoRandom(int n, int k, ClosestPoint& (*algoritmo)(Point[], int, int&), ClosestPoint& (*algoritmo2)(Point[], int, int&)){
    ClosestPoint* tiempos = new ClosestPoint[k];
    ClosestPoint* tiempos2 = new ClosestPoint[k];
    int comparaciones, comparaciones2;
    for (int i = 0; i < k; i++) {
        Point* P = generarPuntos(n);
        comparaciones = 0;
        comparaciones2 = 0;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        ClosestPoint resultado1 = algoritmo(P, n, comparaciones);
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        resultado1.tiempo = duration_cast<duration<double>>(t2 - t1).count();
        tiempos[i] = resultado1;
        high_resolution_clock::time_point t3 = high_resolution_clock::now();
        ClosestPoint resultado2 = algoritmo2(P, n, comparaciones2);
        high_resolution_clock::time_point t4 = high_resolution_clock::now();
        resultado2.tiempo = duration_cast<duration<double>>(t4 - t3).count();
        tiempos2[i] = resultado2;
    }
    ClosestPoint** tiemposTotales = new ClosestPoint*[2];
    tiemposTotales[0] = tiempos;
    tiemposTotales[1] = tiempos2;
    return tiemposTotales;
}

ClosestPoint*** experimentoRandom(int nMin, int nMax, int nStep, int k, ClosestPoint& (*algoritmo)(Point[], int, int&), ClosestPoint& (*algoritmo2)(Point[], int, int&)){
    int n = (nMax - nMin) / nStep + 1;
    ClosestPoint** tiempos = new ClosestPoint*[n];
    ClosestPoint** tiempos2 = new ClosestPoint*[n];
    for (int i = 0; i < n; i++) {
        tiempos[i] = new ClosestPoint[k];
        tiempos2[i] = new ClosestPoint[k];
    }
    for (int i = 0; i < n; i++) {
        int nActual = nMin + i * nStep;
        ClosestPoint** tiemposActual = pruebaAlgoritmoRandom(nActual, k, algoritmo, algoritmo2);
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
    // int n = 1'000;
    // Point* P = generarPuntos(n);
    // printArrayPoints(P, n);
    // cout << "Brute force:" << "\n";
    // probarAlgoritmo(P, n, 10, bruteForce);
    // cout << "Divide and conquer:" << "\n";
    // probarAlgoritmo(P, n, 10, closestDivide);
    ClosestPoint ***resultados = experimento(5'000, 50'000, 5'000, 10, bruteForce, closestDivide);
    ClosestPoint **resultadosBruteForce = resultados[0];
    ClosestPoint **resultadosDivideAndConquer = resultados[1];
    for (int i = 0; i < 10; i++) {
        cout << "n = " << 5 + i * 5 << endl;
        cout << "Brute force:" << endl;
        for (int j = 0; j < 10; j++) {
            cout << resultadosBruteForce[i][j] << endl;
        }
        cout << "Divide and conquer:" << endl;
        for (int j = 0; j < 10; j++) {
            cout << resultadosDivideAndConquer[i][j] << endl;
        }
    }
    return 0;
}