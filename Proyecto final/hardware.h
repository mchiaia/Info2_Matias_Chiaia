#ifndef HARDWARE_H
#define HARDWARE_H

#include "control_ascensor.h"
#include <stdint.h>

void hardware_inicializar(void);
Entradas hardware_leer_entradas(void);
void hardware_escribir_salidas(const Salidas *salidas);
uint32_t hardware_tiempo_ms(void);

#endif
