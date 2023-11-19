// A divide and conquer program in C++ 
// to find the smallest distance from a 
// given set of points. 

// Origin: https://www.geeksforgeeks.org/closest-pair-of-points-using-divide-and-conquer-algorithm/

#include "stdlib.h"
#include "iostream"
#include <resources.h>
using namespace std;


// A utility function to find the 
// distance between the closest points of 
// strip of given size. All points in 
// strip[] are sorted according to 
// y coordinate. They all have an upper
// bound on minimum distance as d. 
// Note that this method seems to be 
// a O(n^2) method, but it's a O(n) 
// method as the inner loop runs at most 6 times 
double stripClosest(Point strip[], int size, double d);

// A recursive function to find the 
// smallest distance. The array P contains 
// all points sorted according to x coordinate 
double closestUtil(Point P[], int n);

// The main function that finds the smallest distance 
// This method mainly uses closestUtil() 
double closestP(Point P[], int n);

// This code is contributed by rathbhupendra
