#include <stdio.h>

#define NUM_SYMBOLS 256 // 256 byte values (0 - 255)
#define BUF_SIZE 8192

/**
 * A Huffman tree.
 */
typedef struct HuffmanTree
{
    int symbol;
    int frequency;
    int number;
    struct HuffmanTree *left;
    struct HuffmanTree *right;
} HuffmanTree;

/**
 * A dictionary that contains the Huffman trees. This is synonamous with a frequency dictionary.
 */
typedef struct HuffmanTreeDict
{
    int size;
    struct HuffmanTree *entries;
} HuffmanTreeDict;

HuffmanTreeDict *build_huffman_tree_dict(FILE *fp_in);

HuffmanTree *build_huffman_tree(HuffmanTreeDict *huffman_tree_dict);

char **get_huffman_tree_codes(HuffmanTree *huffman_tree);

void _get_huffman_tree_codes_helper(HuffmanTree *huffman_tree, char *code, char **codes, int i, int *num);

unsigned char *get_huffman_tree_bytes(FILE *fp_in, HuffmanTree *huffman_tree, unsigned char *buf);

void free_huffman_tree_dict(HuffmanTreeDict *huffman_tree_dict);

void free_huffman_tree(HuffmanTree *huffman_tree);

void free_codes(char **codes);