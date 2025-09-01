#!/bin/bash
OPT=-O0


compile_harfbuzz() {
	if [[ harfbuzz -nt harfbuzz.o ]]; then
	    echo "Building harfbuzz.o..."
	    g++ -c harfbuzz/src/harfbuzz.cc -std=c++17 -O2 -g -o harfbuzz.o && \
		echo "Done building harfbuzz.o"
	else
		echo "Skipping building harfbuzz.o, file is up to date"
	fi
	}

compile_example() {
	echo "Building example.cpp => ./example ..."
	g++ example.cpp harfbuzz.o -std=c++17 $OPT -g  -o example `pkg-config --cflags --libs sdl3` && \
	echo "Done building example"
	}

compile_producer_consumer() {
	echo "Building producerConsumerExample.cpp => ./producerConsumerExample..."
	g++ producerConsumerExample.cpp -std=c++17 $OPT -g  -o producerConsumerExample `pkg-config --cflags --libs sdl3` && \
	echo "Done building producerConsumer!"
	}


compile_harfbuzz

compile_example &
compile_producer_consumer &

wait
