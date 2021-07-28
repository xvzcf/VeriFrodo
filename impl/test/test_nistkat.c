/********************************************************************************************
* Abstract: run tests against known answer test vectors for FrodoKEM-640 using SHAKE128
*
* Modified from a file created by Bassham, Lawrence E (Fed) on 8/29/17.
* Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
*********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "randombytes.h"
#include "../api.h"

void nist_kat_init(unsigned char* entropy_input, unsigned char* personalization_string, int security_strength);

static uint8_t from_hex_digit(char c)
{
    if ((c >= '0') && (c <= '9'))
    {
        return (c - '0');
    }
    if ((c >= 'A') && (c <= 'F'))
    {
        return ((c - 'A') + 10);
    }
    else
    {
        return 0xFF;
    }
}

static int parse_count(const char* line)
{
    size_t i;
    char* key = "count";
    size_t key_len = strlen(key);
    int count = 0;

    for (i = 0; i < key_len; i++)
    {
        if (line[i] != key[i])
        {
            return -1;
        }
    }
    i += 1;
    if (line[i] != '=')
    {
        return -1;
    }
    i += 2;

    // Does not do overflow checking; this should not pose a problem
    // since count <= 99.
    while (line[i] != '\n')
    {
        count = (count * 10) + (line[i] - '0');
        i++;
    }

    return count;
}

static int parse_hex_value(const char* line, const char* key, const size_t key_len, unsigned char* value)
{
    size_t i, j;
    uint8_t byte1, byte2;

    for (i = 0; i < key_len; i++)
    {
        if (line[i] != key[i])
        {
            return -1;
        }
    }
    i += 1;
    if (line[i] != '=')
    {
        return -1;
    }
    i += 2;

    j = 0;
    while (line[i] != '\n')
    {
        byte1 = from_hex_digit(line[i]);
        if (byte1 == 0xFF)
        {
            return -1;
        }
        i += 1;

        byte2 = from_hex_digit(line[i]);
        if (byte2 == 0xFF)
        {
            return -1;
        }
        i += 1;

        value[j] = (byte1 << 4) | byte2;
        j += 1;
    }
    return 0;
}

int main(void)
{
    char* kat_filename = "test/kat.rsp";
    FILE* kat_fp;

    char* line = NULL;
    size_t line_len = 0;
    ssize_t read_len;

    uint8_t seed[48];
    unsigned char kg_randomness[3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];
    unsigned char enc_randomness[JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];

    unsigned char pk[JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES];
    unsigned char kat_pk[JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES];

    unsigned char sk[JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES];
    unsigned char kat_sk[JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES];

    unsigned char ct[JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES];
    unsigned char kat_ct[JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES];

    unsigned char ss[JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];
    unsigned char kat_ss[JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];

    unsigned char ss_decaps[JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];

    int ret, done;

    char* kat_header = "# FrodoKEM-640-SHAKE";

    kat_fp = fopen(kat_filename, "r");
    if (kat_fp == NULL)
    {
        printf("Couldn't open %s for reading.\n", kat_filename);
        ret = EXIT_FAILURE;
        goto cleanup;
    }

    // Read the header
    read_len = getline(&line, &line_len, kat_fp);
    if (read_len == -1)
    {
        printf("Reading header failed.\n");
        ret = EXIT_FAILURE;
        goto cleanup;
    }
    ret = memcmp(line, kat_header, strlen(kat_header));
    if (ret != 0)
    {
        printf("Wrong header.");
        ret = EXIT_FAILURE;
        goto cleanup;
    }

    do
    {
        // Skip the newline.
        read_len = getline(&line, &line_len, kat_fp);
        if (read_len == -1)
        {
            printf("Reading newline failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }

        // Read a new KAT set.
        read_len = getline(&line, &line_len, kat_fp);
        if (read_len == -1)
        {
            printf("Reading count failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        ret = parse_count(line);
        if (ret == -1)
        {
            printf("Parsing count failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        if (ret == 99)
        {
            done = 1;
        }

        read_len = getline(&line, &line_len, kat_fp);
        if (read_len == -1)
        {
            printf("Reading seed failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        ret = parse_hex_value(line, "seed", strlen("seed"), seed);
        if (ret == -1)
        {
            printf("Parsing seed failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        nist_kat_init(seed, NULL, 256);

        // Verify key-generation.
        randombytes(kg_randomness, 3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
        jazz_crypto_kem_keypair(pk, sk, kg_randomness);

        // Check pk.
        read_len = getline(&line, &line_len, kat_fp);
        if (read_len == -1)
        {
            printf("Reading pk failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        ret = parse_hex_value(line, "pk", strlen("pk"), kat_pk);
        if (ret != 0)
        {
            printf("Parsing pk failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        ret = memcmp(pk, kat_pk, JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES);
        if (ret != 0)
        {
            printf("Computed pk does not match KAT.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }

        // Check sk.
        read_len = getline(&line, &line_len, kat_fp);
        if (read_len == -1)
        {
            printf("Reading sk failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        ret = parse_hex_value(line, "sk", strlen("sk"), kat_sk);
        if (ret != 0)
        {
            printf("Parsing sk failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        ret = memcmp(sk, kat_sk, JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES);
        if (ret != 0)
        {
            printf("Computed sk does not match KAT.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }

        // Verify encapsulation.
        randombytes(enc_randomness, JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
        jazz_crypto_kem_enc(ct, ss, pk, enc_randomness);

        // Check ct
        read_len = getline(&line, &line_len, kat_fp);
        if (read_len == -1)
        {
            printf("Reading ct failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        ret = parse_hex_value(line, "ct", strlen("ct"), kat_ct);
        if (ret != 0)
        {
            printf("Parsing ct failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        ret = memcmp(ct, kat_ct, JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES);
        if (ret != 0)
        {
            printf("Computed ct does not match KAT.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }

        // Check ss
        read_len = getline(&line, &line_len, kat_fp);
        if (read_len == -1)
        {
            printf("Reading ss failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        ret = parse_hex_value(line, "ss", strlen("ss"), kat_ss);
        if (ret != 0)
        {
            printf("Parsing ss failed.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
        ret = memcmp(ss, kat_ss, JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
        if (ret != 0)
        {
            printf("Computed ss does not match KAT.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }

        // Verify decapsulation.
        jazz_crypto_kem_dec(ss_decaps, ct, sk);
        ret = memcmp(ss, ss_decaps, JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
        if (ret != 0)
        {
            printf("ss does not match ss_decaps.\n");
            ret = EXIT_FAILURE;
            goto cleanup;
        }
    }
    while (done == 0);

    printf("Success!\n");
    ret = EXIT_SUCCESS;

cleanup:
    free(line);
    fclose(kat_fp);
    return ret;
}
