#define dot 10
#define a 9
#define b 8
#define c 7
#define d 6
#define e 5
#define f 4
#define g 3
#define Button1 2
#define Button2 11
#define ledRed 13
#define ledGreen 12
int randomNumber;
int ButtonState1 = 0;
int LastButtonState1=0;
int ButtonState2=0;
int LastButtonState2 = 0;

boolean number [10][8] = {1,0,0,0,0,0,0,1,  //0
                                          1,1,1,1,0,0,1,1,  //1
                                          0,1,0,0,1,0,0,1,  //2
                                          0,1,1,0,0,0,0,1,  //3
                                          0,0,1,1,0,0,1,1,  //4
                                          0,0,1,0,0,1,0,1,  //5
                                          0,0,0,0,0,1,0,1,  //6
                                          1,1,1,1,0,0,0,1,  //7
                                          0,0,0,0,0,0,0,1,  //8
                                          0,0,1,0,0,0,0,1}; //9

boolean guess [4][8] = {1,0,0,0,0,0,0,0,      //Equal Show
                                     1,0,0,0,0,1,0,0,     //More than
                                     1,0,0,0,1,1,1,0,    //Less than
                                     1,1,1,1,1,1,1,1};    //not Show  

int Button2Show=1;
boolean ForRandom = 1;
int MainRanNum;     //Main number for compare
int CompareNum;     //Compare Number

void setup() {
  Serial.begin(9600);
  for(int i=3;i<=10;i++) pinMode(i,OUTPUT);      //PinMode 7 Segments
  pinMode(Button1,INPUT);
  for(int i=3;i<=10;i++) digitalWrite(i,HIGH);       //PinMode 7 Segments Set not show
  pinMode(ledRed,OUTPUT);
  pinMode(ledGreen,OUTPUT);
}

void loop() {
  randomSeed(analogRead(A0));
  if(ForRandom==1){
    randomNumber = random(1,10);
    MainRanNum=randomNumber;
    ForRandom=!ForRandom;
  }
  //Serial.println(MainRanNum);
  //delay(1000);
  ButtonState1=digitalRead(Button1);
  if((ButtonState1!=LastButtonState1)&&ButtonState1==HIGH){
    Button2Show++;
    if(Button2Show==10) Button2Show=1;
  }
  LastButtonState1=ButtonState1; delay(10);
  CompareNum=Button2Show;
  showNumber(Button2Show);
  //Serial.println(Button2Show);
  
  ButtonState2=digitalRead(Button2);
  if((ButtonState2!=LastButtonState2)&&ButtonState2==HIGH){
    if(CompareNum==MainRanNum){
      for(int j=0;j<5;j++){
        showGuess(0);
        delay(250);
        showGuess(3);
        delay(250);
      }
      ForRandom=!ForRandom;
      Button2Show=1;
    }
    else if(CompareNum>MainRanNum){
      for(int j=0;j<3;j++){
        showGuess(1);
        delay(250);
        showGuess(3);
        delay(250);
      }
    }
    else if(CompareNum<MainRanNum){
      for(int j=0;j<3;j++){
        showGuess(2);
        digitalWrite(ledGreen,HIGH);
        delay(250);
        showGuess(3);
        digitalWrite(ledGreen,LOW);
        delay(250);
      }
    }
  }
  LastButtonState2=ButtonState2; delay(10);
}

void showNumber(int num){
  for(int i=3;i<=10;i++){
    digitalWrite(i, number[num][i-3]);
  }
}

void showGuess(int guessNum){
  for(int i=3;i<=10;i++){
    digitalWrite(i, guess[guessNum][i-3]);
  }
}
