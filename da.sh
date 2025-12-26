#!/bin/sh

set -e -x
cd ${GITHUB_WORKSPACE}

# toolchain preparation
export PATH="${PWD}/toolchain/clang/bin:${PWD}/toolchain/gcc/bin:${PATH}"

export ARCH=arm
export CC=clang
export HOSTCC=clang
export CROSS_COMPILE=arm-linux-androideabi-
export KCFLAGS=-w
export CONFIG_SECTION_MISMATCH_WARN_ONLY=y
export CFLAGS_WARN=-Wunused-but-set-variable
export xxx="KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y CFLAGS_WARN=-Wunused-but-set-variable ARCH=arm CC=clang HOSTCC=clang CROSS_COMPILE=arm-linux-androideabi-"

cp arch/arm/configs/a02_defconfig drivers/input/touchscreen/.config
make O=drivers/input/touchscreen $xxx olddefconfig
make O=drivers/input/touchscreen $xxx prepare
make O=drivers/input/touchscreen $xxx modules
