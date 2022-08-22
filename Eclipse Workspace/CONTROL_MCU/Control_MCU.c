 /*======================================
 *
 * Author: Hossam Nasr
 *
 * Module: Control_MCU
 *
 * File Name: Control_MCU.c
 *
 *=======================================*/



/*******************************************************************************
 *                                  Modules                                    *
 *******************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "micro_config.h"

#include "Control_MCU.h"
#include "eeprom.h"
#include "DCmotor.h"
#include "buzzer.h"
#include "i2c.h"
#include "i2c.h"
#include "timer.h"

/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/

/* Global array to store the password saved in the external EEPROM */
uint8 g_storedPassword[PASSWORD_LENGTH];

/* Global array to store the first password inputed from the user */
uint8 g_receivedPassword[PASSWORD_LENGTH];

/* Global array to store the second password inputed from the user */
uint8 g_confirmPassword[PASSWORD_LENGTH];

/* Global Variable to store the status of the Password after comparing */
uint8 g_matchStatus = PASS_MIS_MATCHED;

/* Global Variable to keep track of the seconds counted by the timer */
uint8 g_tick = 0;

/* Global Variable to keep track of how many times the user has inputed the password incorrectly */
uint8 g_passwordMistakes = 0;

/* Global Variable to keep track of the command sent from the CONTROL MCU through UART */
uint8 g_command;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/* Function main begins program execution */
int main(void)
{
	/**************************** Initialization Code ****************************/

	/* Variable to store the pressed key */
	uint8 key_option;

	SREG  |= ( 1 << 7 ); /* Enable Global Interrupts */

	/* Initialize the UART with Configuration */
	UART_ConfigType UART_Config = {9600, EIGHT_BITS, ONE_STOP_BIT, DISABLED};
	UART_init(&UART_Config);

	/* Initialize TWI with Configuration */
	TWI_ConfigType TWI_Config = {FastMode_400Kb, Prescaler_1, TWI_ADDRESS};
	TWI_init(&TWI_Config);

	/* Initialize DC Motor */
	DcMotor_Init();

	/* Initialize Buzzer */
	Buzzer_Init();

	/* Save the Password for the first time */
	CONTROL_newPassword();

	/* Super Loop */
	for(;;)
	{
		/**************************** Application Code ****************************/

		/* Wait until the HMI MCU send the inputed password */
		while(CONTROL_receiveCommand() != SEND_CHECK_PASSWORD);
		/* Receive the inputed password and store it */
		CONTROL_receivePassword(g_receivedPassword);
		/* Receive the command from the HMI MCU */
		key_option = CONTROL_receiveCommand();

		/* Depending on the pressed key, Perform some operation */
		switch (key_option)
		{

		/************** OPEN DOOR CASE *************/
		case OPEN_DOOR:

			/* Read Password from the EEPROM */
			CONTROL_readPassword(g_storedPassword);
			/* Compare it with the inputed user */
			g_matchStatus = CONTROL_comparePasswords(g_receivedPassword, g_storedPassword);

			/* In case the two passwords matches */
			if(g_matchStatus == PASS_MATCHED)
			{
				/* Send Opening Door command to HMI MCU */
				CONTROL_sendCommand(OPENING_DOOR);
				/* Start Opening Door sequence */
				CONTROL_openingDoor();
			}
			/* In case the two passwords did not match */
			else if(g_matchStatus == PASS_MIS_MATCHED)
			{
				/* Send Wrong Password command to HMI MCU */
				CONTROL_sendCommand(WRONG_PASSWORD);
				/* Start Wrong Password sequence */
				CONTROL_wrongPassword();
			}

			break; /* End of open door case */

			/************* CHANGE PASSWORD CASE *************/
			case CHANGE_PASSWORD:

				/* Read Password from the EEPROM */
				CONTROL_readPassword(g_storedPassword);
				/* Compare it with the inputed user */
				g_matchStatus = CONTROL_comparePasswords(g_receivedPassword, g_storedPassword);

				/* In case the two passwords matches */
				if(g_matchStatus == PASS_MATCHED)
				{
					/* Send Changing Password command to HMI MCU */
					CONTROL_sendCommand(CHANGING_PASSWORD);
					/* Start New Password sequence */
					CONTROL_newPassword();
				}
				/* In case the two passwords did not match */
				else if(g_matchStatus == PASS_MIS_MATCHED)
				{
					/* Send Wrong Password command to HMI MCU */
					CONTROL_sendCommand(WRONG_PASSWORD);
					/* Start Wrong Password sequence */
					CONTROL_wrongPassword();
				}

				break; /* End of change password case */
		} /* End switch case */
	} /* End Super Loop */
} /* End main Function */



