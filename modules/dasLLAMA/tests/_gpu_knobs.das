// The GPU driver knobs the tests pin and put back - the tower knob and the ASR-decoder knob of whichever of the Metal, Vulkan
// and CUDA drivers this build carries - one record the TTS rail, the tower twins and the audio and vision cells capture and
// restore; and the tower families' routing: a cell names its family, and the helpers answer from the home that family's seats
// serve on - the CUDA home's under the CUDA ask for the vision families and the rows of `CUDA_AUDIO_FAMILIES` turned on, else
// the Vulkan home's (Metal's on a das_metal build).

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
require ?cuda dasllama/dasllama_cuda_tower_g4a   // cuda_tower_vad_stats: the VAD's counters on the CUDA home
require ?cuda dasllama/dasllama_cuda_tower_fc   // set_cuda_gigaam_exact_lane: GigaAM's lane knob on the CUDA home
require ?cuda dasllama/dasllama_cuda_asr_dec_fc   // cuda_ctc_stats: the CTC head on the CUDA home
require ?cuda dasllama/dasllama_cuda_asr_dec   // cuda_wdec_stats, set_cuda_wdec: the whisper decoder on the CUDA home
require dasllama/dasllama_gpu_tier   // gpu_want_cuda: the home the tier holds

//! the tower families a cell names its seats by: every vision family (their seats follow the GPU home), the TTS seats (the Vulkan
//! tower's under either home), and each audio family of the engine's `TowerAudioFamily`
enum TowerFamily {
    vision
    tts
    whisper
    qwen3a
    gemma4a
    canary
    parakeet
    gigaam
    transducer
    vad
}

//! the audio families whose cells run on the CUDA home under the CUDA ask (the engine's `CUDA_AUDIO_HOMES` rows); a row off routes
//! the family's cells to the Vulkan home
let CUDA_AUDIO_FAMILIES <- {
    TowerFamily.whisper => true,
    TowerFamily.qwen3a => true,
    TowerFamily.gemma4a => true,
    TowerFamily.canary => true,
    TowerFamily.parakeet => true,
    TowerFamily.gigaam => true,
    TowerFamily.transducer => true,
    TowerFamily.vad => true
}

//! Every GPU knob, as a cell found them: false for a driver the build omits.
struct GpuKnobs {
    metal_tower : bool
    metal_wdec : bool
    vulkan_tower : bool
    vulkan_wdec : bool
    cuda_tower : bool
    cuda_wdec : bool
}

//! the CUDA home holds the tier on a build that carries it (DASLLAMA_CUDA=1 or `set_gpu_home("cuda")`)
def cuda_tower_asked() : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return gpu_want_cuda()
    } else {
        return false
    }
}

//! family `fam`'s seats serve on the CUDA home: under the CUDA ask, a vision family or a `CUDA_AUDIO_FAMILIES` row turned on
def tower_family_cuda(fam : TowerFamily) : bool => cuda_tower_asked() && (fam == TowerFamily.vision || (CUDA_AUDIO_FAMILIES?[fam] ?? false))

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
        k.cuda_wdec = cuda_wdec_on()
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
        set_cuda_wdec(k.cuda_wdec)
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
    static_if (typeinfo builtin_module_exists(cuda)) {
        set_cuda_wdec(on)
    }
}

//! the tower knob of the home family `fam`'s seats serve on: Metal's with das_metal, else CUDA's for a CUDA-home family, else Vulkan's
[unused_argument(on, fam)]
def set_serving_gpu_tower(on : bool; fam : TowerFamily) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(on)
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            if (tower_family_cuda(fam)) {
                set_cuda_tower(on)
                return
            }
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            set_vulkan_tower(on)
        }
    }
}

//! the tower knob of the home family `fam`'s seats serve on (`set_serving_gpu_tower`'s getter); false with no driver
def serving_gpu_tower(fam : TowerFamily) : bool {
    let k = gpu_knobs()
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return k.metal_tower
    } else {
        return tower_family_cuda(fam) ? k.cuda_tower : k.vulkan_tower
    }
}

//! the residents of the discrete home family `fam`'s seats serve on released, the decoder seats' among them (the model drop's sweep)
[unused_argument(fam)]
def gpu_tower_shutdown(fam : TowerFamily) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (tower_family_cuda(fam)) {
            cuda_tower_shutdown()
            return
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        vulkan_tower_shutdown()
    }
}

//! the image flavor family `fam`'s seats serve (`#tower-dev-block`): Metal's with das_metal, else CUDA's for a CUDA-home family, else Vulkan's, `none` with none
[unused_argument(fam)]
def serving_tower_dev(fam : TowerFamily) : TowerDev {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return TowerDev.metal
    } else {
        return TowerDev.cuda if (tower_family_cuda(fam))
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return TowerDev.vulkan
        } else {
            return TowerDev.none
        }
    }
}

//! the device family `fam`'s seats serve on stands up with what its chains ride (Metal pipelines, the CUDA tower armed, Vulkan tiles and f16 feed); false with no driver
[unused_argument(fam)]
def serving_tower_device_up(fam : TowerFamily) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_tower_init()
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            return cuda_tower_arms() if (tower_family_cuda(fam))
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_tower_arms() && vulkan_tower_tiles_on() && vulkan_tower_front_route()
        } else {
            return false
        }
    }
}

//! family `fam`'s discrete device armed with a tile family its chains ride, the f16 feed aside: the CUDA tower armed, else Vulkan's tiles; false off the discrete homes
[unused_argument(fam)]
def gpu_tower_tiles_up(fam : TowerFamily) : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return cuda_tower_arms() if (tower_family_cuda(fam))
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vulkan_tower_arms() && vulkan_tower_tiles_on()
    } else {
        return false
    }
}

