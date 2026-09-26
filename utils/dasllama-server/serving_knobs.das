options gen2

module serving_knobs

//! The serving spellings dasllama-server and dasllama-cli both accept, and what each maps to:
//! the weight quant, the KV codec, the Metal mode, the TTS lane and file set, and the `--gpu`
//! backend pick with the Vulkan want it derives. One copy, so the two programs cannot read the
//! same flag differently on the same box.

require dasllama/dasllama       // QuantMode / KVDtype / MetalMode / GpuTierWant / the backend probes and setters
require model_catalog           // gpu_env_master_off — the env rail's GPU master switch
require daslib/fio
require strings

//! True when ``s`` is a quant spelling ``pick_quant`` maps deliberately (any case): the serving
//! modes fp32/f32 | q8 | q4, plus the gguf FILE formats that serve under the kq tier —
//! q8_0 / q4_0 / q4_k / q5_k / q6_k (_M/_S suffixes fine), mxfp4, f16 / bf16.
def quant_spelling_known(s : string) : bool {
    let l = to_lower(s)
    return (l == "fp32" || l == "f32" || l == "q4" || l == "q8" || l == "q8_0" || l == "q4_0"
        || l == "mxfp4" || l == "f16" || l == "bf16"
        || starts_with(l, "q4_k") || starts_with(l, "q5_k") || starts_with(l, "q6_k"))
}

//! The serving modes are QuantMode fp32/q8/q4_0, CLI spellings "fp32" | "q8" | "q4" — "q4"
//! selects the legacy q4_0 requant tier (per-row kernels only, no batched prefill), whose one
//! use is squeezing an f16/f32 file down to 4 bits for RAM. The FILE formats the gguf loader
//! reads are first-class spellings too — all of those serve under QuantMode.q8 (K-quant, mx4,
//! and Q4_0 planes run NATIVELY on the kq tier). An unknown spelling warns and serves q8.
def pick_quant(s : string; tool : string = "dasllama-server") : QuantMode {
    let l = to_lower(s)
    if (l == "fp32" || l == "f32") {
        return QuantMode.fp32
    } elif (l == "q4") {
        return QuantMode.q4_0
    } elif (!empty(l) && !quant_spelling_known(s)) {
        to_log(LOG_WARNING, "{tool}: unknown --quant '{s}', using q8 (valid: fp32 | q8 | q8_0 | q4_0 | q4_k | q5_k | q6_k | mxfp4 | f16 | bf16 | q4 = legacy requant)\n")
    }
    return QuantMode.q8
}

def pick_kv_dtype(s : string; tool : string = "dasllama-server") : KVDtype {
    let l = to_lower(s)   // same normalization as pick_quant — F16/Q8_0/TQ4 spellings accepted
    if (l == "f32") {
        return KVDtype.f32
    } elif (l == "q8_0") {
        return KVDtype.q8_0
    } elif (l == "tq4") {
        return KVDtype.tq4
    } elif (!empty(l) && l != "f16") {
        to_log(LOG_WARNING, "{tool}: unknown --kv-dtype '{s}', using f16 (valid: f32 | f16 | q8_0 | tq4)\n")
    }
    return KVDtype.f16
}

def pick_metal_mode(s : string; tool : string = "dasllama-server") : MetalMode {
    let l = to_lower(s)
    if (l == "required") {
        return MetalMode.required
    } elif (l == "auto" || l == "when_available") {
        return MetalMode.when_available
    } elif (!empty(l) && l != "off") {
        to_log(LOG_WARNING, "{tool}: unknown --metal '{s}', using off (valid: off | auto | required)\n")
    }
    return MetalMode.off
}

let TTS_LANE_DEFAULT = "q8"

//! The TTS weight lane a spelling selects: ``q8`` | ``f32``, anything else warned about and
//! served as ``q8``.
def pick_tts_lane(s : string; tool : string = "dasllama-server") : string {
    let l = to_lower(s)
    if (l == "f32" || l == "q8") {
        return l
    }
    if (!empty(l)) {
        to_log(LOG_WARNING, "{tool}: unknown --tts-lane '{s}', using {TTS_LANE_DEFAULT} (valid: q8 | f32)\n")
    }
    return TTS_LANE_DEFAULT
}

//! The first file of a TTS model's set that is absent — the GGUF at `path`, then, for a phoneme
//! family, each front-end pack (``tts_g2p.bin``, ``tts_postag.bin``) beside it (a Pocket TTS GGUF
//! reads text and needs none). "" when the set is complete, and for an empty `path` (no TTS
//! configured). This is the file ``configure_tts`` drops the route over.
def tts_missing_file(path : string) : string {
    if (empty(path)) {
        return ""
    }
    if (!stat(path).is_valid) {
        return path
    }
    if (!tts_needs_packs(path)) {
        return ""
    }
    let dir = dir_name(path)
    for (pack in TTS_FRONT_END_PACKS) {
        let pack_path = path_join(dir, pack)
        if (!stat(pack_path).is_valid) {
            return pack_path
        }
    }
    return ""
}

//! True when `path` names a TTS model with its front-end packs beside it — the outcome
//! ``configure_tts`` will reach, answerable BEFORE the call (an empty path reads false: nothing
//! to serve). A boot that decides setup mode ahead of ``configure_tts`` reads this.
def tts_files_present(path : string) : bool {
    return !empty(path) && empty(tts_missing_file(path))
}

// ===== The --gpu backend pick =====

