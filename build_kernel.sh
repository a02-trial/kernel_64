#!/bin/sh

set -e -x
cd ${GITHUB_WORKSPACE}

# custom toolchain preparation
# export PATH="${PWD}/toolchain2/clang/bin:${PWD}/toolchain2/gcc/bin:${PATH}"

# toolchain preparation
export PATH="$(pwd)/toolchain/clang/bin:${PWD}/toolchain/gcc/bin:${PATH}"
rm -rf out
mkdir -p out

export ARCH=arm
export CC=clang
export HOSTCC=clang
export CROSS_COMPILE=arm-linux-androideabi-
export KCFLAGS=-w
export CONFIG_SECTION_MISMATCH_WARN_ONLY=y
export CFLAGS_WARN=-Wunused-but-set-variable
export xxx="KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y CFLAGS_WARN=-Wunused-but-set-variable ARCH=arm CC=clang HOSTCC=clang CROSS_COMPILE=arm-linux-androideabi-"

make O=out $xxx a02_defconfig
make O=out $xxx -j16 2>&1 | tee build.log
