#include "stdlib.h"
#include "hashing_universal.h"
#include "queue"
#include "unordered_map"
using namespace std;

// Busca la distancia minima entre los puntos de la celda gij1 y los de la celda gij2
void findMinDistance(const vector<vector<vector<Point*>>> &grid, ClosestPoint *d, Point *p, Point *pt2, const vector<Point*> &gij1, const vector<Point*> &gij2, ull &comparaciones);

// Busca la distancia mínima entre los puntos en la grilla, comparando cada celda con sus vecinos derecha, abajo-izquierda, abajo y abajo-derecha.
void checkGrid4neighbors(int cell_number, const vector<vector<vector<Point*>>> &grid, ClosestPoint *d, Point *p, Point *pt2, ull &comparaciones);

// Busca la distancia mínima entre los puntos en la grilla, comparando cada celda con sus 8 vecinos.
void checkGrid8neighbors(int cell_number, const vector<vector<vector<Point*>>> &grid, ClosestPoint *d, Point *p, Point *pt2, ull &comparaciones);

// Busca la distancia mínima entre 2 puntos en el plano, usando el algoritmo aleatorizado.
ClosestPoint* closestRandom(Point P[], int n, ull &comparaciones);