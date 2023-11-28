#include "stdlib.h"
#include "hashing_universal.h"
using namespace std;

int indiceAleatorio(int n);

void findMinDistance(const vector<vector<vector<Point>>> &grid, ClosestPoint *d, Point *p, Point *pt2, const vector<Point> &gij1, const vector<Point> &gij2, ull &comparaciones);

ClosestPoint* closestRandom(Point P[], int n, ull &comparaciones);