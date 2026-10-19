#include "detector_pisos.h"
void inicializar_detector_pisos(DetectorPisos *detector) {
    detector->fase = DETECTOR_ESPERA_NEUTRO; // ambos sensores libres
    detector->inicializado = false;
    detector->sensor1_anterior = false;
    detector->sensor2_anterior = false;
}
EventoPiso actualizar_detector_pisos(DetectorPisos *detector, bool sensor1,
                                     bool sensor2) {
    EventoPiso evento = SIN_EVENTO_PISO;
    // La marca 11 es exclusiva de cero; se informa una vez al entrar.
    if (sensor1 && sensor2) {
        if (!detector->inicializado || !detector->sensor1_anterior ||
            !detector->sensor2_anterior) {
            evento = REFERENCIA_PISO_CERO;
        }
        detector->fase = DETECTOR_ESPERA_NEUTRO;
        detector->inicializado = true;
        detector->sensor1_anterior = sensor1;
        detector->sensor2_anterior = sensor2;
        return evento;
    }
    if (!detector->inicializado) {
        detector->fase = DETECTOR_ESPERA_NEUTRO;
        detector->inicializado = true;
    }
    switch (detector->fase) {
    case DETECTOR_ESPERA_NEUTRO:
    case DETECTOR_ESPERA_REARME:
        if (!sensor1 && !sensor2) {
            detector->fase = DETECTOR_ESPERA_PRIMER_SENSOR;
        }
        break;
    case DETECTOR_ESPERA_PRIMER_SENSOR:
        if (sensor1 && !sensor2) {
            detector->fase = DETECTOR_VIO_SENSOR_1;
        } else if (!sensor1 && sensor2) {
            detector->fase = DETECTOR_VIO_SENSOR_2;
        }
        break;
    case DETECTOR_VIO_SENSOR_1:
        if (!sensor1 && sensor2) {
            evento = SUBIO_UN_PISO;
            detector->fase = DETECTOR_ESPERA_REARME;
        }
        break;
    case DETECTOR_VIO_SENSOR_2:
        if (sensor1 && !sensor2) {
            evento = BAJO_UN_PISO;
            detector->fase = DETECTOR_ESPERA_REARME;
        }
        break;
    }
    detector->sensor1_anterior = sensor1;
    detector->sensor2_anterior = sensor2;
    return evento;
}
