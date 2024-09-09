#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Prototype for the base64_encode and base64_decode functions
char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length);
unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);

// Helper function to find the index of a character in the Base64 encoding table
int base64_char_index(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;  // Invalid Base64 character
}

unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length) {
    if (input_length % 4 != 0) {
        return NULL; // Input length must be a multiple of 4
    }

    // Calculate the output length
    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    // Allocate memory for the output
    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;

    size_t input_index = 0, output_index = 0;
    while (input_index < input_length) {
        uint32_t sextet_a = data[input_index] == '=' ? 0 & input_index++ : base64_char_index(data[input_index++]);
        uint32_t sextet_b = data[input_index] == '=' ? 0 & input_index++ : base64_char_index(data[input_index++]);
        uint32_t sextet_c = data[input_index] == '=' ? 0 & input_index++ : base64_char_index(data[input_index++]);
        uint32_t sextet_d = data[input_index] == '=' ? 0 & input_index++ : base64_char_index(data[input_index++]);

        uint32_t triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;

        if (output_index < *output_length) decoded_data[output_index++] = (triple >> 16) & 0xFF;
        if (output_index < *output_length) decoded_data[output_index++] = (triple >> 8) & 0xFF;
        if (output_index < *output_length) decoded_data[output_index++] = triple & 0xFF;
    }

    return decoded_data;
}

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length) {
    // The Base64 encoding characters
    static const char encoding_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    static const char padding_char = '=';

    // Calculate output length
    *output_length = 4 * ((input_length + 2) / 3);

    // Allocate memory for the output string
    char *encoded_data = malloc(*output_length + 1); // +1 for null terminator
    if (encoded_data == NULL) return NULL;

    size_t input_index = 0, output_index = 0;
    while (input_length >= 3) {
        uint32_t triple = (data[input_index] << 16) | (data[input_index + 1] << 8) | data[input_index + 2];
        encoded_data[output_index++] = encoding_table[(triple >> 18) & 0x3F];
        encoded_data[output_index++] = encoding_table[(triple >> 12) & 0x3F];
        encoded_data[output_index++] = encoding_table[(triple >> 6) & 0x3F];
        encoded_data[output_index++] = encoding_table[triple & 0x3F];
        input_index += 3;
        input_length -= 3;
    }

    // Handle remaining bytes and padding
    if (input_length > 0) {
        uint32_t triple = (data[input_index] << 16) | ((input_length > 1 ? data[input_index + 1] : 0) << 8);
        encoded_data[output_index++] = encoding_table[(triple >> 18) & 0x3F];
        encoded_data[output_index++] = encoding_table[(triple >> 12) & 0x3F];
        if (input_length == 2) {
            encoded_data[output_index++] = encoding_table[(triple >> 6) & 0x3F];
            encoded_data[output_index++] = padding_char;
        } else {
            encoded_data[output_index++] = padding_char;
            encoded_data[output_index++] = padding_char;
        }
    }

    // Null-terminate the output string
    encoded_data[output_index] = '\0';

    return encoded_data;
}

// Complex unit test for Base64 encode and decode
void run_complex_test(const char *test_name, const unsigned char *input, size_t input_length) {
    size_t encoded_length, decoded_length;

    // Encode the input data
    char *encoded_data = base64_encode(input, input_length, &encoded_length);
    if (encoded_data == NULL) {
        printf("%s: Encoding failed\n", test_name);
        return;
    }

    // Decode the encoded data
    unsigned char *decoded_data = base64_decode(encoded_data, encoded_length, &decoded_length);
    if (decoded_data == NULL) {
        printf("%s: Decoding failed\n", test_name);
        free(encoded_data);
        return;
    }

    // Verify that the decoded data matches the original input
    if (decoded_length == input_length && memcmp(input, decoded_data, input_length) == 0) {
        printf("%s: Passed\n", test_name);
    } else {
        printf("%s: Failed - decoded data does not match original input\n", test_name);
    }

    // Clean up
    free(encoded_data);
    free(decoded_data);
}

void test_base64_encode_decode() {
    // Complex test cases for encode and decode

    // Test with regular ASCII string
    const unsigned char input1[] = "Hello, World!";
    run_complex_test("Test 1: Regular ASCII String", input1, sizeof(input1) - 1);

    // Test with binary data with null bytes
    const unsigned char input2[] = {0x00, 0x01, 0x02, 0x03, 0xFF, 0xFE, 0xFD, 0xFC};
    run_complex_test("Test 2: Binary Data with Null Bytes", input2, sizeof(input2));

    // Test with a larger dataset
    unsigned char input3[1024];
    for (int i = 0; i < sizeof(input3); i++) {
        input3[i] = (unsigned char)(i % 256);
    }
    run_complex_test("Test 3: Large Dataset", input3, sizeof(input3));

    // Test with very small data
    const unsigned char input4[] = {0xFA};
    run_complex_test("Test 4: Very Small Data", input4, sizeof(input4));

    // Test with all possible byte values (0x00 to 0xFF)
    unsigned char input5[256];
    for (int i = 0; i < 256; i++) {
        input5[i] = (unsigned char)i;
    }
    run_complex_test("Test 5: All Byte Values (0x00 to 0xFF)", input5, sizeof(input5));
}

int main() {
    // Run complex tests for Base64 encode and decode
    test_base64_encode_decode();

    return 0;
}
