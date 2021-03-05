SRC=$(wildcard *.c)
OBJ=$(subst .c,.o,$(SRC))
ELF=$(notdir $(CURDIR)).elf
HEX=$(notdir $(CURDIR)).hex
F_CPU=16000000L

CFLAGS=-mmcu=attiny402 -DF_CPU=$(F_CPU) -Wall -Wno-main
CC=avr-gcc

main.o: main.c

all: $(HEX)

$(ELF): $(OBJS)
	$(CC) $(CFLAGS) $(SRC) -o $@

$(HEX): $(ELF)
	avr-objcopy -O ihex -R .eeprom $< $@

flash: $(HEX)
	pyupdi -d tiny402 -c /dev/ttyUSB0 -f $(HEX)

clean:
	rm -f $(OBJS) $(ELF) $(HEX)

