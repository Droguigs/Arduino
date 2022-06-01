//MODIFICAÇÕES LUAN: DECIMAL PARA BINARIO, BINARIO PARA DECIMAL, DECIMAL PARA OCTAL
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <math.h>

//Funcs initialization
void keyEntry();
void decimalCalculator();
void calculator();
void decimalExpression();
void binaryCalculator();
void octalCalculator();
void decimalToBinaryCalculator();
void decimalToOctalCalculator();
void binaryToDecimalCalculator();
void binaryToOctalCalculator();
void octalToBinaryCalculator();
void octalToDecimalCalculator();
int calculaDecimalParaBinario(int n);
int calculaBinarioParaDecimal(int n);
int decimalToOctalCalculator(int n);
void print(int number);
void changeMode();
long SecondNumber();

LiquidCrystal lcd(A4, A5, 3, 2, 1, 0);

long first = 0;
int firstLength = 0;
long second = 0;
int secondLength = 0;
double total = 0;

int lcdPointer = 0;

char key;
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','+'},
  {'4','5','6','-'},
  {'7','8','9','*'},
  {'C','0','=','/'}
};
byte rowPins[ROWS] = {11,10,9,8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7,6,5,4}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

enum {
  DECIMAL,
  BINARY,
  OCTAL,
  DECIMAL_TO_BINARY,
  DECIMAL_TO_OCTAL,
  BINARY_TO_DECIMAL,
  BINARY_TO_OCTAL,
  OCTAL_TO_DECIMAL,
  OCTAL_TO_BINARY
} mode = DECIMAL ;

void setup() {
  lcd.begin(16, 2);               // start lcd
  lcd.setCursor(0,0);
}

void loop() {
  switch(mode) {
    case DECIMAL: decimalCalculator(); break;
    case BINARY: binaryCalculator(); break;
    case OCTAL: octalCalculator(); break;
    case DECIMAL_TO_BINARY: decimalToBinaryCalculator(); break;
    case DECIMAL_TO_OCTAL: decimalToOctalCalculator(); break;
    case BINARY_TO_DECIMAL: binaryToDecimalCalculator(); break;
    case BINARY_TO_OCTAL: binaryToOctalCalculator(); break;
    case OCTAL_TO_DECIMAL: octalToDecimalCalculator(); break;
    case OCTAL_TO_BINARY: octalToBinaryCalculator(); break;
  }
}

void keyEntry() {
  key = keypad.getKey();
  if (mode == DECIMAL || mode == DECIMAL_TO_BINARY || mode == DECIMAL_TO_OCTAL) {
    key = keypad.getKey();
  } else if (mode == BINARY || mode == BINARY_TO_DECIMAL || mode == BINARY_TO_OCTAL) {
    while(!(key == '0' || key == '1') &&
          (key != 'C') &&
          (key != '=') &&
          (key != '*') &&
          (key != '-') &&
          (key != '+') &&
          (key != '/')) {
      key = keypad.getKey();
    }
  } else {
    while(!(key >= '0' && key <= '7') &&
          (key != 'C') &&
          (key != '=') &&
          (key != '*') &&
          (key != '-') &&
          (key != '+') &&
          (key != '/')) {
      key = keypad.getKey();
    }
  }
}

void decimalCalculator() {
  char stop = 'C';
  lcd.print("Calculadora dec");
  while (stop == 'C') {
    decimalExpression(stop);
  }
  changeMode();
}

