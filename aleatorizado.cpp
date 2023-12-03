#include "aleatorizado.h"
#include <iostream>

using namespace std;

void findMinDistance(ClosestPoint *d, Point *p, Point *pt2, const vector<Point*> &gij1, const vector<Point*> &gij2, ull &comparaciones){
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
                #endif
            }
        }
    }
}

void checkGrid4neighbors(int cell_number, const unordered_map<Coord, vector<Point*>, HashU> &grid, ClosestPoint *d, Point *p, Point *pt2, ull &comparaciones){
    // for(int i=0;i<cell_number;i++){
    //     for(int j=0;j<cell_number;j++){
    for (const auto& [coord, value] : grid){
        int size = value.size();
        for(int k=0;k<size;k++){
            for(int l=k+1;l<size;l++){
                comparaciones += 1;
                float distance = distSquared(*value[k],*value[l]);
                if(distance < d->distance){
                    d->distance = distance;
                    #if SavePoints
                    *p = *value[k];
                    *pt2 = *value[l];
                    #endif
                }
            }
        }
        Coord coord2 = coord;
        coord2.y += 1;
        if (grid.contains(coord2))
            findMinDistance(d,p,pt2,grid.at(coord),grid.at(coord2),comparaciones);
        coord2.x -= 1;
        if (grid.contains(coord2))
            findMinDistance(d,p,pt2,grid.at(coord),grid.at(coord2),comparaciones);
        coord2 = coord;
        coord2.x += 1;
        if (grid.contains(coord2)){
            findMinDistance(d,p,pt2,grid.at(coord),grid.at(coord2),comparaciones);
        }
        coord2.y += 1;
        if (grid.contains(coord2)){
            findMinDistance(d,p,pt2,grid.at(coord),grid.at(coord2),comparaciones);
        }
    }

}

void checkGrid8neighbors(int cell_number, unordered_map<Coord, vector<Point*>, HashU> &grid, ClosestPoint *d, Point *p, Point *pt2, ull &comparaciones){
    int size1;
    queue<vector<Point*>> neighbors;
    for(int i=0;i<cell_number;i++){
        for(int j=0;j<cell_number;j++){
            Coord coord = {i,j};
            if (!grid.contains(coord))
                continue;
            size1 = grid.at(coord).size();
            if (i>0){
                neighbors.push(grid.at(Coord {i-1,j}));
                if (j>0)
                    neighbors.push(grid.at(Coord {i-1,j-1}));
                if (j<cell_number-1)
                    neighbors.push(grid.at(Coord {i-1,j+1}));
            }
            if (j>0)
                neighbors.push(grid.at(Coord {i,j-1}));
            if (j<cell_number-1)
                neighbors.push(grid.at(Coord {i,j+1}));
            if (i<cell_number-1){
                neighbors.push(grid.at(Coord {i+1,j}));
                if (j>0)
                    neighbors.push(grid.at(Coord {i+1,j-1}));
                if (j<cell_number-1)
                    neighbors.push(grid.at(Coord {i+1,j+1}));
            }
            for (int k=0;k<size1;k++){
                for (int l=k+1;l<size1;l++){
                    comparaciones += 1;
                    float distance = distSquared(*grid.at(coord)[k],*grid.at(coord)[l]);
                    if(distance < d->distance){
                        d->distance = distance;
                        #if SavePoints
                        *p = *grid.at(coord)[k];
                        *pt2 = *grid.at(coord)[l];
                        #endif
                    }
                }
            }
            while (!neighbors.empty()){
                findMinDistance(d,p,pt2,grid.at(coord),neighbors.front(),comparaciones);
                neighbors.pop();
            }
            grid.at(coord).clear();
        }
    }
}

// static vector<Coord> roundToGrid(Point P[], int n, float distance_cell){
//     std::vector<Coord> roundedPoints;
//     for (int i = 0; i < n; i++){
//         Point p = P[i];
//         int roundedX = static_cast<int>(p.x / distance_cell);
//         int roundedY = static_cast<int>(p.y / distance_cell);
//         roundedPoints.push_back({roundedX, roundedY});
//     }
//     return roundedPoints;
// }

