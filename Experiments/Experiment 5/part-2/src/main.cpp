#include <Arduino.h>
#include <ESP32Servo.h> 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" 

int rpin = 13;
int gpin = 14;
int bpin = 12;

int remaining = 800;
SemaphoreHandle_t remainingMutex; 

void setColor(int R, int G, int B) {
    analogWrite(rpin, R);
    analogWrite(gpin, G);
    analogWrite(bpin, B);
}

void vtasktimer(void *pvParameters) {
    while (1) {
        int currentRemaining = 0; 

        if (xSemaphoreTake(remainingMutex, portMAX_DELAY) == pdTRUE) {
            remaining--;
            if (remaining < 0) {
                remaining = 0;
            }
            currentRemaining = remaining; // Make a local copy
            xSemaphoreGive(remainingMutex); // Release the mutex
        }

        if (currentRemaining >= 765) {
            setColor(255, 255, 255);
        } else if (currentRemaining >= 510) {
            setColor(255, 255, currentRemaining - 510);
        } else if (currentRemaining >= 255) {
            setColor(255, currentRemaining - 255, 0);
        } else if (currentRemaining >= 0) {
            setColor(currentRemaining, 0, 0);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

/**
 * @brief This task reads from Serial and sets the 'remaining' timer.
 */
void vtaskinterface(void *pvParameters) {
    while (1) {
        if (Serial.available() > 0) {
            
            int newValue = Serial.parseInt();

            while(Serial.available()) {
                Serial.read();
            }

            if (xSemaphoreTake(remainingMutex, portMAX_DELAY) == pdTRUE) {
                remaining = newValue;
                xSemaphoreGive(remainingMutex); // Release the mutex
            }

            Serial.print("Timer set to: ");
            Serial.println(newValue);
        }

        vTaskDelay(pdMS_TO_TICKS(50)); // Check for serial data every 50ms
    }
}

TaskHandle_t handletasktimer;

void setup() {
    Serial.begin(115200); // <-- Must initialize Serial
    pinMode(rpin, OUTPUT);
    pinMode(gpin, OUTPUT);
    pinMode(bpin, OUTPUT);

    // Create the mutex *before* starting the tasks that use it
    remainingMutex = xSemaphoreCreateMutex();

    // Create the Timer task
    xTaskCreatePinnedToCore(
        vtasktimer,
        "TIMER",
        configMINIMAL_STACK_SIZE + 1024,
        NULL,
        2, // Higher priority for smooth fading
        &handletasktimer,
        1);

    // Create the Interface task
    xTaskCreatePinnedToCore(
        vtaskinterface,
        "INTERFACE",
        configMINIMAL_STACK_SIZE + 1024,
        NULL,
        1,
        NULL, 
        0); 
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(3000));
}