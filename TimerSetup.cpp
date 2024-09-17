#include "TimerSetup.h"

// Initialize the DHT sensor object
DHT dht(DHT_PIN, DHT_TYPE);  // Create a DHT object using the defined pin and sensor type

// Constants for timer intervals
#define Sensor_ReadInterval 10000   // Interval for DHT sensor reads: 10 seconds (in milliseconds)
#define Blink_Interval_20Hz 50   // Interval for LED blinking: 50 milliseconds (20Hz blink rate)

// Function to configure Timer TC3 for DHT sensor readings
void TimerDHT() {
    // Enable the clock for Timer TC3
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(TC3_GCLK_ID);
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);  // Wait until the clock is stable
    Serial.println("Clock for TC3 enabled.");

    // Configure TC3 as a 16-bit timer with a prescaler of 1024
    TC3->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1024;
    while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);  // Wait for timer configuration to complete
    Serial.println("TC3 configured as 16-bit timer.");

    // Calculate and set the timer interval for sensor readings
    uint16_t ccValue = (Sensor_ReadInterval * (SystemCoreClock / 1024) / 1000) - 1;
    TC3->COUNT16.CC[0].reg = ccValue;
    while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);  // Wait for the interval to be set
    Serial.print("TC3 interval set to: ");
    Serial.println(ccValue);

    // Enable the timer overflow interrupt for TC3
    TC3->COUNT16.INTENSET.reg = TC_INTENSET_OVF;
    NVIC_EnableIRQ(TC3_IRQn);  // Enable the interrupt in the NVIC
    Serial.println("TC3 overflow interrupt enabled.");

    // Start Timer TC3
    TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
    while (TC3->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);  // Wait for timer to start
    Serial.println("TC3 timer started.");
}

// Function to configure Timer TC4 for LED blinking
void TimerLED() {
    // Enable the clock for Timer TC4
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(TC4_GCLK_ID);
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);  // Wait until the clock is stable
    Serial.println("Clock for TC4 enabled.");

    // Configure TC4 as a 16-bit timer with a prescaler of 1024
    TC4->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1024;
    while (TC4->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);  // Wait for timer configuration to complete
    Serial.println("TC4 configured as 16-bit timer.");

    // Calculate and set the timer interval for LED blinking
    uint16_t ccValue = (Blink_Interval_20Hz * (SystemCoreClock / 1024) / 1000) - 1;
    TC4->COUNT16.CC[0].reg = ccValue;
    while (TC4->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);  // Wait for the interval to be set
    Serial.print("TC4 interval set to: ");
    Serial.println(ccValue);

    // Enable the timer overflow interrupt for TC4
    TC4->COUNT16.INTENSET.reg = TC_INTENSET_OVF;
    NVIC_EnableIRQ(TC4_IRQn);  // Enable the interrupt in the NVIC
    Serial.println("TC4 overflow interrupt enabled.");

    // Start Timer TC4
    TC4->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
    while (TC4->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);  // Wait for timer to start
    Serial.println("TC4 timer started.");
}

// Interrupt Service Routine (ISR) for Timer TC3 (DHT Sensor)
extern "C" void TC3_Handler() {
    if (TC3->COUNT16.INTFLAG.reg & TC_INTFLAG_OVF) {  // Check if the timer overflow interrupt flag is set
        TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF;  // Clear the overflow flag

        // Read data from the DHT sensor
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();
        
        if (isnan(humidity) || isnan(temperature)) {  // Check for read errors
            Serial.println("Failed to read from DHT sensor!");
        } else {
            // Output the sensor data to the serial monitor
            Serial.print("Humidity: ");
            Serial.print(humidity);
            Serial.print("%  Temperature: ");
            Serial.print(temperature);
            Serial.println("°C");
        }
    }
}

// Interrupt Service Routine (ISR) for Timer TC4 (LED Blinking)
extern "C" void TC4_Handler() {
    if (TC4->COUNT16.INTFLAG.reg & TC_INTFLAG_OVF) {  // Check if the timer overflow interrupt flag is set
        TC4->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF;  // Clear the overflow flag

        // Toggle the state of the LED
        static bool ledState = false;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
        Serial.print("LED state: ");
        Serial.println(ledState ? "ON" : "OFF");
    }
}
