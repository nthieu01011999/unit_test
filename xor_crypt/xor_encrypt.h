// xor_encrypt.h

#ifndef XOR_ENCRYPT_H
#define XOR_ENCRYPT_H

void make_new_name_encrypt(char *new_name, char *original_name);
int length_of_password(char *password);
int is_alpha(char c);
int is_digit(char c);
int is_valid_password(char *password);
void perform_XOR_encrypt(char *input_filename, char *output_filename, char *password);

#endif // XOR_ENCRYPT_H
