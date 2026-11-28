options gen2
options indenting = 4
options _dasllama_internal = true
options loop_unroll_cost = 0    // kernel source: the emitter gets loops and locals as written
options disable_forward

//! The Metal home's TTS stamps: the kernels the shared TTS driver (`dasllama_tower_tts`) dispatches, each on the template
//! the discrete homes stamp under the same dispatch name (`dasllama_gpu_kernels_common`).
module dasllama_metal_classes_tts shared public

require dasllama/dasllama_lint public
require metal/msl_shader
require metal/das_metal_boost
require daslib/shader_lingua_franca
require dasllama/dasllama_metal_rail public
require dasllama/dasllama_metal_dispatch
require dasllama/dasllama_metal_kernels   // kmask_scales: the Q4_K scale strip's decode
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common public
require daslib/templates
require daslib/typemacro_boost
require math

//! the front end's tanh-form gelu in place, on the template every home stamps (`TtsGeluTanhT`)
[metal_rail_dispatch(name = "tts_gelu_tanh_cls", kernel = "run", grid = "nelem/1024", params = "nelem : int64", tg = 256)]
class MtTtsGeluTanh : TtsGeluTanhT {
    [metal_kernel(name = "tts_gelu_tanh_cls_msl")]
    def run {
        elem_body()
    }
}

//! the leaky ReLU in place, on the template every home stamps (`TtsLeakyT`)
[metal_rail_dispatch(name = "tts_leaky_cls", kernel = "run", grid = "nelem/1024", params = "nelem : int64", tg = 256)]
class MtTtsLeaky : TtsLeakyT {
    [metal_kernel(name = "tts_leaky_cls_msl")]
    def run {
        elem_body()
    }
}

