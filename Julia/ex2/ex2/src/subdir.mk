################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/DocumentErrorHandler.cpp \
../src/GasNode.cpp \
../src/ex2.cpp \
../src/tinyxml2.cpp 

OBJS += \
./src/DocumentErrorHandler.o \
./src/GasNode.o \
./src/ex2.o \
./src/tinyxml2.o 

CPP_DEPS += \
./src/DocumentErrorHandler.d \
./src/GasNode.d \
./src/ex2.d \
./src/tinyxml2.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


