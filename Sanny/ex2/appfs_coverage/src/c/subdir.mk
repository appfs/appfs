################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/c/MeasurePoint.cpp \
../src/c/MeasurePointFromDocumentReader.cpp \
../src/c/ParserErrorHandler.cpp \
../src/c/XMLParser.cpp \
../src/c/ex2.cpp 

OBJS += \
./src/c/MeasurePoint.o \
./src/c/MeasurePointFromDocumentReader.o \
./src/c/ParserErrorHandler.o \
./src/c/XMLParser.o \
./src/c/ex2.o 

CPP_DEPS += \
./src/c/MeasurePoint.d \
./src/c/MeasurePointFromDocumentReader.d \
./src/c/ParserErrorHandler.d \
./src/c/XMLParser.d \
./src/c/ex2.d 


# Each subdirectory must supply rules for building sources it contributes
src/c/%.o: ../src/c/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++0x -O0 -g3 -ftest-coverage -fprofile-arcs -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


