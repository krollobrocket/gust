gust: gust.c
	${CC} -o $@ -std=c99 ${CCFLAGS} $? `curl-config --libs`

clean:
	rm -rf gust
