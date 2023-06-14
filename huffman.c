#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "heap.h"

/**
 * Build the HuffmanTreeDict from the file.
 */
HuffmanTreeDict *build_huffman_tree_dict(FILE *fp_in)
{
    HuffmanTreeDict *huffman_tree_dict;
    if ((huffman_tree_dict = (HuffmanTreeDict *)malloc(sizeof(HuffmanTreeDict))) == NULL)
    {
        perror("Malloc error.");
        return NULL;
    }

    huffman_tree_dict->size = NUM_SYMBOLS;
    if ((huffman_tree_dict->entries = (HuffmanTree *)malloc(sizeof(HuffmanTree) * huffman_tree_dict->size)) == NULL)
    {
        free(huffman_tree_dict);
        perror("Malloc error.");
        return NULL;
    }

    // initialize default values
    for (int i = 0; i < huffman_tree_dict->size; i++)
    {
        huffman_tree_dict->entries[i].symbol = i;
        huffman_tree_dict->entries[i].frequency = 0;
        huffman_tree_dict->entries[i].number = -1;
        huffman_tree_dict->entries[i].left = NULL;
        huffman_tree_dict->entries[i].right = NULL;
    }

    size_t bytes_read;
    unsigned char buf[BUF_SIZE];

    while ((bytes_read = fread(buf, sizeof(unsigned char), BUF_SIZE, fp_in)) > 0)
    {
        for (size_t i = 0; i < bytes_read; i++)
        {
            huffman_tree_dict->entries[buf[i]].frequency++;
        }
    }

    build_min_heap(huffman_tree_dict); // convert to a min-heap

    return huffman_tree_dict;
}

/**
 * Build the Huffman tree from the HuffmanTreeDict.
 */
HuffmanTree *build_huffman_tree(HuffmanTreeDict *huffman_tree_dict)
{
    while (huffman_tree_dict->size > 1)
    {
        HuffmanTree *huffman_tree_1 = extract_min(huffman_tree_dict);
        HuffmanTree *huffman_tree_2 = extract_min(huffman_tree_dict);

        HuffmanTree new_huffman_tree;
        new_huffman_tree.symbol = -1;
        new_huffman_tree.frequency = huffman_tree_1->frequency + huffman_tree_2->frequency;
        new_huffman_tree.number = -1;
        new_huffman_tree.left = huffman_tree_1;
        new_huffman_tree.right = huffman_tree_2;

        insert(huffman_tree_dict, new_huffman_tree);
    }

    HuffmanTree *huffman_tree = extract_min(huffman_tree_dict);
    return huffman_tree;
}

/**
 * Get the path codes from the Huffman tree.
 */
char **get_huffman_tree_codes(HuffmanTree *huffman_tree)
{
    char code[NUM_SYMBOLS]; // worst tree has a maximum n-1 height
    char **codes = (char **)malloc((sizeof(char) * NUM_SYMBOLS) * (sizeof(char) * NUM_SYMBOLS));
    if (codes == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < NUM_SYMBOLS; i++)
    {
        codes[i] = '\0';
    }
    int num = 0;

    _get_huffman_tree_codes_helper(huffman_tree, code, codes, 0, &num);

    return codes;
}

/**
 * A helper functio to get the path codes from the Huffman tree.
 */
void _get_huffman_tree_codes_helper(HuffmanTree *huffman_tree, char *code, char **codes, int i, int *num)
{
    if (huffman_tree->left != NULL)
    {
        code[i] = '0';
        _get_huffman_tree_codes_helper(huffman_tree->left, code, codes, i + 1, num);
    }

    if (huffman_tree->right != NULL)
    {
        code[i] = '1';
        _get_huffman_tree_codes_helper(huffman_tree->right, code, codes, i + 1, num);
    }

    if (huffman_tree->left == NULL && huffman_tree->right == NULL)
    {
        code[i] = '\0';
        codes[huffman_tree->symbol] = strdup(code);
    }
    else
    {
        huffman_tree->number = *num;
        (*num)++;
    }
}

/**
 * Write the bytes from the Huffman treee to a buffer and return the buffer.
 */
unsigned char *get_huffman_tree_bytes(FILE *fp_in, HuffmanTree *huffman_tree, unsigned char *buf)
{
    unsigned char bytes[4];

    if (huffman_tree != NULL)
    {
        HuffmanTree *left = huffman_tree->left;
        HuffmanTree *right = huffman_tree->right;

        if (left != NULL || right != NULL)
        {
            if (left->symbol != -1)
            {
                bytes[0] = 0;
                bytes[1] = (unsigned char)left->symbol;
            }
            else
            {
                bytes[0] = 1;
                bytes[1] = (unsigned char)left->number;
                buf = get_huffman_tree_bytes(fp_in, left, buf);
            }

            if (right->symbol != -1)
            {
                bytes[2] = 0;
                bytes[3] = (unsigned char)right->symbol;
            }
            else
            {
                bytes[2] = 1;
                bytes[3] = (unsigned char)right->number;
                buf = get_huffman_tree_bytes(fp_in, right, buf);
            }
        }

        memcpy(buf, bytes, 4);
    }

    return buf + 4;
}

/**
 * Free the memory for a HuffmanTreeDict.
 */
void free_huffman_tree_dict(HuffmanTreeDict *huffman_tree_dict)
{
    free(huffman_tree_dict->entries);
    free(huffman_tree_dict);
}

/**
 * Free the memory for a HuffmanTree.
 */
void free_huffman_tree(HuffmanTree *huffman_tree)
{
    if (huffman_tree != NULL)
    {
        free_huffman_tree(huffman_tree->left);
        free_huffman_tree(huffman_tree->right);
        free(huffman_tree);
    }
}

/**
 * Free the memory for Huffman tree codes.
 */
void free_codes(char **codes)
{
    for (int i = 0; i < NUM_SYMBOLS; i++)
    {
        free(codes[i]);
    }
    free(codes);
}