#include "control_ascensor.h"
#include "hardware.h"

#define PERIODO_MS 10

void app_main(void) {
    Control control;
    uint32_t ultimo_ciclo;

    hardware_inicializar();
    inicializar_control(&control);
    ultimo_ciclo = hardware_tiempo_ms();

    while (1) {
        uint32_t ahora = hardware_tiempo_ms();

        if ((uint32_t)(ahora - ultimo_ciclo) >= PERIODO_MS) {
            ultimo_ciclo = ahora;
            Entradas entradas = hardware_leer_entradas();
            actualizar_control(&control, &entradas);
            hardware_escribir_salidas(&control.salidas);
        }
    }
}
