options gen2
options indenting = 4
options _dasllama_internal = true

module dasllama_cuda_classes_dflash shared public

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

//! the block drafter's dynamic two-tap convolution (`GkDflashConv`)
[cuda_dispatch(name = "dflash_conv_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuDflashConv : GkDflashConv {
    [cuda_kernel(local_size_x = 256, name = "dflash_conv_cls_cuda")]
    def run {
        dflash_conv_body()
    }
}

//! the block drafter's head norm and rope (`GkDflashQkn`), the inverse norm `1 / sqrt` as the CPU pass spells it
[cuda_dispatch(name = "dflash_qkn_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuDflashQkn : GkDflashQkn {
    [cuda_kernel(local_size_x = 256, name = "dflash_qkn_cls_cuda")]
    def run {
        dflash_qkn_body()
    }
}

//! the block drafter's attention over its ring and its own rows (`GkDflashAttn`)
[cuda_dispatch(name = "dflash_attn_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuDflashAttn : GkDflashAttn {
    [cuda_kernel(local_size_x = 256, name = "dflash_attn_cls_cuda")]
    def run {
        dflash_attn_body()
    }
}

//! the device selector's top-k (`GkDflashTopk`): a row's slices' lists, then the row's list off them
[cuda_dispatch(name = "dflash_topk_slices_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuDflashTopkSlices : GkDflashTopk {
    [cuda_kernel(local_size_x = 256, name = "dflash_topk_slices_cls_cuda")]
    def run {
        topk_slices_body()
    }
}

//! the selector's greedy walk over the draft rows' top-k (`GkDflashWalk`), one workgroup
[cuda_dispatch(name = "dflash_walk_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuDflashWalk : GkDflashWalk {
    [cuda_kernel(local_size_x = 256, name = "dflash_walk_cls_cuda")]
    def run {
        walk_body()
    }
}

//! the top-k's merge stage (`GkDflashTopk`): a workgroup a row over its slices' lists
[cuda_dispatch(name = "dflash_topk_merge_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class CuDflashTopkMerge : GkDflashTopk {
    [cuda_kernel(local_size_x = 256, name = "dflash_topk_merge_cls_cuda")]
    def run {
        topk_merge_body()
    }
}
