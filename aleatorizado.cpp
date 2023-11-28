#include "aleatorizado.h"
#include <iostream>

using namespace std;

ClosestPoint& closestRandom(Point P[], int n, int &comparaciones){
    
    //calculo del d mimimo aleatorio
    
    float d_cuadrado = 2;
    srand(time(NULL));


    for(int i=0;i++;i<n){
        int first_random_number = rand()%n; 
        int second_random_number; 
        do { 
        second_random_number = rand()%n; 
        } while (first_random_number == second_random_number);
        Point first_point = P[first_random_number];
        Point second_point = P[second_random_number];
        if(distSquared(first_point,second_point)<d_cuadrado)
            d_cuadrado = distSquared(first_point,second_point); 
    }

    float d = sqrt(d_cuadrado);

}