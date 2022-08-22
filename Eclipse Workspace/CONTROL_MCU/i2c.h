 /*======================================
 *
 * Author: Hossam Nasr
 *
 * Module: I2C
 *
 * File Name: i2c.h
 *
 *=======================================*/


#ifndef I2C_H_
#define I2C_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	NormalMode_100Kb = 100000,
	FastMode_400Kb = 400000,
	FastModePlus_1Mb = 1000000,
	HighSpeedMode_3Mb = 3400000
}SCL_Frquency;

typedef enum
{
	Prescaler_1, Prescaler_4, Prescaler_16, Prescaler_64
}TWI_Prescaler;

typedef struct
{
	SCL_Frquency mode;
	TWI_Prescaler prescaler;
	uint8 slaveAddress;
}TWI_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(const TWI_ConfigType * Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8 data);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);


#endif /* I2C_H_ */
