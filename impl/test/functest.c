// SPDX-FileCopyrightText: 2019 The PQClean Authors
// SPDX-License-Identifier: CC0-1.0

#include "../api.h"
#include "randombytes.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define NTESTS 5

const uint8_t canary[8] =
{
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
};

/* allocate a bit more for all keys and messages and
 * make sure it is not touched by the implementations.
 */
static void write_canary(uint8_t* d)
{
    for (size_t i = 0; i < 8; i++)
    {
        d[i] = canary[i];
    }
}

static int check_canary(const uint8_t* d)
{
    for (size_t i = 0; i < 8; i++)
    {
        if (d[i] != canary[i])
        {
            return -1;
        }
    }
    return 0;
}

inline static void* malloc_s(size_t size)
{
    void* ptr = malloc(size);
    if (ptr == NULL)
    {
        perror("Malloc failed!");
        exit(1);
    }
    return ptr;
}

static int test_mem(void)
{
    /*
     * This is most likely going to be aligned by the compiler.
     * 16 extra bytes for canary
     * 1 extra byte for unalignment
     */
    int res = 0;

    uint8_t* ss_a_aligned = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES + 16 + 1);
    uint8_t* ss_b_aligned = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES + 16 + 1);
    uint8_t* pk_a_aligned    = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES + 16 + 1);
    uint8_t* ct_b_aligned = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES + 16 + 1);
    uint8_t* sk_a_aligned  = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES + 16 + 1);
    uint8_t* kg_randomness_aligned = malloc_s((3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES) + 16 + 1);
    uint8_t* enc_randomness_aligned = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES + 16 + 1);

    /*
     * Make sure all pointers are odd.
     * This ensures that the implementation does not assume anything about the
     * data alignment. For example this would catch if an implementation
     * directly uses these pointers to load into vector registers using movdqa.
     */
    uint8_t* ss_a = (uint8_t*) ((uintptr_t) ss_a_aligned | (uintptr_t) 1);
    uint8_t* ss_b = (uint8_t*) ((uintptr_t) ss_b_aligned | (uintptr_t) 1);
    uint8_t* pk_a    = (uint8_t*) ((uintptr_t) pk_a_aligned | (uintptr_t) 1);
    uint8_t* ct_b = (uint8_t*) ((uintptr_t) ct_b_aligned | (uintptr_t) 1);
    uint8_t* sk_a  = (uint8_t*) ((uintptr_t) sk_a_aligned | (uintptr_t) 1);
    uint8_t* kg_randomness = (uint8_t*) ((uintptr_t) kg_randomness_aligned | (uintptr_t) 1);
    uint8_t* enc_randomness = (uint8_t*) ((uintptr_t) enc_randomness_aligned | (uintptr_t) 1);

    /*
     * Write 8 byte canary before and after the actual memory regions.
     * This is used to validate that the implementation does not assume
     * anything about the placement of data in memory
     * (e.g., assuming that the pk_a is always behind the sk)
     */
    write_canary(ss_a);
    write_canary(ss_a + JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES + 8);
    write_canary(ss_b);
    write_canary(ss_b + JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES + 8);
    write_canary(pk_a);
    write_canary(pk_a +  JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES + 8);
    write_canary(ct_b);
    write_canary(ct_b + JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES + 8);
    write_canary(sk_a);
    write_canary(sk_a + JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES + 8);
    write_canary(kg_randomness);
    write_canary(kg_randomness + (3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES) + 8);
    write_canary(enc_randomness);
    write_canary(enc_randomness + (JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES) + 8);

    for (size_t i = 0; i < NTESTS; i++)
    {
        // Alice generates a public and secret key.
        randombytes(kg_randomness + 8, 3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
        jazz_crypto_kem_keypair(pk_a + 8, sk_a + 8, kg_randomness + 8);

        // Bob computes a shared secret and encapsulates it in a
        // ciphertext using Alice's public key.
        randombytes(enc_randomness + 8, JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
        jazz_crypto_kem_enc(ct_b + 8, ss_b + 8, pk_a + 8, enc_randomness + 8);

        // Alice decapsulates Bob's ciphertext with her secret key
        // to get the shared secret.
        jazz_crypto_kem_dec(ss_a + 8, ct_b + 8, sk_a + 8);

        if (memcmp(ss_a + 8, ss_b + 8, JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES) != 0)
        {
            printf("Error: Shared secrets don't match.\n");
            res = 1;
            goto end;
        }

        // Validate that the implementation did not touch any of the
        // canaries.
        if (check_canary(ss_a) != 0)
        {
            printf("Error: ss_a left canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary((ss_a + 8) + JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES) != 0)
        {
            printf("Error: ss_a right canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary(ss_b) != 0)
        {
            printf("Error: ss_b left canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary((ss_b + 8) + JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES) != 0)
        {
            printf("Error: ss_b right canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary(pk_a) != 0)
        {
            printf("Error: pk_a left canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary((pk_a + 8) + JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES) != 0)
        {
            printf("Error: pk_a right canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary(ct_b) != 0)
        {
            printf("Error: ct_b left canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary((ct_b + 8) + JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES) != 0)
        {
            printf("Error: ct_b right canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary(sk_a) != 0)
        {
            printf("Error: sk_a left canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary((sk_a + 8) + JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES) != 0)
        {
            printf("error: sk_a right canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary(kg_randomness) != 0)
        {
            printf("error: kg_randomness left canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary((kg_randomness + 8) + (3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES)) != 0)
        {
            printf("error: kg_randomness right canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary(enc_randomness) != 0)
        {
            printf("error: enc_randomness left canary overwritten\n");
            res = 1;
            goto end;
        }
        else if (check_canary((enc_randomness + 8) + JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES))
        {
            printf("error: enc_randomness right canary overwritten\n");
            res = 1;
            goto end;
        }
    }

end:
    free(ss_a_aligned);
    free(ss_b_aligned);
    free(pk_a_aligned);
    free(ct_b_aligned);
    free(sk_a_aligned);
    free(kg_randomness_aligned);
    free(enc_randomness_aligned);

    return res;
}

static int test_invalid_sk_a(void)
{
    uint8_t* sk_a  = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES);
    uint8_t* ss_a = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
    uint8_t* ss_b = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
    uint8_t* pk_a    = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES);
    uint8_t* ct_b = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES);
    uint8_t* kg_randomness = malloc_s(3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
    uint8_t* enc_randomness = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
    int res = 0;

    for (size_t i = 0; i < NTESTS; i++)
    {
        // Alice generates a public and secret key.
        randombytes(kg_randomness, 3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
        jazz_crypto_kem_keypair(pk_a, sk_a, kg_randomness);

        // Bob computes a shared secret and encapsulates it in a
        // ciphertext using Alice's public key.
        randombytes(enc_randomness, JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
        jazz_crypto_kem_enc(ct_b, ss_b, pk_a, enc_randomness);

        // Overwrite Alice's secret key with random values.
        randombytes(sk_a, JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES);

        // Alice decapsulates Bob's ciphertext with her secret key
        // to get the shared secret.
        jazz_crypto_kem_dec(ss_a, ct_b, sk_a);

        if (memcmp(ss_a, ss_b, JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES) == 0)
        {
            printf("Error: invalid sk_a\n");
            res = 1;
            goto end;
        }
    }

end:
    free(sk_a);
    free(ss_a);
    free(ss_b);
    free(pk_a);
    free(ct_b);

    return res;
}

static int test_invalid_ciphertext(void)
{
    uint8_t* sk_a = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES);
    uint8_t* ss_a = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
    uint8_t* ss_b = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
    uint8_t* pk_a = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES);
    uint8_t* ct_b = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES);
    uint8_t* kg_randomness = malloc_s(3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
    uint8_t* enc_randomness = malloc_s(JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
    int i;
    int res = 0;

    for (i = 0; i < NTESTS; i++)
    {
        // Alice generates a public and secret key.
        randombytes(kg_randomness, 3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
        jazz_crypto_kem_keypair(pk_a, sk_a, kg_randomness);

        // Bob computes a shared secret and encapsulates it in a
        // ciphertext using Alice's public key.
        randombytes(enc_randomness, JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES);
        jazz_crypto_kem_enc(ct_b, ss_b, pk_a, enc_randomness);

        // Overwrite Bob's ciphertext with random values.
        randombytes(ct_b, sizeof(ct_b));

        // Alice decapsulates Bob's ciphertext with her secret key
        // to get the shared secret.
        jazz_crypto_kem_dec(ss_a, ct_b, sk_a);

        if (memcmp(ss_a, ss_b, JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES) == 0)
        {
            printf("Error: invalid ciphertext\n");
            res = 1;
            goto end;
        }
    }
end:
    free(sk_a);
    free(ss_a);
    free(ss_b);
    free(pk_a);
    free(ct_b);

    return res;
}

int main(void)
{
    assert(test_mem() == 0);
    assert(test_invalid_sk_a() == 0);
    assert(test_invalid_ciphertext() == 0);

    printf("Success!\n");
    return EXIT_SUCCESS;
}
