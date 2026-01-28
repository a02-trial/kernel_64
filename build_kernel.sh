#!/bin/sh

set -e -x
cd ${GITHUB_WORKSPACE}

# custom toolchain preparation
# export PATH="${PWD}/toolchain2/clang/bin:${PWD}/toolchain2/gcc/bin:${PATH}"

# toolchain preparation
export PATH="$(pwd)/toolchain/clang/bin:${PWD}/toolchain/gcc/bin:${PATH}"
rm -rf out
mkdir -p out

sed -i 's/msleep(ilits->rst_edge_delay);/msleep(1);/g' drivers/input/touchscreen/ili9881x/ili9881x.c
sed -i 's/msleep(10)/msleep(1)/g' drivers/input/touchscreen/ili9881x/ili9881x.c
sed -i 's/msleep(20)/msleep(1)/g' drivers/input/touchscreen/ili9881x/ili9881x.c
sed -i '/ili_ic_init();/a \    device_init_wakeup(ilits->dev, true);' drivers/input/touchscreen/ili9881x/ili9881x.c
sed -i 's/usleep_range(15000, 15000);/usleep_range(100, 100);/g' drivers/input/touchscreen/ili9881x/ili9881x.c

export ARCH=arm
export CC=clang
export HOSTCC=clang
export CROSS_COMPILE=arm-linux-androideabi-
export KCFLAGS=-w
export CONFIG_SECTION_MISMATCH_WARN_ONLY=y
export CFLAGS_WARN=-Wunused-but-set-variable
export xxx="KCFLAGS=-w CONFIG_SECTION_MISMATCH_WARN_ONLY=y CFLAGS_WARN=-Wunused-but-set-variable ARCH=arm CC=clang HOSTCC=clang CROSS_COMPILE=arm-linux-androideabi-"

make O=out $xxx a02_defconfig
make O=out $xxx -j"$(nproc --all)" 2>&1 | tee build.log
