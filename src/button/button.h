/**
 * @file button.h
 * @brief Header file for button press handling functions.
 *
 * This file contains the declarations of the button press handling functions,
 * including single, double, and long press actions for navigating the menu
 * and controlling various settings.
 */

#ifndef BUTTON_H
#define BUTTON_H
#include <libraries.h>

/**
 * @brief Handles single button press events.
 *
 * This function is called when a single press is detected. It manages the 
 * navigation through different menus and performs specific actions based on 
 * the current menu and selection.
 *
 * @param btn Pointer to the button object.
 * @param pattern The press pattern detected (single press in this case).
 */
void singlePressHandler(BfButton *btn, BfButton::press_pattern_t pattern);

/**
 * @brief Handles double button press events.
 *
 * This function is called when a double press is detected. It toggles the 
 * engineering mode or any other logic that needs to be implemented for a 
 * double press event.
 *
 * @param btn Pointer to the button object.
 * @param pattern The press pattern detected (double press in this case).
 */
void doublePressHandler(BfButton *btn, BfButton::press_pattern_t pattern);

/**
 * @brief Handles long button press events.
 *
 * This function is called when a long press is detected. It toggles the screen 
 * on/off, adjusts the brightness, and manages the sleep mode of the display.
 *
 * @param btn Pointer to the button object.
 * @param pattern The press pattern detected (long press in this case).
 */
void longPressHandler(BfButton *btn, BfButton::press_pattern_t pattern);

#endif // BUTTON_H