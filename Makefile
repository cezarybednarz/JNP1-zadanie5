all:
	g++ -Wall -Wextra -std=c++17 -O2 -c insertion_ordered_map.h
	g++ -Wall -Wextra -std=c++17 -O2 -c insertion_ordered_map_example.cc
	g++ insertion_ordered_map.o insertion_ordered_map_example.o -o insertion_ordered_map_example
