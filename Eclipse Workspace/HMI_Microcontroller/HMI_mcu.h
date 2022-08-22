 /*======================================
 *
 * Author: Hossam Nasr
 *
 * Module: HMI
 *
 * File Name: HMI_mcu.h
 *
 *=======================================*/



#ifndef HMI_MCU_H_
#define HMI_MCU_H_


#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Definitions for Hand Shaking Protocol */
#define OPENING_DOOR          			0xF0
#define WRONG_PASSWORD        			0xF1
#define CHANGING_PASSWORD     			0xF2
#define OPEN_DOOR             			'+'
#define CHANGE_PASSWORD       			'-'
#define READY_TO_SEND         			0xF3
#define READY_TO_RECEIVE      			0xF4
#define RECEIVE_DONE          			0xF5
#define SEND_FIRST_PASSWORD   			0xF6
#define SEND_SECOND_PASSWORD 			0xF7
#define SEND_CHECK_PASSWORD   			0xF8

/* Definitions for Password */
#define PASSWORD_LENGTH         		5
#define MAX_NUM_OF_MISTAKES     		3
#define PASS_MIS_MATCHED              	0
#define PASS_MATCHED				  	1

/* Definitions for Time Periods */
#define SEND_RECEIVE_TIME      			10
#define STAND_PRESENTATION_TIME         1500
#define KEYPAD_CLICK_TIME         		500
#define OPEN_DOOR_TIME      			15
#define HOLD_DOOR_TIME       			3
#define CLOSE_DOOR_TIME      			15
#define WARNING_TIME           			60

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Function to set the call back function for the Interrupt to be executed
 */
void HMI_TimerCallBackProcessing(void);

/*
 * Description:
 * Function to initialize Timer to operate depending on specific configuration
 * Aim of Timer --> Count the period for specific operations
 */
void HMI_startTimer(void);

/*
 * Description:
 * Function to send specific commands to the CONTROL MCU through UART
 */
void HMI_sendCommand(uint8 g_command);

/*
 * Description:
 * Function to receive specific command from the CONTROL MCU through UART
 * and Direct the MCU to do predefined set of operations
 */
uint8 HMI_receiveCommand(void);

/*
 * Description:
 * Function to set a new Password
 * Use cases:
 * 			 --> Set the Password for the first time
 *      	 --> When user need to change the Password
 */
void HMI_newPassword(void);

/*
 * Description:
 * Function that takes Password characters form array
 * and Send that password to the CONTROL MCU through UART
 */
void HMI_sendPassword(uint8 a_inputPassword[]);

/*
 * Description:
 * Function that takes Password from Keypad
 * and Store it in array for later use
 * and Display asterisk on the screen
 */
void HMI_getPassword(uint8 a_inputPassword[]);

/*
 * Description:
 * Function that displays the main options for our project
 */
void HMI_mainOptions(void);

/*
 * Description:
 * Prompt the user to input a password
 */
void HMI_promptPassword(void);

/*
 * Description:
 * Function that explain door phase on the screen
 */
void HMI_openingDoor(void);

/*
 * Description:
 * Function that take care of wrong password scenarios
 */
void HMI_wrongPassword(void);


#endif /* HMI_MCU_H_ */
