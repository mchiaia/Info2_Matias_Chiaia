#pragma once

// Asignacion  para ESP32-S3-DevKitC-1.
// Los pulsadores se conectan entre GPIO y GND; se usa pull-up interno.
//


#define PIN_SENSOR_1 4
#define PIN_SENSOR_2 5
#define PIN_SEGURIDAD 6
#define PIN_PEDIDO_PISO_0 7
#define PIN_PEDIDO_PISO_1 8
#define PIN_PEDIDO_PISO_2 9

#define PIN_LED_SUBIR 10
#define PIN_LED_BAJAR 11
#define PIN_LED_PISO_0 12
#define PIN_LED_PISO_1 13
#define PIN_LED_PISO_2 14

// Los sensores de piso y los pulsadores son activos en nivel bajo.
#define ENTRADA_ACTIVA_NIVEL_BAJO 1
