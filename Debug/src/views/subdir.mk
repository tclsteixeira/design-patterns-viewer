################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/views/mainWindow.c \
../src/views/messageWindow.c 

C_DEPS += \
./src/views/mainWindow.d \
./src/views/messageWindow.d 

OBJS += \
./src/views/mainWindow.o \
./src/views/messageWindow.o 


# Each subdirectory must supply rules for building sources it contributes
src/views/%.o: ../src/views/%.c src/views/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc `pkg-config --cflags gtk+-3.0 gtksourceview-3.0 pango gobject-2.0` -DDEBUG_MODE -I/usr/include -I/usr/include/pango-1.0/pango -I/usr/include/gtksourceview-3.0 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/lib/gcc/x86_64-redhat-linux/13/include -O0 -g3 -p -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-views

clean-src-2f-views:
	-$(RM) ./src/views/mainWindow.d ./src/views/mainWindow.o ./src/views/messageWindow.d ./src/views/messageWindow.o

.PHONY: clean-src-2f-views

