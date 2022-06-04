// C++ code
//
//#include <stdlib.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Keypad.h>
#include <SoftwareSerial.h>

#define rxPin A4
#define txPin A5

#define PIN 4   // input pin Neopixel is attached to
#define NUMPIXELS 12 // number of neopixels in Ring

SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

Adafruit_NeoPixel pixels0 = Adafruit_NeoPixel(NUMPIXELS, PIN+2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS, PIN+1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const byte ROWS = 3; //three rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'}
};
byte rowPins[ROWS] = {12, 11, 10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7}; //connect to the column pinouts of the keypad

//Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

char key;

//LED
int NUM = -1;

//Transmitting
char c;

//sorteio
int sorteio = 2;

//pontuação dos players.
int pontuacao_jogador1 = 0;
int pontuacao_jogador2 = 0;

//Posicoes
char celulas[] = {};

bool jogador1;
bool jogador2;

volatile int vitoria = 0;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  pixels0.begin();
  pixels0.Color(0,0,255);
  pixels1.Color(0,0,255);
  pixels2.Color(0,0,255);

  pinMode(13, OUTPUT);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
}

//sorteio
void startSorteio() {
  if (sorteio == 2) {
    sorteio = random(0,2);
    delay(1500);

    if (sorteio == 0) {
      jogador1 = true;
      jogador2 = false;
      Serial.println("jogador 1 comeca o jogo");
      joga_1();
    } else if (sorteio ==  1) {
      jogador1 = false;
      jogador2 = true;
      Serial.println("jogador 2 comeca o jogo");
      joga_2();
    }
  }
}

char escolhePosicao() {
  Serial.println("Escolha uma posicao: ");
  key = NO_KEY;
  while (key == NO_KEY) {
    key = keypad.getKey();
  }
  return key;
}

void checkPosicao(char pos) {
  int i = 0;
  bool valido = true;
  while (celulas[i]) {
    if (pos == celulas[i]) {
      valido = false;
    }
    i++;
  }
  if (valido) {
    marcarPosicao(pos);
  } else {
    pos = escolhePosicao();
    checkPosicao(pos);
  }
}

void marcarPosicao(char porta) {
  switch(porta) {
      case '1':
        Serial.println("(1,1)");
        celulas[0] = '1';
        pixels0.setPixelColor(0, pixels0.Color(0, 200, 0));
    	  pixels0.show();
        break;
      case '2':
        Serial.println("(1,2)");
        celulas[1] = '2';
        pixels0.setPixelColor(1, pixels0.Color(0, 200, 0));
        pixels0.show();
        break;
      case '3':
        Serial.println("(1,3)");
        celulas[2] = '3';
        pixels0.setPixelColor(2, pixels0.Color(0, 200, 0));
        pixels0.show();
        break;
      case '4':
        Serial.println("(2,1)");
        celulas[3] = '4';
        pixels1.setPixelColor(0, pixels1.Color(0, 200, 0));
        pixels1.show();
        break;
      case '5':
        Serial.println("(2,2)");
        celulas[4] = '5';
        pixels1.setPixelColor(1, pixels1.Color(0, 200, 0));
        pixels1.show();
        break;
      case '6':
        Serial.println("(2,3)");
        celulas[5] = '6';
        pixels1.setPixelColor(2, pixels1.Color(0, 200, 0));
        pixels1.show();
        break;
      case '7':
        Serial.println("(3,1)");
        celulas[6] = '7';
        pixels2.setPixelColor(0, pixels2.Color(0, 200, 0));
        pixels2.show();
        break;
      case '8':
        Serial.println("(3,2)");
        celulas[7] = '8';
        pixels2.setPixelColor(1, pixels2.Color(0, 200, 0));
        pixels2.show();
        break;
      case '9':
        Serial.println("(3,3)");
        celulas[8] = '9';
        pixels2.setPixelColor(2, pixels2.Color(0, 200, 0));
        pixels2.show();
        break;
  	}
}

void joga_2() {
  // recebe
  while (mySerial.available() == 0) { } // termina com a chegada do primeiro byte
  delay(1000); // espera o restante dos bytes
  int N = mySerial.available(); // número total de bytes recebidos
  int contador = 0;

  while (contador < N-1) { // mySerial.available() > 0
  	contador++;
  	Serial.print('[');
  	Serial.print(contador);
  	Serial.print(']');

  	char c = mySerial.read();
  	Serial.print('[');
  	Serial.print(c);
  	Serial.print(']');
  	Serial.print(" (ainda disponivel: ");
  	Serial.print(mySerial.available());
  	Serial.println(')');
  }

  char charc = mySerial.read();

  if (charc == 's') {
    Serial.println("JOGADOR 1 REPETE A JOGADA");
    joga_1();
  }
  else if (charc >= '0' && charc <= '9') {
    marcarPosicao(charc);
    Serial.println("JOGADOR 1 RECEBEU VALIDO");

    joga_1();
  } else if (charc == 'f') {
    Serial.println("JOGADOR 2 RECEBEU VALIDO");
    vitoria = 1;
    Serial.println("Vitoria do jogador 1");
    pontuacao_jogador1 +=1;
    mostrarPontuacao();
    pararExecucao();
  }
}

void joga_1() {
  Serial.println(" vez do jogador 1: ");
  checkPosicao(escolhePosicao());
  // envia
  mySerial.print(key);
  checaVitoria();
  alterna_vez();
 }

