NAME=garden
SDCC=sdcc

CCFLAGS=-DSTM8S103 -I/ -I../../stm8  -mstm8 --opt-code-size --out-fmt-ihx
LDFLAGS= -mstm8 --out-fmt-ihx -lstm8
FLASHFLAGS=-cstlinkv2 -pstm8s103f3

SRC=$(wildcard *.c)

OBJ=$(SRC:%.c=%.rel)
TRASH=$(OBJ) $(SRC:%.c=%.rst) $(SRC:%.c=%.asm) $(SRC:%.c=%.lst)
TRASH+=$(SRC:%.c=%.sym) $(NAME).lk $(NAME).map
INDEPENDENT_HEADERS=../stm8l.h ports_definition.h Makefile

all: $(NAME).ihx

#$(SRC) : %.c : %.h $(INDEPENDENT_HEADERS)
#	@touch $@
#
#%.h: ;

clean:
	-rm -f $(TRASH)
	-rm -f $(NAME).*

load: $(NAME).ihx
	stm8flash $(FLASHFLAGS) -w $(NAME).ihx

%.rel: %.c
	$(SDCC) $(CCFLAGS) -c $<

$(NAME).ihx: $(OBJ)
	$(SDCC) $(LDFLAGS) $(OBJ) -o $(NAME).ihx

.PHONY: all
