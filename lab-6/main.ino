//https://www.tinkercad.com/things/bRGZ6y8NTTK-itclab06/editel?sharecode=3dKz5NzOS3wfOk1bbLCaQBCOpLpORYr-RO3TIFIsIVA

#include <Wire.h>
int slaveAddress2 = 9;
int slaveAddress3 = 10;
char data[]="HELLO1";
int timeDelay = 250;

void setup()

{
  Wire.begin(); 
  Serial.begin( 9600 );
}

void loop()
{
  //Sent data to board 2
  Wire.beginTransmission( slaveAddress2 ); 
  Wire.write(data);
  Wire.endTransmission(); 
  delay(timeDelay);
  
  //Get data from board 2
  Wire.requestFrom(slaveAddress2, 6);

  while (Wire.available() > 0) 
  {
    for (int i = 0 ; i < 6; i++)
    {
      data[i] = Wire.read();
    }
  }
  Serial.println(data);
  delay(timeDelay);
  
  //Sent data to board 3
  Wire.beginTransmission( slaveAddress3 ); 
  Wire.write(data);
  Wire.endTransmission(); 
  delay(timeDelay);
  
  //Get data from board 3
  Wire.requestFrom(slaveAddress3, 6);

  while (Wire.available() > 0) 
  {
    for (int i = 0 ; i < 6; i++)
    {
      data[i] = Wire.read();
    }
  }
  Serial.println(data);
  delay(timeDelay);
}

