#include "resources.h"

using namespace std;

/* Following two functions are needed for library function qsort(). 
Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */

ostream& operator<<(ostream& os, const Point& p){
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

#if SavePoints
ostream& operator<<(ostream& os, const ClosestPoint& p){
    os << "P1: " << *(p.p1) << " P2: " << *(p.p2) << " Distance: " << sqrt(p.distance) << " Comparaciones: " << p.comparaciones << " Tiempo: " << p.tiempo;
    return os;
}

void fprintf(FILE *out, const ClosestPoint& p){
    // (x1, y1), (x2, y2), distance, comparaciones, tiempo
    fprintf(out, "%1.7f,%1.7f,%1.7f,%1.7f,%1.7f,%lld,%f\n", p.p1->x, p.p1->y, p.p2->x, p.p2->y, sqrt(p.distance), p.comparaciones, p.tiempo);
}
#else
ostream& operator<<(ostream& os, const ClosestPoint& p){
    os << "Distance: " << sqrt(p.distance) << " Comparaciones: " << p.comparaciones << " Tiempo: " << p.tiempo;
    return os;
}

void fprintf(FILE *out, const ClosestPoint& p){
    // (x1, y1), (x2, y2), distance, comparaciones, tiempo
    fprintf(out, "%1.7f,%lld,%f\n", sqrt(p.distance), p.comparaciones, p.tiempo);
}
#endif

// Needed to sort array of points 
// according to X coordinate 
int compareX(const void* a, const void* b) 
{ 
	Point *p1 = (Point *)a, *p2 = (Point *)b; 
	if (p1->x > p2->x) 
        return 1;
    else if (p1->x < p2->x)
        return -1;
    else
        return 0;
} 

// Needed to sort array of points according to Y coordinate 
int compareY(const void* a, const void* b) 
{ 
	Point *p1 = (Point *)a, *p2 = (Point *)b; 
	if (p1->y > p2->y) 
        return 1;
    else if (p1->y < p2->y)
        return -1;
    else
        return 0;
} 

// A utility function to find the 
// distance between two points 
float dist(Point p1, Point p2) 
{ 
	return sqrt( (p1.x - p2.x)*(p1.x - p2.x) + 
				(p1.y - p2.y)*(p1.y - p2.y) 
			); 
}

float distSquared(Point &p1, Point &p2) 
{ 
    return ( (p1.x - p2.x)*(p1.x - p2.x) + 
                (p1.y - p2.y)*(p1.y - p2.y) 
            ); // al no hacer la raiz cuadrada, se ahorra tiempo, pero se pierde precision en la distancia pues se compara con el cuadrado de la distancia,
            // y al ser una comparacion de floats en números entre 0 y 1 (no calcular la raíz vuelve los decimales muy pequeños), se pierde precision.
}

// A Brute Force method to return the 
// smallest distance between two points 
// in P[] of size n 
ClosestPoint* bruteForce(Point P[], int n, ull &comparaciones) 
{ 
	float min = numeric_limits<float>::max();
    float distance;
    #if SavePoints
    // Point *p = new Point();
    // Point *pt2 = new Point();
    Point* p = (Point*)malloc(sizeof(Point));
    Point* pt2 = (Point*)malloc(sizeof(Point));
	for (int i = 0; i < n-1; ++i) 
		for (int j = i+1; j < n; ++j){
            comparaciones += 1;
            distance = distSquared(P[i], P[j]);
			if (distance < min){
				min = distance;
                *p = P[i];
                *pt2 = P[j];
            }
        }
    // ClosestPoint *c = new ClosestPoint();
    ClosestPoint *c = (ClosestPoint*)malloc(sizeof(ClosestPoint));
    
    c->p1 = p;
    c->p2 = pt2;

    c->distance = min;
    c->comparaciones = comparaciones;
	return c;
    #else
    for (int i = 0; i < n; ++i) 
		for (int j = i+1; j < n; ++j){
            comparaciones += 1;
			if (distSquared(P[i], P[j]) < min){
				min = distSquared(P[i], P[j]);
            }
        }
    // ClosestPoint *c = new ClosestPoint();
    ClosestPoint *c = (ClosestPoint*)malloc(sizeof(ClosestPoint));
    c->distance = min;
    c->comparaciones = comparaciones;
    return c;
    #endif
} 

// A utility function to find 
// minimum distance between the closest points 
ClosestPoint* min(ClosestPoint* x, ClosestPoint* y) 
{ 
    #if SavePoints
    if (x->distance < y->distance){
        free(y->p1);
        free(y->p2);
        free(y);
        return x; 
    }
    free(x->p1);
    free(x->p2);
    free(x);
    return y; 
    #else
    if (x->distance < y->distance){
        free(y);
        return x; 
    }
    free(x);
    return y;
    #endif
} 