void calculator() {
  keyEntry();
  switch(key) {
  case '0' ... '9':
    lcd.setCursor(lcdPointer,1);
    if (firstLength < 3) {
      first = first * 10 + (key - '0');
      firstLength++;
      print(first);
    }
    break;

  case '+':
    first = (total != 0 ? total : first);
    lcd.print("+");
    second = SecondNumber(); // get the collected the second number
    total = first + second;
    print(total);
    first = 0, second = 0; // reset values back to zero for next use
    break;

  case '-':
    first = (total != 0 ? total : first);
    // lcd.setCursor(0,1);
    lcd.print("-");
    second = SecondNumber();
    total = first - second;
    lcd.setCursor(0,1);
    print(total);
    first = 0, second = 0;
    break;

  case '*':
    first = (total != 0 ? total : first);
    // lcd.setCursor(0,1);
    lcd.print("*");
    second = SecondNumber();
    total = first * second;
    lcd.setCursor(0,1);
    print(total);
    first = 0, second = 0;
    break;

  case '/':
    first = (total != 0 ? total : first);
    // lcd.setCursor(0,1);
    lcd.print("/");
    second = SecondNumber();
    lcd.setCursor(0,1);

    second == 0 ? lcd.print("Invalid") : total = (float)first / (float)second;

    print(total);
    first = 0, second = 0;
    break;

  case 'C':
    total = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Calculadora dec");
    break;

  case '=':
    stop = '=';
    break;
  }
}

void decimalExpression(char &stop) {
  keyEntry();
  switch(key) {
  case '0' ... '9': // This keeps collecting the first value until a operator is pressed "+-*/"
    lcd.setCursor(lcdPointer,1);
    if (firstLength < 3) {
      first = first * 10 + (key - '0');
      firstLength++;
      print(first);
    }
    break;

  case '+':
    first = (total != 0 ? total : first);
    lcd.print("+");
    second = SecondNumber(); // get the collected the second number
    total = first + second;
    print(total);
    first = 0, second = 0; // reset values back to zero for next use
    break;

  case '-':
    first = (total != 0 ? total : first);
    // lcd.setCursor(0,1);
    lcd.print("-");
    second = SecondNumber();
    total = first - second;
    lcd.setCursor(0,1);
    print(total);
    first = 0, second = 0;
    break;

  case '*':
    first = (total != 0 ? total : first);
    // lcd.setCursor(0,1);
    lcd.print("*");
    second = SecondNumber();
    total = first * second;
    lcd.setCursor(0,1);
    print(total);
    first = 0, second = 0;
    break;

  case '/':
    first = (total != 0 ? total : first);
    // lcd.setCursor(0,1);
    lcd.print("/");
    second = SecondNumber();
    lcd.setCursor(0,1);

    second == 0 ? lcd.print("Invalid") : total = (float)first / (float)second;

    print(total);
    first = 0, second = 0;
    break;

  case 'C':
    total = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Calculadora dec");
    break;

  case '=':
    stop = '=';
    break;
  }
}

void binaryCalculator() {
  keyEntry();
  switch(key) {
  case '0' ... '1': // This keeps collecting the first value until a operator is pressed "+-*/"
    lcd.setCursor(0,0);
    first = first * 10 + (key - '0');
    lcd.print(first);
    break;

  case '+':
    first = (total != 0 ? total : first);
    // lcd.setCursor(0,1);
    lcd.print("+");
    second = SecondNumber(); // get the collected the second number
    total = first + second;
    lcd.setCursor(0,1);
    lcd.print(total);
    first = 0, second = 0; // reset values back to zero for next use
    break;

  case '-':
    first = (total != 0 ? total : first);
    // lcd.setCursor(0,1);
    lcd.print("-");
    second = SecondNumber();
    total = first - second;
    lcd.setCursor(0,1);
    lcd.print(total);
    first = 0, second = 0;
    break;

  case '*':
    first = (total != 0 ? total : first);
    // lcd.setCursor(0,1);
    lcd.print("*");
    second = SecondNumber();
    total = first * second;
    lcd.setCursor(0,1);
    lcd.print(total);
    first = 0, second = 0;
    break;

  case '/':
    first = (total != 0 ? total : first);
    // lcd.setCursor(0,1);
    lcd.print("/");
    second = SecondNumber();
    lcd.setCursor(0,1);

    second == 0 ? lcd.print("Invalid") : total = (float)first / (float)second;

    lcd.print(total);
    first = 0, second = 0;
    break;

  case 'C':
    total = 0;
    lcd.clear();
    break;
  }
}

// void octalCalculator() {
//
// }


