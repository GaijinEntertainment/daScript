#!/usr/bin/env bash
# Cross-compile libDaScriptNano and the tier-A standalone example for a
# cortex-m4, and print the size of what comes out.
#
# nano's drift tripwire: nothing else in CI builds freestanding, so a change that
# pulls the compiler, fmt, the host's I/O or a thread primitive back into the
# minimal runtime fails here first and nowhere else.
#
# Usage: ci/nano_arm_build.sh <path-to-daslang> [out-dir]
# Toolchain: apt-get install gcc-arm-none-eabi
set -euo pipefail

DASLANG="${1:?usage: ci/nano_arm_build.sh <path-to-daslang> [out-dir]}"
OUT="${2:-build/nano-arm}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CXX="${ARM_CXX:-arm-none-eabi-g++}"
SIZE="${ARM_SIZE:-arm-none-eabi-size}"

cd "$ROOT"
mkdir -p "$OUT/generated"

# nano/CMakeLists.txt is the one source list. Reading it here keeps this script
# from becoming a second one that drifts. The two variables it writes paths
# against mean different roots: nano's own sources sit under nano/, the shared
# ones under the repo root.
sources() {
    sed -n '/^set(NANO_OWN_SRC/,/^)/p;/^set(NANO_SHARED_SRC/,/^)/p' nano/CMakeLists.txt \
        | grep -o '\${[A-Z_]*}/[^ ]*\.cpp' \
        | sed -e 's|^\${CMAKE_CURRENT_SOURCE_DIR}/|nano/|' \
              -e 's|^\${DASLANG_NANO_ROOT}/||'
}

echo "== generating standalone C++ for examples/standalone/01_pure"
"$DASLANG" utils/aot/main.das -- -ctx \
    examples/standalone/01_pure/pure_math.das "$OUT/generated/"

# -Os and no exceptions/RTTI is what an embedded target actually builds with;
# nano's das_config.h already forces DAS_ENABLE_EXCEPTIONS off, and this proves
# the compiler agrees.
ARM_FLAGS=(
    -mcpu=cortex-m4 -mthumb -mfloat-abi=soft
    -Os -ffunction-sections -fdata-sections
    -fno-exceptions -fno-rtti -fno-threadsafe-statics
    -std=c++17
    -I nano/include -I include -I "$OUT/generated"
)

OBJS=()
echo "== compiling nano"
for src in $(sources); do
    obj="$OUT/$(echo "$src" | tr '/' '_').o"
    "$CXX" "${ARM_FLAGS[@]}" -c "$src" -o "$obj"
    OBJS+=("$obj")
done

echo "== compiling the generated context and the example"
"$CXX" "${ARM_FLAGS[@]}" -c "$OUT/generated/pure_math.das.cpp" -o "$OUT/pure_math.o"
"$CXX" "${ARM_FLAGS[@]}" -c examples/standalone/01_pure/main.cpp -o "$OUT/main.o"
OBJS+=("$OUT/pure_math.o" "$OUT/main.o")

echo "== linking"
"$CXX" "${ARM_FLAGS[@]}" --specs=nosys.specs -Wl,--gc-sections \
    "${OBJS[@]}" -o "$OUT/nano_01_pure.elf"

echo "== size (cortex-m4, -Os, gc-sections)"
"$SIZE" "$OUT/nano_01_pure.elf"
