options gen2
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_gemm_gen_f32 shared private

require dasllama/dasllama_lint public
require llvm/daslib/llvm_boost
require llvm/daslib/llvm_jit_code
require llvm/daslib/llvm_jit_common
require daslib/ast_boost
require math

//! the perm of the f32 GEMM tile family, read off the stamped `[llvm_code]` arguments
struct private F32Perm {
    width : int = 128   //! vector bits: 128, 256 (x64 AVX2), 512 (x64 AVX-512)
    mr : int = 4        //! rows a tile
    nv : int = 4        //! vectors a row in the full tile
    kstep : int = 1     //! k positions a main-loop pass
}

def private parse_f32_perm(gc : LlvmCodeCtx) : F32Perm {
    var p = F32Perm()
    let wv = find_arg(gc.ann.arguments, "width")
    if (wv is tInt) { p.width = wv as tInt }
    let mv = find_arg(gc.ann.arguments, "mr")
    if (mv is tInt) { p.mr = mv as tInt }
    let nv = find_arg(gc.ann.arguments, "nv")
    if (nv is tInt) { p.nv = nv as tInt }
    let kv = find_arg(gc.ann.arguments, "kstep")
    if (kv is tInt) { p.kstep = kv as tInt }
    return p
}

//! the one decline predicate the tile and its companions share: the width the target emits, the knobs' ranges, and the
//! tile's accumulators, weight vectors and broadcast inside the target's vector registers
def private f32_perm_declines(gc : LlvmCodeCtx; p : F32Perm) : bool {
    let x64 = target_x64(gc.jit.target)
    if (p.width == 256) {
        return true if (!x64 || !gc.jit.target.features.avx2)
    } elif (p.width == 512) {
        return true if (!x64 || !gc.jit.target.features.avx512bw)
    } elif (p.width != 128) {
        return true
    }
    return true if (p.mr < 1 || p.mr > 8 || p.nv < 1 || p.nv > 8 || (p.kstep != 1 && p.kstep != 2 && p.kstep != 4))
    let regs = target_aarch64(gc.jit.target) || p.width == 512 ? 32 : 16
    return p.mr * p.nv + p.nv + 1 > regs
}

//! what an emit pass carries: the builder, the tile's operands and its perm
struct private F32Emit {
    b : LLVMOpaqueBuilder?
    ctx : LLVMOpaqueContext?
    impl : LLVMOpaqueValue?
    types : PrimitiveTypes?
    cp : LLVMOpaqueValue?
    ap : LLVMOpaqueValue?
    bp : LLVMOpaqueValue?
    i0 : LLVMOpaqueValue?
    kn : LLVMOpaqueValue?       //! k positions
    cs : LLVMOpaqueValue?       //! C's row stride
    astride : LLVMOpaqueValue?  //! A's row stride
    bs : LLVMOpaqueValue?       //! B's row stride
    mr : int
    kstep : int
}

def private f32_ci64(e : F32Emit; v : int64) : LLVMOpaqueValue? => e.types->ConstI64(uint64(v))

