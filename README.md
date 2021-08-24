<!-- SPDX-FileCopyrightText: 2021 The VeriFrodo Authors -->
<!-- SPDX-License-Identifier: MIT -->

[![Unitary Fund](https://img.shields.io/badge/Supported%20By-UNITARY%20FUND-brightgreen.svg?style=for-the-badge)](http://unitary.fund)

# (Work in Progress) VeriFrodo

VeriFrodo is an implementation of [FrodoKEM-640-SHAKE](https://frodokem.org/) in [Jasmin](https://github.com/jasmin-lang/jasmin/wiki/About) accompanied by correctness proofs in [EasyCrypt](https://www.easycrypt.info/trac/).

- [Overview](#overview)
- [Quickstart](#quickstart)
- [Contributing](#contributing)

## Overview

[Jasmin](https://github.com/jasmin-lang/jasmin/wiki/About) is an assembly-like programming language with a view to making programs written in it amenable to formal verification. The assembly-like syntax means programmers control many low-level details that are performance-critical, such as instruction selection and scheduling, what registers to spill and when, etc.

The Jasmin compiler is formally verified and transforms source code to x86-64 assembly. Jasmin programs can also be automatically checked for safety and termination using a bundled static analyzer.

Jasmin leverages the [EasyCrypt](https://www.easycrypt.info/trac/) proof assistant for formal verification: Jasmin source can be extracted to the corresponding EasyCrypt, thereby making it possible to prove functional correctness, cryptographic security, and security against side-channel attacks.

[FrodoKEM](https://frodokem.org/) is a post-quantum key encapsulation algorithm whose security is based on the Learning with Errors (LWE) problem.
VeriFrodo is an implementation in Jasmin of FrodoKEM-640-SHAKE, an instantiation that uses SHAKE128 and has 128-bit security. This Jasmin code is accompanied by correctness proofs in [EasyCrypt](https://www.easycrypt.info/trac/).

## Quickstart

0. Ensure your Docker installation is able to use at least 5 GB of memory.

1. To build and test the code, run:

```bash
docker build -t verifrodo-test .
```

## Contributing

Contributions are gratefully welcomed. See the list of [good first issues](https://github.com/xvzcf/VeriFrodo/issues?q=is%3Aissue+is%3Aopen+label%3A%22good+first+issue%22) for ideas.
