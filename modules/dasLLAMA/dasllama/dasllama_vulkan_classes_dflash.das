options gen2
options indenting = 4
options _dasllama_internal = true
options loop_unroll_cost = 0    // kernel source: the emitter gets loops and locals as written
options disable_forward

module dasllama_vulkan_classes_dflash shared public

//! The block drafter's stamps on the Vulkan home: the common templates Metal and the CUDA home stamp
//! (`GkDflashConv`, `GkDflashQkn`, `GkDflashAttnTile`, `GkDflashAttnMerge`, `GkDflashTopk`, `GkDflashWalk`), each under this home's dispatch stem

require dasllama/dasllama_lint public
require spirv/spirv_shader
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_vulkan_common
require dasllama/dasllama_vulkan_dispatch
require dasllama/dasllama_gpu_math
require dasllama/dasllama_gpu_kernels_common public
require dasllama/dasllama_gpu_kernels_discrete public
require daslib/templates
require daslib/typemacro_boost
require math

//! the block drafter's dynamic two-tap convolution (`GkDflashConv`)
[vk_dispatch(name = "dflash_conv_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class DflashConv : GkDflashConv {
    [spirv_kernel(local_size_x = 256)]
    def run {
        dflash_conv_body()
    }
}

//! the block drafter's head norm and rope (`GkDflashQkn`)
[vk_dispatch(name = "dflash_qkn_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class DflashQkn : GkDflashQkn {
    [spirv_kernel(local_size_x = 256)]
    def run {
        dflash_qkn_body()
    }
}

//! a key tile of the block drafter's attention over its ring and its own rows (`GkDflashAttnTile`)
[vk_dispatch(name = "dflash_attn_tile_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class DflashAttnTile : GkDflashAttnTile {
    [spirv_kernel(local_size_x = 256)]
    def run {
        dflash_attn_tile_body()
    }
}

//! the attention's tiles joined a (row, head) (`GkDflashAttnMerge`)
[vk_dispatch(name = "dflash_attn_merge_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class DflashAttnMerge : GkDflashAttnMerge {
    [spirv_kernel(local_size_x = 256)]
    def run {
        dflash_attn_merge_body()
    }
}

//! the device selector's top-k (`GkDflashTopk`): a row's slices' lists, then the row's list off them
[vk_dispatch(name = "dflash_topk_slices_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class DflashTopkSlices : GkDflashTopk {
    [spirv_kernel(local_size_x = 256)]
    def run {
        topk_slices_body()
    }
}

//! the top-k's merge stage (`GkDflashTopk`): a workgroup a row over its slices' lists
[vk_dispatch(name = "dflash_topk_merge_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class DflashTopkMerge : GkDflashTopk {
    [spirv_kernel(local_size_x = 256)]
    def run {
        topk_merge_body()
    }
}

//! the selector's greedy walk over the draft rows' top-k (`GkDflashWalk`), one workgroup
[vk_dispatch(name = "dflash_walk_cls", kernel = "run", grid = "wgs", params = "wgs : int64")]
class DflashWalk : GkDflashWalk {
    [spirv_kernel(local_size_x = 256)]
    def run {
        walk_body()
    }
}
