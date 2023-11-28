#include "divide-and-conquer.h"
using namespace std;

//////////////////////////
/*       Hashing        */
//////////////////////////

// // Funcion de hash universal 
typedef struct {
    int a, b, p, m;
} HashU;

// Crea una funcion de hash universal
HashU *createHashU(int a, int b, int p, int m);

// Aplica la funcion de hash a un entero
int applyHashU(HashU *f, float x);

int applyHashRapido(HashU *f, float x);

// Libera la memoria de la funcion de hash
void destroyHashU(HashU *f);

// Crea a en el rango [1, p-1]
int createA(int p);

// Crea b en el rango [0, p-1]
int createB(int p);

// Aplica la funcion de hash a un punto
Point applyPointHashU(HashU *f, Point p);

// Aplica la funcion de hash rapido a un punto
Point applyPointHashRapido(HashU *f, Point p);


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