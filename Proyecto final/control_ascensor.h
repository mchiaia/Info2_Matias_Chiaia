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
} Entradas;
typedef struct Salidas {
    bool subir;
    bool bajar;
    int piso;
} Salidas;
typedef enum Estado {
    INICIO,
    SINCRONIZANDO,
    ESPERA,
    SUBIENDO,
    BAJANDO,
    CANTIDAD_ESTADOS
} Estado;
typedef struct Control {
    Estado estado;
    int piso_actual; // -1 mientras todavia no se conoce el piso.
    int destino;    // -1 cuando no hay un pedido guardado.
    bool calibrado;
    bool pedidos_anteriores[CANTIDAD_PISOS];
    DetectorPisos detector;
    EventoPiso evento_piso;
    Salidas salidas;
} Control;
void inicializar_control(Control *control);
Estado actualizar_control(Control *control, const Entradas *entradas);
const char *nombre_estado(Estado estado);
Estado inicio(Control *control, const Entradas *entradas);
Estado sincronizando(Control *control, const Entradas *entradas);
Estado espera(Control *control, const Entradas *entradas);
Estado subiendo(Control *control, const Entradas *entradas);
Estado bajando(Control *control, const Entradas *entradas);
#endif
