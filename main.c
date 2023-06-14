#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "huffman.h"
#include "compress.h"
#include "decompress.h"

int main()
{
    /**
     * The main function.
     */

    // get user input
    printf("Please type 'c' to compress a file, 'd' to decompress a file, and any other character to exit the program:\n");
    char c = getchar();

    // exit program
    if (c != 'c' && c != 'd')
    {
        printf("Exiting program.");
        return 0;
    }

    // get file name from user input
    char *replace_word = c == 'c' ? "compress" : "decompress";
    printf("Please type the file name that you would like to %s:\n", replace_word);

    char in_file_name[256]; // 256 file path limit
    scanf("%255s", in_file_name);
    in_file_name[strlen(in_file_name)] = '\0';

    // compress or decompress file
    time_t start = clock();
    char *out_file_name = (c == 'c') ? compress_file(in_file_name) : decompress_file(in_file_name);
    time_t end = clock();
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (out_file_name == NULL)
    {
        perror("File compression or decompression error.");
        return 1;
    }

    printf("Successfully %sed the file %s to %s in %.2f seconds!", replace_word, in_file_name, out_file_name, elapsed);
    free(out_file_name);
    return 0;
}
