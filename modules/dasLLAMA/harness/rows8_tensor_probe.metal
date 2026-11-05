#include <metal_stdlib>
#include <metal_tensor>
#include <MetalPerformancePrimitives/MetalPerformancePrimitives.h>
using namespace metal;
using namespace mpp::tensor_ops;

template <int MM, int KK, typename LT = uint8_t, int NN = 8>
void probe_k(device int* out, device half* scratch, uint lane) {
    constexpr auto desc = matmul2d_descriptor(MM, NN, KK, false, true, false, matmul2d_descriptor::mode::multiply);
    matmul2d<desc, execution_simdgroup> op;
    auto Lw = op.template get_left_input_cooperative_tensor<LT, half, float>();
    auto tX = tensor<device half, dextents<int32_t, 2>, tensor_inline>(scratch, dextents<int32_t, 2>(KK, NN));
    auto Rx = tX.template slice<KK, NN>(0, 0);
    auto d = op.template get_destination_cooperative_tensor<decltype(Lw), decltype(Rx), float>();
    const uint lc = Lw.get_capacity();
    const uint dc = d.get_capacity();
    out[lane * 2 + 0] = int(lc);
    out[lane * 2 + 1] = int(dc);
    for (uint i = 0; i < lc; ++i) {
        auto mi = Lw.get_multidimensional_index(i);
        out[64 + (lane * 128 + i) * 2 + 0] = int(mi[0]);
        out[64 + (lane * 128 + i) * 2 + 1] = int(mi[1]);
    }
    for (uint i = 0; i < dc; ++i) {
        auto mi = d.get_multidimensional_index(i);
        out[64 + 8192 + (lane * 128 + i) * 2 + 0] = int(mi[0]);
        out[64 + 8192 + (lane * 128 + i) * 2 + 1] = int(mi[1]);
    }
}

kernel void probe128(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 128>(out, scratch, lane);
}
kernel void probe_h_16x128(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 128, half>(out, scratch, lane);
}
kernel void probe_u8_16x16(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 16, uint8_t>(out, scratch, lane);
}
kernel void probe_i8_16x16(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 16, int8_t>(out, scratch, lane);
}
kernel void probe_h_16x16(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 16, half>(out, scratch, lane);
}
kernel void probe32x32(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<32, 32>(out, scratch, lane);
}
kernel void probe_u8_16x32(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 32, uint8_t>(out, scratch, lane);
}
kernel void probe_u8_16x64(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 64, uint8_t>(out, scratch, lane);
}
kernel void probe_h_16x32(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 32, half>(out, scratch, lane);
}
kernel void probe_h_16x64(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 64, half>(out, scratch, lane);
}
kernel void probe_u8_16x64n16(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 64, uint8_t, 16>(out, scratch, lane);
}
kernel void probe_i8_16x32(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 32, int8_t>(out, scratch, lane);
}
kernel void probe_i8_16x64(device int* out [[buffer(0)]], device half* scratch [[buffer(1)]], uint lane [[thread_index_in_simdgroup]]) {
    probe_k<16, 64, int8_t>(out, scratch, lane);
}
