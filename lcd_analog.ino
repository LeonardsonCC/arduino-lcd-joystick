#include <LiquidCrystal.h>

// Analog constants
const int LEFT = 0;
const int UP = 1;
const int DOWN = 2;
const int RIGHT = 3;
const int BUTTON = 4;
const int NOTHING = 99;

// Setup components
int eixo_X= A0;
int eixo_Y = A1;
int botao = 8;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


// alphabet
char letters[27] = {' ', 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

int currLetter = 0;
int currPos = 0;
int currLine = 0;
int screen[2][16] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(botao, INPUT_PULLUP);

  lcd.clear();
  lcd.setCursor(0, 0);
  char *row_updated = get_row_updated();
  lcd.print(*row_updated);
}

int last_press = -1;
void loop()
{  
  int pressing = check_joystick_direction();

  if (pressing == last_press) {
    return;
  } else {
    last_press = pressing;
  }
  
  switch (pressing) {
    case LEFT:
      currPos--;
      if (currPos < 0) {
        currPos = 15;
      }
      currLetter = screen[currLine][currPos];
      break;
    case RIGHT:
      currPos++;
      if (currPos > 15) {
        currPos = 0;
      }
      currLetter = screen[currLine][currPos];
      break;
    case DOWN:
      currLetter--;
      if (currLetter < 0) {
        currLetter = 26;
      }
      lcd.setCursor(currPos, currLine);
      lcd.print(letters[currLetter]);
      screen[currLine][currPos] = currLetter;
      break;
    case UP:
      currLetter++;
      if (currLetter > 26) {
        currLetter = 0;
      }
      lcd.setCursor(currPos, currLine);
      lcd.print(letters[currLetter]);
      screen[currLine][currPos] = currLetter;
      break;
    case BUTTON:
      if (currLine == 1) {
        currLine = 0;
      } else {
        currLine = 1;
      }
      break;
    case NOTHING:
      lcd.setCursor(currPos, currLine);
      lcd.blink();
  }
  delay(100);
}

char *get_row_updated() {
  char row[16];
  for(int i=0; i < sizeof(screen[currLine]); i++) {
    row[i] = letters[screen[currLine][i]];
  }
  return row;
}

// Check if what the joystick is doing
int check_joystick_direction() {
   if((analogRead(eixo_X)) == 0){
        return UP;
    }else{
          if((analogRead(eixo_X)) == 1023){
              return DOWN;
          }else{
                if((analogRead(eixo_Y)) == 0){
                  return RIGHT;
                }else{
                      if((analogRead(eixo_Y)) == 1023){
                          return LEFT;
                      }else{
                            if(digitalRead(botao) == LOW){
                               return BUTTON;
                            }  
                      }
                }
          }
    }
    return NOTHING;
}
