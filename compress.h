#define COMPRESS_EXT ".huf"

char *compress_file(char *in_file_name);

void write_compressed_bytes_from_file(FILE *fp_in, FILE *fp_out, char **huffman_tree_codes);