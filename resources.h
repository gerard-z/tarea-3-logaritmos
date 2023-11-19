#include "cmath"
#include "cfloat"
#include "limits.h"
#include <iostream>

// A structure to represent a Point in 2D plane 
class Point 
{ 
	public:
	double x, y; 

    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

class ClosestPoint
{
    public:
    Point *p1, *p2;
    double distance;
};

// Needed to sort array of points 
// according to X coordinate 
int compareX(const void* a, const void* b);

// Needed to sort array of points according to Y coordinate 
int compareY(const void* a, const void* b);

// A utility function to find the 
// distance between two points 
double dist(Point p1, Point p2);

// A Brute Force method to return the 
// smallest distance between two points 
// in P[] of size n 
ClosestPoint& bruteForce(Point P[], int n);

// A utility function to find 
// minimum of two float values 
ClosestPoint& min(ClosestPoint &x, ClosestPoint &y);