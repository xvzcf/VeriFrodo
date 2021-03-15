# SPDX-FileCopyrightText: 2021 The VeriFrodo Authors
# SPDX-License-Identifier: MIT

JASMINC ?= /Users/wxyz/Projects/jasmin/compiler/jasminc
IMPL_DIR ?= $(PWD)/../impl
EXTRACTION_DIR = $(PWD)/jasmin_extracted

.DEFAULT_GOAL:=all
all: $(EXTRACTION_DIR)/kem.ec $(EXTRACTION_DIR)/util.ec

$(EXTRACTION_DIR)/kem.ec: $(IMPL_DIR)/kem.jo
	mkdir -p $(EXTRACTION_DIR)
	cd $(EXTRACTION_DIR) && $(JASMINC) -ec jazz_crypto_kem_keypair -oec $(notdir $@) $<

# Temporary, remove once encryption is implemented.
$(EXTRACTION_DIR)/util.ec: $(IMPL_DIR)/util.jo
	mkdir -p $(EXTRACTION_DIR)
	cd $(EXTRACTION_DIR) && $(JASMINC) -ec add -oec $(notdir $@) $<

$(IMPL_DIR)/%.jo:
	$(MAKE) -C $(IMPL_DIR) $(notdir $@)

.PHONY: clean
clean:
	$(RM) -rf $(EXTRACTION_DIR)
