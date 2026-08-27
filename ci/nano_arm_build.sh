#!/usr/bin/env bash
# Cross-compile libDaScriptNano and every standalone example for a cortex-m4,
# and print the size of what comes out.
#
# nano's drift tripwire: nothing else in CI builds freestanding, so a change that
# pulls the compiler, fmt, the host's I/O or a thread primitive back into the
# minimal runtime fails here first and nowhere else.
#
# Build them ALL, not just the first one. The examples differ in what they reach
# for - the heap, closures, a C binding, an array index - and a type that is
# spelled two ways on this target (int32_t is long int here, not int) only
# becomes ambiguous at the site that uses it. A gate that builds one example
# passes while the others are broken.
#
# Usage: ci/nano_arm_build.sh <path-to-daslang> [out-dir]
# Toolchain: apt-get install gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
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

# and examples/standalone/CMakeLists.txt is the one example list, for the same reason.
# tr -d: a Windows checkout hands this script CRLF lines, and a field at end of
# line would otherwise carry the CR into a path
examples() {
    tr -d '\r' < examples/standalone/CMakeLists.txt \
        | grep -o '^das_nano_example([^)]*' \
        | sed -e 's/^das_nano_example(//' \
        | awk '{ print $1 " " $2 " " $3 }'
}

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

NANO_OBJS=()
echo "== compiling nano"
for src in $(sources); do
    obj="$OUT/$(echo "$src" | tr '/' '_').o"
    "$CXX" "${ARM_FLAGS[@]}" -c "$src" -o "$obj"
    NANO_OBJS+=("$obj")
done

SUMMARY=()
while read -r name dir das_file; do
    [ -n "$name" ] || continue
    echo
    echo "== $name ($dir/$das_file)"
    # </dev/null: the loop is fed by a process substitution, and a child that
    # reads stdin would swallow the examples still queued behind this one
    # -cross_platform: the host is 64-bit and the target is not, so anything the
    # emitter would otherwise pin to host layout (lambda captures hold pointers)
    # has to be left to the target compiler
    "$DASLANG" utils/aot/main.das -- -cross_platform -ctx \
        "examples/standalone/$dir/$das_file" "$OUT/generated/" </dev/null
    "$CXX" "${ARM_FLAGS[@]}" -c "$OUT/generated/$das_file.cpp" -o "$OUT/$name.gen.o"
    "$CXX" "${ARM_FLAGS[@]}" -c "examples/standalone/$dir/main.cpp" -o "$OUT/$name.main.o"
    "$CXX" "${ARM_FLAGS[@]}" --specs=nosys.specs -Wl,--gc-sections \
        "${NANO_OBJS[@]}" "$OUT/$name.gen.o" "$OUT/$name.main.o" -o "$OUT/$name.elf"
    text=$("$SIZE" "$OUT/$name.elf" | awk 'NR==2{print $1}')
    SUMMARY+=("$(printf '%-24s %8s' "$name" "$text")")
done < <(examples)

echo
echo "== .text (cortex-m4, -Os, gc-sections)"
for line in "${SUMMARY[@]}"; do
    echo "   $line"
done
