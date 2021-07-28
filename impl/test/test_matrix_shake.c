// SPDX-FileCopyrightText: 2021 The VeriFrodo Authors
// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "randombytes.h"

#include "pqclean-impl/params.h"
#include "pqclean-impl/common.h"
#include "../common.h"

static void test_mul_add_as_plus_e(void)
{
    uint16_t s[PARAMS_N * PARAMS_NBAR];
    uint16_t e[PARAMS_N * PARAMS_NBAR];
    uint8_t seed_A[BYTES_SEED_A];

    uint16_t out_pqclean[PARAMS_N * PARAMS_NBAR];
    uint16_t out_jasmin[PARAMS_N * PARAMS_NBAR];

    randombytes((uint8_t*)s, (PARAMS_N * PARAMS_NBAR) * sizeof(s[0]));
    randombytes((uint8_t*)e, (PARAMS_N * PARAMS_NBAR) * sizeof(e[0]));
    randombytes(seed_A, BYTES_SEED_A * sizeof(seed_A[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_add_as_plus_e(out_pqclean, s, e, seed_A);
    jazz_mul_add_as_plus_e(out_jasmin, s, e, seed_A);
    for (size_t i = 0; i < PARAMS_N * PARAMS_NBAR; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }
}
static void test_mul_add_sa_plus_e(void)
{
    uint16_t s[PARAMS_N * PARAMS_NBAR];
    uint16_t e[PARAMS_N * PARAMS_NBAR];
    uint8_t seed_A[BYTES_SEED_A];

    uint16_t out_pqclean[PARAMS_NBAR * PARAMS_N];
    uint16_t out_jasmin[PARAMS_NBAR * PARAMS_N];

    randombytes((uint8_t*)s, (PARAMS_N * PARAMS_NBAR) * sizeof(s[0]));
    randombytes((uint8_t*)e, (PARAMS_N * PARAMS_NBAR) * sizeof(e[0]));
    randombytes(seed_A, BYTES_SEED_A * sizeof(seed_A[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_add_sa_plus_e(out_pqclean, s, e, seed_A);
    jazz_mul_add_sa_plus_e(out_jasmin, s, e, seed_A);
    for (size_t i = 0; i < PARAMS_NBAR * PARAMS_N; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }
}

int main(void)
{
    test_mul_add_as_plus_e();
    test_mul_add_sa_plus_e();

    printf("Success!\n");
    return EXIT_SUCCESS;
}
