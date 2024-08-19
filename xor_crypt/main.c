#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include "xor_encrypt.h"
#include "xor_decrypt.h"

#define MAX_FILES 100  // Maximum number of files to store

// Fixed directory and password
const char* directory = "/home/it/Desktop/unit_test/xor_crypt/configure";
const char* password = "password123";

void perform_XOR_encrypt(char *filename, char *password);
void perform_XOR_decrypt(char *filename, char *password);
void print_first_five(char *filename);
int has_extension(const char *filename, const char *extension);
int filter_and_store_files(const char *directory, char *filenames[], int max_files);
void print_filenames(char *filenames[], int file_count);

int main() {
    char *filenames[MAX_FILES];
    int file_count;

    // Filter files and store the filenames
    file_count = filter_and_store_files(directory, filenames, MAX_FILES);
    if (file_count < 0) {
        return 1;
    }

    // Encrypt files
    // printf("Encrypting files in directory: %s\n", directory);
    // for (int i = 0; i < file_count; i++) {
    //     printf("Encrypting %s...\n", filenames[i]);
    //     perform_XOR_encrypt(filenames[i], (char*)password);
    // }



    // Decrypt files (if needed)
    // Uncomment the following lines if you want to decrypt after encrypting
    
    printf("Decrypting files in directory: %s\n", directory);
    for (int i = 0; i < file_count; i++) {
        printf("Decrypting %s...\n", filenames[i]);
        perform_XOR_decrypt(filenames[i], (char*)password);
    }
    

    // Free allocated memory for filenames
    for (int i = 0; i < file_count; i++) {
        free(filenames[i]);
    }

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

int has_extension(const char *filename, const char *extension) {
    size_t len_filename = strlen(filename);
    size_t len_extension = strlen(extension);

    if (len_filename >= len_extension) {
        return strcmp(filename + len_filename - len_extension, extension) == 0;
    }

    return 0;
}

// Function to filter files based on extension and store the valid filenames
int filter_and_store_files(const char *directory, char *filenames[], int max_files) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char filepath[1024];
    int file_count = 0;

    // Open the directory
    dir = opendir(directory);
    if (dir == NULL) {
        perror("Unable to open directory");
        return -1;
    }

    // Read the directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip the "." and ".." entries
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // Construct the full file path
            snprintf(filepath, sizeof(filepath), "%s/%s", directory, entry->d_name);

            // Get file statistics
            if (stat(filepath, &file_stat) == 0) {
                // Check if it is a regular file
                if (S_ISREG(file_stat.st_mode)) {
                    // Check if the file does not have .h, .o, .cpp, or .c extension
                    if (!has_extension(entry->d_name, ".h") &&
                        !has_extension(entry->d_name, ".o") &&
                        !has_extension(entry->d_name, ".cpp") &&
                        !has_extension(entry->d_name, ".c")) {
                        
                        // Store the filename in a dynamically allocated array
                        filenames[file_count] = malloc(strlen(filepath) + 1);
                        if (filenames[file_count] == NULL) {
                            perror("Memory allocation failed");
                            closedir(dir);
                            return -1;
                        }
                        strcpy(filenames[file_count], filepath);
                        file_count++;

                        if (file_count >= max_files) {
                            printf("Maximum file count reached, stopping.\n");
                            break;
                        }
                    }
                }
            } else {
                perror("Unable to get file status");
            }
        }
    }

    // Close the directory
    closedir(dir);

    return file_count;
}
