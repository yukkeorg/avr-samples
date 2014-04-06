CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size

CONTROLER = usbasp
#TARGET_MCU = attiny2313
TARGET_MCU = atmega168p
#TARGET_MCU = atmega328p
FCPU = 8000000UL

PROG = lcd
OBJS := main.o lcd.o
DEPS := $(OBJS:%.o=%.d)
CFLAGS := -Os -Wall -g -std=c99 -mmcu=$(TARGET_MCU) -DF_CPU=$(FCPU)
LFLAGS := -lc -Wl,-Map=$(PROG).map

.PHONY: all disasm clean burn mcustatus

all: $(PROG).hex

disasm: $(PROG).elf
	$(OBJDUMP) -d $< > $(PROG).dump

$(PROG).hex: $(PROG).elf
	$(SIZE) -C --mcu=$(TARGET_MCU) $<
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(PROG).elf: $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) $(LFLAGS)

-include $(DEPS)

.c.o:
	$(CC) -MMD -MP -c $(CFLAGS) $<

burn: $(PROG).hex
	avrdude -c $(CONTROLER) -p $(TARGET_MCU) -U flash:w:$(PROG).hex

mcustatus:
	-avrdude -v -v -c $(CONTROLER) -p $(TARGET_MCU)

clean:
	rm -rf *.elf *.hex *.map *.dump *.o *.d

