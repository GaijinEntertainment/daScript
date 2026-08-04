#!/bin/sh
# Build the daslang host used to CROSS-COMPILE .das to wasm64.
#
# This is not an ordinary daslang build. `daspkg build --wasm` / `release wasm`
# run this binary to JIT-codegen the sample, and it bakes C++ handled-type
# layouts (Context, jobque, every std::string-bearing struct) from ITS OWN ABI
# into the wasm module. The emscripten target is libc++ with C++ exceptions, so
# a host built against libstdc++ with setjmp exceptions bakes the WRONG offsets
# and sizes -- std::string 32 vs 24 bytes, mutex 80 vs 40 -- and the wasm heap
# corrupts at runtime, with no diagnostic at build time. Host stdlib and
# exception config must MATCH the target; host pointer width must match target
# pointer width (wasm64 => 64-bit host).
#
# Authority for these flags is .github/workflows/pages.yml, step
# "Build: Daslang host (+ glfw/opengl shared modules)". Keep the two in
# lockstep: if that step changes, change this script in the same commit.
#
# web/CMakeLists.txt only WARNS when the host compiler is not clang ("degraded
# but builds"). This script refuses instead -- a silent degradation here is the
# failure mode that reaches production as corrupted wasm.
#
# The built binary lands in the TREE's bin/daslang (not in the build dir) --
# which is the same path an ordinary native build of the same tree writes. Give
# the wasm host its own worktree, or a later native build silently replaces the
# libc++ host with a libstdc++ one and the corruption returns with no signal.
#
# Usage:
#   web/build_wasm_host.sh [build-dir]
#
# Environment:
#   HOST_CC / HOST_CXX   compiler to use (default: clang / clang++)
#                        Debian 12 note: /usr/bin/clang is clang-14 there and
#                        cannot build this tree -- pass clang-19 explicitly:
#                          HOST_CC=clang-19 HOST_CXX=clang++-19 web/build_wasm_host.sh
#   JOBS                 parallel build jobs (default: cmake's own choice)
set -eu

BUILD_DIR="${1:-build-wasm-host}"
HOST_CC="${HOST_CC:-clang}"
HOST_CXX="${HOST_CXX:-clang++}"

REPO=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$REPO"

# --- Gate 1: the compiler really is clang -------------------------------------
if ! "$HOST_CXX" --version 2>/dev/null | grep -qi clang; then
    echo "build_wasm_host: $HOST_CXX is not clang." >&2
    echo "  A non-clang host cannot be given a libc++ + C++-exceptions config that" >&2
    echo "  matches the emscripten target, and web/CMakeLists.txt would only warn." >&2
    echo "  Set HOST_CC/HOST_CXX to a clang that can build this tree." >&2
    exit 1
fi

# --- Gate 2: libc++ is actually usable, not merely requested ------------------
# -stdlib=libc++ is accepted by the driver even with no libc++ installed; the
# failure then surfaces hundreds of TUs later as a missing <__config>. Probe now.
PROBE=$(mktemp -d)
trap 'rm -rf "$PROBE"' EXIT
cat > "$PROBE/probe.cpp" <<'PROBE_EOF'
#include <string>
#include <mutex>
#ifndef _LIBCPP_VERSION
#error "not libc++"
#endif
int main() { return (int)(sizeof(std::string) + sizeof(std::mutex)); }
PROBE_EOF
if ! "$HOST_CXX" -stdlib=libc++ -DDAS_ENABLE_EXCEPTIONS=1 "$PROBE/probe.cpp" -o "$PROBE/probe" 2>"$PROBE/err"; then
    echo "build_wasm_host: $HOST_CXX cannot compile against libc++." >&2
    echo "  Install it (Debian/Ubuntu, matching the compiler's major version), e.g." >&2
    echo "    sudo apt install libc++-19-dev libc++abi-19-dev" >&2
    echo "  Compiler output:" >&2
    sed 's/^/    /' "$PROBE/err" >&2
    exit 1
fi

echo "build_wasm_host: $($HOST_CXX --version | head -1)"
echo "build_wasm_host: libc++ OK; configuring $BUILD_DIR"

# --- Configure ----------------------------------------------------------------
# Verbatim from pages.yml's host step. NB: a `#` comment inside the `\`-continued
# cmake call terminates it (exit 127) -- keep comments outside the call.
cmake --no-warn-unused-cli -B"./$BUILD_DIR" \
  -DCMAKE_BUILD_TYPE:STRING=Release \
  -DCMAKE_C_COMPILER="$HOST_CC" -DCMAKE_CXX_COMPILER="$HOST_CXX" \
  -DCMAKE_C_FLAGS="-DDAS_ENABLE_EXCEPTIONS=1" \
  -DCMAKE_CXX_FLAGS="-stdlib=libc++ -DDAS_ENABLE_EXCEPTIONS=1 -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS" \
  -DCMAKE_EXE_LINKER_FLAGS="-stdlib=libc++" \
  -DCMAKE_SHARED_LINKER_FLAGS="-stdlib=libc++" \
  -DDAS_HV_DISABLED=OFF \
  -DDAS_PUGIXML_DISABLED=OFF \
  -DDAS_LLVM_DISABLED=OFF \
  -DDAS_GLFW_DISABLED=OFF \
  -G Ninja

# dasLLVM is what makes this a cross-compile host at all; dasGlfw/dasOpenGL are
# the shared modules the example games link against.
if [ -n "${JOBS:-}" ]; then
    cmake --build "./$BUILD_DIR" --target daslang dasModuleGlfw dasModuleOpenGL -j "$JOBS"
else
    cmake --build "./$BUILD_DIR" --target daslang dasModuleGlfw dasModuleOpenGL
fi

HOST_BIN="$REPO/bin/daslang"
echo "build_wasm_host: host daslang at $HOST_BIN"
echo "build_wasm_host: pass -DDAS_HOST_DASLANG_OVERRIDE=$HOST_BIN when configuring web/build64"
if ldd "$HOST_BIN" 2>/dev/null | grep -q 'libstdc++'; then
    echo "build_wasm_host: WARNING - the host links libstdc++, not libc++; handled-type layouts will not match the wasm target." >&2
fi