//! The backend knobs as one program read them off its flags and config: the `--gpu` spelling
//! (auto | off | metal | metal-required | vulkan; "" = unset), the legacy `--metal` spelling,
//! and the Vulkan detail knobs with whether each was set explicitly (an unset detail takes the
//! blessed serving shape under gpu = vulkan).
struct GpuKnobs {
    gpu : string
    metal : string
    layers : int
    layers_set : bool
    stream : int
    stream_set : bool
    dn : bool
    dn_set : bool
    attn : bool
    attn_set : bool
    dense : bool
    vram_mb : int
}

def private gpu_auto_pick(metal_ok, vulkan_ok : bool) : string {
    if (metal_ok) {
        return "metal"
    }
    return vulkan_ok ? "vulkan" : "off"
}

var private g_gpu_detected = ""   // cached auto-pick ("" = not probed yet; the probes cache too)

def detected_gpu() : string {
    if (empty(g_gpu_detected)) {
        // an explicit DASLLAMA_GPU=0 stays the opt-out: the env master only overrides the
        // auto_tier field of an explicit want, so arming the blessed shape would out-vote it
        g_gpu_detected = (gpu_env_master_off() ? "off"
                          : gpu_auto_pick(kernel_backend_available("metal"), kernel_backend_available("vulkan")))
    }
    return g_gpu_detected
}

// the ONE derivation of "does the pick fall to auto-detection" — effective_gpu and the boot
// announce both read it, so the log can never disagree with the pick (an unknown --gpu
// spelling falls through to auto exactly like unset)
def gpu_pick_is_auto(k : GpuKnobs) : bool {
    let l = to_lower(k.gpu)
    if (l == "off" || l == "metal" || l == "metal-required" || l == "vulkan") {
        return false
    }
    return l == "auto" || empty(k.metal)
}

// --gpu selector: one knob over the metal mode and the vulkan tier. Defaults-first: unset
// (with no legacy --metal flag) and the explicit "auto" spelling both serve on the detected
// backend — `gpu = off` is the opt-out. A set --metal keeps the legacy env-driven path.
def effective_gpu(k : GpuKnobs; tool : string = "dasllama-server") : string {
    let l = to_lower(k.gpu)
    if (l == "off" || l == "metal" || l == "metal-required" || l == "vulkan") {
        return l
    }
    if (!empty(l) && l != "auto") {
        to_log(LOG_WARNING, "{tool}: unknown --gpu '{k.gpu}', ignoring (valid: auto | off | metal | metal-required | vulkan)\n")
    }
    return gpu_pick_is_auto(k) ? detected_gpu() : ""
}

// vulkan detail knobs — explicit config/CLI wins; unset gets the blessed serving shape
// (K=2 resident + S=35 streamed + DN + ATTN) under gpu = vulkan
// 0 / unset = auto (-1): the loader fills the VRAM budget and streams the rest; a positive value
// pins it exactly.
def gpu_detail_layers(k : GpuKnobs) : int64
    => int64(k.layers_set && k.layers > 0 ? k.layers : -1)

def gpu_detail_stream(k : GpuKnobs) : int64
    => int64(k.stream_set && k.stream > 0 ? k.stream : -1)

def gpu_detail_dn(k : GpuKnobs) : bool
    => k.dn_set ? k.dn : true

def gpu_detail_attn(k : GpuKnobs) : bool
    => k.attn_set ? k.attn : true

[arch(at = "README.md#gpu-key")]
def vulkan_serving_want(k : GpuKnobs) : GpuTierWant
    => GpuTierWant(resident = true,
                   moe_layers = gpu_detail_layers(k), moe_stream = gpu_detail_stream(k),
                   vram_mb = int64(k.vram_mb), dn = gpu_detail_dn(k),
                   attn = gpu_detail_attn(k), dense = k.dense, shexp = true)

//! Land the picked backend on the engine before any load: the Metal mode pins the row-major
//! backend the GPU kernels read, and the Vulkan tier want must be armed before the load walk
//! uploads stacks. `gpu` is ``effective_gpu``'s answer; returns whether the Vulkan tier armed
//! (false under every other pick).
def apply_gpu_backend(k : GpuKnobs; gpu : string; tool : string = "dasllama-server") : bool {
    var metal_mode = pick_metal_mode(k.metal, tool)
    if (gpu == "metal") {
        metal_mode = MetalMode.when_available
    } elif (gpu == "metal-required") {
        metal_mode = MetalMode.required
    } elif (!empty(gpu)) {
        metal_mode = MetalMode.off   // gpu = off | vulkan both keep metal out
    }
    set_metal_mode(metal_mode)
    if (metal_mode == MetalMode.off) {
        // the Metal-build CPU-prefill guard is a profiling tripwire; its panic would drop every
        // live stream, and serving on the CPU arm is a legitimate configuration
        allow_cpu_prefill()
        if (gpu != "vulkan") {
            to_log(LOG_INFO, "{tool}: prefill runs on the CPU arm - pass --gpu metal for the Metal rails\n")
        }
    }
    to_log(LOG_INFO, "{tool}: metal mode {get_metal_mode()}\n")
    if (gpu != "vulkan") {
        return false
    }
    set_gpu_tier_want(vulkan_serving_want(k))
    if (moe_gpu_tier_arm()) {
        let ktag = gpu_detail_layers(k) < 0l ? "auto" : "{gpu_detail_layers(k)}"
        let stag = gpu_detail_stream(k) < 0l ? "auto" : "{gpu_detail_stream(k)}"
        to_log(LOG_INFO, "{tool}: vulkan GPU tier armed (K={ktag}, S={stag}, dn={gpu_detail_dn(k)}, attn={gpu_detail_attn(k)})\n")
        return true
    }
    to_log(LOG_WARNING, "{tool}: gpu = vulkan requested but the tier did not arm (no vulkan device, or this build lacks the vulkan module)\n")
    return false
}
