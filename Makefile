@phony: clear all run

all: domino

domino: domino.cc
	g++ -std=c++11 -Wall -Werror -pedantic -g -O0 -o $@ $<

run: domino
	./domino

clear:
	rm -f domino
