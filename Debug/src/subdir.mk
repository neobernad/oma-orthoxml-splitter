################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/oma-orthoxml-splitter.cpp 

OBJS += \
./src/oma-orthoxml-splitter.o 

CPP_DEPS += \
./src/oma-orthoxml-splitter.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++11 -DDEBUG -I"/usr/include/libxml2/" -O3 -Wall -fopenmp -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


