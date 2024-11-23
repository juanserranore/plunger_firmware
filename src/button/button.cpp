/**
 * @file button.cpp
 * @brief Implementation file for button press handling functions.
 *
 * This file contains the implementation of functions that handle button 
 * press events, including single, double, and long presses. These functions 
 * allow users to navigate through menus, control the motor, adjust screen settings, 
 * and perform other actions.
 */

#include "button.h"
#include <globals/globals.h>
#include <motor/motor.h>
#include <GUI/GUI.h>
/**
 * @brief Handles single button press events.
 *
 * This function manages the navigation between different menus (main menu, settings,
 * motor control, etc.) based on the current state and selection. It sends update
 * commands to the screen to reflect the changes in the UI.
 *
 * @param btn Pointer to the button object.
 * @param pattern The press pattern detected (single press in this case).
 */
void singlePressHandler(BfButton *btn, BfButton::press_pattern_t pattern) {
  previousTick = xTaskGetTickCount();
  if (screenOn) {
    Serial.println("Single push");
    if (currentMenu == MAIN_MENU) {
      if (menuIndex == 1) {
        currentMenu = SETTINGS;
        menuIndex = 0;
        ScreenUpdateCommand command = UPDATE_MAIN_MENU;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } else if (menuIndex == 2){
        currentMenu = SERVIDOR_TCP;
        menuIndex = 0;
        ScreenUpdateCommand command = UPDATE_MAIN_MENU;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } else if (menuIndex == 3){
        currentMenu = STEPPER_MOTOR;
        menuIndex = 0;
        ScreenUpdateCommand command = UPDATE_MAIN_MENU;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      }
    } else if (currentMenu == SETTINGS) {
      previousMenu = MAIN_MENU;
      
      if (menuIndex ==0){
        currentMenu = previousMenu;
        
        ScreenUpdateCommand command = UPDATE_CONFIG_BACKGROUNDCOLOR;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } else if (menuIndex == 1) {
        currentMenu = BRIGHTNESS;
        counter = brightnessLevel; // Synchronise the counter with the current brightness level
        ScreenUpdateCommand command = UPDATE_CONFIG_BRIGHTNESS;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } else if (menuIndex == 2){
        currentMenu = COLOR;
        
        
        menuIndex = 0; 
        ScreenUpdateCommand command = UPDATE_CONFIG_ASPECT;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } else if (menuIndex == 3){
        currentMenu = TEXT_TYPE;
        
        menuIndex = 0;
        ScreenUpdateCommand command = UPDATE_CONFIG_ASPECT;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } 
    } else if (currentMenu == BRIGHTNESS) {
      currentMenu = SETTINGS;
      ScreenUpdateCommand command = UPDATE_CONFIG_BACKGROUNDCOLOR;
      xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
    } else if (currentMenu == COLOR){
        previousMenu = SETTINGS;
        if (menuIndex ==0){
          currentMenu = previousMenu;
          
          ScreenUpdateCommand command = UPDATE_CONFIG_BACKGROUNDCOLOR;
          xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
        } else if (menuIndex == 1){
          currentMenu = BACKGROUND;
        } else if (menuIndex == 2){
          currentMenu = SELECTION_BAR;
        } else if (menuIndex == 3){
          currentMenu = SELECTION_BORDER;
        } else if (menuIndex == 4){
          currentMenu = TEXT;
        } 
      ScreenUpdateCommand command = UPDATE_MAIN_MENU;
      xQueueSend(screenUpdateQueue, &command, portMAX_DELAY); // Send update command

    } else if (currentMenu == BACKGROUND ||currentMenu == SELECTION_BAR ||currentMenu == SELECTION_BORDER || currentMenu == TEXT){
      ScreenUpdateCommand command = UPDATE_CONFIG_BACKGROUNDCOLOR;
      xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      currentMenu = COLOR;
    } else if (currentMenu == TEXT_TYPE){
      previousMenu = SETTINGS;
        for (int i = 0; i<numFonts; i++){
          if (menuIndex == 0){
            currentMenu = previousMenu;
          
            ScreenUpdateCommand command = UPDATE_CONFIG_BACKGROUNDCOLOR;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          }
          else if (menuIndex == i){
            textType12 = possibleFont12[i];
            textType9 = possibleFont9[i];
           ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          }
        }
    } else if (currentMenu == SERVIDOR_TCP){
      currentMenu = MAIN_MENU;
      ScreenUpdateCommand command = UPDATE_MAIN_MENU;
      xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
    } else if (currentMenu == STEPPER_MOTOR) {
      previousMenu = MAIN_MENU;
      
      if (menuIndex ==0){
        currentMenu = previousMenu;
        
        ScreenUpdateCommand command = UPDATE_CONFIG_BACKGROUNDCOLOR;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } else if (menuIndex == 1) {
        currentMenu = AUTO;
        
        ScreenUpdateCommand command = UPDATE_CONFIG_BRIGHTNESS;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } else if (menuIndex == 2){
        currentMenu = MANUAL;
        counter = 0; 
        ScreenUpdateCommand command = UPDATE_CONFIG_ASPECT;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } 
    } else if (currentMenu == AUTO) { 
      currentMenu = STEPPER_MOTOR;
    }else if (currentMenu == MANUAL) {
      previousMenu = STEPPER_MOTOR;
      
      if (menuIndex ==0){
        currentMenu = previousMenu;
        
        
        ScreenUpdateCommand command = UPDATE_CONFIG_BACKGROUNDCOLOR;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } else if (menuIndex == 1) {
          digitalWrite(DIR_PIN, HIGH); // HIGH for one direction, LOW for the other one
          // Moves the motor one step
          moveMotor(1);

          
          ScreenUpdateCommand command = UPDATE_CONFIG_BACKGROUNDCOLOR;
          xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      } else if (menuIndex == 2){
          digitalWrite(DIR_PIN, LOW); // HIGH for one direction, LOW for the other one
          //Moves the motor one step
          moveMotor(-1);
          

            
          ScreenUpdateCommand command = UPDATE_CONFIG_BACKGROUNDCOLOR;
          xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);    
      } 
    }
  }
}
/**
 * @brief Handles double button press events.
 *
 * This function toggles the engineering mode when a double press is detected.
 *
 * @param btn Pointer to the button object.
 * @param pattern The press pattern detected (double press in this case).
 */
void doublePressHandler(BfButton *btn, BfButton::press_pattern_t pattern) {
  Serial.println("Double push");
  engineeringMode != engineeringMode;
  // Implement your logic here for double tapping
}

/**
 * @brief Handles long button press events.
 *
 * This function toggles the screen on/off. If the screen is turned off, it reduces
 * the brightness to zero and puts the display into sleep mode. When turned back on, 
 * the display is woken up and the brightness is restored to the previous level.
 *
 * @param btn Pointer to the button object.
 * @param pattern The press pattern detected (long press in this case).
 */
void longPressHandler(BfButton *btn, BfButton::press_pattern_t pattern) {
  previousTick = xTaskGetTickCount();
  Serial.println("Long push");
  screenOn = !screenOn;
  if (!screenOn) {
    setBrightness(0); // Turn off the screen
    tft.writecommand(TFT_DISPOFF); // Put the screen in sleep mode
    ScreenUpdateCommand command = UPDATE_MAIN_MENU;
    xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
  } else {
    tft.writecommand(TFT_DISPON); // Taking the screen out of sleep mode
    setBrightness(brightnessLevel);
  }
}