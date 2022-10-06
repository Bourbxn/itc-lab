#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Time Variables
unsigned long timeStop = millis();
unsigned long timeStop2 = millis();

//Button Variables
#define buttonLeft 3
#define buttonRight 4
#define buttonAction 8
bool buttonLeftState;
bool buttonRightState;
bool buttonActionState;

//OLED Variables
#define screenWidth 128 
#define screenHeight 64  
Adafruit_SSD1306 OLED(screenWidth, screenHeight, &Wire);

//Buzzer Variables
#define buzzer 12
bool buzzerBeep = false;
int roundBuzzer = 0;
bool buzzerGameOver = false;
int roundBuzzerGO = 0;

//Game Variables
#define shipSize screenHeight/4
#define pongSize 4
#define speedMove 2
#define pongSpeedMove 2
int moveLeft = 0;
int moveLeftMax = -24;
int moveRight = 0;
int moveRightMax = 24;
int pongMoveLeft = 0;               //Left
int poveMoveLeftMax = -24;
int pongMoveRight = 0;              //Right
int pongMoveRightMax = 24;
int pongMoveUp = 0;                 //Up
int pongMoveUpMax = -24;
int pongMoveDown = 0;             //Down
int pongMoveDownMax = 24;
int pongMoveState = 1;
int lastPongMoveState = 1;
int pongPosX = 0;
int pongPosY = 0;

//Action
bool restartGame = false;

void setup() {
  Serial.begin(9600);
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(buttonLeft,INPUT);
  pinMode(buttonRight,INPUT);
  pinMode(buzzer,OUTPUT);
}

void loop() {
  //Render
  render();
  
  //Time update
  if(millis()-timeStop>=1){
    timeStop=millis();
    if(pongMoveState == 0 && restartGame==true){
      gameOverRender();
    }
    if(pongMoveState ==1){
      restartGame=false;
      pongMoveUp+=pongSpeedMove;
      pongMoveLeft+=pongSpeedMove;
      pongPosX -=pongSpeedMove;
      pongPosY +=pongSpeedMove;
    }
    else if(pongMoveState==2){
      pongMoveUp+=pongSpeedMove;
      pongMoveRight+=pongSpeedMove;
      pongPosX+=pongSpeedMove;
      pongPosY+=pongSpeedMove;
    }
    else if(pongMoveState==3){
      pongMoveDown+=pongSpeedMove;
      pongMoveRight+=pongSpeedMove;
      pongPosY-=pongSpeedMove;
      pongPosX+=pongSpeedMove;
    }
    else if(pongMoveState==4){
      pongMoveDown+=pongSpeedMove;
      pongMoveLeft+=pongSpeedMove;
      pongPosY-=pongSpeedMove;
      pongPosX-=pongSpeedMove;
    }
    else if(pongMoveState==5){
      buzzerGameOver = true;
    }

    //Buzzer 
    //Bounce platform
    if(buzzerBeep == true){
      tone(buzzer,261);
      roundBuzzer++;
      if(roundBuzzer==2){
        buzzerBeep = false;
        roundBuzzer = 0;
      }
    }
    else if(buzzerBeep==false){
      noTone(buzzer);
    }
    //Game Over
    if(buzzerGameOver==true){
      if(roundBuzzerGO>=0 && roundBuzzerGO<=4){
        tone(buzzer,998);
      }
      else if(roundBuzzerGO>=5 && roundBuzzerGO<=9){
        tone(buzzer,880);
      }
      else if(roundBuzzerGO>=10 && roundBuzzerGO<=14){
        tone(buzzer,998);
      }
      else if(roundBuzzerGO>=15 && roundBuzzerGO<=20){
        tone(buzzer,698);
      }
      roundBuzzerGO++;
      if(roundBuzzerGO==20){
        noTone(buzzer);
        buzzerGameOver=false;
        roundBuzzerGO=0;
        pongMoveState = 0;
        lastPongMoveState = 0;
      }
    }
  }
  //Update pong move 
  if(pongPosX<-24 && pongPosY<=110 && lastPongMoveState ==1){
    pongMoveState = 2;
    lastPongMoveState=2;
  }
  else if (pongPosX>24 && pongPosY<=110 && lastPongMoveState==2){
    pongMoveState=1;
    lastPongMoveState=1;
  }
  else if(pongPosY>110 && lastPongMoveState==1){
    pongMoveState=4;
    lastPongMoveState=4;
  }
  else if(pongPosY>110 && lastPongMoveState==2){
    pongMoveState=3;
    lastPongMoveState=3;
  }
  else if(pongPosX<-24 && pongPosY<=110 && lastPongMoveState==4){
    pongMoveState=3;
    lastPongMoveState=3;
  }
  else if(pongPosX>24 && pongPosY<=110 && lastPongMoveState==3){
    pongMoveState=4;
    lastPongMoveState=4;
  }

  //Check pong bounce platform
  if((screenWidth-18+pongMoveDown-pongMoveUp==screenWidth-18)&&
    ((screenHeight/2)-(pongSize/2)+pongMoveLeft-pongMoveRight>=(screenHeight/2)-(shipSize/2)+moveLeft-moveRight)&&
    ((screenHeight/2)-(pongSize/2)+pongMoveLeft-pongMoveRight<=(screenHeight/2)-(shipSize/2)+moveLeft-moveRight+16)){
      buzzerBeep=true;
    if(lastPongMoveState==4){
      pongMoveState=1;
      lastPongMoveState=1;
    }
    else if(lastPongMoveState==3){
      pongMoveState = 2;
      lastPongMoveState = 2;
    }
  }
  else if(screenWidth-18+pongMoveDown-pongMoveUp>screenWidth && pongMoveState != 0){
    pongMoveState = 5;
    lastPongMoveState = 5;
  }
  
  //Update input
  //Left input
  buttonLeftState = digitalRead(buttonLeft);
  if(buttonLeftState == HIGH && moveLeft-moveRight <24)
  {
    moveLeft+=speedMove; 
    delay(10); 
  }
  //Right input
  buttonRightState = digitalRead(buttonRight);
  if(buttonRightState == HIGH && moveLeft-moveRight >-24)
  {
    moveRight+=speedMove; 
    delay(10); 
  }
  //Action input
  buttonActionState = digitalRead(buttonAction);
  if(buttonActionState==HIGH && pongMoveState==0){
    restartGame = true;
    delay(10);
  }
}

//Functions
void render()
{
  OLED.clearDisplay();
  drawPlatform(screenWidth-16,(screenHeight/2)-(shipSize/2)+moveLeft-moveRight);
  drawCircle(screenWidth-18,(screenHeight/2)-(pongSize/2));
  OLED.display(); 
  delay(10); 
}

void gameOverRender()
{
  pongMoveState=1;
  lastPongMoveState=1;
  pongMoveLeft=0;
  pongMoveRight=0;
  pongMoveUp=0;
  pongMoveDown=0;
  pongPosX=0;
  pongPosY=0;
  moveLeft=0;
  moveRight=0;
  render();
}

void drawCircle(int y,int x)
{
  OLED.drawRoundRect(y+pongMoveDown-pongMoveUp,x+pongMoveLeft-pongMoveRight,pongSize/2,pongSize,100,WHITE);
  OLED.fillRoundRect(y+pongMoveDown-pongMoveUp,x+pongMoveLeft-pongMoveRight,pongSize/2,pongSize,100,WHITE);
}

void drawPlatform(int y,int x)
{
  OLED.drawFastVLine(y,x,shipSize,WHITE);
}
