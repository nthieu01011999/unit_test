// xor_decrypt.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void make_new_name_decrypt(char *new_name, char *original_name);
void perform_XOR_decrypt(char *input_filename, char *output_filename, char *password);

void make_new_name_decrypt(char *new_name, char *original_name) {
    strcpy(new_name, "decrypted-");
    strcat(new_name, original_name);
}

void perform_XOR_decrypt(char *input_filename, char *output_filename, char *password) {
    FILE *input_file, *output_file;
    int block_size = strlen(password);
    unsigned char *block = malloc(block_size * sizeof(unsigned char));
    int num_bytes;

    if (block == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    input_file = fopen(input_filename, "rb");
    output_file = fopen(output_filename, "wb");

    if (input_file == NULL || output_file == NULL) {
        printf("Error opening files!\n");
        free(block);
        return;
    }

    while ((num_bytes = fread(block, 1, block_size, input_file)) > 0) {
        for (int i = 0; i < num_bytes; i++) {
            block[i] ^= password[i];
        }

        fwrite(block, 1, num_bytes, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    free(block);
}
