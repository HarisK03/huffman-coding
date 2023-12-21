# Huffman Coding #

## About the Project ##

This application allows the compression and decompression of arbitrary files using the Huffman Coding algorithm.

## Compression ##

### Frequency Calculation ###

The algorithm begins by analyzing the input data to determine the frequency of each character or symbol.

### Building the Huffman Tree ###

The algorithm creates a binary tree called the Huffman tree. Characters with lower frequencies are placed higher in the tree, while those with higher frequencies are placed closer to the tree's leaves. This tree is built using a heap data structure. Initially, each character is considered a single-node tree with its frequency as the node's weight. 

### Merging Nodes ###

The two nodes with the lowest frequencies are extracted from the heap and merged to form a new internal node in the Huffman tree. The frequency of the new node is the sum of the frequencies of the nodes merged. This new node is then inserted back into the heap. This continues until there is only one node left in the priority queue. This node becomes the root of the Huffman tree.

### Assigning Codes ###

Traversing the Huffman tree from the root to each leaf node generates unique binary codes for each character. Left traversal is assigned a '0', and right traversal is assigned a '1'.

### Generating the Encoded Output ###

With the assigned codes, the algorithm then encodes the input data by replacing each character with its corresponding Huffman code.

## Decompression ##

### Reconstructing the Huffman Tree ###

The encoded data and the Huffman tree are used to regenerate the original Huffman tree.

### Decoding ###

Starting from the root, each bit in the encoded data is read. The traversal follows the path down the Huffman tree by interpreting '0' as a move to the left child and '1' as a move to the right child. When a leaf node is reached, the corresponding character is output. This process continues until the entire encoded data is decoded, recreating the original input data.

## Contact ##

Haris Kamal - HarisKamal03@gmail.com

Project Link - https://github.com/HarisK03/huffman-coding