//! a counted loop over [start, limit) in steps of `step`, passes taken while a whole step fits; `accs` enter as the
//! loop's carried values and leave as the values at its exit. Returns the index the loop stopped at.
def private f32_loop(e : F32Emit; var start, limit : LLVMOpaqueValue?; step : int64; tag : string; ty : LLVMOpaqueType?;
                     var accs : array<LLVMOpaqueValue?>; body : block<(var idx : LLVMOpaqueValue?; var accs : array<LLVMOpaqueValue?>) : void>) : LLVMOpaqueValue? {
    let b = e.b
    let i64 = e.types.t_int64
    var pre = LLVMGetInsertBlock(b)
    var head = LLVMAppendBasicBlockInContext(e.ctx, e.impl, "{tag}.head")
    var lbody = LLVMAppendBasicBlockInContext(e.ctx, e.impl, "{tag}.body")
    var exit = LLVMAppendBasicBlockInContext(e.ctx, e.impl, "{tag}.exit")
    LLVMBuildBr(b, head)
    LLVMPositionBuilderAtEnd(b, head)
    var idx = LLVMBuildPhi(b, i64, "{tag}.i")
    var vals <- [start]
    var blocks <- [pre]
    LLVMAddIncoming(idx, vals, blocks)
    var phis <- [for (_a in accs); LLVMBuildPhi(b, ty, "{tag}.acc")]
    for (ph, a in phis, accs) {
        var avals <- [a]
        var ablocks <- [pre]
        LLVMAddIncoming(ph, avals, ablocks)
    }
    var fits = LLVMBuildICmp(b, LLVMIntPredicate.LLVMIntSLE, LLVMBuildAdd(b, idx, f32_ci64(e, step), ""), limit, "{tag}.fits")
    LLVMBuildCondBr(b, fits, lbody, exit)
    LLVMPositionBuilderAtEnd(b, lbody)
    var cur := phis
    invoke(body, idx, cur)
    var next = LLVMBuildAdd(b, idx, f32_ci64(e, step), "{tag}.next")
    var tail = LLVMGetInsertBlock(b)
    LLVMBuildBr(b, head)
    var nvals <- [next]
    var nblocks <- [tail]
    LLVMAddIncoming(idx, nvals, nblocks)
    for (ph, c in phis, cur) {
        var cvals <- [c]
        var cblocks <- [tail]
        LLVMAddIncoming(ph, cvals, cblocks)
    }
    LLVMPositionBuilderAtEnd(b, exit)
    for (a, ph in accs, phis) {
        a = ph
    }
    return idx
}

//! `v` in every lane of `ty`; a scalar `ty` (lanes 1) takes it as it is
def private f32_splat(var e : F32Emit; ty : LLVMOpaqueType?; lanes : int; var v : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    return v if (lanes == 1)
    var one = LLVMBuildInsertElement(e.b, LLVMGetUndef(ty), v, e.types->ConstI32(0ul), "")
    let mask <- [for (_i in range(lanes)); 0]
    return LLVMBuildShuffleVector(e.b, e.types, one, one, mask, "a.splat")
}

//! b * a + acc as one fused step where the target fuses, the product and the sum elsewhere
def private f32_fma(var e : F32Emit; var ty : LLVMOpaqueType?; var bv, av, acc : LLVMOpaqueValue?) : LLVMOpaqueValue? {
    let id = LLVMLookupIntrinsicID("llvm.fmuladd")
    if (id != 0u) {
        var tys <- [ty]
        var decl = LLVMGetIntrinsicDeclaration(LLVMGetGlobalParent(e.impl), id, tys)
        if (decl != null) {
            var fty = LLVMFunctionType(ty, [ty, ty, ty])
            var args <- [bv, av, acc]
            return LLVMBuildCall2(e.b, fty, decl, args, "acc")
        }
    }
    return LLVMBuildFAdd(e.b, LLVMBuildFMul(e.b, bv, av, ""), acc, "acc")
}

//! one k position of a column block: every row's a broadcast against the block's B vectors
def private f32_k_step(var e : F32Emit; var ty : LLVMOpaqueType?; lanes, ncv : int; var jcol, kk : LLVMOpaqueValue?; var accs : array<LLVMOpaqueValue?>) {
    let b = e.b
    var brow = LLVMBuildAdd(b, LLVMBuildMul(b, kk, e.bs, ""), jcol, "b.at")
    var bvs <- [for (v in range(ncv)); LLVMBuildLoad2Aligned(b, ty, LLVMBuildGEP2(b, e.types.t_float, e.bp, v == 0 ? brow : LLVMBuildAdd(b, brow, f32_ci64(e, int64(v * lanes)), ""), ""), 4u, "b")]
    for (r in range(e.mr)) {
        var row = r == 0 ? e.i0 : LLVMBuildAdd(b, e.i0, f32_ci64(e, int64(r)), "")
        var aat = LLVMBuildAdd(b, LLVMBuildMul(b, row, e.astride, ""), kk, "a.at")
        var a = LLVMBuildLoad2Aligned(b, e.types.t_float, LLVMBuildGEP2(b, e.types.t_float, e.ap, aat, ""), 4u, "a")
        var av = f32_splat(e, ty, lanes, a)
        for (v in range(ncv)) {
            accs[r * ncv + v] = f32_fma(e, ty, bvs[v], av, accs[r * ncv + v])
        }
    }
}

