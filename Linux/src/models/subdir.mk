################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/models/xmlNodeCustom.cpp \
../src/models/xmlNodeOrthoGroups.cpp \
../src/models/xmlNodeSpecies.cpp 

OBJS += \
./src/models/xmlNodeCustom.o \
./src/models/xmlNodeOrthoGroups.o \
./src/models/xmlNodeSpecies.o 

CPP_DEPS += \
./src/models/xmlNodeCustom.d \
./src/models/xmlNodeOrthoGroups.d \
./src/models/xmlNodeSpecies.d 


# Each subdirectory must supply rules for building sources it contributes
src/models/%.o: ../src/models/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -DDEBUG -I"/usr/include/libxml2" -O3 -Wall -fopenmp -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


