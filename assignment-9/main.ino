#include <Arduino_FreeRTOS.h>
#include "semphr.h"
#include "queue.h"
#include "task.h"

//LED
#define Red 3
#define Yellow 4
#define Green 5
int redState = 0;
int greenState = 0;

//Button
#define buttonRed 10
#define buttonYellow 11
#define buttonGreen 12

//Timer
unsigned long CurrentTimeRed = 0;
unsigned long CurrentTimeYellow = 0;
unsigned long CurrentTimeGreen = 0;
unsigned long debounceRed = 0;
unsigned long debounceYellow = 0;
unsigned long debounceGreen = 0;

//Queue
QueueHandle_t ledQueue;

//Token for led priority
SemaphoreHandle_t token[3];

//SETUP
void setup() {
  Serial.begin(9600);
  ledQueue = xQueueCreate(10, sizeof(int32_t));
  xTaskCreate(vSenderTask, "Red SW",     100, buttonRed,     1, NULL);
  xTaskCreate(vSenderTask, "Yellow SW",  100,  buttonYellow ,  1, NULL);
  xTaskCreate(vSenderTask, "Green SW",   100, buttonGreen,   1, NULL);

  xTaskCreate(redLED,    "Red ",    100, NULL, 1, NULL);
  xTaskCreate(yellowLED, "Yellow ", 100, NULL, 1, NULL);
  xTaskCreate(greenLED,  "Green ",  100, NULL, 1, NULL);

  // Givetoken for each LED
  for (int i = 0; i < 3; i++) {
    token[i] = xSemaphoreCreateBinary();
    xSemaphoreGive(token[i]);
  }
}

//FUNCTION : Task for button
void vSenderTask(void *pvParameters) {
  BaseType_t qStatus;
  int32_t valueToSend = 0;
  int32_t button = (int32_t) pvParameters;
  pinMode(button, INPUT);
  while (1) {
    // Check input_pullup
    if (digitalRead(button)) {
      //Send button pin
      valueToSend = button;
      //add to queue
      qStatus = xQueueSend(ledQueue, &valueToSend, 0);
      vTaskDelay(1);
    }
  }
}

void redLED(void* pvParameters) {
  int32_t valueReceived;
  pinMode(Red, OUTPUT);
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  while (1) {
    qStatus = xQueueReceive(ledQueue, &valueReceived, xTicksToWait);
    if (qStatus == pdPASS) {

      // Check red button
      if (valueReceived == buttonRed  && millis() - debounceRed >= 500) {
        debounceRed = millis();
        //Check for red token
        if (xSemaphoreTake(token[0], 1) == pdTRUE) {
          CurrentTimeRed = millis();
          redState = 1;
        } else {
          // If push red button for 2nd time, it'll turn off
          redState = 0;
        }
      }
    }
    red_Control(redState);// For controlling red LED
  }
}


void yellowLED(void* pvParameters) {
  int32_t valueReceived;
  pinMode(Yellow, OUTPUT);
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  while (1) {
    qStatus = xQueueReceive(ledQueue, &valueReceived, xTicksToWait);
    if (qStatus == pdPASS) {
      // Check for yellow button
      if (valueReceived == buttonYellow   && millis() - debounceYellow >= 500) {
        debounceRed = millis();
        // check for red token
        if (xSemaphoreTake(token[0], 1) == pdTRUE) {
          //Unlock red token
          xSemaphoreGive(token[0]);
          //Check for green token
          if (xSemaphoreTake(token[2], 1) == pdTRUE) {
            //Unlock greeen token
            xSemaphoreGive(token[2]);
            //Check for yellow token
            if (xSemaphoreTake(token[1], 1) == pdTRUE) {
              //Turn yellow LED on
              yellow_Control(1);
            }
          }
        }
      }
    }
  }
}

void greenLED(void* pvParameters) {
  int32_t valueReceived;
  pinMode(Green, OUTPUT);
  BaseType_t qStatus;
  const TickType_t xTicksToWait = pdMS_TO_TICKS(100);

  while (1) {
    qStatus = xQueueReceive(ledQueue, &valueReceived, xTicksToWait);
    if (qStatus == pdPASS) {
      // Check for green button
      if (valueReceived == buttonGreen  && millis() - debounceGreen >= 500) {
        debounceGreen = millis();
        //Check red token
        if (xSemaphoreTake(token[0], 1) == pdTRUE) {
          //Unlock red token
          xSemaphoreGive(token[0]);
          //Check for green token ifself
          if (xSemaphoreTake(token[2], 1) == pdTRUE) {
            CurrentTimeGreen = millis();
            greenState = 1; //Turn on
          } else {
            greenState = 0; //Turn off if push green button when green LED is on
          }
        }
      }
    }
    green_Control(greenState); // controlling green LED
  }
}

//LOOP
void loop() {}

//FUNCTION : Control
void red_Control(int status) {
  // if red state is on
  if (status) {
    digitalWrite(Red, HIGH);
    //check for timer 3sec.
    if (millis() - CurrentTimeRed >= 3000) {
      digitalWrite(Red, LOW);
      redState = 0;
      //unlock red token
      xSemaphoreGive(token[0]);
    }
  } else { // for push button 2nd time
    digitalWrite(Red, LOW);
    xSemaphoreGive(token[0]); //unlock
  }
}

void yellow_Control(int status) {
  // if yellow lED on
  if (status) {
    // counter for yellow blink
    int cnt_Yellow = 0;
    while (cnt_Yellow < 4) {
      //check for each time of blink
      if (millis() - CurrentTimeYellow >= 500) {
        // switching between on and off
        digitalWrite(Yellow, !digitalRead(Yellow) ^ 0);
        cnt_Yellow++;
        CurrentTimeYellow = millis();
      }
    }
    //unlock yellow token
    xSemaphoreGive(token[1]);
  }
}

void green_Control(int status) {
  // If green led state on
  if ( status) {
    digitalWrite(Green, HIGH);
    //check for timer 3 sec.
    if (millis() - CurrentTimeGreen >= 3000) {
      digitalWrite(Green, LOW);
      greenState = 0;
      //unlock token
      xSemaphoreGive(token[2]);
    }
  } else { // for push button 2nd time
    digitalWrite(Green, LOW);
    // unlock token
    xSemaphoreGive(token[2]);
  }
}
