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
Point applyPointHashU(HashU *f, Point p) {
    int x = applyHashU(f, p.x);
    int y = applyHashU(f, p.y);
    return {x, y};
}

// Aplica la funcion de hash rapido a un punto
Point applyPointHashRapido(HashU *f, Point p) {
    int x = applyHashRapido(f, p.x);
    int y = applyHashRapido(f, p.y);
    return {x, y};
}


////////////////////////
 /*      Tabla        */
////////////////////////

// Linked list para las celdas de la tabla
typedef struct List {
    Point p;
    struct List *next;
} List;

// Crea un elemento de lista
List *createList(Point p) {
    List *newNode = (List *) malloc(sizeof(List));
    newNode->p = p;
    newNode->next = NULL;
    return newNode;
}

// Añade un elemento a la lista
void putBack(List **head, Point p) {
    List *newNode = createList(p);
    if (*head == NULL) {
        *head = newNode;
    } else {
        List *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

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
HashTable *createHashTable(int d) {
    HashTable *tabla = (HashTable *) malloc(sizeof(HashTable));
    // Tamaños de la tabla
    tabla->d = d;
    int realSize = (int) (1/d) + 1;
    tabla->realSize = realSize;
    
    // Inicializar la tabla
    tabla->table = (Node **) malloc(sizeof(Node *) * realSize); // Se pide memoria para las filas
    for (int i = 0; i < realSize; i++) {
        tabla->table[i] = (Node *) malloc(realSize * sizeof(Node)); // Se pide memoria para las columnas
        for (int j = 0; j < realSize; j++) {
            // Inicializar cada elemento de la tabla en nulo
            tabla->table[i][j].ptr = NULL;
            tabla->table[i][j].next = NULL;
        }
    }

    return tabla;
}

// Añade un elemento a la tabla
void addHashTable(HashTable *tabla, Point p, HashU *f) {
    Point pHashed = applyPointHashU(f, p);
    int x = pHashed.x;
    int y = pHashed.y;
    
    // Si la posicion esta vacia, se crea una lista y se agrega el elemento
    if (tabla->table[x][y].ptr == NULL) {
        List *head = createList(p);
        tabla->table[x][y].ptr = head;
    } else {
        // Si la posicion no esta vacia, se agrega a la lista
        List *head = (List *) tabla->table[x][y].ptr;
        putBack(&head, p);
    }
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
    // Point p2 = applyPointHashU(f, p1);
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
    Point p2 = applyPointHashRapido(f, p1);
    cout << "Hashing ";
    printPoint(p1);
    cout << " = ";
    printPoint(p2);
    cout << endl;
    destroyHashU(f);
    
    return 0;
}