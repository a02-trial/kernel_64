#!/bin/sh

set -e -x

# custom toolchain preparation
cd ${GITHUB_WORKSPACE}
export PATH="${PWD}/toolchain2/clang/bin:${PWD}/toolchain2/gcc/bin:${PATH}"
mkdir -p out
make O=out ARCH=arm CC=clang HOSTCC=clang CROSS_COMPILE=arm-linux-androideabi- clean
make O=out ARCH=arm CC=clang HOSTCC=clang CROSS_COMPILE=arm-linux-androideabi- mrproper
make O=out ARCH=arm CC=clang HOSTCC=clang CROSS_COMPILE=arm-linux-androideabi- a02_defconfig
make O=out ARCH=arm CC=clang HOSTCC=clang CROSS_COMPILE=arm-linux-androideabi- -j16

cp out/arch/arm/boot/zImage ${PWD}/zImage
mv zImage boot.img-kernel

# git config user.name "rdbckp"
# git config user.email "ardibackup@gmail.com"
# git add boot.img-kernel
# git commit -m "kernel ready"
# git push
