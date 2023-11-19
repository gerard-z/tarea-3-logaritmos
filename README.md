# tarea-3-logaritmos
 tarea 3 del curso Diseño y análisis de algoritmo


# Compilar con dependencias:
Tener en cuenta que compilar archivos con otras dependencias deben ser enunciadas al momento de compilar.
-   -I.\ indica que debe incluir la carpeta actual como parte del fichero de archivos a poder incluir
-   -o "ejecutable" indica el nombre del archivo de salida.
-   Luego se agregan los archivos a compilar, el primero es el principal y los otros las dependencias.

### Para compilar el algoritmo de divide y conquista con el archivo recursos.
```
g++ -Wall -I.\ -o divide-and-conquer.exe divide-and-conquer.cpp resources.cpp
```

También se puede hacer paso a paso:
```
g++ -Wall -c resources.cpp
g++ -I.\ -Wall -c divide-and-conquer.cpp
g++ -Wall -o divide-and-conquer.exe divide-and-conquer.o resources.o
```