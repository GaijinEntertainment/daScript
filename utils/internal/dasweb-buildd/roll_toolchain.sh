#!/usr/bin/env bash
# Roll the wasm toolchain to a new commit, on the build box, as the box user.
#
# The toolchain id is the wasm worktree's HEAD sha, so moving the worktree
# re-keys every cached artifact — every sample pays a real build on its next
# click. That makes this a one-shot operation to run after a batch has landed,
# never per-merge.
#
# Moving the worktree alone is NOT a roll. run_build.sh consumes built things,
# and a daslang change lands in one of them:
#   bin/daslang                       the cross-compile host, which emits the
#                                     emcc link line (link flags live here)
#   web/output64/lib/*_runtime*.a     the runtime archive linked INTO artifacts
#                                     (runtime behaviour lives here)
#   web/output64/lib/liblib*Imgui*.a  the dasImgui family — daspkg build --wasm
#                                     does NOT build these, and the sandbox
#                                     cannot (no cmake, read-only worktree)
# Pulling without rebuilding announces a new id whose artifacts still carry the
# old code — the worst outcome, because the cache is invalidated and nothing is
# actually fixed.
#
#   ./roll_toolchain.sh                 roll to origin/master
#   ./roll_toolchain.sh --ref <sha>     roll to a specific commit
#   ./roll_toolchain.sh --green         roll to the newest origin/master commit
#                                       whose required CI workflows all passed
#   ./roll_toolchain.sh --if-changed    exit 0 without touching anything when the
#                                       last COMPLETED roll already reached the
#                                       target (the nightly timer's mode)
#   ./roll_toolchain.sh --green-check <runs.json>
#                                       exit 0 when the Actions runs in the file
#                                       make a commit green, 1 otherwise; touches
#                                       nothing (the predicate's test seam)
#   ./roll_toolchain.sh --dry-run       print the plan, touch nothing
#   ./roll_toolchain.sh --skip-restart  build and warm, leave the service alone
#
# Env (defaults match the systemd unit and the box):
#   DASWEB_WASM_WORKTREE   the wasm worktree whose HEAD is the toolchain id
#   EMSDK                  the pinned emsdk root
#   HOST_CC / HOST_CXX     compiler for the cross-compile host. Debian 12's bare
#                          clang is 14 and cannot build the tree; clang-19 can.
#   DASWEB_GITHUB_REPO     owner/name whose Actions runs --green reads
#   DASWEB_GREEN_DEPTH     how many origin/master commits --green walks (30)
#   GITHUB_TOKEN           optional; --green reads the public API unauthenticated
#                          (60 requests/hour) and sends this as a bearer when set
set -euo pipefail

WORKTREE="${DASWEB_WASM_WORKTREE:-/home/boris/daScript-wasm}"
EMSDK_ROOT="${EMSDK:-/home/boris/emsdk}"
HOST_CC="${HOST_CC:-clang-19}"
HOST_CXX="${HOST_CXX:-clang++-19}"
SERVICE="dasweb-buildd"
GITHUB_REPO="${DASWEB_GITHUB_REPO:-GaijinEntertainment/daScript}"
# The workflows a master commit must have passed to count as green.
REQUIRED_WORKFLOWS='["wasm_build","build","build_eastl","extended checks"]'
GREEN_DEPTH="${DASWEB_GREEN_DEPTH:-30}"
# Written after the last roll step. --if-changed skips only when the marker AND
# the worktree's HEAD both name the target: a roll that died after its checkout
# leaves HEAD at a target with nothing rebuilt, and a later roll can leave HEAD
# somewhere the marker never named.
ROLL_MARKER=".dasweb-roll-complete"
REF=""
GREEN=0
IF_CHANGED=0
GREEN_CHECK=""
DRY_RUN=0
SKIP_RESTART=0

while [ $# -gt 0 ]; do
    case "$1" in
        --ref)          REF="${2:?--ref needs a commit-ish}"; shift 2 ;;
        --green)        GREEN=1; shift ;;
        --if-changed)   IF_CHANGED=1; shift ;;
        --green-check)  GREEN_CHECK="${2:?--green-check needs a runs.json path}"; shift 2 ;;
        --dry-run)      DRY_RUN=1; shift ;;
        --skip-restart) SKIP_RESTART=1; shift ;;
        -h|--help)      awk 'NR > 1 && /^#/ { print; next } NR > 1 { exit }' "$0"; exit 0 ;;
        *)              echo "unknown argument: $1" >&2; exit 2 ;;
    esac
done
if [ "$GREEN" = 1 ] && [ -n "$REF" ]; then
    echo "--green and --ref both name the target; pass one" >&2
    exit 2
fi
[ -n "$REF" ] || REF="origin/master"

