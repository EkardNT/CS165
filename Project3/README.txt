GROUP MEMBERS:
Drake Tetreault, ID 35571095
Andrew Core, ID 93077921

USE:
We have provided a makefile that can be used to build the program. To use it, simply type 'make' in the same folder as our source code and makefile. The makefile executes the following build operations:

	g++ -std=c++0x -o Project3 main.cpp LempelZiv.cpp Huffman.cpp Bits.cpp
	
To run the program, type "Project3 OPERATION {input_file.txt}", where OPERATION is one of HUFF, LZ1, LZ2, or EXPAND, and input_file.txt is an optional input file to perform the operation on. If no input file name is specified, the program will read from standard input.

DATA STRUCTURES:
For Huffman encoding, we first went over the input and collected the frequencies for every 8-bit byte. To build the Huffman tree, we used the STL's priority_queue class with a custome Node class and custom comparer to iteratively remove the two nodes with least frequency until only one node was left, which became the root node. After traversing the Huffman tree to build the code point bit strings, we went over the input data a second time and wrote the corresponding bit string (in reversed bit order) for every input byte.

For Lempel Ziv encoding, we used a linear array to hold our input data. Due to time constraints, we were unable to implement a time-efficient window search data structure, which is the reason our Lempel Ziv encoding takes so long, because we linearly search the past 4096 or 2048 entries for a prefix matching the lookahead buffer.

TIME ANALYSIS:
For Huffman encoding, the constraining part of the algorithm is the time it takes to build the Huffman tree. For every possible symbol, we initially add the leaf node to the priority queue. We then iteratively remove two nodes from the priority queue and add back a single node that is the parent of the two removed nodes. Because insertion and removal operations on a priority queue are O(log n), the total time for the algorithm is O(n * log n).

For Lempel Ziv encoding, the constraining part of the algorithm is searching the sliding window for the longest prefix match to the lookahead buffer. Because we were unable to implement a more efficient data structure such as a trie or hash map, the running time of our encoding algorithm is O(n * m), where n is the size of the input and m is the window size.