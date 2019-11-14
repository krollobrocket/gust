gust: gust.c
	${CC} -o $@ ${CCFLAGS} $? -lcurl

clean:
	rm -rf gust
