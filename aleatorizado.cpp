#include "aleatorizado.h"
#include <iostream>

using namespace std;

int indiceAleatorio(int n) {
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0,n);
    
    return distribution(generator);
}

ClosestPoint* closestRandom(Point P[], int n, ull &comparaciones){
    
    //calculo del d mimimo aleatorio
    
    ClosestPoint *d = (ClosestPoint*)malloc(sizeof(ClosestPoint));
    d->distance = 2;
    #if DEBUG
    Point *p = (Point*)malloc(sizeof(Point));
    Point *pt2 = (Point*)malloc(sizeof(Point));
    #endif

    // se escogen n pares de puntos aleatorios y se calcula la distancia entre ellos, se guarda la menor distancia.
    for(int i=0;i<n;i++){
        comparaciones += 1;
        int first_random_number = indiceAleatorio(n);
        int second_random_number;
        do { 
        second_random_number = indiceAleatorio(n);
        } while (first_random_number == second_random_number);
        Point first_point = P[first_random_number];
        Point second_point = P[second_random_number];
        float distance = distSquared(first_point,second_point);
        if(distance < d->distance){
            d->distance = distance;
            #if DEBUG
            *p = first_point;
            *pt2 = second_point;
            d->p1 = p;
            d->p2 = pt2;
            #endif
        }
    }

    // Dividir el plano [0,1]x[0,1] en d x d celulas de lado 1/d, agrupando los puntos en función de un hash.
    // Se crea un arreglo de listas de puntos, donde cada lista representa una celda del plano.
    float distance_cell = sqrt(d->distance);
    int cell_number = (int)(1/distance_cell);
    cout << "d: " << distance_cell << " cell_number: " << cell_number << endl;

    vector<vector<vector<Point>>> grid(cell_number, vector<vector<Point>>(cell_number));
    HashU *f = createHashU(100, 20, 1000000007, cell_number);
    for (int i = 0; i < n; i++){
        comparaciones += 1;
        int x = applyHashU(f, P[i].x);
        int y = applyHashU(f, P[i].y);
        cout << P[i] << " hash: " << x << " " << y << endl;
        grid[x][y].push_back(P[i]);
    }

    // Se recorren las celdas del plano, y se calcula la distancia entre los puntos de cada celda y los puntos de las celdas vecinas.
    // Se guarda la menor distancia.
    int size, size2;
    for(int i=0;i<cell_number;i++){
        for(int j=0;j<cell_number;j++){
            size = grid[i][j].size();
            for(int k=0;k<size;k++){
                for(int l=k+1;l<size;l++){
                    comparaciones += 1;
                    float distance = distSquared(grid[i][j][k],grid[i][j][l]);
                    if(distance < d->distance){
                        d->distance = distance;
                        #if DEBUG
                        *p = grid[i][j][k];
                        *pt2 = grid[i][j][l];
                        d->p1 = p;
                        d->p2 = pt2;
                        #endif
                    }
                }
            }
            if (j<cell_number-1){
                size2 = grid[i][j+1].size();
                for(int k=0;k<size;k++){
                    for(int l=0;l<size2;l++){
                        comparaciones += 1;
                        float distance = distSquared(grid[i][j][k],grid[i][j+1][l]);
                        if(distance < d->distance){
                            d->distance = distance;
                            #if DEBUG
                            *p = grid[i][j][k];
                            *pt2 = grid[i][j+1][l];
                            d->p1 = p;
                            d->p2 = pt2;
                            #endif
                        }
                    }
                }
            }
            if (i<cell_number-1){
                size2 = grid[i+1][j].size();
                for(int k=0;k<size;k++){
                    for(int l=0;l<size2;l++){
                        comparaciones += 1;
                        float distance = distSquared(grid[i][j][k],grid[i+1][j][l]);
                        if(distance < d->distance){
                            d->distance = distance;
                            #if DEBUG
                            *p = grid[i][j][k];
                            *pt2 = grid[i+1][j][l];
                            d->p1 = p;
                            d->p2 = pt2;
                            #endif
                        }
                    }
                }
            }
            if (i==cell_number-1 || j==cell_number-1){
                continue;
            }
            size2 = grid[i+1][j+1].size();
            for(int k=0;k<size;k++){
                for(int l=0;l<size2;l++){
                    comparaciones += 1;
                    float distance = distSquared(grid[i][j][k],grid[i+1][j+1][l]);
                    if(distance < d->distance){
                        d->distance = distance;
                        #if DEBUG
                        *p = grid[i][j][k];
                        *pt2 = grid[i+1][j+1][l];
                        d->p1 = p;
                        d->p2 = pt2;
                        #endif
                    }
                }
            }
        }
    }

    // Se entrega el resultado
    d->comparaciones = comparaciones;
    return d;

}