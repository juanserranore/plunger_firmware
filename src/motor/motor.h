/**
 * @file motor.h
 * @author Juan Alberto Serrano Redondo
 * @brief Header file for motor control functions.
 *
 * This file contains the declarations of functions and variables used for controlling
 * a stepper motor. It defines the necessary pins and functions to move the motor
 * and update the screen based on the motor's position.
 */

#ifndef MOTOR_H
#define MOTOR_H

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
void moveMotor(int steps);

#endif // MOTOR_H