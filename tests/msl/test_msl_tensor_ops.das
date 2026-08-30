options gen2
options indenting = 4

// Metal-4 tensor-op gate (text layer): the tmm2d surface lowers two ways. The all-device forms
// (tmm2d_f16w_f32, tmm2d_q8u_f32) each become ONE generated preamble helper mangled by
// kind/m/n/sgs — plus the staged K-chunk depth for the q8u family, whose 32-deep helper name is a
// strict PREFIX of the 64- and 128-deep ones, so every name assert here is delimiter-anchored. The staged
// protocol (begin / step / rowscale / store) keys a persistent cooperative tensor to the acc
// local and emits the MPP op inline; the acc's own declaration is suppressed on the GPU.

require dastest/testing_boost public
require _msl_common
require daslib/strings_boost

[test]
def test_tmm2d_device_forms(t : T?) {
    t |> run("f16w: one helper per operand-type pair, picked by the activation pointee") <| @(t : T?) {
        t |> success(contains(tmmhw_msl, "static void __das_tmm2d_hw_8_16_2(device const float * a"), "hw streams the licensed f32 A")
        t |> success(contains(tmmhw_msl, "device const half * wv, uint ldw"), "hw takes a device-half W panel")
        t |> success(contains(tmmhw_msl, "__das_tmm2d_hw_8_16_2(ap, 64u, wp, 64u, cp, 16u, 64u);"), "call site passes the plain buffer pointers")
        t |> success(contains(tmmhwh_msl, "static void __das_tmm2d_hwh_8_16_2(device const half * a"), "hwh (half-A twin) picked by the activation pointee")
        t |> success(!contains(tmmhw_msl, "threadgroup half * wt"), "the f16w form is all-device - no staged W tile, no barriers")
        t |> success(contains(tmmhw_msl, "MetalPerformancePrimitives"), "MPP include present")
        t |> success(contains(tmmhw_msl, "using namespace mpp::tensor_ops;"), "tensor-op namespace present")
        t |> success(find(tmmhw_msl, "static void __das_tmm2d_hw_8_16_2(") < find(tmmhw_msl, "kernel void"), "the helper precedes the kernel")
    }
    t |> run("q8u: staged W tile, bk names the helper") <| @(t : T?) {
        t |> success(contains(tmmq8u_msl, "static void __das_tmm2d_q8u_8_16_2(threadgroup half * wt"), "q8u stages W into a threadgroup half tile")
        t |> success(contains(tmmq8u_msl, "device const char * wq, device const float * a"), "the licensed f32 A stream")
        t |> success(contains(tmmq8u_msl, "matmul2d_descriptor::mode::multiply_accumulate"), "ONE held C across the whole K loop")
        t |> success(contains(tmmq8uh_msl, "static void __das_tmm2d_q8uh_8_16_2(threadgroup half * wt"), "q8uh (half-A twin) picked by the activation pointee")
        t |> success(contains(tmmq8uh_msl, "device const char * wq, device const half * a"), "q8uh streams a device-half A")
    }
    t |> run("q8u bk=64: the _k64 helper is a DISTINCT symbol, not a longer spelling") <| @(t : T?) {
        // the 32-deep name is a prefix of the 64-deep one - both asserts are anchored on the
        // opening paren so neither can pass on the other's text
        t |> success(contains(tmmq8uh_msl, "__das_tmm2d_q8uh_8_16_2("), "bk=32 emits the unsuffixed helper")
        t |> success(!contains(tmmq8uh_msl, "__das_tmm2d_q8uh_8_16_2_k64"), "bk=32 emits NO k64 helper")
        t |> success(contains(tmmq8uh_msl, "const uint nb = kk / 32u;"), "bk=32 walks 32-deep chunks")
        t |> success(contains(tmmq8uh64_msl, "__das_tmm2d_q8uh_8_16_2_k64("), "bk=64 emits the k64 helper")
        t |> success(!contains(tmmq8uh64_msl, "__das_tmm2d_q8uh_8_16_2("), "bk=64 emits NO unsuffixed helper")
        t |> success(contains(tmmq8uh64_msl, "const uint nb = kk / 64u;"), "bk=64 walks 64-deep chunks")
        t |> success(contains(tmmq8uh64_msl, "if ((kk & 32u) != 0u) \{"), "bk=64 finishes a kk % 64 == 32 width with one 32-chunk")
        t |> success(contains(tmmq8uh64_msl, "__das_tmm2d_q8uh_8_16_2_k64(tq64,"), "the call site hands over the n*64 staging tile")
    }
    t |> run("bk=64: staged rows pad to 72 halves, the 32-deep tail to 40") <| @(t : T?) {
        t |> success(contains(tmmq8uh64_msl, "threadgroup half * dstp = wt + col * 72u"), "staging writes one 72-half row per W column")
        t |> success(contains(tmmq8uh64_msl, "metal::array<int32_t, 2>\{1, 72\}"), "the 64-deep B extent carries the 72-half row stride")
        t |> success(contains(tmmq8uh64_msl, "metal::array<int32_t, 2>\{1, 40\}"), "the 32-deep tail extent carries the 40-half row stride")
        t |> success(contains(tmmq8uh64_msl, "threadgroup half * dstp = wt + col * 40u + kh;"), "the tail stages at the 40-half stride it reads back")
    }
    t |> run("bk=128: ping-pong halves, and no preload when nb == 0") <| @(t : T?) {
        t |> success(contains(tmmq8uh128_msl, "__das_tmm2d_q8uh_8_16_2_k128(tq128,"), "bk=128 emits the k128 helper over the n*144 staging tile")
        t |> success(!contains(tmmq8uh128_msl, "__das_tmm2d_q8uh_8_16_2_k64("), "bk=128 emits NO k64 helper")
        // a kk < 64 width has no chunk to preload; without the guard the loop reads W blocks past
        // the panel AND races the tail staging, which writes the same wt bytes with no barrier between
        t |> success(contains(tmmq8uh128_msl, "if (nb > 0u) \{"), "the initial preload is guarded on there being a 64-deep chunk")
        t |> success(contains(tmmq8uh128_msl, "(wt + (b & 1u) * 16u * 72u)"), "each chunk runs off its own ping-pong half")
        t |> success(contains(tmmq8uh128_msl, "threadgroup half * nxt = wt + ((b + 1u) & 1u) * 16u * 72u;"), "chunk b+1 stages into the half chunk b is not consuming")
    }
}

