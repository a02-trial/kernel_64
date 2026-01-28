#!/bin/sh

set -e -x
cd ${GITHUB_WORKSPACE}

# custom toolchain preparation
# export PATH="${PWD}/toolchain2/clang/bin:${PWD}/toolchain2/gcc/bin:${PATH}"

# toolchain preparation
export PATH="$(pwd)/toolchain/clang/bin:${PWD}/toolchain/gcc/bin:${PATH}"
rm -rf out
mkdir -p out

sed -i '/CONFIG_PM_SLEEP=y/a CONFIG_PM_WAKEREFS_TRACKING=y' arch/arm/configs/a02_defconfig
sed -i '/CONFIG_PM_SLEEP=y/a CONFIG_PM_AUTOSLEEP=y' arch/arm/configs/a02_defconfig
echo "# CONFIG_SPI_MTK_RUNTIME_PM is not set" >> arch/arm/configs/a02_defconfig
sed -i 's/CONFIG_HZ=100/CONFIG_HZ=300/g' arch/arm/configs/a02_defconfig 2>/dev/null || echo "CONFIG_HZ=300" >> arch/arm/configs/a02_defconfig
echo "CONFIG_PM_DEBUG=y" >> arch/arm/configs/a02_defconfig
echo "CONFIG_PM_ADVANCED_DEBUG=y" >> arch/arm/configs/a02_defconfig
echo "CONFIG_SUSPEND_SKIP_SET_WAKEUP=y" >> arch/arm/configs/a02_defconfig
sed -i 's/CONFIG_KERNEL_GZIP=y/# CONFIG_KERNEL_GZIP is not set/g' arch/arm/configs/a02_defconfig
sed -i 's/# CONFIG_KERNEL_LZMA is not set/CONFIG_KERNEL_LZMA=y/g' arch/arm/configs/a02_defconfig
BUILD_TIME=$(date +"%I_%M%p")
sed -i "s/CONFIG_LOCALVERSION=.*/CONFIG_LOCALVERSION=\"-a02-RooXie_${BUILD_TIME}\"/g" arch/arm/configs/a02_defconfig

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
