 /*======================================
 *
 * Author: Hossam Nasr
 *
 * Module: EEPROM
 *
 * File Name: eeprom.h
 *
 *=======================================*/


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

#define TWI_ADDRESS 0b0000001

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);

#endif /* EXTERNAL_EEPROM_H_ */
