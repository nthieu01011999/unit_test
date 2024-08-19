// xor_encrypt.h

#ifndef XOR_ENCRYPT_H
#define XOR_ENCRYPT_H

int is_valid_password(char *password);
void perform_XOR_encrypt(char *filename, char *password);
int is_alpha(char c);
int is_digit(char c);

#endif // XOR_ENCRYPT_H
