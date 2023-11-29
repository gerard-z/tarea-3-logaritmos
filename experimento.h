// Experimento, parte general.

#include <iostream>
#include <ctime>
#include <aleatorizado.h>

// Genera un set de n puntos pertenecientes al cuadrado [0,1)x[0,1)
Point* generarPuntos(int n);

// Se prueba el algoritmo en el arreglo de puntos P de tamaño n, una cantidad de k veces.
// Se devuelve un arreglo con los tiempos de ejecución de cada prueba.
// El algoritmo debe ser una función que reciba un arreglo de puntos y su tamaño, y devuelva un double (o el par de puntos).
double* probarAlgoritmo(Point P[], int n, int k, ClosestPoint& (*algoritmo)(Point[], int));

// Realiza el experimento de probar 2 algoritmos k veces para cada n en el rango [nMin, nMax] con saltos de nStep.
// Devuelve una tupla de 2 arreglos de tamaño n, donde cada arreglo es un arreglo de tiempos de tamaño k.
double*** experimento(int nMin, int nMax, int nStep, int k, ClosestPoint& (*algoritmo)(Point[], int), ClosestPoint& (*algoritmo2)(Point[], int));

// Realiza otro experimento, midiendo el tiempo de cada algoritmo para un input aleatorio de tamaño n, k veces, 1 por cada arreglo aleatorio.
// Devuelve un arreglo de tiempos de tamaño k.
double** pruebaAlgoritmoRandom(int n, int k, ClosestPoint& (*algoritmo)(Point[], int), ClosestPoint& (*algoritmo2)(Point[], int));

// Realiza el mismo experimento anterior, pero por arreglos de tamaño n en el rango [nMin, nMax] con saltos de nStep.
// Devuelve una tupla de 2 arreglos de tamaño n, donde cada arreglo es un arreglo de tiempos de tamaño k.
double*** experimentoRandom(int nMin, int nMax, int nStep, int k, ClosestPoint& (*algoritmo)(Point[], int), ClosestPoint& (*algoritmo2)(Point[], int));