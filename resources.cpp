#include "resources.h"

using namespace std;

/* Following two functions are needed for library function qsort(). 
Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */

ostream& operator<<(ostream& os, const Point& p){
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

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
ClosestPoint& bruteForce(Point P[], int n) 
{ 
	float min = numeric_limits<float>::max(); 
    Point p1, p2;
	for (int i = 0; i < n; ++i) 
		for (int j = i+1; j < n; ++j) 
			if (dist(P[i], P[j]) < min){
				min = dist(P[i], P[j]); 
                p1 = P[i];
                p2 = P[j];
            }
    ClosestPoint *c = (ClosestPoint*)malloc(sizeof(ClosestPoint));
    Point *p = (Point*)malloc(sizeof(Point));
    p->x = p1.x;
    p->y = p1.y;
    c->p1 = p;
    Point *pt2 = (Point*)malloc(sizeof(Point));
    pt2->x = p2.x;
    pt2->y = p2.y;
    c->p2 = pt2;
    // c->p1 = P[p1];
    // c->p2 = P[p2];
    c->distance = min;
	return *c;
} 

// A utility function to find 
// minimum distance between the closest points 
ClosestPoint& min(ClosestPoint &x, ClosestPoint &y) 
{ 
	return (x.distance < y.distance)? x : y; 
} 