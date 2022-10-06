//Import Library
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <TimerOne.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Time
struct myTime {
  unsigned long hours;
  unsigned long minutes;
  unsigned long seconds;
  char AmPm[3];
};
unsigned long timerSec = millis();
bool AmPmState = true;


//Button
#define saveButton 7
#define editButton 4
#define increaseButton 5
#define decreaseButton 6
bool increaseButtonState;
bool decreaseButtonState;
bool lastIncreaseBState;
bool lastDecreaseBState;
bool editButtonState;
bool lastEditButtonState;

//EEPROM
int eeAddress = 0;

myTime Clock = {
  0,
  0,
  0,
  "AM"
};

//Clock
int editClockTimeUnit = 0;
bool editTimeClock = true;
bool editTimePressed = false;
bool lastEditTimePressed = false;
unsigned long timerSec2 = millis();
bool clockBlink = false;

//LDR
#define LDR A0
int valueLDR;
bool clockInverse = false;
int lastValueLDR;

//ADXL355
#define analogZ A1
#define analogY A2
#define analogX A3
bool clockRotation = false;
const unsigned int numAxes = 3;
const unsigned int pins[numAxes] = {analogX, analogY, analogZ};
const unsigned int  bufferSize = 16;
int buffer[numAxes] [bufferSize];
int bufferPos[numAxes] = {0};

//MAIN FUNCTION
void setup() {
  //Set up Serial Monitor
  Serial.begin(9600);

  //Set up OLED
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("SSD1306 allocation failed");
  }
  else
  {
    Serial.println("ArdinoAll OLED Start Work !!!");
  }

  //Set up button
  pinMode(saveButton, INPUT);
  pinMode(editButton, INPUT);
  pinMode(increaseButton, INPUT);
  pinMode(decreaseButton, INPUT);

  //EEPROM
  EEPROM.get(eeAddress, Clock);
  Serial.println(Clock.hours);
  Serial.println(Clock.minutes);
  Serial.println(Clock.seconds);
  Serial.println(Clock.AmPm);

  //Timer Interupt
  Timer1.initialize(500000);
  Timer1.attachInterrupt(blinkCount);
}

void loop() {
  //Time
  timeCount();


  //OLED State
  if (editTimePressed == false)
  {
    clockTime();
  }
  else if (editTimePressed == true)
  {
    editClockBlink();
  }


  //Save Button
  if (!digitalRead(saveButton))
  {
    Serial.println("save push");
    EEPROM.put(eeAddress, Clock);
  }


  //Edit Button
  editButtonState = digitalRead(editButton);
  if (editButtonState == LOW && lastEditButtonState != editButtonState)
  {
    editClockTimeUnit++;
    Serial.println(editClockTimeUnit);
  }
  lastEditButtonState = editButtonState;

  if (editClockTimeUnit == 1)
  {
    editTimePressed = true;
    indeCrease(1);
  }
  else if (editClockTimeUnit == 2)
  {
    editTimePressed = true;
    indeCrease(2);
  }
  else if (editClockTimeUnit == 3)
  {
    editTimePressed = true;
    indeCrease(3);
  }
  else if (editClockTimeUnit == 4)
  {
    editTimePressed = true;
    indeCrease(4);
  }
  else if (editClockTimeUnit == 5)
  {
    editClockTimeUnit = 0;
    editTimePressed = false;
  }

  //LDR
  valueLDR = analogRead(LDR);
  if (valueLDR > 600)
  {
    clockInverse = true;
  }
  else
  {
    clockInverse = false;
  }

  //ADXL355
  if (getZ() < 400)
  {
    clockRotation = true;
  }
  else
  {
    clockRotation = false;
  }
}


//FUNCTIONS
void timeCount()
{
  //Every 1 second
  if (millis() - timerSec >= 1000)
  {
    Clock.seconds += 1;
    timerSec += 1000;
  }


  //Set time
  if (Clock.seconds == 60)
  {
    Clock.minutes += 1;
    Clock.seconds = 0;
  }
  if (Clock.minutes == 60)
  {
    Clock.hours += 1;
    Clock.minutes = 0;
  }
  if (Clock.hours > 12)
  {
    Clock.hours = 1;
    AmPmState = !AmPmState;
  }
}

void clockTime()
{
  OLED.clearDisplay();
  if (clockRotation == true)
  {
    OLED.setRotation(2);
  }
  else
  {
    OLED.setRotation(0);
  }
  if (clockInverse == true)
  {
    OLED.invertDisplay(1);
  }
  else
  {
    OLED.invertDisplay(0);
  }
  OLED.setTextColor(WHITE, BLACK);
  OLED.setCursor(0, 8);
  OLED.setTextSize(2);
  if (Clock.hours < 10)
  {
    OLED.print(0);
  }
  OLED.print(Clock.hours);
  OLED.print(":");
  if (Clock.minutes < 10)
  {
    OLED.print(0);
  }
  OLED.print(Clock.minutes);
  OLED.print(":");
  if (Clock.seconds < 10)
  {
    OLED.print(0);
  }
  OLED.print(Clock.seconds);
  OLED.setCursor(96, 16);
  OLED.setTextSize(1);
  OLED.print(" ");
  if (AmPmState == true)
  {
    strcpy(Clock.AmPm, "AM");
    OLED.print(Clock.AmPm);
  }
  else
  {
    strcpy(Clock.AmPm, "PM");
    OLED.print(Clock.AmPm);
  }
  OLED.display();
}

