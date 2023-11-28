#include "aleatorizado.h"
#include <iostream>

using namespace std;

int indiceAleatorio(int n) {
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0,n);
    
    return distribution(generator);
}

int hashTest(Point *p, int d){
    return (int)((p->x + p->y)*100) % d;
}

ClosestPoint& closestRandom(Point P[], int n, int &comparaciones){
    
    //calculo del d mimimo aleatorio
    
    float d_cuadrado = 2;
    srand(time(NULL));

    // se escogen n pares de puntos aleatorios y se calcula la distancia entre ellos, se guarda la menor distancia.
    for(int i=0;i++;i<n){
        int first_random_number = indiceAleatorio(n);
        int second_random_number;
        do { 
        second_random_number = indiceAleatorio(n);
        } while (first_random_number == second_random_number);
        Point first_point = P[first_random_number];
        Point second_point = P[second_random_number];
        if(distSquared(first_point,second_point)<d_cuadrado)
            d_cuadrado = distSquared(first_point,second_point); 
    }
    float d = sqrt(d_cuadrado);

    // Dividir el plano [0,1]x[0,1] en d x d celulas de lado 1/d, agrupando los puntos en función de un hash.
    // Se crea un arreglo de listas de puntos, donde cada lista representa una celda del plano.
    int cell_number = (int)(1/d);

    vector<vector<vector<Point>>> grid(cell_number, vector<vector<Point>>(cell_number));

    for (int i = 0; i < n; i++){
        int x = hashTest(&P[i], cell_number);
        int y = hashTest(&P[i], cell_number);
        grid[x][y].push_back(P[i]);
    }



}