[test]
def test_tmm2d_staged_protocol(t : T?) {
    t |> run("staged protocol: the acc local IS the cooperative tensor") <| @(t : T?) {
        t |> success(!contains(tmmdeva_msl, "float acc["), "the acc array declaration is suppressed on the GPU")
        t |> success(contains(tmmdeva_msl, "auto __tgf_acc = __tgop_acc.template get_destination_cooperative_tensor<"), "the destination tensor is keyed to the acc variable")
        t |> success(contains(tmmdeva_msl, "__tgop_acc.run(__A, __B, __tgf_acc);"), "each step accumulates into the held tensor")
    }
    t |> run("deva: the begin's A witness types the operand tensor") <| @(t : T?) {
        t |> success(contains(tmmdeva_msl, "using __tgta_acc = tensor<device half, dextents<int32_t, 2>, tensor_inline>;"), "an f16 witness types a half A operand")
        t |> success(contains(tmmdeva_msl, "auto __A = __tgta_acc((device half *)"), "the step streams half")
        t |> success(contains(tmmdevaf_msl, "using __tgta_acc = tensor<device float, dextents<int32_t, 2>, tensor_inline>;"), "the licensed f32 witness types a float A operand")
        t |> success(contains(tmmdevaf_msl, "auto __A = __tgta_acc((device float *)"), "the licensed step streams float")
        t |> success(contains(tmmdeva_msl, "matmul2d_descriptor(8, 16, 32, false, true, true, matmul2d_descriptor::mode::multiply_accumulate)"), "deva keeps transpose_right = true")
    }
    t |> run("nt / devant: transpose_right = false, B staged NATURAL [kk][n]") <| @(t : T?) {
        t |> success(contains(tmmnt_msl, "matmul2d_descriptor(8, 16, 32, false, false, true, matmul2d_descriptor::mode::multiply_accumulate)"), "nt flips transpose_right off")
        t |> success(contains(tmmnt_msl, "auto __B = __tgt_acc((threadgroup half *)tgc, dextents<int32_t, 2>(16, 32));"), "the nt B tile carries [kk][n] extents")
        t |> success(contains(tmmdevant_msl, "matmul2d_descriptor(8, 16, static_cast<int>(dynamic_extent), false, false, true, matmul2d_descriptor::mode::multiply_accumulate)"), "devant is dynamic-extent K + natural B")
        t |> success(contains(tmmdevant_msl, "using __tgta_acc = tensor<device float, dextents<int32_t, 2>, tensor_inline>;"), "devant streams a float A by construction")
        t |> success(contains(tmmdevant_msl, "auto __A = __tgta_acc((device float *)"), "devant's step streams from device memory")
    }
    t |> run("rowscale + stores: each lane edits only the elements it owns") <| @(t : T?) {
        t |> success(contains(tmmdeva_msl, "__tgf_acc[__i] *= (trow)[__ids[1]];"), "rowscale multiplies by the per-ROW factor")
        t |> success(contains(tmmdeva_msl, "= half(__tgf_acc[__i]);"), "store_half narrows per owned element")
        t |> success(!contains(tmmdeva_msl, "__tgf_acc.store(__C);"), "the half store bypasses the same-type cooperative store")
        t |> success(contains(tmmdevaf_msl, "__tgf_acc[__i] += (bp)[__ids[0]];"), "store_bias folds the per-COLUMN bias first")
        t |> success(contains(tmmdevaf_msl, "__tgf_acc.store(__C);"), "store_bias still lands through the cooperative store")
    }
    t |> run("tile_tg: a whole tile into THREADGROUP float scratch") <| @(t : T?) {
        t |> success(contains(tmmnt_msl, "constexpr auto __tld = matmul2d_descriptor(8, 16, 32, false, true, true);"), "a fresh overwrite-mode op per call")
        t |> success(contains(tmmnt_msl, "auto __A = __tlt((threadgroup half *)tga, dextents<int32_t, 2>(32, 8));"), "both operands are staged f16 tiles")
        t |> success(contains(tmmnt_msl, "auto __S = tensor<threadgroup float, dextents<int32_t, 2>, tensor_inline>((threadgroup float *)tso"), "the score tile lands in threadgroup float scratch")
        t |> success(contains(tmmnt_msl, "__c.store(__S);"), "the tile result never touches device memory")
    }
}

[test]
def test_sgmat_to_half(t : T?) {
    t |> run("sgmat_to_half: per-lane narrowing through thread_elements()") <| @(t : T?) {
        t |> success(contains(sgmathalf_msl, "for (ushort __sgh = 0; __sgh < 2; ++__sgh) \{ hout.thread_elements()[__sgh] = half(acc.thread_elements()[__sgh]); \}"), "8x8 over 32 lanes = 2 owned elements per lane")
        t |> success(contains(sgmathalf_msl, "simdgroup_float8x8 acc = make_filled_simdgroup_matrix<float, 8, 8>(0.0f);"), "the source accumulator stays f32")
        t |> success(contains(sgmathalf_msl, "simdgroup_store(hout, och + 0u, 8u);"), "the narrowed tile stores same-type")
    }
}
