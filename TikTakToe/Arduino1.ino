#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Keypad.h>

#define MY_ADRRESS 1
#define OTHER_ADDRESS 2

// Neopixel first pin and size
#define PIN 4
#define NUMPIXELS 4

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

//Transmitting
char c;

//sorteio
int sorteio = 2;

//pontuação dos players.
int pontuacao_jogador1 = 0;
int pontuacao_jogador2 = 0;

//Posicoes
char celulas[] = {'a','a','a','a','a','a','a','a','a'};

bool jogador1 = false;
bool jogador2 = false;

volatile int vitoria = 0;

bool waiting_for_message = false;

//Functions declare
void startSorteio();
char escolhePosicao();
void checkPosicao(char pos);
void marcarPosicao(char porta);
void joga_1();
void joga_2();
void alterna_vez();
void vez();
void mostrarPontuacao();
void checaVitoria();
void pararExecucao();
void receiveEvent(int howMany);

void setup() {
  Serial.begin(9600);
  Wire.begin(MY_ADRRESS);
  Wire.onReceive(receiveEvent);

  pixels0.begin();
  pixels0.Color(0,0,255);
  pixels1.begin();
  pixels1.Color(0,0,255);
  pixels2.begin();
  pixels2.Color(0,0,255);

  pinMode(13, OUTPUT);
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

  for(i = 1; valido && i<=9; i++) {
    if (pos == (i+48)) {
      if(celulas[i-1] == 'f')
      	valido = false;
      else
        i = 10;
    }
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
  Serial.println("Vez do jogador 2: ");
  while (waiting_for_message) { delay(1000); } // espera o restante dos bytes
}

void joga_1() {
  Serial.println("Vez do jogador 1: ");
  checkPosicao(escolhePosicao());
  // envia
  checaVitoria();
  alterna_vez();
 }

//jogador 1
void alterna_vez() {
  if (jogador1) {
    key = NULL;
    jogador1 = false;
    jogador2 = true;
  } else {
    key = NULL;
    jogador1 = true;
    jogador2 = false;
  }
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
  Wire.beginTransmission(OTHER_ADDRESS);
  //horizontal
  if((celulas[0] == '1') && (celulas[1] == '2') && (celulas[2] == '3')) {
    //horizontal 1
    vitoria = 1;
    Serial.println("Vitoria do jogador 1 na horizontal 1");
    Wire.write(key);
    Wire.write('f');
    pontuacao_jogador2++;
    mostrarPontuacao();
  	pararExecucao();
  } else if((celulas[3] == '4') && (celulas[4] == '5') && (celulas[5] == '6')) {
    //horizontal 2
    vitoria = 1;
    Serial.println("Vitoria do jogador 1 na horizontal 2");
    Wire.write(key);
    Wire.write('f');
    pontuacao_jogador2++;
    mostrarPontuacao();
    pararExecucao();
  } else if((celulas[6] == '7') && (celulas[7] == '8') && (celulas[8] == '9')) {
    //horizontal 3
    vitoria = 1;
    Serial.println("Vitoria do jogador 1 na horizontal 3");
    Wire.write(key);
    Wire.write('f');
    pontuacao_jogador2++;
    mostrarPontuacao();
    pararExecucao();
  } else if((celulas[0] == '1') && (celulas[3] == '4') && (celulas[6] == '7')) {
    //vertical 1
    vitoria = 1;
    Serial.println("Vitoria do jogador 1 na vertical 1");
    Wire.write(key);
    Wire.write('f');
    pontuacao_jogador2++;
    mostrarPontuacao();
    pararExecucao();
  } else if((celulas[1] == '2') && (celulas[4] == '5') && (celulas[7] == '8')) {
    //vertical 2
    vitoria = 1;
    Serial.println("Vitoria do jogador 1 na vertical 2");
    Wire.write(key);
    Wire.write('f');
    pontuacao_jogador2++;
    mostrarPontuacao();
    pararExecucao();
  } else if((celulas[2] == '3') && (celulas[5] == '6') && (celulas[8] == '9')) {
    //vertical 3
    vitoria = 1;
    Serial.println("Vitoria do jogador 1 na vertical 3");
    Wire.write(key);
    Wire.write('f');
    pontuacao_jogador2++;
    mostrarPontuacao();
    pararExecucao();
  } else if((celulas[0] == '1') && (celulas[4] == '5') && (celulas[8] == '9')) {
    //diagonal 1
    vitoria = 1;
    Serial.println("Vitoria do jogador 1 na diagonal 1");
    Wire.write(key);
    Wire.write('f');
    pontuacao_jogador2++;
    mostrarPontuacao();
    pararExecucao();
  } else if((celulas[6] == '7') && (celulas[4] == '5') && (celulas[2] == '3')) {
    //diagonal 2
    vitoria = 1;
    Serial.println("Vitoria do jogador 1 na diagonal 2");
    Wire.write(key);
    Wire.write('f');
    pontuacao_jogador2++;
    mostrarPontuacao();
    pararExecucao();
  } else if((celulas[0] == '1' || celulas[0] == 'f') &&
            (celulas[1] == '2' || celulas[1] == 'f') &&
            (celulas[2] == '3' || celulas[2] == 'f') &&
            (celulas[3] == '4' || celulas[3] == 'f') &&
            (celulas[4] == '5' || celulas[4] == 'f') &&
            (celulas[5] == '6' || celulas[5] == 'f') &&
            (celulas[6] == '7' || celulas[6] == 'f') &&
            (celulas[7] == '8' || celulas[7] == 'f') &&
            (celulas[8] == '9' || celulas[8] == 'f')) {
    vitoria = 1;
    Serial.println("Deu velha!");
    Wire.write(key);
    Wire.write('v');
    mostrarPontuacao();
    pararExecucao();
  } else {
    Wire.write(key);
    Wire.write('n');
  }
  Wire.endTransmission();
}

//Recebe os dados da Wire
void receiveEvent(int howMany) {
  int pos = Wire.read();
  switch (pos) {
    case '1':
      Serial.println("Jogador 2 escolheu - (1,1)");
      celulas[0] = 'f';
      pixels0.setPixelColor(0, pixels0.Color(200, 0, 0));
      pixels0.show();
      break;
    case '2':
      Serial.println("Jogador 2 escolheu - (1,2)");
      celulas[1] = 'f';
      pixels0.setPixelColor(1, pixels0.Color(200, 0, 0));
      pixels0.show();
      break;
    case '3':
      Serial.println("Jogador 2 escolheu - (1,3)");
      celulas[2] = 'f';
      pixels0.setPixelColor(2, pixels0.Color(200, 0, 0));
      pixels0.show();
      break;
    case '4':
      Serial.println("Jogador 2 escolheu - (2,1)");
      celulas[3] = 'f';
      pixels1.setPixelColor(0, pixels1.Color(200, 0, 0));
      pixels1.show();
      break;
    case '5':
      Serial.println("Jogador 2 escolheu - (2,2)");
      celulas[4] = 'f';
      pixels1.setPixelColor(1, pixels1.Color(200, 0, 0));
      pixels1.show();
      break;
    case '6':
      Serial.println("Jogador 2 escolheu - (2,3)");
      celulas[5] = 'f';
      pixels1.setPixelColor(2, pixels1.Color(200, 0, 0));
      pixels1.show();
      break;
    case '7':
      Serial.println("Jogador 2 escolheu - (3,1)");
      celulas[6] = 'f';
      pixels2.setPixelColor(0, pixels2.Color(200, 0, 0));
      pixels2.show();
      break;
    case '8':
      Serial.println("Jogador 2 escolheu - (3,2)");
      celulas[7] = 'f';
      pixels2.setPixelColor(1, pixels2.Color(200, 0, 0));
      pixels2.show();
      break;
    case '9':
      Serial.println("Jogador 2 escolheu - (3,3)");
      celulas[8] = 'f';
      pixels2.setPixelColor(2, pixels2.Color(200, 0, 0));
      pixels2.show();
      break;
  }

  char c = Wire.read();    // receive byte as an integer
  if (c == 'f') {
    vitoria = 1;
    Serial.println("Vitoria do jogador 2");
    pontuacao_jogador2++;
    mostrarPontuacao();
    pararExecucao();
  } else if (c == 'v') {
    vitoria = 1;
    Serial.println("DEU VELHA!");
    mostrarPontuacao();
    pararExecucao();
  } else {
    alterna_vez();
  }
}

void pararExecucao() {
  waiting_for_message = false;
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
