CC = g++
CFLAGS = -Wall -pedantic -std=c++11 

all: dns
        
dns: dns.cpp
	$(CC) $(CFLAGS) dns.cpp -o dns

test: test_script.py dns.cpp
	$(CC) $(CFLAGS) dns.cpp -o dns
	python3 ./test_script.py

clean:
	rm -f dns
