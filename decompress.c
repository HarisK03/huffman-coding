#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "decompress.h"

/**
 * Decompress the file and return the decompressed file name.
 */
char *decompress_file(char *in_file_name)
{
    char *out_file_name = (char *)malloc(sizeof(char) * (strlen(in_file_name) + strlen(DECOMPRESS_EXT) + 1));
    if (out_file_name == NULL)
    {
        perror("Malloc error.");
        return NULL;
    }
    sprintf(out_file_name, "%s%s", in_file_name, DECOMPRESS_EXT);

    FILE *fp_in;
    if ((fp_in = fopen(in_file_name, "rb")) == NULL)
    {
        free(out_file_name);
        perror("File open error");
        return NULL;
    }

    int num_nodes;
    num_nodes = fgetc(fp_in);
    int buf_len = num_nodes * 4;
    unsigned char buf[buf_len];
    fread(buf, sizeof(unsigned char), buf_len, fp_in);

    Node *nodes;
    HuffmanTree *huffman_tree;

    if (((nodes = build_nodes_from_file(buf, buf_len)) == NULL) || ((huffman_tree = build_huffman_tree_from_nodes(nodes, num_nodes - 1)) == NULL))
    {
        return NULL;
    }

    free(nodes);

    unsigned char bytes[4];
    fread(bytes, sizeof(unsigned char), 4, fp_in);
    int size = (bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24);

    FILE *fp_out;
    if ((fp_out = fopen(out_file_name, "wb")) == NULL)
    {
        perror("File open error");
        return NULL;
    }

    write_decompressed_bytes_from_file(fp_in, fp_out, huffman_tree, size);

    free_huffman_tree(huffman_tree);

    fclose(fp_in);
    fclose(fp_out);

    return out_file_name;
}

/**
 * Create Nodes to keep track of Huffman tree information.
 */
Node *build_nodes_from_file(unsigned char *buf, int buf_len)
{
    Node *nodes = (Node *)malloc(sizeof(Node) * (buf_len / 4));
    if (nodes == NULL)
    {
        perror("Malloc error.");
        return NULL;
    }

    for (int i = 0; i < buf_len; i += 4)
    {
        Node node;
        node.left_type = buf[i];
        node.left_data = buf[i + 1];
        node.right_type = buf[i + 2];
        node.right_data = buf[i + 3];
        nodes[i / 4] = node;
    }

    return nodes;
}

/**
 * Build the Huffman tree from the array of Nodes.
 */
HuffmanTree *build_huffman_tree_from_nodes(Node *nodes, int root_index)
{
    Node root = nodes[root_index];
    HuffmanTree *huffman_tree = (HuffmanTree *)malloc(sizeof(HuffmanTree));
    if (huffman_tree == NULL)
    {
        perror("Malloc error.");
        return NULL;
    }
    huffman_tree->symbol = -1;
    huffman_tree->left = NULL;
    huffman_tree->right = NULL;

    if (root.left_type == 0)
    {
        huffman_tree->left = (HuffmanTree *)malloc(sizeof(HuffmanTree));
        if (huffman_tree->left == NULL)
        {
            perror("Malloc error.");
            return NULL;
        }
        huffman_tree->left->symbol = root.left_data;
        huffman_tree->left->left = NULL;
        huffman_tree->left->right = NULL;
    }
    else
    {
        huffman_tree->left = build_huffman_tree_from_nodes(nodes, root.left_data);
    }

    if (root.right_type == 0)
    {
        huffman_tree->right = (HuffmanTree *)malloc(sizeof(HuffmanTree));
        if (huffman_tree->right == NULL)
        {
            perror("Malloc error.");
            return NULL;
        }
        huffman_tree->right->symbol = root.right_data;
        huffman_tree->right->left = NULL;
        huffman_tree->right->right = NULL;
    }
    else
    {
        huffman_tree->right = build_huffman_tree_from_nodes(nodes, root.right_data);
    }

    return huffman_tree;
}

/**
 * Decompress the bytes from the Huffman tree and write them to stdout.
 */
void write_decompressed_bytes_from_file(FILE *fp_in, FILE *fp_out, HuffmanTree *huffman_tree, int size)
{
    unsigned char byte_buf[BUF_SIZE];
    unsigned char decompressed_bytes[BUF_SIZE];
    int bytes_written = 0;
    HuffmanTree *tree = huffman_tree;

    size_t bytes_read;
    while ((bytes_read = fread(byte_buf, sizeof(unsigned char), 8192, fp_in)) > 0)
    {
        for (size_t i = 0; i < bytes_read; i++)
        {
            unsigned char byte = byte_buf[i];
            for (int i = 7; i >= 0; i--)
            {
                if (((byte >> i) & 1) == 0)
                {
                    tree = tree->left;
                }
                else
                {
                    tree = tree->right;
                }
                if (tree->symbol != -1)
                {
                    decompressed_bytes[bytes_written++] = (unsigned char)tree->symbol;
                    tree = huffman_tree;
                    if (--size == 0)
                    {
                        fwrite(decompressed_bytes, sizeof(unsigned char), bytes_written, fp_out);
                        return;
                    }
                }
                if (bytes_written == BUF_SIZE)
                {
                    fwrite(decompressed_bytes, sizeof(unsigned char), bytes_written, fp_out);
                    bytes_written = 0;
                }
            }
        }
    }
}