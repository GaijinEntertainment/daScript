; platform-independent-reduce experiment (M1 milestone) — does generic vector IR select sdot?
;   opt -O3 reduce_experiment.ll -S | llc -O3 -mcpu=apple-m1 -o -            (arm64 leg)
;   opt -O3 reduce_experiment.ll -S | llc -O3 -mtriple=x86_64-unknown-linux-gnu -mcpu=znver4 -o -
; (use the LLVM version dasLLVM pins — 22.1.5; brew llvm matches. opt first: llc alone
; does not run the inliner, dotD's helper stays an outlined call and spills.)
;
; FINDINGS (2026-07-04, LLVM 22.1.5, full table in ../../../../history/dasLLAMA/gemm_generator_plan.md):
;   arm64: A → sdot+addv; B → a SINGLE sdot; D → tight hand-shaped loop, zero spills.
;          C: the lane splat lowers to dup + plain sdot — the INDEXED sdot form only comes
;          from the target intrinsic (see dot_intrin_lane below), costing generic IR one dup
;          per sdot in tile kernels.
;   x64:   A → vpmovsxbw+vpmaddwd+hadd (sane baseline); B → zmm widening + vpmulld +
;          extract chain (unusable). Per-ISA dot emitters stay mandatory on x64.
target triple = "arm64-apple-macosx"

; A) whole-vector scalar reduce: acc += sum(sext(a) * sext(b)) over 16 i8
;    expectation: sdot (zero vec) + addv
define i32 @dotA_scalar_reduce(<16 x i8> %a, <16 x i8> %b, i32 %acc) {
  %sa = sext <16 x i8> %a to <16 x i32>
  %sb = sext <16 x i8> %b to <16 x i32>
  %m  = mul <16 x i32> %sa, %sb
  %r  = call i32 @llvm.vector.reduce.add.v16i32(<16 x i32> %m)
  %o  = add i32 %acc, %r
  ret i32 %o
}

; B) partial reduce into 4-lane accumulator (the laneq accumulator shape)
;    expectation: one sdot, no widening materialized
define <4 x i32> @dotB_partial_reduce(<16 x i8> %a, <16 x i8> %b, <4 x i32> %acc) {
  %sa = sext <16 x i8> %a to <16 x i32>
  %sb = sext <16 x i8> %b to <16 x i32>
  %m  = mul <16 x i32> %sa, %sb
  %r  = call <4 x i32> @llvm.vector.partial.reduce.add.v4i32.v16i32(<4 x i32> %acc, <16 x i32> %m)
  ret <4 x i32> %r
}

; C) lane-splat broadcast of one activation dword-group feeding the partial reduce
;    (the laneq form: same weight vector, activation lane selected per sdot)
;    expectation: indexed sdot v, v, v[lane]
define <4 x i32> @dotC_lane_splat(<16 x i8> %w, <16 x i8> %x, <4 x i32> %acc) {
  %xs = shufflevector <16 x i8> %x, <16 x i8> poison, <16 x i32> <i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7>
  %sw = sext <16 x i8> %w to <16 x i32>
  %sx = sext <16 x i8> %xs to <16 x i32>
  %m  = mul <16 x i32> %sw, %sx
  %r  = call <4 x i32> @llvm.vector.partial.reduce.add.v4i32.v16i32(<4 x i32> %acc, <16 x i32> %m)
  ret <4 x i32> %r
}

; D) block-loop GEMV in fully generic IR: 4-row interleaved group x one token, nb blocks,
;    2 chunks x 4 lanes of partial reduce per block, f32 scale fold at block boundary
;    (dot_q8q8_laneq4 written without a single target intrinsic)
define <4 x float> @dotD_block_loop(ptr %wg, ptr %sg, ptr %xq, ptr %xs, i64 %nb) {
entry:
  br label %loop
loop:
  %bi   = phi i64 [ 0, %entry ], [ %bin, %loop ]
  %facc = phi <4 x float> [ zeroinitializer, %entry ], [ %fma, %loop ]
  %wb = mul i64 %bi, 128
  %xb = mul i64 %bi, 32
  %wp = getelementptr i8, ptr %wg, i64 %wb
  %xp = getelementptr i8, ptr %xq, i64 %xb
  %x0v = load <16 x i8>, ptr %xp, align 1
  %xp1 = getelementptr i8, ptr %xp, i64 16
  %x1v = load <16 x i8>, ptr %xp1, align 1
  %acc0 = call <4 x i32> @dot_group(ptr %wp, <16 x i8> %x0v, <4 x i32> zeroinitializer, i64 0)
  %wp1 = getelementptr i8, ptr %wp, i64 64
  %acc1 = call <4 x i32> @dot_group(ptr %wp1, <16 x i8> %x1v, <4 x i32> %acc0, i64 0)
  ; scale fold: facc += f32(acc) * s4 * splat(xs[bi])
  %sb = mul i64 %bi, 16
  %sp = getelementptr i8, ptr %sg, i64 %sb
  %s4 = load <4 x float>, ptr %sp, align 4
  %xsp = getelementptr float, ptr %xs, i64 %bi
  %xsv = load float, ptr %xsp, align 4
  %xss0 = insertelement <4 x float> poison, float %xsv, i32 0
  %xss = shufflevector <4 x float> %xss0, <4 x float> poison, <4 x i32> zeroinitializer
  %af = sitofp <4 x i32> %acc1 to <4 x float>
  %sf = fmul <4 x float> %s4, %xss
  %fma = call <4 x float> @llvm.fmuladd.v4f32(<4 x float> %af, <4 x float> %sf, <4 x float> %facc)
  %bin = add i64 %bi, 1
  %done = icmp eq i64 %bin, %nb
  br i1 %done, label %exit, label %loop
exit:
  ret <4 x float> %fma
}

