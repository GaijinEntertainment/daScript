options gen2
options _dasllama_internal = true

module _gpu_tier_armed shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require dasllama/dasllama_gpu_tier

//! why the GPU tier is not armed; "" where it is - DASLLAMA_GPU=1 on a device that serves the whole-model driver
def gpu_tier_why() : string {
    return "" if (moe_gpu_tier_installed() && gpu_want_auto())
    return "the GPU tier is not armed - needs DASLLAMA_GPU=1 on a device that serves the whole-model driver"
}

//! true when the GPU tier is armed (`gpu_tier_why`); else the cell has registered the loud skip and its caller returns
def gpu_tier_armed(t : T?) : bool {
    let why = gpu_tier_why()
    return true if (empty(why))
    t |> skip(why)
    return false
}
