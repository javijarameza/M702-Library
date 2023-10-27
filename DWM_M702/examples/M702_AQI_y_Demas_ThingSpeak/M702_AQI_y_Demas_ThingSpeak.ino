// SendToThingSpeak.ino
#include "M702.h"
#include <WiFi.h>
#include <ThingSpeak.h>

// Configuración de WiFi
const char* ssid = "tu_SSID";
const char* password = "tu_PASSWORD";

// Configuración de ThingSpeak
const char* thingSpeakServer = "api.thingspeak.com";
unsigned long channelID = TU_NUMERO_DE_CANAL;
const char* writeAPIKey = "TU_API_KEY";

HardwareSerial mySerial(2);
M702 mySensor(mySerial);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  mySensor.begin();

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  ThingSpeak.begin(client);
}

void loop() {
  if (mySensor.readSensor()) {
    // Enviar datos a ThingSpeak
    ThingSpeak.setField(1, mySensor.CO2);
    ThingSpeak.setField(2, mySensor.CH2O);
    ThingSpeak.setField(3, mySensor.TVOC);
    ThingSpeak.setField(4, mySensor.PM25);
    ThingSpeak.setField(5, mySensor.PM10);
    ThingSpeak.setField(6, mySensor.temperatura);
    ThingSpeak.setField(7, mySensor.humedad);
    int aqiPM25 = calculateAQI(mySensor.PM25, 0);
    int aqiPM10 = calculateAQI(mySensor.PM10, 1);
    ThingSpeak.setField(8, max(aqiPM25, aqiPM10));  // AQI

    int x = ThingSpeak.writeFields(channelID, writeAPIKey);
    if(x == 200) {
      Serial.println("Actualización de canal exitosa.");
    }
    else {
      Serial.println("Problema al actualizar el canal. Código de error HTTP " + String(x));
    }
  } else {
    Serial.println("Error al leer del sensor!");
  }

  delay(20000);  // Esperar 20 segundos antes de la próxima lectura y envío de datos
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

