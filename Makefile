gust: gust.c
	${CC} -o $@ -std=c99 ${CCFLAGS} $? -lcurl

clean:
	rm -rf gust
