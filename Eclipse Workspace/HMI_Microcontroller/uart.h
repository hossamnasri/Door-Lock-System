 /*======================================
 *
 * Author: Hossam Nasr
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 *=======================================*/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
#include "micro_config.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	FIVE_BITS, SIX_BITS, SEVEN_BITS, EIGHT_BITS, NINE_BITS = 7
}UART_NumberOfBits;

typedef enum
{
	ONE_STOP_BIT, TWO_STOP_BIT
}UART_StopBits;

typedef enum
{
	DISABLED, RESERVED, EVEN_PARITY, ODD_PARITY
}UART_ParityBitType;

typedef struct
{
	uint32 baud_rate;
	UART_NumberOfBits number_of_bits;
	UART_StopBits number_of_stop_bits;
	UART_ParityBitType parity_bit_type;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
