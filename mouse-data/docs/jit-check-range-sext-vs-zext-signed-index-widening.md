---
slug: jit-check-range-sext-vs-zext-signed-index-widening
title: In daslang's LLVM JIT, when do I need LLVMBuildSExt vs LLVMBuildZExt for widening an i32 index to i64 before an unsigned bounds compare?
created: 2026-05-21
last_verified: 2026-05-21
links: []
---

The JIT codegen widens index expressions to match the size operand's width before emitting `ICmp UGE` (unsigned greater-or-equal) for the bounds check. The choice between **SExt** (sign-extend) and **ZExt** (zero-extend) is determined by the daslang **index type's signedness**, not by anything LLVM can infer from the IR.

## The trap

Naive code uses ZExt for both:

```das
def normalize_int_widths(lhs : LLVMOpaqueValue?; rhs : LLVMOpaqueValue?) {
    ...
    return (LLVMBuildZExt(g_builder, lhs, LLVMTypeOf(rhs), ""), rhs) if (wl < wr)
    ...
}
```

For a signed `int idx = -2` JIT'd into i32, ZExt to i64 = `0x00000000FFFFFFFE` = `4294967294`. Then `ICmp UGE 4294967294, 5_000_000_000` is `false` — bounds check passes silently → corruption identical to the C++ tier (see [[uint32-cast-negative-int-wrap-bounds-check-bug-pattern]]).

## The fix

For **signed** index types (`tInt`, `tInt64`), widen with `LLVMBuildSExt`. Then `int32(-2)` becomes `0xFFFFFFFFFFFFFFFE` (sign-extended), and the unsigned compare against any reasonable size correctly catches it. For **unsigned** types (`tUInt`, `tUInt64`), keep `ZExt` — sign-extending a `uint32(0x80000000)` would incorrectly reject the legitimate large positive index.

```das
def check_range(tidx, maxIdx : LLVMOpaqueValue?; at : LineInfo; message : string;
                idxIsSigned : bool = true) {
    if (option_no_range_check) return
    let wl = LLVMGetIntTypeWidth(LLVMTypeOf(tidx))
    let wr = LLVMGetIntTypeWidth(LLVMTypeOf(maxIdx))
    var ntidx = tidx
    var nmax = maxIdx
    if (wl < wr) {
        if (idxIsSigned) {
            ntidx = LLVMBuildSExt(g_builder, tidx, LLVMTypeOf(maxIdx), "")
        } else {
            ntidx = LLVMBuildZExt(g_builder, tidx, LLVMTypeOf(maxIdx), "")
        }
    } elif (wr < wl) {
        nmax = LLVMBuildZExt(g_builder, maxIdx, LLVMTypeOf(tidx), "")  // size is always unsigned
    }
    ...
    var cond = LLVMBuildICmp(g_builder, LLVMIntPredicate.LLVMIntUGE, ntidx, nmax, "range_check")
    ...
}
```

## Pulling signedness from the daslang AST

LLVMOpaqueValue carries no signedness info — the IR layer is signedness-agnostic. Read it from `expr.index._type.baseType` at the callsite:

```das
def is_signed_index_type(t : TypeDeclPtr) : bool {
    return t != null && (t.baseType == Type.tInt || t.baseType == Type.tInt64)
}

// in visitExprAt(expr : ExprAt?):
check_range(tidx, size, expr.at, "array index out of range",
            is_signed_index_type(expr.index._type))
```

Three callsites in `llvm_jit.das`'s `visitExprAt`: dim (line ~1829), array (~1841), vector (~1872). All three need the same signedness arg.

## Bump LLVM_JIT_CODEGEN_VERSION when you change this

JIT artifacts are cached as content-addressed DLLs in `.jitted_scripts/<ns>/<hash>.dll`. The codegen-version constant in `modules/dasLLVM/daslib/llvm_macro.das` is folded into the hash. Any change to `check_range` shape — including the SExt fix — must bump `LLVM_JIT_CODEGEN_VERSION` (currently 0x04 post-#2776) so previously-cached DLLs miss and get GC'd.

## Related

- The C++ tiers carry the same neg-index bug — see [[where-array-indexing-lives-five-tier-map]] for the full sweep.
- Why ZExt works for unsigned: a `uint32(0x80000000)` (~2.1G) is a legitimate index into a >2.1G-element array; sign-extending it would corrupt the legitimate positive value into a negative-looking i64.

## Questions
- In daslang's LLVM JIT, when do I need LLVMBuildSExt vs LLVMBuildZExt for widening an i32 index to i64 before an unsigned bounds compare?
