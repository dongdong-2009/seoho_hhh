###############################################################################
# Makefile for the project OK2560_BD
###############################################################################

## General Flags
PROJECT = OK2560_BD
MCU = atmega2560
TARGET = OK2560_BD.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2                                              -DF_CPU=16000000UL -O0 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=OK2560_BD.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"D:\work\SEOHO_VD\modbus_1000_mmi_090724\source\modbus" -I"D:\work\SEOHO_VD\modbus_1000_mmi_090724\source\mcu" 

## Objects that must be built in order to link
OBJECTS = main.o serial.o util.o modbus_application.o modbus_server.o 128by64GLCD.o debug_printf.o key.o display.o ds1307.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) OK2560_BD.hex OK2560_BD.eep size

## Compile
main.o: ../source/main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

serial.o: ../source/mcu/serial.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

util.o: ../source/mcu/util.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

modbus_application.o: ../source/modbus/modbus_application.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

modbus_server.o: ../source/modbus/modbus_server.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

128by64GLCD.o: ../source/mcu/128by64GLCD.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

debug_printf.o: ../source/mcu/debug_printf.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

key.o: ../source/mcu/key.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

display.o: ../source/mcu/display.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ds1307.o: ../source/mcu/ds1307.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) OK2560_BD.elf dep/* OK2560_BD.hex OK2560_BD.eep OK2560_BD.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

