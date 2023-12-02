#include "divide-and-conquer.h"
using namespace std;

//////////////////////////
/*       Hashing        */
//////////////////////////

// // Funcion de hash universal 
typedef struct HashU{
    ull a, b, p, m;
    int cell_number;

    HashU(ull new_a, ull new_b, ull new_p, ull new_m, int c) : a(new_a), b(new_b), p(new_p), m(new_m), cell_number(c) {}

    std::size_t operator()(const Coord &c) const noexcept
    {
        int h1 = c.x * cell_number + c.y;
        return ((static_cast<ull>(a * h1) + b) % p) % m;
    }
} HashU;


typedef struct HashR{
    ull a, b, p, m;
    int cell_number;

    HashR(ull new_a, ull new_b, ull new_p, ull new_m, int c) : a(new_a), b(new_b), p(new_p), m(new_m), cell_number(c) {}

    std::size_t operator()(const Coord &c) const noexcept
    {
        int h1 = c.x * cell_number + c.y;
        ull l = ceil(log2(m));
        ull k_2 = pow(2, l+2) - 1; // k = 2^l - 1 = 111...1 (l+2 veces)
        ull resultado = ((static_cast<ull>(a * h1) + b) & k_2) >> l;
        return resultado;
    }
} HashR;

// Crea una funcion de hash universal
HashU *createHashU(ull a, ull b, ull p, ull m, int c);

// Crea una funcion de hash rápido
HashR *createHashR(ull a, ull b, ull p, ull m, int c);

// Aplica la funcion de hash a un entero
ull applyHashU(const HashU *f, int x);

ull applyHashRapido(const HashR *f, int x);

// Libera la memoria de la funcion de hash
void destroyHashU(HashU *f);

// Crea a en el rango [1, p-1]
int createA(int p);

// Crea b en el rango [0, p-1]
int createB(int p);

// Aplica la funcion de hash a un punto
Point applyPointHashU(const HashU *f, Point p);

// Aplica la funcion de hash rapido a un punto
Point applyPointHashRapido(const HashR *f, Point p);


////////////////////////
 /*      Tabla        */
////////////////////////

// Linked list para las celdas de la tabla
typedef struct List {
    Point p;
    struct List *next;
} List;

// Crea un elemento de lista
List *createList(Point p);

// Libera la memoria de la lista
void destroyList(List *head);

// Añade un elemento a la lista
void putBack(List **head, Point p);

// Nodos de la tabla (filas y columnas)
typedef struct Node {
    void *ptr;
    Node *next;
} Node;

// Hash table
typedef struct {
    int d; // Tamaño de grilla
    int realSize; // Tamaño real de la tabla (int)
    Node **table; // Tabla
} HashTable;


// Crea una tabla de hash
HashTable *createHashTable(int d);

// Libera la memoria de la tabla
void destroyHashTable(HashTable *tabla);


// Añade un elemento a la tabla
void addHashTable(HashTable *tabla, Point p, HashU *f);

// Minima distancia entre un punto a los puntos de sus 8 casillas vecinas (incluyendose a si mismo)
void minDistanceCell(HashTable *tabla, Point p, int x_i, int y_i, int k, int size, float *minDistance);



// Comparar minima distancia de cada punto con sus 8 vecinos y casilla propia
float nMinDistance(HashTable *tabla);