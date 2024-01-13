OBJS = ims-projekt.o
SOURCE = ims-projekt.cpp
OUT = ims-projekt
CC = g++
CFLAGS = -g

all: $(OUT)

$(OUT): $(SOURCE)
	$(CC) $(SOURCE) -o $(OUT) -lsimlib

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)


