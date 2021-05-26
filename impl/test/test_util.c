// SPDX-FileCopyrightText: 2021 The VeriFrodo Authors
// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "randombytes.h"

#include "pqclean-impl/params.h"
#include "pqclean-impl/common.h"
#include "../common.h"

int main(void)
{
    uint16_t a1[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t b1[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t out_pqclean[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t out_jasmin[PARAMS_NBAR * PARAMS_NBAR];

    randombytes((uint8_t*)a1, (PARAMS_NBAR * PARAMS_NBAR) * sizeof(uint16_t));
    randombytes((uint8_t*)b1, (PARAMS_NBAR * PARAMS_NBAR) * sizeof(uint16_t));

    // Test add
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_add(out_pqclean, a1, b1);
    jazz_add(out_jasmin, a1, b1);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        //printf("a1[%zu]: %u, b1[%zu]: %u, JASMIN[%zu]: %u, PQCLEAN[%zu]: %u\n", i, a1[i], i, b1[i], i, out_jasmin[i], i, out_pqclean[i]);
        assert(out_pqclean[i] == out_jasmin[i]);
    }

    // Test sub
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_sub(out_pqclean, a1, b1);
    jazz_sub(out_jasmin, a1, b1);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }

    uint16_t b2[PARAMS_NBAR * PARAMS_N];
    uint16_t s1[PARAMS_N * PARAMS_NBAR];
    randombytes((uint8_t*)b2, (PARAMS_NBAR * PARAMS_N) * sizeof(uint16_t));
    randombytes((uint8_t*)s1, (PARAMS_N * PARAMS_NBAR) * sizeof(uint16_t));

    // Test mul_bs
    /*for(size_t i = 0; i < PARAMS_NBAR * PARAMS_N; i++)
    {
        b2[i] = 2;
    }

    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_N; i++)
    {
        s1[i] = 2;
    }*/
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_bs(out_pqclean, b2, s1);
    jazz_mul_bs(out_jasmin, b2, s1);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        //printf("JASMIN[%zu]: %u, PQCLEAN[%zu]: %u\n", i, out_jasmin[i], i, out_pqclean[i]);
        assert(out_pqclean[i] == out_jasmin[i]);
    }

    uint16_t e1[PARAMS_NBAR * PARAMS_NBAR];
    randombytes((uint8_t*)e1, (PARAMS_NBAR * PARAMS_NBAR) * sizeof(uint16_t));

    // Test mul_add_sb_plus_e
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_add_sb_plus_e(out_pqclean, b2, s1, e1);
    jazz_mul_add_sb_plus_e(out_jasmin, b2, s1, e1);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }

    uint8_t pack_out_pqclean[(PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8];
    uint8_t pack_out_jazz[(PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8];
    size_t outlen = (PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8;

    randombytes((uint8_t*)s1, (PARAMS_N * PARAMS_NBAR) * sizeof(uint16_t));

    // Test pack
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_pack(pack_out_pqclean, outlen, s1, PARAMS_N * PARAMS_NBAR, PARAMS_LOGQ);
    jazz_pack_N_by_NBAR(pack_out_jazz, s1);
    for(size_t i = 0; i < outlen; i++)
    {
        assert(pack_out_pqclean[i] == pack_out_jazz[i]);
    }

    printf("Success!\n");
    return EXIT_SUCCESS;
}
