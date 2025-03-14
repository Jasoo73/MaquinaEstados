#include <Arduino.h>
#include <ClosedCube_HDC1080.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <ESP8266WiFi.h>

static const int RXPin = 2, TXPin = 0;

static const uint32_t GPSBaud = 9600;

const char* ssid = "UPBWiFi";       
const char* password = "";    
const char* serverIP = "54.211.24.118";  

double latitud = 6.2455678;
double longitud = -75.467888;
double temperatura = 23;
int id = 1001;

TinyGPSPlus gps;

WiFiClient client;
// The serial connection to the GPS device​

SoftwareSerial ss(RXPin, TXPin);

ClosedCube_HDC1080 sensor;

static void smartDelay(unsigned long ms);

void setup() {
  sensor.begin(0x40);
  Serial.begin(115200);

  delay(10);
  pinMode(A0, INPUT);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.println("Connecting to");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
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

// Función para enviar los datos al servidor Flask
void sendthingspeak() {
  String PostData = "";
  Serial.println("Datos para enviar:");
  PostData = String("id=" + String(id)+"; temperatura =" + String(temperatura,7) +"; longitud="+ String(longitud,7)+"; latitud="+ String(latitud,7));
  // Prepara la solicitud HTTP POST
  Serial.println(PostData);
  if (client.connect(serverIP, 80))
  {
    Serial.println("conectado");
    client.println("POST /sensor_send_data HTTP/1.1\n");
    // poner la dirección IP del servidor
    client.println("Host: 192.168.0.101\n");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
  }
  else 
    {
        Serial.println("error de conexion");
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
