#include "M702.h"
// Definiciones de constantes en la parte superior del archivo
const uint8_t ENCABEZADO_ESPERADO[] = {0x3C, 0x02}; // Valores de encabezado esperados
const size_t BUFFER_ESTANDAR = 17;                  // Tamaño estándar del buffer para los datos del sensor

M702::M702(HardwareSerial& serial) : _serial(&serial) {}

void M702::begin() {
    _serial->begin(9600); // Iniciar comunicación serial con el baud rate del sensor
}

bool M702::readSensor() {
    // Si hay datos disponibles para leer desde el sensor
    if (_serial->available()) {
        uint8_t buffer[BUFFER_ESTANDAR]; // Para almacenar los datos del sensor
        size_t len = _serial->readBytes(buffer, BUFFER_ESTANDAR); // Leer los datos en el buffer

        // Imprimir datos en bruto para depuración
        //Serial.print("Datos en bruto: ");
        //for (size_t i = 0; i < len; ++i) {
        //    Serial.print(buffer[i], HEX); // imprimir en hexadecimal
        //    Serial.print(" "); // espacio entre bytes para claridad
        //}
        //Serial.println(); // nueva línea al final

        if (len == BUFFER_ESTANDAR && buffer[0] == ENCABEZADO_ESPERADO[0] && buffer[1] == ENCABEZADO_ESPERADO[1]) {
            // Verificar si el checksum es correcto
            if (buffer[BUFFER_ESTANDAR - 1] == calculateChecksum(buffer, BUFFER_ESTANDAR -1)) {
                parseData(buffer, BUFFER_ESTANDAR); // Interpretar los datos
                return true;
            } else {
                Serial.println("Error en la verificación del checksum");
            }
        }
    }
    return false; // Falló en la lectura o los datos no son válidos
}

void M702::parseData(uint8_t buffer[], size_t length) {
    // Asumiendo que cada valor está representado por dos bytes y se encuentra en big-endian.
    CO2 =  (buffer[2]  << 8) | buffer[3]; // CO2: bytes 3 y 4
    CH2O = (buffer[4]  << 8) | buffer[5]; // CH2O: bytes 5 y 6
    TVOC = (buffer[6]  << 8) | buffer[7]; // TVOC: bytes 7 y 8
    PM25 = (buffer[8]  << 8) | buffer[9]; // PM2.5: bytes 9 y 10
    PM10 = (buffer[10] << 8) | buffer[11];// PM10: bytes 11 y 12
    temperatura = buffer[12] + (buffer[13] / 100.0); // byte 12 (entero) y byte 13 (decimales)
    humedad =     buffer[14] + (buffer[15] / 100.0); // byte 12 (entero) y byte 13 (decimales)
}

uint8_t M702::calculateChecksum(uint8_t buffer[], size_t length) {
    uint8_t checksum = 0;
    for (size_t i = 0; i < length; ++i) {
        checksum += buffer[i]; // Sumando cada byte individual en el buffer
    }
    return checksum; // Este es el valor que debería coincidir con el checksum enviado por el sensor.
}