void CONTROL_TimerCallBackProcessing(void)
{
	g_tick++; /* Increment the counter */
} /* End HMI_TimerCallBackProcessing Function */



/*
 * Description :
 * Produce a delay timer using Polling Technique
 * TCNT0 = 0	,,	0 --> 7813	,,	Ticks = 7814
 * F)cpu = 8 Mhz // Prescaler = 1024
 * F)timer = F)cpu / Prescaler = ( 8 Mhz ) / 1024								----->	F)timer = 7812.5
 * Resolution = 1 / F)timer = 1 / 7812.5										----->	Resolution = 128 us
 * Maximum suitable number of counts per compare match = 7812.5 Counts			----->	T)ctc = 1000 us
 * Number of compare matches per One Second = 1 / T)ctc = 1 / ( 1000 us )		----->	Number of compare matches per One Second = 1
 */
void CONTROL_startTimer(void)
{
	/* Setup Timer Configuration */
	TIMER_ConfigType TIMER_Config = { TIMER1_ID, TIMER_CTC_Mode, 0, F_CPU_1024, 7813 };

	/* Initialize the Timer */
	Timer_init(&TIMER_Config);

	/* Set Call Back function for the timer */
	Timer_setCallBack(CONTROL_TimerCallBackProcessing, TIMER1_ID);
} /* End HMI_startTimer Function */



void CONTROL_newPassword(void)
{
	/* Set its status at first as mis-matched */
	g_matchStatus = PASS_MIS_MATCHED;

	/* Loop until the HMI MCU get the same password */
	while(g_matchStatus == PASS_MIS_MATCHED)
	{
		/* Wait until the HMI MCU send command for first password */
		while (CONTROL_receiveCommand() != SEND_FIRST_PASSWORD );
		/* Receive the first password from HMI MCU */
		CONTROL_receivePassword(g_receivedPassword);

		/* Wait until the HMI MCU send command for second password */
		while (CONTROL_receiveCommand() != SEND_SECOND_PASSWORD );
		/* Receive the first password from HMI MCU */
		CONTROL_receivePassword(g_confirmPassword);

		/* Compare the Two received passwords */
		g_matchStatus = CONTROL_comparePasswords(g_receivedPassword, g_confirmPassword);

		/* In case the Two Passwords did not match */
		if( g_matchStatus == PASS_MIS_MATCHED )
		{
			/* Send command informing that the passwords mis-matched */
			CONTROL_sendCommand(PASS_MIS_MATCHED);
		}
		/* In case the Two Passwords matches */
		else if( g_matchStatus == PASS_MATCHED )
		{
			CONTROL_savePassword(g_receivedPassword);
			/* Send command informing that the passwords matched */
			CONTROL_sendCommand(PASS_MATCHED);
		}
	} /* End while loop */
} /* End CONTROL_newPassword Function */



void CONTROL_receivePassword(uint8 a_Password[])
{
	uint8 counter; /* Variable to work as a counter */

	/* Loop on the passwords elements */
	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		a_Password[counter] = UART_recieveByte(); /* Receive Password from HMI MCU */
		_delay_ms(SEND_RECEIVE_TIME);      /* Delay for the time gap for sending receiving time between the MCUs */
	} /* End for */
} /* End CONTROL_receivePassword Function */



uint8 CONTROL_comparePasswords(uint8 a_password1[], uint8 a_password2[])
{
	uint8 counter; /* Variable to work as a counter */

	/* Loop on the passwords elements */
	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		if (a_password1[counter] != a_password2[counter])
		{
			return PASS_MIS_MATCHED; /* Return passwords has not matched of one element mis-matched */
		} /* End if */
	} /* End for */

	return PASS_MATCHED; /* Return passwords matches if known of their elements mis-match */
} /* End CONTROL_comparePasswords Function */