say()  { printf '\n=== %s\n' "$*"; }
run()  { if [ "$DRY_RUN" = 1 ]; then printf '  would run: %s\n' "$*"; else "$@"; fi; }

# A commit is green when its push-triggered Actions runs cover every required
# workflow with success and nothing that ran on it failed. An in-progress
# required run is not proof, so such a commit is not green yet.
runs_are_green() {
    jq -e --argjson need "$REQUIRED_WORKFLOWS" '
        [.workflow_runs[] | select(.event == "push")] as $runs
        | ($need - [$runs[] | select(.conclusion == "success") | .name]) == []
          and ([$runs[] | select(.conclusion != null and .conclusion != "success" and .conclusion != "skipped")] == [])
    ' >/dev/null
}

# Walk origin/master newest-first and print the first green commit.
newest_green_master() {
    local sha auth=()
    [ -n "${GITHUB_TOKEN:-}" ] && auth=(-H "Authorization: Bearer $GITHUB_TOKEN")
    for sha in $(git rev-list --first-parent -n "$GREEN_DEPTH" origin/master); do
        if curl -fsS "${auth[@]}" -H "Accept: application/vnd.github+json" \
                "https://api.github.com/repos/$GITHUB_REPO/actions/runs?head_sha=$sha&per_page=100" \
                | runs_are_green; then
            echo "$sha"
            return 0
        fi
    done
    echo "no green origin/master commit in the last $GREEN_DEPTH" >&2
    return 1
}

if [ -n "$GREEN_CHECK" ]; then
    command -v jq >/dev/null || { echo "--green-check needs jq" >&2; exit 15; }
    runs_are_green < "$GREEN_CHECK"
    exit $?
fi

# .git is a FILE in a linked worktree (a gitdir pointer), so ask git itself.
git -C "$WORKTREE" rev-parse --git-dir >/dev/null 2>&1 \
    || { echo "no git worktree at $WORKTREE" >&2; exit 10; }
[ -f "$EMSDK_ROOT/emsdk_env.sh" ] || { echo "no emsdk at $EMSDK_ROOT" >&2; exit 11; }

cd "$WORKTREE"

# Tracked-file changes mean someone edited the toolchain by hand; rolling would
# silently discard or preserve them. Untracked build dirs are expected and fine.
if [ -n "$(git status --porcelain --untracked-files=no)" ]; then
    echo "worktree has modified tracked files — resolve before rolling:" >&2
    git status --short --untracked-files=no >&2
    exit 12
fi

OLD_ID="$(git rev-parse HEAD)"
# Fetch even under --dry-run: it only moves remote-tracking refs, and without it
# the printed plan would target whatever origin/master pointed at LAST fetch.
git fetch origin --quiet
if [ "$GREEN" = 1 ]; then
    command -v jq >/dev/null || { echo "--green needs jq" >&2; exit 15; }
    NEW_ID="$(newest_green_master)" || exit 14
    TARGET_LABEL="newest green origin/master"
else
    NEW_ID="$(git rev-parse "$REF")"
    TARGET_LABEL="$REF"
fi

if [ "$IF_CHANGED" = 1 ] && [ "$OLD_ID" = "$NEW_ID" ] && [ "$(cat "$ROLL_MARKER" 2>/dev/null)" = "$NEW_ID" ]; then
    echo "last completed roll already reached ${NEW_ID:0:9} ($TARGET_LABEL); nothing to do"
    exit 0
fi

say "toolchain roll"
printf '  worktree : %s\n  from     : %s\n  to       : %s (%s)\n' \
    "$WORKTREE" "${OLD_ID:0:9}" "${NEW_ID:0:9}" "$TARGET_LABEL"
# Already-at-target is an early exit only under --if-changed, and only once the
# marker says that roll completed: a roll that died mid-way leaves the worktree
# moved but the rebuild/warm/restart undone, and re-running must finish the job.
# Every later step is idempotent and cheap when already up to date.
if [ "$OLD_ID" = "$NEW_ID" ]; then
    echo "  worktree already at target — resuming (rebuild/warm/restart are idempotent)"
fi

say "1/6  move the worktree"
run git checkout --quiet --detach "$NEW_ID"

# The host bakes its OWN C++ ABI (struct layouts, exception model) into every
# wasm module it codegens, so it must be built by this script — see its header.
# emsdk must NOT be on PATH here: emsdk_env prepends its own clang, and a host
# accidentally built by the wasm-targeting compiler is exactly the silent-heap-
# corruption mismatch that script's header warns about.
say "2/6  rebuild the cross-compile host (bin/daslang, $HOST_CXX)"
run env HOST_CC="$HOST_CC" HOST_CXX="$HOST_CXX" bash web/build_wasm_host.sh

