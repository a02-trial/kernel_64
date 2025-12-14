#!/bin/sh

set -e -x

cd ${GITHUB_WORKSPACE}
export PATH="${PWD}/toolchain/clang/bin:${PWD}/toolchain/gcc/bin:${PATH}"
export ARCH=arm
export CC=clang
export HOSTCC=clang
export CLANG_TRIPLE=arm-linux-gnueabi-
export CROSS_COMPILE=arm-linux-androideabi-
export KCFLAGS=-w
export CONFIG_SECTION_MISMATCH_WARN_ONLY=y
curl -LSs "https://raw.githubusercontent.com/tiann/KernelSU/main/kernel/setup.sh" | bash -s v0.9.5
KSU_VERSION=$(cd KernelSU && expr $(/usr/bin/git rev-list --count HEAD) + 10200)
echo "UPLOADNAME=-KernelSU_$KSU_VERSION" >> $GITHUB_ENV
rm -rf out
mkdir -p out
make O=out KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y ARCH=arm CC=clang HOSTCC=clang CLANG_TRIPLE=arm-linux-gnueabi- CROSS_COMPILE=arm-linux-androidkernel- clean
make O=out KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y ARCH=arm CC=clang HOSTCC=clang CLANG_TRIPLE=arm-linux-gnueabi- CROSS_COMPILE=arm-linux-androidkernel- mrproper
make O=out KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y ARCH=arm CC=clang HOSTCC=clang CLANG_TRIPLE=arm-linux-gnueabi- CROSS_COMPILE=arm-linux-androidkernel- a02_defconfig
make O=out KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y ARCH=arm CC=clang HOSTCC=clang CLANG_TRIPLE=arm-linux-gnueabi- CROSS_COMPILE=arm-linux-androidkernel- -j16

cp out/arch/arm/boot/zImage ${PWD}/zImage
# mv zImage boot.img-kernel

# git config user.name "rdbckp"
# git config user.email "ardibackup@gmail.com"
# git add boot.img-kernel
# git commit -m "success"
# git push

