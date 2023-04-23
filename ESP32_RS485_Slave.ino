#define SLAVE_ID 0x21

HardwareSerial rs485(2);  // Crea un objeto de la clase HardwareSerial para el puerto 2

void setup() {
  Serial.begin(9600);
  rs485.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  pinMode(RE_PIN, OUTPUT);
  pinMode(DE_PIN, OUTPUT);
  digitalWrite(RE_PIN, HIGH); // Configura el transceptor MAX485 en modo de recepción
  digitalWrite(DE_PIN, HIGH); // Desactiva la transmisión del MAX485
}

void loop() {
  // Espera a recibir un mensaje en el puerto serial
  while (!Serial.available()) {}

  // Lee el primer byte del mensaje para obtener la dirección del slave
  byte address = Serial.read();

  // Si el mensaje está dirigido al slave con ID = 0x21, lee el resto del mensaje
  if (address == SLAVE_ID) {
    while (rs485.available()) {
      message += char(rs485.read());
    }
  }
}

void serialEvent() {
  // Cuando se recibe un mensaje en el monitor serie, lo envía al bus RS485
  while (Serial.available()) {
    char c = Serial.read();
    rs485.write(c);
  }
}
