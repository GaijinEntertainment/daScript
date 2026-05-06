#!/usr/bin/env bash
# CI smoke test: full daspkg install -> release -> launch cycle for the sequence
# game on Linux/macOS. Exercises the release_include_dll manifest API + daspkg's
# native-dep shipping + the rpath-based loader resolution end-to-end.
#
# Args: $1 (optional) — path to a daslang install root (must contain bin/daslang
#       and utils/daspkg/main.das). Defaults to env DAS_ROOT.
#
# Linux note: invoke under xvfb-run if no display server is present. macOS
# runners have a desktop session; no wrapper needed.
#
# Exit: 0 on success; non-zero on any step failure.

set -euo pipefail

DAS_ROOT="${1:-${DAS_ROOT:-}}"
if [ -z "$DAS_ROOT" ]; then
    echo "usage: ci_smoke_test.sh <daslang-install-root>  (or set DAS_ROOT)" >&2
    exit 2
fi
DAS_ROOT="$(cd "$DAS_ROOT" && pwd)"
# Honor $BIN if set (extended_checks sets it to ./bin/Release on Windows
# multi-config; ./bin elsewhere); default to <DAS_ROOT>/bin for installed layouts.
BIN_DIR="${BIN:-$DAS_ROOT/bin}"
BIN_DIR="$(cd "$BIN_DIR" && pwd)"
DASLANG="$BIN_DIR/daslang"
DASPKG="$DAS_ROOT/utils/daspkg/main.das"
for p in "$DASLANG" "$DASPKG"; do
    [ -f "$p" ] || { echo "missing: $p" >&2; exit 2; }
done

# Source dir of THIS script's project (the in-tree sequence example).
SRC_DIR="$(cd "$(dirname "$0")" && pwd)"
WORK_DIR="$(mktemp -d -t daspkg_smoke_sequence.XXXXXXXX)"
echo "smoke: src=$SRC_DIR"
echo "smoke: work=$WORK_DIR"

# Copy the project to a scratch dir so we don't dirty the source tree on rerun.
# Filter out previous bundle/install artifacts so we always start clean.
shopt -s dotglob nullglob
for entry in "$SRC_DIR"/*; do
    name="$(basename "$entry")"
    case "$name" in
        modules|sequence|sequence.app|daspkg.lock|.daspkg_cache|.daspkg_tmp|.daspkg.log) continue;;
    esac
    cp -R "$entry" "$WORK_DIR/"
done
shopt -u dotglob nullglob

cd "$WORK_DIR"

echo "smoke: daspkg install"
"$DASLANG" "$DASPKG" -- install

echo "smoke: daspkg release"
"$DASLANG" "$DASPKG" -- release

# Determine bundle layout (.app on macOS, plain dir elsewhere) and the per-OS
# native dep filename. Matches the layout daspkg's commands.das produces.
# Note: daslang -exe always writes <name>.exe; only macOS renames to drop the
# extension. CMake target `libDaScriptDyn` becomes `liblibDaScriptDyn` on POSIX
# (auto `lib` prefix on top of the `lib` already in the target name).
case "$(uname -s)" in
    Darwin)
        BUNDLE="$WORK_DIR/sequence.app/Contents/MacOS"
        NATIVE="libglfw.3.4.dylib"
        RUNTIME_PATTERN="liblibDaScriptDyn*.dylib"
        EXE="$BUNDLE/sequence"
        ;;
    *)
        BUNDLE="$WORK_DIR/sequence"
        NATIVE="libglfw.so.3.4"
        RUNTIME_PATTERN="liblibDaScriptDyn*.so"
        EXE="$BUNDLE/sequence.exe"
        ;;
esac

# Verify expected bundle artifacts. Fail the test if anything is missing —
# these are the load-bearing files for a runnable bundle.
required=(
    "$EXE"
    "$BUNDLE/modules/dasGlfw/dasModuleGlfw.shared_module"
    # The whole point: native dep colocated with its shared_module.
    "$BUNDLE/modules/dasGlfw/$NATIVE"
    # Regression check: dep release_include assets still ship.
    "$BUNDLE/modules/das-cards/cards/svg-cards.svg"
)
for f in "${required[@]}"; do
    if [ ! -f "$f" ]; then
        echo "missing bundle artifact: $f" >&2
        exit 1
    fi
    echo "smoke: ok  $f"
done
# Runtime DLL is glob-matched (filename includes a suffix that varies).
if ! ls "$BUNDLE"/$RUNTIME_PATTERN >/dev/null 2>&1; then
    echo "missing bundle artifact: $BUNDLE/$RUNTIME_PATTERN" >&2
    exit 1
fi
echo "smoke: ok  $BUNDLE/$RUNTIME_PATTERN"

# Launch the bundled exe under a 60s outer timeout. The launch flag varies:
#   Linux/Windows -> --max-frames 60 (~1 sec at 60fps): full GLFW init + main
#                    loop + clean shutdown.
#   macOS         -> --smoke: module load + arg parse only, no glfwCreateWindow.
#                    macOS arm64-xlarge runners have no WindowServer session, so
#                    full launch crashes at window creation. --smoke still
#                    exercises the complete loader chain (every `require`
#                    resolves its .shared_module and dlopens its native deps at
#                    module-load time, before main() runs).
# Failure modes: missing DLL -> non-zero exit immediately; hang -> outer timeout.
#
# `timeout` isn't installed on macOS by default (BSD userland; coreutils ships
# `gtimeout` via brew). Fall back through gtimeout to a perl alarm wrapper -
# perl ships on every macOS and Linux runner we use.
if command -v timeout >/dev/null 2>&1; then
    TIMEOUT_PREFIX=(timeout 60)
elif command -v gtimeout >/dev/null 2>&1; then
    TIMEOUT_PREFIX=(gtimeout 60)
else
    TIMEOUT_PREFIX=(perl -e '$SIG{ALRM}=sub{exit 124}; alarm(shift); exec(@ARGV) or exit 127' 60)
fi

case "$(uname -s)" in
    Darwin) LAUNCH_ARGS=(--smoke) ;;
    *)      LAUNCH_ARGS=(--max-frames 60) ;;
esac

echo "smoke: launching bundled sequence ${LAUNCH_ARGS[*]} (under: ${TIMEOUT_PREFIX[*]})"
STDOUT_LOG="$WORK_DIR/smoke_stdout.log"
STDERR_LOG="$WORK_DIR/smoke_stderr.log"
set +e
( cd "$BUNDLE" && "${TIMEOUT_PREFIX[@]}" "$EXE" "${LAUNCH_ARGS[@]}" ) \
    >"$STDOUT_LOG" 2>"$STDERR_LOG"
rc=$?
set -e

echo "--- stdout ---"; tail -n 40 "$STDOUT_LOG" || true
echo "--- stderr ---"; tail -n 40 "$STDERR_LOG" || true

# Both `timeout`/`gtimeout` and our perl fallback signal "outer timeout fired"
# the same way: exit 124.
if [ "$rc" -eq 124 ]; then
    echo "smoke: TIMEOUT (60s) - sequence did not exit" >&2
    exit 1
fi
if [ "$rc" -ne 0 ]; then
    echo "smoke: sequence exited with rc=$rc" >&2
    exit 1
fi
echo "smoke: PASS"
echo "smoke: artifacts in $WORK_DIR"
