#include "TimerSetup.h"

void setup() {
    // Initialize serial communication with the computer at 9600 baud rate
    Serial.begin(9600);
    while (!Serial);  // Wait for the serial port to be ready

    // Configure the pin connected to the LED as an output
    pinMode(LED_PIN, OUTPUT);

    // Initialize the DHT sensor
    dht.begin();  // Start the DHT sensor
    // Configure the pin connected to the DHT sensor as an input
    pinMode(DHT_PIN, INPUT);

    // Setup timers for various tasks
    Serial.println("Configuring Timer for DHT sensor.");
    TimerDHT();  // Initialize the timer for handling DHT sensor readings

    Serial.println("Configuring Timer for LED blinking.");
    TimerLED();  // Initialize the timer for LED blinking functionality

    Serial.println("System setup complete.");
}

void loop() {
    // The loop function remains empty as all operations are handled by interrupt service routines (ISRs)
}
