options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_classes_front shared public

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

//! the audio fronts' im2col of the k3 s2 p1 conv2d as the f16 tile's feed, on the template both homes stamp (`GkIm2col2d`)
[cuda_dispatch(name = "tower_im2col_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerIm2col : GkIm2col2d {
    typedef OT = float16

    [cuda_kernel(local_size_x = 256, name = "tower_im2col_cls_cuda")]
    def run {
        im2col2d_body()
    }
}

//! the 1-D conv's im2col as the f16 tile's feed, four columns an invocation, on the template both homes stamp (`GkIm2col1d`)
[cuda_dispatch(name = "tts_im2col16_cls", kernel = "run", grid = "total/1024", params = "total : int64")]
class CuTtsIm2col16 : GkIm2col1d {
    override HALF = true

    [cuda_kernel(local_size_x = 256, name = "tts_im2col16_cls_cuda")]
    def run {
        im2col1d_body()
    }
}
//! the qwen3a feature rows as the f16 tile's feed, on the template both homes stamp (`GkQ3aShuffle`)
[cuda_dispatch(name = "tower_q3a_shuffle_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerQ3aShuffle : GkQ3aShuffle {
    typedef OT = float16

    [cuda_kernel(local_size_x = 256, name = "tower_q3a_shuffle_cls_cuda")]
    def run {
        q3a_shuffle_body()
    }
}
//! the windowed frames' spectrum, the magnitude or the power, on the template both discrete homes stamp (`TowerSpecDftT`)
[cuda_dispatch(name = "tower_spec_dft_cls", grid = "wgs", params = "wgs : int64")]
class CuTowerSpecDft : TowerSpecDftT {
    [cuda_kernel(local_size_x = 256, name = "tower_spec_dft_cls_cuda")]
    def run {
        spec_dft_body()
    }
}

//! the log filterbank in its floor laws, on the template both homes stamp (`GkMelLog`)
[cuda_dispatch(name = "tower_mel_log_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerMelLog : GkMelLog {
    [cuda_kernel(local_size_x = 256, name = "tower_mel_log_cls_cuda")]
    def run {
        mel_log_body()
    }
}

//! canary's per-feature mel normalization, on the template both homes stamp (`GkMelNorm`)
[cuda_dispatch(name = "tower_mel_norm_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerMelNorm : GkMelNorm {
    [cuda_kernel(local_size_x = 64, name = "tower_mel_norm_cls_cuda")]
    def run {
        mel_norm_body()
    }
}

//! gigaam's windowed frames off the raw samples, on the template both homes stamp (`GkFrameWindow`)
[cuda_dispatch(name = "tower_ga_frames_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerGaFrames : GkFrameWindow {
    [cuda_kernel(local_size_x = 256, name = "tower_ga_frames_cls_cuda")]
    def run {
        frame_window_body()
    }
}

//! canary's subsample depthwise conv2d k3 s2 p1 with its bias, on the template both homes stamp (`GkDwConv2d`)
[cuda_dispatch(name = "tower_dw_conv_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerDwConv : GkDwConv2d {
    [cuda_kernel(local_size_x = 256, name = "tower_dw_conv_cls_cuda")]
    def run {
        dw_conv2d_body()
    }
}

//! parakeet's subsample depthwise conv2d over tap-major taps, on the template both homes stamp (`GkDwConv2d`)
[cuda_dispatch(name = "tower_pk_dw2d_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerPkDwConv2d : GkDwConv2d {
    override TAP_MAJOR = true

    [cuda_kernel(local_size_x = 256, name = "tower_pk_dw2d_cls_cuda")]
    def run {
        dw_conv2d_body()
    }
}

//! the subsample stack's feature rows, on the template both homes stamp (`GkFeatShuffle`)
[cuda_dispatch(name = "tower_feat_shuffle_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerFeatShuffle : GkFeatShuffle {
    [cuda_kernel(local_size_x = 256, name = "tower_feat_shuffle_cls_cuda")]
    def run {
        feat_shuffle_body()
    }
}

//! the subsample convs' LayerNorm + ReLU rows, on the template both homes stamp (`GkLnReluRows`)
[cuda_dispatch(name = "tower_ln_relu_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerLnRelu : GkLnReluRows {
    [cuda_kernel(local_size_x = 256, name = "tower_ln_relu_cls_cuda")]
    def run {
        ln_relu_rows_body()
    }
}
