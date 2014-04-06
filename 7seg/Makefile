CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

TARGET_MCU = atmega168p
#TARGET_MCU = atmega328p
FCPU = 8000000UL

PROG = 7seg
CFLAGS = -Os -Wall -g -std=c99 -mmcu=$(TARGET_MCU) -DF_CPU=$(FCPU)
LFLAGS = -lc -Wl,-Map=$(PROG).map

all: $(PROG).hex

$(PROG).hex: $(PROG).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

$(PROG).elf: $(PROG).o
	$(CC) -o $@ $(CFLAGS) $< $(LFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm -rf *.elf *.hex *.o

disasm: $(PROG).elf
	$(OBJDUMP) -d $< > $(PROG).dump

burn: all
	avrdude -c usbasp -p $(TARGET_MCU) -U flash:w:$(PROG).hex
