#include <TimerOne.h>

#include <Wire.h>
#include <MultiFuncShield.h>

void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);
  //writeLongText("Calculator 1.0");
}

int firstNumber = 0;
int secondNumber = 0;
double result = 0;

// 0 = +
// 1 = -
// 2 = *
// 3 = /
byte op = 0;
char opNames[][4] = {
  {'A', 'D', 'D'},
  {'S', 'U', 'B'},
  {'M', 'U', 'L'},
  {'D', 'I', 'V'}
};
// 0 = first number
// 1 = operation
// 2 = second number
// 3 = result
byte state = 0;

int testLED = 10;
void loop() {
  handleButtonPress();
  if(state == 3) {
    doCalc();
  }
  showStateLed();
  showText();
}

void doCalc() {
  switch(op) {
    case 0:
      result = firstNumber + secondNumber;
      break;
    case 1:
      result = firstNumber - secondNumber;
      break;
    case 2:
      result = firstNumber * secondNumber;
      break;
    case 3:
      result = firstNumber / secondNumber;
      break;
  }
}

void showText() {
  switch(state){
    case 0:
      writeNumber(firstNumber);
      break;
    case 1:
      writeText(opNames[op]);
      break;
    case 2:
      writeNumber(secondNumber);
      break;
    case 3:
      writeNumber(result);
      break;
    default:
      writeText("ERROR");
  }
}

void showStateLed() {
  int led = state+1;
  for(int i = 1; i < 5; i++) {
    setLed(i, led==i);
  }
}

void handleButtonPress() {
  byte btn = MFS.getButton();

  if (btn)
  {
    byte buttonNumber = btn & B00111111;
    byte buttonAction = btn & B11000000;

    if(buttonNumber == 1 && buttonAction == BUTTON_PRESSED_IND) {
      switch(state) {
        case 0:
          firstNumber++;
          break;
        case 1:
          if (++op > 3) {
            op = 0;
          }
          break;
        case 2:
          secondNumber++;
          break;
      }
    } else if(buttonNumber == 2 && buttonAction == BUTTON_PRESSED_IND) {
      switch(state) {
        case 0:
          firstNumber--;
          break;
        case 1:
          if (-- op < 0) {
            op = 3;
          }
          break;
        case 2:
          secondNumber--;
          break;
      }
    } else if(buttonNumber == 3 && buttonAction == BUTTON_PRESSED_IND) {
      if(++state > 3) {
        state = 0;
        firstNumber = 0;
        secondNumber = 0;
        result = 0;
        op = 0;
      }
    } 
    
  }
}

////////////////////
// Util functions //
////////////////////

void writeNumber(int value) {
  MFS.write(value);
}

void writeText(char text[]) {
  if(strlen(text) > 4) {
    writeLongText(text);
  } else {
    MFS.write(text); 
  }
}

void writeLongText(char text[]) {
  int len = strlen(text);
  
  char toDisplay[5] = "    ";
  
  for(int i = 0; i < len+4; i++) {
    for(int j = 0; j < 3; j++) {
      toDisplay[j] = toDisplay[j+1];
    }
    toDisplay[3] = text[i];
    MFS.write(toDisplay);
    delay(400);
  }
}

void setLed(int led, boolean on) {
      if(led < 1 || led > 4) return;
      // pass 0 to turn it on??
      analogWrite(14-led, !on ? 255 : 0);
}
