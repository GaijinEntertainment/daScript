#!/usr/bin/env bash
# run.sh — flatten shader harness driver.
#
# The shaders under shaders/ are the real EdenSpark sample shaders, compiled
# UNEDITED against a local testing copy of the engine's shader-graph backend
# (backend/), with daslib/flatten wired in front of it. flatten does all the
# lowering (inline helper calls, if/else -> ?:, unroll fixed loops); the backend
# then walks the now-branchless shader and prints a human-readable opcode graph.
#
# Usage:
#   ./run.sh                         # regression: compile every shader, report pass/fail
#   ./run.sh <name>                  # print one shader's opcode graph
#                                    #   e.g. ./run.sh cel_shading
#   ./run.sh --update-golden         # rewrite golden/<name>.golden, the flattened daslang
#                                    #   source of every shader under shaders/, capability/
#                                    #   and scalar_packing/
#                                    #   (test_flatten_fold.das compares each shader with it)
#   ./run.sh [--das] <name|path>     # --das prints the flattened daslang source (the
#                                    #   branchless rewrite) instead of the opcode graph;
#                                    #   <name|path> accepts a bare name or a .shader path,
#                                    #   e.g. ./run.sh --das capability/cap_control.shader
#
# Binary: $DASLANG if set, else the first of bin/daslang (single-config layout),
# bin/Release/daslang[.exe] (MSVC layout), or `daslang` on PATH (installed SDK).
# The candidate list is duplicated in capability/check.sh — keep them in step.

set -u
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
root="$(cd "$here/../.." && pwd)"
proj="$here/flatten_shaders.das_project"

find_daslang() {
    if [[ -n "${DASLANG:-}" ]]; then echo "$DASLANG"; return; fi
    local c
    for c in "$root/bin/daslang" "$root/bin/Release/daslang.exe" "$root/bin/Release/daslang"; do
        if [[ -x "$c" ]]; then echo "$c"; return; fi
    done
    command -v daslang || command -v daslang.exe || true
}
daslang="$(find_daslang)"

# the backend logs its opcode graph at debug level
export DAS_LOG_LEVEL="${DAS_LOG_LEVEL:-debug}"

flattened_source() {   # the flattened daslang source of shader $1; fails when its compile does
    local dump
    dump="$(env DAS_LOG_LEVEL=warning FLATTEN_DUMP_DAS=1 \
        "$daslang" -compile-only -project "$proj" "$1" 2>&1)" || return 1
    echo "$dump" | sed -n '/^;; ---- flattened daslang ----$/,$p' | tail -n +2
}

dump_das=0
update_golden=0
shaders=()
for a in "$@"; do
    if [[ "$a" == "--das" ]]; then
        dump_das=1
    elif [[ "$a" == "--update-golden" ]]; then
        update_golden=1
    else
        shaders+=("$a")
    fi
done

if [[ -z "$daslang" || ! -x "$daslang" ]]; then
    echo "error: no daslang binary found — build daScript first (cmake --build build --config Release) or set DASLANG" >&2
    exit 1
fi

if [[ "$update_golden" -eq 1 ]]; then
    mkdir -p "$here/golden"
    count=0
    for f in "$here"/shaders/*/*.shader "$here"/capability/*.shader \
             "$here"/scalar_packing/*.shader; do
        if ! out="$(flattened_source "$f")" || [[ -z "$out" ]]; then
            echo "error: no flattened source for $f" >&2
            exit 1
        fi
        printf '%s\n' "$out" > "$here/golden/$(basename "$f" .shader).golden"
        count=$((count + 1))
    done
    echo "golden: $count written"
    exit 0
fi

find_shader() {
    local name="$1"
    find "$here/shaders" -name "${name}.shader" | head -1
}

# Single-shader mode: dump its opcodes (and, with --das, the flattened source).
if [[ "${#shaders[@]}" -ge 1 ]]; then
    name="${shaders[0]}"
    if [[ -f "$name" ]]; then
        f="$name"
    else
        f="$(find_shader "$name")"
    fi
    if [[ -z "$f" ]]; then
        echo "error: no shader named '$name' under shaders/ (and not a file path)" >&2
        exit 1
    fi
    if [[ "$dump_das" -eq 1 ]]; then
        exec env DAS_LOG_LEVEL=warning FLATTEN_DUMP_DAS=1 "$daslang" -compile-only -project "$proj" "$f"
    fi
    exec "$daslang" -compile-only -project "$proj" "$f"
fi

# Regression mode: every shader must compile through flatten + the backend and
# emit a graph with no errors (flatten is transparent on these branchless shaders).
pass=0
fail=0
failed=()
for f in "$here"/shaders/*/*.shader; do
    out="$("$daslang" -compile-only -project "$proj" "$f" 2>&1)"
    if echo "$out" | grep -qi "error"; then
        fail=$((fail + 1))
        failed+=("$(basename "$f"): $(echo "$out" | grep -i error | head -1)")
    else
        nodes="$(echo "$out" | grep -c '^node ')"
        pass=$((pass + 1))
        printf "  ok   %-34s %s nodes\n" "$(basename "$f")" "$nodes"
    fi
done

echo
echo "regression: $pass passed, $fail failed"
if [[ $fail -gt 0 ]]; then
    printf '  FAIL %s\n' "${failed[@]}"
    exit 1
fi
