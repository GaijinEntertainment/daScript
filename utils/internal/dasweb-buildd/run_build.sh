#!/usr/bin/env bash
# The compute-box build recipe dasweb-buildd invokes per claimed job:
#
#   run_build.sh <src_dir> <out_dir> <mode> <entry>
#
# Contract with the service (buildd_core.run_build_command):
#   - exit 0                => success; the service then collects EXACTLY the
#                              files this mode declares, by name, and ignores
#                              anything else in <out_dir>. A build cannot widen
#                              its own output set, and must not be changed to.
#   - nonzero exit          => failure, stdout becomes the user-facing error
#   - stderr is redirected into stdout here — the service captures one stream
#
# THE SANDBOX IS THE SECURITY BOUNDARY. Compiling user .das is remote code
# execution by design: macros and [init] run at compile time, and plain `fio`
# reads files with no `unsafe` anywhere. daslang's own policies (no_unsafe,
# no_init) are defense in depth, never the boundary.
#
# The boundary is a container, and the reason is fail direction. A namespace
# built by subtracting from the host (bwrap `--ro-bind / /`) leaves a hole open
# and silent whenever a flag is forgotten. A container starts with nothing, so
# a forgotten mount fails the BUILD, loudly, on the next job. Everything the
# build may touch is one of the `-v` lines below; that list is the whole review
# surface.
#
# Mounts are identity-mapped (same path inside as out) because emsdk's config
# and the daslang host resolve paths relative to their own location. Only these
# paths exist in the container: no $HOME, no /srv, no /etc/ssh, no token.
#
# Environment (systemd unit on the box):
#   DASWEB_WASM_WORKTREE   the dedicated wasm worktree (wasm and native builds
#                          poison each other's bin/ and lib/ — see
#                          plans/dasweb_wasm_pipeline.md)
#   EMSDK                  the pinned emsdk root (5.0.7, the version pages.yml pins)
# Optional overrides: DASWEB_BUILDER_IMAGE, DASWEB_LLVM_LIB,
#   DASWEB_BUILD_MEMORY, DASWEB_BUILD_PIDS, DASWEB_BUILD_CPUS
#
# There is deliberately NO unsandboxed escape hatch. If podman or the image is
# missing, no build runs.
#
# mode=module: the per-sample recipe, verbatim from web/CMakeLists.txt
# (all_wasm) — host daslang -exe with the wasm64 jit target against the
# web/output64 runtime archive, emitting <out_dir>/sample.wasm.
# mode=page: the standalone-page recipe — daspkg release wasm, the same rail
# the five /examples cards use — for any sample whose requires pull in a native
# graphics/audio module, since GL and audio live in emscripten's JS glue that a
# bare wasi module cannot carry. The service writes the .das_package naming the
# app `sample` into SRC_DIR first; the rail nests its output under <out>/sample,
# which is flattened here INSIDE the sandbox.

set -u
exec 2>&1

SRC_DIR="${1:?usage: run_build.sh <src_dir> <out_dir> <mode> <entry>}"
OUT_DIR="${2:?missing out_dir}"
MODE="${3:?missing mode}"
ENTRY="${4:?missing entry}"

WORKTREE="${DASWEB_WASM_WORKTREE:?DASWEB_WASM_WORKTREE is not set}"
EMSDK_ROOT="${EMSDK:?EMSDK is not set (the pinned emsdk root)}"
IMAGE="${DASWEB_BUILDER_IMAGE:-dasweb-builder:2}"
LLVM_LIB="${DASWEB_LLVM_LIB:-/usr/lib/llvm-19/lib}"
BUILD_MEMORY="${DASWEB_BUILD_MEMORY:-8g}"
BUILD_PIDS="${DASWEB_BUILD_PIDS:-512}"
BUILD_CPUS="${DASWEB_BUILD_CPUS:-8}"

command -v podman >/dev/null 2>&1 || {
    echo "podman is not installed — refusing to build untrusted source"; exit 12; }
