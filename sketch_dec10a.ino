
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte mySnake[5][8] = 
{
{ B00000,
  B00000,
  B00011,
  B01111,
  B11000,
  B00000,
  B00000,
},
{ B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
},
{ B00000,
  B00000,
  B00000,
  B11000,
  B11111,
  B00111,
  B00000,
},
{ B00000,
  B00000,
  B01110,
  B11011,
  B11111,
  B01110,
  B00000,
},
{ B00000,
  B00000,
  B00000,
  B01000,
  B10000,
  B01000,
  B00000,
}
};

int key=0, readKeyValue;
int score = 0;
int lvl = 0;

void startGame(){
  
  lcd.clear();
  lcd.setCursor(0,0);
  for(int i=0; i<5; i++){
    lcd.createChar(i, mySnake[i]);
    lcd.setCursor(i,0);
    lcd.write(byte(i));
  }
  lcd.setCursor(0, 1); 
  lcd.print("SNAKE");
  score=0;
  
  lvl=1;
  lcd.setCursor(7, 0); 
  lcd.print("SELECT");
  lcd.setCursor(7, 1); 
  lcd.print("lvl");
  lcd.setCursor(11, 1); 
  lcd.print(lvl);
}

void gameOver(){

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GAME OVER");
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(score);
}

int evaluateButton(int x) {
  int result = 0;
  if (x < 50) {
    result = 5; //right
  } else if (x < 195) {
    result = 1; //up
  } else if (x < 380) {
    result = 2; //down
  }else if (x < 580) {
    result = 3; //left
  } else if (x < 790) {
    result = 4; //select
  }
  return result;
}



void setup() {
  
  lcd.begin(16, 2); 
  startGame();
  
}


void loop() {
  
  readKeyValue=analogRead(0);
  key=evaluateButton(readKeyValue);
  if(key==1){
    delay(700);
    lvl++;
    lcd.setCursor(11, 1); 
    lcd.print(lvl);
  } 
  

  readKeyValue=analogRead(0);
  key=evaluateButton(readKeyValue);
  if(key==4){
    delay(700);
    lcd.clear();
  } 
}
