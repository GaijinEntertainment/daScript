#!/usr/bin/env bash
# The zen4-side build recipe dasweb-buildd invokes per claimed job:
#
#   run_build.sh <src_dir> <out_dir> <mode> <entry>
#
# Contract with the service (buildd_core.run_build_command):
#   - exit 0 and artifacts in <out_dir>  => success, everything servable there uploads
#   - nonzero exit                        => failure, stdout becomes the user-facing error
#   - stderr is redirected into stdout here — the service captures one stream
#
# Environment (set in the systemd unit / shell profile on the box):
#   DASWEB_WASM_WORKTREE   the dedicated wasm worktree (hard requirement — wasm and
#                          native builds poison each other's bin/ and lib/; see
#                          plans/dasweb_wasm_pipeline.md)
#   EMSDK                  the pinned emsdk root (5.0.7 — the version pages.yml pins)
#   DASWEB_BUILDD_UNSANDBOXED=1  bring-up escape hatch only: skip the sandbox wrapper
#
# Sandbox: compiling user .das is remote code execution by design (macros run at
# compile time), so the build runs under bwrap with no network, a tmpfs scratch,
# and only the worktree + job dirs mapped. Fail-closed: no bwrap and no explicit
# override means no build.
#
# mode=module: the per-sample compute recipe, verbatim from web/CMakeLists.txt
# (all_wasm): host daslang -exe with the wasm64 jit target against the build64
# runtime archive, emitting <out_dir>/sample.wasm.
# mode=page: the standalone game recipe (daspkg release wasm) — lands with the
# game checkpoint of plans/dasweb_wasm_pipeline.md; refused until then.

set -u
exec 2>&1

SRC_DIR="${1:?usage: run_build.sh <src_dir> <out_dir> <mode> <entry>}"
OUT_DIR="${2:?missing out_dir}"
MODE="${3:?missing mode}"
ENTRY="${4:?missing entry}"

WORKTREE="${DASWEB_WASM_WORKTREE:?DASWEB_WASM_WORKTREE is not set}"
DASLANG="$WORKTREE/bin/daslang"
[ -x "$DASLANG" ] || { echo "no daslang host at $DASLANG (build web/build_wasm_host.sh first)"; exit 10; }

# the wasm link shells out to emcc — bring the pinned emsdk onto PATH
EMSDK_ROOT="${EMSDK:?EMSDK is not set (the pinned emsdk root)}"
. "$EMSDK_ROOT/emsdk_env.sh" >/dev/null 2>&1 || { echo "cannot source $EMSDK_ROOT/emsdk_env.sh"; exit 15; }

# The wasm64 runtime archive jit-runtime-lib links against; `daspkg build
# --wasm` stages it (with the module archives) into web/output64/lib.
RUNTIME_LIB="$(ls "$WORKTREE"/web/output64/lib/*libDaScript_runtime*.a 2>/dev/null | head -1)"
[ -n "$RUNTIME_LIB" ] || { echo "no wasm64 runtime archive under $WORKTREE/web/output64/lib (run daspkg build --wasm)"; exit 11; }

run_sandboxed() {
    if command -v bwrap >/dev/null 2>&1; then
        # The emscripten cache is read-only in the sandbox and FROZEN so a
        # hostile build can never poison what later builds link against. The
        # toolchain-bump protocol warms it: one unsandboxed canary build after
        # `daspkg build --wasm` populates every system-lib variant this link
        # line needs.
        export EM_FROZEN_CACHE=1
        # no network, fresh tmpfs /tmp, everything read-only except the job's
        # own dirs (mount order matters: the job binds override the tmpfs)
        exec bwrap \
            --unshare-net --unshare-pid --die-with-parent \
            --ro-bind / / \
            --dev /dev \
            --tmpfs /tmp \
            --ro-bind "$SRC_DIR" "$SRC_DIR" \
            --bind "$OUT_DIR" "$OUT_DIR" \
            --tmpfs "$HOME/.cache" \
            "$@"
    fi
    if [ "${DASWEB_BUILDD_UNSANDBOXED:-0}" = "1" ]; then
        echo "WARNING: running unsandboxed (DASWEB_BUILDD_UNSANDBOXED=1)"
        exec "$@"
    fi
    echo "no bwrap on this box and DASWEB_BUILDD_UNSANDBOXED not set — refusing to build untrusted source"
    exit 12
}

case "$MODE" in
module)
    # sample.wasm: the artifact name is fixed — the playground runtime fetches
    # it by the content-addressed URL, not by source basename
    run_sandboxed "$DASLANG" -exe -output "$OUT_DIR/sample" "$SRC_DIR/$ENTRY" -- \
        --jit-target=wasm64-unknown-emscripten \
        --jit-runtime-lib="$RUNTIME_LIB"
    ;;
page)
    echo "page mode is not wired yet (lands with the game checkpoint)"
    exit 13
    ;;
*)
    echo "unknown mode '$MODE'"
    exit 14
    ;;
esac
