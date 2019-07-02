#include <TimerOne.h>

#include <Wire.h>
#include <MultiFuncShield.h>

void setup() {
  Timer1.initialize();
  MFS.initialize(&Timer1);
  beep();
}

int pos = 0;
bool reverse = false;

int lvl = 0;
int speeds[8] = {300, 250, 200, 100, 50, 25, 20, 15};


void loop() {
  drawField();
  setLvlIndicator();

  switch(getPressedButton()) {
    case 1:
      delay(500);
      if(pos == 1) {
        writeText("GOAL");
        lvl++;
        pos = 2;
        delay(1000);
        char lvlTxt[4] = {'L', 'U', 'L', ' '};
        lvlTxt[3] = lvl+'0';
        writeText(lvlTxt);
        delay(1000);
        return;
      }
      break;
    case 2:
      if(lvl>0) lvl--;
      break;
    case 3:
      lvl++;
      break;
  }

  delay(speeds[lvl]);
}

void drawField() {
  char field[] = {' ', ' ', ' ', ' '};
  field[pos] = 'o';
  writeText(field);
  if(reverse) {
    pos--;
    if(pos == 0) reverse = false;
  } else {
    pos++;
    if(pos == 3) reverse = true;
  }
}

void setLvlIndicator() {
  if(lvl < 4) {
    for(int i = 0; i < 4; i++) {
      setLed(i+1, i==lvl);
    }
  } else {
    for(int i = 0; i < 4; i++) {
      setLed(i+1, true);
    }
  }
}

int getPressedButton() {
  byte btn = MFS.getButton();

  if (btn)
  {
    byte buttonNumber = btn & B00111111;
    byte buttonAction = btn & B11000000;

    if(buttonAction == BUTTON_PRESSED_IND) {
      MFS.beep(1);
      return buttonNumber;
    }
    return 0;
  }
}
////////////////////
// Util functions //
////////////////////

void beep() {
  MFS.beep(1, 5, 1, 1, 50);
}

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
