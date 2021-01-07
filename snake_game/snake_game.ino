
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

void freeList()
{
  node *p,*q;
  p = tail;
  while (p!=NULL)
  {
    q = p;
    p = p->next;
    free(q);
  }
  head = tail = NULL;
}

void gameOverFunction(){

  lcd.clear();
  freeList();
  lcd.setCursor(0,0);
  lcd.print("GAME OVER");
  lcd.setCursor(0,1);
  lcd.print("Score: ");
  lcd.print(score);
  delay(1000);
}

void moveFunction()
{
  node* body;
  body = tail;
  screen[body->row][body ->column] = false;
  while (body->next != NULL)
  {
    body->row = body->next->row;
    body->column = body->next->column;
    body->dir = body->next->dir;
    body = body->next;
  }
    
  if (head->dir==0){ 
    head->row = head->row - 1;
  }
  if (head->dir==1){ 
    head->row =  head->row + 1; 
  }
  if (head->dir==2){ 
    head->column = head->column + 1; 
  }
  if (head->dir==3){ 
    head->column = head->column - 1; 
  }
  
  if (head->column > 79){ 
    head->column = 0;
  }
  if (head->column < 0){ 
    head->column = 79;
  }
  if (head->row > 15){ 
    head->row = 0;
  }
  if (head->row < 0){ 
    head->row = 15;
  }
  
  node *p;
  p = tail;
  while (p != head && !gameOver)
  {
    if (p->row == head->row && p->column == head->column){
      gameOver = true;
    }
    p = p->next;
  }
  
   if (maze[lvl][head->row / 8][head->column / 5]){ 
    gameOver = true; 
  }
  
  if (gameOver){
    gameOverFunction();
  }
  else
  {
    screen[head->row][head->column] = true;
    if (head->row == food_r && head->column == food_c)
    {
      score++;
      node* eaten;
      eaten = (node*)malloc(sizeof(node));
      eaten->row = tail->row;
      eaten->column = tail->column;
      eaten->dir = tail->dir;
      eaten->next = tail;
      tail = eaten;
      createFood();
    }
  }
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
     timeNow = millis();
     if (timeNow - time > 100){
        drawGame();
        moveFunction();
        time = millis();
     }
  }

   if(gameOver){ 
      readKeyValue=analogRead(0);
      key=evaluateButton(readKeyValue);
      if (key == 4){
            startGame();
         }
    }
}
