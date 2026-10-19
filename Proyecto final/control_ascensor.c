#include "control_ascensor.h"
typedef Estado (*FuncionEstado)(Control *, const Entradas *);
static void detener_motor(Control *control) {
    control->salidas.subir = false;
    control->salidas.bajar = false;
}
static void actualizar_piso(Control *control) {
    if (control->evento_piso == REFERENCIA_PISO_CERO) {
        control->piso_actual = 0;
        control->calibrado = true;
    } else if (control->evento_piso == SUBIO_UN_PISO && control->estado == SUBIENDO &&
               control->piso_actual < CANTIDAD_PISOS - 1) {
        control->piso_actual++;
    } else if (control->evento_piso == BAJO_UN_PISO && control->estado == BAJANDO &&
               control->piso_actual > 0) {
        // El piso cero se confirma con ambos sensores, no con un par comun.
        if (control->piso_actual != 1 || control->destino != 0) {
            control->piso_actual--;
        }
    }
}
void inicializar_control(Control *control) {
    int piso;
    control->estado = INICIO;
    control->piso_actual = -1;
    control->destino = -1;
    control->calibrado = false;
    for (piso = 0; piso < CANTIDAD_PISOS; piso++) {
        control->pedidos_anteriores[piso] = false;
    }
    inicializar_detector_pisos(&control->detector);
    control->evento_piso = SIN_EVENTO_PISO;
    control->salidas.subir = false;
    control->salidas.bajar = false;
    control->salidas.piso = -1;
}
Estado actualizar_control(Control *control, const Entradas *entradas) {
    int piso;
    static const FuncionEstado tabla_estados[CANTIDAD_ESTADOS] = {
        inicio, sincronizando, espera, subiendo, bajando};
    control->evento_piso = actualizar_detector_pisos(
        &control->detector, entradas->sensor1, entradas->sensor2);
    actualizar_piso(control);
    control->estado = tabla_estados[control->estado](control, entradas);
    // recordar los botones evita aceptar al llegar un pedido mantenido en viaje.
    for (piso = 0; piso < CANTIDAD_PISOS; piso++) {
        control->pedidos_anteriores[piso] = entradas->pedidos[piso];
    }
    control->salidas.piso = control->piso_actual;
    return control->estado;
}
Estado inicio(Control *control, const Entradas *entradas) {
    (void)entradas;
    detener_motor(control);
    control->destino = -1;
    return SINCRONIZANDO;
}
Estado sincronizando(Control *control, const Entradas *entradas) {
    detener_motor(control);
    if (control->calibrado) {
        control->destino = -1;
        return ESPERA;
    }
    if (entradas->seguridad) {
        control->salidas.bajar = true;
    }
    return SINCRONIZANDO;
}
Estado espera(Control *control, const Entradas *entradas) {
    int piso;
    detener_motor(control);
    if (control->destino == -1) {
        for (piso = 0; piso < CANTIDAD_PISOS; piso++) {
            bool flanco = entradas->pedidos[piso] && !control->pedidos_anteriores[piso];
            if (flanco && piso != control->piso_actual) {
                control->destino = piso;
                break;
            }
        }
    }
    if (control->destino == -1) {
        return ESPERA;
    }
    if (control->destino == control->piso_actual) {
        control->destino = -1;
        return ESPERA;
    }
    if (!entradas->seguridad) {
        return ESPERA;
    }
    return control->destino > control->piso_actual ? SUBIENDO : BAJANDO;
}
Estado subiendo(Control *control, const Entradas *entradas) {
    detener_motor(control);
    if (control->piso_actual == control->destino) {
        control->destino = -1;
        return ESPERA;
    }
    if (entradas->seguridad) {
        control->salidas.subir = true;
    }
    return SUBIENDO;
}
Estado bajando(Control *control, const Entradas *entradas) {
    detener_motor(control);
    if (control->piso_actual == control->destino) {
        control->destino = -1;
        return ESPERA;
    }
    if (entradas->seguridad) {
        control->salidas.bajar = true;
    }
    return BAJANDO;
}
const char *nombre_estado(Estado estado) {
    static const char *nombres[CANTIDAD_ESTADOS] = {"inicio", "sincronizando", "espera",
                                                    "subiendo", "bajando"};
    if (estado < INICIO || estado >= CANTIDAD_ESTADOS) {
        return "desconocido";
    }
    return nombres[estado];
}
