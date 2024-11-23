/**
 * @file GUI.cpp
 * @brief Implementation of the functions related to the graphical user interface (GUI).
 *
 * This file contains the implementation of the functions to handle the GUI,
 * such as drawing the dial needle, selection bar, and other functionalities.
 * 
 * @author Juan Alberto Serrano Redondo.
 */

#include "GUI.h"

/**
 * @brief Displays the date and time on the screen and in the serial monitor.
 * 
 * This function takes a RtcDateTime object, formats the time as HH:MM, 
 * and displays it both on the serial monitor and the TFT screen.
 * 
 * @param dt The current date and time in RtcDateTime format.
 */
void printDateTime(const RtcDateTime& dt)
{
    char datestring[6]; // Set to show only hours and minutes (HH:MM)
    snprintf_P(datestring, 
               sizeof(datestring),
               PSTR("%02u:%02u"),
               dt.Hour(),
               dt.Minute());

    // Displays the time on the serial monitor
    Serial.print("Hora: ");
    Serial.println(datestring);

    // Compares the current time with the last displayed time.
    // Updates the display only if the time has changed.
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // Sets the colour of text and background
    tft.setFreeFont(textType9);
    tft.fillRect(80, 0, 110, 15, TFT_BLACK); // Clears the area where the time will be displayed
    tft.setCursor(120, 10); // Adjusts the position of the cursor on the screen
    tft.print(datestring);
}

/**
 * @brief Draws a needle on the dial.
 * 
 * This function draws a needle on the dial based on the specified angle and length.
 * 
 * @param angle Angle of the needle.
 * @param length Length of the needle.
 * @param color Color of the needle.
 */
void drawNeedle(int angle, int length, uint32_t color) {
  float rad = radians(angle);
  int xBase1 = centerX + 2 * cos(rad + radians(90));
  int yBase1 = centerY + 2 * sin(rad + radians(90));
  int xBase2 = centerX + 2 * cos(rad - radians(90));
  int yBase2 = centerY + 2 * sin(rad - radians(90));
  int xTip = centerX + length * cos(rad);
  int yTip = centerY + length * sin(rad);
  tft.fillTriangle(xBase1, yBase1, xBase2, yBase2, xTip, yTip, color);
  tft.drawLine(centerX, centerY, xTip, yTip, color);
  tft.fillCircle(centerX, centerY, 3, TFT_RED);
}
/**
 * @brief Draws the dial marks.
 * 
 * Draws the marks and numbers at regular intervals around the dial.
 */
void drawDialMarks() {
  tft.setTextColor(TFT_BLACK, TFT_SKYBLUE);
  tft.setTextSize(1);
  for (int i = 0; i < 360; i += 15) {
    float rad = radians(i);
    int xOuter = centerX + (radius - 2) * cos(rad);
    int yOuter = centerY + (radius - 2) * sin(rad);
    int xInner = centerX + (radius - (i % 60 == 0 ? 10 : 6)) * cos(rad);
    int yInner = centerY + (radius - (i % 60 == 0 ? 10 : 6)) * sin(rad);
    tft.drawLine(xOuter, yOuter, xInner, yInner, TFT_BLACK);
    if (i % 60 == 0) {
      int xText = centerX + (radius - 18) * cos(rad);
      int yText = centerY + (radius - 18) * sin(rad);
      tft.setCursor(xText - 3, yText - 3);
      tft.print(i);
    }
  }
}
/**
 * @brief Updates the dial by moving the needle to the new position.
 */
