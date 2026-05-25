# Trabajo Practico 1 - Informatica 2

Este proyecto es una refactorizacion del codigo de Resolucion_Final_Informatica_1.

La consigna principal es separar el programa en distintos archivos, usando funciones y estructuras.

## Archivos

- `main.c`: contiene el flujo principal del programa.
- `funciones.c`: contiene el desarrollo de las funciones.
- `funciones.h`: contiene las constantes, la estructura y los prototipos.
- `CMakeLists.txt`: configuracion para compilar el proyecto con CMake/CLion.

## Funcionamiento

El programa permite cargar articulos, indicar la cantidad correspondiente a una sucursal, mostrar los datos cargados y luego ordenarlos de mayor a menor segun el total.

Cada articulo se guarda con la estructura `articulos_t`, que contiene:

- descripcion
- cantidades por sucursal
- total

## Compilacion

```powershell
gcc main.c funciones.c -o tp1.exe
```

También se puede abrir el proyecto desde CLion y ejecutar el target `TP1`.


## Observaciones

- El Cmake se copió de otro proyecto personal y se modifico para este.