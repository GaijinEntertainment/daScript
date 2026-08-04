options gen2
options _comment_hygiene = true

//! The explicit .dlim bake configuration: every input that changes image BYTES, in one struct —
//! built at load time from the live box, suppliable to the converter as JSON, embedded in the
//! image meta. The identity is a pure formatter of this struct; nothing in here probes the host.
//!
//! Discipline rule: a knob that changes image bytes MUST be a field here (missing = silent stale
//! images); a serve-only knob MUST NOT be (present = spurious rebakes on every knob flip).

module dasllama_config shared public

require strings
require daslib/utf8_utils

//! The CPU tier's layout half: which backend's repack shapes the planes, its interleave numbers,
//! and the scale/native-format outcomes. `wscale_f16` is the OUTCOME (backend has the s16 twins),
//! not the request — an image's identity must describe its bytes, not its intent.
struct public DlimCpuConfig {
    backend : string
    q8_mr : int64
    q8_wbias : int64
    q8_kgroup : int64
    kq_mr4 : int64
    kq_mr5 : int64
    kq_mr6 : int64
    kq_mr40 : int64
    q51_mr : int64
    wscale_f16 : bool
    kquant_native : bool
    kq_q40_native : bool
    kq_q50_native : bool
    kq_q51_native : bool
}

//! The vulkan flavor's plan inputs, RESOLVED — `coopmat_mode` is the running mode (not the raw
//! env string), `vram_mb` the concrete budget the plan is computed against. The dense bits and
//! `cls` are here so every walk-shaping switch is in the identity (the old tag missed them).
struct public DlimVulkanConfig {
    coopmat_mode : int
    expert_q8n : bool
    vram_mb : int64
    moe_layers : int64
    moe_stream : int64
    dn : bool
    attn : bool
    dnd : bool
    shexp : bool
    qkv : bool
    cls : bool
    dense : bool
    dense_attn : bool
    dense_shexp : bool
    trim : bool   //!< P3: big CPU weight families dropped from the image (GPU-resident consumers only)
    subgroup_size : int
    max_storage_range : int64
}

//! The metal flavor's half. Minimal for now — the metal-flavor audit (arc phase G) decides
//! whether servability needs more than the mode.
struct public DlimMetalConfig {
    mode : string
}

//! The whole bake configuration. `vulkan_on`/`metal_on` are presence flags — a flavor section
//! whose tier is absent on this box stays zeroed and MUST NOT be read.
struct public DlimConfiguration {
    quant : string = "q8"
    cpu : DlimCpuConfig
    vulkan_on : bool
    vulkan : DlimVulkanConfig
    metal_on : bool
    metal : DlimMetalConfig
}

// ===== section sources (tiers register at [init]; config never probes the host itself) =====

[unused_argument(c)]
def private dlim_no_cpu_source(var c : DlimCpuConfig) {
    panic("dasLLAMA: no DlimConfiguration cpu source registered (dasllama_common's [init] provides it)")
}

[unused_argument(c)]
def private dlim_no_vulkan_source(var c : DlimVulkanConfig) : bool => false

[unused_argument(c)]
def private dlim_no_metal_source(var c : DlimMetalConfig) : bool => false

var private g_dlim_cpu_source = @@dlim_no_cpu_source
var private g_dlim_vulkan_source = @@dlim_no_vulkan_source
var private g_dlim_metal_source = @@dlim_no_metal_source

// Function values do not survive program serialization (a baked exe restores globals as
// DATA — the defaults arrive null and only tier-registered seams heal), so the defaults are
// boot-restored here; the null guards let a real registrar win in either [init] order.
[init]
def private restore_dlim_source_defaults {
    if (g_dlim_cpu_source == null) {
        g_dlim_cpu_source = @@dlim_no_cpu_source
    }
    if (g_dlim_vulkan_source == null) {
        g_dlim_vulkan_source = @@dlim_no_vulkan_source
    }
    if (g_dlim_metal_source == null) {
        g_dlim_metal_source = @@dlim_no_metal_source
    }
}

//! The CPU tier registers its section filler (must run its backend select ONCE before reading
//! the layout getters — that side effect lives in the source, never in the identity).
def public set_dlim_cpu_source(f : function<(var c : DlimCpuConfig) : void>) {
    g_dlim_cpu_source = f
}

//! A GPU tier registers its section filler; returns false when the tier is absent/unarmed.
def public set_dlim_vulkan_source(f : function<(var c : DlimVulkanConfig) : bool>) {
    g_dlim_vulkan_source = f
}

def public set_dlim_metal_source(f : function<(var c : DlimMetalConfig) : bool>) {
    g_dlim_metal_source = f
}

//! Build the configuration this process would bake/load with. The ONE place host state is read.
def public dlim_config_current(quant : string = "q8") : DlimConfiguration {
    var c = DlimConfiguration(quant = quant)
    invoke(g_dlim_cpu_source, c.cpu)
    c.vulkan_on = invoke(g_dlim_vulkan_source, c.vulkan)
    c.metal_on = invoke(g_dlim_metal_source, c.metal)
    return c
}

// ===== pure formatters (identity, tags, JSON) =====

//! The identity string an image file/header is keyed by — a pure formatter of the struct.
//! `image_version` comes from the caller (dasllama_image owns the version constant).
def public dlim_identity(c : DlimConfiguration; image_version : int; tag : string = "") : string {
    return ("v{image_version}|{c.quant}|{c.cpu.backend}|{c.cpu.wscale_f16 ? "s16" : "s32"}"
        + "|q8 mr{c.cpu.q8_mr} b{c.cpu.q8_wbias} g{c.cpu.q8_kgroup}"
        + "|kq {c.cpu.kq_mr4}/{c.cpu.kq_mr5}/{c.cpu.kq_mr6}/{c.cpu.kq_mr40}"
        + "|q51 mr{c.cpu.q51_mr}"
        + "|nat {c.cpu.kquant_native ? 1 : 0}{c.cpu.kq_q40_native ? 1 : 0}{c.cpu.kq_q50_native ? 1 : 0}{c.cpu.kq_q51_native ? 1 : 0}"
        + (tag != "" ? "|{tag}" : ""))
}

//! The vulkan flavor's identity tag, from the RESOLVED section (the old tag folded raw env
//! strings, so "auto" hid device-derived byte differences).
def public dlim_vulkan_tag(v : DlimVulkanConfig) : string {
    return ("vulkan|m{v.coopmat_mode}{v.expert_q8n ? "n" : ""} b{v.vram_mb}"
        + " l{v.moe_layers} s{v.moe_stream}"
        + " r{v.dn ? 1 : 0}{v.attn ? 1 : 0}{v.dnd ? 1 : 0}{v.shexp ? 1 : 0}{v.qkv ? 1 : 0}{v.cls ? 1 : 0}"
        + "{v.dense ? 1 : 0}{v.dense_attn ? 1 : 0}{v.dense_shexp ? 1 : 0}{v.trim ? " T" : ""}")
}

//! Round-trip: the JSON embedded in the image meta and consumed by `dasllama-convert --config`.
def public dlim_config_json(c : DlimConfiguration) : string => sprint_json(c, true)

// hand-carried config files grow a UTF-8 BOM on Windows editors/tools — tolerate it
def public dlim_config_from_json(text : string; var c : DlimConfiguration) : bool => (
    sscan_json(contains_utf8_bom(text) ? slice(text, 3) : text, c))
