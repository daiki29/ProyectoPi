#include <HardwareSerial.h>

// Definimos el objeto de comunicación serie
HardwareSerial RS485(2); //Puerto Serie 2

#define RX_PIN 16  // Pin RX del ESP32
#define TX_PIN 17  // Pin TX del ESP32
#define RE_PIN 4   // Pin de habilitación del transceptor MAX485
#define DE_PIN 5   // Pin de habilitación de la transmisión del MAX485
#define SLAVE_ID 0x21 // ID del slave


void setup() {
  Serial.begin(9600);
  rs485.begin(9600,SERIAL_8N1,RX_PIN, TX_PIN);
  pinMode(RE_PIN, OUTPUT);
  pinMode(DE_PIN, OUTPUT);
  digitalWrite(RE_PIN, HIGH); // Configura el transceptor MAX485 en modo de recepción
  digitalWrite(DE_PIN, HIGH); // Desactiva la transmisión del MAX485
}

void loop() {
  // Envía un mensaje al slave con ID = 0x21
  digitalWrite(DE_PIN, LOW); // Activa la transmisión del MAX485
  rs485.write(SLAVE_ID); // Envía la dirección del slave en el primer byte del mensaje
  rs485.write("Hola mundo!");
  delay(100);
  digitalWrite(DE_PIN, HIGH); // Desactiva la transmisión del MAX485
}
