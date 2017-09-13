#Huffc
Huffc is a file compression program. It uses its own implementations of the 
generic priority queue and generic tree data structures to compress files
via a simple implementation of Huffman Coding.

Huffc was made primarily as a hobby project with the intent of exploring
the data structures and algorithms involved in Huffman Coding and
compression. It is neither especially fast or especially good at compressing
files. In any case, if you would like to compile it and try it out for
yourself, you can simply run make in this directory and then run the 
resulting executable.

Use:

   ./Huffc -c <filename>

to compress a given file. The file will be output in the same directory as
"<filename>.huff". 

Use:

   ./Huffc -d <filename>

to decompress a compressed file. The program will automatically remove the 
.huff extension.

Alternatively, the code can be used in the context of another c++ program by 
including huff.h, and then creating a "huffCompressor" object. The methods 
compress(char*) and decompres(char*) can then be used to compress and 
decompress files.

The code for the generic tree data structure is in the gTree.h and .cpp files.
Similarly, the code for the generic priority queue data structure is in the 
gPriorityQueue.h and .cpp files.

-Brandon Kilpatrick