; one 64-byte weight chunk (4 lane-groups) against one activation v16i8: 4 lane-splat partial reduces
define internal <4 x i32> @dot_group(ptr %wp, <16 x i8> %x, <4 x i32> %acc, i64 %unused) alwaysinline {
  %w0 = load <16 x i8>, ptr %wp, align 1
  %wp1 = getelementptr i8, ptr %wp, i64 16
  %w1 = load <16 x i8>, ptr %wp1, align 1
  %wp2 = getelementptr i8, ptr %wp, i64 32
  %w2 = load <16 x i8>, ptr %wp2, align 1
  %wp3 = getelementptr i8, ptr %wp, i64 48
  %w3 = load <16 x i8>, ptr %wp3, align 1
  %x0 = shufflevector <16 x i8> %x, <16 x i8> poison, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 0, i32 1, i32 2, i32 3, i32 0, i32 1, i32 2, i32 3, i32 0, i32 1, i32 2, i32 3>
  %x1 = shufflevector <16 x i8> %x, <16 x i8> poison, <16 x i32> <i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7>
  %x2 = shufflevector <16 x i8> %x, <16 x i8> poison, <16 x i32> <i32 8, i32 9, i32 10, i32 11, i32 8, i32 9, i32 10, i32 11, i32 8, i32 9, i32 10, i32 11, i32 8, i32 9, i32 10, i32 11>
  %x3 = shufflevector <16 x i8> %x, <16 x i8> poison, <16 x i32> <i32 12, i32 13, i32 14, i32 15, i32 12, i32 13, i32 14, i32 15, i32 12, i32 13, i32 14, i32 15, i32 12, i32 13, i32 14, i32 15>
  %sw0 = sext <16 x i8> %w0 to <16 x i32>
  %sx0 = sext <16 x i8> %x0 to <16 x i32>
  %m0 = mul <16 x i32> %sw0, %sx0
  %a0 = call <4 x i32> @llvm.vector.partial.reduce.add.v4i32.v16i32(<4 x i32> %acc, <16 x i32> %m0)
  %sw1 = sext <16 x i8> %w1 to <16 x i32>
  %sx1 = sext <16 x i8> %x1 to <16 x i32>
  %m1 = mul <16 x i32> %sw1, %sx1
  %a1 = call <4 x i32> @llvm.vector.partial.reduce.add.v4i32.v16i32(<4 x i32> %a0, <16 x i32> %m1)
  %sw2 = sext <16 x i8> %w2 to <16 x i32>
  %sx2 = sext <16 x i8> %x2 to <16 x i32>
  %m2 = mul <16 x i32> %sw2, %sx2
  %a2 = call <4 x i32> @llvm.vector.partial.reduce.add.v4i32.v16i32(<4 x i32> %a1, <16 x i32> %m2)
  %sw3 = sext <16 x i8> %w3 to <16 x i32>
  %sx3 = sext <16 x i8> %x3 to <16 x i32>
  %m3 = mul <16 x i32> %sw3, %sx3
  %a3 = call <4 x i32> @llvm.vector.partial.reduce.add.v4i32.v16i32(<4 x i32> %a2, <16 x i32> %m3)
  ret <4 x i32> %a3
}

; E) target intrinsic + lane shuffle — the per-ISA dot emitter's arm64 form
;    result: sdot v2, v0, v1[1] — the INDEXED encoding, zero dups (what the hand laneq
;    kernels get today; the partial.reduce forms above never fold the lane)
define <4 x i32> @dot_intrin_lane(<16 x i8> %w, <16 x i8> %x, <4 x i32> %acc) {
  %xs = shufflevector <16 x i8> %x, <16 x i8> poison, <16 x i32> <i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7>
  %r = call <4 x i32> @llvm.aarch64.neon.sdot.v4i32.v16i8(<4 x i32> %acc, <16 x i8> %w, <16 x i8> %xs)
  ret <4 x i32> %r
}

; F) operand-order control for C: splat as FIRST mul operand — still dup + plain sdot
define <4 x i32> @dot_partial_swapped(<16 x i8> %w, <16 x i8> %x, <4 x i32> %acc) {
  %xs = shufflevector <16 x i8> %x, <16 x i8> poison, <16 x i32> <i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7, i32 4, i32 5, i32 6, i32 7>
  %sx = sext <16 x i8> %xs to <16 x i32>
  %sw = sext <16 x i8> %w to <16 x i32>
  %m  = mul <16 x i32> %sx, %sw
  %r  = call <4 x i32> @llvm.vector.partial.reduce.add.v4i32.v16i32(<4 x i32> %acc, <16 x i32> %m)
  ret <4 x i32> %r
}

declare i32 @llvm.vector.reduce.add.v16i32(<16 x i32>)
declare <4 x i32> @llvm.vector.partial.reduce.add.v4i32.v16i32(<4 x i32>, <16 x i32>)
declare <4 x float> @llvm.fmuladd.v4f32(<4 x float>, <4 x float>, <4 x float>)
declare <4 x i32> @llvm.aarch64.neon.sdot.v4i32.v16i8(<4 x i32>, <16 x i8>, <16 x i8>)
