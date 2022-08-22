################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Buzzer.c \
../Control_MCU.c \
../DCmotor.c \
../GPIO.c \
../eeprom.c \
../i2c.c \
../timer.c \
../uart.c 

OBJS += \
./Buzzer.o \
./Control_MCU.o \
./DCmotor.o \
./GPIO.o \
./eeprom.o \
./i2c.o \
./timer.o \
./uart.o 

C_DEPS += \
./Buzzer.d \
./Control_MCU.d \
./DCmotor.d \
./GPIO.d \
./eeprom.d \
./i2c.d \
./timer.d \
./uart.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


