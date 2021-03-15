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
    uint16_t out_pqclean[PARAMS_N * PARAMS_NBAR];
    uint16_t out_jasmin[PARAMS_N * PARAMS_NBAR];

    randombytes((uint8_t*)out_pqclean, PARAMS_N * PARAMS_NBAR * sizeof(out_pqclean[0]));
    memcpy(out_jasmin, out_pqclean, PARAMS_N * PARAMS_NBAR * sizeof(out_pqclean[0]));

    // Test sample_n
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_sample_n(out_pqclean, PARAMS_N * PARAMS_NBAR);
    jazz_sample_N_by_NBAR(out_jasmin, PARAMS_N * PARAMS_NBAR);
    for(size_t i = 0; i < PARAMS_N * PARAMS_NBAR; i++)
    {
        assert((out_pqclean[i] ^ out_jasmin[i]) == 0);
    }

    printf("Success!\n");
    return EXIT_SUCCESS;
}
