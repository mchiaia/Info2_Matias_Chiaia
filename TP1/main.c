//
// Created by matia on 25/5/2026.
//

#include "funciones.h"
#include "stdio.h"

int main(void)
{
    articulos_t articulos[CANT_ARTICULOS] = {0};

    printf("Bienvendio al final de Info 1\n\n");

    /*CARGA de las fichas*/

    Cargar_fichas(articulos);

    /* Impresion de fichas cargadas*/

    Impresion_fichas(articulos);

    /* Ordenamiento */

    Ordenamiento(articulos);

    /* Impresion */

    Impresion_fichas_ordenadas(articulos);

    return 0;
}
