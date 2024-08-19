#include <stdio.h>
#include <string.h>
#include "xor_encrypt.h"

int is_valid_password(char *password);
int is_alpha(char c);
int is_digit(char c);

void perform_XOR_encrypt(char *filename, char *password);

int is_valid_password(char *password) {
    int has_char = 0, has_digit = 0;
    int len = strlen(password);

    for (int i = 0; i < len; i++) {
        if (is_alpha(password[i])) has_char = 1;
        if (is_digit(password[i])) has_digit = 1;
    }

    if (len < 8) {
        printf("The password needs to have at least 8 characters.\n");
    }
    if (!has_char) {
        printf("The password needs to contain at least 1 alphabetical character.\n");
    }
    if (!has_digit) {
        printf("The password needs to contain at least 1 digit.\n");
    }

    return (len >= 8 && has_char && has_digit) ? 1 : 0;
}

void perform_XOR_encrypt(char *filename, char *password) {
    FILE *file;
    int block_size = strlen(password);
    unsigned char block[block_size];
    int num_bytes;

    // Open file in read/write mode
    file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // Encrypt and write back in-place
    do {
        num_bytes = fread(block, 1, block_size, file);
        
        // Encrypt in place
        for (int i = 0; i < num_bytes; i++) 
            block[i] ^= password[i];
        
        // Move file pointer back to the beginning of the block just read
        fseek(file, -num_bytes, SEEK_CUR);
        
        // Write the encrypted block back to the file
        fwrite(block, 1, num_bytes, file);

    } while (num_bytes == block_size);

    // Close the file
    fclose(file);
}

int is_alpha(char c) {
    return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) ? 1 : 0;
}

int is_digit(char c) {
    return ('0' <= c && c <= '9') ? 1 : 0;
}
