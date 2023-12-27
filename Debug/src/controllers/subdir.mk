################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/controllers/dpcatcontroller.c \
../src/controllers/dpcodecontroller.c \
../src/controllers/dppartcontroller.c \
../src/controllers/dpucasescontroller.c 

C_DEPS += \
./src/controllers/dpcatcontroller.d \
./src/controllers/dpcodecontroller.d \
./src/controllers/dppartcontroller.d \
./src/controllers/dpucasescontroller.d 

OBJS += \
./src/controllers/dpcatcontroller.o \
./src/controllers/dpcodecontroller.o \
./src/controllers/dppartcontroller.o \
./src/controllers/dpucasescontroller.o 


# Each subdirectory must supply rules for building sources it contributes
src/controllers/%.o: ../src/controllers/%.c src/controllers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc  `pkg-config --cflags gtk+-3.0 gtksourceview-3.0 pango gobject-2.0` -I/usr/include -I/usr/include/pango-1.0/pango -I/usr/include/gtksourceview-3.0 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/lib/gcc/x86_64-redhat-linux/13/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-controllers

clean-src-2f-controllers:
	-$(RM) ./src/controllers/dpcatcontroller.d ./src/controllers/dpcatcontroller.o ./src/controllers/dpcodecontroller.d ./src/controllers/dpcodecontroller.o ./src/controllers/dppartcontroller.d ./src/controllers/dppartcontroller.o ./src/controllers/dpucasescontroller.d ./src/controllers/dpucasescontroller.o

.PHONY: clean-src-2f-controllers

