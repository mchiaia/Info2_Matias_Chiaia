#ifndef DETECTOR_PISOS_H
#define DETECTOR_PISOS_H
#include <stdbool.h>

typedef enum EventoPiso {
    SIN_EVENTO_PISO,
    REFERENCIA_PISO_CERO,
    SUBIO_UN_PISO,
    BAJO_UN_PISO
} EventoPiso;
typedef enum FaseDetectorPisos {
    DETECTOR_ESPERA_NEUTRO,
    DETECTOR_ESPERA_PRIMER_SENSOR,
    DETECTOR_VIO_SENSOR_1,
    DETECTOR_VIO_SENSOR_2,
    DETECTOR_ESPERA_REARME
} FaseDetectorPisos;
typedef struct DetectorPisos {
    FaseDetectorPisos fase;
    bool inicializado;
    bool sensor1_anterior;
    bool sensor2_anterior;
} DetectorPisos;
void inicializar_detector_pisos(DetectorPisos *detector);
EventoPiso actualizar_detector_pisos(DetectorPisos *detector, bool sensor1, bool sensor2);
#endif
