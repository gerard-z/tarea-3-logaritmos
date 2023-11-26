#include "divide-and-conquer.h"
using namespace std;

#include <bits/stdc++.h>
 
// To find the closest pair of points
ClosestPoint& sweepClosestPair(vector<Point> v, int size, ClosestPoint &d, int &comparaciones)
{

 
    // Sort them according to their
    // x-coordinates
    sort(v.begin(), v.end());
 
    // Minimum distance b/w points
    // seen so far
    float d = 3;
 
    // Keeping the points in
    // increasing order
    set<Point> st;
    st.insert({ v[0].x, v[0].y });

    Point* p1,p2;

    p1 = (Point*)malloc(sizeof(Point));
    p2 = (Point*)malloc(sizeof(Point));
    ClosestPoint *c = (ClosestPoint*)malloc(sizeof(ClosestPoint));

    Point tempP1, tempP2;

    st.insert({ v[0].x, v[0].y });
 
    for (i = 1; i < n; i++) {
        auto l = st.lower_bound(
            { v[i].x - d, v[i].y - d });
        auto r = st.upper_bound(
            { v[i].x, v[i].y + d });
        if (l == st.end())
            continue;
 
        for (auto p = l; p != r; p++) {
            Point val = *p;
            float dis = (v[i].x - val.x)
                           * (v[i].x - val.x)
                       + (v[i].y - val.y)
                             * (v[i].y - val.y);
 
            // Updating the minimum
            // distance dis
            if (d > dis)
            {
                d = dis;
                tempP1.x = v[i].x;
                tempP1.y = v[i].y;
                tempP2.x = val.x;
                tempP2.y = val.y;    
            }
                

        }
        st.insert({ v[i].x, v[i].y });
    }
 
    d = sqrtf(d);
    p1->x = tempP1.x;
    p1->y = tempP1.y;
    p2->x = tempP2.y;
    p2->y = tempP2.y;
    c->p1 = p1;
    c->p2 = p2;
    c->distance = d;
    //c->comparaciones = NULL
    return *c;

}
 

 /**
// Driver code
  int main()
{
 
    // Points on a plane P[i] = {x, y}
    vector<pair<int, int> > P = {
        { 1, 2 }, { 2, 3 }, { 3, 4 }, { 5, 6 }, { 2, 1 }
    };
    int n = P.size();
 
    // Function call
    cout << "The smallest distance is "
         << closestPair(P, n);
    return 0;
} 
 */


