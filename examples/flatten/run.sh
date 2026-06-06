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
#   ./run.sh [--das] <name|path>     # --das also prints the flattened daslang source
#                                    #   (the branchless rewrite) above the opcodes;
#                                    #   <name|path> accepts a bare name or a .shader path,
#                                    #   e.g. ./run.sh --das capability/cap_control.shader
#
# Requires bin/daslang built in the daScript root.

set -u
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
root="$(cd "$here/../.." && pwd)"
daslang="$root/bin/daslang"
proj="$here/flatten_shaders.das_project"

dump_das=0
shaders=()
for a in "$@"; do
    if [[ "$a" == "--das" ]]; then
        dump_das=1
    else
        shaders+=("$a")
    fi
done

if [[ ! -x "$daslang" ]]; then
    echo "error: $daslang not found — build daScript first (cmake --build build --config Release)" >&2
    exit 1
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
        exec env FLATTEN_DUMP_DAS=1 "$daslang" -compile-only -project "$proj" "$f"
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