# Everything from here needs emcc, so each build step sources emsdk in its own
# subshell — nothing leaks into this shell or into the restart.
say "3/6  restage the wasm runtime + module archives (web/output64/lib)"
run bash -c '. "$1/emsdk_env.sh" >/dev/null 2>&1 && cd "$2" && ./bin/daslang utils/daspkg/main.das -- build --wasm' _ "$EMSDK_ROOT" "$WORKTREE"

# daspkg build --wasm covers the in-tree module list only. The dasImgui family
# builds via its own .das_package recipe — normally run by daspkg's
# ensure_external_wasm_archives, which the SANDBOX cannot run (no cmake, and the
# worktree is mounted read-only), so it runs here, host-side, per toolchain.
# The build command and archives MIRROR modules/dasImgui/.das_package, minus
# liblibDasModuleClipboard.a — that one is in daspkg's own in-tree list and is
# staged by step 3. REVIEW.md: "changing one side without the other is a defect".
say "4/6  bake the dasImgui wasm archives"
run bash -c '
    set -euo pipefail
    . "$1/emsdk_env.sh" >/dev/null 2>&1
    cd "$2/modules/dasImgui"
    emcmake cmake -S . -B _wasm_build -DDASLANG_DIR="$2" -DCMAKE_BUILD_TYPE=Release -G Ninja
    cmake --build _wasm_build --target dasModuleImgui imguiApp imguiAppHeadless
    cp -f _wasm/liblibDasModuleImgui.a \
          _wasm/liblibImguiApp.a \
          _wasm/liblibImguiAppHeadless.a \
          _wasm_build/_deps/freetype_src-build/libfreetype.a \
          "$2/web/output64/lib/"
' _ "$EMSDK_ROOT" "$WORKTREE"

# The sandbox mounts the emcc cache READ-ONLY with EM_FROZEN_CACHE=1, so a job
# cannot populate it. Both modes must be warmed here, outside the sandbox: the
# page rail links -pthread and pulls the -mt system libraries a module build
# never touches, so warming only one leaves the other failing on a cold cache.
say "5/6  warm the emcc cache — one build of EACH mode, outside the sandbox"
if [ "$DRY_RUN" = 1 ]; then
    echo "  would run: a module build and a page build into a scratch dir"
else
    WARM="$(mktemp -d)"
    trap 'rm -rf "$WARM"' EXIT
    RUNTIME_LIB="$(ls "$WORKTREE"/web/output64/lib/*libDaScript_runtime*.a 2>/dev/null | head -1)"
    [ -n "$RUNTIME_LIB" ] || { echo "no runtime archive after step 3" >&2; exit 13; }

    mkdir -p "$WARM/src" "$WARM/mod" "$WARM/page"
    printf 'options gen2\n[export]\ndef main {\n    print("warm\\n")\n}\n' > "$WARM/src/main.das"

    echo "  module mode…"
    bash -c '. "$1/emsdk_env.sh" >/dev/null 2>&1 && "$2/bin/daslang" -exe -output "$3/mod/sample" "$3/src/main.das" -- --jit-target=wasm64-unknown-emscripten --jit-runtime-lib="$4"' \
        _ "$EMSDK_ROOT" "$WORKTREE" "$WARM" "$RUNTIME_LIB"

    echo "  page mode…"
    # The same .das_package shape write_page_package (buildd_core.das) writes
    # for a real page job — a change to either without the other is a defect.
    printf 'options gen2\nrequire daslib/daspkg\n\n[export]\ndef package() {\n    package_name("sample")\n}\n\n[export]\ndef release() {\n    release_name("sample")\n    release_main("main.das")\n}\n' \
        > "$WARM/src/.das_package"
    bash -c '. "$1/emsdk_env.sh" >/dev/null 2>&1 && "$2/bin/daslang" "$2/utils/daspkg/main.das" -- release wasm --root "$3/src" --out "$3/page"' \
        _ "$EMSDK_ROOT" "$WORKTREE" "$WARM"
fi

say "6/6  restart the service (announces the new toolchain id)"
if [ "$SKIP_RESTART" = 1 ]; then
    echo "  skipped (--skip-restart); the service still announces ${OLD_ID:0:9}"
else
    run sudo systemctl restart "$SERVICE"
    run sleep 3
    run systemctl is-active "$SERVICE"
    [ "$DRY_RUN" = 1 ] || printf '%s\n' "$NEW_ID" > "$ROLL_MARKER"
fi

say "rolled ${OLD_ID:0:9} -> ${NEW_ID:0:9}"
echo "Every cached artifact is now re-keyed; the first click on each sample pays a real build."
echo "Verify with: node utils/internal/dasweb-verify/browser/runner.mjs --mode wasm"
