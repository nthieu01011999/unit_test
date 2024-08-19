#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  // Include this for memory management functions

// define some constants
#define MAX_FILENAME  15
#define MIN_PASSWORD  8

// Function declarations
void make_new_name_decrypt(char *new_name, char *original_name);
int length_of_password(char *password);
int is_alpha(char c);
int is_digit(char c);
int is_valid_password(char *password);
void perform_XOR_decrypt(char *input_filename, char *output_filename, char *password);
void print_first_five(char *filename);

int main(int argc, char *argv[]) {
    // Check valid arguments
    if (argc != 3) {
        printf("usage:   %s <filename> <password>\n",  argv[0]);
        printf("example: %s encrypted_file.jpg password1\n", argv[0]);
        return 1;
    }

    // Step 1: Create a new filename
    char filename_new[40];  // Increased buffer size to handle longer filenames

    make_new_name_decrypt(filename_new, argv[1]);
    printf("New filename = %s\n", filename_new);
    printf("Password length = %d\n", length_of_password(argv[2]));

    // Step 2: Validate the password
    if (!is_valid_password(argv[2])) return 1;

    // Step 3: Perform the decryption
    perform_XOR_decrypt(argv[1], filename_new, argv[2]);

    // Step 4: Print the first five bytes of the decrypted file
    print_first_five(filename_new);

    return 0;
}

// Append 'decrypted-' in front of filename
void make_new_name_decrypt(char *new_name, char *original_name) {
    strcpy(new_name, "decrypted-");
    strcat(new_name, original_name);
}

// Return the length of the password
int length_of_password(char *password) {
    return strlen(password);
}

// Check if character is alphabetic
int is_alpha(char c) {
    return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) ? 1 : 0;
}

// Check if character is a digit
int is_digit(char c) {
    return ('0' <= c && c <= '9') ? 1 : 0;
}

// Verify if the password meets requirements
int is_valid_password(char *password) {
    int has_char = 0, has_digit = 0;
    int len = strlen(password);

    // Check if it contains both characters and digits
    for (int i = 0; i < len; i++) {
        if (is_alpha(password[i])) has_char = 1;
        if (is_digit(password[i])) has_digit = 1;
    }

    // Messages
    if (len < 8)
        printf("The password needs to have at least 8 characters.\n");
    if (!has_char)
        printf("The password needs to contain at least 1 alphabetical character.\n");
    if (!has_digit)
        printf("The password needs to contain at least 1 digit.\n");

    return (len >= 8 && has_char && has_digit) ? 1 : 0;
}

// Decrypt files with XOR operation using a password
void perform_XOR_decrypt(char *input_filename, char *output_filename, char *password) {
    FILE *input_file, *output_file;
    int block_size = strlen(password);
    unsigned char *block = malloc(block_size * sizeof(unsigned char)); // Dynamically allocate memory
    int num_bytes;

    if (block == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    // Open files
    input_file  = fopen(input_filename, "rb");
    output_file = fopen(output_filename, "wb");

    if (input_file == NULL) {
        printf("Error opening input file!\n");
        free(block);
        return;
    }
    if (output_file == NULL) {
        printf("Error opening output file!\n");
        fclose(input_file);
        free(block);
        return;
    }

    // Decryption (XOR operation is symmetric)
    while ((num_bytes = fread(block, 1, block_size, input_file)) > 0) {
        // XOR byte by byte
        for (int i = 0; i < num_bytes; i++) {
            block[i] ^= password[i];
        }

        fwrite(block, 1, num_bytes, output_file);
    }

    // Clean up
    fclose(input_file);
    fclose(output_file);
    free(block);
}

// Output the first five bytes of the file in 2-digit hex
void print_first_five(char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    unsigned char block[5] = {0};  // Initialize block to ensure no garbage values

    fread(block, 1, 5, file);

    for (int i = 0; i < 5; i++)
        printf("%02x\n", block[i]);

    // Clean up
    fclose(file);
}

