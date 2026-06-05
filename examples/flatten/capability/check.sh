#!/usr/bin/env bash
# check.sh — flatten capability tier.
#
# These shaders use constructs the backend bans outright (if/else, helper calls).
# flatten lowers them so the backend accepts them. Two checks:
#
#   1. cap_control.shader (if/else)  -> compiles to a Select-based graph.
#      The backend bans ExprIfThenElse, so without flatten this would not compile;
#      with flatten the `if` becomes `?:` -> a `select` node (+ comparison masks).
#
#   2. cap_helper.shader (helper fn) -> identical opcode multiset to its hand-inlined
#      twin cap_inlined.shader. flatten's inlining is transparent.
#
#   3. cap_loop.shader (fixed for-loop) -> compiles to an unrolled `add` chain.
#      The backend bans ExprFor; flatten unrolls it to straight-line accumulation.
#
#   4. cap_swizzle.shader (user call buried under a `.xyz` swizzle) -> compiles.
#      flatten recurses through the swizzle operand to inline the call; the
#      backend bans user calls, so a clean compile proves it was inlined.
#
#   5. cap_with.shader (a `with (inp)` scope) -> compiles. The backend bans
#      ExprWith; flatten unwraps it to its body (post-infer it's pure name
#      resolution), so a clean compile proves it was omitted.

set -u
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
root="$(cd "$here/../../.." && pwd)"
daslang="$root/bin/daslang"
proj="$here/../flatten_shaders.das_project"

compile() { "$daslang" -compile-only -project "$proj" "$1" 2>&1; }
hist()    { compile "$1" | grep '^node ' | awk '{print $3}' | sort | uniq -c; }

fail=0

echo "1. cap_control.shader (if/else -> Select)"
out="$(compile "$here/cap_control.shader")"
selects="$(echo "$out" | grep -c ' select ')"
errs="$(echo "$out" | grep -ci error)"
if [[ "$errs" -eq 0 && "$selects" -gt 0 ]]; then
    echo "   ok — compiles, $selects select node(s) from the lowered if/else"
else
    echo "   FAIL — errors=$errs selects=$selects"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "2. cap_helper.shader == cap_inlined.shader (helper inlining parity)"
h="$(hist "$here/cap_helper.shader")"
i="$(hist "$here/cap_inlined.shader")"
if [[ -n "$h" && "$h" == "$i" ]]; then
    echo "   ok — identical opcode multiset ($(echo "$h" | awk '{s+=$1} END {print s}') nodes)"
else
    echo "   FAIL — opcode histograms differ:"
    diff <(echo "$h") <(echo "$i")
    fail=1
fi

echo "3. cap_loop.shader (for-loop -> unrolled accumulation)"
out="$(compile "$here/cap_loop.shader")"
adds="$(echo "$out" | grep -c ' add ')"
errs="$(echo "$out" | grep -ci error)"
if [[ "$errs" -eq 0 && "$adds" -gt 0 ]]; then
    echo "   ok — compiles, $adds add node(s) from the unrolled loop"
else
    echo "   FAIL — errors=$errs adds=$adds"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "4. cap_swizzle.shader (user call under a .xyz swizzle -> inlined)"
out="$(compile "$here/cap_swizzle.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
errs="$(echo "$out" | grep -ci error)"
if [[ "$errs" -eq 0 && "$nodes" -gt 0 ]]; then
    echo "   ok — compiles ($nodes nodes); the swizzled user call was inlined"
else
    echo "   FAIL — errors=$errs nodes=$nodes"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "5. cap_with.shader (with-scope -> unwrapped to body)"
out="$(compile "$here/cap_with.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
errs="$(echo "$out" | grep -ci error)"
if [[ "$errs" -eq 0 && "$nodes" -gt 0 ]]; then
    echo "   ok — compiles ($nodes nodes); the with-scope was unwrapped"
else
    echo "   FAIL — errors=$errs nodes=$nodes"
    echo "$out" | grep -i error | head
    fail=1
fi

echo
if [[ "$fail" -eq 0 ]]; then echo "capability: all checks passed"; else echo "capability: FAILED"; fi
exit $fail
