 /*======================================
 *
 * Author: Hossam Nasr
 *
 * Module: Buzzer
 *
 * File Name: Buzzer.c
 *
 *=======================================*/

#include "gpio.h"
#include "buzzer.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * 				Initialize Buzzer by adjusting the direction of its Pin as Output
 */
void Buzzer_Init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, PIN_OUTPUT);
} /* End Buzzer_Init Function */



/*
 * Description :
 * 				Turn On the Buzzer
 */
void Buzzer_On(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
} /* End Buzzer_On Function */



/*
 * Description :
 * 				Turn Off the Buzzer
 */
void Buzzer_Off(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
} /* End Buzzer_Off Function */
