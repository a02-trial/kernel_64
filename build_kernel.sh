#!/bin/sh

set -e -x

# custom toolchain preparation
CLANG_URL=${{ inputs.CLANG_AOSP }}
GCC_URL=${{ inputs.GCC_AOSP }}
ls
mkdir toolchain2
cd toolchain2
if [ ! -d clang ]; then mkdir clang && curl -Lsq "${CLANG_URL}" -o clang.tgz && tar -xzf clang.tgz -C clang; fi
if [ ! -d gcc ]; then mkdir gcc && curl -Lsq "${GCC_URL}" -o gcc.tgz && tar -xzf gcc.tgz -C gcc; fi
cd ${GITHUB_WORKSPACE}
export PATH="${PWD}/toolchain2/clang/bin:${PWD}/toolchain2/gcc/bin:${PATH}"

# export PATH="${PWD}/toolchain/clang/bin:${PWD}/toolchain/gcc/bin:${PATH}"
export ARCH=arm
export CC=clang
export HOSTCC=clang
export CLANG_TRIPLE=arm-linux-gnueabi-
export CROSS_COMPILE=arm-linux-androideabi-
export KCFLAGS=-w
export CONFIG_SECTION_MISMATCH_WARN_ONLY=y
rm -rf out
mkdir -p out
make O=out KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y ARCH=arm CC=clang HOSTCC=clang CLANG_TRIPLE=arm-linux-gnueabi- CROSS_COMPILE=arm-linux-androidkernel- clean
make O=out KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y ARCH=arm CC=clang HOSTCC=clang CLANG_TRIPLE=arm-linux-gnueabi- CROSS_COMPILE=arm-linux-androidkernel- mrproper
make O=out KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y ARCH=arm CC=clang HOSTCC=clang CLANG_TRIPLE=arm-linux-gnueabi- CROSS_COMPILE=arm-linux-androidkernel- a02_defconfig
make O=out KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y ARCH=arm CC=clang HOSTCC=clang CLANG_TRIPLE=arm-linux-gnueabi- CROSS_COMPILE=arm-linux-androidkernel- -j16

cp out/arch/arm/boot/zImage ${PWD}/zImage
mv zImage boot.img-kernel

# git config user.name "rdbckp"
# git config user.email "ardibackup@gmail.com"
# git add boot.img-kernel
# git commit -m "kernel ready"
# git push
