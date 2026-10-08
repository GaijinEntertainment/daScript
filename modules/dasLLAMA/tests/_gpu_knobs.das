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

//! Every GPU knob, as a cell found them: false for a driver the build omits.
struct GpuKnobs {
    metal_tower : bool
    metal_wdec : bool
    vulkan_tower : bool
    vulkan_wdec : bool
}

def gpu_knobs() : GpuKnobs {
    var k = GpuKnobs()
    static_if (typeinfo builtin_module_exists(das_metal)) {
        k.metal_tower = metal_tower_knob()
        k.metal_wdec = metal_wdec_knob()
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
        set_metal_wdec(k.metal_wdec)
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

//! the CPU-decoder claims pin every whisper ASR-decoder driver off
[unused_argument(on)]
def set_every_gpu_wdec(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_wdec(on)
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_wdec(on)
    }
}

//! the Metal tower's knob where das_metal is built in, else the Vulkan tower's
[unused_argument(on)]
def set_serving_gpu_tower(on : bool) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(on)
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            set_vulkan_tower(on)
        }
    }
}

typedef GpuTowerCounters = tuple<encodes : int64; rows : int64; blocks : int64; declines : int64>   //! the serving tower's counters since the process started

//! the serving tower's counters: the Metal tower's where das_metal is built in, else the Vulkan tower's, zeros with neither
def gpu_tower_counters() : GpuTowerCounters {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let s = metal_tower_stats()
        return (encodes = s.encodes, rows = s.rows, blocks = s.blocks, declines = s.declines)
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let s = vulkan_tower_stats()
            return (encodes = s.encodes, rows = s.rows, blocks = s.blocks, declines = s.declines)
        } else {
            return (encodes = 0l, rows = 0l, blocks = 0l, declines = 0l)
        }
    }
}

//! tower encodes served since the process started, on the serving tower
def gpu_tower_encodes() : int64 => gpu_tower_counters().encodes

//! the serving tower's `quant_mode` declines since the process started
def gpu_tower_quant_declines() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_tower_declines()?["quant_mode"] ?? 0l
    } else {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_tower_declines()?["quant_mode"] ?? 0l
        } else {
            return 0l
        }
    }
}
