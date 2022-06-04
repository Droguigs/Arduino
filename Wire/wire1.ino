#include <Wire.h>

#define MEU_ENDERECO 1
#define OUTRO_ENDERECO 2

void setup() {
  Wire.begin(MEU_ENDERECO);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

bool aguardando_mensagem;

void espera_mensagem() {
  aguardando_mensagem = true;
  while (aguardando_mensagem) { delay(100); }
}

void envia_mensagem() {
  delay(random(300, 700));
  Wire.beginTransmission(OUTRO_ENDERECO); // transmit to device #1
  Wire.write("y is ");        // sends five bytes
  Wire.write(y);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  y++;
}

void loop() {
  envia_mensagem();
  espera_mensagem();
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  for (int count = 1; count < howMany; count++)
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
  aguardando_mensagem = false;
}
