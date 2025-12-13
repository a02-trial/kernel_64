#!/bin/sh

set -e -x

cd ${GITHUB_WORKSPACE}
export PATH="${PWD}/toolchain/clang/host/linux-x86/clang-r383902/bin:${PWD}/toolchain/android_prebuilts_gcc_linux-x86_arm_arm-linux-androideabi-4.9-lineage-19.1/bin:${PATH}"
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

git config user.name "github-actions[bot]"
git config user.email "actions@github.com"
git add boot.img-kernel
git commit -m "noice"
git push

