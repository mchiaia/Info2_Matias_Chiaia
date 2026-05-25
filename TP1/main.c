//
// Created by matia on 25/5/2026.
//

#include "funciones.h"
#include "stdio.h"

int main(void)
{
    printf("Bienvendio al final de Info 1\n\n");

    /*CARGA de las fichas*/

    Cargar_fichas();

    /* Impresion de fichas cargadas*/

    Impresion_fichas();

    /* Ordenamiento */

    Ordenamiento();

    /* Impresion */

    Impresion_fichas_ordenadas();

    return 0;
}
