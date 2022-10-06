void setup() {
  pinMode(4,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,INPUT);

}

void showLED(int  LED){
   if(LED==1){
      pinMode(4,OUTPUT);
      pinMode(7,OUTPUT);
      pinMode(8,INPUT);
      digitalWrite(4,LOW);
      digitalWrite(7,HIGH);
      delay(500);
    }
    else if(LED==2){
      pinMode(4,OUTPUT);
      pinMode(7,OUTPUT);
      pinMode(8,INPUT);
      digitalWrite(4,HIGH);
      digitalWrite(7,LOW);
      delay(500);
    }
    else if(LED==3){
      pinMode(4,INPUT);
      pinMode(7,OUTPUT);
      pinMode(8,OUTPUT);
      digitalWrite(7,LOW);
      digitalWrite(8,HIGH);
      delay(500);
      
    }
    else if(LED==4){
      pinMode(4,INPUT);
      pinMode(7,OUTPUT);
      pinMode(8,OUTPUT);
      digitalWrite(7,HIGH);
      digitalWrite(8,LOW);
      delay(500);
    }
    else if(LED==5){
      pinMode(4,OUTPUT);
      pinMode(7,INPUT);
      pinMode(8,OUTPUT);
      digitalWrite(4,HIGH);
      digitalWrite(8,LOW);
      delay(500);
    }
    else if(LED==6){
      pinMode(4,OUTPUT);
      pinMode(7,INPUT);
      pinMode(8,OUTPUT);
      digitalWrite(4,LOW);
      digitalWrite(8,HIGH);
      delay(500);
    }
}

void loop() {
  showLED(1);
  showLED(2);
  showLED(3);
  showLED(4);
  showLED(5);
  showLED(6);
  showLED(5);
  showLED(4);
  showLED(3);
  showLED(2);
}
