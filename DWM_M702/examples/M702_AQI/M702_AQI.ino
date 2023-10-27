// AQICalculation.ino
#include "M702.h"

HardwareSerial mySerial(2);
M702 mySensor(mySerial);

void setup() {
  Serial.begin(115200);
  mySensor.begin();
}

void loop() {
  if (mySensor.readSensor()) {
    int aqiPM25 = calculateAQI(mySensor.PM25, 0);
    int aqiPM10 = calculateAQI(mySensor.PM10, 1);
    int aqi = max(aqiPM25, aqiPM10);  // El AQI es el máximo de los dos AQIs
    Serial.print("AQI: ");
    Serial.println(aqi);
  } else {
    Serial.println("Error al leer del sensor!");
  }

  delay(2000);
}

int calculateAQI(float conc, int pollutant) {
  float C_low, C_high, I_low, I_high;
  // Definir los rangos basados en el contaminante (0 para PM2.5, 1 para PM10)
  if (pollutant == 0) {  // PM2.5
    if (conc <= 12.0) {
      C_low = 0.0; C_high = 12.0; I_low = 0; I_high = 50;
    } else if (conc <= 35.4) {
      C_low = 12.1; C_high = 35.4; I_low = 51; I_high = 100;
    } else if (conc <= 55.4) {
      C_low = 35.5; C_high = 55.4; I_low = 101; I_high = 150;
    } else if (conc <= 150.4) {
      C_low = 55.5; C_high = 150.4; I_low = 151; I_high = 200;
    } else if (conc <= 250.4) {
      C_low = 150.5; C_high = 250.4; I_low = 201; I_high = 300;
    } else {
      C_low = 250.5; C_high = 500.4; I_low = 301; I_high = 500;
    }
  } else {  // PM10
    if (conc <= 54) {
      C_low = 0.0; C_high = 54; I_low = 0; I_high = 50;
    } else if (conc <= 154) {
      C_low = 55; C_high = 154; I_low = 51; I_high = 100;
    } else if (conc <= 254) {
      C_low = 155; C_high = 254; I_low = 101; I_high = 150;
    } else if (conc <= 354) {
      C_low = 255; C_high = 354; I_low = 151; I_high = 200;
    } else if (conc <= 424) {
      C_low = 355; C_high = 424; I_low = 201; I_high = 300;
    } else {
      C_low = 425; C_high = 604; I_low = 301; I_high = 500;
    }
  }

  // Calcular el AQI
  return ((I_high - I_low) / (C_high - C_low)) * (conc - C_low) + I_low;
}