podman image exists "$IMAGE" 2>/dev/null || {
    echo "sandbox image '$IMAGE' is missing; build it with:"
    echo "  podman build -t $IMAGE -f utils/internal/dasweb-buildd/Containerfile ."
    exit 16; }

DASLANG="$WORKTREE/bin/daslang"
[ -x "$DASLANG" ] || { echo "no daslang host at $DASLANG (run web/build_wasm_host.sh first)"; exit 10; }

# The wasm64 runtime archive jit-runtime-lib links against; `daspkg build
# --wasm` stages it (with the module archives) into web/output64/lib.
RUNTIME_LIB="$(ls "$WORKTREE"/web/output64/lib/*libDaScript_runtime*.a 2>/dev/null | head -1)"
[ -n "$RUNTIME_LIB" ] || { echo "no wasm64 runtime archive under $WORKTREE/web/output64/lib (run daspkg build --wasm)"; exit 11; }

# EM_FROZEN_CACHE: the emcc cache is mounted read-only and frozen, so one job
# can never poison what later jobs link against. The toolchain-bump protocol
# warms it with one build outside the sandbox.
run_sandboxed() {
    podman run --rm \
        --network=none \
        --read-only --tmpfs /tmp:rw,size=4g \
        --memory="$BUILD_MEMORY" --memory-swap="$BUILD_MEMORY" \
        --pids-limit="$BUILD_PIDS" --cpus="$BUILD_CPUS" \
        --cap-drop=ALL --security-opt=no-new-privileges \
        --userns=keep-id \
        -e HOME=/tmp -e TMPDIR=/tmp \
        -e EMSDK="$EMSDK_ROOT" -e EM_FROZEN_CACHE=1 \
        -e LD_LIBRARY_PATH="$LLVM_LIB:$WORKTREE/lib" \
        -v "$WORKTREE:$WORKTREE:ro" \
        -v "$EMSDK_ROOT:$EMSDK_ROOT:ro" \
        -v "$LLVM_LIB:$LLVM_LIB:ro" \
        -v "$SRC_DIR:$SRC_DIR:ro" \
        -v "$OUT_DIR:$OUT_DIR:rw" \
        -w "$OUT_DIR" \
        "$IMAGE" \
        /bin/bash -c '. "$EMSDK/emsdk_env.sh" >/dev/null 2>&1; exec "$@"' _ "$@"
        # bash, not sh: emsdk_env.sh is bash-shaped and silently declines under
        # dash, which surfaces much later as "emcc: not found" at link time
}

case "$MODE" in
module)
    # sample.wasm: the artifact name is fixed — the playground fetches it by
    # the content-addressed URL, not by source basename
    run_sandboxed "$DASLANG" -exe -output "$OUT_DIR/sample" "$SRC_DIR/$ENTRY" -- \
        --jit-target=wasm64-unknown-emscripten \
        --jit-runtime-lib="$RUNTIME_LIB"
    ;;
page)
    # sample.{html,js,wasm}: daspkg's release-wasm rail — the same one the five
    # /examples cards use — cross-compile + emcc link with the module archives,
    # GL/thread flags and the canvas shell. The service wrote a .das_package
    # naming the app `sample` into SRC_DIR before this ran, so the output set
    # is exact. The rail nests it under <out>/sample/; flatten INSIDE the
    # sandbox so the host never touches build-controlled paths.
    run_sandboxed /bin/bash -c '
        set -e
        "$1" "$2/utils/daspkg/main.das" -- release wasm --root "$3" --out "$4"
        mv "$4/sample/sample.html" "$4/sample/sample.js" "$4/sample/sample.wasm" "$4/"
        rm -rf "$4/sample"
    ' _ "$DASLANG" "$WORKTREE" "$SRC_DIR" "$OUT_DIR"
    ;;
*)
    echo "unknown mode '$MODE'"
    exit 14
    ;;
esac
