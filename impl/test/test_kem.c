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
    uint8_t kg_randomness_pqclean[3 * CRYPTO_BYTES];
    uint8_t kg_randomness_jasmin[3 * CRYPTO_BYTES];

    uint8_t out_pk_pqclean[CRYPTO_PUBLICKEYBYTES];
    uint8_t out_pk_jasmin[CRYPTO_PUBLICKEYBYTES];

    uint8_t out_sk_pqclean[CRYPTO_SECRETKEYBYTES];
    uint8_t out_sk_jasmin[CRYPTO_SECRETKEYBYTES];

    /*for (size_t i = 0; i < 3 * CRYPTO_BYTES; i++)
    {
        kg_randomness_pqclean[i] = 0x1;
    }*/
    randombytes(kg_randomness_pqclean, 3 * CRYPTO_BYTES * sizeof(kg_randomness_pqclean[0]));
    memcpy(kg_randomness_jasmin, kg_randomness_pqclean, 3 * CRYPTO_BYTES * sizeof(kg_randomness_jasmin[0]));

    /* Test crypto_kem_keypair */
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_crypto_kem_keypair(out_pk_pqclean, out_sk_pqclean, kg_randomness_pqclean);
    jazz_crypto_kem_keypair(out_pk_jasmin, out_sk_jasmin, kg_randomness_jasmin);
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

    /* Test crypto_kem_enc */
    uint8_t enc_randomness_pqclean[BYTES_MU];
    uint8_t enc_randomness_jasmin[BYTES_MU];

    uint8_t out_ct_pqclean[CRYPTO_CIPHERTEXTBYTES];
    uint8_t out_ct_jasmin[CRYPTO_CIPHERTEXTBYTES];

    uint8_t out_ss_pqclean[CRYPTO_BYTES];
    uint8_t out_ss_jasmin[CRYPTO_BYTES];

    randombytes(enc_randomness_pqclean, BYTES_MU * sizeof(enc_randomness_pqclean[0]));
    memcpy(enc_randomness_jasmin, enc_randomness_pqclean, BYTES_MU * sizeof(enc_randomness_jasmin[0]));

    /*for(size_t i = 0; i < CRYPTO_PUBLICKEYBYTES; i++)
    {
        out_pk_pqclean[i] = 0;
        out_pk_jasmin[i] = 0;
    }
    for(size_t i = 0; i < BYTES_MU; i++)
    {
        enc_randomness_pqclean[i] = 0;
        enc_randomness_jasmin[i] = 0;
    }*/

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_crypto_kem_enc(out_ct_pqclean, out_ss_pqclean, out_pk_pqclean, enc_randomness_pqclean);
    jazz_crypto_kem_enc(out_ct_jasmin, out_ss_jasmin, out_pk_jasmin, enc_randomness_jasmin);
    for(size_t i = 0; i < CRYPTO_CIPHERTEXTBYTES; i++)
    {
        //printf("JASMIN[%zu]: %u, PQCLEAN[%zu]: %u\n", i, out_ct_jasmin[i], i, out_ct_pqclean[i]);
        assert(out_ct_jasmin[i] == out_ct_pqclean[i]);
    }
    for(size_t i = 0; i < CRYPTO_BYTES; i++)
    {
        //printf("JASMIN[%zu]: %u, PQCLEAN[%zu]: %u\n", i, out_ss_jasmin[i], i, out_ss_pqclean[i]);
        assert(out_ss_jasmin[i] == out_ss_pqclean[i]);
    }

    printf("Success!\n");
    return EXIT_SUCCESS;
}
