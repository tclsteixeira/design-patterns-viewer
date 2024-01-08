################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/filepaths.c \
../src/globals.c \
../src/main.c \
../src/viewinterface.c 

C_DEPS += \
./src/filepaths.d \
./src/globals.d \
./src/main.d \
./src/viewinterface.d 

OBJS += \
./src/filepaths.o \
./src/globals.o \
./src/main.o \
./src/viewinterface.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc `pkg-config --cflags gtk+-3.0 gtksourceview-3.0 pango gobject-2.0` -DDEBUG_MODE -I/usr/include -I/usr/include/pango-1.0/pango -I/usr/include/gtksourceview-3.0 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/lib/gcc/x86_64-redhat-linux/13/include -O0 -g3 -p -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/filepaths.d ./src/filepaths.o ./src/globals.d ./src/globals.o ./src/main.d ./src/main.o ./src/viewinterface.d ./src/viewinterface.o

.PHONY: clean-src

