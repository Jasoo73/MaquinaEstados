#include <Arduino.h>
#include <ClosedCube_HDC1080.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

static const int RXPin = 2, TXPin = 0;

static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

// The serial connection to the GPS device​

SoftwareSerial ss(RXPin, TXPin);

ClosedCube_HDC1080 sensor;

static void smartDelay(unsigned long ms);

void setup() {
  sensor.begin(0x40);
  Serial.begin(115200);
}

void loop() {
  char estado = 'a';
  double temperatura = 0;
  double humedad = 0;

  switch (estado) {
        case 'a':
            for (int i = 0; i < 10; i++){
              
              smartDelay(10);

              // Output raw GPS data to the serial monitor​

              temperatura += sensor.readTemperature();
              humedad += sensor.readHumidity();

            }
            //contador ++;
            estado = 'b';

        case 'b':
            temperatura = temperatura/10;
            humedad = humedad / 10;

            smartDelay(10);

            estado = 'c';

        case 'c':
            for (int i = 0; i < 5; i++){
              smartDelay(10);
            }
            estado = 'a';
    }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
