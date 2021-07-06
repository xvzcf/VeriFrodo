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

    // Test mul_add_sb_plus_e
    uint16_t e1[PARAMS_NBAR * PARAMS_NBAR];
    randombytes((uint8_t*)e1, (PARAMS_NBAR * PARAMS_NBAR) * sizeof(uint16_t));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_add_sb_plus_e(out_pqclean, b2, s1, e1);
    jazz_mul_add_sb_plus_e(out_jasmin, b2, s1, e1);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }

    // Test pack
    uint8_t pack_out_pqclean[(PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8];
    uint8_t pack_out_jazz[(PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8];
    size_t pack_outlen = (PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8;

    randombytes((uint8_t*)s1, (PARAMS_N * PARAMS_NBAR) * sizeof(uint16_t));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_pack(pack_out_pqclean, pack_outlen, s1, PARAMS_N * PARAMS_NBAR, PARAMS_LOGQ);
    jazz_pack_N_by_NBAR(pack_out_jazz, s1);
    for(size_t i = 0; i < pack_outlen; i++)
    {
        assert(pack_out_pqclean[i] == pack_out_jazz[i]);
    }

    // Test unpack
    uint16_t unpack_out_pqclean[PARAMS_N * PARAMS_NBAR];
    uint16_t unpack_out_jazz[PARAMS_N * PARAMS_NBAR];

    jazz_unpack_to_N_by_NBAR(unpack_out_jazz, pack_out_pqclean);
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_unpack(unpack_out_pqclean, PARAMS_N * PARAMS_NBAR, pack_out_pqclean, pack_outlen, PARAMS_LOGQ);
    for(size_t i = 0; i < PARAMS_N * PARAMS_NBAR; i++)
    {
        //printf("JASMIN[%zu]: %u, PQCLEAN[%zu]: %u\n", i, unpack_out_jazz[i], i, unpack_out_pqclean[i]);
        assert(unpack_out_pqclean[i] == unpack_out_jazz[i]);
    }

    randombytes(pack_out_pqclean, pack_outlen);
    randombytes(pack_out_jazz, pack_outlen);

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_unpack(unpack_out_pqclean, PARAMS_N * PARAMS_NBAR, pack_out_pqclean, pack_outlen, PARAMS_LOGQ);
    jazz_unpack_to_N_by_NBAR(unpack_out_jazz, pack_out_pqclean);
    for(size_t i = 0; i < PARAMS_N * PARAMS_NBAR; i++)
    {
        //printf("JASMIN[%zu]: %u, PQCLEAN[%zu]: %u\n", i, unpack_out_jazz[i], i, unpack_out_pqclean[i]);
        assert(unpack_out_pqclean[i] == unpack_out_jazz[i]);
    }

    // Test key_encode
    uint8_t key_enc_in[BYTES_MU];

    uint16_t pqclean_out[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t jazz_out[PARAMS_NBAR * PARAMS_NBAR];

    randombytes(key_enc_in, BYTES_MU);

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_key_encode(pqclean_out, (uint16_t*)key_enc_in);
    jazz_key_encode(jazz_out, (uint16_t*)key_enc_in);

    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        //printf("JASMIN[%zu]: %u, PQCLEAN[%zu]: %u\n", i, pqclean_out[i], i, custom_out[i]);
        assert(pqclean_out[i] == jazz_out[i]);
    }

    // Test key_decode
    uint16_t key_dec_in[PARAMS_NBAR * PARAMS_NBAR];

    uint16_t pqclean_dec_out[BYTES_MU / 2];
    uint16_t jazz_dec_out[BYTES_MU / 2];

    randombytes((uint8_t*)key_dec_in, PARAMS_NBAR * PARAMS_NBAR * sizeof(key_dec_in[0]));

    jazz_key_decode(jazz_dec_out, key_dec_in);
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_key_decode(pqclean_dec_out, key_dec_in);

    for(size_t i = 0; i < (BYTES_MU / 2); i++)
    {
        //printf("JASMIN[%zu]: %u, PQCLEAN[%zu]: %u\n", i, jazz_dec_out[i], i, pqclean_dec_out[i]);
        assert(jazz_dec_out[i] == pqclean_dec_out[i]);
    }


    // Test ct_verify
    uint16_t jazz_vf_in[PARAMS_N * PARAMS_NBAR];
    uint16_t pqclean_vf_in[PARAMS_N * PARAMS_NBAR];

    randombytes((uint8_t*)jazz_vf_in, PARAMS_N * PARAMS_NBAR * sizeof(jazz_vf_in[0]));
    randombytes((uint8_t*)pqclean_vf_in, PARAMS_N * PARAMS_NBAR * sizeof(pqclean_vf_in[0]));

    int8_t jazz_res = jazz_ct_verify_N_by_NBAR(jazz_vf_in, pqclean_vf_in);
    int8_t pqclean_res = PQCLEAN_FRODOKEM640SHAKE_CLEAN_ct_verify(jazz_vf_in, pqclean_vf_in, PARAMS_N * PARAMS_NBAR);
    assert(jazz_res == pqclean_res);

    // Test ct_select
    int8_t selector = 0xFF;
    uint8_t jazz_selected_out[CRYPTO_BYTES];
    uint8_t pqclean_selected_out[CRYPTO_BYTES];

    jazz_ct_select_CRYPTO_BYTES(jazz_selected_out, (uint8_t*)jazz_vf_in, (uint8_t*)pqclean_vf_in, selector);
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_ct_select(pqclean_selected_out, (uint8_t*)jazz_vf_in, (uint8_t*)pqclean_vf_in, CRYPTO_BYTES, selector);
    for(size_t i = 0; i < CRYPTO_BYTES; i++)
    {
        assert(jazz_selected_out[i] == pqclean_selected_out[i]);
    }

    selector = 0;
    jazz_ct_select_CRYPTO_BYTES(jazz_selected_out, (uint8_t*)jazz_vf_in, (uint8_t*)pqclean_vf_in, selector);
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_ct_select(pqclean_selected_out, (uint8_t*)jazz_vf_in, (uint8_t*)pqclean_vf_in, CRYPTO_BYTES, selector);
    for(size_t i = 0; i < CRYPTO_BYTES; i++)
    {
        assert(jazz_selected_out[i] == pqclean_selected_out[i]);
    }

    // Test ct_verify with matching inputs
    memcpy(pqclean_vf_in, jazz_vf_in, PARAMS_N * PARAMS_NBAR * sizeof(jazz_vf_in[0]));
    jazz_res = jazz_ct_verify_N_by_NBAR(jazz_vf_in, pqclean_vf_in);
    pqclean_res = PQCLEAN_FRODOKEM640SHAKE_CLEAN_ct_verify(jazz_vf_in, pqclean_vf_in, PARAMS_N * PARAMS_NBAR);
    assert(jazz_res == pqclean_res);


    printf("Success!\n");
    return EXIT_SUCCESS;
}