void clockTimeBlink()
{
  OLED.clearDisplay();
  if (clockRotation == true)
  {
    OLED.setRotation(2);
  }
  else
  {
    OLED.setRotation(0);
  }
  if (clockInverse == true)
  {
    OLED.invertDisplay(1);
  }
  else
  {
    OLED.invertDisplay(0);
  }
  OLED.setTextColor(BLACK, BLACK);
  OLED.setCursor(0, 8);
  OLED.setTextSize(2);
  if (Clock.hours < 10)
  {
    OLED.print(0);
  }
  OLED.print(Clock.hours);
  OLED.print(":");
  if (Clock.minutes < 10)
  {
    OLED.print(0);
  }
  OLED.print(Clock.minutes);
  OLED.print(":");
  if (Clock.seconds < 10)
  {
    OLED.print(0);
  }
  OLED.print(Clock.seconds);
  OLED.setCursor(96, 16);
  OLED.setTextSize(1);
  OLED.print(" ");
  if (AmPmState == true)
  {
    strcpy(Clock.AmPm, "AM");
    OLED.print(Clock.AmPm);
  }
  else
  {
    strcpy(Clock.AmPm, "PM");
    OLED.print(Clock.AmPm);
  }
  OLED.display();
  timerSec2 += 1000;
}

void editClockBlink()
{
  if (clockBlink)
  {
    clockTimeBlink();
  }
  else
  {
    clockTime();
  }
}

void blinkCount()
{
  clockBlink = !clockBlink;
}

void indeCrease(int timeUnit)
{
  increaseButtonState = digitalRead(increaseButton);
  decreaseButtonState = digitalRead(decreaseButton);
  //Set Hours
  if (timeUnit == 1)
  {
    if (increaseButtonState == LOW && lastIncreaseBState != increaseButtonState)
    {
      Clock.hours++;
      if (Clock.hours > 12)
      {
        Clock.hours = 1;
      }
    }
    lastIncreaseBState = increaseButtonState;
    if (decreaseButtonState == LOW && lastDecreaseBState != decreaseButtonState)
    {
      Clock.hours--;
      Serial.println("-");
      if (Clock.hours < 1)
      {
        Clock.hours = 12;
      }
    }
    lastDecreaseBState = decreaseButtonState;
  }

  //Set Minutes
  if (timeUnit == 2)
  {
    if (increaseButtonState == LOW && lastIncreaseBState != increaseButtonState)
    {
      Clock.minutes++;
      if (Clock.minutes > 59)
      {
        Clock.minutes = 1;
      }
    }
    lastIncreaseBState = increaseButtonState;
    if (decreaseButtonState == LOW && lastDecreaseBState != decreaseButtonState)
    {
      Clock.minutes--;
      if (Clock.minutes < 1)
      {
        Clock.minutes = 59;
      }
    }
    lastDecreaseBState = decreaseButtonState;
  }

  //Set Seconds
  if (timeUnit == 3)
  {
    if (increaseButtonState == LOW && lastIncreaseBState != increaseButtonState)
    {
      Clock.seconds++;
      if (Clock.seconds > 59)
      {
        Clock.seconds = 1;
      }
    }
    lastIncreaseBState = increaseButtonState;
    if (decreaseButtonState == LOW && lastDecreaseBState != decreaseButtonState)
    {
      Clock.seconds--;
      if (Clock.seconds < 1)
      {
        Clock.seconds = 59;
      }
    }
    lastDecreaseBState = decreaseButtonState;
  }

  //Set AmPm
  if (timeUnit == 4)
  {
    if (increaseButtonState == LOW && lastIncreaseBState != increaseButtonState)
    {
      AmPmState = !AmPmState;
    }
    lastIncreaseBState = increaseButtonState;
    if (decreaseButtonState == LOW && lastDecreaseBState != decreaseButtonState)
    {
      AmPmState = !AmPmState;
    }
    lastDecreaseBState = decreaseButtonState;
  }
}

int getAxis(const int axis)
{
  buffer[axis][bufferPos[axis]] = analogRead(pins[axis]);
  bufferPos[axis] = (bufferPos[axis] + 1) % bufferSize;
  long sum = 0;
  for (unsigned int i = 0; i < bufferSize; i++)
  {
    sum += buffer[axis][i];
  }
  return round(sum / bufferSize);
}

int getX()
{
  return getAxis(0);
}

int getY()
{
  return getAxis(1);
}

int getZ()
{
  return getAxis(2);
}

//END
