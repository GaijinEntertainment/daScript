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
#
#  10. cap_fold_ctor.shader (const vector ctor `float3(0.5)`) -> a const. The typer
#      folds scalar const arithmetic but leaves const CONSTRUCTORS; flatten collapses
#      it to a single `float3Const` node.
#
#  11. cap_fold_bool.shader (boolean identities `true && c` / `false || c`) -> fold away.
#      The typer folds only fully-const ops, so the `&&`/`||` survive in source; flatten
#      folds them to the bare comparison, so no `and`/`or` node reaches the backend.
#
#  12. cap_fold_accumulator.shader (const accumulator `var acc=0; acc+=0.1`×4) -> a const.
#      flatten SSA-renames the reassigned accumulator to single-def versions and const-
#      props the chain to one constant: 0 add nodes, result = base * <const>. Without it
#      the backend rejects the self-referential accumulator (50503).
#
#  13. cap_fold_reassoc.shader (scattered const chain `0.3 + gain + 0.4`) -> `gain + 0.7`.
#      The typer can't fold non-adjacent consts and never reassociates; flatten does (fast-math),
#      gathering them so exactly 1 add survives (not the 2 the source spells), + 1 base*k mul.
#
#  14. cap_preshader.shader (uniform `factor` = props-only) -> hoisted to a per-draw `_preshader_`
#      let. Uniform work the general compiler runs per-pixel; flatten routes it to the preshader so
#      the per-pixel graph is only `base * factor`. Asserted on the --das dump (per-draw routing is
#      not a node-count change).
#
#  15. cap_cse.shader (repeated `dot(c, LUMA)`) -> one shared node. Neither the compiler nor the
#      backend CSEs; flatten value-numbers the body and shares the repeat, so 1 dot node (not 2).
#
#  16. cap_mad.shader (mul-adds + a hand-written lerp shape) -> fused. The finishing pass packs
#      `a*b + c` into mad nodes (incl. the scalar-broadcast form) and `(b-a)*t + a` into a lerp
#      node, so the graph carries 2 mad + 1 lerp instead of mul/add/sub chains.
#
#  17. cap_rcp.shader (two per-pixel '/ exposure') -> ONE shared per-draw reciprocal
#      (`_preshader_N = 1f / exposure`) and two muls; the per-pixel graph carries no division
#      by the uniform. Asserted on the --das dump (fast-math; the reciprocal rounds).

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

echo "10. cap_fold_ctor.shader (const vector ctor float3(0.5) folds to a const)"
out="$(compile "$here/cap_fold_ctor.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
f3c="$(echo "$out" | grep -c ' float3Const ')"
errs="$(echo "$out" | grep -ci error)"
# The typer leaves `float3(0.5)` as a ctor (const constructors aren't infer-folded);
# flatten collapses it to a single float3Const node. Without the fold there is no
# float3Const for this expression (it'd be a vector ctor + floatConst inputs).
if [[ "$errs" -eq 0 && "$f3c" -ge 1 ]]; then
    echo "   ok — compiles ($nodes nodes), $f3c float3Const node(s) (const ctor folded)"
else
    echo "   FAIL — errors=$errs float3Const=$f3c (expected >=1 float3Const)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "11. cap_fold_bool.shader (boolean identities true&& / false|| fold away)"
out="$(compile "$here/cap_fold_bool.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
ands="$(echo "$out" | grep -c ' and ')"
ors="$(echo "$out" | grep -c ' or ')"
selects="$(echo "$out" | grep -c ' select ')"
errs="$(echo "$out" | grep -ci error)"
# `true && c` / `false || c` keep their `&&`/`||` in source (the typer folds only
# fully-const ops); flatten folds them to the bare compare, so no `and`/`or` node
# reaches the backend — just the comparisons feeding the selects.
if [[ "$errs" -eq 0 && "$ands" -eq 0 && "$ors" -eq 0 && "$selects" -gt 0 ]]; then
    echo "   ok — compiles ($nodes nodes), 0 and, 0 or, $selects select(s) (identities folded)"
else
    echo "   FAIL — errors=$errs and=$ands or=$ors selects=$selects (expected 0 and, 0 or, >0 select)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "12. cap_fold_accumulator.shader (const accumulator folds to a constant)"
out="$(compile "$here/cap_fold_accumulator.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
adds="$(echo "$out" | grep -c ' add ')"
muls="$(echo "$out" | grep -c ' mul ')"
errs="$(echo "$out" | grep -ci error)"
# `var acc = 0.0; acc += 0.1` ×4 — the general compiler does no const-propagation across
# the reassignments, and the backend rejects the self-referential accumulator outright
# (50503). flatten SSA-renames acc to single-def versions and const-props the chain to
# one constant, so it compiles with ZERO `add` nodes and result = `base * <const>`.
if [[ "$errs" -eq 0 && "$adds" -eq 0 && "$muls" -ge 1 ]]; then
    echo "   ok — compiles ($nodes nodes), 0 add (accumulator folded), $muls mul (base*const)"
