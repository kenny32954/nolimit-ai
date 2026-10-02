#include "base64.h"

static char const table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static int decode_value(char c)
{
    if(c >= 'A' && c <= 'Z') return c - 'A';
    if(c >= 'a' && c <= 'z') return c - 'a' + 26;
    if(c >= '0' && c <= '9') return c - '0' + 52;
    if(c == '+') return 62;
    if(c == '/') return 63;
    return -1;
}

size_t qb_base64_encoded_size(size_t input_size)
{
    return ((input_size + 2) / 3) * 4;
}

size_t qb_base64_encode(unsigned char const *input, size_t input_size,
                        char *output, size_t output_size)
{
    size_t need = qb_base64_encoded_size(input_size);
    size_t i;
    size_t o = 0;

    if(output_size < need + 1) return 0;

    for(i = 0; i < input_size; i += 3) {
        size_t remain = input_size - i;
        unsigned int a = input[i];
        unsigned int b = remain > 1 ? input[i + 1] : 0;
        unsigned int c = remain > 2 ? input[i + 2] : 0;
        unsigned int triple = (a << 16) | (b << 8) | c;

        output[o++] = table[(triple >> 18) & 0x3f];
        output[o++] = table[(triple >> 12) & 0x3f];
        output[o++] = remain > 1 ? table[(triple >> 6) & 0x3f] : '=';
        output[o++] = remain > 2 ? table[triple & 0x3f] : '=';
    }

    output[o] = '\0';
    return o;
}

size_t qb_base64_decode(char const *input, unsigned char *output,
                        size_t output_size)
{
    size_t o = 0;
    int vals[4];
    int n = 0;

    while(*input) {
        char c = *input++;
        int value = decode_value(c);

        if(c == '=') {
            vals[n++] = -2;
        }
        else if(value >= 0) {
            vals[n++] = value;
        }
        else {
            continue;
        }

        if(n == 4) {
            unsigned int a = vals[0] < 0 ? 0 : (unsigned int)vals[0];
            unsigned int b = vals[1] < 0 ? 0 : (unsigned int)vals[1];
            unsigned int c2 = vals[2] < 0 ? 0 : (unsigned int)vals[2];
            unsigned int d = vals[3] < 0 ? 0 : (unsigned int)vals[3];
            unsigned int triple = (a << 18) | (b << 12) | (c2 << 6) | d;

            if(o >= output_size) return 0;
            output[o++] = (triple >> 16) & 0xff;

            if(vals[2] != -2) {
                if(o >= output_size) return 0;
                output[o++] = (triple >> 8) & 0xff;
            }

            if(vals[3] != -2) {
                if(o >= output_size) return 0;
                output[o++] = triple & 0xff;
            }

            n = 0;
        }
    }

    return o;
}
