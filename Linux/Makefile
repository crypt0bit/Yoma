CC=gcc
CFLAGS=-O2
LIBS=-limobiledevice -lplist

SRC=src/main.c src/ziputil.c src/sign.c src/install.c

yoma:
	$(CC) $(SRC) $(CFLAGS) -o yoma $(LIBS)
