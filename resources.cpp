#include "resources.h"
#include "cmath"
#include "cfloat"

using namespace std;

/* Following two functions are needed for library function qsort(). 
Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */

// Needed to sort array of points 
// according to X coordinate 
int compareX(const void* a, const void* b) 
{ 
	Point *p1 = (Point *)a, *p2 = (Point *)b; 
	return (p1->x - p2->x); 
} 

// Needed to sort array of points according to Y coordinate 
int compareY(const void* a, const void* b) 
{ 
	Point *p1 = (Point *)a, *p2 = (Point *)b; 
	return (p1->y - p2->y); 
} 

// A utility function to find the 
// distance between two points 
float dist(Point p1, Point p2) 
{ 
	return sqrt( (p1.x - p2.x)*(p1.x - p2.x) + 
				(p1.y - p2.y)*(p1.y - p2.y) 
			); 
} 

// A Brute Force method to return the 
// smallest distance between two points 
// in P[] of size n 
float bruteForce(Point P[], int n) 
{ 
	float min = FLT_MAX; 
	for (int i = 0; i < n; ++i) 
		for (int j = i+1; j < n; ++j) 
			if (dist(P[i], P[j]) < min) 
				min = dist(P[i], P[j]); 
	return min; 
} 

// A utility function to find 
// minimum of two float values 
float min(float x, float y) 
{ 
	return (x < y)? x : y; 
} 