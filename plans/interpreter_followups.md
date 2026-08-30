# Interpreter follow-ups

Ledger for interpreter-speed findings that are not being acted on in the arc that found them.
Boris's standing direction (2026-08-29): the honest plan is a new interpreter altogether - the
same node-based design with less overhead per node - not more fusion permutations on this one,
which is already very fast. Entries here are the yardsticks and the evidence for that rewrite.

## op2 fusion has no array-element operand kind

Yardstick: dasProfile `queen`, interpreter lane, 3990X - das 1.50 ms vs LuaJIT `-joff` 1.23 ms,
one of the few rows the interpreter loses.

Evidence: the `isplaceok` inner loop runs ~11 sim nodes per iteration (`ForRange1` step,
`IfThen`, `OrAny`, three `Equ*`, three `AtR2VUArgLoc` loads, two `Add/SubArgLoc`) at ~3.5 ns
each; LuaJIT's interpreter covers the same body in 8-9 bytecodes. Removing `[unroll_full]`, the
`unsafe_range_check` hint, hand-inlining `isplaceok` (the compiler does not inline it), or
indexing `a[i]` directly all leave it at 1.46-1.52 ms - the node count is the cost.

The op2 fusion (`include/daScript/simulate/simulate_fusion_op2_impl.h`) matches operands by
node kind - `GetArgument`, `GetLocalR2V`, `ConstValue`, `GetLocalRefOffR2V`,
`GetArgumentRefOffR2V`, `GetThisBlockArgument` - and an array element (`AtR2V*`) is not one, so
`a[i] == c` fuses only as `EquAny_Arg` with the load as a virtual sub-eval. An `At`-operand kind
would collapse 3 of the 11 nodes (~25%), which is parity with LuaJIT on this row, not a win.
