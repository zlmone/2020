SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
ELF:=FTP_server
CC:=gcc
CFLAGS:=-Wall -I /usr/include/mysql/
$(ELF):$(OBJS)
	gcc -o $@ $^ -pthread -lcrypt -lmysqlclient
clean:
	rm -rf $(OBJS) $(ELF)

