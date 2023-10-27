#ifndef M702_H
#define M702_H

#include "Arduino.h"

class M702 {
    public:
    M702(HardwareSerial& serial);
    void begin();
    bool readSensor();

    // Valores obtenidos del sensor
    int CO2;
    int CH2O;
    int TVOC;
    int PM25;
    int PM10;
    float temperatura; // La temperatura en grados Celsius
    float humedad;     // La humedad en porcentaje

    private:
    HardwareSerial* _serial;
    void parseData(uint8_t buffer[], size_t length);
    uint8_t calculateChecksum(uint8_t buffer[], size_t length);
};

#endif //M702_H