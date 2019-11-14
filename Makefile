gust: gust.o
	${CC} -o $@ ${CCFLAGS} $? -lcurl

clean:
	rm -rf gust *.o
