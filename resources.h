#include "cmath"
#include "cfloat"
#include "limits.h"
#include <iostream>
#include <random>
#include <cstdlib>
#include <chrono>
using namespace std::chrono;




typedef unsigned long long ull;
#define SavePoints true // Para guardar los puntos más cercanos
#define DEBUG true // Para imprimir los tiempos de ejecución

// A structure to represent a Point in 2D plane 
// class Point 
// { 
// 	public:
// 	float x, y; 

//     friend std::ostream& operator<<(std::ostream& os, const Point& p);
// };
// A structure to represent a Point in 2D plane
typedef struct Point{
    float x, y;
} Point;

std::ostream& operator<<(std::ostream& os, const Point& p);



#if SavePoints
// class ClosestPoint
// {
//     public:
//     Point *p1, *p2;
//     float distance;
//     ull comparaciones;
//     double tiempo;

//     friend std::ostream& operator<<(std::ostream& os, const ClosestPoint& p);
// };
typedef struct ClosestPoint{
    Point *p1, *p2;
    float distance;
    ull comparaciones;
    double tiempo;
} ClosestPoint;

std::ostream& operator<<(std::ostream& os, const ClosestPoint& p);
#else
// class ClosestPoint
// {
//     public:
//     float distance;
//     ull comparaciones;
//     double tiempo;

//     friend std::ostream& operator<<(std::ostream& os, const ClosestPoint& p);
// };
typedef struct ClosestPoint{
    float distance;
    ull comparaciones;
    double tiempo;
} ClosestPoint;

std::ostream& operator<<(std::ostream& os, const ClosestPoint& p);
#endif

void fprintf(FILE *out, const ClosestPoint& p);

// Needed to sort array of points 
// according to X coordinate 
int compareX(const void* a, const void* b);

// Needed to sort array of points according to Y coordinate 
int compareY(const void* a, const void* b);

// A utility function to find the 
// distance between two points 
float dist(Point p1, Point p2);

float distSquared(Point &p1, Point &p2);

// A Brute Force method to return the 
// smallest distance between two points 
// in P[] of size n 
ClosestPoint* bruteForce(Point P[], int n, ull &comparaciones);

// A utility function to find 
// minimum of two float values 
ClosestPoint* min(ClosestPoint* x, ClosestPoint* y);