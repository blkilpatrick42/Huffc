FILES = gTree.cpp gPriorityQueue.cpp huffCompressor.cpp main.cpp
CC = g++

Huffc: $(FILES)
	$(CC) $(FILES) -o huffc

