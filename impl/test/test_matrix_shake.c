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

int main(void)
{
    uint16_t s[PARAMS_N * PARAMS_NBAR];
    uint16_t e[PARAMS_N * PARAMS_NBAR];
    uint8_t seed_A[BYTES_SEED_A];

    uint16_t out_pqclean[PARAMS_N * PARAMS_NBAR];
    uint16_t out_jasmin[PARAMS_N * PARAMS_NBAR];

    randombytes((uint8_t*)s, (PARAMS_N * PARAMS_NBAR) * sizeof(s[0]));
    randombytes((uint8_t*)e, (PARAMS_N * PARAMS_NBAR) * sizeof(e[0]));
    randombytes(seed_A, BYTES_SEED_A * sizeof(seed_A[0]));

    /*for(size_t i = 0; i < PARAMS_N * PARAMS_NBAR; i++)
    {
        s[i] = 1;
        e[i] = 1;
    }
    for(size_t i = 0; i < BYTES_SEED_A; i++)
    {
        seed_A[i] = 0;
    }*/

    /* Test mul_add_as_plus_e */
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_add_as_plus_e(out_pqclean, s, e, seed_A);
    jazz_mul_add_as_plus_e(out_jasmin, s, e, seed_A);
    for(size_t i = 0; i < PARAMS_N * PARAMS_NBAR; i++)
    {
        //printf("i = %zu, PQCLEAN = %u, JASMIN = %u\n", i, out_pqclean[i], out_jasmin[i]);
        assert(out_pqclean[i] == out_jasmin[i]);
    }

    memset(out_pqclean, 0, PARAMS_NBAR * PARAMS_N * sizeof(out_pqclean[0]));
    memset(out_jasmin, 1, PARAMS_NBAR * PARAMS_N * sizeof(out_jasmin[0]));

    /* Test mul_add_sa_plus_e */
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_add_sa_plus_e(out_pqclean, s, e, seed_A);
    jazz_mul_add_sa_plus_e(out_jasmin, s, e, seed_A);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_N; i++)
    {
        //printf("i = %zu, PQCLEAN = %u, JASMIN = %u\n", i, out_pqclean[i], out_jasmin[i]);
        assert(out_pqclean[i] == out_jasmin[i]);
    }

    printf("Success!\n");
    return EXIT_SUCCESS;
}
