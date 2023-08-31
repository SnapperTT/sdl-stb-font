#!/bin/bash
compile_example() {
	echo "Building example.cpp => ./example ..."
	g++ example.cpp -std=c++17 -O2 -g  -o example `sdl2-config --cflags --libs` && \
	echo "Done building example"
	}

compile_producer_consumer() {
	echo "Building producerConsumerExample.cpp => ./producerConsumerExample..."
	g++ producerConsumerExample.cpp -std=c++17 -O2 -g  -o producerConsumerExample `sdl2-config --cflags --libs` && \
	echo "Done building producerConsumer!"
	}

compile_example &
compile_producer_consumer &

wait
