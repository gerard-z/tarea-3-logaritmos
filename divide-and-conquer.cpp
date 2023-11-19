// A divide and conquer program in C++ 
// to find the smallest distance from a 
// given set of points. 

// Origin: https://www.geeksforgeeks.org/closest-pair-of-points-using-divide-and-conquer-algorithm/

#include "divide-and-conquer.h"
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
float stripClosest(Point strip[], int size, float d) 
{ 
	float min = d; // Initialize the minimum distance as d 

	qsort(strip, size, sizeof(Point), compareY); 

	// Pick all points one by one and try the next points till the difference 
	// between y coordinates is smaller than d. 
	// This is a proven fact that this loop runs at most 6 times 
	for (int i = 0; i < size; ++i) 
		for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j) 
			if (dist(strip[i],strip[j]) < min) 
				min = dist(strip[i], strip[j]); 

	return min; 
} 

// A recursive function to find the 
// smallest distance. The array P contains 
// all points sorted according to x coordinate 
float closestUtil(Point P[], int n) 
{ 
	// If there are 2 or 3 points, then use brute force 
	if (n <= 3) 
		return bruteForce(P, n); 

	// Find the middle point 
	int mid = n/2; 
	Point midPoint = P[mid]; 

	// Consider the vertical line passing 
	// through the middle point calculate 
	// the smallest distance dl on left 
	// of middle point and dr on right side 
	float dl = closestUtil(P, mid); 
	float dr = closestUtil(P + mid, n - mid); 

	// Find the smaller of two distances 
	float d = min(dl, dr); 

	// Build an array strip[] that contains 
	// points close (closer than d) 
	// to the line passing through the middle point 
	Point strip[n]; 
	int j = 0; 
	for (int i = 0; i < n; i++) 
		if (abs(P[i].x - midPoint.x) < d) 
			strip[j] = P[i], j++; 

	// Find the closest points in strip. 
	// Return the minimum of d and closest 
	// distance is strip[] 
	return min(d, stripClosest(strip, j, d) ); 
} 

// The main function that finds the smallest distance 
// This method mainly uses closestUtil() 
float closest(Point P[], int n) 
{ 
	qsort(P, n, sizeof(Point), compareX); 

	// Use recursive function closestUtil()
	// to find the smallest distance 
	return closestUtil(P, n); 
} 

// Driver code 
int main() 
{ 
	Point P[] = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 4}};
	int n = sizeof(P) / sizeof(P[0]); 
	cout << "The smallest distance is " << closest(P, n); 
	return 0; 
} 

// This code is contributed by rathbhupendra
