#include "M702.h"

HardwareSerial mySerial(2); // Usar UART2 (puede cambiar según su configuración de hardware)
M702 mySensor(mySerial);

void setup() {
    Serial.begin(115200);
    mySensor.begin();
}

void loop() {
    if (mySensor.readSensor()) {
        Serial.print("CO2: ");
        Serial.print(mySensor.CO2);
        Serial.print(" ppm, CH2O: ");
        Serial.print(mySensor.CH2O);
        Serial.print(" g/m3, TVOC: ");
        Serial.print(mySensor.TVOC);
        Serial.print(" g/m3, Temp: ");
        Serial.print(mySensor.temperatura);
        Serial.print(" ºC, Humed: ");
        Serial.print(mySensor.humedad);
        Serial.print(" %, PM2.5: ");
        Serial.print(mySensor.PM25);
        Serial.print(" g/m3, PM10: ");
        Serial.print(mySensor.PM10); 
        Serial.println(" g/m3");
    } else {
        Serial.println("Error al leer del sensor!");
      }

    delay(2000); // Esperar antes de la próxima lectura
}