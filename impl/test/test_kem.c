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
    uint8_t kg_randomness_pqclean[3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];
    uint8_t kg_randomness_jasmin[3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];

    uint8_t enc_randomness_pqclean[BYTES_MU];
    uint8_t enc_randomness_jasmin[BYTES_MU];

    uint8_t out_pk_pqclean[JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES];
    uint8_t out_pk_jasmin[JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES];

    uint8_t out_sk_pqclean[JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES];
    uint8_t out_sk_jasmin[JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES];

    uint8_t out_ct_pqclean[JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES];
    uint8_t out_ct_jasmin[JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES];

    uint8_t out_ss_pqclean[JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];
    uint8_t out_ss_jasmin[JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];

    uint8_t out_dec_ss_pqclean[JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];
    uint8_t out_dec_ss_jasmin[JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES];

    /* Test crypto_kem_keypair */
    randombytes(kg_randomness_pqclean, 3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES * sizeof(kg_randomness_pqclean[0]));
    memcpy(kg_randomness_jasmin, kg_randomness_pqclean, 3 * JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES * sizeof(kg_randomness_jasmin[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_crypto_kem_keypair(out_pk_pqclean, out_sk_pqclean, kg_randomness_pqclean);
    jazz_crypto_kem_keypair(out_pk_jasmin, out_sk_jasmin, kg_randomness_jasmin);
    for (size_t i = 0; i < JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES; i++)
    {
        assert(out_pk_pqclean[i] == out_pk_jasmin[i]);
    }
    for (size_t i = 0; i < JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES; i++)
    {
        assert(out_sk_pqclean[i] == out_sk_jasmin[i]);
    }

    /* Test crypto_kem_enc */
    randombytes(enc_randomness_pqclean, BYTES_MU * sizeof(enc_randomness_pqclean[0]));
    memcpy(enc_randomness_jasmin, enc_randomness_pqclean, BYTES_MU * sizeof(enc_randomness_jasmin[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_crypto_kem_enc(out_ct_pqclean, out_ss_pqclean, out_pk_pqclean, enc_randomness_pqclean);
    jazz_crypto_kem_enc(out_ct_jasmin, out_ss_jasmin, out_pk_jasmin, enc_randomness_jasmin);
    for (size_t i = 0; i < JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES; i++)
    {
        assert(out_ct_pqclean[i] == out_ct_jasmin[i]);
    }
    for (size_t i = 0; i < JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES; i++)
    {
        assert(out_ss_pqclean[i] == out_ss_jasmin[i]);
    }

    /* Test crypto_kem_dec */
    jazz_crypto_kem_dec(out_dec_ss_jasmin, out_ct_jasmin, out_sk_jasmin);
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_crypto_kem_dec(out_dec_ss_pqclean, out_ct_pqclean, out_sk_pqclean);
    for (size_t i = 0; i < JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES; i++)
    {
        assert(out_dec_ss_pqclean[i] == out_ss_pqclean[i]);
        assert(out_dec_ss_jasmin[i] == out_ss_jasmin[i]);
        assert(out_dec_ss_pqclean[i] == out_dec_ss_jasmin[i]);
    }

    printf("Success!\n");
    return EXIT_SUCCESS;
}