ClosestPoint* closestRandom(Point P[], int n, ull &comparaciones){
    
    //calculo del d mimimo aleatorio
    
    ClosestPoint *d = (ClosestPoint*)malloc(sizeof(ClosestPoint));
    d->distance = 2.0f;
    #if SavePoints
    Point *p = (Point*)malloc(sizeof(Point));
    Point *pt2 = (Point*)malloc(sizeof(Point));
    d->p1 = p;
    d->p2 = pt2;
    #endif

    // se escogen n pares de puntos aleatorios y se calcula la distancia entre ellos, se guarda la menor distancia.
    // Generador de numeros aleatorios entre 0 y n-1
    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<int> distribution(0,n-1);
    #if DEBUG
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    #endif

    for(int i=0;i<n;i++){
        comparaciones += 1;
        // Obtiene un indice aleatorio en el rango [0, n-1]
        int first_random_number = distribution(generator);
        int second_random_number;
        do { 
        second_random_number = distribution(generator);
        } while (first_random_number == second_random_number);
        float distance = distSquared(P[first_random_number],P[second_random_number]);
        if(distance < d->distance){
            d->distance = distance;
            #if SavePoints
            *p = P[first_random_number];
            *pt2 = P[second_random_number];
            #endif
        }
    }

    #if DEBUG
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Tiempo distancia de puntos aleatorizados: " << time_span.count() << " segundos." << endl;
    #endif

    // Crear una grilla de celdas d x d, aproximando los puntos a dichos cuadrados.
    float distance_cell = sqrt(d->distance);
    int cell_number = (int)(1.0f/distance_cell)+1;

    #if DEBUG
    d->comparaciones = 0;
    d->tiempo = 0;
    std::cout << "d: " << *d << " cell_number: " << cell_number << endl;
    t1 = high_resolution_clock::now();
    #endif

    // vector<Coord> roundedPoints = roundToGrid(P, n, distance_cell);
    std::vector<Coord> roundedPoints;
    for (int i = 0; i < n; i++){
        Point p = P[i];
        int roundedX = static_cast<int>(p.x / distance_cell);
        int roundedY = static_cast<int>(p.y / distance_cell);
        roundedPoints.push_back({roundedX, roundedY});
    }

    #if DEBUG
    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Tiempo rounding: " << time_span.count() << " segundos." << endl;
    #endif

    // Se crea una tabla hash donde las llaves corresponden a los puntos en la grilla, y los valores a los puntos en el plano.
    #if DEBUG
    t1 = high_resolution_clock::now();
    #endif

    uniform_int_distribution<int> uniformAB(cell_number,1'000'000'006); // Lograr que distintos puntos correspondan a entero distintos
    int a = uniformAB(generator);
    int b = uniformAB(generator);
    #if DEBUG
    std::cout << "a: " << a << " b: " << b << endl;
    #endif
    HashU *f = createHashU(a, b, 1000000007, n, cell_number);
    unordered_map<Coord, vector<Point*>, HashU> hashTable(0, *f);
    for (int i = 0; i < n; i++){
        comparaciones += 1;
        hashTable[roundedPoints[i]].push_back(&P[i]);
    }
    destroyHashU(f);

    #if DEBUG
    // auto print_key_value = [](const auto& key, const auto& value)
    // {
    //     std::std::cout << "Key:[" << key << "] Value:[";
    //     for (const auto& point : value)
    //         std::std::cout << "Point: " << *point << " ";
    //     std::std::cout << "]\n";
    // };
    // for (const auto& [key, value] : hashTable)
    //     print_key_value(key, value);

    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Tiempo hash: " << time_span.count() << " segundos." << endl;
    #endif

    // Se recorren las celdas del plano, y se calcula la distancia entre los puntos de cada celda y los puntos de las celdas vecinas.
    // Se guarda la menor distancia.
    #if DEBUG
    t1 = high_resolution_clock::now();
    #endif
    // algunas pruebas parecen mostrar que el mejor rendimiento es con la 1era opcion
    checkGrid4neighbors(cell_number,hashTable,d,p,pt2,comparaciones);
    // checkGrid8neighbors(cell_number,hashTable,d,p,pt2,comparaciones);
    #if DEBUG
    t2 = high_resolution_clock::now();
    time_span = duration_cast<duration<double>>(t2 - t1);
    std::cout << "Tiempo distancia de puntos en celdas: " << time_span.count() << " segundos." << endl;
    #endif

    // Se entrega el resultado
    d->comparaciones = comparaciones;
    return d;

}