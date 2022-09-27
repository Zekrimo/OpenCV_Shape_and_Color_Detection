################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Main.cpp \
../ShapeAndColorDetection.cpp 

CPP_DEPS += \
./Main.d \
./ShapeAndColorDetection.d 

OBJS += \
./Main.o \
./ShapeAndColorDetection.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/opencv4 -O0 -g3 -Wall -c -fmessage-length=0 $(shell pkg-config --cflags opencv4) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./Main.d ./Main.o ./ShapeAndColorDetection.d ./ShapeAndColorDetection.o

.PHONY: clean--2e-

