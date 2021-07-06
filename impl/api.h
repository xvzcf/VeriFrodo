// SPDX-FileCopyrightText: 2021 The VeriFrodo Authors
// SPDX-License-Identifier: MIT

#ifndef JASMIN_FRODOKEM640SHAKE_REF_API_H
#define JASMIN_FRODOKEM640SHAKE_REF_API_H

#include <stdint.h>

void jazz_crypto_kem_keypair(uint8_t *pk, uint8_t *sk, uint8_t *randomness);
void jazz_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk, uint8_t *randomness);
void jazz_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk);

#endif
