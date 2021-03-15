<!-- SPDX-FileCopyrightText: 2021 The VeriFrodo Authors -->
<!-- SPDX-License-Identifier: MIT -->

[![Unitary Fund](https://img.shields.io/badge/Supported%20By-UNITARY%20FUND-brightgreen.svg?style=for-the-badge)](http://unitary.fund)

# (Work in Progress) VeriFrodo

VeriFrodo is an implementation of [FrodoKEM-640-SHAKE](https://frodokem.org/) in [Jasmin](https://github.com/jasmin-lang/jasmin/wiki/About) accompanied by correctness proofs in [EasyCrypt](https://www.easycrypt.info/trac/).

## Quickstart (using Docker)

0. Ensure your Docker installation is able to use at least 5 GB of memory.

1. To build and test the code, run:

```bash
docker build -t verifrodo-test .
```
