SRC=$(wildcard *.c)
OBJ=$(wildcard *.o)
ELF=$(notdir $(CURDIR)).elf
HEX=$(notdir $(CURDIR)).hex
F_CPU=16000000L

CFLAGS=-mmcu=attiny402 -DF_CPU=$(F_CPU)
CC=avr-gcc

all: $(HEX)

$(ELF): $(OBJS)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(OBJS)

$(HEX): $(ELF)
	avr-objcopy -O ihex -R .eeprom $< $@

flash: $(HEX)
	pyupdi -d tiny402 -c /dev/ttyUSB0 -f $(HEX)

clean:
	rm -rf $(OBJS) $(ELF) $(HEX)

