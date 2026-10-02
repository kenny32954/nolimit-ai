#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/base64.h"

static void fail(char const *message)
{
    fprintf(stderr, "FAIL: %s\n", message);
    exit(1);
}

int main(void)
{
    static unsigned char const input[] = "Quantum Breaks AI - CG50";
    char encoded[128];
    unsigned char decoded[128];
    size_t enc_len;
    size_t dec_len;

    enc_len = qb_base64_encode(input, strlen((char const *)input),
                               encoded, sizeof(encoded));
    if(enc_len == 0) fail("encode returned zero");

    dec_len = qb_base64_decode(encoded, decoded, sizeof(decoded) - 1);
    decoded[dec_len] = '\0';

    if(strcmp((char const *)decoded, (char const *)input) != 0) {
        fail("round trip mismatch");
    }

    if(strcmp(encoded, "UXVhbnR1bSBCcmVha3MgQUkgLSBDRzUw") != 0) {
        fail("unexpected encoded form");
    }

    if(qb_base64_encode(input, strlen((char const *)input), encoded, 4) != 0) {
        fail("small output buffer should fail");
    }

    puts("CG50 Base64 tests passed");
    return 0;
}
