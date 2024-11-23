/**
 * @file libraries.h
 * @author Juan Alberto Serrano Redondo
 * @brief This header file groups all the libraries and external dependencies required for the project.
 * Including this file in the code ensures the correct inclusion of all necessary libraries,
 * simplifying dependency management and keeping the main code clean and organized.
 *
 * Included libraries:
 *  - Arduino.h: Provides essential functions for interacting with Arduino hardware.
 *  - FreeRTOS: Enables the use of the real-time operating system (RTOS) on compatible microcontrollers.
 *  - BfButton: Provides advanced functionalities for handling buttons.
 *  - TFT_eSPI: Facilitates communication and control of TFT displays.
 *  - EthernetENC: Provides support for Ethernet communication using the ENC28J60 chip.
 *  - SPI: Serial Peripheral Interface communication protocol.
 *  - ThreeWire: Library for communication on three-wire buses.
 *  - RtcDS1302: Provides functions for interacting with the DS1302 RTC module.
 *  - AccelStepper: Enables control of stepper motors.
 *  - Icons: Defines all icons used in the project's graphical interface.
 *  - Fonts: Defines the fonts used in the project.
 * 
 * Including this file in any part of the project ensures that all necessary dependencies 
 * are available and correctly configured.
 */

#ifndef LIBRARIES_H
#define LIBRARIES_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <BfButton.h>
#include <TFT_eSPI.h>
#include <EthernetENC.h>
#include <SPI.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <AccelStepper.h>
#include "icons/images.h" // Includes all necessary icons
#include <custom_fonts/custom_fonts.h>

#endif // LIBRARIES_H