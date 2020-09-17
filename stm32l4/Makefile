# modified 1/31/16 by Chris Martin
# make flash now finds where STM board is mounted and
# directly copies the TARGET.bin without the need for stlink

# NOTE: Only using this makefile temporarly to test
# some of the software written
TARGET = keypad-test

CORESRC = ../Core/Src/
COREINC = ../Core/Inc/

OBJS  = $(TARGET).o $(CORESRC)lcd-i2c.o $(CORESRC)timer.o $(CORESRC)leds_hal.o $(CORESRC)calc.o $(CORESRC)stack.o $(CORESRC)keypad.o
INSTALLDIR = /usr/local/stmdev/
ARCH = STM32L476xx

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

INCDIRS = -I$(INSTALLDIR)/include -I$(COREINC)
LIBDIRS = -L$(INSTALLDIR)/lib

LIBS= -lece486_$(ARCH) -l$(ARCH) -lm

LINKSCRIPT = $(INSTALLDIR)/lib/$(ARCH)_FLASH.ld

CFLAGS = -mcpu=cortex-m4 -mthumb -O3 -Wall  \
         -fomit-frame-pointer -fno-strict-aliasing -fdata-sections \
         -include stm32l4xx_hal_conf.h -DARM_MATH_CM4 -D$(ARCH) \
         -mfpu=fpv4-sp-d16 -mfloat-abi=softfp $(INCDIRS) \
         -fsingle-precision-constant -ffunction-sections

LDFLAGS = -u _printf_float \
	  -Wl,-T$(LINKSCRIPT) \
	  -static \
          -Wl,--gc-sections $(LIBDIRS)


.PHONY : all flash clean debug

all: $(TARGET) $(TARGET).bin

debug : CFLAGS += -DDEBUG -g -Og
debug : LDFLAGS += -Wl,-Map,$(TARGET).map

debug : all

$(TARGET): $(OBJS)
	$(CC)  -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)

$(TARGET).bin: $(TARGET)
	$(OBJCOPY) -Obinary $(TARGET) $(TARGET).bin

flash: $(TARGET).bin
#	st-flash write $(TARGET).bin 0x08000000
	cp $(TARGET).bin $(shell mount|grep DIS_L476VG|awk '{print $$3}')

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).bin $(TARGET).map
