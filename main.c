#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_FILES 100  // Maximum number of files to store

// Function to check if a file has a specific extension
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
                        filenames[file_count] = malloc(strlen(entry->d_name) + 1);
                        if (filenames[file_count] == NULL) {
                            perror("Memory allocation failed");
                            closedir(dir);
                            return -1;
                        }
                        strcpy(filenames[file_count], entry->d_name);
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

// Function to print the stored filenames
void print_filenames(char *filenames[], int file_count) {
    printf("Regular files (excluding .h, .o, .cpp, .c files):\n");
    for (int i = 0; i < file_count; i++) {
        printf("%s\n", filenames[i]);
        free(filenames[i]);  // Free the memory allocated for each filename
    }
}

int main() {
    char *filenames[MAX_FILES];
    int file_count;

    // Filter files and store the filenames
    file_count = filter_and_store_files("/home/it/Desktop/unit_test/xor_crypt/", filenames, MAX_FILES);
    if (file_count < 0) {
        return 1;
    }

    // Print the stored filenames
    print_filenames(filenames, file_count);

    return 0;
}
