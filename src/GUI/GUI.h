#ifndef GUI_H
#define GUI_H

#include <globals/globals.h>
#include <libraries.h>

/**
 * @file GUI.h
 * @brief Graphical User Interface (GUI) module for managing the display elements.
 * 
 * This header file contains the function declarations for managing the graphical user interface (GUI) of the system.
 * The functions provided here allow drawing different UI elements like needles, dials, selection bars, and adjusting
 * the screen's brightness. This module interacts with the hardware display and manages visual elements based on user input
 * or system state changes.
 * 
 * The primary purpose of this module is to offer functions that facilitate displaying information in a structured and 
 * visually intuitive manner on the screen, including dynamic updates such as moving the needle on a dial, changing
 * brightness levels, and displaying the date and time.
 * 
 * It is assumed that the GUI interacts with a TFT display and uses a rotary encoder for user input, as well as 
 * other UI elements such as selection bars for menu navigation.
 * 
 * The file also defines various graphical components like brightness bars, dials, and selection bars.
 * 
 * Dependencies:
 * - This file relies on the global variables defined in `globals.h` and the libraries included in `libraries.h`.
 * 
 * @author Juan Alberto Serrano Redondo.
 */

/**
 * @brief Draws a needle on the dial.
 * 
 * This function draws a needle on the dial at the specified angle and length, using the given color.
 * 
 * @param angle The angle at which to draw the needle (in degrees).
 * @param length The length of the needle.
 * @param color The color of the needle, represented as a 32-bit unsigned integer (usually RGB color).
 */
void drawNeedle(int angle, int length, uint32_t color);

/**
 * @brief Draws the dial marks.
 * 
 * This function draws the marks around the dial, including numbers at angles 0, 60, 120, etc.
 * It is used to set up the dial's appearance with evenly spaced markings.
 */
void drawDialMarks();

/**
 * @brief Draws a selection bar on the screen.
 * 
 * This function draws a selection bar at the specified vertical position. 
 * The bar will be drawn differently based on whether it is selected or not, 
 * and its appearance can change depending on the current and total items.
 * 
 * @param y Vertical position of the bar (in pixels).
 * @param isSelected A boolean flag indicating whether the bar is selected.
 * @param currentIndex Index of the currently selected item (starting from 0).
 * @param totalItems Total number of items in the selection list.
 */
void drawSelectionBar(int16_t y, bool isSelected, int currentIndex, int totalItems);

/**
 * @brief Updates the dial by moving the needle to the new position.
 * 
 * This function is responsible for refreshing the dial display by updating the needle's position 
 * based on new data or changes in the system state.
 */
void updateDial();

/**
 * @brief Sets the screen brightness level.
 * 
 * This function adjusts the brightness level of the screen based on the provided level value.
 * 
 * @param level The brightness level to set (usually between a predefined min and max value).
 */
void setBrightness(int level);

/**
 * @brief Draws the brightness bar on the screen.
 * 
 * This function draws a visual representation of the current brightness level on the screen, 
 * showing the user the current setting.
 * 
 * @param level Current brightness level.
 */
void drawBrightnessBar(int level);

/**
 * @brief Displays the date and time on the screen and in the serial monitor.
 * 
 * This function prints the current date and time on the screen, as well as in the serial monitor for debugging purposes.
 * 
 * @param dt The current date and time in RtcDateTime format, typically retrieved from a real-time clock (RTC).
 */
void printDateTime(const RtcDateTime& dt);

#endif // GUI_H
