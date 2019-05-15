################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../IR.c \
../btn.c \
../fsm.c \
../joystick.c \
../kbhit.c \
../keyboard.c \
../lcdpiTankGo.c \
../piTankGo_1.c \
../player.c \
../tmr.c \
../torreta.c \
../ultrasonic.c 

OBJS += \
./IR.o \
./btn.o \
./fsm.o \
./joystick.o \
./kbhit.o \
./keyboard.o \
./lcdpiTankGo.o \
./piTankGo_1.o \
./player.o \
./tmr.o \
./torreta.o \
./ultrasonic.o 

C_DEPS += \
./IR.d \
./btn.d \
./fsm.d \
./joystick.d \
./kbhit.d \
./keyboard.d \
./lcdpiTankGo.d \
./piTankGo_1.d \
./player.d \
./tmr.d \
./torreta.d \
./ultrasonic.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"C:\SysGCC\Raspberry\include" -I"C:\SysGCC\Raspberry\include\wiringpi" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


