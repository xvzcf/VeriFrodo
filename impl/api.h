// SPDX-FileCopyrightText: 2021 The VeriFrodo Authors
// SPDX-License-Identifier: MIT

#ifndef JASMIN_FRODOKEM640SHAKE_REF_API_H
#define JASMIN_FRODOKEM640SHAKE_REF_API_H

#include <stdint.h>

#define JASMIN_FRODOKEM640SHAKE_CRYPTO_SECRETKEYBYTES  19888
#define JASMIN_FRODOKEM640SHAKE_CRYPTO_PUBLICKEYBYTES   9616
#define JASMIN_FRODOKEM640SHAKE_CRYPTO_BYTES              16
#define JASMIN_FRODOKEM640SHAKE_CRYPTO_CIPHERTEXTBYTES  9720

#define JASMIN_FRODOKEM640SHAKE_CRYPTO_ALGNAME "FrodoKEM-640-SHAKE"

void jazz_crypto_kem_keypair(uint8_t* pk, uint8_t* sk, uint8_t* randomness);
void jazz_crypto_kem_enc(uint8_t* ct, uint8_t* ss, const uint8_t* pk, uint8_t* randomness);
void jazz_crypto_kem_dec(uint8_t* ss, const uint8_t* ct, const uint8_t* sk);

#endif
