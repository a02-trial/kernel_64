#!/bin/bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

CC_BIN="${CC:-clang}"
HOSTCC_BIN="${HOSTCC:-$CC_BIN}"

export PATH="$ROOT_DIR/toolchain/clang2/bin:$ROOT_DIR/toolchain/gcc/bin:$PATH"
export ARCH=arm64
export CC="$CC_BIN"
export HOSTCC="$HOSTCC_BIN"
export CROSS_COMPILE=aarch64-linux-android-
export KCFLAGS=-w
export CONFIG_SECTION_MISMATCH_WARN_ONLY=y
export CFLAGS_WARN=-Wunused-but-set-variable
export KBUILD_BUILD_USER="${KBUILD_BUILD_USER:-codespace}"
export KBUILD_BUILD_HOST="${KBUILD_BUILD_HOST:-codespaces-6cd300}"
export KBUILD_BUILD_VERSION="${KBUILD_BUILD_VERSION:-3}"
export KBUILD_BUILD_TIMESTAMP="${KBUILD_BUILD_TIMESTAMP:-Mon Oct 19 17:24:29 UTC 2026}"

make_args=(
	ARCH=arm64
	CC="$CC_BIN"
	HOSTCC="$HOSTCC_BIN"
	CROSS_COMPILE=aarch64-linux-android-
	KCFLAGS=-w
	CONFIG_SECTION_MISMATCH_WARN_ONLY=y
	CFLAGS_WARN=-Wunused-but-set-variable
)

rm -rf out
mkdir -p out

make O=out "${make_args[@]}" a02_64_defconfig
make O=out "${make_args[@]}" -j"$(nproc --all)" Image.gz-dtb dtbs 2>&1 | tee build.log
