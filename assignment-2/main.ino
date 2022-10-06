#define Button1 12 //RED
#define Button2 11 //YELLOW
#define Button3 10 //GREEN
#define LED1 5 //RED
#define LED2 4 //YELLOW
#define LED3 3 //GREEN
//RED
unsigned long LedShow1 = 0;
unsigned char LedState1 = LOW;
unsigned char ButtonState1 = LOW;
unsigned char LastButtonState1 = LOW;
//YELLOW
unsigned long LedShow2 = 0;
unsigned char LedState2 = LOW;
unsigned char LedStateBlink = LOW;
unsigned char ButtonState2 = LOW;
unsigned char LastButtonState2 = LOW;
//GREEN
unsigned long LedShow3 = 0;
unsigned char LedState3 = LOW;
unsigned char ButtonState3 = LOW;
unsigned char LastButtonState3 = LOW;

unsigned long DBbutton1 = 0, DBbutton3 = 0, DBbutton2 = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(Button1,INPUT_PULLUP);
  pinMode(Button2,INPUT);
  pinMode(Button3,INPUT_PULLUP);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
}

void loop()
{
  
  //RED
  ButtonState1 = digitalRead(Button1);

  if(LedState1 == HIGH)
  {
    if(millis()-LedShow1 > 3000)
    {
      digitalWrite(LED1,LOW);
      LedState1 = LOW;
    }
  }
  
  if(ButtonState1 != LastButtonState1){
    LastButtonState1 = ButtonState1;
    if((ButtonState1 == LOW) && (LedState1 == LOW)){
      if (millis() - DBbutton1 >= 300){      
        digitalWrite(LED1,HIGH);
        LedState1 = HIGH;
        LedShow1 = millis();  
        DBbutton1 = millis(); 
      }  
    }
    else if((ButtonState1 == LOW) && (LedState1 == HIGH)){
        if (millis() - DBbutton1 >= 300){
        digitalWrite(LED1,LOW);
        LedState1 = LOW;
        DBbutton1 = millis(); 
       }
    }
  }


 //YELLOW
  ButtonState2 = digitalRead(Button2);

  if(LedState2 == HIGH)
  {
    LedStateBlink=LedState2;
    if(millis()-LedShow2>500){
      LedStateBlink=LOW;
    }
    if(millis()-LedShow2>1000){
      LedStateBlink=HIGH;
    }
    if(millis()-LedShow2>1500){
      LedStateBlink=LOW;
    }
    if(millis()-LedShow2>2000){
      LedStateBlink=LOW;
      digitalWrite(LED2,LOW);
      LedState2=LOW;
    }
    digitalWrite(LED2,LedStateBlink);
      
  }
  if(ButtonState2 != LastButtonState2){
   LastButtonState2 = ButtonState2;
    if((ButtonState2 == LOW) && (LedState2 == LOW) && (LedState1 == LOW) && (LedState3 == LOW)){
      if (millis() - DBbutton2 >= 300){     
        digitalWrite(LED2,HIGH);
        LedState2 = HIGH;
        LedShow2 = millis();  
        DBbutton2 = millis(); 
      }
    }
    else if((ButtonState2 == LOW) && (LedState2 == HIGH)){
       if (millis() - DBbutton2 >= 300){   
        digitalWrite(LED2,LOW);
        LedState2 = LOW;
        DBbutton2 = millis(); 
       }
    }
  }

  //GREEN
  
  ButtonState3 = digitalRead(Button3);
  if(LedState3 == HIGH)
  {
    if(millis()-LedShow3 > 3000)
    {
      digitalWrite(LED3,LOW);
      LedState3 = LOW;
    }
  }
  
  if(ButtonState3 != LastButtonState3){
    LastButtonState3 = ButtonState3;
    if((ButtonState3 == LOW) && (LedState3 == LOW) && (LedState1 == LOW)){
       if (millis() - DBbutton3 >= 300){   
      digitalWrite(LED3,HIGH);
      LedState3 = HIGH;
      LedShow3 = millis();    
      DBbutton3 = millis(); 
       }
    }
    else if((ButtonState3 == LOW) && (LedState3 == HIGH)){
        if (millis() - DBbutton3 >= 300){
          digitalWrite(LED3,LOW);
          LedState3 = LOW;
          DBbutton3 = millis(); 
        }
    }
  }
}
