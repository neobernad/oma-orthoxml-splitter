################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/utils/orthoXMLFile/OrthoXMLFile.cpp 

OBJS += \
./src/utils/orthoXMLFile/OrthoXMLFile.o 

CPP_DEPS += \
./src/utils/orthoXMLFile/OrthoXMLFile.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/orthoXMLFile/%.o: ../src/utils/orthoXMLFile/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++11 -DDEBUG --I"/usr/include/libxml2/" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


