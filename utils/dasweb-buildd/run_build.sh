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

# The wasm64 runtime archive daspkg's jit-runtime-lib links against; built by
# `daspkg build --wasm` into web/build64.
RUNTIME_LIB="$(ls "$WORKTREE"/web/build64/*libDaScript_runtime*.a 2>/dev/null | head -1)"
[ -n "$RUNTIME_LIB" ] || { echo "no wasm64 runtime archive under $WORKTREE/web/build64 (run daspkg build --wasm)"; exit 11; }

run_sandboxed() {
    if command -v bwrap >/dev/null 2>&1; then
        # no network, fresh tmpfs /tmp, worktree read-only, only the job dirs writable
        exec bwrap \
            --unshare-net --unshare-pid --die-with-parent \
            --ro-bind / / \
            --tmpfs /tmp \
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
