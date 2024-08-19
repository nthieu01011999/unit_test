// main.c
#include <stdio.h>
#include <string.h>

void make_new_name_encrypt(char *new_name, char *original_name);
void make_new_name_decrypt(char *new_name, char *original_name);
int is_valid_password(char *password);
void perform_XOR_encrypt(char *input_filename, char *output_filename, char *password);
void perform_XOR_decrypt(char *input_filename, char *output_filename, char *password);

void print_first_five(char *filename);

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("usage:   %s <encrypt/decrypt> <filename> <password>\n", argv[0]);
        printf("example: %s encrypt auckland.jpg password1\n", argv[0]);
        printf("         %s decrypt new-auckland.jpg password1\n", argv[0]);
        return 1;
    }

    char filename_new[40];  // Increased buffer size to handle longer filenames

    if (strcmp(argv[1], "encrypt") == 0) {
        make_new_name_encrypt(filename_new, argv[2]);
        printf("Encrypting... New filename = %s\n", filename_new);
        if (!is_valid_password(argv[3])) return 1;
        perform_XOR_encrypt(argv[2], filename_new, argv[3]);
    } else if (strcmp(argv[1], "decrypt") == 0) {
        make_new_name_decrypt(filename_new, argv[2]);
        printf("Decrypting... New filename = %s\n", filename_new);
        if (!is_valid_password(argv[3])) return 1;
        perform_XOR_decrypt(argv[2], filename_new, argv[3]);
    } else {
        printf("Invalid operation: %s. Please use 'encrypt' or 'decrypt'.\n", argv[1]);
        return 1;
    }

    print_first_five(filename_new);
    return 0;
}

void print_first_five(char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    unsigned char block[5] = {0};

    fread(block, 1, 5, file);

    for (int i = 0; i < 5; i++)
        printf("%02x\n", block[i]);

    fclose(file);
}
