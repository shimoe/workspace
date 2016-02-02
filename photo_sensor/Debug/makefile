################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: photo_sensor.axf

# Tool invocations
photo_sensor.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:/nxp/LPCXpresso_7.9.2_493/workspace/workspace/photo_sensor/lib" -Xlinker --gc-sections -Xlinker -Map=photo_sensor.map -mcpu=cortex-m3 -mthumb -T "../Debug/vs-wrc103lv_Debug.ld" -o "photo_sensor.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) photo_sensor.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size photo_sensor.axf; arm-none-eabi-size photo_sensor.axf; arm-none-eabi-objcopy -O binary photo_sensor.axf photo_sensor.bin; checksum photo_sensor.bin;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
