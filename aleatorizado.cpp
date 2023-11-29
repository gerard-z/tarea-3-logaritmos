#include "aleatorizado.h"
#include <iostream>

using namespace std;

void findMinDistance(const vector<vector<vector<Point*>>> &grid, ClosestPoint *d, Point *p, Point *pt2, const vector<Point*> &gij1, const vector<Point*> &gij2, ull &comparaciones){
    int size = gij1.size();
    int size2 = gij2.size();
    for(int k=0;k<size;k++){
        for(int l=0;l<size2;l++){
            comparaciones += 1;
            float distance = distSquared(*gij1[k],*gij2[l]);
            if(distance < d->distance){
                d->distance = distance;
                #if SavePoints
                *p = *gij1[k];
                *pt2 = *gij2[l];
                d->p1 = p;
                d->p2 = pt2;
                #endif
            }
        }
    }
}

void checkGrid4neighbors(int cell_number, const vector<vector<vector<Point*>>> &grid, ClosestPoint *d, Point *p, Point *pt2, ull &comparaciones){
    for(int i=0;i<cell_number;i++){
        for(int j=0;j<cell_number;j++){
            int size = grid[i][j].size();
            for(int k=0;k<size;k++){
                for(int l=k+1;l<size;l++){
                    comparaciones += 1;
                    float distance = distSquared(*grid[i][j][k],*grid[i][j][l]);
                    if(distance < d->distance){
                        d->distance = distance;
                        #if SavePoints
                        *p = *grid[i][j][k];
                        *pt2 = *grid[i][j][l];
                        d->p1 = p;
                        d->p2 = pt2;
                        #endif
                    }
                }
            }
            if (j<cell_number-1){
                findMinDistance(grid,d,p,pt2,grid[i][j],grid[i][j+1],comparaciones);
                if (i>0)
                    findMinDistance(grid,d,p,pt2,grid[i][j],grid[i-1][j+1],comparaciones);
            }
            if (i<cell_number-1){
                findMinDistance(grid,d,p,pt2,grid[i][j],grid[i+1][j],comparaciones);
            }
            if (i==cell_number-1 || j==cell_number-1){
                continue;
            }
            findMinDistance(grid,d,p,pt2,grid[i][j],grid[i+1][j+1],comparaciones);
        }
    }

}

void checkGrid8neighbors(int cell_number, vector<vector<vector<Point*>>> &grid, ClosestPoint *d, Point *p, Point *pt2, ull &comparaciones){
    int size1;
    queue<vector<Point*>> neighbors;
    for(int i=0;i<cell_number;i++){
        for(int j=0;j<cell_number;j++){
            size1 = grid[i][j].size();
            if (i>0){
                neighbors.push(grid[i-1][j]);
                if (j>0)
                    neighbors.push(grid[i-1][j-1]);
                if (j<cell_number-1)
                    neighbors.push(grid[i-1][j+1]);
            }
            if (j>0)
                neighbors.push(grid[i][j-1]);
            if (j<cell_number-1)
                neighbors.push(grid[i][j+1]);
            if (i<cell_number-1){
                neighbors.push(grid[i+1][j]);
                if (j>0)
                    neighbors.push(grid[i+1][j-1]);
                if (j<cell_number-1)
                    neighbors.push(grid[i+1][j+1]);
            }
            for (int k=0;k<size1;k++){
                for (int l=k+1;l<size1;l++){
                    comparaciones += 1;
                    float distance = distSquared(*grid[i][j][k],*grid[i][j][l]);
                    if(distance < d->distance){
                        d->distance = distance;
                        #if SavePoints
                        *p = *grid[i][j][k];
                        *pt2 = *grid[i][j][l];
                        d->p1 = p;
                        d->p2 = pt2;
                        #endif
                    }
                }
            }
            while (!neighbors.empty()){
                findMinDistance(grid,d,p,pt2,grid[i][j],neighbors.front(),comparaciones);
                neighbors.pop();
            }
            grid[i][j].clear();
        }
    }
}

ClosestPoint* closestRandom(Point P[], int n, ull &comparaciones){
    
    //calculo del d mimimo aleatorio
    
    ClosestPoint *d = (ClosestPoint*)malloc(sizeof(ClosestPoint));
    d->distance = 2;
    #if SavePoints
    Point *p = (Point*)malloc(sizeof(Point));
    Point *pt2 = (Point*)malloc(sizeof(Point));
    #endif

    // se escogen n pares de puntos aleatorios y se calcula la distancia entre ellos, se guarda la menor distancia.
    #if DEBUG
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    #endif
    // Generador de numeros aleatorios entre 0 y n-1
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0,n-1);

    for(int i=0;i<n;i++){
        comparaciones += 1;
        // Obtiene un indice aleatorio en el rango [0, n-1]
        int first_random_number = distribution(generator);
        int second_random_number;
        do { 
        second_random_number = distribution(generator);
        } while (first_random_number == second_random_number);
        Point first_point = P[first_random_number];
        Point second_point = P[second_random_number];
        float distance = distSquared(first_point,second_point);
        if(distance < d->distance){
            d->distance = distance;
            #if SavePoints
            *p = first_point;
            *pt2 = second_point;
            d->p1 = p;
            d->p2 = pt2;
            #endif
        }
    }

    #if DEBUG
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    cout << "Tiempo distancia de puntos aleatorizados: " << time_span.count() << " segundos." << endl;
    #endif

    // Dividir el plano [0,1]x[0,1] en d x d celulas de lado 1/d, agrupando los puntos en función de un hash.
    // Se crea un arreglo de listas de puntos, donde cada lista representa una celda del plano.
    float distance_cell = sqrt(d->distance);
    int cell_number = (int)(1/distance_cell)+1;
    // cout << "d: " << distance_cell << " cell_number: " << cell_number << endl;

    #if DEBUG
    t1 = high_resolution_clock::now();
    #endif

    vector<vector<vector<Point*>>> grid(cell_number, vector<vector<Point*>>(cell_number));
    HashU *f = createHashU(100, 20, 1000000007, cell_number);
    for (int i = 0; i < n; i++){
        comparaciones += 1;
        int x = applyHashU(f, P[i].x);
        int y = applyHashU(f, P[i].y);
        // cout << P[i] << " hash: " << x << " " << y << endl;
        grid[x][y].push_back(&P[i]);
    }
    destroyHashU(f);

    #if DEBUG
    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    cout << "Tiempo hash: " << time_span.count() << " segundos." << endl;
    #endif

    // Se recorren las celdas del plano, y se calcula la distancia entre los puntos de cada celda y los puntos de las celdas vecinas.
    // Se guarda la menor distancia.
    #if DEBUG
    t1 = high_resolution_clock::now();
    #endif
    // algunas pruebas parecen mostrar que el mejor rendimiento es con la 1era opcion
    checkGrid4neighbors(cell_number,grid,d,p,pt2,comparaciones);
    // checkGrid8neighbors(cell_number,grid,d,p,pt2,comparaciones);
    #if DEBUG
    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    cout << "Tiempo distancia de puntos en celdas: " << time_span.count() << " segundos." << endl;
    #endif

    // Se entrega el resultado
    d->comparaciones = comparaciones;
    return d;

}