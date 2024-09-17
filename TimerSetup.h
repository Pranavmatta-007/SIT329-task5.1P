#ifndef TIMER_SETUP_H
#define TIMER_SETUP_H

#include <Arduino.h>
#include <DHT.h>  // Include the DHT library for handling temperature and humidity sensors

// Pin assignments
#define LED_PIN 3   // Pin connected to the LED (PA07)
#define DHT_PIN 2   // Pin connected to the DHT sensor (PB10)

// Specify the type of DHT sensor used
#define DHT_TYPE DHT11  // Set to DHT11; change to DHT22 if a different sensor is used

// Declare an external DHT object to be used in other files
extern DHT dht;  

// Function declarations for timer setup
void TimerDHT();  // Function to configure the timer for DHT sensor readings
void TimerLED();  // Function to configure the timer for LED blinking

#endif // TIMER_SETUP_H
