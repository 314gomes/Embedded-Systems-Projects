#include <Arduino.h>
#include <ESP32Servo.h> 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" 

// Pin definitions for RGB LED
int rpin = 13;
int gpin = 14;
int bpin = 12;

// Global variable for the timer countdown and its mutex
int remaining = 800;
SemaphoreHandle_t remainingMutex; 

// Sets the RGB LED color
void setColor(int R, int G, int B) {
    analogWrite(rpin, R);
    analogWrite(gpin, G);
    analogWrite(bpin, B);
}

// Task to decrement the timer and update the LED color based on the remaining time
void vtasktimer(void *pvParameters) {
    while (1) {
        int currentRemaining = 0; 

        // Protect access to 'remaining' with a mutex
        if (xSemaphoreTake(remainingMutex, portMAX_DELAY) == pdTRUE) {
            remaining--;
            if (remaining < 0) {
                remaining = 0;
            }
            currentRemaining = remaining; 
            xSemaphoreGive(remainingMutex); 
        }

        // Color fading logic based on 'currentRemaining'
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

            // Clear any remaining characters in the serial buffer
            while(Serial.available()) {
                Serial.read();
            }

            // Update 'remaining' with mutex protection
            if (xSemaphoreTake(remainingMutex, portMAX_DELAY) == pdTRUE) {
                remaining = newValue;
                xSemaphoreGive(remainingMutex); 
            }

            Serial.print("Timer set to: ");
            Serial.println(newValue);
        }

        vTaskDelay(pdMS_TO_TICKS(50)); 
    }
}

TaskHandle_t handletasktimer;

void setup() {
    Serial.begin(115200); 
    pinMode(rpin, OUTPUT);
    pinMode(gpin, OUTPUT);
    pinMode(bpin, OUTPUT);

    // Create the mutex before starting the tasks that use it
    remainingMutex = xSemaphoreCreateMutex();

    // Create the Timer task with higher priority for smooth fading
    xTaskCreatePinnedToCore(
        vtasktimer,
        "TIMER",
        configMINIMAL_STACK_SIZE + 1024,
        NULL,
        2, 
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
    // The main loop is mostly idle as FreeRTOS tasks handle the functionality
    vTaskDelay(pdMS_TO_TICKS(3000));
}