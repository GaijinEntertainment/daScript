options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_classes_blocks shared public

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

// The CUDA stamps of the audio towers' block classes and the whisper-class chat tails: each the
// Vulkan stamp of the same dispatch stem, its template, overrides, workgroup size, grid and params
// copied, so one chain body encodes either home with the same args and workgroup count.

//! the Transformer-XL rel-position table (`GkRelSinCos`)
[cuda_dispatch(name = "tower_rel_sincos_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerRelSinCos : GkRelSinCos {
    [cuda_kernel(local_size_x = 256, name = "tower_rel_sincos_cls_cuda")]
    def run {
        rel_sincos_body()
    }
}

//! canary's conv module tail: the centered depthwise conv, the folded BatchNorm and silu (`GkDwConvRows`)
[cuda_dispatch(name = "tower_cn_dw_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerCnDw : GkDwConvRows {
    override FORM = 2

    [cuda_kernel(local_size_x = 256, name = "tower_cn_dw_cls_cuda")]
    def run {
        dw_conv_rows_body()
    }
}

//! parakeet's conv module tail: the same over tap-major taps (`GkDwConvRows`)
[cuda_dispatch(name = "tower_pk_dw_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerPkDw : GkDwConvRows {
    override FORM = 2
    override TAP_MAJOR = true

    [cuda_kernel(local_size_x = 256, name = "tower_pk_dw_cls_cuda")]
    def run {
        dw_conv_rows_body()
    }
}

//! GigaAM's centered depthwise conv over tap-major taps, the bias first and no epilogue (`GkDwConvRows`)
[cuda_dispatch(name = "tower_ga_dw_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerGaDw : GkDwConvRows {
    override FORM = 3
    override TAP_MAJOR = true

    [cuda_kernel(local_size_x = 256, name = "tower_ga_dw_cls_cuda")]
    def run {
        dw_conv_rows_body()
    }
}

//! the same conv over the carrier's f16 taps at binding 3 (`GkDwConvRows`)
[cuda_dispatch(name = "tower_ga_dwh_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerGaDwH : GkDwConvRows {
    override FORM = 3
    override TAP_MAJOR = true
    override TAP_F16 = true

    [cuda_kernel(local_size_x = 256, name = "tower_ga_dwh_cls_cuda")]
    def run {
        dw_conv_rows_body()
    }
}

//! gemma4a's causal depthwise conv, k = 5 (`GkDwConvRows`)
[cuda_dispatch(name = "tower_dw_conv5_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerDwConv5 : GkDwConvRows {
    override FORM = 1
    override K = 5u

    [cuda_kernel(local_size_x = 256, name = "tower_dw_conv5_cls_cuda")]
    def run {
        dw_conv_rows_body()
    }
}

//! the conv module's GLU off the pointwise rows (`GkGatedRows`)
[cuda_dispatch(name = "tower_glu_sig_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerGluSig : GkGatedRows {
    [cuda_kernel(local_size_x = 256, name = "tower_glu_sig_cls_cuda")]
    def run {
        gated_rows_body()
    }
}

//! the ultravox projector's gate, silu(second half) . first half (`GkGatedRows`)
[cuda_dispatch(name = "tower_swiglu_swapped_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerSwigluSwapped : GkGatedRows {
    override FORM = 1

    [cuda_kernel(local_size_x = 256, name = "tower_swiglu_swapped_cls_cuda")]
    def run {
        gated_rows_body()
    }
}

//! the chat projector tails' pair pool (`GkPoolRows`)
[cuda_dispatch(name = "tower_pool_rows_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerPoolRows : GkPoolRows {
    [cuda_kernel(local_size_x = 256, name = "tower_pool_rows_cls_cuda")]
    def run {
        pool_rows_body()
    }
}

//! the seam of a block whose GEMMs carry no bias row, the next layernorm behind it (`GkPostAddLn`)
[cuda_dispatch(name = "tower_post_add_ln_nb_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerPostAddLnNb : GkPostAddLn {
    override RSQRT = true
    override BIAS = false

    [cuda_kernel(local_size_x = 256, name = "tower_post_add_ln_nb_cls_cuda")]
    def run {
        post_add_ln_body()
    }
}

//! the plain seam: the branch added as it is, the next rms behind it (`GkPostAddLn`)
[cuda_dispatch(name = "tower_post_add_plain_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerPostAddPlain : GkPostAddLn {
    override RSQRT = true
    override RMS = true
    override BIAS = false

    [cuda_kernel(local_size_x = 256, name = "tower_post_add_plain_cls_cuda")]
    def run {
        post_add_ln_body()
    }
}

//! gemma4a's causal sliding-window attention with the projected RPE table at a head of 128 (`GkG4aAttn`)
[cuda_dispatch(name = "tower_g4a_attn_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerG4aAttn : GkG4aAttn {
    override HS = 128u

    [cuda_kernel(local_size_x = 64, name = "tower_g4a_attn_cls_cuda")]
    def run {
        g4a_attn_body()
    }
}

//! ... at a head of 64 (`GkG4aAttn`)
[cuda_dispatch(name = "tower_g4a_attn64_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerG4aAttn64 : GkG4aAttn {
    override HS = 64u

    [cuda_kernel(local_size_x = 64, name = "tower_g4a_attn64_cls_cuda")]
    def run {
        g4a_attn_body()
    }
}

//! canary's rel-plane attention at a head of 128 (`TowerCnAttnRT`)
[cuda_dispatch(name = "tower_cn_attn_r_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerCnAttnR : TowerCnAttnRT {
    [cuda_kernel(local_size_x = 64, name = "tower_cn_attn_r_cls_cuda")]
    def run {
        cn_attn_r_body()
    }
}

//! ... at a head of 64 (`TowerCnAttnRT`)
[cuda_dispatch(name = "tower_cn_attn_r64_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerCnAttnR64 : TowerCnAttnRT {
    override HS = 64u

    [cuda_kernel(local_size_x = 64, name = "tower_cn_attn_r64_cls_cuda")]
    def run {
        cn_attn_r_body()
    }
}
