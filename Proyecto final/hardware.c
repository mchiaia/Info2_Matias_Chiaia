#include "hardware.h"

#include "config_hardware.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_timer.h"

#include <stddef.h>
#include <stdint.h>

static const gpio_num_t pines_entradas[] = {
    PIN_SENSOR_1,      PIN_SENSOR_2,      PIN_SEGURIDAD,
    PIN_PEDIDO_PISO_0, PIN_PEDIDO_PISO_1, PIN_PEDIDO_PISO_2,
};

static const gpio_num_t pines_salidas[] = {
    PIN_LED_SUBIR, PIN_LED_BAJAR, PIN_LED_PISO_0, PIN_LED_PISO_1, PIN_LED_PISO_2,
};

#define TIEMPO_REBOTE_MS 30

typedef struct {
    bool estable;
    bool candidata;
    uint32_t desde_ms;
} FiltroEntrada;

static FiltroEntrada filtros[6];
static bool filtros_iniciados = false;
static FiltroEntrada filtro_seguridad;

static bool entrada_activa(gpio_num_t pin) {
    return gpio_get_level(pin) == 0;
}

static bool filtrar(FiltroEntrada *filtro, bool lectura, uint32_t ahora_ms) {
    if (lectura != filtro->candidata) {
        filtro->candidata = lectura;
        filtro->desde_ms = ahora_ms;
    }
    if (lectura != filtro->estable && ahora_ms - filtro->desde_ms >= TIEMPO_REBOTE_MS) {
        filtro->estable = lectura;
    }
    return filtro->estable;
}

static void escribir(gpio_num_t pin, bool encendido) {
	gpio_set_level(pin, encendido);
}

void hardware_inicializar(void) {
    // Se dejan todas las salidas apagadas antes de configurar entradas.
    gpio_config_t config_salidas = {0};
    config_salidas.pin_bit_mask = 0;
    for (size_t i = 0; i < sizeof(pines_salidas) / sizeof(pines_salidas[0]); i++) {
        config_salidas.pin_bit_mask |= (1ULL << pines_salidas[i]);
    }
    config_salidas.mode = GPIO_MODE_OUTPUT;
    config_salidas.pull_up_en = GPIO_PULLUP_DISABLE;
    config_salidas.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config_salidas.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&config_salidas));
    for (size_t i = 0; i < sizeof(pines_salidas) / sizeof(pines_salidas[0]); i++) {
        escribir(pines_salidas[i], false);
    }

    gpio_config_t config_entradas = {0};
    config_entradas.pin_bit_mask = 0;
    for (size_t i = 0; i < sizeof(pines_entradas) / sizeof(pines_entradas[0]); i++) {
        config_entradas.pin_bit_mask |= (1ULL << pines_entradas[i]);
    }
    config_entradas.mode = GPIO_MODE_INPUT;
    config_entradas.pull_up_en = GPIO_PULLUP_ENABLE;
    config_entradas.pull_down_en = GPIO_PULLDOWN_DISABLE;
    config_entradas.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&config_entradas));
}

Entradas hardware_leer_entradas(void) {
    const uint32_t ahora_ms = hardware_tiempo_ms();
    if (!filtros_iniciados) {
        for (size_t i = 0; i < 6; ++i) {
            filtros[i].candidata = entrada_activa(pines_entradas[i]);
            filtros[i].desde_ms = ahora_ms;
        }
        filtro_seguridad.candidata = entrada_activa(PIN_SEGURIDAD);
        filtro_seguridad.desde_ms = ahora_ms;
        filtros_iniciados = true;
    }

    Entradas entradas = {0};
    // En el arranque todas las entradas necesitan 30 ms para confirmarse.
    // Luego, seguridad abre de inmediato y requiere 30 ms para habilitarse.
    bool seguridad_raw = entrada_activa(PIN_SEGURIDAD);
    if (!seguridad_raw) {
        filtro_seguridad.estable = false;
        filtro_seguridad.candidata = false;
        filtro_seguridad.desde_ms = ahora_ms;
    }
    entradas.seguridad = filtrar(&filtro_seguridad, seguridad_raw, ahora_ms);

    bool lecturas[6];
    for (size_t i = 0; i < 6; ++i) {
        lecturas[i] = filtrar(&filtros[i], entrada_activa(pines_entradas[i]), ahora_ms);
    }
    entradas.sensor1 = lecturas[0];
    entradas.sensor2 = lecturas[1];
    entradas.pedidos[0] = lecturas[3];
    entradas.pedidos[1] = lecturas[4];
    entradas.pedidos[2] = lecturas[5];
    return entradas;
}

void hardware_escribir_salidas(const Salidas *salidas) {
    // SEG abierto apaga las ordenes de movimiento.
    const bool seguridad_habilitada = entrada_activa(PIN_SEGURIDAD);
    bool subir = salidas->subir && !salidas->bajar && seguridad_habilitada;
    bool bajar = salidas->bajar && !salidas->subir && seguridad_habilitada;

    // Primero se apagan las dos direcciones; luego se activa como maximo una.
    escribir(PIN_LED_SUBIR, false);
    escribir(PIN_LED_BAJAR, false);
    escribir(PIN_LED_SUBIR, subir);
    escribir(PIN_LED_BAJAR, bajar);

    bool piso_valido = salidas->piso >= 0 && salidas->piso < 3;
    escribir(PIN_LED_PISO_0, piso_valido && salidas->piso == 0);
    escribir(PIN_LED_PISO_1, piso_valido && salidas->piso == 1);
    escribir(PIN_LED_PISO_2, piso_valido && salidas->piso == 2);
}

uint32_t hardware_tiempo_ms(void) {
    return (uint32_t)(esp_timer_get_time() / 1000);
}
