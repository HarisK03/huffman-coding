#define DECOMPRESS_EXT ".unhuf"

/**
 * A Node.
 */
typedef struct Node
{
    int left_type;
    int left_data;
    int right_type;
    int right_data;
} Node;

char *decompress_file(char *in_file_name);

Node *build_nodes_from_file(unsigned char *buf, int buf_len);

HuffmanTree *build_huffman_tree_from_nodes(Node *nodes, int root_index);

void write_decompressed_bytes_from_file(FILE *fp_in, FILE *fp_out, HuffmanTree *huffman_tree, int size);