void CONTROL_savePassword(uint8 a_receivedPassword[])
{
	uint8 counter; /* Variable to work as a counter */

	/* Loop on the passwords elements */
	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		/* Save each element of the password in external EEPROM */
		EEPROM_writeByte( (0x0311+counter), a_receivedPassword[counter]);
		/* Delay for the time gap for storing data in EEPROM */
		_delay_ms(STORING_TIME);
	} /* End for */
} /* End CONTROL_savePassword Function */



void CONTROL_readPassword(uint8 a_storedPassword[])
{
	uint8 counter; /* Variable to work as a counter */

	/* Loop on the passwords elements */
	for( counter = 0; counter < PASSWORD_LENGTH; counter++)
	{
		/* Read each element of the password in external EEPROM */
		EEPROM_readByte( (0x0311+counter), &a_storedPassword[counter]);
		/* Delay for the time gap for storing data in EEPROM */
		_delay_ms(STORING_TIME);
	} /* End for */
} /* End CONTROL_readPassword Function */



void CONTROL_openingDoor(void)
{
	CONTROL_startTimer(); /* Start the Timer */

	/*
	 * Do Open Door Task:
	 * 					 --> Rotate the DC Motor
	 * 					 --> Clock Wise
	 * 					 --> 15 seconds
	 */
	DcMotor_Rotate(DC_MOTOR_CW);
	while(g_tick != OPEN_DOOR_TIME); /* Count up to 15 */
    g_tick = 0; /* Reset counter to reuse it */

	/*
	 * Do Hold Task:
	 * 					 --> Stop the DC Motor
	 */
    DcMotor_Rotate(DC_MOTOR_STOP);
	while(g_tick != HOLD_DOOR_TIME); /* Count up to 15 */
    g_tick = 0; /* Reset counter to reuse it */

	/*
	 * Do Close Door Task:
	 * 					 --> Rotate the DC Motor
	 * 					 --> Anti Clock Wise
	 * 					 --> 15 seconds
	 */
    DcMotor_Rotate(DC_MOTOR_ACW);
	while(g_tick != CLOSE_DOOR_TIME); /* Count up to 15 */
    g_tick = 0; /* Reset counter to reuse it */

    DcMotor_Rotate(DC_MOTOR_STOP); /* Stop the Motor */

    Timer_DeInit(TIMER1_ID); /* Stop the timer */
} /* End CONTROL_openingDoor Function */



void CONTROL_wrongPassword(void)
{
	g_passwordMistakes++; /* Increment the wrong counter */

	/* If the user entered the password 3 times wrong */
	if(g_passwordMistakes == MAX_NUM_OF_MISTAKES)
	{
		CONTROL_startTimer(); /* Start the timer to measure time period */

		Buzzer_On(); /* Turn on the buzzer */
		while(g_tick != WARNING_TIME); /* Count up to 15 */

		/* Reset the counters */
		g_tick = 0;
		g_passwordMistakes = 0;


		Timer_DeInit(TIMER1_ID); /* Stop the timer */
	} /* End if */

	Buzzer_Off(); /* Turn off the buzzer */
} /* End CONTROL_wrongPassword Function */



void CONTROL_sendCommand(uint8 g_command)
{
	/* Inform HMI MCU that you are to send */
	UART_sendByte(READY_TO_SEND);

	/* Wait until HMI MCU are ready to receive */
	while(UART_recieveByte() != READY_TO_RECEIVE);

	/* Send the required command to the HMI MCU */
	UART_sendByte(g_command);

	/* Wait until the HMI MCU receive the command */
	while(UART_recieveByte() != RECEIVE_DONE);
} /* End CONTROL_sendCommand Function */



uint8 CONTROL_receiveCommand(void)
{
	/* Wait until the HMI MCU is ready to send */
	while(UART_recieveByte() != READY_TO_SEND);

	/* Inform the HMI MCU that you are ready to receive */
	UART_sendByte(READY_TO_RECEIVE);

	/* Receive the command from the CONTROL MCU */
	g_command = UART_recieveByte();

	/* Inform the HMI MCU that the receive has been done successfully */
	UART_sendByte(RECEIVE_DONE);

	return g_command; /* Return the command value */
} /* End CONTROL_receiveCommand Function */
