	#ifndef CONTROL_ASCENSOR_H
#define CONTROL_ASCENSOR_H
#include "detector_pisos.h"
#include <stdbool.h>
#define CANTIDAD_PISOS 3

typedef struct Entradas {
    bool sensor1;
    bool sensor2;
    bool seguridad;
    bool pedidos[CANTIDAD_PISOS];
} Entradas_t;
typedef struct Salidas {
    bool subir;
    bool bajar;
    int piso;
} Salidas_t;
typedef enum Estado {
    INICIO,
    SINCRONIZANDO,
    ESPERA,
    SUBIENDO,
    BAJANDO,
    CANTIDAD_ESTADOS
} Estado_t;
typedef struct Control {
    Estado_t estado;
    int piso_actual; // -1 mientras todavia no se conoce el piso.
    int destino;    // -1 cuando no hay un pedido guardado.
    bool calibrado;
    bool pedidos_anteriores[CANTIDAD_PISOS];
    DetectorPisos_t detector;
    EventoPiso_t evento_piso;
    Salidas_t salidas;
} Control_t;
void inicializar_control(Control_t *control);
Estado_t actualizar_control(Control_t *control, const Entradas_t *entradas);
const char *nombre_estado(Estado_t estado);
Estado_t inicio(Control_t *control, const Entradas_t *entradas);
Estado_t sincronizando(Control_t *control, const Entradas_t *entradas);
Estado_t espera(Control_t *control, const Entradas_t *entradas);
Estado_t subiendo(Control_t *control, const Entradas_t *entradas);
Estado_t bajando(Control_t *control, const Entradas_t *entradas);
#endif
