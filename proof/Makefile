# SPDX-FileCopyrightText: 2021 The VeriFrodo Authors
# SPDX-License-Identifier: MIT

EASYCRYPT ?= /Users/wxyz/.opam/easycrypt/bin/easycrypt
JASMIN_SRC ?= /Users/wxyz/Projects/jasmin
JASMINC = $(JASMIN_SRC)/compiler/jasminc

IMPL_DIR ?= $(PWD)/../impl
EXTRACTION_DIR = $(PWD)/jasmin_extracted

.DEFAULT_GOAL:=all
all: $(EXTRACTION_DIR)/kem.ec

$(EXTRACTION_DIR)/kem.ec: $(IMPL_DIR)/kem.jazz
	mkdir -p $(EXTRACTION_DIR)
	cd $(EXTRACTION_DIR) && $(JASMINC) \
							-ec jazz_crypto_kem_keypair \
							-ec jazz_crypto_kem_enc \
							-ec jazz_crypto_kem_dec \
							-oec $(notdir $@) $<

.PHONY: test
test: $(wildcard correctness/*.ec) $(EXTRACTION_DIR)/kem.ec
	$(EASYCRYPT) -I Jasmin:$(JASMIN_SRC)/eclib -I $(EXTRACTION_DIR) correctness/Util.ec
	$(EASYCRYPT) -I Jasmin:$(JASMIN_SRC)/eclib -I $(EXTRACTION_DIR) correctness/Noise.ec

.PHONY: clean
clean:
	$(RM) -rf $(EXTRACTION_DIR)
