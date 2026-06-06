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
#
#   6. cap_loop_cond.shader (loop-var-gated accumulate) -> const-folds to zero selects.
#
#   7. cap_loop_break.shader (RUNTIME break) -> compiles to a stored bool mask
#      (boolConst init + and/not narrows) feeding per-copy selects. The backend has
#      no jump; flatten's loop-scoped mask + the backend's Phase-C bool-accumulator
#      rail make a runtime break expressible.
#
#   8. cap_loop_continue.shader (RUNTIME continue) -> a PER-COPY bool mask (one
#      boolConst init per unrolled copy) gating that copy's accumulation.
#
#   9. cap_fold_identity.shader (scalar identities `gain*1.0 + 0.0`) -> fold away.
#      The general compiler leaves them (gain is a runtime prop) and the backend has
#      no const-fold; flatten's fold removes them, so the graph has only the genuine
#      `base*gain` multiply: exactly 1 mul, 0 add.

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

echo "6. cap_loop_cond.shader (loop-var-gated accumulate -> const-folded, no select)"
out="$(compile "$here/cap_loop_cond.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
selects="$(echo "$out" | grep -c ' select ')"
errs="$(echo "$out" | grep -ci error)"
# The cond is a function of the loop var only, so every select const-folds away and
# flatten_fold drops the false-iteration self-assigns. Without the fold this errors
# (50503, self-referential accumulator); with it, zero select nodes remain.
if [[ "$errs" -eq 0 && "$nodes" -gt 0 && "$selects" -eq 0 ]]; then
    echo "   ok — compiles ($nodes nodes), $selects select nodes (all const-folded)"
else
    echo "   FAIL — errors=$errs nodes=$nodes selects=$selects (expected 0 selects)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "7. cap_loop_break.shader (runtime break -> stored bool mask + selects)"
out="$(compile "$here/cap_loop_break.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
selects="$(echo "$out" | grep -c ' select ')"
masks="$(echo "$out" | grep -c ' boolConst ')"
errs="$(echo "$out" | grep -ci error)"
# A runtime break leaves a genuine runtime mask: the bool mask local (boolConst init,
# narrowed by `&&`/`!` per copy) feeds a select on each unrolled copy. This is the
# first construct whose mask survives as a stored bool — Phase C lets the backend
# consume it.
if [[ "$errs" -eq 0 && "$selects" -gt 0 && "$masks" -gt 0 ]]; then
    echo "   ok — compiles ($nodes nodes), $selects select node(s) gated by a stored bool mask"
else
    echo "   FAIL — errors=$errs selects=$selects masks=$masks (expected >0 selects and a bool mask)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "8. cap_loop_continue.shader (runtime continue -> per-copy bool masks)"
out="$(compile "$here/cap_loop_continue.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
selects="$(echo "$out" | grep -c ' select ')"
masks="$(echo "$out" | grep -c ' boolConst ')"
errs="$(echo "$out" | grep -ci error)"
# continue is a PER-COPY mask: each unrolled copy declares its own `var __flat_iter = true`,
# so there are multiple boolConst inits (one per copy) — distinct from break's single
# persistent mask. Each gates that copy's accumulation via a select.
if [[ "$errs" -eq 0 && "$selects" -gt 0 && "$masks" -gt 1 ]]; then
    echo "   ok — compiles ($nodes nodes), $masks per-copy bool masks gating $selects select(s)"
else
    echo "   FAIL — errors=$errs selects=$selects masks=$masks (expected >0 selects and >1 per-copy masks)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "9. cap_fold_identity.shader (scalar identities gain*1.0 / +0.0 fold away)"
out="$(compile "$here/cap_fold_identity.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
muls="$(echo "$out" | grep -c ' mul ')"
adds="$(echo "$out" | grep -c ' add ')"
errs="$(echo "$out" | grep -ci error)"
# `gain * 1.0 + 0.0` collapses to `gain` (gain is a runtime prop, so the general
# compiler can't; the backend has no const-fold), leaving only the genuine
# `base * gain` multiply: exactly 1 mul, 0 add. Without the fold: 2 mul, 1 add.
if [[ "$errs" -eq 0 && "$muls" -eq 1 && "$adds" -eq 0 ]]; then
    echo "   ok — compiles ($nodes nodes), 1 mul (base*gain), 0 add (identities folded)"
else
    echo "   FAIL — errors=$errs muls=$muls adds=$adds (expected 1 mul, 0 add)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo
if [[ "$fail" -eq 0 ]]; then echo "capability: all checks passed"; else echo "capability: FAILED"; fi
exit $fail
