// SPDX-FileCopyrightText: 2021 The VeriFrodo Authors
// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#include "randombytes.h"

#include "pqclean-impl/params.h"
#include "pqclean-impl/common.h"
#include "../common.h"

int main(void)
{
    uint16_t out_pqclean_n_by_nbar[PARAMS_N * PARAMS_NBAR];
    uint16_t out_jasmin_n_by_nbar[PARAMS_N * PARAMS_NBAR];

    uint16_t out_pqclean_nbar_by_nbar[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t out_jasmin_nbar_by_nbar[PARAMS_NBAR * PARAMS_NBAR];

    randombytes((uint8_t*)out_pqclean_n_by_nbar, PARAMS_N * PARAMS_NBAR * sizeof(out_pqclean_n_by_nbar[0]));
    memcpy(out_jasmin_n_by_nbar, out_pqclean_n_by_nbar, PARAMS_N * PARAMS_NBAR * sizeof(out_pqclean_n_by_nbar[0]));

    // Test sample_N_by_NBAR
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_sample_n(out_pqclean_n_by_nbar, PARAMS_N * PARAMS_NBAR);
    jazz_sample_N_by_NBAR(out_jasmin_n_by_nbar, PARAMS_N * PARAMS_NBAR);
    for(size_t i = 0; i < PARAMS_N * PARAMS_NBAR; i++)
    {
        assert(out_pqclean_n_by_nbar[i] == out_jasmin_n_by_nbar[i]);
    }

    // Test sample_NBAR_by_NBAR
    randombytes((uint8_t*)out_pqclean_nbar_by_nbar, PARAMS_NBAR * PARAMS_NBAR * sizeof(out_pqclean_nbar_by_nbar[0]));
    memcpy(out_jasmin_nbar_by_nbar, out_pqclean_nbar_by_nbar, PARAMS_NBAR * PARAMS_NBAR * sizeof(out_pqclean_nbar_by_nbar[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_sample_n(out_pqclean_nbar_by_nbar, PARAMS_NBAR * PARAMS_NBAR);
    jazz_sample_NBAR_by_NBAR(out_jasmin_nbar_by_nbar, PARAMS_NBAR * PARAMS_NBAR);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        assert(out_pqclean_nbar_by_nbar[i] == out_jasmin_nbar_by_nbar[i]);
    }

    printf("Success!\n");
    return EXIT_SUCCESS;
}
