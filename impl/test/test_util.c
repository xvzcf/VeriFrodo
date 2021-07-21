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

static void test_add(void)
{
    uint16_t lhs[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t rhs[PARAMS_NBAR * PARAMS_NBAR];

    uint16_t out_pqclean[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t out_jasmin[PARAMS_NBAR * PARAMS_NBAR];

    randombytes((uint8_t*)lhs, (PARAMS_NBAR * PARAMS_NBAR) * sizeof(lhs[0]));
    randombytes((uint8_t*)rhs, (PARAMS_NBAR * PARAMS_NBAR) * sizeof(rhs[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_add(out_pqclean, lhs, rhs);
    jazz_add(out_jasmin, lhs, rhs);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }
}
static void test_sub(void)
{
    uint16_t lhs[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t rhs[PARAMS_NBAR * PARAMS_NBAR];

    uint16_t out_pqclean[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t out_jasmin[PARAMS_NBAR * PARAMS_NBAR];

    randombytes((uint8_t*)lhs, (PARAMS_NBAR * PARAMS_NBAR) * sizeof(lhs[0]));
    randombytes((uint8_t*)rhs, (PARAMS_NBAR * PARAMS_NBAR) * sizeof(rhs[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_sub(out_pqclean, lhs, rhs);
    jazz_sub(out_jasmin, lhs, rhs);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }
}
static void test_mul_bs(void)
{
    uint16_t b[PARAMS_NBAR * PARAMS_N];
    uint16_t s[PARAMS_N * PARAMS_NBAR];

    uint16_t out_pqclean[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t out_jasmin[PARAMS_NBAR * PARAMS_NBAR];

    randombytes((uint8_t*)b, (PARAMS_NBAR * PARAMS_N) * sizeof(b[0]));
    randombytes((uint8_t*)s, (PARAMS_N * PARAMS_NBAR) * sizeof(s[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_bs(out_pqclean, b, s);
    jazz_mul_bs(out_jasmin, b, s);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }
}
static void test_mul_add_sb_plus_e(void)
{
    uint16_t b[PARAMS_NBAR * PARAMS_N];
    uint16_t s[PARAMS_N * PARAMS_NBAR];
    uint16_t e[PARAMS_NBAR * PARAMS_NBAR];

    uint16_t out_pqclean[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t out_jasmin[PARAMS_NBAR * PARAMS_NBAR];

    randombytes((uint8_t*)b, (PARAMS_NBAR * PARAMS_N) * sizeof(b[0]));
    randombytes((uint8_t*)s, (PARAMS_N * PARAMS_NBAR) * sizeof(s[0]));
    randombytes((uint8_t*)e, (PARAMS_NBAR * PARAMS_NBAR) * sizeof(e[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_mul_add_sb_plus_e(out_pqclean, b, s, e);
    jazz_mul_add_sb_plus_e(out_jasmin, b, s, e);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }
}
static void test_pack(void)
{
    uint16_t in[PARAMS_N * PARAMS_NBAR];

    size_t pack_outlen = (PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8;
    uint8_t out_pqclean[(PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8];
    uint8_t out_jasmin[(PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8];

    randombytes((uint8_t*)in, (PARAMS_N * PARAMS_NBAR) * sizeof(in[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_pack(out_pqclean, pack_outlen, in, PARAMS_N * PARAMS_NBAR, PARAMS_LOGQ);
    jazz_pack_N_by_NBAR(out_jasmin, in);
    for(size_t i = 0; i < pack_outlen; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }
}
static void test_unpack(void)
{
    size_t inlen = (PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8;
    uint8_t in[(PARAMS_LOGQ * PARAMS_N * PARAMS_NBAR) / 8];

    uint16_t out_pqclean[PARAMS_N * PARAMS_NBAR];
    uint16_t out_jasmin[PARAMS_N * PARAMS_NBAR];

    randombytes(in, inlen * sizeof(in[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_unpack(out_pqclean, PARAMS_N * PARAMS_NBAR, in, inlen, PARAMS_LOGQ);
    jazz_unpack_to_N_by_NBAR(out_jasmin, in);
    for(size_t i = 0; i < PARAMS_N * PARAMS_NBAR; i++)
    {
        assert(out_pqclean[i] == out_jasmin[i]);
    }
}
static void test_key_encode(void)
{
    uint8_t in[BYTES_MU];

    uint16_t pqclean_out[PARAMS_NBAR * PARAMS_NBAR];
    uint16_t jasmin_out[PARAMS_NBAR * PARAMS_NBAR];

    randombytes(in, BYTES_MU * sizeof(in[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_key_encode(pqclean_out, (uint16_t*)in);
    jazz_key_encode(jasmin_out, (uint16_t*)in);
    for(size_t i = 0; i < PARAMS_NBAR * PARAMS_NBAR; i++)
    {
        assert(pqclean_out[i] == jasmin_out[i]);
    }
}
static void test_key_decode(void)
{
    uint16_t in[PARAMS_NBAR * PARAMS_NBAR];

    uint16_t pqclean_out[BYTES_MU / 2];
    uint16_t jasmin_out[BYTES_MU / 2];

    randombytes((uint8_t*)in, PARAMS_NBAR * PARAMS_NBAR * sizeof(in[0]));

    PQCLEAN_FRODOKEM640SHAKE_CLEAN_key_decode(pqclean_out, in);
    jazz_key_decode(jasmin_out, in);
    for(size_t i = 0; i < (BYTES_MU / 2); i++)
    {
        assert(pqclean_out[i] == jasmin_out[i]);
    }
}
static void test_ct_verify(void)
{
    uint16_t pqclean_in[PARAMS_N * PARAMS_NBAR];
    uint16_t jasmin_in[PARAMS_N * PARAMS_NBAR];

    // Different inputs.
    randombytes((uint8_t*)pqclean_in, PARAMS_N * PARAMS_NBAR * sizeof(pqclean_in[0]));
    randombytes((uint8_t*)jasmin_in, PARAMS_N * PARAMS_NBAR * sizeof(jasmin_in[0]));
    pqclean_in[0] = ~jasmin_in[0];

    int8_t pqclean_res = PQCLEAN_FRODOKEM640SHAKE_CLEAN_ct_verify(pqclean_in, jasmin_in, PARAMS_N * PARAMS_NBAR);
    int8_t jasmin_res = jazz_ct_verify_N_by_NBAR(pqclean_in, jasmin_in);
    assert(pqclean_res == jasmin_res);

    // Same inputs.
    memcpy(pqclean_in, jasmin_in, PARAMS_N * PARAMS_NBAR * sizeof(jasmin_in[0]));
    pqclean_res = PQCLEAN_FRODOKEM640SHAKE_CLEAN_ct_verify(pqclean_in, jasmin_in, PARAMS_N * PARAMS_NBAR);
    jasmin_res = jazz_ct_verify_N_by_NBAR(pqclean_in, jasmin_in);
    assert(pqclean_res == jasmin_res);
}
static void test_ct_select(void)
{
    uint16_t pqclean_in[PARAMS_N * PARAMS_NBAR];
    uint16_t jasmin_in[PARAMS_N * PARAMS_NBAR];

    int8_t selector;
    uint8_t pqclean_out[CRYPTO_BYTES];
    uint8_t jasmin_out[CRYPTO_BYTES];

    randombytes((uint8_t*)pqclean_in, PARAMS_N * PARAMS_NBAR * sizeof(pqclean_in[0]));
    randombytes((uint8_t*)jasmin_in, PARAMS_N * PARAMS_NBAR * sizeof(jasmin_in[0]));

    selector = 0xFF;
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_ct_select(pqclean_out, (uint8_t*)pqclean_in, (uint8_t*)jasmin_in, CRYPTO_BYTES, selector);
    jazz_ct_select_CRYPTO_BYTES(jasmin_out, (uint8_t*)pqclean_in, (uint8_t*)jasmin_in, selector);
    for(size_t i = 0; i < CRYPTO_BYTES; i++)
    {
        assert(pqclean_out[i] == jasmin_out[i]);
    }

    selector = 0;
    PQCLEAN_FRODOKEM640SHAKE_CLEAN_ct_select(pqclean_out, (uint8_t*)pqclean_in, (uint8_t*)jasmin_in, CRYPTO_BYTES, selector);
    jazz_ct_select_CRYPTO_BYTES(jasmin_out, (uint8_t*)pqclean_in, (uint8_t*)jasmin_in, selector);
    for(size_t i = 0; i < CRYPTO_BYTES; i++)
    {
        assert(pqclean_out[i] == jasmin_out[i]);
    }
}

int main(void)
{
    test_add();
    test_sub();
    test_mul_bs();
    test_mul_add_sb_plus_e();
    test_pack();
    test_unpack();
    test_key_encode();
    test_key_decode();
    test_ct_verify();
    test_ct_select();

    printf("Success!\n");
    return EXIT_SUCCESS;
}
