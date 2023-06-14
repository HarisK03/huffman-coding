#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"
#include "compress.h"

/**
 * Compress the file and return the compressed file name.
 */
char *compress_file(char *in_file_name)
{
    // create the output filename
    char *out_file_name = (char *)malloc(sizeof(char) * (strlen(in_file_name) + strlen(COMPRESS_EXT) + 1));
    if (out_file_name == NULL)
    {
        perror("Malloc error.");
        return NULL;
    }
    sprintf(out_file_name, "%s%s", in_file_name, COMPRESS_EXT);

    // open the file to read bytes
    FILE *fp_in;
    if ((fp_in = fopen(in_file_name, "rb")) == NULL)
    {
        free(out_file_name);
        perror("File open error.");
        return NULL;
    }

    HuffmanTreeDict *huffman_tree_dict;
    HuffmanTree *huffman_tree;
    char **codes;

    if (((huffman_tree_dict = build_huffman_tree_dict(fp_in)) == NULL) || ((huffman_tree = build_huffman_tree(huffman_tree_dict)) == NULL) || ((codes = get_huffman_tree_codes(huffman_tree)) == NULL))
    {
        return NULL;
    }

    free_huffman_tree_dict(huffman_tree_dict);

    FILE *fp_out;
    if ((fp_out = fopen(out_file_name, "wb")) == NULL)
    {
        perror("File open error");
        return NULL;
    }

    // write bytes from the Huffman tree
    unsigned char num_nodes = (unsigned char)(huffman_tree->number + 1);
    unsigned char buf[num_nodes * 4 + 1];
    buf[0] = (unsigned char)num_nodes;
    get_huffman_tree_bytes(fp_out, huffman_tree, buf + 1);
    free_huffman_tree(huffman_tree);
    fwrite(&buf, sizeof(unsigned char), num_nodes * 4 + 1, fp_out);

    // little endian
    long file_length = ftell(fp_in);
    unsigned char bytes[4] = {0};
    bytes[0] = (unsigned char)(file_length & 0xFF);
    bytes[1] = (unsigned char)((file_length >> 8) & 0xFF);
    bytes[2] = (unsigned char)((file_length >> 16) & 0xFF);
    bytes[3] = (unsigned char)((file_length >> 24) & 0xFF);
    fwrite(bytes, sizeof(unsigned char), 4, fp_out);

    write_compressed_bytes_from_file(fp_in, fp_out, codes);

    free_codes(codes);

    fclose(fp_in);
    fclose(fp_out);

    return out_file_name;
}

/**
 * Compress the bytes from the file and write them to stdout.
 */
void write_compressed_bytes_from_file(FILE *fp_in, FILE *fp_out, char **codes)
{
    fseek(fp_in, 0, SEEK_SET); // reset stdin

    unsigned char buf[BUF_SIZE];
    unsigned char byte_buffer[BUF_SIZE];
    unsigned char compressed_byte = 0;
    int bit_count = 0;
    size_t bytes_read;

    while ((bytes_read = fread(byte_buffer, sizeof(unsigned char), BUF_SIZE, fp_in)) > 0)
    {
        for (size_t i = 0; i < bytes_read; i++)
        {
            char *code = codes[byte_buffer[i]];

            while (*code != '\0')
            {
                compressed_byte = compressed_byte << 1;
                if (*code == '1')
                {
                    compressed_byte = compressed_byte | 1;
                }
                bit_count++;
                code++;

                if (bit_count > 0 && bit_count % 8 == 0)
                {
                    buf[bit_count / 8 - 1] = compressed_byte;
                    if (bit_count == BUF_SIZE * 8)
                    {
                        fwrite(&buf, sizeof(unsigned char), BUF_SIZE, fp_out);
                        bit_count = 0;
                    }
                }
            }
        }
    }

    // write remaining bits padded with 0s
    if (bit_count % 8 > 0)
    {
        for (int i = 0; i < 8 - (bit_count % 8); i++)
        {
            compressed_byte = compressed_byte << 1;
        }
        bit_count += 8 - (bit_count % 8);
        buf[bit_count / 8 - 1] = compressed_byte;
    }

    fwrite(&buf, sizeof(unsigned char), bit_count / 8, fp_out);
}