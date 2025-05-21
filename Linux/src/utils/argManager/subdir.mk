################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/utils/argManager/argManager.cpp 

OBJS += \
./src/utils/argManager/argManager.o 

CPP_DEPS += \
./src/utils/argManager/argManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/argManager/%.o: ../src/utils/argManager/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -DDEBUG -I"/usr/include/libxml2" -O3 -Wall -fopenmp -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