//! the 1-D conv's im2col on the template every home stamps: the f32 columns, the f32 tile's feed (`GkIm2col1d`)
[metal_rail_dispatch(name = "tts_im2col_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsIm2col : GkIm2col1d {
    [metal_kernel(name = "tts_im2col_cls_msl")]
    def run {
        im2col1d_body()
    }
}

//! o [rows][width] = cat(x, res, f0, nd) - the CPU `concat_rows` (`GkConcat`)
[metal_rail_dispatch(name = "tts_concat_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsConcat : GkConcat {
    [metal_kernel(name = "tts_concat_cls_msl")]
    def run {
        concat_body()
    }
}

//! the duration sigmoid sums - the CPU `styletts2_durations`' raw durations (`GkSigSum`)
[metal_rail_dispatch(name = "tts_sigsum_cls", kernel = "run", grid = "t/256", params = "t : int64", tg = 256)]
class MtTtsSigSum : GkSigSum {
    [metal_kernel(name = "tts_sigsum_cls_msl")]
    def run {
        sigsum_body()
    }
}

//! AdaIN then LeakyReLU(0.2) / Snake (`GkAdain`)
[metal_rail_dispatch(name = "tts_adain_leaky_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsAdainLeaky : GkAdain {
    [metal_kernel(name = "tts_adain_leaky_cls_msl")]
    def run {
        adain_body()
    }
}

//! a stamp of `GkAdain`
[metal_rail_dispatch(name = "tts_adain_snake_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsAdainSnake : GkAdain {
    override SNAKE = true

    [metal_kernel(name = "tts_adain_snake_cls_msl")]
    def run {
        adain_body()
    }
}

//! the depthwise transposed pool on rows: y [t_out][c] gathers the rows of x [t_in][c] its taps reach - the CPU
//! `conv1d_rows_transposed_depthwise` (`GkDwConvRows`)
[metal_rail_dispatch(name = "tts_pool_dw_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsPoolDw : GkDwConvRows {
    [metal_kernel(name = "tts_pool_dw_cls_msl")]
    def run {
        dw_conv_rows_body()
    }
}

//! the residual join's add-and-scale, in place where o is a (`GkAddScale`)
[metal_rail_dispatch(name = "tts_add_scale_cls", kernel = "run", grid = "nelem/256", params = "nelem : int64", tg = 256)]
class MtTtsAddScale : GkAddScale {
    [metal_kernel(name = "tts_add_scale_cls_msl")]
    def run {
        add_scale_body()
    }
}

//! o = (zero ? 0 : o) + scale * y over nelem elements - the generator stage's averaged residual-block sum (`GkAxpy`)
[metal_rail_dispatch(name = "tts_axpy_cls", kernel = "run", grid = "nelem/256", params = "nelem : int64", tg = 256)]
class MtTtsAxpy : GkAxpy {
    [metal_kernel(name = "tts_axpy_cls_msl")]
    def run {
        axpy_body()
    }
}

//! the low-rate phase increments on either resample law (`GkSrcLow`)
[metal_rail_dispatch(name = "tts_src_low_torch_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsSrcLowTorch : GkSrcLow {
    override TORCH = true

    [metal_kernel(name = "tts_src_low_torch_cls_msl", fastmath = false)]
    def run {
        src_low_body()
    }
}

//! a stamp of `GkSrcLow`
[metal_rail_dispatch(name = "tts_src_low_onnx_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsSrcLowOnnx : GkSrcLow {
    [metal_kernel(name = "tts_src_low_onnx_cls_msl", fastmath = false)]
    def run {
        src_low_body()
    }
}

//! the phase per frame on the torch law, wgs = 1; `precise` keeps the two-float sum's compensation (`GkSrcCumsum`)
[metal_rail_dispatch(name = "tts_src_cumsum_torch_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTtsSrcCumsumTorch : GkSrcCumsum {
    override TORCH = true

    [metal_kernel(name = "tts_src_cumsum_torch_cls_msl", fastmath = false)]
    def run {
        src_cumsum_body()
    }
}

//! a stamp of `GkSrcCumsum`
[metal_rail_dispatch(name = "tts_src_cumsum_onnx_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTtsSrcCumsumOnnx : GkSrcCumsum {
    [metal_kernel(name = "tts_src_cumsum_onnx_cls_msl", fastmath = false)]
    def run {
        src_cumsum_body()
    }
}

//! the source's own noise draw (`GkSrcNoise`)
[metal_rail_dispatch(name = "tts_src_noise_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsSrcNoise : GkSrcNoise {
    [metal_kernel(name = "tts_src_noise_cls_msl")]
    def run {
        src_noise_body()
    }
}

//! the mixed source signal on either resample law; `precise` keeps `tts_sin_big`'s four-piece reduction and the mix's
//! operation order - a driver's inexact algebra (Mesa's NIR) folds the pieces into one 2 pi and lands 0.02 rad off at 7e5 (`GkSrcSines`)
[metal_rail_dispatch(name = "tts_src_sines_torch_cls", kernel = "run", grid = "n/256", params = "n : int64", tg = 256)]
class MtTtsSrcSinesTorch : GkSrcSines {
    override TORCH = true

    [metal_kernel(name = "tts_src_sines_torch_cls_msl", fastmath = false)]
    def run {
        src_sines_body()
    }
}

//! a stamp of `GkSrcSines`
[metal_rail_dispatch(name = "tts_src_sines_onnx_cls", kernel = "run", grid = "n/256", params = "n : int64", tg = 256)]
class MtTtsSrcSinesOnnx : GkSrcSines {
    [metal_kernel(name = "tts_src_sines_onnx_cls_msl", fastmath = false)]
    def run {
        src_sines_body()
    }
}

//! the STFT on the reflect pad law (`GkStft`)
[metal_rail_dispatch(name = "tts_stft_reflect_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsStftReflect : GkStft {
    override REFLECT = true

    [metal_kernel(name = "tts_stft_reflect_cls_msl")]
    def run {
        stft_body()
    }
}

//! a stamp of `GkStft`
[metal_rail_dispatch(name = "tts_stft_edge_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsStftEdge : GkStft {
    [metal_kernel(name = "tts_stft_edge_cls_msl")]
    def run {
        stft_body()
    }
}

//! the inverse STFT from conv_post's rows (`GkIstft`)
[metal_rail_dispatch(name = "tts_istft_cls", kernel = "run", grid = "n/256", params = "n : int64", tg = 256)]
class MtTtsIstft : GkIstft {
    [metal_kernel(name = "tts_istft_cls_msl")]
    def run {
        istft_body()
    }
}

//! a stamp of `GkPkRows`
[metal_rail_dispatch(name = "tts_pk_rows_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsPkRows : GkPkRows {
    [metal_kernel(name = "tts_pk_rows_cls_msl")]
    def run {
        pk_rows_body()
    }
}

//! a stamp of `GkPkRows`
[metal_rail_dispatch(name = "tts_pk_rows_elu_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsPkRowsElu : GkPkRows {
    override ELU = true

    [metal_kernel(name = "tts_pk_rows_elu_cls_msl")]
    def run {
        pk_rows_body()
    }
}

//! x [rows][d] *= the scale row at boff in place - a transformer layer's per-channel layer scale (the CPU
//! `layer_scale_rows`) on the bias stamp's row-broadcast shell: four elements an invocation, d a 4-multiple, `act` unread (`GkBiasAct`)
[metal_rail_dispatch(name = "tts_pk_row_scale_cls", kernel = "run", grid = "nelem/1024", params = "nelem : int64", tg = 256)]
class MtTtsPkRowScale : GkBiasAct {
    override SCALE = true

    [metal_kernel(name = "tts_pk_row_scale_cls_msl")]
    def run {
        bias_act_body()
    }
}

//! the Pocket chains' causal cached attention row - off `WgReduceBase` onto the template every home stamps (`GkPkAttn`, which
//! derives the same reductions through `GkWgReduce`)
[metal_rail_dispatch(name = "tts_pk_attn_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTtsPkAttn : GkPkAttn {
    [metal_kernel(name = "tts_pk_attn_cls_msl")]
    def run {
        pk_attn_body()
    }
}

//! the Pocket chains' adjacent-pair rope over the q and k spans, the tables in a slab or the voice slot (cos_t and sin_t bind one buffer) (`GkRopeTab`)
[metal_rail_dispatch(name = "tts_pk_rope_cls", kernel = "run", grid = "pairs/256", params = "pairs : int64", tg = 256)]
class MtTtsPkRope : GkRopeTab {
    [metal_kernel(name = "tts_pk_rope_cls_msl")]
    def run {
        rope_tab_body()
    }
}

//! the window-sort row gather of qwen25v's stem on the template every home stamps (`GkRowGather`)
[metal_rail_dispatch(name = "tts_row_gather_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsRowGather : GkRowGather {
    [metal_kernel(name = "tts_row_gather_cls_msl")]
    def run {
        row_gather_body()
    }
}

//! the front end's bidirectional attention, on the template every home stamps (`TtsAttnT`)
[metal_rail_dispatch(name = "tts_attn_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTtsAttn : TtsAttnT {
    [metal_kernel(name = "tts_attn_cls_msl")]
    def run {
        attn_body()
    }
}

//! the instance norm's column statistics, on the templates every home stamps (`TtsColPartT`, `TtsColReduceT`)
[metal_rail_dispatch(name = "tts_colpart_sum_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTtsColPartSum : TtsColPartT {
    [metal_kernel(name = "tts_colpart_sum_cls_msl")]
    def run {
        colpart_body()
    }
}

//! a stamp of `TtsColPartT`
[metal_rail_dispatch(name = "tts_colpart_sq_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTtsColPartSq : TtsColPartT {
    override SQ = true

    [metal_kernel(name = "tts_colpart_sq_cls_msl")]
    def run {
        colpart_body()
    }
}

//! a stamp of `TtsColReduceT`
[metal_rail_dispatch(name = "tts_colreduce_mean_cls", kernel = "run", grid = "c/256", params = "c : int64", tg = 256)]
class MtTtsColReduceMean : TtsColReduceT {
    [metal_kernel(name = "tts_colreduce_mean_cls_msl")]
    def run {
        colreduce_body()
    }
}

//! a stamp of `TtsColReduceT`
[metal_rail_dispatch(name = "tts_colreduce_sq_cls", kernel = "run", grid = "c/256", params = "c : int64", tg = 256)]
class MtTtsColReduceSq : TtsColReduceT {
    override SQ = true

    [metal_kernel(name = "tts_colreduce_sq_cls_msl")]
    def run {
        colreduce_body()
    }
}

//! a transposed conv's overlap-add over its lift rows (`TtsFoldT`)
[metal_rail_dispatch(name = "tts_fold_cls", kernel = "run", grid = "total/256", params = "total : int64", tg = 256)]
class MtTtsFold : TtsFoldT {
    [metal_kernel(name = "tts_fold_cls_msl")]
    def run {
        fold_body()
    }
}

//! one LSTM direction in one workgroup, on the template every home stamps (`TtsLstmDirT`)
[metal_rail_dispatch(name = "tts_lstm_dir_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 1024)]
class MtTtsLstmDir : TtsLstmDirT {
    [metal_kernel(name = "tts_lstm_dir_cls_msl")]
    def run {
        lstm_dir_body()
    }
}

//! the Pocket frame's row GEMV stamps take sixteen rows a threadgroup on this home: x is staged, and normed, once for them all (`TtsPkGemvF32T`)
[metal_rail_dispatch(name = "tts_pk_gemv_dot_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvDot : TtsPkGemvF32T {
    override ROWS = 16
    override RSQRT = true
    override QUAD = false
    [metal_kernel(name = "tts_pk_gemv_dot_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[metal_rail_dispatch(name = "tts_pk_gemv_ln_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvLnSilu : TtsPkGemvF32T {
    override ROWS = 16
    override RSQRT = true
    override QUAD = false
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [metal_kernel(name = "tts_pk_gemv_ln_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[metal_rail_dispatch(name = "tts_pk_gemv_gate_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvGate : TtsPkGemvF32T {
    override ROWS = 16
    override RSQRT = true
    override QUAD = false
    override MOD_IN = true
    override EPI = 2

    [metal_kernel(name = "tts_pk_gemv_gate_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[metal_rail_dispatch(name = "tts_pk_gemv_add_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvAddSilu : TtsPkGemvF32T {
    override ROWS = 16
    override RSQRT = true
    override QUAD = false
    override AUX_IN = true
    override EPI = 3

    [metal_kernel(name = "tts_pk_gemv_add_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[metal_rail_dispatch(name = "tts_pk_gemv_tail_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvTail : TtsPkGemvF32T {
    override ROWS = 16
    override RSQRT = true
    override QUAD = false
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [metal_kernel(name = "tts_pk_gemv_tail_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[metal_rail_dispatch(name = "tts_pk_gemv_ln_qkv_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvLnQkv : TtsPkGemvF32T {
    override ROWS = 16
    override RSQRT = true
    override QUAD = false
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [metal_kernel(name = "tts_pk_gemv_ln_qkv_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[metal_rail_dispatch(name = "tts_pk_gemv_add_scale_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvAddScale : TtsPkGemvF32T {
    override ROWS = 16
    override RSQRT = true
    override QUAD = false
    override AUX_IN = true
    override EPI = 6

    [metal_kernel(name = "tts_pk_gemv_add_scale_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[metal_rail_dispatch(name = "tts_pk_gemv_add_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvAdd : TtsPkGemvF32T {
    override ROWS = 16
    override RSQRT = true
    override QUAD = false
    override EPI = 7

    [metal_kernel(name = "tts_pk_gemv_add_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! a stamp of `TtsPkGemvF32T`
[metal_rail_dispatch(name = "tts_pk_gemv_ln_gelu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvLnGelu : TtsPkGemvF32T {
    override ROWS = 16
    override RSQRT = true
    override QUAD = false
    override NORM_IN = true
    override EPI = 8

    [metal_kernel(name = "tts_pk_gemv_ln_gelu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! layernorm rows alone (`GkPostAddLn`)
[metal_rail_dispatch(name = "tower_ln_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTowerLn : GkPostAddLn {
    override RSQRT = true
    override ADD = false

    [metal_kernel(name = "tower_ln_cls_msl")]
    def run {
        post_add_ln_body()
    }
}

//! the residual seam and the next layernorm, a workgroup a row (`GkPostAddLn`)
[metal_rail_dispatch(name = "tower_post_add_ln_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTowerPostAddLn : GkPostAddLn {
    override RSQRT = true

    [metal_kernel(name = "tower_post_add_ln_cls_msl")]
    def run {
        post_add_ln_body()
    }
}

//! the row-broadcast bias and activation (`GkBiasAct`)
[metal_rail_dispatch(name = "tower_bias_act_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTowerBiasAct : GkBiasAct {
    [metal_kernel(name = "tower_bias_act_cls_msl")]
    def run {
        bias_act_body()
    }
}

//! the 1-D conv's im2col as the f16 tile's feed, four columns an invocation, on the template every home stamps (`GkIm2col1d`)
[metal_rail_dispatch(name = "tts_im2col16_cls", kernel = "run", grid = "total/1024", params = "total : int64", tg = 256)]
class MtTtsIm2col16 : GkIm2col1d {
    override HALF = true

    [metal_kernel(name = "tts_im2col16_cls_msl")]
    def run {
        im2col1d_body()
    }
}

//! layernorm rows alone as the cm2 tile's f16 feed (`GkPostAddLn`)
[metal_rail_dispatch(name = "tower_ln16_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTowerLn16 : GkPostAddLn {
    override RSQRT = true
    override ADD = false
    override OUT16 = true
    override OUT32 = false

    [metal_kernel(name = "tower_ln16_cls_msl")]
    def run {
        post_add_ln_body()
    }
}

//! the seam with the normed rows as the cm2 tile's f16 feed (`GkPostAddLn`)
[metal_rail_dispatch(name = "tower_post_add_ln16_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtTowerPostAddLn16 : GkPostAddLn {
    override RSQRT = true
    override OUT16 = true
    override OUT32 = false

    [metal_kernel(name = "tower_post_add_ln16_cls_msl")]
    def run {
        post_add_ln_body()
    }
}

//! the f32 tile on the template every home stamps (`F32GemmT`): the vision stems, tails and exact lanes
[metal_rail_dispatch(name = "f32_gemm_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtF32Gemm : F32GemmT {
    [metal_kernel(name = "f32_gemm_cls_msl")]
    def run {
        f32_gemm_body()
    }
}

//! the f32 tile with the bias row added at the store (`F32GemmT`)
[metal_rail_dispatch(name = "f32_gemm_b_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtF32GemmB : F32GemmT {
    override BIAS = true

    [metal_kernel(name = "f32_gemm_b_cls_msl")]
    def run {
        f32_gemm_body()
    }
}

//! the f32 tile over the file's Q8_0 blocks as stored (`F32GemmT`'s stored member), the bias row added where the args name one
[metal_rail_dispatch(name = "q8_f32_gemm_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtF32GemmQ8 : F32GemmT {
    override BIAS = true
    override WSTORED = true
    override WQ8 = true
    override WWORDS = true
    override WSC = true

    [metal_kernel(name = "q8_f32_gemm_cls_msl")]
    def run {
        f32_gemm_body()
    }
}

//! the f32 tile over the file's Q4_K superblocks as stored (`F32GemmT`'s stored member), the bias row added where the args name one
[metal_rail_dispatch(name = "kq_f32_gemm_k4_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtF32GemmK4 : F32GemmT {
    override BIAS = true
    override WSTORED = true
    override WK4 = true
    override WWORDS = true
    override WQ4 = true
    override WSC = true

    [metal_kernel(name = "kq_f32_gemm_k4_cls_msl")]
    def run {
        f32_gemm_body()
    }
}

//! the f32 tile over the file's Q5_K superblocks as stored (`F32GemmT`'s stored member), the bias row added where the args name one
[metal_rail_dispatch(name = "kq_f32_gemm_k5_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtF32GemmK5 : F32GemmT {
    override BIAS = true
    override WSTORED = true
    override WK5 = true
    override WWORDS = true
    override WQ4 = true
    override WSC = true

    [metal_kernel(name = "kq_f32_gemm_k5_cls_msl")]
    def run {
        f32_gemm_body()
    }
}

//! the f32 tile over the file's Q6_K superblocks as stored (`F32GemmT`'s stored member), the bias row added where the args name one
[metal_rail_dispatch(name = "kq_f32_gemm_k6_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtF32GemmK6 : F32GemmT {
    override BIAS = true
    override WSTORED = true
    override WK6 = true
    override WWORDS = true
    override WQ4 = true
    override WSC = true

    [metal_kernel(name = "kq_f32_gemm_k6_cls_msl")]
    def run {
        f32_gemm_body()
    }
}

//! the f32 tile over the file's half floats as stored (`F32GemmT`'s stored member), the bias row added where the args name one
[metal_rail_dispatch(name = "f16_f32_gemm_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 256)]
class MtF32GemmF16 : F32GemmT {
    override BIAS = true
    override WSTORED = true
    override WF16 = true
    override WWORDS = true

    [metal_kernel(name = "f16_f32_gemm_cls_msl")]
    def run {
        f32_gemm_body()
    }
}

//! ... the recurrence read from its stored Q8_0 plane (`TtsLstmDirT`)
[metal_rail_dispatch(name = "tts_lstm_dir_q8_cls", kernel = "run", grid = "wgs", params = "wgs : int64", tg = 1024)]
class MtTtsLstmDirQ8 : TtsLstmDirT {
    override WQ8 = true

    [metal_kernel(name = "tts_lstm_dir_q8_cls_msl")]
    def run {
        lstm_dir_body()
    }
}

//! The Pocket frame's stored row GEMV on this home (`TtsPkGemvStoredT`): beside the shared layout's members it reads Q8_0
//! and Q4_K rows in the Metal layout - 34-byte Q8_0 blocks, the scale half ahead of the quants; a Q4_K superblock's quants in
//! disk order with its 16-byte scale strip - as the home's decode GEMVs read them, so the prompt's rows and the frame's GEMV
//! share one plane
[ |> template_struct_instance]
class template MtPkGemvStoredT : TtsPkGemvStoredT {
    @template_constant OWN : int = 0   //!< the member in this home's layout, spelled as its literal: 0 none (the shared layout's `WFMT`), 1 Q8_0, 2 Q4_K
    @ssbo @binding = 9 @role = "alias" swqh : array<float16>   //! the quant plane as halves: a Q8_0 block's scale
    @ssbo @binding = 9 @role = "alias" swqb : array<int8>      //! ... as bytes: its quants

    //! Q8_0 blocks: a lane four quants of a block, the 32 lanes eight blocks a step
    def own_q8_dot_into(row, lane : uint; var acc : float&) {
        let nb4 = pa.nin / 4u
        let b0 = pa.woff + row * (pa.nin / 32u) * 34u
        var a = acc
        var k = lane
        while (k < nb4) {
            let base = b0 + (k / 8u) * 34u
            let d = float(swqh[base / 2u])
            let qb = base + 2u + (k % 8u) * 4u
            let xb = k * 4u
            a += (float(swqb[qb]) * xs[xb] + float(swqb[qb + 1u]) * xs[xb + 1u] + float(swqb[qb + 2u]) * xs[xb + 2u] + float(swqb[qb + 3u]) * xs[xb + 3u]) * d
            k += 32u
        }
        acc = a
    }

    //! Q4_K superblocks: a lane eight elements of each of four sub-blocks - a quadrant's pair and the pair half a superblock on -
    //! and every fourth superblock, the scale strip decoded a quadrant at a time (`kmask_scales`), the mins folded over x's sums
    def own_k4_dot_into(row, lane : uint; var acc : float&) {
        let ix = lane / 8u
        let iq = (lane % 8u) / 4u
        let ir = lane % 4u
        let nb = pa.nin / 256u
        let q0 = pa.woff / 4u
        let s0 = pa.wsoff / 4u
        var a = acc
        var ib = ix
        while (ib < nb) {
            let blk = row * nb + ib
            let sw = s0 + blk * 4u
            let sv = uint4(sws[sw], sws[sw + 1u], sws[sw + 2u], sws[sw + 3u])
            let scs = kmask_scales(sv, iq)
            let dm = unpackHalf2x16(sv.x)
            let qb = q0 + blk * 32u + iq * 8u + ir * 2u
            let q1a = swq[qb]
            let q1b = swq[qb + 1u]
            let q2a = swq[qb + 16u]
            let q2b = swq[qb + 17u]
            let xb = ib * 256u + iq * 64u + ir * 8u
            var a10 = 0.0
            var a11 = 0.0
            var a12 = 0.0
            var a13 = 0.0
            var a20 = 0.0
            var a21 = 0.0
            var a22 = 0.0
            var a23 = 0.0
            var sy0 = 0.0
            var sy1 = 0.0
            var sy2 = 0.0
            var sy3 = 0.0
            for (i in range(4)) {
                let ui = uint(i)
                let sh = 16u * (ui & 1u)
                let q1 = ((i < 2 ? q1a : q1b) >> sh) & 0xFFFFu
                let q2 = ((i < 2 ? q2a : q2b) >> sh) & 0xFFFFu
                let e0 = xs[xb + 2u * ui]
                let e1 = xs[xb + 2u * ui + 1u]
                let e2 = xs[xb + 2u * ui + 32u]
                let e3 = xs[xb + 2u * ui + 33u]
                let e4 = xs[xb + 2u * ui + 128u]
                let e5 = xs[xb + 2u * ui + 129u]
                let e6 = xs[xb + 2u * ui + 160u]
                let e7 = xs[xb + 2u * ui + 161u]
                a10 += e0 * float(q1 & 0x000Fu)
                a11 += e1 * float(q1 & 0x0F00u)
                a12 += e2 * float(q1 & 0x00F0u)
                a13 += e3 * float(q1 & 0xF000u)
                a20 += e4 * float(q2 & 0x000Fu)
                a21 += e5 * float(q2 & 0x0F00u)
                a22 += e6 * float(q2 & 0x00F0u)
                a23 += e7 * float(q2 & 0xF000u)
                sy0 += e0 + e1
                sy1 += e2 + e3
                sy2 += e4 + e5
                sy3 += e6 + e7
            }
            a += dm.x * ((a10 + 0.00390625 * a11) * float(scs.x & 255u) + (a12 + 0.00390625 * a13) * float(scs.x >> 8u) * 0.0625
                + (a20 + 0.00390625 * a21) * float(scs.z & 255u) + (a22 + 0.00390625 * a23) * float(scs.z >> 8u) * 0.0625)
            a -= dm.y * (sy0 * float(scs.y & 255u) + sy1 * float(scs.y >> 8u) + sy2 * float(scs.w & 255u) + sy3 * float(scs.w >> 8u))
            ib += 4u
        }
        acc = a
    }

    def override row_dot_into(row, lane : uint; var acc : float&) {
        static_if (OWN == 2) {
            own_k4_dot_into(row, lane, acc)
        } else {
            static_if (OWN == 1) {
                own_q8_dot_into(row, lane, acc)
            } else {
                stored_dot_into(row, lane, acc)
            }
        }
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the plain dot
[metal_rail_dispatch(name = "q8_tts_pkst_dot_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStDotQ8 : MtPkGemvStoredT {
    override WFMT = 1
    override ROWS = 16
    override RSQRT = true
    [metal_kernel(name = "q8_tts_pkst_dot_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the plain dot
[metal_rail_dispatch(name = "kq_tts_pkst_k4_dot_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStDotK4 : MtPkGemvStoredT {
    override WFMT = 2
    override ROWS = 16
    override RSQRT = true
    [metal_kernel(name = "kq_tts_pkst_k4_dot_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the plain dot
[metal_rail_dispatch(name = "kq_tts_pkst_k5_dot_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStDotK5 : MtPkGemvStoredT {
    override WFMT = 3
    override ROWS = 16
    override RSQRT = true
    [metal_kernel(name = "kq_tts_pkst_k5_dot_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the plain dot
[metal_rail_dispatch(name = "kq_tts_pkst_k6_dot_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStDotK6 : MtPkGemvStoredT {
    override WFMT = 4
    override ROWS = 16
    override RSQRT = true
    [metal_kernel(name = "kq_tts_pkst_k6_dot_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the plain dot
[metal_rail_dispatch(name = "f16_tts_pkst_dot_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStDotF16 : MtPkGemvStoredT {
    override WFMT = 5
    override ROWS = 16
    override RSQRT = true
    [metal_kernel(name = "f16_tts_pkst_dot_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows in this home's layout: the plain dot
[metal_rail_dispatch(name = "q8_tts_pkown_dot_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStDotOwnQ8 : MtPkGemvStoredT {
    override OWN = 1
    override ROWS = 16
    override RSQRT = true
    [metal_kernel(name = "q8_tts_pkown_dot_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows in this home's layout: the plain dot
[metal_rail_dispatch(name = "kq_tts_pkown_k4_dot_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStDotOwnK4 : MtPkGemvStoredT {
    override OWN = 2
    override ROWS = 16
    override RSQRT = true
    [metal_kernel(name = "kq_tts_pkown_k4_dot_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the normed, modulated input and the SiLU
[metal_rail_dispatch(name = "q8_tts_pkst_ln_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnSiluQ8 : MtPkGemvStoredT {
    override WFMT = 1
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [metal_kernel(name = "q8_tts_pkst_ln_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the normed, modulated input and the SiLU
[metal_rail_dispatch(name = "kq_tts_pkst_k4_ln_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnSiluK4 : MtPkGemvStoredT {
    override WFMT = 2
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [metal_kernel(name = "kq_tts_pkst_k4_ln_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the normed, modulated input and the SiLU
[metal_rail_dispatch(name = "kq_tts_pkst_k5_ln_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnSiluK5 : MtPkGemvStoredT {
    override WFMT = 3
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [metal_kernel(name = "kq_tts_pkst_k5_ln_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the normed, modulated input and the SiLU
[metal_rail_dispatch(name = "kq_tts_pkst_k6_ln_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnSiluK6 : MtPkGemvStoredT {
    override WFMT = 4
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [metal_kernel(name = "kq_tts_pkst_k6_ln_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the normed, modulated input and the SiLU
[metal_rail_dispatch(name = "f16_tts_pkst_ln_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnSiluF16 : MtPkGemvStoredT {
    override WFMT = 5
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [metal_kernel(name = "f16_tts_pkst_ln_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows in this home's layout: the normed, modulated input and the SiLU
[metal_rail_dispatch(name = "q8_tts_pkown_ln_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnSiluOwnQ8 : MtPkGemvStoredT {
    override OWN = 1
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [metal_kernel(name = "q8_tts_pkown_ln_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows in this home's layout: the normed, modulated input and the SiLU
[metal_rail_dispatch(name = "kq_tts_pkown_k4_ln_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnSiluOwnK4 : MtPkGemvStoredT {
    override OWN = 2
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override EPI = 1

    [metal_kernel(name = "kq_tts_pkown_k4_ln_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the gated residual
[metal_rail_dispatch(name = "q8_tts_pkst_gate_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStGateQ8 : MtPkGemvStoredT {
    override WFMT = 1
    override ROWS = 16
    override RSQRT = true
    override MOD_IN = true
    override EPI = 2

    [metal_kernel(name = "q8_tts_pkst_gate_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the gated residual
[metal_rail_dispatch(name = "kq_tts_pkst_k4_gate_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStGateK4 : MtPkGemvStoredT {
    override WFMT = 2
    override ROWS = 16
    override RSQRT = true
    override MOD_IN = true
    override EPI = 2

    [metal_kernel(name = "kq_tts_pkst_k4_gate_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the gated residual
[metal_rail_dispatch(name = "kq_tts_pkst_k5_gate_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStGateK5 : MtPkGemvStoredT {
    override WFMT = 3
    override ROWS = 16
    override RSQRT = true
    override MOD_IN = true
    override EPI = 2

    [metal_kernel(name = "kq_tts_pkst_k5_gate_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the gated residual
[metal_rail_dispatch(name = "kq_tts_pkst_k6_gate_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStGateK6 : MtPkGemvStoredT {
    override WFMT = 4
    override ROWS = 16
    override RSQRT = true
    override MOD_IN = true
    override EPI = 2

    [metal_kernel(name = "kq_tts_pkst_k6_gate_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the gated residual
[metal_rail_dispatch(name = "f16_tts_pkst_gate_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStGateF16 : MtPkGemvStoredT {
    override WFMT = 5
    override ROWS = 16
    override RSQRT = true
    override MOD_IN = true
    override EPI = 2

    [metal_kernel(name = "f16_tts_pkst_gate_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows in this home's layout: the gated residual
[metal_rail_dispatch(name = "q8_tts_pkown_gate_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStGateOwnQ8 : MtPkGemvStoredT {
    override OWN = 1
    override ROWS = 16
    override RSQRT = true
    override MOD_IN = true
    override EPI = 2

    [metal_kernel(name = "q8_tts_pkown_gate_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows in this home's layout: the gated residual
[metal_rail_dispatch(name = "kq_tts_pkown_k4_gate_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStGateOwnK4 : MtPkGemvStoredT {
    override OWN = 2
    override ROWS = 16
    override RSQRT = true
    override MOD_IN = true
    override EPI = 2

    [metal_kernel(name = "kq_tts_pkown_k4_gate_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the add and the SiLU
[metal_rail_dispatch(name = "q8_tts_pkst_add_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddSiluQ8 : MtPkGemvStoredT {
    override WFMT = 1
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 3

    [metal_kernel(name = "q8_tts_pkst_add_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the add and the SiLU
[metal_rail_dispatch(name = "kq_tts_pkst_k4_add_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddSiluK4 : MtPkGemvStoredT {
    override WFMT = 2
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 3

    [metal_kernel(name = "kq_tts_pkst_k4_add_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the add and the SiLU
[metal_rail_dispatch(name = "kq_tts_pkst_k5_add_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddSiluK5 : MtPkGemvStoredT {
    override WFMT = 3
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 3

    [metal_kernel(name = "kq_tts_pkst_k5_add_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the add and the SiLU
[metal_rail_dispatch(name = "kq_tts_pkst_k6_add_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddSiluK6 : MtPkGemvStoredT {
    override WFMT = 4
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 3

    [metal_kernel(name = "kq_tts_pkst_k6_add_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the add and the SiLU
[metal_rail_dispatch(name = "f16_tts_pkst_add_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddSiluF16 : MtPkGemvStoredT {
    override WFMT = 5
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 3

    [metal_kernel(name = "f16_tts_pkst_add_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows in this home's layout: the add and the SiLU
[metal_rail_dispatch(name = "q8_tts_pkown_add_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddSiluOwnQ8 : MtPkGemvStoredT {
    override OWN = 1
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 3

    [metal_kernel(name = "q8_tts_pkown_add_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows in this home's layout: the add and the SiLU
[metal_rail_dispatch(name = "kq_tts_pkown_k4_add_silu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddSiluOwnK4 : MtPkGemvStoredT {
    override OWN = 2
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 3

    [metal_kernel(name = "kq_tts_pkown_k4_add_silu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the frame tail
[metal_rail_dispatch(name = "q8_tts_pkst_tail_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStTailQ8 : MtPkGemvStoredT {
    override WFMT = 1
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [metal_kernel(name = "q8_tts_pkst_tail_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the frame tail
[metal_rail_dispatch(name = "kq_tts_pkst_k4_tail_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStTailK4 : MtPkGemvStoredT {
    override WFMT = 2
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [metal_kernel(name = "kq_tts_pkst_k4_tail_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the frame tail
[metal_rail_dispatch(name = "kq_tts_pkst_k5_tail_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStTailK5 : MtPkGemvStoredT {
    override WFMT = 3
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [metal_kernel(name = "kq_tts_pkst_k5_tail_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the frame tail
[metal_rail_dispatch(name = "kq_tts_pkst_k6_tail_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStTailK6 : MtPkGemvStoredT {
    override WFMT = 4
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [metal_kernel(name = "kq_tts_pkst_k6_tail_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the frame tail
[metal_rail_dispatch(name = "f16_tts_pkst_tail_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStTailF16 : MtPkGemvStoredT {
    override WFMT = 5
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [metal_kernel(name = "f16_tts_pkst_tail_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows in this home's layout: the frame tail
[metal_rail_dispatch(name = "q8_tts_pkown_tail_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStTailOwnQ8 : MtPkGemvStoredT {
    override OWN = 1
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [metal_kernel(name = "q8_tts_pkown_tail_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows in this home's layout: the frame tail
[metal_rail_dispatch(name = "kq_tts_pkown_k4_tail_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStTailOwnK4 : MtPkGemvStoredT {
    override OWN = 2
    override ROWS = 16
    override RSQRT = true
    override NORM_MOD_IN = true
    override MOD_IN = true
    override AUX_IN = true
    override TAIL = true
    override EPI = 4

    [metal_kernel(name = "kq_tts_pkown_k4_tail_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the normed qkv row, roped and stored
[metal_rail_dispatch(name = "q8_tts_pkst_ln_qkv_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnQkvQ8 : MtPkGemvStoredT {
    override WFMT = 1
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [metal_kernel(name = "q8_tts_pkst_ln_qkv_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the normed qkv row, roped and stored
[metal_rail_dispatch(name = "kq_tts_pkst_k4_ln_qkv_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnQkvK4 : MtPkGemvStoredT {
    override WFMT = 2
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [metal_kernel(name = "kq_tts_pkst_k4_ln_qkv_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the normed qkv row, roped and stored
[metal_rail_dispatch(name = "kq_tts_pkst_k5_ln_qkv_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnQkvK5 : MtPkGemvStoredT {
    override WFMT = 3
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [metal_kernel(name = "kq_tts_pkst_k5_ln_qkv_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the normed qkv row, roped and stored
[metal_rail_dispatch(name = "kq_tts_pkst_k6_ln_qkv_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnQkvK6 : MtPkGemvStoredT {
    override WFMT = 4
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [metal_kernel(name = "kq_tts_pkst_k6_ln_qkv_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the normed qkv row, roped and stored
[metal_rail_dispatch(name = "f16_tts_pkst_ln_qkv_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnQkvF16 : MtPkGemvStoredT {
    override WFMT = 5
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [metal_kernel(name = "f16_tts_pkst_ln_qkv_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows in this home's layout: the normed qkv row, roped and stored
[metal_rail_dispatch(name = "q8_tts_pkown_ln_qkv_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnQkvOwnQ8 : MtPkGemvStoredT {
    override OWN = 1
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [metal_kernel(name = "q8_tts_pkown_ln_qkv_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows in this home's layout: the normed qkv row, roped and stored
[metal_rail_dispatch(name = "kq_tts_pkown_k4_ln_qkv_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnQkvOwnK4 : MtPkGemvStoredT {
    override OWN = 2
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override ROPE_STORE = true
    override EPI = 5

    [metal_kernel(name = "kq_tts_pkown_k4_ln_qkv_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the scaled residual add
[metal_rail_dispatch(name = "q8_tts_pkst_add_scale_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddScaleQ8 : MtPkGemvStoredT {
    override WFMT = 1
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 6

    [metal_kernel(name = "q8_tts_pkst_add_scale_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the scaled residual add
[metal_rail_dispatch(name = "kq_tts_pkst_k4_add_scale_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddScaleK4 : MtPkGemvStoredT {
    override WFMT = 2
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 6

    [metal_kernel(name = "kq_tts_pkst_k4_add_scale_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the scaled residual add
[metal_rail_dispatch(name = "kq_tts_pkst_k5_add_scale_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddScaleK5 : MtPkGemvStoredT {
    override WFMT = 3
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 6

    [metal_kernel(name = "kq_tts_pkst_k5_add_scale_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the scaled residual add
[metal_rail_dispatch(name = "kq_tts_pkst_k6_add_scale_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddScaleK6 : MtPkGemvStoredT {
    override WFMT = 4
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 6

    [metal_kernel(name = "kq_tts_pkst_k6_add_scale_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the scaled residual add
[metal_rail_dispatch(name = "f16_tts_pkst_add_scale_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddScaleF16 : MtPkGemvStoredT {
    override WFMT = 5
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 6

    [metal_kernel(name = "f16_tts_pkst_add_scale_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows in this home's layout: the scaled residual add
[metal_rail_dispatch(name = "q8_tts_pkown_add_scale_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddScaleOwnQ8 : MtPkGemvStoredT {
    override OWN = 1
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 6

    [metal_kernel(name = "q8_tts_pkown_add_scale_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows in this home's layout: the scaled residual add
[metal_rail_dispatch(name = "kq_tts_pkown_k4_add_scale_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddScaleOwnK4 : MtPkGemvStoredT {
    override OWN = 2
    override ROWS = 16
    override RSQRT = true
    override AUX_IN = true
    override EPI = 6

    [metal_kernel(name = "kq_tts_pkown_k4_add_scale_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the residual add
[metal_rail_dispatch(name = "q8_tts_pkst_add_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddQ8 : MtPkGemvStoredT {
    override WFMT = 1
    override ROWS = 16
    override RSQRT = true
    override EPI = 7

    [metal_kernel(name = "q8_tts_pkst_add_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the residual add
[metal_rail_dispatch(name = "kq_tts_pkst_k4_add_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddK4 : MtPkGemvStoredT {
    override WFMT = 2
    override ROWS = 16
    override RSQRT = true
    override EPI = 7

    [metal_kernel(name = "kq_tts_pkst_k4_add_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the residual add
[metal_rail_dispatch(name = "kq_tts_pkst_k5_add_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddK5 : MtPkGemvStoredT {
    override WFMT = 3
    override ROWS = 16
    override RSQRT = true
    override EPI = 7

    [metal_kernel(name = "kq_tts_pkst_k5_add_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the residual add
[metal_rail_dispatch(name = "kq_tts_pkst_k6_add_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddK6 : MtPkGemvStoredT {
    override WFMT = 4
    override ROWS = 16
    override RSQRT = true
    override EPI = 7

    [metal_kernel(name = "kq_tts_pkst_k6_add_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the residual add
[metal_rail_dispatch(name = "f16_tts_pkst_add_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddF16 : MtPkGemvStoredT {
    override WFMT = 5
    override ROWS = 16
    override RSQRT = true
    override EPI = 7

    [metal_kernel(name = "f16_tts_pkst_add_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows in this home's layout: the residual add
[metal_rail_dispatch(name = "q8_tts_pkown_add_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddOwnQ8 : MtPkGemvStoredT {
    override OWN = 1
    override ROWS = 16
    override RSQRT = true
    override EPI = 7

    [metal_kernel(name = "q8_tts_pkown_add_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows in this home's layout: the residual add
[metal_rail_dispatch(name = "kq_tts_pkown_k4_add_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStAddOwnK4 : MtPkGemvStoredT {
    override OWN = 2
    override ROWS = 16
    override RSQRT = true
    override EPI = 7

    [metal_kernel(name = "kq_tts_pkown_k4_add_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows: the normed input and the GELU
[metal_rail_dispatch(name = "q8_tts_pkst_ln_gelu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnGeluQ8 : MtPkGemvStoredT {
    override WFMT = 1
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override EPI = 8

    [metal_kernel(name = "q8_tts_pkst_ln_gelu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows: the normed input and the GELU
[metal_rail_dispatch(name = "kq_tts_pkst_k4_ln_gelu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnGeluK4 : MtPkGemvStoredT {
    override WFMT = 2
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override EPI = 8

    [metal_kernel(name = "kq_tts_pkst_k4_ln_gelu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q5_K rows: the normed input and the GELU
[metal_rail_dispatch(name = "kq_tts_pkst_k5_ln_gelu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnGeluK5 : MtPkGemvStoredT {
    override WFMT = 3
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override EPI = 8

    [metal_kernel(name = "kq_tts_pkst_k5_ln_gelu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q6_K rows: the normed input and the GELU
[metal_rail_dispatch(name = "kq_tts_pkst_k6_ln_gelu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnGeluK6 : MtPkGemvStoredT {
    override WFMT = 4
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override EPI = 8

    [metal_kernel(name = "kq_tts_pkst_k6_ln_gelu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over half-float rows: the normed input and the GELU
[metal_rail_dispatch(name = "f16_tts_pkst_ln_gelu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnGeluF16 : MtPkGemvStoredT {
    override WFMT = 5
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override EPI = 8

    [metal_kernel(name = "f16_tts_pkst_ln_gelu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q8_0 rows in this home's layout: the normed input and the GELU
[metal_rail_dispatch(name = "q8_tts_pkown_ln_gelu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnGeluOwnQ8 : MtPkGemvStoredT {
    override OWN = 1
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override EPI = 8

    [metal_kernel(name = "q8_tts_pkown_ln_gelu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

//! `TtsPkGemvStoredT` over Q4_K rows in this home's layout: the normed input and the GELU
[metal_rail_dispatch(name = "kq_tts_pkown_k4_ln_gelu_cls", kernel = "run", grid = "nout/16", params = "nout : int64", tg = 512)]
class MtTtsPkGemvStLnGeluOwnK4 : MtPkGemvStoredT {
    override OWN = 2
    override ROWS = 16
    override RSQRT = true
    override NORM_IN = true
    override EPI = 8

    [metal_kernel(name = "kq_tts_pkown_k4_ln_gelu_cls_msl")]
    def run {
        pk_gemv_body(gl_WorkGroupID.x, gl_LocalInvocationID.x)
    }
}

struct MtZeroArgs {
    nelem : uint   //! the floats zeroed
    off : uint     //! their element base in x
}

//! a run of floats zeroed in place, four an invocation: the plane fill the discrete homes spell as a device command
[metal_rail_dispatch(name = "rail_zero_cls", kernel = "run", grid = "nelem/1024", params = "nelem : int64", tg = 256)]
class MtRailZero {
    @ssbo @binding = 0 @role = "write" x : array<float>
    @kargs pa : MtZeroArgs

    [metal_kernel(name = "rail_zero_cls_msl")]
    def run {
        var i = gl_GlobalInvocationID.x * 4u
        let last = min(i + 4u, pa.nelem)
        while (i < last) {
            x[pa.off + i] = 0.0
            i++
        }
    }
}

//! a run of `bytes` of plane `dst` from byte `off` zeroed, in the order of the rail's dispatches
def rail_zero(var rl : MtRail; dst : TowerBuf; off, bytes : int64) {
    if (!ensure_rail_zero_cls()) {
        panic("dasLLAMA metal rail: the zero stamp did not compile on this device")
    }
    var pa = MtZeroArgs(nelem = uint(bytes / 4l), off = uint(off / 4l))
    enc_rail_zero_cls(rl, fixed_array(dst), pa, bytes / 4l)
}
