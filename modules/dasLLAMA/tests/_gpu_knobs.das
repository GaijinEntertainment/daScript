// The GPU driver knobs the tests pin and put back - the tower knob and the ASR-decoder knob of whichever of the Metal and Vulkan
// drivers this build carries - one record the TTS rail, the tower twins and the audio and vision cells capture and restore.

options gen2
options _dasllama_internal = true

module _gpu_knobs shared public

require dasllama/dasllama_lint public
require daslib/defer
require ?das_metal dasllama/dasllama_metal_tower
require ?das_metal dasllama/dasllama_metal_asr_dec
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_asr_dec

//! Every GPU knob with a getter, as a cell found them: false for a driver the build omits. The Metal ASR-decoder knob has no getter, so no record carries it.
struct GpuKnobs {
    metal_tower : bool
    vulkan_tower : bool
    vulkan_wdec : bool
}

def gpu_knobs() : GpuKnobs {
    var k = GpuKnobs()
    static_if (typeinfo builtin_module_exists(das_metal)) {
        k.metal_tower = metal_tower_knob()
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        k.vulkan_tower = vulkan_tower_on()
        k.vulkan_wdec = vulkan_wdec_on()
    }
    return k
}

[unused_argument(k)]
def set_gpu_knobs(k : GpuKnobs) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(k.metal_tower)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(k.vulkan_tower)
        set_vulkan_wdec(k.vulkan_wdec)
    }
}

//! `body` with every GPU knob put back as it found them, however `body` leaves
def with_gpu_knobs(body : block<() : void>) {
    let k = gpu_knobs()
    defer() {
        set_gpu_knobs(k)
    }
    invoke(body)
}

//! the CPU-lane claims pin every GPU tower driver off (a hook that serves silently stops the cell measuring the CPU chain)
[unused_argument(on)]
def set_every_gpu_tower(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(on)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_tower(on)
    }
}

//! the CPU-decoder claims pin every whisper ASR-decoder driver off; the Metal knob has no getter, so a cell that pins it puts it back with `set_every_gpu_wdec(true)`, its default
[unused_argument(on)]
def set_every_gpu_wdec(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_wdec(on)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_wdec(on)
    }
}
