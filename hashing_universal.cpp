#include "hashing_universal.h"
using namespace std;

//////////////////////////
/*       Hashing        */
//////////////////////////

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
int applyHashU(HashU *f, float x) {
    return (((int)(f->a * x) + f->b) % f->p) % f->m;
}

int applyHashRapido(HashU *f, float x) {
    int k = f->m + 1 + rand();
    int k_2 = pow(2, k);
    int resultado = (((int)(f->a * x) + f->b) % k_2) / f->m;
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

// Aplica la funcion de hash a un punto
Point applyPointHashU(HashU *f, Point p) {
    float x = applyHashU(f, p.x);
    float y = applyHashU(f, p.y);
    return {x, y};
}

// Aplica la funcion de hash rapido a un punto
Point applyPointHashRapido(HashU *f, Point p) {
    float x = applyHashRapido(f, p.x);
    float y = applyHashRapido(f, p.y);
    return {x, y};
}


////////////////////////
 /*      Tabla        */
////////////////////////

// Linked list para las celdas de la tabla

// Crea un elemento de lista
List *createList(Point p) {
    List *newNode = (List *) malloc(sizeof(List));
    newNode->p = p;
    newNode->next = NULL;
    return newNode;
}

// Libera la memoria de la lista
void destroyList(List *head) {
    List *current = head;
    while (current != NULL) {
        List *next = current->next;
        free(current);
        current = next;
    }
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

// Libera la memoria de la tabla
void destroyHashTable(HashTable *tabla) {
    for (int i = 0; i < tabla->realSize; i++) {
        for (int j = 0; j < tabla->realSize; j++) {
            // Liberar cada lista
            List *head = (List *) tabla->table[i][j].ptr;
            if (head != NULL) {
                destroyList(head);
            }
        }
        // Liberar cada fila
        free(tabla->table[i]);
    }
    // Liberar la tabla
    free(tabla->table);
    free(tabla);
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

// Minima distancia entre un punto a los puntos de sus 8 casillas vecinas (incluyendose a si mismo)
void minDistanceCell(HashTable *tabla, Point p, int x_i, int y_i, int k, int size, float *minDistance) { // k es la profundidad en la LinkedList
    // Casillas vecinas a comparar, las demas son irrelevantes, pues las demas comparaciones ya las consideran.
    int dx[] = {0, 1, -1, 0, 1}; 
    int dy[] = {0, 0, -1, -1, -1};

    int selfCell = 1; // Si la casilla es la misma que la del punto
    
    for (int i = 0; i < 5; i++) {
        int x = x_i + dx[i];
        int y = y_i + dy[i];


        if (x >= 0 && x < size && y >= 0 && y < size) { // Si la casilla vecina esta dentro de la tabla
            List *head = (List *) tabla->table[x][y].ptr; // Lista de la casilla vecina

            int k_i = 0; // Profundidad en la lista

            while (head != NULL) { // Recorre la lista
                if (k_i++ < k && selfCell) // Si no se ha llegado a la profundidad k, las profundidades menores ya se compararon en la misma casilla
                    continue; // a su vez se post incrementa k_i

                // Point p2 = head->p; // Punto k_i de la lista
                float d = dist(p, head->p); // Distancia entre el punto y el punto k_i de la lista
                if (d < *minDistance) { // Si la distancia es menor que la minima distancia
                    *minDistance = d; // Se actualiza la minima distancia
                }
                head = head->next; // Siguiente punto de la lista
            }
        if (selfCell)
            selfCell = 0;
        }
    }
}



// Comparar minima distancia de cada punto con sus 8 vecinos y casilla propia
float nMinDistance(HashTable *tabla){
    float minDistance; // Valor de retorno
    int size = tabla->realSize; // Tamaño de la tabla

    for (int i = 0; i < size; i++) { // Recorre filas
        for (int j = 0; j < size; j++) { // Recorre columnas
            List *head = (List *) tabla->table[i][j].ptr; // Lista de la posicion actual
            int k = 0; // Profundidad en la lista
            while (head != NULL) { // Si la lista no esta vacia
                Point p = head->p; // Primer punto de la lista
                minDistanceCell(tabla, p, i, j, k, size, &minDistance); // Se calcula la minima distancia del punto con sus 8 vecinos
                head = head->next; // Siguiente punto de la lista
                k++; // Se incrementa k
            }
        }
    }

    return minDistance;
}


// int main() {
//     // /* Hashing test */
//     // int p = 1000000007;
//     // int a = createA(p);
//     // int b = createB(p);
//     // int m = 100;
//     // HashU *f = createHashU(a, b, p, m);
//     // int x = 123456789;
//     // cout << "Hashing " << x << " = " << applyHashU(f, x) << endl;
//     // destroyHashU(f);
    
//     // /* Point hashing test */
//     // int p = 1000000007;
//     // int a = createA(p);
//     // int b = createB(p);
//     // int m = 10;
//     // HashU *f = createHashU(a, b, p, m);
//     // Point p1 = {123456789, 87654321};
//     // Point p2 = applyPointHashU(f, p1);
//     // cout << "Hashing ";
//     // printPoint(p1);
//     // cout << " = ";
//     // printPoint(p2);
//     // cout << endl;
//     // destroyHashU(f);

//     /* Point hashing rapido test */
//     int p = 1000000007;
//     int a = createA(p);
//     int b = createB(p);
//     int m = 10;
//     HashU *f = createHashU(a, b, p, m);
//     Point p1 = {0.123456789f, 0.87654321f};
//     Point p2 = applyPointHashRapido(f, p1);
//     cout << "Hashing ";
//     cout << p1;
//     cout << " = ";
//     cout << p2;
//     cout << endl;
//     destroyHashU(f);
    
//     return 0;
// }