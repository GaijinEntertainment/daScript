// The GPU driver knobs the tests pin and put back - the tower knob and the ASR-decoder knob of whichever of the Metal, Vulkan
// and CUDA drivers this build carries - one record the TTS rail, the tower twins and the audio and vision cells capture and restore.

options gen2
options _dasllama_internal = true

module _gpu_knobs shared public

require dasllama/dasllama_lint public
require daslib/defer
require dasllama/dasllama_tower public   // TowerDev
require ?das_metal dasllama/dasllama_metal_tower
require ?das_metal dasllama/dasllama_metal_asr_dec
require ?vulkan dasllama/dasllama_vulkan_tower
require ?vulkan dasllama/dasllama_vulkan_asr_dec
require ?vulkan dasllama/dasllama_vulkan_common   // vulkan_tower_tiles_on: the device-up box fact
require ?cuda dasllama/dasllama_cuda_tower   // the vision seats under the CUDA ask
require dasllama/dasllama_gpu_tier   // gpu_want_cuda: the home the tier holds

//! Every GPU knob, as a cell found them: false for a driver the build omits.
struct GpuKnobs {
    metal_tower : bool
    metal_wdec : bool
    vulkan_tower : bool
    vulkan_wdec : bool
    cuda_tower : bool
}

//! the CUDA home holds the tier on a build that carries it (DASLLAMA_CUDA=1 or `set_gpu_home("cuda")`): its tower serves the vision seats, the Vulkan tower's stand down
def cuda_tower_asked() : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return gpu_want_cuda()
    } else {
        return false
    }
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
    static_if (typeinfo builtin_module_exists(cuda)) {
        k.cuda_tower = cuda_tower_on()
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
    static_if (typeinfo builtin_module_exists(cuda)) {
        set_cuda_tower(k.cuda_tower)
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
    static_if (typeinfo builtin_module_exists(cuda)) {
        set_cuda_tower(on)
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

//! the Metal tower's knob where das_metal is built in, else the CUDA tower's for the vision seats under the CUDA ask, else the Vulkan tower's (the audio seats' under either home)
[unused_argument(on, vision)]
def set_serving_gpu_tower(on : bool; vision : bool = false) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(on)
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            if (vision && cuda_tower_asked()) {
                set_cuda_tower(on)
                return
            }
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            set_vulkan_tower(on)
        }
    }
}

//! the serving discrete home's residents released (the model drop's sweep): the CUDA tower's under the CUDA ask, else the Vulkan tower's
[unused_argument(vision)]
def serving_tower_shutdown(vision : bool = false) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (vision && cuda_tower_asked()) {
            cuda_tower_shutdown()
            return
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        vulkan_tower_shutdown()
    }
}

//! the serving tower's image flavor (`#tower-dev-block`): Metal's where das_metal is built in, else CUDA's for the vision seats under the CUDA ask, else Vulkan's, `none` with none
[unused_argument(vision)]
def serving_tower_dev(vision : bool = false) : TowerDev {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return TowerDev.metal
    } else {
        return TowerDev.cuda if (vision && cuda_tower_asked())
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return TowerDev.vulkan
        } else {
            return TowerDev.none
        }
    }
}

//! a box fact: the serving tower's device stands up with what its chains ride - Metal's pipelines built, the CUDA tower armed for
//! the vision seats under the CUDA ask, else Vulkan's tier armed with its tiles and the f16 GEMM feed - false on a build with no driver
[unused_argument(vision)]
def serving_tower_device_up(vision : bool = false) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_tower_init()
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            return cuda_tower_arms() if (vision && cuda_tower_asked())
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_tower_arms() && vulkan_tower_tiles_on() && vulkan_tower_front_route()
        } else {
            return false
        }
    }
}

typedef GpuTowerCounters = tuple<encodes : int64; rows : int64; blocks : int64; declines : int64; convs : int64; mels : int64>  //! the serving tower's counters since the process started

//! the serving tower's counters: the Metal tower's where das_metal is built in, else the CUDA tower's for the vision seats under the CUDA ask, else the Vulkan tower's, zeros with none
[unused_argument(vision)]
def gpu_tower_counters(vision : bool = false) : GpuTowerCounters {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let s = metal_tower_stats()
        return (encodes = s.encodes, rows = s.rows, blocks = s.blocks, declines = s.declines, convs = s.convs, mels = metal_tower_mel_encodes())
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            if (vision && cuda_tower_asked()) {
                let c = cuda_tower_stats()
                return (encodes = c.encodes, rows = c.rows, blocks = c.blocks, declines = c.declines, convs = c.convs, mels = c.mels)
            }
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            let s = vulkan_tower_stats()
            return (encodes = s.encodes, rows = s.rows, blocks = s.blocks, declines = s.declines, convs = s.convs, mels = s.mels)
        } else {
            return (encodes = 0l, rows = 0l, blocks = 0l, declines = 0l, convs = 0l, mels = 0l)
        }
    }
}

//! tower encodes served since the process started, on the serving tower
def gpu_tower_encodes(vision : bool = false) : int64 => gpu_tower_counters(vision).encodes

//! the serving tower's declines by reason (the decline enum's member names) since the process started, empty with no driver
[unused_argument(vision)]
def gpu_tower_declines(vision : bool = false) : table<string; int64> {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return <- metal_tower_declines()
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            return <- cuda_tower_declines() if (vision && cuda_tower_asked())
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return <- vulkan_tower_declines()
        } else {
            var none : table<string; int64>
            return <- none
        }
    }
}

//! the serving tower's declines under `reason` (the decline enum's member name) since the process started
def gpu_tower_declines_of(reason : string; vision : bool = false) : int64 => gpu_tower_declines(vision)?[reason] ?? 0l

//! the serving tower's `quant_mode` declines since the process started
def gpu_tower_quant_declines(vision : bool = false) : int64 => gpu_tower_declines_of("quant_mode", vision)
