options gen2
options indenting = 4
options _dasllama_internal = true
options loop_unroll_cost = 0    // kernel source: the emitter gets loops and locals as written
options disable_forward

//! The CUDA home's TTS stamps: the kernels the discrete TTS driver (`dasllama_tower_tts`) dispatches, each on the template
//! the Vulkan home stamps under the same dispatch name (`dasllama_gpu_kernels_common`, `dasllama_gpu_kernels_discrete`).
//! A stamp marked `fastmath = false` compiles without NVRTC's fast math (`ARCHITECTURE_GPU_TOWER_TTS.md#tts-st2-seats`).
module dasllama_cuda_classes_tts shared public

require dasllama/dasllama_lint public
require cuda/cuda_kernel public
require daslib/shader_lingua_franca
require spirv/spirv_markers
require dasllama/dasllama_cuda_common public
require dasllama/dasllama_cuda_dispatch
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common public
require dasllama/dasllama_gpu_kernels_discrete public
require daslib/templates
require daslib/typemacro_boost
require math

//! the front end's tanh-form gelu in place, on the template every home stamps (`TtsGeluTanhT`)
[cuda_dispatch(name = "tts_gelu_tanh_cls", kernel = "run", grid = "nelem/1024", params = "nelem : int64")]
class CuTtsGeluTanh : TtsGeluTanhT {
    [cuda_kernel(local_size_x = 256, name = "tts_gelu_tanh_cls_cuda", fastmath = false)]
    def run {
        elem_body()
    }
}

//! the leaky ReLU in place, on the template every home stamps (`TtsLeakyT`)
[cuda_dispatch(name = "tts_leaky_cls", kernel = "run", grid = "nelem/1024", params = "nelem : int64")]
class CuTtsLeaky : TtsLeakyT {
    [cuda_kernel(local_size_x = 256, name = "tts_leaky_cls_cuda")]
    def run {
        elem_body()
    }
}

