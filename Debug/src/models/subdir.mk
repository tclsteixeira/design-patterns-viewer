################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/models/base.c \
../src/models/dpcatmodel.c \
../src/models/dpcodemodel.c \
../src/models/dpmodel.c \
../src/models/dppartmodel.c \
../src/models/dpucasemodel.c \
../src/models/records.c 

C_DEPS += \
./src/models/base.d \
./src/models/dpcatmodel.d \
./src/models/dpcodemodel.d \
./src/models/dpmodel.d \
./src/models/dppartmodel.d \
./src/models/dpucasemodel.d \
./src/models/records.d 

OBJS += \
./src/models/base.o \
./src/models/dpcatmodel.o \
./src/models/dpcodemodel.o \
./src/models/dpmodel.o \
./src/models/dppartmodel.o \
./src/models/dpucasemodel.o \
./src/models/records.o 


# Each subdirectory must supply rules for building sources it contributes
src/models/%.o: ../src/models/%.c src/models/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc `pkg-config --cflags gtk+-3.0 gtksourceview-3.0 pango gobject-2.0` -DDEBUG_MODE -I/usr/include -I/usr/include/pango-1.0/pango -I/usr/include/gtksourceview-3.0 -I/usr/include/glib-2.0 -I/usr/include/gtk-3.0 -I/usr/lib/gcc/x86_64-redhat-linux/13/include -O0 -g3 -p -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-models

clean-src-2f-models:
	-$(RM) ./src/models/base.d ./src/models/base.o ./src/models/dpcatmodel.d ./src/models/dpcatmodel.o ./src/models/dpcodemodel.d ./src/models/dpcodemodel.o ./src/models/dpmodel.d ./src/models/dpmodel.o ./src/models/dppartmodel.d ./src/models/dppartmodel.o ./src/models/dpucasemodel.d ./src/models/dpucasemodel.o ./src/models/records.d ./src/models/records.o

.PHONY: clean-src-2f-models

