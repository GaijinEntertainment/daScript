#!/usr/bin/env bash
# Build daslang with the dagor-flavored config (cmake/das_config_eastl):
# dagor's EASTL fork, dag::Vector as das::vector, ska::flat_hash_map as das_map.
# Used by .github/workflows/build_eastl.yml and runnable locally:
#
#   ci/build_eastl_config.sh [pc|no_fileio]
#
#   DAGOR_ROOT   existing DagorEngine checkout; without it a sparse one is made
#                into .dagor_headers (only the four dirs the config needs)
#   BUILD_DIR    build dir (default build_eastl_<variant>)
#   EXTRA_FLAGS  extra compiler flags
#
# pc        - full compiler binary, fileio on
# no_fileio - _TARGET_PC=0 turns on DAS_NO_FILEIO (fio entry points become throwing
#             stubs) and DAS_TARGET_PS=1 makes das_getenv answer nullptr. Static lib
#             only, the way a console build consumes daScript. Host toolchain: this
#             proves the source compiles and links with no filesystem and no
#             environment, it is not a console build.
set -euo pipefail

VARIANT=${1:-pc}
DAS_ROOT=$(cd "$(dirname "$0")/.." && pwd)
BUILD_DIR=${BUILD_DIR:-$DAS_ROOT/build_eastl_$VARIANT}
# _TARGET_PC is per-variant, set below.
GCC_FLAGS=${GCC_FLAGS:--O3 -std=c++20 -m64 -msse4 -mno-recip -minline-all-stringops -pipe \
 -ffast-math -ffinite-math-only -fexceptions -fno-rtti -fno-omit-frame-pointer -fconserve-space \
 -fdata-sections -ffunction-sections -fstack-protector -fvisibility-inlines-hidden \
 -DDAGOR_DBGLEVEL=1 -DDAGOR_EXCEPTIONS_ENABLED -DDAS_ENABLE_KEEPALIVE=0 -DFMT_UNICODE=0 \
 -D_TARGET_64BIT -D_TARGET_ARCH_X86_64 -D_TARGET_SIMD_SSE=4 -D_TARGET_STATIC_LIB \
 -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -D__STDC_CONSTANT_MACROS -D__LINUX__=64}

# -Wno-maybe-uninitialized is what makes gcc report the definite form instead of a
# "maybe", so an uninitialized read through a punned vec4f fails here. The
# mismatched-dealloc/new-delete pair is a lane artifact: dag::Vector reallocs what
# eastl::allocator got from operator new, where dagor routes both through its own
# allocator.
WARN_FLAGS=${WARN_FLAGS:--Werror=uninitialized -Wuninitialized -Wno-maybe-uninitialized \
 -Wno-use-after-free -Wno-mismatched-dealloc -Wno-mismatched-new-delete -Wno-deprecated -Wno-deprecated-declarations -Wno-invalid-offsetof \
 -Wno-ignored-attributes -Wno-stringop-overflow -Wno-stringop-overread -Wno-format-truncation \
 -Wno-format-extra-args -Wno-multichar -Wno-nonnull -Wno-trigraphs}
EXTRA_FLAGS="${EXTRA_FLAGS:-} $GCC_FLAGS $WARN_FLAGS"
# gcc, not clang: clang's uninitialized analysis reports nothing on these shapes
# (-Weverything included).
CC=${CC:-gcc}
CXX=${CXX:-g++}

case $VARIANT in
  pc)        TARGET=daslang_static
             EXTRA_FLAGS="-D_TARGET_PC=3 -D_TARGET_PC_LINUX=3 $EXTRA_FLAGS" ;;
  # no_libc shadows <csignal>, which the PS SDK does not ship (it has <signal.h>
  # only), so an unguarded include fails here rather than in the console lane.
  no_fileio) TARGET=libDaScript
             EXTRA_FLAGS="-D_TARGET_PC=0 -DDAS_TARGET_PS=1 -I$DAS_ROOT/cmake/das_config_eastl/no_libc $EXTRA_FLAGS" ;;
  *)         echo "unknown variant: $VARIANT (expected pc or no_fileio)" >&2 ; exit 2 ;;
esac

if [ -z "${DAGOR_ROOT:-}" ]; then
  DAGOR_ROOT=$DAS_ROOT/.dagor_headers
  if [ ! -d "$DAGOR_ROOT/.git" ]; then
    git clone --depth 1 --filter=blob:none --sparse \
      https://github.com/GaijinEntertainment/DagorEngine.git "$DAGOR_ROOT"
    git -C "$DAGOR_ROOT" sparse-checkout set \
      prog/1stPartyLibs/dag prog/3rdPartyLibs/eastl prog/3rdPartyLibs/ska_hash_map prog/dagorInclude/util
  fi
fi

# Stage only the include roots the config asks for. Symlinking whole dagor
# include trees would let dagor's own daScript/ and vecmath/ headers shadow ours.
STAGE=$BUILD_DIR/dagor_include
rm -rf "$STAGE" && mkdir -p "$STAGE"
ln -s "$DAGOR_ROOT/prog/1stPartyLibs/dag" "$STAGE/dag"
ln -s "$DAGOR_ROOT/prog/3rdPartyLibs/ska_hash_map" "$STAGE/ska_hash_map"
ln -s "$DAGOR_ROOT/prog/dagorInclude/util" "$STAGE/util"

EASTL_DIR=$DAGOR_ROOT/prog/3rdPartyLibs/eastl
EASTL_INC=$EASTL_DIR/include

# EASTL is not header-only; dagor's fork ships its own operator new[] glue
# (mem.cpp) and EA::StdC::Vsnprintf (snprintf.cpp), so no extra glue is needed.
EASTL_LIB=$BUILD_DIR/libeastl.a
if [ ! -f "$EASTL_LIB" ]; then
  mkdir -p "$BUILD_DIR/eastl_obj"
  for f in "$EASTL_DIR"/source/*.cpp "$EASTL_DIR"/mem.cpp "$EASTL_DIR"/snprintf.cpp; do
    $CXX -O2 -fPIC -std=c++17 -isystem "$EASTL_INC" -c "$f" -o "$BUILD_DIR/eastl_obj/$(basename "$f").o"
  done
  ar rcs "$EASTL_LIB" "$BUILD_DIR"/eastl_obj/*.o
fi

CC=$CC CXX=$CXX cmake -S "$DAS_ROOT" -B "$BUILD_DIR" -G Ninja \
  -DDAS_LLVM_DISABLED=ON -DDAS_GLFW_DISABLED=ON -DDAS_HV_DISABLED=ON \
  -DDAS_TESTS_DISABLED=ON -DDAS_TUTORIAL_DISABLED=ON \
  -DDAS_AOT_EXAMPLES_DISABLED=ON \
  -DDAS_STRICT_ALIASING=ON \
  -DDAS_CONFIG_INCLUDE_DIR="$DAS_ROOT/cmake/das_config_eastl" \
  -DCMAKE_CXX_FLAGS="-isystem $EASTL_INC -isystem $STAGE $EXTRA_FLAGS" \
  -DCMAKE_CXX_STANDARD_LIBRARIES="$EASTL_LIB"

# The pc variant builds daslang_static, not daslang: dagor links daScript as a
# static lib, and the shared-module build breaks ska's empty_default_table()
# sentinel (a hidden-visibility function-local static gets one copy per module,
# so a table allocated in one and freed in another frees static data).
cmake --build "$BUILD_DIR" --target "$TARGET" --parallel
