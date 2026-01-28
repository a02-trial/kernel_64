#!/bin/sh

set -e -x
cd ${GITHUB_WORKSPACE}

# custom toolchain preparation
# export PATH="${PWD}/toolchain2/clang/bin:${PWD}/toolchain2/gcc/bin:${PATH}"

# toolchain preparation
export PATH="$(pwd)/toolchain/clang/bin:${PWD}/toolchain/gcc/bin:${PATH}"
rm -rf out
mkdir -p out

# =============================================================================================================================
# =============================================================================================================================
# =============================================================================================================================

# --- 1. SETTING PERFORMANCE & TOUCH ---
sed -i 's/CONFIG_HZ=100/CONFIG_HZ=300/g' arch/arm/configs/a02_defconfig
sed -i 's/CONFIG_MTK_PPM=y/CONFIG_MTK_PPM=n/' arch/arm/configs/a02_defconfig
sed -i 's/CONFIG_MTK_VCOREFS=y/CONFIG_MTK_VCOREFS=n/' arch/arm/configs/a02_defconfig
sed -i 's/CONFIG_MTK_IN_HOUSE_INPUT_BOOSTER=y/CONFIG_MTK_IN_HOUSE_INPUT_BOOSTER=n/' arch/arm/configs/a02_defconfig
sed -i 's/CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL=y/CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE=y/' arch/arm/configs/a02_defconfig
sed -i 's/CONFIG_SPI_MTK_RUNTIME_PM=y/CONFIG_SPI_MTK_RUNTIME_PM=n/' arch/arm/configs/a02_defconfig
echo "CONFIG_SUSPEND_SKIP_SET_WAKEUP=y" >> arch/arm/configs/a02_defconfig

# --- 2. SAMSUNG DEBLOAT & SECURITY ---
sed -i 's/CONFIG_TIMA=y/CONFIG_TIMA=n/' arch/arm/configs/a02_defconfig
sed -i 's/CONFIG_KNOX_KAP=y/CONFIG_KNOX_KAP=n/' arch/arm/configs/a02_defconfig
sed -i 's/CONFIG_SAMSUNG_PRODUCT_SHIP=y/CONFIG_SAMSUNG_PRODUCT_SHIP=n/' arch/arm/configs/a02_defconfig
sed -i 's/CONFIG_SEC_DEBUG=y/CONFIG_SEC_DEBUG=n/' arch/arm/configs/a02_defconfig

# --- 3. COMPRESS & VERSIONING ---
sed -i 's/CONFIG_KERNEL_GZIP=y/CONFIG_KERNEL_LZMA=y/' arch/arm/configs/a02_defconfig
grep -q "CONFIG_HAVE_KERNEL_LZMA=y" arch/arm/configs/a02_defconfig || echo "CONFIG_HAVE_KERNEL_LZMA=y" >> arch/arm/configs/a02_defconfig
BUILD_TIME=$(date +"%I_%M%p")
sed -i "s/CONFIG_LOCALVERSION=.*/CONFIG_LOCALVERSION=\"-a02-RooXie_${BUILD_TIME}\"/g" arch/arm/configs/a02_defconfig

# --- 4. CLEANING ---
sed -i '/is not set/d' arch/arm/configs/a02_defconfig
sed -i '/^#/d' arch/arm/configs/a02_defconfig
sed -i '/^$/d' arch/arm/configs/a02_defconfig

# --- 5. ADJUST DRIVER TOUCH (MTK_TPD.C) ---
# Gue udah tambahin tanda kutip penutup di akhir baris, janji gak error lagi!
sed -i '/static int tpd_suspend(struct device \*dev)/!b;n;a \    return 0;' drivers/input/touchscreen/mediatek/mtk_tpd.c
sed -i '/static int tpd_resume(struct device \*dev)/!b;n;a \    return 0;' drivers/input/touchscreen/mediatek/mtk_tpd.c
sed -i 's/create_singlethread_workqueue("mtk-tpd")/alloc_workqueue("mtk-tpd", WQ_HIGHPRI | WQ_UNBOUND, 1)/' drivers/input/touchscreen/mediatek/mtk_tpd.c

# Cek hasil suntikan touch (biar mantap)
grep -A 2 "static int tpd_resume" drivers/input/touchscreen/mediatek/mtk_tpd.c
# Cek hasil suntikan config
grep "CONFIG_LOCALVERSION" arch/arm/configs/a02_defconfig

# =============================================================================================================================
# =============================================================================================================================
# =============================================================================================================================

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
