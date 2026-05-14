---
slug: how-does-foo-bft-bit-foo-bft-bit-actually-look-in-the-ast-when-foo-is-a-bitfield-does-the-rhs-stay-as-exprfield-or-get-folded
title: How does `foo |= BfT.bit` / `foo &= ~BfT.bit` actually look in the AST when `foo` is a bitfield — does the RHS stay as ExprField or get folded?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Two shapes depending on compile policies.** A lint rule that targets these patterns must handle both.

**`foo |= BfT.bit`** — outer is `ExprOp2(op="|=", left=ExprVar(foo), right=...)`. The RHS:
- Under **lint policies** (`no_optimizations` + `no_infer_time_folding`): RHS stays as `ExprField(value=ExprVar(BfT), name="bit")`. Symbolic.
- Under **normal compile** (dastest): RHS folds to `ExprConstBitfield(value=<single-bit mask>)`. The bit name is lost — only the mask survives.

**`foo &= ~BfT.bit`** — outer is `ExprOp2(op="&=", left=ExprVar(foo), right=...)`:
- Under lint: RHS is `ExprOp1(op="~", subexpr=ExprField(...))`.
- Under normal compile: RHS folds to `ExprConstBitfield(value=<complement mask>)`. To detect single-bit-clear, compute `complement = ~v & ((1ul << length(argNames)) - 1ul)` and check it's a power of two.

**Recover the bit name from a folded constant:**
```das
let v = uint64((expr as ExprConstBitfield).value)
if (v == 0ul || (v & (v - 1ul)) != 0ul) return ""  // not single-bit
var idx = 0
var probe = v
while (probe > 1ul) { probe = probe >> 1ul; idx++ }
return string(bf_type.argNames[idx])
```

Both shapes appear in real code — `daslib/style_lint.das:443-505` (STYLE022 detection) is the reference implementation. Test under the lint runner to verify both branches fire.

## Questions
- How does `foo |= BfT.bit` / `foo &= ~BfT.bit` actually look in the AST when `foo` is a bitfield — does the RHS stay as ExprField or get folded?
