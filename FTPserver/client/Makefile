SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=client
CC:=gcc
CFLAGS:=-Wall
$(ELF):$(OBJS)
	gcc -o $@ $^ -pthread
clean:
	rm -rf $(OBJS) $(ELF)
