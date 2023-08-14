CC=g++

parser: parser.cpp
	$(CC) -o parser parser.cpp $(CFLAGS)