//! rows [i0, i0 + mr) by `ncv` vectors of `lanes` columns from `jcol`: C's elements loaded, every k added in k order, stored
def private f32_cols(var e : F32Emit; var ty : LLVMOpaqueType?; lanes, ncv : int; var jcol : LLVMOpaqueValue?; tag : string) {
    let b = e.b
    var ptrs, accs : array<LLVMOpaqueValue?>
    ptrs |> reserve(e.mr * ncv)
    accs |> reserve(e.mr * ncv)
    for (r in range(e.mr)) {
        var row = r == 0 ? e.i0 : LLVMBuildAdd(b, e.i0, f32_ci64(e, int64(r)), "")
        var cat = LLVMBuildAdd(b, LLVMBuildMul(b, row, e.cs, ""), jcol, "c.at")
        for (v in range(ncv)) {
            var p = LLVMBuildGEP2(b, e.types.t_float, e.cp, v == 0 ? cat : LLVMBuildAdd(b, cat, f32_ci64(e, int64(v * lanes)), ""), "")
            ptrs |> push(p)
            accs |> push(LLVMBuildLoad2Aligned(b, ty, p, 4u, "c"))
        }
    }
    let kstep = e.kstep
    var kk = f32_loop(e, f32_ci64(e, 0l), e.kn, int64(kstep), "{tag}.k", ty, accs) $(var k0; var cur) {
        for (u in range(kstep)) {
            f32_k_step(e, ty, lanes, ncv, jcol, u == 0 ? k0 : LLVMBuildAdd(e.b, k0, f32_ci64(e, int64(u)), ""), cur)
        }
    }
    if (kstep > 1) {
        f32_loop(e, kk, e.kn, 1l, "{tag}.kt", ty, accs) $(var k0; var cur) {
            f32_k_step(e, ty, lanes, ncv, jcol, k0, cur)
        }
    }
    for (p, a in ptrs, accs) {
        LLVMBuildStoreAligned(b, a, p, 4u)
    }
}

//! the column walk of one row block: whole tiles of `nv` vectors, single vectors, each narrower vector down to four
//! lanes, then columns one at a time - so any column count takes the same ordered sum an output
def private f32_col_walk(var e : F32Emit; lanes, nv : int; var j0, jend : LLVMOpaqueValue?) {
    var none : array<LLVMOpaqueValue?>
    var vty = LLVMVectorType(e.types.t_float, uint(lanes))
    var j = j0
    if (nv > 1) {
        j = f32_loop(e, j, jend, int64(nv * lanes), "tile", vty, none) $(var jc; var _accs) {
            f32_cols(e, vty, lanes, nv, jc, "tile")
        }
    }
    var l = lanes
    while (l >= 4) {
        var lty = LLVMVectorType(e.types.t_float, uint(l))
        let ll = l
        j = f32_loop(e, j, jend, int64(l), "v{l}", lty, none) $(var jc; var _accs) {
            f32_cols(e, lty, ll, 1, jc, "v{ll}")
        }
        l /= 2
    }
    f32_loop(e, j, jend, 1l, "col", e.types.t_float, none) $(var jc; var _accs) {
        f32_cols(e, e.types.t_float, 1, 1, jc, "col")
    }
}

//! `(cp, ap, bp, i0, j0, nc, kn, cs, astride, bs) : void` - three pointers, seven int64
def private is_f32_tile_signature(fn : Function const?) : bool {
    return false if (length(fn.arguments) != 10 || fn.result.baseType != Type.tVoid)
    for (i in range(3)) {
        return false if (fn.arguments[i]._type.baseType != Type.tPointer)
    }
    for (i in range(3, 10)) {
        return false if (fn.arguments[i]._type.baseType != Type.tInt64)
    }
    return true
}

