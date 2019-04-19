CC=cc
IDIR=/usr/local/Cellar/curl/7.64.1/lib/
CFLAGS=-I$(IDIR)

gust: gust.o
	$(CC) -o gust gust.c -lcurl