//! the front end's bidirectional attention, on the template every home stamps (`TtsAttnT`)
[cuda_dispatch(name = "tts_attn_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTtsAttn : TtsAttnT {
    [cuda_kernel(local_size_x = 256, name = "tts_attn_cls_cuda")]
    def run {
        attn_body()
    }
}

//! the 1-D conv's im2col on the template both homes stamp: the f32 columns, the f32 tile's feed (`GkIm2col1d`)
[cuda_dispatch(name = "tts_im2col_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsIm2col : GkIm2col1d {
    [cuda_kernel(local_size_x = 256, name = "tts_im2col_cls_cuda")]
    def run {
        im2col1d_body()
    }
}

//! one LSTM direction in one workgroup, on the template every home stamps (`TtsLstmDirT`)
[cuda_dispatch(name = "tts_lstm_dir_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTtsLstmDir : TtsLstmDirT {
    [cuda_kernel(local_size_x = 1024, name = "tts_lstm_dir_cls_cuda")]
    def run {
        lstm_dir_body()
    }
}

//! ... the recurrence read from its stored Q8_0 plane (`TtsLstmDirT`)
[cuda_dispatch(name = "tts_lstm_dir_q8_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTtsLstmDirQ8 : TtsLstmDirT {
    override WQ8 = true

    [cuda_kernel(local_size_x = 1024, name = "tts_lstm_dir_q8_cls_cuda")]
    def run {
        lstm_dir_body()
    }
}

//! o [rows][width] = cat(x, res, f0, nd) - the CPU `concat_rows` (`GkConcat`)
[cuda_dispatch(name = "tts_concat_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsConcat : GkConcat {
    [cuda_kernel(local_size_x = 256, name = "tts_concat_cls_cuda")]
    def run {
        concat_body()
    }
}

//! the duration sigmoid sums - the CPU `styletts2_durations`' raw durations (`GkSigSum`)
[cuda_dispatch(name = "tts_sigsum_cls", kernel = "run", grid = "t/256", params = "t : int64")]
class CuTtsSigSum : GkSigSum {
    [cuda_kernel(local_size_x = 256, name = "tts_sigsum_cls_cuda")]
    def run {
        sigsum_body()
    }
}

//! the instance norm's column statistics, on the templates every home stamps (`TtsColPartT`, `TtsColReduceT`)
[cuda_dispatch(name = "tts_colpart_sum_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTtsColPartSum : TtsColPartT {
    [cuda_kernel(local_size_x = 256, name = "tts_colpart_sum_cls_cuda")]
    def run {
        colpart_body()
    }
}

//! a stamp of `TtsColPartT`
[cuda_dispatch(name = "tts_colpart_sq_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTtsColPartSq : TtsColPartT {
    override SQ = true

    [cuda_kernel(local_size_x = 256, name = "tts_colpart_sq_cls_cuda")]
    def run {
        colpart_body()
    }
}

//! a stamp of `TtsColReduceT`
[cuda_dispatch(name = "tts_colreduce_mean_cls", kernel = "run", grid = "c/256", params = "c : int64")]
class CuTtsColReduceMean : TtsColReduceT {
    [cuda_kernel(local_size_x = 256, name = "tts_colreduce_mean_cls_cuda")]
    def run {
        colreduce_body()
    }
}

//! a stamp of `TtsColReduceT`
[cuda_dispatch(name = "tts_colreduce_sq_cls", kernel = "run", grid = "c/256", params = "c : int64")]
class CuTtsColReduceSq : TtsColReduceT {
    override SQ = true

    [cuda_kernel(local_size_x = 256, name = "tts_colreduce_sq_cls_cuda")]
    def run {
        colreduce_body()
    }
}

//! AdaIN then LeakyReLU(0.2) / Snake (`GkAdain`)
[cuda_dispatch(name = "tts_adain_leaky_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsAdainLeaky : GkAdain {
    [cuda_kernel(local_size_x = 256, name = "tts_adain_leaky_cls_cuda")]
    def run {
        adain_body()
    }
}

//! a stamp of `GkAdain`
[cuda_dispatch(name = "tts_adain_snake_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsAdainSnake : GkAdain {
    override SNAKE = true

    [cuda_kernel(local_size_x = 256, name = "tts_adain_snake_cls_cuda")]
    def run {
        adain_body()
    }
}

//! the depthwise transposed pool on rows: y [t_out][c] gathers the rows of x [t_in][c] its taps reach - the CPU
//! `conv1d_rows_transposed_depthwise` (`GkDwConvRows`)
[cuda_dispatch(name = "tts_pool_dw_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsPoolDw : GkDwConvRows {
    [cuda_kernel(local_size_x = 256, name = "tts_pool_dw_cls_cuda")]
    def run {
        dw_conv_rows_body()
    }
}

//! the residual join's add-and-scale, in place where o is a (`GkAddScale`)
[cuda_dispatch(name = "tts_add_scale_cls", kernel = "run", grid = "nelem/256", params = "nelem : int64")]
class CuTtsAddScale : GkAddScale {
    [cuda_kernel(local_size_x = 256, name = "tts_add_scale_cls_cuda")]
    def run {
        add_scale_body()
    }
}

//! o = (zero ? 0 : o) + scale * y over nelem elements - the generator stage's averaged residual-block sum (`GkAxpy`)
[cuda_dispatch(name = "tts_axpy_cls", kernel = "run", grid = "nelem/256", params = "nelem : int64")]
class CuTtsAxpy : GkAxpy {
    [cuda_kernel(local_size_x = 256, name = "tts_axpy_cls_cuda")]
    def run {
        axpy_body()
    }
}

//! the low-rate phase increments on either resample law (`GkSrcLow`)
[cuda_dispatch(name = "tts_src_low_torch_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsSrcLowTorch : GkSrcLow {
    override TORCH = true

    [cuda_kernel(local_size_x = 256, name = "tts_src_low_torch_cls_cuda")]
    def run {
        src_low_body()
    }
}

//! a stamp of `GkSrcLow`
[cuda_dispatch(name = "tts_src_low_onnx_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsSrcLowOnnx : GkSrcLow {
    [cuda_kernel(local_size_x = 256, name = "tts_src_low_onnx_cls_cuda")]
    def run {
        src_low_body()
    }
}

//! the phase per frame on the torch law, wgs = 1, its double accumulator a two-float sum (`GkSrcCumsum`)
[cuda_dispatch(name = "tts_src_cumsum_torch_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTtsSrcCumsumTorch : GkSrcCumsum {
    override TORCH = true

    [cuda_kernel(local_size_x = 256, name = "tts_src_cumsum_torch_cls_cuda")]
    def run {
        src_cumsum_body()
    }
}

//! a stamp of `GkSrcCumsum`
[cuda_dispatch(name = "tts_src_cumsum_onnx_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTtsSrcCumsumOnnx : GkSrcCumsum {
    [cuda_kernel(local_size_x = 256, name = "tts_src_cumsum_onnx_cls_cuda")]
    def run {
        src_cumsum_body()
    }
}

//! the source's own noise draw (`GkSrcNoise`)
[cuda_dispatch(name = "tts_src_noise_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsSrcNoise : GkSrcNoise {
    [cuda_kernel(local_size_x = 256, name = "tts_src_noise_cls_cuda", fastmath = false)]
    def run {
        src_noise_body()
    }
}

//! the mixed source signal on either resample law (`GkSrcSines`)
[cuda_dispatch(name = "tts_src_sines_torch_cls", kernel = "run", grid = "n/256", params = "n : int64")]
class CuTtsSrcSinesTorch : GkSrcSines {
    override TORCH = true

    [cuda_kernel(local_size_x = 256, name = "tts_src_sines_torch_cls_cuda", fastmath = false)]
    def run {
        src_sines_body()
    }
}

//! a stamp of `GkSrcSines`
[cuda_dispatch(name = "tts_src_sines_onnx_cls", kernel = "run", grid = "n/256", params = "n : int64")]
class CuTtsSrcSinesOnnx : GkSrcSines {
    [cuda_kernel(local_size_x = 256, name = "tts_src_sines_onnx_cls_cuda", fastmath = false)]
    def run {
        src_sines_body()
    }
}

//! the STFT on the reflect pad law (`GkStft`)
[cuda_dispatch(name = "tts_stft_reflect_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsStftReflect : GkStft {
    override REFLECT = true

    [cuda_kernel(local_size_x = 256, name = "tts_stft_reflect_cls_cuda")]
    def run {
        stft_body()
    }
}

//! a stamp of `GkStft`
[cuda_dispatch(name = "tts_stft_edge_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsStftEdge : GkStft {
    [cuda_kernel(local_size_x = 256, name = "tts_stft_edge_cls_cuda")]
    def run {
        stft_body()
    }
}

//! the inverse STFT from conv_post's rows (`GkIstft`)
[cuda_dispatch(name = "tts_istft_cls", kernel = "run", grid = "n/256", params = "n : int64")]
class CuTtsIstft : GkIstft {
    [cuda_kernel(local_size_x = 256, name = "tts_istft_cls_cuda")]
    def run {
        istft_body()
    }
}

//! a stamp of `GkPkRows`
[cuda_dispatch(name = "tts_pk_rows_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsPkRows : GkPkRows {
    [cuda_kernel(local_size_x = 256, name = "tts_pk_rows_cls_cuda")]
    def run {
        pk_rows_body()
    }
}

//! a stamp of `GkPkRows`
[cuda_dispatch(name = "tts_pk_rows_elu_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsPkRowsElu : GkPkRows {
    override ELU = true

    [cuda_kernel(local_size_x = 256, name = "tts_pk_rows_elu_cls_cuda")]
    def run {
        pk_rows_body()
    }
}

//! x [rows][d] *= the scale row at boff in place - a transformer layer's per-channel layer scale (the CPU
//! `layer_scale_rows`) on the bias stamp's row-broadcast shell: four elements an invocation, d a 4-multiple, `act` unread (`GkBiasAct`)
[cuda_dispatch(name = "tts_pk_row_scale_cls", kernel = "run", grid = "nelem/1024", params = "nelem : int64")]
class CuTtsPkRowScale : GkBiasAct {
    override SCALE = true

    [cuda_kernel(local_size_x = 256, name = "tts_pk_row_scale_cls_cuda")]
    def run {
        bias_act_body()
    }
}

//! the Pocket chains' causal cached attention row - off `WgReduceBase` onto the template both homes stamp (`GkPkAttn`, which
//! derives the same reductions through `GkWgReduce`)
[cuda_dispatch(name = "tts_pk_attn_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTtsPkAttn : GkPkAttn {
    [cuda_kernel(local_size_x = 256, name = "tts_pk_attn_cls_cuda")]
    def run {
        pk_attn_body()
    }
}

//! a stamp of `TtsPkGemvF32T`
[cuda_dispatch(name = "tts_pk_gemv_dot_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvDot : TtsPkGemvF32T {
    [cuda_kernel(local_size_x = 128, name = "tts_pk_gemv_dot_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[cuda_dispatch(name = "tts_pk_gemv_ln_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvLnSilu : TtsPkGemvF32T {
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [cuda_kernel(local_size_x = 128, name = "tts_pk_gemv_ln_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[cuda_dispatch(name = "tts_pk_gemv_gate_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvGate : TtsPkGemvF32T {
    override MOD_IN = true
    override EPI = 2

    [cuda_kernel(local_size_x = 128, name = "tts_pk_gemv_gate_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[cuda_dispatch(name = "tts_pk_gemv_add_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvAddSilu : TtsPkGemvF32T {
    override AUX_IN = true
    override EPI = 3

    [cuda_kernel(local_size_x = 128, name = "tts_pk_gemv_add_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[cuda_dispatch(name = "tts_pk_gemv_tail_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvTail : TtsPkGemvF32T {
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [cuda_kernel(local_size_x = 128, name = "tts_pk_gemv_tail_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[cuda_dispatch(name = "tts_pk_gemv_ln_qkv_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvLnQkv : TtsPkGemvF32T {
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [cuda_kernel(local_size_x = 128, name = "tts_pk_gemv_ln_qkv_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[cuda_dispatch(name = "tts_pk_gemv_add_scale_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvAddScale : TtsPkGemvF32T {
    override AUX_IN = true
    override EPI = 6

    [cuda_kernel(local_size_x = 128, name = "tts_pk_gemv_add_scale_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[cuda_dispatch(name = "tts_pk_gemv_add_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvAdd : TtsPkGemvF32T {
    override EPI = 7

    [cuda_kernel(local_size_x = 128, name = "tts_pk_gemv_add_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[cuda_dispatch(name = "tts_pk_gemv_ln_gelu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvLnGelu : TtsPkGemvF32T {
    override NORM_IN = true
    override EPI = 8

    [cuda_kernel(local_size_x = 128, name = "tts_pk_gemv_ln_gelu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a transposed conv's overlap-add over its lift rows (`TtsFoldT`)
[cuda_dispatch(name = "tts_fold_cls", kernel = "run", grid = "total/256", params = "total : int64")]
class CuTtsFold : TtsFoldT {
    [cuda_kernel(local_size_x = 256, name = "tts_fold_cls_cuda")]
    def run {
        fold_body()
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the plain dot
[cuda_dispatch(name = "q8_tts_pkst_dot_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStDotQ8 : TtsPkGemvStoredT {
    override WFMT = 1
    [cuda_kernel(local_size_x = 128, name = "q8_tts_pkst_dot_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the plain dot
[cuda_dispatch(name = "kq_tts_pkst_k4_dot_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStDotK4 : TtsPkGemvStoredT {
    override WFMT = 2
    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k4_dot_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the plain dot
[cuda_dispatch(name = "kq_tts_pkst_k5_dot_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStDotK5 : TtsPkGemvStoredT {
    override WFMT = 3
    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k5_dot_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the plain dot
[cuda_dispatch(name = "kq_tts_pkst_k6_dot_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStDotK6 : TtsPkGemvStoredT {
    override WFMT = 4
    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k6_dot_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the plain dot
[cuda_dispatch(name = "f16_tts_pkst_dot_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStDotF16 : TtsPkGemvStoredT {
    override WFMT = 5
    [cuda_kernel(local_size_x = 128, name = "f16_tts_pkst_dot_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the normed, modulated input and the SiLU
[cuda_dispatch(name = "q8_tts_pkst_ln_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnSiluQ8 : TtsPkGemvStoredT {
    override WFMT = 1
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [cuda_kernel(local_size_x = 128, name = "q8_tts_pkst_ln_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the normed, modulated input and the SiLU
[cuda_dispatch(name = "kq_tts_pkst_k4_ln_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnSiluK4 : TtsPkGemvStoredT {
    override WFMT = 2
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k4_ln_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the normed, modulated input and the SiLU
[cuda_dispatch(name = "kq_tts_pkst_k5_ln_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnSiluK5 : TtsPkGemvStoredT {
    override WFMT = 3
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k5_ln_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the normed, modulated input and the SiLU
[cuda_dispatch(name = "kq_tts_pkst_k6_ln_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnSiluK6 : TtsPkGemvStoredT {
    override WFMT = 4
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k6_ln_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the normed, modulated input and the SiLU
[cuda_dispatch(name = "f16_tts_pkst_ln_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnSiluF16 : TtsPkGemvStoredT {
    override WFMT = 5
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [cuda_kernel(local_size_x = 128, name = "f16_tts_pkst_ln_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the gated residual
[cuda_dispatch(name = "q8_tts_pkst_gate_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStGateQ8 : TtsPkGemvStoredT {
    override WFMT = 1
    override MOD_IN = true
    override EPI = 2

    [cuda_kernel(local_size_x = 128, name = "q8_tts_pkst_gate_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the gated residual
[cuda_dispatch(name = "kq_tts_pkst_k4_gate_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStGateK4 : TtsPkGemvStoredT {
    override WFMT = 2
    override MOD_IN = true
    override EPI = 2

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k4_gate_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the gated residual
[cuda_dispatch(name = "kq_tts_pkst_k5_gate_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStGateK5 : TtsPkGemvStoredT {
    override WFMT = 3
    override MOD_IN = true
    override EPI = 2

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k5_gate_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the gated residual
[cuda_dispatch(name = "kq_tts_pkst_k6_gate_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStGateK6 : TtsPkGemvStoredT {
    override WFMT = 4
    override MOD_IN = true
    override EPI = 2

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k6_gate_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the gated residual
[cuda_dispatch(name = "f16_tts_pkst_gate_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStGateF16 : TtsPkGemvStoredT {
    override WFMT = 5
    override MOD_IN = true
    override EPI = 2

    [cuda_kernel(local_size_x = 128, name = "f16_tts_pkst_gate_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the add and the SiLU
[cuda_dispatch(name = "q8_tts_pkst_add_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddSiluQ8 : TtsPkGemvStoredT {
    override WFMT = 1
    override AUX_IN = true
    override EPI = 3

    [cuda_kernel(local_size_x = 128, name = "q8_tts_pkst_add_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the add and the SiLU
[cuda_dispatch(name = "kq_tts_pkst_k4_add_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddSiluK4 : TtsPkGemvStoredT {
    override WFMT = 2
    override AUX_IN = true
    override EPI = 3

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k4_add_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the add and the SiLU
[cuda_dispatch(name = "kq_tts_pkst_k5_add_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddSiluK5 : TtsPkGemvStoredT {
    override WFMT = 3
    override AUX_IN = true
    override EPI = 3

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k5_add_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the add and the SiLU
[cuda_dispatch(name = "kq_tts_pkst_k6_add_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddSiluK6 : TtsPkGemvStoredT {
    override WFMT = 4
    override AUX_IN = true
    override EPI = 3

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k6_add_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the add and the SiLU
[cuda_dispatch(name = "f16_tts_pkst_add_silu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddSiluF16 : TtsPkGemvStoredT {
    override WFMT = 5
    override AUX_IN = true
    override EPI = 3

    [cuda_kernel(local_size_x = 128, name = "f16_tts_pkst_add_silu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the frame tail
[cuda_dispatch(name = "q8_tts_pkst_tail_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStTailQ8 : TtsPkGemvStoredT {
    override WFMT = 1
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [cuda_kernel(local_size_x = 128, name = "q8_tts_pkst_tail_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the frame tail
[cuda_dispatch(name = "kq_tts_pkst_k4_tail_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStTailK4 : TtsPkGemvStoredT {
    override WFMT = 2
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k4_tail_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the frame tail
[cuda_dispatch(name = "kq_tts_pkst_k5_tail_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStTailK5 : TtsPkGemvStoredT {
    override WFMT = 3
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k5_tail_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the frame tail
[cuda_dispatch(name = "kq_tts_pkst_k6_tail_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStTailK6 : TtsPkGemvStoredT {
    override WFMT = 4
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k6_tail_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the frame tail
[cuda_dispatch(name = "f16_tts_pkst_tail_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStTailF16 : TtsPkGemvStoredT {
    override WFMT = 5
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [cuda_kernel(local_size_x = 128, name = "f16_tts_pkst_tail_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the normed qkv row, roped and stored
[cuda_dispatch(name = "q8_tts_pkst_ln_qkv_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnQkvQ8 : TtsPkGemvStoredT {
    override WFMT = 1
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [cuda_kernel(local_size_x = 128, name = "q8_tts_pkst_ln_qkv_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the normed qkv row, roped and stored
[cuda_dispatch(name = "kq_tts_pkst_k4_ln_qkv_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnQkvK4 : TtsPkGemvStoredT {
    override WFMT = 2
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k4_ln_qkv_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the normed qkv row, roped and stored
[cuda_dispatch(name = "kq_tts_pkst_k5_ln_qkv_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnQkvK5 : TtsPkGemvStoredT {
    override WFMT = 3
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k5_ln_qkv_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the normed qkv row, roped and stored
[cuda_dispatch(name = "kq_tts_pkst_k6_ln_qkv_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnQkvK6 : TtsPkGemvStoredT {
    override WFMT = 4
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k6_ln_qkv_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the normed qkv row, roped and stored
[cuda_dispatch(name = "f16_tts_pkst_ln_qkv_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnQkvF16 : TtsPkGemvStoredT {
    override WFMT = 5
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [cuda_kernel(local_size_x = 128, name = "f16_tts_pkst_ln_qkv_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the scaled residual add
[cuda_dispatch(name = "q8_tts_pkst_add_scale_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddScaleQ8 : TtsPkGemvStoredT {
    override WFMT = 1
    override AUX_IN = true
    override EPI = 6

    [cuda_kernel(local_size_x = 128, name = "q8_tts_pkst_add_scale_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the scaled residual add
[cuda_dispatch(name = "kq_tts_pkst_k4_add_scale_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddScaleK4 : TtsPkGemvStoredT {
    override WFMT = 2
    override AUX_IN = true
    override EPI = 6

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k4_add_scale_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the scaled residual add
[cuda_dispatch(name = "kq_tts_pkst_k5_add_scale_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddScaleK5 : TtsPkGemvStoredT {
    override WFMT = 3
    override AUX_IN = true
    override EPI = 6

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k5_add_scale_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the scaled residual add
[cuda_dispatch(name = "kq_tts_pkst_k6_add_scale_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddScaleK6 : TtsPkGemvStoredT {
    override WFMT = 4
    override AUX_IN = true
    override EPI = 6

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k6_add_scale_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the scaled residual add
[cuda_dispatch(name = "f16_tts_pkst_add_scale_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddScaleF16 : TtsPkGemvStoredT {
    override WFMT = 5
    override AUX_IN = true
    override EPI = 6

    [cuda_kernel(local_size_x = 128, name = "f16_tts_pkst_add_scale_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the residual add
[cuda_dispatch(name = "q8_tts_pkst_add_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddQ8 : TtsPkGemvStoredT {
    override WFMT = 1
    override EPI = 7

    [cuda_kernel(local_size_x = 128, name = "q8_tts_pkst_add_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the residual add
[cuda_dispatch(name = "kq_tts_pkst_k4_add_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddK4 : TtsPkGemvStoredT {
    override WFMT = 2
    override EPI = 7

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k4_add_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the residual add
[cuda_dispatch(name = "kq_tts_pkst_k5_add_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddK5 : TtsPkGemvStoredT {
    override WFMT = 3
    override EPI = 7

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k5_add_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the residual add
[cuda_dispatch(name = "kq_tts_pkst_k6_add_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddK6 : TtsPkGemvStoredT {
    override WFMT = 4
    override EPI = 7

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k6_add_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the residual add
[cuda_dispatch(name = "f16_tts_pkst_add_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStAddF16 : TtsPkGemvStoredT {
    override WFMT = 5
    override EPI = 7

    [cuda_kernel(local_size_x = 128, name = "f16_tts_pkst_add_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the normed input and the GELU
[cuda_dispatch(name = "q8_tts_pkst_ln_gelu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnGeluQ8 : TtsPkGemvStoredT {
    override WFMT = 1
    override NORM_IN = true
    override EPI = 8

    [cuda_kernel(local_size_x = 128, name = "q8_tts_pkst_ln_gelu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the normed input and the GELU
[cuda_dispatch(name = "kq_tts_pkst_k4_ln_gelu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnGeluK4 : TtsPkGemvStoredT {
    override WFMT = 2
    override NORM_IN = true
    override EPI = 8

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k4_ln_gelu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the normed input and the GELU
[cuda_dispatch(name = "kq_tts_pkst_k5_ln_gelu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnGeluK5 : TtsPkGemvStoredT {
    override WFMT = 3
    override NORM_IN = true
    override EPI = 8

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k5_ln_gelu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the normed input and the GELU
[cuda_dispatch(name = "kq_tts_pkst_k6_ln_gelu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnGeluK6 : TtsPkGemvStoredT {
    override WFMT = 4
    override NORM_IN = true
    override EPI = 8

    [cuda_kernel(local_size_x = 128, name = "kq_tts_pkst_k6_ln_gelu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the normed input and the GELU
[cuda_dispatch(name = "f16_tts_pkst_ln_gelu_cls", kernel = "run", grid = "nout/4", params = "nout : int64")]
class CuTtsPkGemvStLnGeluF16 : TtsPkGemvStoredT {
    override WFMT = 5
    override NORM_IN = true
    override EPI = 8

    [cuda_kernel(local_size_x = 128, name = "f16_tts_pkst_ln_gelu_cls_cuda", fastmath = false)]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! the Pocket chains' adjacent-pair rope over the q and k spans, the tables in a slab or the voice slot (cos_t and sin_t bind one buffer) (`GkRopeTab`)
[cuda_dispatch(name = "tts_pk_rope_cls", kernel = "run", grid = "pairs/256", params = "pairs : int64")]
class CuTtsPkRope : GkRopeTab {
    [cuda_kernel(local_size_x = 256, name = "tts_pk_rope_cls_cuda")]
    def run {
        rope_tab_body()
    }
}
