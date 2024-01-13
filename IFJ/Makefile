OBJS	= main.o scanner.o str.o pstack.o parse.o symtable.o precedence.o codegen.o tokenqueue.o
SOURCE	= main.c scanner.c str.c pstack.c parse.c symtable.c precedence.c codegen.c tokenqueue.c
HEADER	= errors.h scanner.h str.h pstack.h parse.h symtable.h precedence.h codegen.h tokenqueue.h
OUT	= interpret
TEST = example.txt
CC	 = gcc
CFLAGS	 = -g -c -Wall

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

main.o: main.c
	$(CC) $(CFLAGS) main.c

scanner.o: scanner.c
	$(CC) $(CFLAGS) scanner.c

str.o: str.c
	$(CC) $(CFLAGS) str.c

pstack.o: pstack.c
	$(CC) $(CFLAGS) pstack.c

parse.o: parse.c
	$(CC) $(CFLAGS) parse.c

symtable.o: symtable.c
	$(CC) $(CFLAGS) symtable.c

precedence.o: precedence.c
	$(CC) $(CFLAGS) precedence.c

codegen.o: codegen.c
	$(CC) $(CFLAGS) codegen.c

tokenqueue.o: tokenqueue.c
	$(CC) $(CFLAGS) tokenqueue.c

clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT) < $(TEST)