//! C[mr rows from i0][nc columns from j0] += A x B over `kn` k; `rows` overrides the perm's mr
[arch(at = "../ARCHITECTURE_CPU_KERNELS_TILES.md#f32-gemm-tile-family")]
def private emit_f32_tile(var gc : LlvmCodeCtx; rows : int) : bool {
    if (!is_f32_tile_signature(gc.fn)) return false
    let p = parse_f32_perm(gc)
    if (f32_perm_declines(gc, p)) return false
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    LLVMPositionBuilderAtEnd(gc.jit.builder, entry)
    var e = F32Emit(b = gc.jit.builder, ctx = gc.jit.ctx, impl = gc.impl, types = gc.jit.types,
        cp = LLVMGetParam(gc.impl, 0u), ap = LLVMGetParam(gc.impl, 1u), bp = LLVMGetParam(gc.impl, 2u),
        i0 = LLVMGetParam(gc.impl, 3u), kn = LLVMGetParam(gc.impl, 6u), cs = LLVMGetParam(gc.impl, 7u),
        astride = LLVMGetParam(gc.impl, 8u), bs = LLVMGetParam(gc.impl, 9u),
        mr = rows > 0 ? rows : p.mr, kstep = p.kstep)
    var j0 = LLVMGetParam(gc.impl, 4u)
    var jend = LLVMBuildAdd(e.b, j0, LLVMGetParam(gc.impl, 5u), "jend")
    f32_col_walk(e, p.width / 32, rows > 0 ? min(8, 16 / rows) : p.nv, j0, jend)
    LLVMBuildRetVoid(e.b)
    return true
}

def private f32_tile_gen(var gc : LlvmCodeCtx) : bool => emit_f32_tile(gc, 0)

//! the row companion: one row of the same width, eight vectors abreast, for the rows past the last whole tile
def private f32_row_gen(var gc : LlvmCodeCtx) : bool => emit_f32_tile(gc, 1)

//! a companion `() : int` answering `value(perm)` where the tile emits
def private f32_int_companion(var gc : LlvmCodeCtx; value : block<(p : F32Perm) : int>) : bool {
    if (!empty(gc.fn.arguments) || gc.fn.result.baseType != Type.tInt) return false
    let p = parse_f32_perm(gc)
    if (f32_perm_declines(gc, p)) return false
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    LLVMPositionBuilderAtEnd(gc.jit.builder, entry)
    LLVMBuildRet(gc.jit.builder, gc.jit.types->ConstI32(uint64(invoke(value, p))))
    return true
}

//! the tile's rows, 0 from the reference body: the walk reads it to step its row blocks and to tell a stamped family
def private f32_mr_gen(var gc : LlvmCodeCtx) : bool => f32_int_companion(gc) $(p) => p.mr

//! the full tile's columns
def private f32_cols_gen(var gc : LlvmCodeCtx) : bool => f32_int_companion(gc) $(p) => p.nv * (p.width / 32)

//! 1 where the target converts halves in hardware, whatever the perm; the reference body answers 0
def private half_converts_gen(var gc : LlvmCodeCtx) : bool {
    if (!empty(gc.fn.arguments) || gc.fn.result.baseType != Type.tInt || !target_has_half_converts(gc.jit.target)) return false
    var entry = LLVMAppendBasicBlockInContext(gc.jit.ctx, gc.impl, "entry")
    LLVMPositionBuilderAtEnd(gc.jit.builder, entry)
    LLVMBuildRet(gc.jit.builder, gc.jit.types->ConstI32(1ul))
    return true
}

[macro_function]
def public register_dasllama_f32_gemm_generators {
    register_llvm_code_generator("dasllama_gemm_gen::f32_tile", @@f32_tile_gen)
    register_llvm_code_generator("dasllama_gemm_gen::f32_row", @@f32_row_gen)
    register_llvm_code_generator("dasllama_gemm_gen::f32_mr", @@f32_mr_gen)
    register_llvm_code_generator("dasllama_gemm_gen::f32_cols", @@f32_cols_gen)
    register_llvm_code_generator("dasllama_gemm_gen::half_converts", @@half_converts_gen)
}
