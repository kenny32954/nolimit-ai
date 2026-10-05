#pragma once
#include <stddef.h>

size_t qb_base64_encoded_size(size_t input_size);
size_t qb_base64_encode(unsigned char const *input, size_t input_size,
                        char *output, size_t output_size);
size_t qb_base64_decode(char const *input, unsigned char *output,
                        size_t output_size);