// funções para conversão
int calculaDecimalParaBinario(int n){
  if(n == 0){
    printf("%d", n);

  }else{
    binario(n/2);
    printf("%d", n % 2);
  }

}

int calculaBinarioParaDecimal(int n ){
  if(n < 2) {
    return 1;
  } else{
    return 2 * calculaBinarioParaDecimal(n / 10) + n % 10;
  }

}

int calculaDecimalParaOctal(int n){
   if(n > 8){
     return n;
   }else{
     return calculaDecimalParaOctal(n/8) * 10 + n % 8;
   }

}

void decimalToBinaryCalculator() {
  keyEntry();
  switch(key) {
  // case '0' ... '1': // This keeps collecting the first value until a operator is pressed "+-*/"
  //   lcd.setCursor(0,0);
  //   first = first * 10 + (key - '0');
  //   lcd.print(first);
  //   break;

  case '=':
    // first = (total != 0 ? total : first);
    lcd.print("dec to bin");
    second = SecondNumber();
    total = calculaDecimalParaBinario(second);
    lcd.setCursor(0,1);
    lcd.print(total);
    second = 0;
    break;


  case 'C':
    total = 0;
    lcd.clear();
    break;
  }
}

void decimalToOctalCalculator() {
  keyEntry();
  switch(key) {
  // case '0' ... '1': // This keeps collecting the first value until a operator is pressed "+-*/"
  //   lcd.setCursor(0,0);
  //   first = first * 10 + (key - '0');
  //   lcd.print(first);
  //   break;

  case '=':
    // first = (total != 0 ? total : first);
    lcd.print("dec to octal");
    second = SecondNumber();
    total = calculaDecimalParaOctal(second);
    lcd.setCursor(0,1);
    lcd.print(total);
    second = 0;
    break;


  case 'C':
    total = 0;
    lcd.clear();
    break;
  }
}

void binaryToDecimalCalculator() {
  keyEntry();
  switch(key) {
  // case '0' ... '1': // This keeps collecting the first value until a operator is pressed "+-*/"
  //   lcd.setCursor(0,0);
  //   first = first * 10 + (key - '0');
  //   lcd.print(first);
  //   break;

  case '=':
    // first = (total != 0 ? total : first);
    lcd.print("bin to dec");
    second = SecondNumber();
    total = calculaBinarioParaDecimal(second);
    lcd.setCursor(0,1);
    lcd.print(total);
    second = 0;
    break;


  case 'C':
    total = 0;
    lcd.clear();
    break;
  }
}

void binaryToOctalCalculator() {

}

void octalToBinaryCalculator() {

}

void octalToDecimalCalculator() {

}

void print(int number) {
  lcd.setCursor(lcdPointer,1);
  lcd.print(number);
}

void changeMode() {
  switch(mode) {
    case DECIMAL: mode = BINARY; break;
    case BINARY: mode = OCTAL; break;
    case OCTAL: mode = DECIMAL_TO_BINARY; break;
    case DECIMAL_TO_BINARY: mode = DECIMAL_TO_OCTAL; break;
    case DECIMAL_TO_OCTAL: mode = BINARY_TO_DECIMAL; break;
    case BINARY_TO_DECIMAL: mode = BINARY_TO_OCTAL; break;
    case BINARY_TO_OCTAL: mode = OCTAL_TO_DECIMAL; break;
    case OCTAL_TO_DECIMAL: mode = OCTAL_TO_BINARY; break;
    case OCTAL_TO_BINARY: mode = DECIMAL; break;
  }
}

long SecondNumber() {
  lcdPointer++;
  while(secondLength < 3) {
    key = keypad.getKey();
    if(key >= '0' && key <= '9') {
      second = second * 10 + (key - '0');
      secondLength++;
      lcd.setCursor(lcdPointer+firstLength,1);
      lcd.print(second);
    }

    if(key == '=') {
      break;  //return second;
    }
  }
  lcdPointer = firstLength + secondLength + 2;
  lcd.print("=");
  return second;
}
