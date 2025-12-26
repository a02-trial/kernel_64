#!/bin/sh

set -e -x
cd ${GITHUB_WORKSPACE}

# toolchain preparation
export PATH="${PWD}/toolchain/clang/bin:${PWD}/toolchain/gcc/bin:${PATH}"
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

make $xxx clean && make $xxx mrproper
cp arch/arm/configs/a02_defconfig out
make O=out $xxx olddefconfig
make O=out $xxx prepare
make O=out $xxx modules
