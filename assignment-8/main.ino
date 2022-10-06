#include<Arduino_FreeRTOS.h>
#include "queue.h"

#define btn_red 8
#define btn_yellow 9
#define btn_green 10

#define led_red 3
#define led_yellow 4
#define led_green 5


QueueHandle_t Qred, Qyellow, Qgreen;
int32_t green_state = 1, yellow_state = 1, red_state = 1;
unsigned long red_time = 0 , yellow_time = 0 , green_time = 0 ;



void setup() {
  Serial.begin(9600);
  Qred = xQueueCreate(5, sizeof(int32_t));
  Qyellow = xQueueCreate(2, sizeof(int32_t));
  Qgreen = xQueueCreate(1, sizeof(int32_t));

  xTaskCreate(redBUT , "RED BUTTON" , 100, NULL, 1, NULL);
  xTaskCreate(yellowBUT , "YELLOW BUTTON" , 100, NULL, 1, NULL);
  xTaskCreate(greenBUT , "GREEN BUTTON" , 100, NULL, 1, NULL);

  xTaskCreate(redLED , "RED LED" , 100, NULL, 1, NULL);
  xTaskCreate(yellowLED , "YELLOW LED" , 100, NULL, 1, NULL);
  xTaskCreate(greenLED , "GREEN LED" , 100 , NULL, 1, NULL);

}

//============= RED ==============//

void redBUT(void *pvParameters) // <= send
{
  BaseType_t qStatus_red;
  int32_t  valueToSend_red = 0;
  pinMode(btn_red, INPUT);
  while (1)
  {
    if (digitalRead(btn_red ) == LOW)
    {
      if (millis() - red_time >= 400)
      {
        red_state = 0;
        valueToSend_red = red_state;
        qStatus_red = xQueueSend(Qred, &valueToSend_red, 0);
        red_time = millis();
      }
    }
    vTaskDelay(5);
  }
}

void redLED(void *pvParameters) // <= received
{

  int32_t valueReceived_red;
  BaseType_t qStatus_red;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(led_red , OUTPUT);
  digitalWrite(led_red, HIGH);
  while (1)
  {
    xQueueReceive(Qred, &valueReceived_red, xTicksToWait);
    if (valueReceived_red == 0)
    {
      digitalWrite(led_red, LOW);
      Serial.println("on");
      vTaskDelay(pdMS_TO_TICKS(3000));
      valueReceived_red = 1;
    }
    else
    {
      Serial.println("off");
      digitalWrite(led_red, HIGH);
    }
  }
}

//============= YELLOW ==============//

void yellowBUT(void *pvParameters) // <= send
{
  BaseType_t qStatus_yellow;
  int32_t  valueToSend_yellow = 0;
  pinMode(btn_yellow, INPUT);
  while (1)
  {
    if (digitalRead(btn_yellow ) == LOW)
    {
      if (millis() - yellow_time >= 400)
      {
        if (yellow_state == 1)
        {
          yellow_state = 0;
        }
        else
        {
          yellow_state = 1;
        }
        yellow_time = millis();
      }
    }
    valueToSend_yellow = yellow_state;
    qStatus_yellow = xQueueSend(Qyellow, &valueToSend_yellow, 0);
    vTaskDelay(5);
  }
}
void yellowLED(void *pvParameters)  // <= received
{
  int32_t valueReceived_yellow;
  BaseType_t qStatus_yellow;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(led_yellow , OUTPUT);
  digitalWrite(led_yellow, HIGH);

  while (1)
  {
    xQueueReceive(Qyellow, &valueReceived_yellow, xTicksToWait);

    if (valueReceived_yellow == 0)
    {
      digitalWrite(led_yellow, LOW);
      vTaskDelay(8);
      digitalWrite(led_yellow, HIGH);
      vTaskDelay(8);
    }
  }
}

//============= GREEN ==============//

void greenBUT(void *pvParameters) // <= send
{
  int ledOn = 0;
  BaseType_t qStatus_green;
  int32_t  valueToSend_green = 0;
  pinMode(btn_green, INPUT);

  while (1)
  {
    valueToSend_green = digitalRead(btn_green);
    qStatus_green = xQueueSend(Qgreen, &valueToSend_green, 0);

    vTaskDelay(15);

  }
}
void greenLED(void *pvParameters) // <= received
{

  int32_t valueReceived_green;
  BaseType_t qStatus_green;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);
  pinMode(led_green , OUTPUT);
  digitalWrite(led_green, HIGH);
  while (1)
  {
    xQueueReceive(Qgreen , &valueReceived_green , xTicksToWait);
    if (valueReceived_green == 0)
    {
      green_state = 0;
    }
    if (green_state == 0)
    {

      for (int i = 1 ; i <= 3 ; i++)
      {
        digitalWrite(led_green, LOW);
        vTaskDelay(50);
        digitalWrite(led_green, HIGH);
        vTaskDelay(50);
      }
      green_state = 1;
      Serial.println("out");
    }
  }
}
void loop() {}