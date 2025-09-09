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
	g++ example.cpp -std=c++17 $OPT -g  -o example -fmax-errors=5 `pkg-config --cflags --libs sdl3` && \
	echo "Done building example"
	}

hb_compile_example() {
	echo "Building example.cpp => ./hb_example ..."
	g++ example.cpp harfbuzz.o -DSSF_HARFBUZZ_ENABLED -DSSF_HARFBUZZ_IMPL_HANDLED -std=c++17 $OPT -g  -o hb_example -fmax-errors=5 `pkg-config --cflags --libs sdl3` && \
	echo "Done building hb_example"
	}


compile_producer_consumer() {
	echo "Building producerConsumerExample.cpp => ./producerConsumerExample..."
	g++ producerConsumerExample.cpp -std=c++17 $OPT -g  -o producerConsumerExample -fmax-errors=5 `pkg-config --cflags --libs sdl3` && \
	echo "Done building producerConsumer!"
	}

hb_compile_producer_consumer() {
	echo "Building producerConsumerExample.cpp => ./hb_producerConsumerExample..."
	g++ producerConsumerExample.cpp harfbuzz.o -DSSF_HARFBUZZ_ENABLED -DSSF_HARFBUZZ_IMPL_HANDLED -std=c++17 $OPT -g  -o hb_producerConsumerExample -fmax-errors=5 `pkg-config --cflags --libs sdl3` && \
	echo "Done building hb_producerConsumer!"
	}


compile_harfbuzz

compile_example & pid1=$!
compile_producer_consumer & pid2=$!
hb_compile_example & pid3=$!
hb_compile_producer_consumer & pid4=$!

status=0
wait $pid1 || status=$?
wait $pid2 || status=$?
wait $pid3 || status=$?
wait $pid4 || status=$?


exit $status
