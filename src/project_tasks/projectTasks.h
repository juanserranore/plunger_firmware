/**
 * @file projectTasks.h
 * @brief Header file for system task management.
 * 
 * This header file contains function declarations for managing system tasks,
 * user interface on the screen, Ethernet communication, stepper motor control, 
 * and rotary encoder handling.
 * 
 * @note Ensure necessary libraries and definitions are included in the corresponding .cpp file.
 * @note Adjust constants and configurations according to hardware requirements.
 */

#ifndef PROJECTTASK_H
#define PROJECTTASK_H
#include <libraries.h>

// LCD tasks
void taskUpdateScreen(void *pvParameters);
void taskStartScreen(void *pvParameters);
void taskShowTime(void *pvParameters);
void taskScreenTimeout(void *pvParameters);

// Motor tasks
void rotarymotorTask(void *pvParameters);
void motorTask(void *pvParameters);

// Ethernet tasks
void taskEthernet(void *pvParameters);

// Rotary encoder tasks
void taskEncoder(void *pvParameters);
void taskButtonPress(void *pvParameters);

#endif // PROJECTTASK_H