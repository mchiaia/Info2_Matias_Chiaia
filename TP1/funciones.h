//
// Created by matia on 25/5/2026.
//

#ifndef INFO2_MATIAS_CHIAIA_FUNCIONES_H
#define INFO2_MATIAS_CHIAIA_FUNCIONES_H

#define CANT_ARTICULOS 60

#define SUCURSAL_1 0
#define SUCURSAL_2 1
#define SUCURSAL_3 2
//#define TOTAL 4 //No es necesario

typedef struct {
        char descripcion[90];
        int cantidad_sucursal[3]; // sucursal 1 2 3
        int total;
}articulos_t;

void Cargar_fichas(articulos_t *articulos);
void Impresion_fichas(articulos_t *articulos);
void Ordenamiento(articulos_t *articulos);
void Impresion_fichas_ordenadas(articulos_t *articulos);

#endif //INFO2_MATIAS_CHIAIA_FUNCIONES_H
