#include <iostream>
#include <cstdlib>
#include <cmath>
#include "random"

using namespace std;

//////////////////////////
/*       Hashing        */
//////////////////////////

// // Funcion de hash universal 
typedef struct {
    int a, b, p, m;
} HashU;

// Crea una funcion de hash universal
HashU *createHashU(int a, int b, int p, int m) {
    HashU *f = (HashU *) malloc(sizeof(HashU));
    f->a = a;
    f->b = b;
    f->p = p;
    f->m = m;
    return f;
}

// Aplica la funcion de hash a un entero
int applyHashU(HashU *f, int x) {
    return ((f->a * x + f->b) % f->p) % f->m;
}

int applyHashRapido(HashU *f, int x) {
    int k = f->m + 1 + rand();
    int k_2 = pow(2, k);
    int resultado = ((f->a * x + f->b) % k_2) / f->m;
    return resultado;
}

// Libera la memoria de la funcion de hash
void destroyHashU(HashU *f) {
    free(f);
}

// Crea a en el rango [1, p-1]
int createA(int p){
    return 1 + rand() % (p - 1);
}

// Crea b en el rango [0, p-1]
int createB(int p){
    return rand() % p;
}

////////////////////////
 /*      Punto        */
////////////////////////
typedef struct {
    int x, y;
} Point;

// Imprimir punto
void printPoint(Point p) {
    cout << "(" << p.x << ", " << p.y << ")";
}

// Aplica la funcion de hash a un punto
Point applyPointHashU(HashU *f, Point p, int d) {
    int x = applyHashU(f, p.x);
    int y = applyHashU(f, p.y);
    return {x, y};
}

// Aplica la funcion de hash rapido a un punto
Point applyPointHashRapido(HashU *f, Point p, int d) {
    int x = applyHashRapido(f, p.x);
    int y = applyHashRapido(f, p.y);
    return {x, y};
}


////////////////////////
 /*      Tabla        */
////////////////////////
// Linked list
typedef struct Node {
    Point p;
    struct Node *next;
} Node;

// Añadir elemento a la lista
void add(Node **head, Point p) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    newNode->p = p;
    newNode->next = *head;
    *head = newNode;
}

// Hash table
typedef struct {
    int m;
    Node **table;
} HashTable;

// Crear tabla
HashTable *createHashTable(int m) {
    HashTable *t = (HashTable *) malloc(sizeof(HashTable));
    t->m = m;
    t->table = (Node **) malloc(m * sizeof(Node *));
    for (int i = 0; i < m; i++) {
        t->table[i] = NULL;
    }
    return t;
}

int main() {
    // /* Hashing test */
    // int p = 1000000007;
    // int a = createA(p);
    // int b = createB(p);
    // int m = 100;
    // HashU *f = createHashU(a, b, p, m);
    // int x = 123456789;
    // cout << "Hashing " << x << " = " << applyHashU(f, x) << endl;
    // destroyHashU(f);
    
    // /* Point hashing test */
    // int p = 1000000007;
    // int a = createA(p);
    // int b = createB(p);
    // int m = 10;
    // HashU *f = createHashU(a, b, p, m);
    // Point p1 = {123456789, 87654321};
    // Point p2 = applyPointHashU(f, p1, m);
    // cout << "Hashing ";
    // printPoint(p1);
    // cout << " = ";
    // printPoint(p2);
    // cout << endl;
    // destroyHashU(f);

    /* Point hashing rapido test */
    int p = 1000000007;
    int a = createA(p);
    int b = createB(p);
    int m = 10;
    HashU *f = createHashU(a, b, p, m);
    Point p1 = {123456789, 87654321};
    Point p2 = applyPointHashRapido(f, p1, m);
    cout << "Hashing ";
    printPoint(p1);
    cout << " = ";
    printPoint(p2);
    cout << endl;
    destroyHashU(f);
    
    return 0;
}