################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/utils/transformer/transformer.cpp 

OBJS += \
./src/utils/transformer/transformer.o 

CPP_DEPS += \
./src/utils/transformer/transformer.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/transformer/%.o: ../src/utils/transformer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -DDEBUG -I"/usr/include/libxml2" -O3 -Wall -fopenmp -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


