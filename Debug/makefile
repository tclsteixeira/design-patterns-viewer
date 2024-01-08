################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/views/subdir.mk
-include src/models/subdir.mk
-include src/controllers/subdir.mk
-include src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := DPViewergtk
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: DPViewergtk

# Tool invocations
DPViewergtk: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc `pkg-config --libs gtk+-3.0 gtksourceview-3.0 pango gobject-2.0` -p -o "DPViewergtk" $(OBJS) $(USER_OBJS) $(LIBS) -lsqlite3
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) DPViewergtk
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
