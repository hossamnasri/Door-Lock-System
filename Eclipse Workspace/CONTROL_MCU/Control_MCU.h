 /*======================================
 *
 * Author: Hossam Nasr
 *
 * Module: Control_MCU
 *
 * File Name: Control_MCU.h
 *
 *=======================================*/



#ifndef CONTROL_MCU_H_
#define CONTROL_MCU_H_

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
#define OPEN_DOOR_TIME      			15
#define HOLD_DOOR_TIME       			3
#define CLOSE_DOOR_TIME      			15
#define WARNING_TIME           			60
#define STORING_TIME           			80

/* Definitions for TWI */
#define TWI_ADDRESS    0b0000001

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description:
 * Function to set the call back function for the Interrupt to be executed
 */
void CONTROL_TimerCallBackProcessing(void);

/*
 * Description:
 * Function to initialize Timer to operate depending on specific configuration
 * Aim of Timer --> Count the period for specific operations
 */
void CONTROL_startTimer(void);

/*
 * Description:
 * Function to set a new Password
 * Use cases:
 * 			 --> Set the Password for the first time
 *      	 --> When user need to change the Password
 */
void CONTROL_newPassword(void);

/*
 * Description :
 * Takes the Password which received from UART
 * and Store it in an array for later use
 */
void CONTROL_receivePassword(uint8 a_Password[]);

/*
 * Description :
 * Function to compare two passwords received from HMI MCU
 * and return the match status between them
 */
uint8 CONTROL_comparePasswords(uint8 a_password1[], uint8 a_password2[]);

/*
 * Description :
 * Function that save the matched password in external EEPROM
 */
void CONTROL_savePassword(uint8 a_receivedPassword[]);

/*
 * Description :
 * Load the Password from EEPROM
 * and Store it in an array
 */
void CONTROL_readPassword(uint8 a_storedPassword[]);

/*
 * Description:
 * Function that rotates the DC Motor
 */
void CONTROL_openingDoor(void);

/*
 * Description:
 * Function that take care of wrong password scenarios
 */
void CONTROL_wrongPassword(void);

/*
 * Description:
 * Function to send specific commands to the HMI MCU through UART
 */
void CONTROL_sendCommand(uint8 g_command);

/*
 * Description:
 * Function to receive specific command from the HMI MCU through UART
 * and Direct the MCU to do predefined set of operations
 */
uint8 CONTROL_receiveCommand(void);

#endif /* CONTROL_MCU_H_ */
