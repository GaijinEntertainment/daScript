options gen2
options _dasllama_internal = true

module _gpu_tier_armed shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_gpu_tier

//! true when the GPU tier is armed - DASLLAMA_GPU=1 on a device that serves the whole-model
//! driver; else the cell has registered the loud skip and its caller returns
def gpu_tier_armed(t : T?) : bool {
    return true if (moe_gpu_tier_installed() && gpu_want_auto())
    t |> skip("the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver")
    return false
}
