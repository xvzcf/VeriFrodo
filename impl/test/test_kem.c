// SPDX-FileCopyrightText: 2021 The VeriFrodo Authors
// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "randombytes.h"

#include "pqclean-impl/api.h"
#include "pqclean-impl/params.h"
#include "../api.h"

int main(void)
{
    uint8_t randomness_pqclean[3 * CRYPTO_BYTES];
    uint8_t randomness_jasmin[3 * CRYPTO_BYTES];

    uint8_t out_pk_pqclean[CRYPTO_PUBLICKEYBYTES];
    uint8_t out_pk_jasmin[CRYPTO_PUBLICKEYBYTES];

    uint8_t out_sk_pqclean[CRYPTO_SECRETKEYBYTES];
    uint8_t out_sk_jasmin[CRYPTO_SECRETKEYBYTES];

    /*for (size_t i = 0; i < 3 * CRYPTO_BYTES; i++)
    {
        randomness_pqclean[i] = 0x1;
    }*/
    randombytes(randomness_pqclean, 3 * CRYPTO_BYTES * sizeof(randomness_pqclean[0]));
    memcpy(randomness_jasmin, randomness_pqclean, 3 * CRYPTO_BYTES * sizeof(randomness_jasmin[0]));

    /* Test crypto_kem_keypair */
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_crypto_kem_keypair(out_pk_pqclean, out_sk_pqclean, randomness_pqclean);
    jazz_crypto_kem_keypair(out_pk_jasmin, out_sk_jasmin, randomness_jasmin);
    for(size_t i = 0; i < CRYPTO_PUBLICKEYBYTES; i++)
    {
        //printf("JASMIN[%zu]: %u, PQCLEAN[%zu]: %u\n", i, out_pk_jasmin[i], i, out_pk_pqclean[i]);
        assert(out_pk_jasmin[i] == out_pk_pqclean[i]);
    }
    for(size_t i = 0; i < CRYPTO_SECRETKEYBYTES; i++)
    {
        //printf("JASMIN[%zu]: %u, PQCLEAN[%zu]: %u\n", i, out_sk_jasmin[i], i, out_sk_pqclean[i]);
        assert(out_sk_jasmin[i] == out_sk_pqclean[i]);
    }

    printf("Success!\n");
    return EXIT_SUCCESS;
}
