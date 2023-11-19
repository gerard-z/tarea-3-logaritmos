// A structure to represent a Point in 2D plane 
class Point 
{ 
	public:
	int x, y; 
}; 

// Needed to sort array of points 
// according to X coordinate 
int compareX(const void* a, const void* b);

// Needed to sort array of points according to Y coordinate 
int compareY(const void* a, const void* b);

// A utility function to find the 
// distance between two points 
float dist(Point p1, Point p2);

// A Brute Force method to return the 
// smallest distance between two points 
// in P[] of size n 
float bruteForce(Point P[], int n);

// A utility function to find 
// minimum of two float values 
float min(float x, float y);