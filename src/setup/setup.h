/**
 * @file setup.h
 * @brief Contains all setup configurations for the project including Ethernet, LCD, Motor, Pin, RTC, and Task setups.
 */

#ifndef SETUP_H
#define SETUP_H

#include <globals/globals.h>
#include <libraries.h>
#include <GUI/GUI.h>
#include <project_tasks/projectTasks.h>
void setup(){
    /**
     * @brief Ethernet setup configuration for ENC28J60 module.
     *
     * Initializes SPI communication and configures the Ethernet with the provided MAC address and IP.
     */
    SPI.begin(); /**< Initializes SPI communication */
    Ethernet.init(26); /**< CS pin for ENC28J60 module */
    Ethernet.begin(mac, ip); /**< Start Ethernet with MAC address and IP */

    /**
     * @brief LCD setup configuration for the TFT display.
     *
     * Configures serial communication, initializes the display, sets text properties, 
     * and creates necessary queues and semaphores for screen management.
     */
    Serial.begin(115200); /**< Initialize serial communication at 115200 baud */
    tft.init(); /**< Initialize TFT display */
    tft.setTextWrap(false); /**< Disable text wrapping */
    tft.setTextSize(1); /**< Set text size to small */
    tft.setRotation(1); /**< Set display rotation */
    screenUpdateQueue = xQueueCreate(10, sizeof(ScreenUpdateCommand)); /**< Create a queue for screen updates */
    setBrightness(brightnessLevel); /**< Set initial screen brightness */
    tft.setSwapBytes(true); /**< Enable byte swapping for better display rendering */
    gatekeeper = xSemaphoreCreateMutex(); /**< Create a mutex for controlling screen updates */

    /**
     * @brief Motor setup configuration.
     *
     * Configures motor speed, acceleration, and reads initial state from encoder.
     */
    aLastState = digitalRead(CLK); /**< Read initial state of the encoder */
    motor.setMaxSpeed(2000.0); /**< Set maximum motor speed */
    motor.setAcceleration(1000.0); /**< Set motor acceleration */

    /**
     * @brief Pin setup configuration.
     *
     * Configures the pins for the encoder and motor control.
     */
    pinMode(CLK, INPUT_PULLUP); /**< Set CLK pin as input with pull-up resistor */
    pinMode(DT, INPUT_PULLUP); /**< Set DT pin as input with pull-up resistor */
    pinMode(DIR_PIN, OUTPUT); /**< Set direction pin as output */
    pinMode(STEP_PIN, OUTPUT); /**< Set step pin as output */

    /**
     * @brief RTC setup configuration.
     *
     * Initializes the Real-Time Clock (RTC) and sets the date and time based on the compilation time.
     * Verifies if the RTC is write-protected and ensures it is running.
     */
    Rtc.Begin(); /**< Initialize RTC communication */

    Serial.print("compiled: ");
    Serial.print(__DATE__); /**< Display the compilation date */
    Serial.print(" ");
    Serial.println(__TIME__); /**< Display the compilation time */

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__); /**< Set compiled date and time */

    /** 
     * Check if the RTC's date and time are valid, and if not, set the compiled date and time.
     */
    if (!Rtc.IsDateTimeValid()) {
        Serial.println("RTC lost confidence in the DateTime! Setting compilation date and time.");
        Rtc.SetDateTime(compiled); /**< Set RTC to compilation time */
    }

    /** 
     * Check if the RTC is write-protected, and disable write protection if necessary.
     */
    if (Rtc.GetIsWriteProtected()) {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false); /**< Disable write protection */
    }

    /** 
     * Check if the RTC is running, and start it if it is not.
     */
    if (!Rtc.GetIsRunning()) {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true); /**< Start RTC */
    }

    RtcDateTime now = Rtc.GetDateTime(); /**< Get the current date and time from the RTC */

    /** 
     * Compare the RTC time with the compiled time and update the RTC if necessary.
     */
    if (now < compiled) {
        Serial.println("RTC is older than compile time! Updating DateTime.");
        Rtc.SetDateTime(compiled); /**< Update RTC to compiled date and time */
    } else if (now > compiled) {
        Serial.println("RTC is newer than compile time. (this is expected)");
    } else if (now == compiled) {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }

    /**
     * @brief Task setup configuration.
     *
     * Creates various FreeRTOS tasks for managing display, button presses, Ethernet, motor control, and screen updates.
     */
    xTaskCreatePinnedToCore(taskShowTime, "ShowTime", 16384, NULL, 1, NULL, 1); 
    xTaskCreatePinnedToCore(taskStartScreen, "TaskScreenStart", 16384, NULL, 5, NULL, 0); 
    xTaskCreatePinnedToCore(taskEncoder, "TaskEncoder", 16384, NULL, 3, NULL, 0); 
    xTaskCreatePinnedToCore(taskScreenTimeout, "ScreenTimeout", 16384, NULL, 10, NULL, 1); 
    xTaskCreatePinnedToCore(taskButtonPress, "ButtonPress", 16384, NULL, 3, NULL, 0); 
    xTaskCreatePinnedToCore(taskEthernet, "TaskEthernet", 16384, NULL, 7, NULL, 1); 
    xTaskCreatePinnedToCore(taskUpdateScreen, "UpdateScreen", 16384, NULL, 2, NULL, 0); 
    xTaskCreate(rotarymotorTask, "Rotary motor Task", 2048, NULL, 4, NULL); 
    xTaskCreate(motorTask, "Motor Task", 2048, NULL, 6, NULL); 
    vTaskStartScheduler(); /**< Start the FreeRTOS scheduler */

    ScreenUpdateCommand initialCommand = UPDATE_MAIN_MENU;
    xQueueSend(screenUpdateQueue, &initialCommand, portMAX_DELAY);
}
#endif // COMBINED_SETUP_H
