# SPDX-FileCopyrightText: 2021 The VeriFrodo Authors
# SPDX-License-Identifier: MIT

JASMINC ?= /Users/wxyz/Projects/jasmin/compiler/jasminc
JASMINFLAGS = -lea

PQCLEAN_IMPL = test/pqclean-impl

TEST_OBJECTS = test_util.o test_noise.o test_matrix_shake.o test_kem.o
JASMIN_OBJECTS = util.o noise.o matrix_shake.o kem.o

TEST_CFLAGS = -Wall -Wextra

.DEFAULT_GOAL:=all
all: $(TEST_OBJECTS) $(JASMIN_OBJECTS)

# Recipes for building the test programs.
test_util.o: test/test_util.c $(PQCLEAN_IMPL)/util.o util.o test/randombytes.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

test_noise.o: test/test_noise.c $(PQCLEAN_IMPL)/noise.o noise.o test/randombytes.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

test_matrix_shake.o: test/test_matrix_shake.c $(PQCLEAN_IMPL)/matrix_shake.o $(PQCLEAN_IMPL)/fips202.o $(PQCLEAN_IMPL)/util.o matrix_shake.o test/randombytes.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

test_kem.o: test/test_kem.c $(PQCLEAN_IMPL)/kem.o $(PQCLEAN_IMPL)/matrix_shake.o $(PQCLEAN_IMPL)/fips202.o $(PQCLEAN_IMPL)/noise.o $(PQCLEAN_IMPL)/util.o kem.o test/randombytes.c
	$(CC) $(TEST_CFLAGS) $^ -o $@

# Recipes for building the Jasmin code.
%.o: %.S
	$(CC) $(CFLAGS) $< -c -o $@

%.S: %.jo
	$(JASMINC) $(JASMINFLAGS) $< -o $@

kem.jo: kem.jazz shake128.jahh util.jahh params.jahh noise.jahh matrix_shake.jahh
	cat $< | $(CC) -E - | grep -v ^# > $@
	sed -e "s/@/#/g" $@ > $@.tmp
	mv $@.tmp $@

util.jo: util.jazz params.jahh util.jahh
	cat $< | $(CC) -E - | grep -v ^# > $@

noise.jo: noise.jazz params.jahh noise.jahh
	cat $< | $(CC) -E - | grep -v ^# > $@

matrix_shake.jo: matrix_shake.jazz params.jahh shake128.jahh matrix_shake.jahh
	cat $< | $(CC) -E - | grep -v ^# > $@
	sed -e "s/@/#/g" $@ > $@.tmp
	mv $@.tmp $@

# Recipe for building the PQClean code used for testing
$(PQCLEAN_IMPL)/%.o:
	$(MAKE) -C $(PQCLEAN_IMPL)

.PHONY: test
test: $(TEST_OBJECTS)
	for f in $^; do ./$$f; done

.PHONY: clean
clean:
	$(RM) -f $(TEST_OBJECTS)
	$(RM) -f $(JASMIN_OBJECTS)
	$(RM) *.S *.jo
	$(MAKE) -C $(PQCLEAN_IMPL) clean
