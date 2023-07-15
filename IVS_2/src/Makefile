LIBFLAGS := -O2
CXX := g++

all: build_tests profile calc_ivs

build_tests:
	mkdir -p build && cd build/ && cmake .. && make

#builds lib for calc use
libmlib.a: mlib/mlib_basic.cpp mlib/mlib_eval.cpp
	$(CXX) -c $(LIBFLAGS) mlib/mlib_basic.cpp -o mlib_basic.o
	$(CXX) -c $(LIBFLAGS) mlib/mlib_eval.cpp -o mlib_eval.o
	ar rcs libmlib.a mlib_basic.o mlib_eval.o

calcIVS/calcIVS: libmlib.a
	cd calcIVS && qmake && make
#	cd calcIVS && make

run: calcIVS/calcIVS
	cd calcIVS && ./calcIVS

#runs test
test: build_tests 
	cd build/ && ./mlib_test

profile: stddev.cpp libmlib_prof.a
	$(CXX) -pg stddev.cpp libmlib_prof.a -o stddev

gprof: profile
	gprof stddev gmon.out > gprof.txt

doc:
	doxygen Doxyfile

debug: debug.cpp
	$(CXX) -g debug.cpp mlib/mlib_eval.cpp mlib/mlib_basic.cpp -o debug

#builds lib for profiling
libmlib_prof.a: mlib/mlib_basic.cpp mlib/mlib_eval.cpp
	$(CXX) -c -pg mlib/mlib_basic.cpp -o mlib_basic_prof.o
	$(CXX) -c -pg mlib/mlib_eval.cpp -o mlib_eval_prof.o
	ar rcs libmlib_prof.a mlib_basic_prof.o mlib_eval_prof.o

#emergency calculator
calc_console: calc_console.cpp libmlib.a
	$(CXX) calc_console.cpp libmlib.a -o calc_console.elf

.PHONY: clean
clean: 
	rm -f -r build/ *.o gprof.txt gmon.out *.a stddev *.elf ../docs/ ../xkonde05_xniesl00_xrepin00_xsocha03.zip
	cd calcIVS && make clean

pack: clean doc
	cd ../ && mkdir -p xkonde05_xniesl00_xrepin00_xsocha03
	mv ../doc ../xkonde05_xniesl00_xrepin00_xsocha03
	cp -r ../install ../xkonde05_xniesl00_xrepin00_xsocha03
	cd ../xkonde05_xniesl00_xrepin00_xsocha03 && mkdir -p repo
	cd ../ && rsync -r --exclude 'xkonde05_xniesl00_xrepin00_xsocha03' * xkonde05_xniesl00_xrepin00_xsocha03/repo
	cd ../ && cp -r .git xkonde05_xniesl00_xrepin00_xsocha03/repo
	cd ../ && cp .gitignore xkonde05_xniesl00_xrepin00_xsocha03/repo
	cd ../ && zip -rm xkonde05_xniesl00_xrepin00_xsocha03.zip xkonde05_xniesl00_xrepin00_xsocha03



	
