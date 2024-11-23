/// @file main.cpp
/// @brief Main project file
/*! 
 *
 * @mainpage 
 * 
 * @section intro_sec Introduction
 * 
 * This project handles user interface interactions using a rotary encoder and buttons,
 * allowing for navigation through menus, adjustment of settings, and controlling a display.
 * It implements a multitasking approach using FreeRTOS to manage various tasks concurrently.
 * 
 * @section dependencies 
 * Dependencies:
 * The libraries needed for this to compile are:
 * - Arduino.h
 * - FreeRTOS
 * - BfButton
 * - TFT_eSPI
 * - EthernetENC
 * - SPI
 * - ThreeWire
 * - RtcDS1302
 * - AccelStepper
 * 
 * @section author Author
 * 
 * Written by GranaSAT Students:
 *
 * @author Juan Alberto Serrano Redondo <juan@ugr.es>
 * @author Prof. Andrés Roldán Aranda  <amroldan@ugr.es>
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 * 
 * @section Flow Flow chart of the project
 * ![](flow_chart.png)
 * ![](tareas.png)
 * 
 * @section HISTORY
 *
 * V00 (07/2024):
 * - Implemented rotary encoder input handling.
 * - Integrated button press detection (single, double, long presses).
 * - Configured display updates based on menu navigation.
 * - Developed tasks using FreeRTOS for efficient multitasking.
 * V00.1 (08/2024):
 * - Added Ethernet communication features for remote monitoring.
 * - Implemented real-time clock (RTC) functionality for accurate time management.
 * - Enhanced display functionality to show system status and settings.
 * - Improved input handling to support simultaneous button and encoder interactions.
 * - Introduced settings menu for configuring system parameters.
 * - Added logging functionality for troubleshooting and performance analysis.
 * - Optimized task scheduling to reduce CPU load and improve responsiveness.
 * - Refined user interface design for better user experience on the TFT display.
 *
 * V01 (10/2024):
 * - Implemented data visualization features for displaying sensor readings.
 * - Developed communication protocols for data transmission over Ethernet.
 * - Conducted extensive testing and debugging to ensure system reliability.
 * - Refined rotary encoder response for smoother menu navigation.
 * - Implemented power management features to reduce energy consumption.
 * - Expanded documentation and comments for better maintainability.
 * 
 * @brief Main file containing the setup and main loop functions.
 *
 * This file includes the global configurations and necessary libraries
 * for the project. The setup function initializes all configurations, 
 * including RTOS (Real-Time Operating System) tasks, which manage the entire 
 * functionality of the system.
 *
 * The main loop (`void loop()`) is intentionally left empty, as the RTOS tasks 
 * are responsible for the continuous execution of system tasks, following a 
 * multi-threading approach. The tasks are created and managed in the setup 
 * function.
*/

// Include global variables and configuration headers
#include <globals/globals.h>
#include <libraries.h>

/**
 * @brief Initial system configuration and RTOS task setup.
 *
 * The setup function is responsible for initializing the system's peripherals 
 * and configuring the RTOS tasks. The use of an RTOS eliminates the need for 
 * traditional looping within the main loop function.
 */
#include <setup/setup.h>

void loop() {
  
}