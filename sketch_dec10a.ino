
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
bool maze[2][2][16];
long int time=0; 
long int timeNow=0; 
bool gameStarted=false;
bool gameOver=false;
bool screen[16][80];
int food_c,food_r;

struct NodeT
{
  int row;
  int column;
  int dir; //0 - sus, 1 - jos, 2 - dreapta, 3 - stanga
  struct NodeT *next;
}; 
typedef NodeT node;

node *head, *tail;

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

void drawGame()
{
    int k = 0;
    int check = 0;
    byte m[8];
    maze[1][0][0]=true;
    maze[1][1][5]=true;
    maze[1][0][10]=true;
    maze[1][1][15]=true;
    if (!gameOver)
    {
        screen[food_r][food_c] = true;
        for (int r = 0; r < 2; r++)
        {
            for (int c = 0; c < 16; c++)
            {
                check = 0;
                for (int i = 0; i < 8; i++)
                {
                    byte b = B00000;
                    if (screen[r * 8 + i][c * 5 + 0]) { 
                        b += B10000; 
                        check = 1; 
                    }
                    if (screen[r * 8 + i][c * 5 + 1]) { 
                        b += B01000; 
                        check = 1; 
                    }
                    if (screen[r * 8 + i][c * 5 + 2]) { 
                        b += B00100; 
                        check = 1; 
                    }
                    if (screen[r * 8 + i][c * 5 + 3]) { 
                        b += B00010; 
                        check = 1; 
                    }
                    if (screen[r * 8 + i][c * 5 + 4]) { 
                        b += B00001; 
                        check = 1; 
                    }
                    m[i] = b;
                }
                if (check == 1)
                {
                    lcd.createChar(k, m);
                    lcd.setCursor(c, r);
                    lcd.write(byte(k));
                    k++;
                }
                else
                {
                    lcd.setCursor(c, r);
                    if (maze[lvl][r][c]) {
                        lcd.write(255);
                    }
                    else {
                        lcd.write(254);
                    }
                }
            }
        }
    }
}

void createSnake(int n)
{
  for (int i=0;i<16;i++)
    for (int j=0;j<80;j++)
      screen[i][j] = false;
    
  node *p, *q;
  tail = (node*)malloc(sizeof(node));
  tail->row = 7;
  tail->column = 35;
  tail->dir = 2;
  q = tail;
  screen[tail->row][tail->column] = true;
  for (int i = 0; i < n-1; i++) 
  {
    p = (node*)malloc(sizeof(node));
    p->row = q->row;
    p->column = q->column+1;
    screen[p->row][p->column] = true;
    p->dir = q->dir;
    q->next = p;
    q = p;
  }
  if (n>1)
  {
    p->next = NULL;
    head  = p;
  }
  else 
  {
    tail->next = NULL;
    head = tail;
  }
}

void createFood()
{

  maze[1][0][0]=true;
  maze[1][1][5]=true;
  maze[1][0][10]=true;
  maze[1][1][15]=true;
  node *p;
  p = tail;
  bool food = true;
  while (food)
  {
    food_r = random(16);
    food_c = random(80);
    food = false;
    if (maze[lvl][food_r / 8][food_c / 5]){
      food=true;
    }
    while (p->next != NULL && !food) {
      if (p->row == food_r && p->column == food_c){ 
         food = true;
      }
      p = p->next;
    }
  } 
}



void gameOverFunction(){

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GAME OVER");
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(score);
}



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
  createSnake(5);
  time = 0;
}


void setup() {
  
  lcd.begin(16, 2); 
  startGame();
  
}

void loop() {
  
  if (!gameStarted && !gameOver){  
     readKeyValue=analogRead(0);
     key=evaluateButton(readKeyValue); 
     
       if (key==1 && lvl<2) lvl++;
       if (key==2 && lvl>1) lvl--;
       if (key==4) 
       {
         lvl--;
         lcd.clear();
         gameStarted = true;
         createFood();      
       }
       else
       {
         lcd.setCursor(11, 1); 
         lcd.print(lvl);
       }
     
   }
 
  if (gameStarted && !gameOver){ 
     readKeyValue=analogRead(0);
     key=evaluateButton(readKeyValue);
     if (key==5 && head->dir!=3){
      head->dir = 2;
     }
     if (key==1 && head->dir!=1){
      head->dir = 0;
     }
     if (key==2 && head->dir!=0){
      head->dir = 1;
     }
     if (key==3 && head->dir!=2){
      head->dir = 3;
     }     
        drawGame();
  }

}