//jogador 1
void alterna_vez() {
  Serial.println("vez do jogador 2: ");
  key = NULL;
  jogador2 = true;
  jogador1 = false;
  vez();
}

//turn
void vez() {
  if(jogador1) {
    joga_1();
  } else if(jogador2) {
    joga_2();
  }
}

void mostrarPontuacao() {
  Serial.print("Pontuacao: ");
  Serial.print(" Jogador 1: ");
  Serial.print(pontuacao_jogador1);
  Serial.print(" - ");
  Serial.print(pontuacao_jogador2);
  Serial.print(" :Jogador 2");
}

// vitoria

void checaVitoria() {
  //horizontal
  if((celulas[0] == '0') && (celulas[1] == '1') && (celulas[2] == '2')) {
    if(pixels.getPixelColor(0)== pixels.getPixelColor(1) &&
       pixels.getPixelColor(0)== pixels.getPixelColor(2)) {
    	vitoria = 1;
    	Serial.println("Vitoria do jogador 2 na horizontal1");

        mySerial.print('f');

        pontuacao_jogador2 +=1;
        mostrarPontuacao();
    	pararExecucao();
    }
  }

  if((celulas[3] == '3') && (celulas[4] == '4') && (celulas[5] == '5')) {
     if(pixels.getPixelColor(4)== pixels.getPixelColor(5) &&
        pixels.getPixelColor(4)== pixels.getPixelColor(6)) {
       //horizontal2
       vitoria = 1;
       Serial.println("Vitoria do jogador 2 na horizontal2");
       mySerial.print('f');
       pontuacao_jogador2 +=1;
       mostrarPontuacao();
       pararExecucao();
     }
  }
  if((celulas[6] == '6') && (celulas[7] == '7') && (celulas[8] == '8'))
  {
     if(pixels.getPixelColor(8)== pixels.getPixelColor(9) && pixels.getPixelColor(8)== pixels.getPixelColor(10))
    {
       //horizontal3
       vitoria = 1;
       Serial.println("Vitoria do jogador 2 na horizontal3");
       mySerial.print('f');
       pontuacao_jogador2 +=1;
       mostrarPontuacao();
       pararExecucao();
     }
  }

  if((celulas[0] == '0') && (celulas[3] == '3') && (celulas[6] == '6')) {
     if(pixels.getPixelColor(0)== pixels.getPixelColor(4) &&
        pixels.getPixelColor(0)== pixels.getPixelColor(8)) {
      //vertical1
      vitoria = 1;
      Serial.println("Vitoria do jogador 2 na vertical1");
      mySerial.print('f');
      pontuacao_jogador2 +=1;
      mostrarPontuacao();
      pararExecucao();
     }
  }

  if((celulas[1] == '1') && (celulas[4] == '4') && (celulas[7] == '7')) {
     if(pixels.getPixelColor(1)== pixels.getPixelColor(5) &&
        pixels.getPixelColor(1)== pixels.getPixelColor(9)) {
      //vertical2
      vitoria = 1;
      Serial.println("Vitoria do jogador 2 na vertical2");
      mySerial.print('f');
      pontuacao_jogador2 +=1;
      mostrarPontuacao();
      pararExecucao();
     }
  }

  if((celulas[2] == '2') && (celulas[5] == '5') && (celulas[8] == '8')) {
     if(pixels.getPixelColor(2)== pixels.getPixelColor(6) &&
        pixels.getPixelColor(2)== pixels.getPixelColor(10)) {
      //vertical3
      vitoria = 1;
      Serial.println("Vitoria do jogador 2 na vertical3");
      mySerial.print('f');
      pontuacao_jogador2 +=1;
      mostrarPontuacao();
      pararExecucao();
     }
  }

  if((celulas[0] == '0') && (celulas[4] == '4') && (celulas[8] == '8')) {
     if(pixels.getPixelColor(0)== pixels.getPixelColor(5) &&
        pixels.getPixelColor(0)== pixels.getPixelColor(10)) {
      //diagonal1
      vitoria = 1;
      Serial.println("Vitoria do jogador 2 na diagonal1");
      mySerial.print('f');
      pontuacao_jogador2 +=1;
      mostrarPontuacao();
      pararExecucao();
     }
  }

  if((celulas[6] == '6') && (celulas[4] == '4') && (celulas[2] == '2')) {
     if(pixels.getPixelColor(8) == pixels.getPixelColor(5) &&
        pixels.getPixelColor(8)== pixels.getPixelColor(2)) {
      //diagonal2
      vitoria = 1;
      Serial.println("Vitoria do jogador 2 na diagonal2");
      mySerial.print('f');
      pontuacao_jogador2 +=1;
      mostrarPontuacao();
      pararExecucao();
     }
  }

  if((celulas[0] == '1') && (celulas[1] == '2') && (celulas[2] == '3') &&
     (celulas[3] == '4') && (celulas[4] == '5') && (celulas[5] == '6') &&
     (celulas[6] == '7') && (celulas[7] == '8') && (celulas[8] == '9')) {
    vitoria = 1;
    Serial.println("Deu velha!");
    mostrarPontuacao();
    pararExecucao();
  }
}

void pararExecucao() {
  while(vitoria == 1);
}

void loop() {
  pixels0.show();
  pixels1.show();
  pixels2.show();
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
  startSorteio();
}
