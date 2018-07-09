MCU=stm8
DEVICE=stm8s103f3
FLASHER=stlinkv2
CFLAGS=-I ./USER/ -I ../STM8S_StdPeriph_Driver/inc/ -DSTM8S103 -c -O1
CC=sdcc
SRCS= ../STM8S_StdPeriph_Driver/src/
SRCS2= ./USER/
LIB = stm8s_gpio.rel
LIB += stm8s_uart1.rel stm8s_clk.rel
LIB += clk.rel leds.rel uart.rel common.rel interrupts.rel 
OBJ=main.rel
OUTPUT_DIR=
TARGET=main

.PHONY: all clean

%.rel:	%.c
	$(CC) -m$(MCU)  $(CFLAGS) $(LDFLAGS) $<

%.rel:	$(SRCS)%.c
	$(CC) -m$(MCU)  $(CFLAGS) $(LDFLAGS) $<
	
%.rel:	$(SRCS2)%.c
	$(CC) -m$(MCU)  $(CFLAGS) $(LDFLAGS) $<
	
all:	$(OBJ) $(LIB)
	$(CC) -m$(MCU) -o $(TARGET).ihx $(OBJ) $(LIB)

download:
	stm8flash -c $(FLASHER) -p $(DEVICE) -w $(TARGET).ihx
clean:
	@rm -v *.sym *.asm *.lst *.rel *.ihx *lk *.rst *.cdb *.map
