# SPDX-FileCopyrightText: 2021 The VeriFrodo Authors
# SPDX-License-Identifier: MIT

FROM nixos/nix:2.3.10

ARG JASMIN_COMMIT=762709d12668f02c2bf064e1f7755558dae6d672
ARG EASYCRYPT_COMMIT=ba45bb574b848c6e12a153b2bd7c6a6754a44fba

ARG NIX_CHANNEL=https://github.com/NixOS/nixpkgs/archive/711ad46bd6133c749931d2b77f3345f1c05d189f.tar.gz
ENV NIX_PATH=""

RUN nix-env --file ${NIX_CHANNEL} \
            --install --attr git && \
    git clone \
        --branch glob_array3 https://github.com/jasmin-lang/jasmin.git && \
    git clone https://github.com/EasyCrypt/easycrypt.git

WORKDIR /jasmin
RUN git checkout ${JASMIN_COMMIT} && \
    nix-shell \
        --include nixpkgs=${NIX_CHANNEL} \
        --run 'make -C compiler CIL && \
               make -C compiler && \
               make -C compiler check'

WORKDIR /easycrypt
COPY check-easycrypt-install.ec /easycrypt/check-easycrypt-install.ec
RUN git checkout ${EASYCRYPT_COMMIT} && \
    nix-env \
        --file ${NIX_CHANNEL} \
        --install --attr z3 && \
    env NIXPKGS_ALLOW_UNFREE=1 nix-env \
        --file ${NIX_CHANNEL} \
        --install --attr alt-ergo && \
    nix-shell \
        --include nixpkgs=${NIX_CHANNEL} \
        --run 'make -j$NIX_BUILD_CORES && \
               why3 config detect' && \
    ./ec.native check-easycrypt-install.ec

COPY impl /impl
WORKDIR /impl
RUN nix-shell \
        --include nixpkgs=${NIX_CHANNEL} \
        --packages clang gnumake binutils-unwrapped \
        --run 'make clean && \
               make -e JASMINC=/jasmin/compiler/jasminc && \
               make test'

COPY proof /proof
WORKDIR /proof
RUN nix-shell \
        --include nixpkgs=${NIX_CHANNEL} \
        --packages gnumake \
        --run 'make \
                   -e JASMINC=/jasmin/compiler/jasminc \
                   -e IMPL_DIR=/impl && \
               /easycrypt/ec.native \
                   -I Jasmin:/jasmin/eclib \
                   -I /proof/jasmin_extracted \
                   correctness/frodo_util.ec'