void updateDial() {
  drawNeedle(lastAngle, radius - 10, TFT_SKYBLUE);
  drawDialMarks();
  int newAngle = position % 360;
  drawNeedle(newAngle, radius - 10, TFT_RED);
  lastAngle = newAngle;

  String positionStr = String(position % 360);
  int textWidth = tft.textWidth(positionStr);
  int textX = centerX - (textWidth / 2);
  int textY = centerY + 10 - (tft.fontHeight() / 2);

  tft.fillRect(centerX - 11, centerY + 4, 21, 11, TFT_BLACK);
  tft.drawRect(centerX - 11, centerY + 4, 21, 11, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(textX, textY);
  tft.print(position % 360);

  delay(200);
}

/**
 * @brief Sets the screen brightness level.
 * 
 * @param level The brightness level to set.
 */
void setBrightness(int level) {
  int mappedLevel = map(level, 1, numBrightnessLevels, minBrightness, maxBrightness);
  dacWrite(BRIGHTNESS_PIN, mappedLevel);
  if (currentMenu == BRIGHTNESS) {
    drawBrightnessBar(level); // Update the brightness bar on the screen
  }
}
/**
 * @brief Draws the brightness bar on the screen.
 * 
 * @param level Current brightness level.
 */
void drawBrightnessBar(int level) {
  const int margin = 10; // Lateral margins
  const int maxBarLength = tft.width() - 2 *margin;

  int barLength = map(level, 1, numBrightnessLevels, 0, maxBarLength); // Bar length 
  tft.fillRoundRect(margin, tft.height()/2-12, maxBarLength, 24,5, LIGHT_GRAY);
  tft.fillRoundRect(margin, tft.height()/2-12, barLength, 24,5, TFT_GREEN); // Draw the white background bar
  tft.drawRoundRect(margin, tft.height()/2-12, maxBarLength, 24,5, TFT_BLACK);
  tft.pushImage(margin, tft.height()/2-15,30,30,empty_sun,TFT_WHITE);
  tft.pushImage(tft.width()-4*margin, tft.height()/2-15,30,30,full_sun,TFT_WHITE);
}

void drawSelectionBar(int16_t y, bool isSelected, int currentIndex, int totalItems) {
    if (isSelected) {
      if (currentMenu != MAIN_MENU && currentMenu != MANUAL) {
        if (currentIndex == 0) {
            // If it is the last element, the rectangle is red and smaller.
            tft.fillRoundRect(5, 0, 70, 15, 5, TFT_RED);
            tft.drawRoundRect(5, 0, 70, 15, 5, TFT_BLACK);
        } else if (currentMenu != COLOR && currentMenu != BACKGROUND  && currentMenu != TEXT_TYPE  && currentMenu != SELECTION_BAR && currentMenu != SELECTION_BORDER && currentMenu != TEXT) {
            // For other elements, the rectangle has the standard colour and size
            tft.fillRoundRect(tft.width()/4, y-2, tft.width()/1.4, 30,10, selectbarpossibleColors[selectedBarColor]);
            tft.drawRoundRect(tft.width()/4, y-2, tft.width()/1.4, 30,10, borderpossibleColors[selectedBorderColor]);
        } else if (currentMenu == TEXT_TYPE){
          tft.fillRoundRect(15, y-2, 75, 20,5, selectbarpossibleColors[selectedBarColor]);
          tft.drawRoundRect(15, y-2, 75, 20,5, borderpossibleColors[selectedBorderColor]);
        } 
        else {
          tft.fillRoundRect(1, y-2, 158, 20,5, selectbarpossibleColors[selectedBarColor]);
          tft.drawRoundRect(1, y-2, 158, 20,5, borderpossibleColors[selectedBorderColor]);
        }
      } else if (currentMenu == MAIN_MENU) {
        if (currentIndex != 0){
          tft.fillRoundRect(tft.width()/4, y-2, tft.width()/1.4, 30,10, selectbarpossibleColors[selectedBarColor]);
          tft.drawRoundRect(tft.width()/4, y-2, tft.width()/1.4, 30,10, borderpossibleColors[selectedBorderColor]);
        }
      } else if (currentMenu == MANUAL) {
        if (currentIndex != 0){
          tft.fillRoundRect(y, 17, 60, 15,5, selectbarpossibleColors[selectedBarColor]);
          tft.drawRoundRect(y, 17, 60, 15,5, borderpossibleColors[selectedBorderColor]);
        } else{
          tft.fillRoundRect(5, 0, 70, 15, 5, TFT_RED);
          tft.drawRoundRect(5, 0, 70, 15, 5, TFT_BLACK);
        }
      } else{
        tft.fillRoundRect(tft.width()/5, y+2, 90, 15,5, selectbarpossibleColors[selectedBarColor]);
        tft.drawRoundRect(tft.width()/5, y+2, 90, 15,5, borderpossibleColors[selectedBorderColor]);}
    }
}