else
    echo "   FAIL — errors=$errs adds=$adds muls=$muls (expected 0 add, >=1 mul)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "13. cap_fold_reassoc.shader (scattered const chain 0.3+gain+0.4 -> gain+0.7)"
out="$(compile "$here/cap_fold_reassoc.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
adds="$(echo "$out" | grep -c ' add ')"
muls="$(echo "$out" | grep -c ' mul ')"
errs="$(echo "$out" | grep -ci error)"
# The source spells two adds (`(0.3 + gain) + 0.4`); the typer can't fold them (consts non-
# adjacent, gain is a runtime prop) and the backend has no const-fold. flatten reassociates the
# chain to `gain + 0.7`, so exactly ONE add survives, plus the one genuine `base * k` multiply.
# Without reassoc this graph carries two adds.
if [[ "$errs" -eq 0 && "$adds" -eq 1 && "$muls" -eq 1 ]]; then
    echo "   ok — compiles ($nodes nodes), 1 add (gain+0.7, consts gathered), 1 mul (base*k)"
else
    echo "   FAIL — errors=$errs adds=$adds muls=$muls (expected 1 add, 1 mul)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "14. cap_preshader.shader (uniform factor -> hoisted to a per-draw _preshader_ let)"
# `factor` reads only props (brightness/tint) + literals, so it is uniform: identical every pixel.
# flatten hoists the whole subtree to a `_preshader_` let the backend routes to the per-draw
# preshader, leaving only `base * factor` per-pixel. Asserted on the --das dump (preshader routing
# is per-draw vs per-pixel, not a node-count change).
out="$(FLATTEN_DUMP_DAS=1 compile "$here/cap_preshader.shader")"
pre="$(echo "$out" | grep -c 'let _preshader_')"
errs="$(echo "$out" | grep -ci error)"
if [[ "$errs" -eq 0 && "$pre" -ge 1 ]]; then
    echo "   ok — $pre _preshader_ let(s) hoisted; per-pixel body is only base * factor"
else
    echo "   FAIL — errors=$errs preshader_lets=$pre (expected >=1)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "15. cap_cse.shader (repeated dot(c,LUMA) -> one shared node)"
out="$(compile "$here/cap_cse.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
dots="$(echo "$out" | grep -c 'dot')"
errs="$(echo "$out" | grep -ci error)"
# `dot(c, LUMA)` is spelled twice; neither the compiler nor the backend CSEs, so without flatten
# this is 2 dot nodes. flatten value-numbers the body and shares the repeat into one `_cse_` let,
# so exactly ONE dot node reaches the backend.
if [[ "$errs" -eq 0 && "$dots" -eq 1 ]]; then
    echo "   ok — compiles ($nodes nodes), 1 dot node (the repeat was CSE'd)"
else
    echo "   FAIL — errors=$errs dots=$dots (expected 1)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "16. cap_mad.shader (mul-adds + lerp shape -> 2 mad + 1 lerp nodes)"
out="$(compile "$here/cap_mad.shader")"
nodes="$(echo "$out" | grep -c '^node ')"
mads="$(echo "$out" | grep '^node ' | grep -cw 'mad')"
lerps="$(echo "$out" | grep '^node ' | grep -cw 'lerp')"
errs="$(echo "$out" | grep -ci error)"
# `lum*0.5 + 0.5` and `c*remap + bias` each pack into one mad; `(white - c)*remap + c` re-fuses
# into one lerp. An un-fused body would carry the raw mul/add/sub nodes instead.
if [[ "$errs" -eq 0 && "$mads" -eq 2 && "$lerps" -eq 1 ]]; then
    echo "   ok — compiles ($nodes nodes), 2 mad + 1 lerp (mul-adds fused)"
else
    echo "   FAIL — errors=$errs mads=$mads lerps=$lerps (expected 2 and 1)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo "17. cap_rcp.shader (per-pixel '/ uniform' -> one shared per-draw reciprocal)"
out="$(FLATTEN_DUMP_DAS=1 compile "$here/cap_rcp.shader")"
rcps="$(echo "$out" | grep -c 'let _preshader_.*1f / exposure')"
divs="$(echo "$out" | grep -v 'let _preshader_' | grep -c '/ exposure')"
errs="$(echo "$out" | grep -ci error)"
# both `c / exposure` and `(c.x+c.y) / exposure` ride the same `_preshader_N = 1f / exposure`;
# the per-pixel body must carry only muls by it.
if [[ "$errs" -eq 0 && "$rcps" -eq 1 && "$divs" -eq 0 ]]; then
    echo "   ok — one shared reciprocal preshader let; no per-pixel division by the uniform"
else
    echo "   FAIL — errors=$errs rcp_lets=$rcps residual_divs=$divs (expected 1 and 0)"
    echo "$out" | grep -i error | head
    fail=1
fi

echo
if [[ "$fail" -eq 0 ]]; then echo "capability: all checks passed"; else echo "capability: FAILED"; fi
exit $fail
