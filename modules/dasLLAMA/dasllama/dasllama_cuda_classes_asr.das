options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_classes_asr shared public

require dasllama/dasllama_lint public
require cuda/cuda_kernel public
require spirv/spirv_markers
require dasllama/dasllama_cuda_common public
require dasllama/dasllama_cuda_dispatch
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common public
require dasllama/dasllama_gpu_kernels_discrete public
require daslib/templates
require daslib/typemacro_boost
require math
require daslib/math_bits   // float_bits_to_uint: the transducer and CTC picks' records

//! the whisper decoder's K/V stores, the CPU chain's f32 layouts, on the template both homes stamp (`GkWdecKv`)
[cuda_dispatch(name = "tower_wdec_kv_cls", grid = "wgs", params = "wgs : int64"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
class CuTowerWdecKv : GkWdecKv {
    [cuda_kernel(local_size_x = 256, name = "tower_wdec_kv_cls_cuda")]
    def run {
        wdec_kv_body()
    }
}

//! the whisper decoder's K/V stores into the resident f16 plane (`GkWdecKv`)
[cuda_dispatch(name = "tower_wdec_kv16_cls", grid = "wgs", params = "wgs : int64")]
class CuTowerWdecKv16 : GkWdecKv {
    override OUT16 = true

    [cuda_kernel(local_size_x = 256, name = "tower_wdec_kv16_cls_cuda")]
    def run {
        wdec_kv_body()
    }
}

//! the whisper decoder's chunked attention partials at its head of 64 (`GkWdecAttnPart`)
[cuda_dispatch(name = "tower_wdec_attn_part_cls", kernel = "run", grid = "wgs", params = "wgs : int64"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
class CuTowerWdecAttnPart : GkWdecAttnPart {
    override HS = 64u

    [cuda_kernel(local_size_x = 128, name = "tower_wdec_attn_part_cls_cuda")]
    def run {
        wdec_attn_part_body()
    }
}

//! the whisper decoder's attention combine into Q8_0 blocks (`GkWdecAttnComb`)
[cuda_dispatch(name = "tower_wdec_attn_comb_rq_cls", kernel = "run", grid = "wgs", params = "wgs : int64"), arch(at = "../ARCHITECTURE_GPU_TOWER_VULKAN.md#vk-asr-decoder")]
class CuTowerWdecAttnCombRq : GkWdecAttnComb {
    override HS = 64u
    override OUT_Q8 = true

    [cuda_kernel(local_size_x = 64, name = "tower_wdec_attn_comb_rq_cls_cuda")]
    def run {
        wdec_attn_comb_body()
    }
}

//! layernorm rows into Q8_0 blocks (`GkPostAddLn`)
[cuda_dispatch(name = "tower_ln_rq_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerLnRq : GkPostAddLn {
    override RSQRT = true
    override ADD = false
    override OUT_Q8 = true
    override OUT32 = false

    [cuda_kernel(local_size_x = 256, name = "tower_ln_rq_cls_cuda")]
    def run {
        post_add_ln_body()
    }
}

//! the residual seam with its next layernorm into Q8_0 blocks (`GkPostAddLn`)
[cuda_dispatch(name = "tower_post_add_ln_rq_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerPostAddLnRq : GkPostAddLn {
    override RSQRT = true
    override OUT_Q8 = true
    override OUT32 = false

    [cuda_kernel(local_size_x = 256, name = "tower_post_add_ln_rq_cls_cuda")]
    def run {
        post_add_ln_body()
    }
}

//! the silero VAD's encoder, a workgroup a chunk (`GkVadEnc`)
[cuda_dispatch(name = "tower_vad_enc_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerVadEnc : GkVadEnc {
    [cuda_kernel(local_size_x = 128, name = "tower_vad_enc_cls_cuda")]
    def run {
        vad_enc_body()
    }
}

//! the silero VAD's recurrence and decoder, one workgroup over every chunk (`GkVadLstm`)
[cuda_dispatch(name = "tower_vad_lstm_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerVadLstm : GkVadLstm {
    [cuda_kernel(local_size_x = 1024, name = "tower_vad_lstm_cls_cuda")]
    def run {
        vad_lstm_body()
    }
}

//! an LSTM layer of the transducer's predictor step (`GkRnntPred`)
[cuda_dispatch(name = "rnnt_gates_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuRnntGates : GkRnntPred {
    [cuda_kernel(local_size_x = 256, name = "rnnt_gates_cls_cuda")]
    def run {
        pred_body()
    }
}

//! joint.pred's projection behind the predictor's last layer (`GkRnntPred`)
[cuda_dispatch(name = "rnnt_proj_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuRnntProj : GkRnntPred {
    override GATES = false

    [cuda_kernel(local_size_x = 256, name = "rnnt_proj_cls_cuda")]
    def run {
        pred_body()
    }
}

//! the transducer's joint over a window of frames (`GkRnntJoint`)
[cuda_dispatch(name = "rnnt_joint_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuRnntJoint : GkRnntJoint {
    [cuda_kernel(local_size_x = 256, name = "rnnt_joint_cls_cuda")]
    def run {
        joint_body()
    }
}

//! the transducer's greedy pick over the window (`GkRnntPick`)
[cuda_dispatch(name = "rnnt_pick_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuRnntPick : GkRnntPick {
    [cuda_kernel(local_size_x = 256, name = "rnnt_pick_cls_cuda")]
    def run {
        pick_body()
    }
}

//! the CTC head's pick over the head GEMM's rows, a workgroup a frame (`GkCtcPick`)
[cuda_dispatch(name = "ctc_pick_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuCtcPick : GkCtcPick {
    [cuda_kernel(local_size_x = 256, name = "ctc_pick_cls_cuda")]
    def run {
        ctc_pick_body()
    }
}

//! the bias + tanh GELU of a plane and its Q8_0 requant in one dispatch (`TowerBiasActRqT`)
[cuda_dispatch(name = "tower_bias_act_rq_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuTowerBiasActRq : TowerBiasActRqT {
    [cuda_kernel(local_size_x = 256, name = "tower_bias_act_rq_cls_cuda")]
    def run {
        rq_body()
    }
}
