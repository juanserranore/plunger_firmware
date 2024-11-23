/**
 * @file motor.cpp
 * @author Juan Alberto Serrano Redondo
 * @brief Implementation file for motor control functions.
 * 
 * This file contains the implementation of functions used to control the
 * stepper motor, including moving the motor a specified number of steps.
 */

#include "motor.h"
#include <globals/globals.h>
#include <libraries.h>


/**
 * @brief Moves the stepper motor a specified number of steps.
 *
 * This function controls the stepper motor by setting the direction pin and
 * generating step pulses to move the motor. It updates the motor's position
 * and refreshes the screen if the current menu is MANUAL or AUTO.
 *
 * @param steps The number of steps to move the motor. Positive values move
 *              the motor in one direction, and negative values move it in the
 *              opposite direction.
 */
void moveMotor(int steps) {
  bool dir = steps > 0;  // Determine the direction of movement
  digitalWrite(DIR_PIN, dir);  // Configure the motor direction
  steps = abs(steps);  // Ensure that the steps are positive
  // Loop to move the motor the indicated number of steps
  for (int i = 0; i < steps; i++) {
    // Generate a pulse to move the motor one step.
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000); 
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000); 
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);

    // Update position based on direction
    if (dir) {
      position++;
    } else {
      position--;
    }

    // Adjust the position if it's less than 0 (wrap around between 0 and 359 degrees)
    if (position < 0) position = 359;

    // If the current menu is MANUAL or AUTO, update the display
    if (currentMenu == MANUAL || currentMenu == AUTO) {
      ScreenUpdateCommand command = UPDATE_MAIN_MENU;
      xQueueSend(screenUpdateQueue, &command, portMAX_DELAY);
    }
  }
}