#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// void perform_XOR_decrypt(char *filename, char *password);

void perform_XOR_decrypt(char *filename, char *password) {
    FILE *file;
    int block_size = strlen(password);
    unsigned char *block = malloc(block_size * sizeof(unsigned char));
    int num_bytes;

    if (block == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Open file in read/write mode
    file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        free(block);
        return;
    }

    // Decrypt and write back in-place
    while ((num_bytes = fread(block, 1, block_size, file)) > 0) {
        for (int i = 0; i < num_bytes; i++) {
            block[i] ^= password[i];
        }

        // Move file pointer back to the beginning of the block just read
        fseek(file, -num_bytes, SEEK_CUR);
        
        // Write the decrypted block back to the file
        fwrite(block, 1, num_bytes, file);
    }

    fclose(file);
    free(block);
}