//! family `fam`'s fronts ride the f16 GEMM feed on its home: always on CUDA's cm2 tiles, the Vulkan feed route else, false off the discrete homes
[unused_argument(fam)]
def gpu_tower_front_route(fam : TowerFamily) : bool {
    return true if (tower_family_cuda(fam))
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vulkan_tower_front_route()
    } else {
        return false
    }
}

typedef GpuTowerCounters = tuple<encodes : int64; rows : int64; blocks : int64; declines : int64; convs : int64; mels : int64>  //! the serving tower's counters since the process started

//! the counters of the tower family `fam`'s seats serve on: Metal's with das_metal, else CUDA's for a CUDA-home family, else Vulkan's; zeros with none
[unused_argument(fam)]
def gpu_tower_counters(fam : TowerFamily) : GpuTowerCounters {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let s = metal_tower_stats()
        return (encodes = s.encodes, rows = s.rows, blocks = s.blocks, declines = s.declines, convs = s.convs, mels = metal_tower_mel_encodes())
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            if (tower_family_cuda(fam)) {
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

//! tower encodes served since the process started on family `fam`'s tower
def gpu_tower_encodes(fam : TowerFamily) : int64 => gpu_tower_counters(fam).encodes

//! family `fam`'s tower's declines by reason (the decline enum's member names) since the process started, empty with no driver
[unused_argument(fam)]
def gpu_tower_declines(fam : TowerFamily) : table<string; int64> {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return <- metal_tower_declines()
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            return <- cuda_tower_declines() if (tower_family_cuda(fam))
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return <- vulkan_tower_declines()
        } else {
            var none : table<string; int64>
            return <- none
        }
    }
}

//! family `fam`'s tower's declines under `reason` (the decline enum's member name) since the process started
def gpu_tower_declines_of(reason : string; fam : TowerFamily) : int64 => gpu_tower_declines(fam)?[reason] ?? 0l

//! family `fam`'s tower's `quant_mode` declines since the process started
def gpu_tower_quant_declines(fam : TowerFamily) : int64 => gpu_tower_declines_of("quant_mode", fam)

//! the projector tails family `fam`'s tower served since the process started: Metal's with das_metal, else the discrete home's, else 0
[unused_argument(fam)]
def gpu_tower_tail_encodes(fam : TowerFamily) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_tower_tail_encodes()
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            return cuda_tower_tail_encodes() if (tower_family_cuda(fam))
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_tower_tail_encodes()
        } else {
            return 0l
        }
    }
}

//! the CTC head's device counters on gigaam's home - heads, handoffs, uploads, declines - zeros off the discrete homes
def gpu_ctc_stats() : tuple<heads : int64; handoffs : int64; uploads : int64; declines : int64> {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (tower_family_cuda(TowerFamily.gigaam)) {
            let c = cuda_ctc_stats()
            return (heads = c.heads, handoffs = c.handoffs, uploads = c.uploads, declines = c.declines)
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let s = vulkan_ctc_stats()
        return (heads = s.heads, handoffs = s.handoffs, uploads = s.uploads, declines = s.declines)
    } else {
        return (heads = 0l, handoffs = 0l, uploads = 0l, declines = 0l)
    }
}

//! gigaam's exact-lane knob on its home pinned; answers the setting it replaced (false off the discrete homes)
[unused_argument(on)]
def set_gpu_gigaam_exact_lane(on : bool) : bool {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return set_cuda_gigaam_exact_lane(on) if (tower_family_cuda(TowerFamily.gigaam))
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return set_vulkan_gigaam_exact_lane(on)
    } else {
        return false
    }
}

//! the whisper decoder's device knob on whisper's home
[unused_argument(on)]
def set_gpu_wdec(on : bool) {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (tower_family_cuda(TowerFamily.whisper)) {
            set_cuda_wdec(on)
            return
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        set_vulkan_wdec(on)
    }
}

//! the whisper decoder's device counters on whisper's home - windows, steps, tokens, declines, handoffs - zeros off the discrete homes
def gpu_wdec_stats() : tuple<windows : int64; steps : int64; tokens : int64; declines : int64; handoffs : int64> {
    static_if (typeinfo builtin_module_exists(cuda)) {
        if (tower_family_cuda(TowerFamily.whisper)) {
            let c = cuda_wdec_stats()
            return (windows = c.windows, steps = c.steps, tokens = c.tokens, declines = c.declines, handoffs = c.handoffs)
        }
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let s = vulkan_wdec_stats()
        return (windows = s.windows, steps = s.steps, tokens = s.tokens, declines = s.declines, handoffs = s.handoffs)
    } else {
        return (windows = 0l, steps = 0l, tokens = 0l, declines = 0l, handoffs = 0l)
    }
}

//! the whisper decoder's declines by reason on whisper's home, empty off the discrete homes
def gpu_wdec_declines() : table<string; int64> {
    static_if (typeinfo builtin_module_exists(cuda)) {
        return <- cuda_wdec_declines() if (tower_family_cuda(TowerFamily.whisper))
    }
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return <- vulkan_wdec_declines()
    } else {
        var none : table<string; int64>
        return <- none
    }
}


//! the VAD chunks the device served on the VAD's home since the process started (0 with no driver)
def gpu_vad_chunks() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_tower_vad_stats().chunks
    } else {
        static_if (typeinfo builtin_module_exists(cuda)) {
            return cuda_tower_vad_stats().chunks if (tower_family_cuda(TowerFamily.vad))
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return vulkan_tower_vad_stats().chunks
        } else {
            return 0l
        }
    }
}
