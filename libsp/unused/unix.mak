CFLAGS=-O -Wall -DPLATFORM_UNIX

all: test

test: sp_task.c plat_unix.c test.c
