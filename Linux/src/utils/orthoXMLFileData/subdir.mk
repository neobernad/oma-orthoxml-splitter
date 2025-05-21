################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/utils/orthoXMLFileData/OrthoXMLFileData.cpp 

OBJS += \
./src/utils/orthoXMLFileData/OrthoXMLFileData.o 

CPP_DEPS += \
./src/utils/orthoXMLFileData/OrthoXMLFileData.d 


# Each subdirectory must supply rules for building sources it contributes
src/utils/orthoXMLFileData/%.o: ../src/utils/orthoXMLFileData/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -DDEBUG -I"/usr/include/libxml2" -O3 -Wall -fopenmp -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


