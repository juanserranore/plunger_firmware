/**
 * @file projectTasks.cpp
 * @brief Header file for system task management.
 * 
 * This header file contains function for managing system tasks,
 * user interface on the screen, Ethernet communication, stepper motor control, 
 * and rotary encoder handling.
 * 
 * @author Juan Alberto Serrano Redondo.
 */

#include "projectTasks.h"
#include <motor/motor.h>
#include <GUI/GUI.h>
#include <button/button.h>
#include <libraries.h>
#include <globals/globals.h>
 /** 
 * @brief Task to display the current time on the screen.
 * 
 * This task shows the current time on the screen if the current menu is not the
 * initial screen.
 * 
 * @param pvParameters Pointer to task parameters (not used).
 */
void taskShowTime(void *pvParameters) {
    while (true) {
        if (xSemaphoreTake(gatekeeper, pdMS_TO_TICKS(100))) {
            TickType_t currentTime = xTaskGetTickCount();
            // Check if you are not on the INITIAL_SCREEN .
            if (currentMenu != INITIAL_SCREEN) {
                RtcDateTime now = Rtc.GetDateTime();

                if (now.IsValid()) {  
                    printDateTime(now);    
                } else {
                    Serial.println("RTC lost confidence in the DateTime!");
                }
            }
            xSemaphoreGive(gatekeeper);
        }
        // 300 ms delay to reduce load
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}
/**
 * @brief Task to initialize and display the startup screen.
 * 
 * Configures and shows the initial screen, then switches to the main menu.
 * 
 * @param pvParameters Pointer to task parameters (not used).
 */
void taskStartScreen(void *pvParameters) {
  while(1){
    if (xSemaphoreTake(gatekeeper,1000)){
      Serial.println("Initial Screen got access");
      // Fill the screen with white
      tft.fillScreen(TFT_WHITE);

      // Display image at coordinates (20, 5) with dimensions 120x120
      tft.pushImage(20, 5, 120, 120, granasat_logo);

      // Delete the task as it is no longer needed
      vTaskDelay(5000);
      xSemaphoreGive(gatekeeper);
      currentMenu = MAIN_MENU;
    }
    vTaskDelete(NULL);
  }
}
/**
 * @brief Task to update the screen based on the current menu.
 * 
 * Updates the screen according to the current menu and commands received in the queue.
 * 
 * @param pvParameters Pointer to task parameters (not used).
 */
void taskUpdateScreen(void *pvParameters) {
    ScreenUpdateCommand command;
    while (!engineeringMode) {
      if (xSemaphoreTake(gatekeeper,1000)){
        Serial.println("Screen menu got access");
        if (xQueueReceive(screenUpdateQueue, &command, portMAX_DELAY) == pdPASS) {
            tft.fillScreen(backgroundpossibleColors[selectedBackgroundColor]);
            tft.setTextColor(textpossibleColors[selectedTextColor]);
            tft.setFreeFont(textType12);
            if (currentMenu == MAIN_MENU) {
              tft.fillRect(0,0,tft.width(),15,TFT_BLACK);
                for (int i = 0; i < menuItems; i++) {
                    
                    int16_t BarY = i*33 + tft.height()/40;
                    
                    // Draw the selection arrow
                    drawSelectionBar(BarY-tft.height()/26-5, i == menuIndex,i, menuItems);
                    // Centre the text horizontally
                    if (i == 0){
                    tft.setTextDatum(TL_DATUM);
                    tft.setFreeFont(textType9);
                    tft.setTextColor(TFT_WHITE);
                    tft.drawCentreString(menu[i], tft.width()/4, BarY,1);
                    } else {
                        tft.setFreeFont(textType12);
                        tft.setTextColor(textpossibleColors[selectedTextColor]);
                        tft.drawString(menu[i], tft.width()/3.5, BarY-2,1);    
                        tft.pushImage(5,BarY-tft.height()/20-5,30,30,images_main[i],TFT_WHITE);  
                      }
                }
            } else if (currentMenu == SETTINGS) {
              tft.fillRect(0,0,tft.width(),15,TFT_BLACK);
                for (int i = 0; i < submenuItems; i++) {
                    
        int16_t BarY = i*33 + tft.height()/40;

        drawSelectionBar(BarY-tft.height()/26-5, i == menuIndex, i, menuItems); 
            
        if (i == 0){
                  tft.setTextDatum(TL_DATUM);
                  tft.setFreeFont(textType9);
                  tft.setTextColor(TFT_WHITE);
                  tft.drawCentreString(submenu[i], tft.width()/4, BarY,1);
                  } else {
                      tft.setFreeFont(textType12);
                      tft.setTextColor(textpossibleColors[selectedTextColor]);
                      tft.drawString(submenu[i], tft.width()/3.5, BarY-2,1);    
                      tft.pushImage(5,BarY-tft.height()/20-5,30,30,images_settings[i],TFT_WHITE);  
                    }
        }

    
} else if (currentMenu == BRIGHTNESS) {
                tft.fillRect(0,0,tft.width(),15,TFT_BLACK);
                tft.setTextDatum(TL_DATUM);
                tft.setFreeFont(textType9);
                tft.setTextColor(TFT_WHITE);
                tft.drawCentreString("BRIGHTNESS", tft.width()/4, tft.height()/40,1);
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
                drawBrightnessBar(brightnessLevel); // Updating the brightness bar
            } else if (currentMenu == COLOR || currentMenu == BACKGROUND ||currentMenu == SELECTION_BAR ||currentMenu == SELECTION_BORDER ||currentMenu == TEXT) {
                tft.fillRect(0,0,tft.width(),15,TFT_BLACK);
                for (int i = 0; i < colormenuItems; i++) {
                
                    int16_t yspacing = i * 25;
                    int16_t x = tft.width() / 8;
                    int16_t y = yspacing + tft.height()/40;  // Vertical spacing
                    // Draw the selection arrow
                    drawSelectionBar(yspacing, i == menuIndex,i, colormenuItems);
                  if (i == 0){
                    tft.setTextDatum(TL_DATUM);
                    tft.setFreeFont(textType9);
                    tft.setTextColor(TFT_WHITE);
                    tft.drawCentreString(colormenu[i], tft.width()/4, y,1);
                  }
                    else {
                    tft.setFreeFont(textType9);
                    tft.setTextColor(textpossibleColors[selectedTextColor]);
                    tft.drawCentreString(colormenu[i], tft.width()/3, y,1);
                    if (i ==1)
                    tft.drawCentreString(background[*selection[i]],tft.width()/1.22,y,1);
                    else if (i == 2)
                    tft.drawCentreString(selectionbar[*selection[i]],tft.width()/1.22,y,1);
                    else if (i == 3)
                    tft.drawCentreString(border[*selection[i]],tft.width()/1.22,y,1);
                    else
                    tft.drawCentreString(text[*selection[i]],tft.width()/1.22,y,1);

                  }
                }
            } else if (currentMenu == TEXT_TYPE) {
                tft.fillRect(0,0,tft.width(),15,TFT_BLACK); 
                for (int i = 0; i < numFonts; i++) {
                
                    int16_t yspacing = i * 25;
                    int16_t x = tft.width() / 8;
                    int16_t y = yspacing + tft.height()/40;  // Vertical spacing
                    //  Draw the selection arrow
                    drawSelectionBar(yspacing, i == menuIndex,i, numFonts);
                  if (i == 0){
                    tft.setTextDatum(TL_DATUM);
                    tft.setFreeFont(textType9);
                    tft.setTextColor(TFT_WHITE);
                    tft.drawCentreString(fontType[i], tft.width()/4, y,1);
                  }
                    else {
                    tft.setFreeFont(textType9);
                    tft.setTextColor(textpossibleColors[selectedTextColor]);
                    tft.drawCentreString(fontType[i], tft.width()/3, y,1);
                  }
                }
            } else if (currentMenu == SERVIDOR_TCP){
              for (int i=0; i<ethernetmenuItems; i++){
                int yspacing = i*30+tft.height()/8;
                for (int i=0; i<ethernetmenuItems; i++){
                int y = i*30 +tft.height()/8;
                tft.fillRect(0,0,tft.width(),15,TFT_BLACK);
                tft.setTextDatum(TL_DATUM);
                tft.setTextColor(TFT_WHITE);
                tft.drawCentreString("TCP SERVER", tft.width()/4, tft.height()/40,1);
                tft.drawLine(tft.width()/2.5,0,tft.width()/2.5,tft.height(),TFT_BLACK);
                tft.drawLine(0,y-3,tft.width(),y-3,TFT_BLACK);
                tft.setTextColor(textpossibleColors[selectedTextColor]);
                tft.setFreeFont(textType12);
                tft.drawCentreString(ethernetmenu[i],tft.width()/5,y+6,1);
                tft.setFreeFont(textType9);
                tft.drawCentreString(ethernetInfo[i],tft.width()/1.4,y+7,1);
                }
              }
            } else if (currentMenu == STEPPER_MOTOR){
              tft.fillRect(0,0,tft.width(),15,TFT_BLACK);
                for (int i = 0; i < steppermenuItems; i++) {
                    
                    int16_t BarY = i*33 + tft.height()/40;
                    
                    //  Draw the selection arrow
                    drawSelectionBar(BarY-tft.height()/26-5, i == menuIndex,i, steppermenuItems);
                    // Centre the text horizontally
                    if (i == 0){
                    tft.setTextDatum(TL_DATUM);
                    tft.setFreeFont(textType9);
                    tft.setTextColor(TFT_WHITE);
                    tft.drawCentreString(steppermenu[i], tft.width()/4, BarY,1);
                    } else {
                        tft.setFreeFont(textType12);
                        tft.setTextColor(textpossibleColors[selectedTextColor]);
                        tft.drawString(steppermenu[i], tft.width()/3.5, BarY-2,1);    
                        tft.pushImage(5,BarY-tft.height()/20-5,30,30,images_stepper[i],TFT_WHITE);  
                      }
                }
            } else if (currentMenu == MANUAL){
              tft.fillRect(0,0,tft.width(),15,TFT_BLACK);
              for (int i = 0; i < manualmenuItems; i++) {
                    
                    int16_t BarY = i*33 + tft.height()/40;
                    int16_t BarX = (i-1)*75+15;
                    // Draw the selection arrow
                    drawSelectionBar(BarX-2, i == menuIndex,i, manualmenuItems);
                    // Centre the text horizontally
                    if (i == 0){
                    tft.setTextDatum(TL_DATUM);
                    tft.setFreeFont(textType9);
                    tft.setTextColor(TFT_WHITE);
                    tft.drawCentreString(manualmenu[i], tft.width()/4, BarY,1);
                    } else {
                        tft.setFreeFont(textType9);
                        tft.setTextColor(textpossibleColors[selectedTextColor]);
                        tft.drawString(manualmenu[i], BarX, 20,1);    
                      }
                }
                tft.setTextFont(1);
                tft.setTextSize(1);
                tft.fillCircle(centerX, centerY, radius, TFT_SKYBLUE);
                tft.drawCircle(centerX, centerY, radius, TFT_BLACK);
                drawDialMarks();
                updateDial();
                
            } else if (currentMenu == AUTO){
              tft.fillRect(0,0,tft.width(),15,TFT_BLACK);
              tft.setTextDatum(TL_DATUM);
              tft.setFreeFont(textType9);
              tft.setTextColor(TFT_WHITE);
              tft.drawCentreString(automenu[0], tft.width()/4, tft.height()/40,1);
              tft.setTextFont(1);
              tft.setTextSize(1);
              tft.fillCircle(centerX, centerY, radius, TFT_SKYBLUE);
              tft.drawCircle(centerX, centerY, radius, TFT_BLACK);
              drawDialMarks();
              updateDial();
            } 
        }
      xSemaphoreGive(gatekeeper);
    }
    vTaskDelay(300);
  }
}

/**
 * @brief Task to manage screen timeout and automatic brightness adjustment.
 * 
 * Handles screen timeout and controls brightness and automatic shutdown.
 * 
 * @param pvParameters Pointer to task parameters (not used).
 */
void taskScreenTimeout(void *pvParameters) {
  TickType_t lastActivityTime = xTaskGetTickCount();

  while (1) {
    TickType_t currentTick = xTaskGetTickCount();
    if (screenOn) {
      if ((currentTick - previousTick) * portTICK_PERIOD_MS >= min_bright_timer * portTICK_PERIOD_MS) {
          setBrightness(1); // Minimal brightness
        }
      if ((currentTick - previousTick) * portTICK_PERIOD_MS >= turn_off_timer * portTICK_PERIOD_MS) {
        screenOn = false;
        setBrightness(0); // Turn off the screen
        tft.writecommand(TFT_DISPOFF); // Put the screen in sleep mode
        ScreenUpdateCommand command = UPDATE_MAIN_MENU;
        xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1000)); // Verify every second
  }
}

/**
 * @brief Task for managing the rotary encoder.
 * 
 * This task reads the state of the rotary encoder and updates the position
 * and display accordingly. It only operates when the menu is set to AUTO.
 * 
 * @param pvParameters Pointer to task parameters (not used).
 */
void rotarymotorTask(void *pvParameters) {
  while (true) {  // Infinite loop for encoder task
    if (currentMenu == AUTO) {  // Only perform actions if the menu is AUTO
      int newClk = digitalRead(CLK);  // Read CLK pin status
      int dtValue = digitalRead(DT);  // Read DT pin status

      // Detect change in CLK status
      if (newClk != lastClk) {
        lastClk = newClk;  // Update the latest status of the CLK
        // Determine the direction of rotation based on the states of CLK and DT
        if (newClk == LOW && dtValue == HIGH) {
          encoderState = 1;  // Clockwise
          position++;  // Increase the position
          tft.setTextFont(1);
          tft.setTextSize(1);
          updateDial();  // Updating the dial display
        } else if (newClk == LOW && dtValue == LOW) {
          encoderState = -1;  // Counterclockwise
          position--;  // Decrease the position
          if (position < 0) position = 359;  // Adjust position if less than 0
          tft.setTextFont(1);
          tft.setTextSize(1);
          updateDial();  // Updating the dial display
        }
      } else {
        encoderState = 0;  // Detained if no change in CLK
      }
    }  
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay to avoid excessive CPU usage
  }
}
/**
 * @brief Task for controlling the motor.
 * 
 * This task adjusts the motor speed based on the rotary encoder's state.
 * It operates only when the menu is set to AUTO.
 * 
 * @param pvParameters Pointer to task parameters (not used).
 */
void motorTask(void *pvParameters) {
  while (true) {  // Infinite loop for the engine task
    if (currentMenu == AUTO) {  // Only perform actions if the menu is AUTO
      switch (encoderState) {
        case 1:
          motor.setSpeed(10); // Positive speed (clockwise)
          Serial.println("Motor: Sentido de las agujas del reloj");
          break;
        case -1:
          motor.setSpeed(-10); // Negative speed (counterclockwise)
          Serial.println("Motor: Sentido contrario a las agujas del reloj");
          break;
        case 0:
        default:
          motor.setSpeed(0); // Stop the engine
          Serial.println("Motor: Detenido");
          break;
      }
    }
    motor.runSpeed(); // Call runSpeed() to move the motor at the set speed.
    vTaskDelay(pdMS_TO_TICKS(10)); // Delay to avoid excessive CPU usage
  }
}
/**
 * @brief Task for managing Ethernet communication.
 * 
 * This task handles Ethernet client connections and commands. It provides
 * a menu for controlling the motor and updates the Ethernet status.
 * 
 * @param pvParameters Pointer to task parameters (not used).
 */
void taskEthernet(void *pvParameters) {
  while (1) {
    if (Ethernet.linkStatus() == LinkON) {
      server.begin();
      EthernetClient client = server.available();
      if (client) {
        if (client.connected()) {
          static bool menuShown = false; // Variable to check if the menu has been shown

          if (!menuShown) {
            client.println("-----------------------------------------");
            client.println("          TCP Motor Control Server       ");
            client.println("-----------------------------------------");
            client.println("Ethernet Link Status: ON");
            client.println("-----------------------------------------");
            client.println("Available Commands:");
            client.println("  1. Move Motor Forward   - \"forward\"");
            client.println("  2. Move Motor Backward  - \"backward\"");
            client.println("  3. Move Motor N Steps   - \"N\" (where N is an integer)");
            client.println("  4. Exit Menu            - \"exit\"");
            client.println("-----------------------------------------");
            client.println("Enter your command:");
            menuShown = true; // Mark the menu as shown
          }

          while (client.connected() && client.available()) {
            String command = client.readStringUntil('\n');
            command.trim(); // Remove whitespace
            Serial.println("Command received: " + command);

            client.println("");  // Insert a new line before the response

            if (command == "forward") {
              moveMotor(1);
              client.println("Motor moved one step forward");
            } else if (command == "backward") {
              moveMotor(-1);
              client.println("Motor moved one step backward");
            } else if (command.toInt() != 0) {
              int steps = command.toInt();
              if (steps > 0) {
                moveMotor(steps);
                client.println("Motor moved " + String(steps) + " steps forward");
              } else {
                moveMotor(steps);
                client.println("Motor moved " + String(-steps) + " steps backward");
              }
            }  else if (command == "exit") {
              client.println("Closing connection. Thank you for using the server!");
              client.stop();
              menuShown = false; // Reset for the next connection
              break;
            } else {
              client.println("Invalid command");
            }
            
            client.println("Enter your next command:");  // Prompt for the next command
          }
        } else {
          client.stop();
        }
      }
      ethernetInfo[3] = "ON";
    } else {
      ethernetInfo[3] = "OFF"; // Update status to "disconnected"
    }
    static String previousStatus = "";
    if (ethernetInfo[3] != previousStatus) {
      previousStatus = ethernetInfo[3];
      ScreenUpdateCommand command = UPDATE_MAIN_MENU;
      xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
    }
    vTaskDelay(LOOP_TICKS);
  }
}

/**
 * @brief Task for handling encoder-related tasks.
 * 
 * This task processes the rotary encoder input to navigate menus and adjust settings.
 * It updates the display based on the current menu and encoder position.
 * 
 * @param pvParameters Pointer to task parameters (not used).
 */
void taskEncoder(void *pvParameters) {
  TickType_t lastRotationTime = 0;
  while (1) {
    btn.read();
    aState = digitalRead(CLK);
    if (aState != aLastState) {
      previousTick = xTaskGetTickCount();
      if (screenOn) {
        setBrightness(brightnessLevel);
      }
      if (millis() - lastRotationTime > 30 && screenOn) {  // Small delay and locked screen verification
        if (digitalRead(DT) != aState) { // Reverse direction of the rotary encoder
          counter++;
        } else {
          counter--;
        }

        if (currentMenu == MAIN_MENU) {
          // Navigate through the main menu
          if (counter >= menuItems) {
            counter = 1;
          }
          if (counter < 1) {
            counter = menuItems - 1;
          }
          if (menuIndex != counter) {  // Only updates the display if the selection changes
            menuIndex = counter;
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          }
        } else if (currentMenu == SETTINGS) {
          // Navigating the settings menu
          if (counter >= submenuItems) {
            counter = 0;
          }
          if (counter < 0) {
            counter = submenuItems - 1;
          }
          if (menuIndex != counter) {  // Only updates the display if the selection changes
            menuIndex = counter;
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          }
          if (menuIndex == 0) {
            submenu[0] = "Exit";
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          } else  {
            submenu[0] = "SETTINGS";
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          }

        } else if (currentMenu == BRIGHTNESS) {
          // Adjust brightness
          if (counter > numBrightnessLevels) {
            counter = numBrightnessLevels;
          }
          if (counter < 1) {
            counter = 1;
          }
          if (brightnessLevel != counter) {  // Only update the display if the brightness changes
            brightnessLevel = counter;
            setBrightness(brightnessLevel);
            ScreenUpdateCommand command = UPDATE_CONFIG_BRIGHTNESS;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          }
        } else if (currentMenu == COLOR){
          menuIndex = 0;
          // Adjust colors
          if (counter >= colormenuItems) {
            counter = 0;
          }
          if (counter < 0) {
            counter = colormenuItems - 1;
          }
          if (menuIndex != counter) {  // Only update the display if the selection changes
            menuIndex = counter;
            ScreenUpdateCommand command = UPDATE_CONFIG_ASPECT;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY); 
          }
          if (menuIndex == 0) {
            colormenu[0] = "Exit";
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          } else  {
            colormenu[0] = "COLORS";
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          }
        }  else if (currentMenu == BACKGROUND ||currentMenu == SELECTION_BAR ||currentMenu == SELECTION_BORDER ||currentMenu == TEXT) {
          int *currentSelection;

    // Determine the current selection according to the menu
    switch (currentMenu) {
        case BACKGROUND:
            currentSelection = &selectedBackgroundColor;
            break;
        case SELECTION_BAR:
            currentSelection = &selectedBarColor;
            break;
        case SELECTION_BORDER:
            currentSelection = &selectedBorderColor;
            break;
        case TEXT:
            currentSelection = &selectedTextColor;
            break;
        default:
            // Unexpected case management
            return;
    }

    if (aState != aLastState) {
        if (digitalRead(DT) == aState) { // Increment the selected colour
            (*currentSelection)++;
        } else {
            (*currentSelection)--;
        }
        for (int i=0;i<4; i++){
        if (currentMenu == colormenuArray[i]){
        if (*currentSelection >= selectnumcolors[i]) {
            (*currentSelection) = 0;
        }
        if (*currentSelection < 0) {
            (*currentSelection) = selectnumcolors[i] - 1;
        }
        }
        }
            // Update the display with the new selected colour
            ScreenUpdateCommand command = UPDATE_CONFIG_BRIGHTNESS;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
        }
    } else if (currentMenu == TEXT_TYPE){
          if (counter >= numFonts) {
            counter = 0;
          }
          if (counter < 0) {
            counter = numFonts - 1;
          }
          if (menuIndex != counter) {  // It only updates the display if the selection is changed
            menuIndex = counter;
            ScreenUpdateCommand command = UPDATE_CONFIG_ASPECT;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY); // Send update command
          }
          if (menuIndex == 0) {
            fontType[0] = "Exit";
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          } else  {
            fontType[0] = "TEXT FONT";
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          }
        } else if (currentMenu == STEPPER_MOTOR){
          // Adjust colors
          if (counter >= steppermenuItems) {
            counter = 0;
          }
          if (counter < 0) {
            counter = steppermenuItems - 1;
          }
          if (menuIndex != counter) {  // Only updates the display if the selection changes
            menuIndex = counter;
            ScreenUpdateCommand command = UPDATE_CONFIG_ASPECT;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY); // Send update command
          }
          if (menuIndex == 0) {
            steppermenu[0] = "Exit";
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          } else  {
            steppermenu[0] = "STEPPER";
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          }
        } else if (currentMenu == MANUAL){
          // Adjust colors
          if (counter >= manualmenuItems) {
            counter = 0;
          }
          if (counter < 0) {
            counter = manualmenuItems - 1;
          }
          if (menuIndex != counter) {  // Only updates the display if the selection changes
            menuIndex = counter;
            ScreenUpdateCommand command = UPDATE_CONFIG_ASPECT;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY); // Send update command
          }
          if (menuIndex == 0) {
            manualmenu[0] = "Exit";
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          } else  {
            manualmenu[0] = "MANUAL";
            ScreenUpdateCommand command = UPDATE_MAIN_MENU;
            xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
          }
        }  
        lastRotationTime = millis();
      }
    }

    aLastState = aState;
    vTaskDelay(20 / portTICK_PERIOD_MS); // Short break for the task
  }
}

/**
 * @brief Task for handling button press events.
 * 
 * This task processes button presses, including single, double, and long presses,
 * and executes the corresponding handlers.
 * 
 * @param pvParameters Pointer to task parameters (not used).
 */
void taskButtonPress(void *pvParameters){
  while(1){
  btn.onPress(singlePressHandler)
     .onDoublePress(doublePressHandler)
     .onPressFor(longPressHandler,1000);
     vTaskDelay(20 / portTICK_PERIOD_MS);
  }
}
