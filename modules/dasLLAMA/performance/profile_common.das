options gen2
options stack = 65536   // the forward/prefill chain + generated kernels need more than the 16K default on arm64
options indenting = 4

// Shared foundation for the dasLLAMA profiling suite: the model catalog, the small/large tier gate,
// the per-box platform block, and the baseline-TSV I/O that the reference establisher writes and the
// das-side generator reads. The suite is deliberately self-contained (it does not cross-require the
// tests/ tier helper) so it lives entirely under modules/dasLLAMA/performance/.

module profile_common shared public

require daslib/fio public
require dasllama/dasllama_env  // env_int/env_str: the shared typed readers + the knob registry
require strings public
require math
require daslib/strings_boost public
require daslib/logger public              // structured, ISO-8601-timestamped ndjson event log
require daslib/json public                // JV(...) for logger field payloads
require dasllama/dasllama_common public   // QuantMode
require dasllama/dasllama_math            // active_kernel_backend — the exec_fmt layout receipt
require llvm/daslib/llvm_tune             // tune_status — stamp provenance for the platform block

// ===== Box + model roots =====

//! Shared log name for the whole profiling suite: one timestamped ndjson file per box
//! ({das_root}/logs/dasllama_profile_<box>.log), appended across all four tools (LLM/ASR ×
//! establisher/generator). `tail`/`type` it for live, per-model, wall-clock-stamped progress — the
//! logger fflushes every record, so there is no stdout-buffering blind spot.
def public prof_log_name() : string {
    return "dasllama_profile_{box_name()}"
}

// Machine-local model root; override with DASLLAMA_MODELS_DIR (the x64/handoff boxes).
def models_dir() : string {
    let env = get_env_variable("DASLLAMA_MODELS_DIR")
    if (empty(env)) {
        return "/Users/borisbatkin/Work/llama.cpp/models/"
    }
    // callers concat "{models_dir()}{file}" — guarantee the separator so a slashless override
    // (DASLLAMA_MODELS_DIR=/models) doesn't silently skip every catalog file
    return ends_with(env, "/") || ends_with(env, "\\") ? env : "{env}/"
}

// Short box tag baked into the JSON/TSV filenames (profile_llm_<box>.json). Override with DASLLAMA_BOX.
def box_name() : string {
    let env = get_env_variable("DASLLAMA_BOX")
    return empty(env) ? "m1" : env
}

//! Profiling lane/thread count for this box. 16-cores-with-affinity is the standing methodology on
//! the x64 boxes; the M1 rig keeps its 8 P-core lanes. Override with DASLLAMA_THREADS.
def profile_threads() : int {
    let env = get_env_variable("DASLLAMA_THREADS")
    if (!empty(env)) {
        return max(to_int(env), 1)
    }
    return box_name() == "m1" ? 8 : 16
}

//! True when profiling lanes pin to distinct physical cores (all Windows/x64 boxes — OS placement
//! is a lottery: SMT pairs measured pp512 bimodal on the 3990X). macOS has no thread-affinity API.
def affinity_on() : bool {
    return is_windows()
}

//! llama-bench affinity arguments (leading space; "" when affinity is off): the first `threads`
//! physical cores as a logical-CPU mask — SMT siblings are adjacent on Windows, so even bits
//! (16 -> 0x55555555) — plus --cpu-strict so the pins hold. The das-side mirror is jobque affinity.
def lcpp_affinity_args(threads : int) : string {
    if (!affinity_on()) {
        return ""
    }
    var mask = 0ul
    for (i in range(min(threads, 32))) {
        mask |= 1ul << uint64(2 * i)
    }
    return " --cpu-mask 0x{mask:x} --cpu-strict 1"
}

// Models at or above this are large-tier (deferred unless --tier large|all). 6 GiB splits the
// ~5GB-and-under carriers (incl. gemma-4-E2B at 4.63GB) from the 7GB+ heavies.
let LARGE_TIER_BYTES = 6ul * 1024ul * 1024ul * 1024ul

// ===== The text-LLM catalog =====

//! One profiled text model: its GGUF basename under [[models_dir]], a scoreboard display name, the
//! quant to load das-side, and the prefill / emission context sizes to measure at.
struct LlmModel {
    file : string
    display : string
    quant : QuantMode
    quant_label : string   // scoreboard quant cell override ("" = quant_str) — Q4_K_M carriers load kq-native under q8 mode
    prefill_ctx : int
    emission_ctx : int
}

// The text LLMs the suite profiles. Absent files are skipped at run time, so this can list every box's
// models; each box measures the subset it actually has. Audio models live in the ASR path, not here.
def llm_catalog() : array<LlmModel> {
    return <- [
        LlmModel(file="SmolLM2-135M-Instruct-Q8_0.gguf", display="SmolLM2-135M", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen2.5-0.5B-Instruct-Q8_0.gguf", display="Qwen2.5-0.5B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen3-0.6B-Q8_0.gguf", display="Qwen3-0.6B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen3.5-0.8B-Q8_0.gguf", display="Qwen3.5-0.8B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="gemma-3-1b-it-Q8_0.gguf", display="gemma-3-1B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="tinyllama-1.1b-chat-v1.0.Q8_0.gguf", display="TinyLlama-1.1B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Llama-3.2-1B-Instruct-Q8_0.gguf", display="Llama-3.2-1B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen2.5-1.5B-Instruct-Q8_0.gguf", display="Qwen2.5-1.5B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="SmolLM2-1.7B-Instruct-Q8_0.gguf", display="SmolLM2-1.7B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="gemma-2-2b-it-Q8_0.gguf", display="gemma-2-2B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Llama-3.2-3B-Instruct-Q8_0.gguf", display="Llama-3.2-3B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="gemma-3-4b-it-Q8_0.gguf", display="gemma-3-4B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen3-4B-Instruct-2507-Q8_0.gguf", display="Qwen3-4B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen3-4B-Instruct-2507-Q4_K_M.gguf", display="Qwen3-4B-q4k", quant=QuantMode.q8, quant_label="q4_k_m", prefill_ctx=512, emission_ctx=128),
        LlmModel(file="gemma-4-E2B-it-Q8_0.gguf", display="gemma-4-E2B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Mistral-7B-Instruct-v0.3-Q8_0.gguf", display="Mistral-7B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="gemma-4-E4B-it-Q8_0.gguf", display="gemma-4-E4B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Meta-Llama-3.1-8B-Instruct-Q8_0.gguf", display="Llama-3.1-8B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="gpt-oss-20b-mxfp4.gguf", display="gpt-oss-20b", quant=QuantMode.q8, quant_label="mxfp4", prefill_ctx=512, emission_ctx=128),
        LlmModel(file="gemma-4-26B-A4B-it-Q8_0.gguf", display="gemma-4-26B-A4B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="gemma-4-26B-A4B-it-Q4_K_M.gguf", display="gemma-4-26B-A4B-q4k", quant=QuantMode.q8, quant_label="q4_k_m", prefill_ctx=512, emission_ctx=128),
        // the QAT file rides the q40 KqFmt tier (native Q4_0 planes on the kq rails) — quant is
        // the loader mode (q8), the label the on-disk format, like the Q4_K_M rows
        LlmModel(file="gemma-4-26B_q4_0-it.gguf", display="gemma-4-26B-A4B-q40", quant=QuantMode.q8, quant_label="q4_0", prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", display="Qwen3-30B-A3B", quant=QuantMode.q8, quant_label="q4_k_m", prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen3.6-35B-A3B-UD-Q4_K_M.gguf", display="Qwen3.6-35B-A3B", quant=QuantMode.q8, quant_label="q4_k_m", prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen3-Coder-30B-A3B-Instruct-Q8_0.gguf", display="Qwen3-Coder-30B", quant=QuantMode.q8, prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen3-Coder-30B-A3B-Instruct-Q4_K_M.gguf", display="Qwen3-Coder-30B-q4k", quant=QuantMode.q8, quant_label="q4_k_m", prefill_ctx=512, emission_ctx=128),
        LlmModel(file="Qwen3-Coder-Next-Q4_K_M.gguf", display="Qwen3-Coder-Next-q4k", quant=QuantMode.q8, quant_label="q4_k_m", prefill_ctx=512, emission_ctx=128)
    ]
}

//! Tier selector matching the --tier flag.
enum Tier {
    small
    large
    all
}

def parse_tier(s : string) : Tier {
    if (s == "large") {
        return Tier.large
    }
    if (s == "all") {
        return Tier.all
    }
    return Tier.small
}

// Full path to a catalog entry's GGUF.
def model_path(m : LlmModel) : string {
    return "{models_dir()}{m.file}"
}

//! The scoreboard spelling of a quant mode (the JSON `quant` cell).
def quant_str(q : QuantMode) : string {
    if (q == QuantMode.q8) {
        return "q8"
    }
    if (q == QuantMode.q4_0) {
        return "q4_0"
    }
    return "fp32"
}

//! The scoreboard quant cell for a catalog entry: its explicit label when set (the kq-native
//! Q4_K_M carriers load under QuantMode.q8), else the plain quant-mode spelling.
def model_quant_str(m : LlmModel) : string {
    return empty(m.quant_label) ? quant_str(m.quant) : m.quant_label
}

//! Active (per-token) parameter count in billions, derived from `t`'s Config geometry: the full
//! embedding (also the tied classifier) + attention + FFN, counting only the `n_expert_used` routed
//! experts for MoE. Dense models get ~total; MoE gets the compute-relevant active size. Norms/biases
//! (negligible) and the per-layer PLE embedding table (an embedding lookup, not a matmul) are omitted.
def active_params_b(t : Model) : double {
    let c = t.config
    var p = c.vocab_size * c.dim   // token embedding (also the tied classifier when shared_weights)
    for (l in range64(c.n_layers)) {
        let qd = layer_qd(c, l)
        let kvd = layer_kv_dim(c, l)
        p += c.dim * qd + c.dim * kvd + qd * c.dim   // wq, wk, wo
        if (t.wv_offs[l] >= 0l) {
            p += c.dim * kvd                         // wv (absent on gemma-4 V-from-K layers)
        }
        if (c.moe) {
            p += c.dim * c.n_expert                        // router
            p += c.n_expert_used * 3l * c.dim * c.n_ff_exp // ACTIVE routed experts only
            if (c.n_ff_shexp > 0l) {
                p += 3l * c.dim * c.n_ff_shexp             // shared expert
            }
        } else {
            p += 3l * c.dim * layer_hidden(t, l)           // dense gate / up / down
        }
    }
    if (!c.shared_weights) {
        p += c.dim * c.vocab_size                          // untied classifier
    }
    return double(p) / 1.0e9lf
}

def private exec_fmt_add(var seen : table<string>; a : array<KqFmt>) {
    if (empty(a)) {
        seen |> insert("q8")
        return
    }
    for (f in a) {
        seen |> insert("{f}")
    }
}

//! What this loaded das Model EXECUTES — per-site weight formats, activation/scale treatment,
//! and the weight layout — the bench record's apples-to-apples receipt (`BenchRun.exec_fmt`).
//! A kq entry (k4/k5/k6/q40/q51) means the DISK format runs natively; q8 planes on a K-quant file
//! are tensors the file itself ships as Q8_0 (norms/embeddings ride fblob, not listed).
def model_exec_fmt(t : Model) : string {
    if (t.quant != QuantMode.q8) {
        return quant_str(t.quant)
    }
    var seen : table<string>
    exec_fmt_add(seen, t.wq_fmt)
    exec_fmt_add(seen, t.wk_fmt)
    exec_fmt_add(seen, t.wv_fmt)
    exec_fmt_add(seen, t.wo_fmt)
    exec_fmt_add(seen, t.w1_fmt)
    exec_fmt_add(seen, t.w2_fmt)
    exec_fmt_add(seen, t.w3_fmt)
    if (t.experts_mx4) {
        seen |> insert("mxfp4")
    } elif (t.config.moe) {
        exec_fmt_add(seen, t.we1_fmt)
        exec_fmt_add(seen, t.we2_fmt)
        exec_fmt_add(seen, t.we3_fmt)
    }
    // deltanet hybrids carry their own projection tags (kq-native since IMAGE_VERSION 5) — walk
    // the recurrent rows only (others hold resize defaults) or the receipt hides dn plane
    // formats (27B: k5 ssm_out ran unlisted). Empty arrays = not a deltanet model, add nothing.
    if (!empty(t.dnqkv_fmt) || !empty(t.dngate_fmt) || !empty(t.dnout_fmt)) {
        for (l in range64(t.config.n_layers)) {
            if (layer_is_recurrent(t.config, l)) {
                if (!empty(t.dnqkv_fmt)) { seen |> insert("{t.dnqkv_fmt[l]}") }
                if (!empty(t.dngate_fmt)) { seen |> insert("{t.dngate_fmt[l]}") }
                if (!empty(t.dnout_fmt)) { seen |> insert("{t.dnout_fmt[l]}") }
            }
        }
    }
    seen |> insert("{t.config.shared_weights ? t.emb_fmt : t.wcls_fmt}")
    var names <- [for (k in keys(seen)); k]
    sort(names)
    let weights = join(names, "/")
    let scales = t.wscale_f16 ? "f16 scales" : "f32 scales"
    var layout = "planar {active_kernel_backend()}"
    if (t.metal_blob) {
        layout = "metal blob"
    } elif (t.kq_repacked || t.mx4_repacked) {
        layout = "planar {active_kernel_backend()} (repacked)"
    }
    return "weights {weights} native; q8 activations; {scales}; {layout}"
}

//! The llama.cpp sibling's execution receipt, derived from the build flavor: every gguf tensor
//! runs in its disk format; the cpu backend additionally layout-repacks supported types at load
//! (GGML_CPU_REPACK is ON in both pinned ref flavors).
def lcpp_exec_fmt(backend : string) : string {
    return backend == "cpu" ? "native gguf formats; cpu runtime layout repack" : "native gguf formats"
}

//! True when `m` is present on disk AND in the requested tier. `sz` receives the file size (0 if absent).
def model_selected(m : LlmModel; tier : Tier; var sz : uint64&) : bool {
    let st = stat(model_path(m))
    sz = st.is_valid ? st.size : 0ul
    if (!st.is_valid) {
        return false
    }
    let is_large = st.size >= LARGE_TIER_BYTES
    if (tier == Tier.all) {
        return true
    }
    return tier == Tier.large ? is_large : !is_large
}

// ===== Platform block =====

//! The per-box platform header stored at the top of every profile JSON.
struct PlatformInfo {
    cpu : string
    os : string
    threads : int
    daslang : string
    llama_cpp : string
    date : string
    box_profile : string
    parsec_off : bool
    // core-pinning provenance ("das jobque mode 1; llama-bench --cpu-mask 0x55555555 --cpu-strict 1");
    // absent = unpinned (the m1 rig)
    @optional affinity : string
    // gen-kernel stamp provenance ("fname=suffix (manifest|fallback|reference)") — an untuned
    // box publishing fallback-tier numbers must be visible in the profile itself
    @optional tune : string
}

// Run `cmd`, return its trimmed stdout (first line only when `first_line`). Uses fread_to_eof (fio's
// pipe-safe reader) — fread(f) whole-file sizes via seek and returns empty on a non-seekable pipe.
def run_capture(cmd : string; first_line : bool) : string {
    var out = ""
    unsafe {
        popen(cmd) $(f) {
            out = fread_to_eof(f)
        }
    }
    out = strip(out)
    if (first_line) {
        let nl = find(out, "\n")
        if (nl >= 0) {
            out = slice(out, 0, nl)
        }
    }
    return out
}

// The running daslang's version, via the box's binary (DASLANG_BIN, default bin/daslang from the
// repo-root cwd these tools run in). Takes the last token of the first --version line ("0.6.3").
def das_version() : string {
    let bin = empty(get_env_variable("DASLANG_BIN")) ? "bin/daslang" : get_env_variable("DASLANG_BIN")
    let line = run_capture("{bin} --version", true)
    let toks <- split(line, " ")
    return empty(toks) ? "unknown" : toks[length(toks) - 1]
}

// True on a Windows box (cmd sets OS=Windows_NT). macOS/Linux leave it empty.
def private is_windows() : bool {
    return get_env_variable("OS") == "Windows_NT"
}

// Windows CPU marketing name from the registry — non-interactive (no `date`-style prompt/hang).
def private win_cpu_name() : string {
    let raw = run_capture("reg query \"HKLM\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0\" /v ProcessorNameString", false)
    for (ln in split(raw, "\n")) {
        let i = find(ln, "REG_SZ")
        if (i >= 0 && find(ln, "ProcessorNameString") >= 0) {
            return strip(slice(ln, i + length("REG_SZ")))
        }
    }
    return ""
}

// The CPU brand string: env override (DASLLAMA_CPU) → OS-native probe.
def private platform_cpu() : string {
    let ov = get_env_variable("DASLLAMA_CPU")
    if (!empty(ov)) {
        return ov
    }
    return is_windows() ? win_cpu_name() : run_capture("sysctl -n machdep.cpu.brand_string", true)
}

// The OS string: env override (DASLLAMA_OS) → OS-native probe (`ver` on Windows, sw_vers on macOS).
def private platform_os() : string {
    let ov = get_env_variable("DASLLAMA_OS")
    if (!empty(ov)) {
        return ov
    }
    if (is_windows()) {
        return run_capture("ver", true)
    }
    // sw_vers is macOS-only; on Linux both probes return "" and the join is a lone space —
    // strip so empty()==true and the record stores "unknown" (or set DASLLAMA_OS on those boxes)
    return strip("{run_capture("sw_vers -productName", true)} {run_capture("sw_vers -productVersion", true)}")
}

def private tune_row_str(r : TuneStatus) : string {
    let sfx = empty(r.suffix) ? "reference" : r.suffix
    return "{r.fname}={sfx} ({r.source})"
}

//! One-line gen-kernel stamp provenance — "" (omitted from JSON) when the driver declares no
//! `[tune_policy]` (tune_status is empty then).
def tune_summary() : string {
    var st <- tune_status()
    var parts <- [for (r in st); tune_row_str(r)]
    let s = join(parts, "; ")
    delete parts
    delete st
    return s
}

// Gather the platform block. `llama_cpp` (the reference commit) and `date` are passed in — the caller
// reads them from the freshly-run baseline / the environment so they stay honest per run. CPU/OS come
// from OS-native probes with DASLLAMA_CPU / DASLLAMA_OS env overrides for boxes without a clean probe.
def gather_platform(threads : int; llama_cpp, date, box_profile : string; parsec_off : bool) : PlatformInfo {
    let cpu = platform_cpu()
    let os = platform_os()
    let das_ver = das_version()
    return PlatformInfo(
        cpu = empty(cpu) ? "unknown" : cpu,
        os = empty(os) ? "unknown" : os,
        threads = threads,
        daslang = empty(das_ver) ? "unknown" : das_ver,
        llama_cpp = llama_cpp,
        date = date,
        box_profile = box_profile,
        parsec_off = parsec_off,
        tune = tune_summary())
}

// ===== Hardware block (public bench records — see modules/dasLLAMA/PUBLIC_BENCH_PLAN.md) =====

//! Auto-gathered hardware identity for a public bench record. Always probed, never typed by the
//! submitter — community records depend on this block being trustworthy. Probes are best-effort
//! per OS; a field the box cannot answer is omitted (`@optional`). Env overrides for boxes with
//! no clean probe: DASLLAMA_CPU, DASLLAMA_OS, DASLLAMA_GPU_NAME, DASLLAMA_RAM_GB.
struct HardwareInfo {
    cpu : string
    arch : string
    os : string
    @optional perf_cores : int    // performance cores (Apple asymmetric SoCs); 0 elsewhere
    @optional total_cores : int   // physical on macOS, logical on Windows/Linux (best-effort)
    @optional ram_gb : int
    @optional gpu : string
}

def private is_macos() : bool {
    return !is_windows() && run_capture("uname -s", true) == "Darwin"
}

def private platform_arch() : string {
    if (is_windows()) {
        return get_env_variable("PROCESSOR_ARCHITECTURE")
    }
    return run_capture("uname -m", true)
}

def private platform_ram_gb() : int {
    let ov = get_env_variable("DASLLAMA_RAM_GB")
    if (!empty(ov)) {
        return to_int(ov)
    }
    let gib = 1024.0 * 1024.0 * 1024.0
    if (is_windows()) {
        // wmic emits "TotalPhysicalMemory=68632934400"
        let raw = run_capture("wmic ComputerSystem get TotalPhysicalMemory /value", false)
        for (ln in split(raw, "\n")) {
            let i = find(ln, "=")
            if (i >= 0 && find(ln, "TotalPhysicalMemory") >= 0) {
                return int(to_float(strip(slice(ln, i + 1))) / gib + 0.5)
            }
        }
        return 0
    }
    if (is_macos()) {
        return int(to_float(run_capture("sysctl -n hw.memsize", true)) / gib + 0.5)
    }
    // linux /proc/meminfo: "MemTotal:       131842348 kB"
    let ln = run_capture("grep MemTotal /proc/meminfo", true)
    let i = find(ln, ":")
    if (i < 0) {
        return 0
    }
    let toks <- split(strip(slice(ln, i + 1)), " ")
    return empty(toks) ? 0 : int(to_float(toks[0]) / (1024.0 * 1024.0) + 0.5)
}

// after-colon tail of the first line of `raw` containing `key`; "" when absent
def private line_value(raw, key : string) : string {
    for (ln in split(raw, "\n")) {
        let k = find(ln, key)
        if (k >= 0) {
            let c = find(ln, ":", k)
            if (c >= 0) {
                return strip(slice(ln, c + 1))
            }
        }
    }
    return ""
}

def private platform_gpu() : string {
    let ov = get_env_variable("DASLLAMA_GPU_NAME")
    if (!empty(ov)) {
        return ov
    }
    if (is_windows()) {
        let raw = run_capture("wmic path win32_VideoController get name /value", false)
        for (ln in split(raw, "\n")) {
            let i = find(ln, "Name=")
            if (i >= 0) {
                return strip(slice(ln, i + length("Name=")))
            }
        }
        return ""
    }
    if (is_macos()) {
        let raw = run_capture("system_profiler SPDisplaysDataType", false)
        let chip = line_value(raw, "Chipset Model")
        let cores = line_value(raw, "Total Number of Cores")
        if (empty(chip)) {
            return ""
        }
        return empty(cores) ? chip : "{chip} ({cores}-core)"
    }
    let ln = run_capture("lspci | grep -iE \"vga|3d\"", true)
    let i = find(ln, ": ")
    return i >= 0 ? strip(slice(ln, i + 2)) : ""
}

def private platform_cores() : tuple<perf : int; total : int> {
    if (is_windows()) {
        return (perf = 0, total = env_int("NUMBER_OF_PROCESSORS", 0))
    }
    if (is_macos()) {
        return (
            perf = to_int(run_capture("sysctl -n hw.perflevel0.physicalcpu", true)),
            total = to_int(run_capture("sysctl -n hw.physicalcpu", true)))
    }
    return (perf = 0, total = to_int(run_capture("nproc", true)))
}

//! Probe this box's hardware identity (see `HardwareInfo`). One-shot — a handful of child
//! processes; call once per run, not per row.
def gather_hardware : HardwareInfo {
    let cpu = platform_cpu()
    let os = platform_os()
    let cores = platform_cores()
    return HardwareInfo(
        cpu = empty(cpu) ? "unknown" : cpu,
        arch = platform_arch(),
        os = empty(os) ? "unknown" : os,
        perf_cores = cores.perf,
        total_cores = cores.total,
        ram_gb = platform_ram_gb(),
        gpu = platform_gpu())
}

// ===== Baseline TSV I/O (reference tok/s the establisher writes, the generator reads) =====

//! One reference-tool baseline row for a model: llama.cpp prefill / emission tok/s (CPU), the lcpp
//! build commit, and the total parameter count llama-bench reported. -1 means "not measured".
struct LlmBaseline {
    model : string
    prefill_tok_s : double
    emission_tok_s : double
    lcpp_commit : string
    n_params : int64
}

// `suffix` distinguishes measurement axes on the same box (e.g. "_metal" for the GPU-offload sweep);
// the default "" is the canonical CPU baseline that gen_profile merges.
def baseline_llm_path(suffix : string = "") : string {
    return "modules/dasLLAMA/performance/baseline_llm_{box_name()}{suffix}.tsv"
}

def write_llm_baselines(rows : array<LlmBaseline>; suffix : string = "") {
    fopen(baseline_llm_path(suffix), "wb") $(f) {
        if (f == null) {
            return
        }
        fwrite(f, "model\tprefill_tok_s\temission_tok_s\tlcpp_commit\tn_params\n")
        for (r in rows) {
            fwrite(f, "{r.model}\t{r.prefill_tok_s}\t{r.emission_tok_s}\t{r.lcpp_commit}\t{r.n_params}\n")
        }
    }
}

def read_llm_baselines(suffix : string = "") : array<LlmBaseline> {
    var rows : array<LlmBaseline>
    let text = fread(baseline_llm_path(suffix))
    var first = true
    for (line in split(text, "\n")) {
        if (first) {   // header
            first = false
            continue
        }
        if (empty(strip(line))) {
            continue
        }
        let col <- split(line, "\t")
        if (length(col) < 5) {
            continue
        }
        rows |> emplace(LlmBaseline(
            model = col[0],
            prefill_tok_s = to_double(col[1]),
            emission_tok_s = to_double(col[2]),
            lcpp_commit = col[3],
            n_params = to_int64(col[4])))
    }
    return <- rows
}

// ===== Profile JSON (the deliverable — one per box, das + reference tok/s) =====

//! One phase (prefill or emission) of a model's LLM profile. `llama_cpp_tok_s` is @optional: when the
//! reference wasn't measured on this box it stays 0.0 and is omitted from the JSON (absent key = no ref).
struct PhaseRec {
    context : int
    das_tok_s : double
    @optional llama_cpp_tok_s : double
}

//! One text-LLM row in a profile: model + metadata, plus its prefill and emission phases.
struct LlmRec {
    model : string
    arch : string
    quant : string
    active_b : double
    prefill : PhaseRec
    emission : PhaseRec
}

//! A complete per-box LLM profile: the platform block + one row per measured model. Serialized with
//! `sprint_json` and round-tripped with `sscan_json` — the machine-readable handoff for the website.
struct LlmProfile {
    platform : PlatformInfo
    tests : array<LlmRec>
}

def profile_llm_path() : string {
    return "modules/dasLLAMA/performance/profile_llm_{box_name()}.json"
}

def write_llm_profile(p : LlmProfile) {
    let text = sprint_json(p, true)
    fopen(profile_llm_path(), "wb") $(f) {
        if (f != null) {
            fwrite(f, text)
        }
    }
}

// Load the existing per-box profile into `p` for a merge run; false if none exists yet.
def read_llm_profile(var p : LlmProfile) : bool {
    let path = profile_llm_path()
    if (!stat(path).is_valid) {
        return false
    }
    return sscan_json(fread(path), p)
}

// Update-or-insert `rec` into `tests` keyed by model name — lets a small-tier run and a later
// large-tier run merge into one file without clobbering each other's rows.
def upsert_llm(var tests : array<LlmRec>; rec : LlmRec) {
    for (c in tests) {
        if (c.model == rec.model) {
            c = rec
            return
        }
    }
    tests |> push(rec)
}

// ===== ASR: audio-model roots + corpus =====

// whisper.cpp model root (the ggml-*.bin parakeet/whisper carriers, shared by das and the *-cli
// references). Override with WHISPER_CPP_MODELS.
def whisper_models_dir() : string {
    let env = get_env_variable("WHISPER_CPP_MODELS")
    return empty(env) ? "/Users/borisbatkin/Work/whisper.cpp/models/" : env
}

// The bench corpus (relative to the repo-root cwd these tools run in).
def asr_corpus_dir() : string {
    return "modules/dasLLAMA/benchmarks/asr/corpus/"
}

// The five standard corpus buckets, shortest→longest (11 s .. 547 s).
def corpus_buckets() : array<string> {
    return <- ["jfk.wav", "jfk3.wav", "gb1.wav", "hp0.wav", "hp0x2.wav"]
}

// ===== ASR: the audio-model catalog =====

//! A reference tool bound to one ASR model + the per-tool handle to invoke it: the onnx variant key,
//! the NeMo HF id, or the mtmd mmproj basename. Empty when the tool loads the model's own das_model
//! directly (the whisper.cpp `parakeet-cli` / `whisper-cli`).
struct AsrRef {
    tool : string      // parakeet-cli | whisper-cli | onnx | nemo | mtmd
    handle : string
}

//! One ASR model to profile: the das-side load (basename under `root`, plus `mmproj` for the two-file
//! audio-chat models), its ordered reference tools, the corpus buckets to time, and a short tag (`v2` /
//! `tiny` …) — non-empty only for models that also join the LibriSpeech dictation aggregate.
struct AsrModelSpec {
    display : string
    das_model : string
    das_mmproj : string
    root : string
    refs : array<AsrRef>
    buckets : array<string>
    libri_short : string
    lang : string          // create_session language: "en" for the pure-ASR models, "auto" for audio-chat
}

// The audio models the suite profiles. Absent files are skipped at run time. The das side is uniform
// (load_asr_model + transcribe drives every entry); the heterogeneity is which reference tool(s) apply.
def asr_catalog() : array<AsrModelSpec> {
    let wroot = whisper_models_dir()
    let lroot = models_dir()
    return <- [
        AsrModelSpec(display = "Parakeet-TDT v2", das_model = "ggml-parakeet-tdt-0.6b-v2-f32.bin",
            das_mmproj = "", root = wroot,
            refs <- [AsrRef(tool = "parakeet-cli", handle = ""), AsrRef(tool = "onnx", handle = "v2-int8")],
            buckets <- corpus_buckets(), libri_short = "v2", lang = "en"),
        AsrModelSpec(display = "Parakeet-TDT v3", das_model = "ggml-parakeet-tdt-0.6b-v3-f32.bin",
            das_mmproj = "", root = wroot,
            refs <- [AsrRef(tool = "parakeet-cli", handle = ""), AsrRef(tool = "onnx", handle = "int8")],
            buckets <- corpus_buckets(), libri_short = "v3", lang = "en"),
        AsrModelSpec(display = "Whisper tiny", das_model = "ggml-tiny.bin",
            das_mmproj = "", root = wroot,
            refs <- [AsrRef(tool = "whisper-cli", handle = ""), AsrRef(tool = "onnx", handle = "whisper-tiny-int8")],
            buckets <- corpus_buckets(), libri_short = "tiny", lang = "en"),
        AsrModelSpec(display = "Whisper large-v3-turbo", das_model = "ggml-large-v3-turbo.bin",
            das_mmproj = "", root = wroot,
            refs <- [AsrRef(tool = "whisper-cli", handle = ""), AsrRef(tool = "onnx", handle = "whisper-turbo-int8")],
            buckets <- corpus_buckets(), libri_short = "", lang = "en"),
        AsrModelSpec(display = "Canary-Qwen 2.5B", das_model = "canary-qwen-2.5b-decoder-f16.gguf",
            das_mmproj = "canary-qwen-2.5b-encoder-f32.bin", root = lroot,
            refs <- [AsrRef(tool = "nemo", handle = "nvidia/canary-qwen-2.5b")],
            buckets <- ["jfk.wav", "jfk3.wav", "gb1.wav"], libri_short = "", lang = "auto"),
        AsrModelSpec(display = "Gemma-4 E2B audio", das_model = "gemma-4-E2B-it-Q8_0.gguf",
            das_mmproj = "mmproj-gemma-4-E2B-it-bf16.gguf", root = lroot,
            refs <- [AsrRef(tool = "mtmd", handle = "mmproj-gemma-4-E2B-it-bf16.gguf")],
            buckets <- ["gemma4a_test2.wav"], libri_short = "", lang = "auto"),
        AsrModelSpec(display = "Qwen3-Omni-30B audio", das_model = "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf",
            das_mmproj = "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf", root = lroot,
            refs <- [AsrRef(tool = "mtmd", handle = "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf")],
            buckets <- ["jfk.wav", "jfk3.wav"], libri_short = "", lang = "auto")
    ]
}

// ===== ASR: baseline TSV I/O (reference tool times the establisher writes, the generator reads) =====

//! One reference-tool baseline row: the tool, the das display name it references, the clip, its audio
//! seconds, the best-of-reps transcribe ms, and (mtmd only) the encode+prefill split (-1 = N/A). `kind`
//! is "corpus" (a named bucket) or "libri" (one of the 25 LibriSpeech dictation clips).
struct AsrBaseRow {
    tool : string
    model : string
    kind : string
    wav : string
    audio_s : double
    ms : double
    encode_ms : double
}

def baseline_asr_path() : string {
    return "modules/dasLLAMA/performance/baseline_asr_{box_name()}.tsv"
}

def write_asr_baselines(rows : array<AsrBaseRow>) {
    fopen(baseline_asr_path(), "wb") $(f) {
        if (f == null) {
            return
        }
        fwrite(f, "tool\tmodel\tkind\twav\taudio_s\tms\tencode_ms\n")
        for (r in rows) {
            fwrite(f, "{r.tool}\t{r.model}\t{r.kind}\t{r.wav}\t{r.audio_s}\t{r.ms}\t{r.encode_ms}\n")
        }
    }
}

def read_asr_baselines() : array<AsrBaseRow> {
    var rows : array<AsrBaseRow>
    let text = fread(baseline_asr_path())
    var first = true
    for (line in split(text, "\n")) {
        if (first) {
            first = false
            continue
        }
        if (empty(strip(line))) {
            continue
        }
        let col <- split(line, "\t")
        if (length(col) < 7) {
            continue
        }
        rows |> emplace(AsrBaseRow(
            tool = col[0], model = col[1], kind = col[2], wav = col[3],
            audio_s = to_double(col[4]), ms = to_double(col[5]), encode_ms = to_double(col[6])))
    }
    return <- rows
}

// ===== ASR: profile JSON (the deliverable — full-fidelity: N references/model, per-bucket ms) =====

//! One measured reference time for a bucket. `encode_ms` is @optional (mtmd only — the audio-tower
//! encode+prefill split); 0.0 means "not split-timed" and is omitted from the JSON.
struct AsrRefTime {
    tool : string
    ms : double
    @optional encode_ms : double
}

//! One corpus clip of an ASR model's profile: das transcribe ms (best-of-reps) + one reference time
//! per tool that ran (a tool that can't run this clip is absent — the renderer shows "-"). Both
//! `das_encode_ms` (mtmd-parity audio-chat models) are @optional.
struct AsrBucket {
    wav : string
    audio_s : double
    das_ms : double
    @optional das_encode_ms : double
    refs : array<AsrRefTime>
}

//! One text-per-side dictation statistic over the LibriSpeech short-clip set (per-clip latency case).
struct LibriStat {
    side : string
    mean_ms : double
    p50_ms : double
    p95_ms : double
}

//! The LibriSpeech dictation aggregate (25 short clips): one stat row per side (das/cli/onnx × model).
struct LibriAggregate {
    n_clips : int
    sides : array<LibriStat>
}

//! One ASR model row: display + das load + ordered reference tool names (the table's column order) +
//! one bucket per corpus clip.
struct AsrRec {
    model : string
    das_model : string
    references : array<string>
    buckets : array<AsrBucket>
}

//! A complete per-box ASR profile: platform block + one row per measured model + the LibriSpeech
//! dictation aggregate. Round-tripped with sprint_json/sscan_json — the machine-readable handoff.
struct AsrProfile {
    platform : PlatformInfo
    tests : array<AsrRec>
    librispeech : LibriAggregate
}

def profile_asr_path() : string {
    return "modules/dasLLAMA/performance/profile_asr_{box_name()}.json"
}

def write_asr_profile(p : AsrProfile) {
    let text = sprint_json(p, true)
    fopen(profile_asr_path(), "wb") $(f) {
        if (f != null) {
            fwrite(f, text)
        }
    }
}

// Load the existing per-box ASR profile into `p` for a merge run; false if none exists yet.
def read_asr_profile(var p : AsrProfile) : bool {
    let path = profile_asr_path()
    if (!stat(path).is_valid) {
        return false
    }
    return sscan_json(fread(path), p)
}

// Update-or-insert `rec` into `tests` keyed by model name (partial per-model runs merge into one file).
def upsert_asr(var tests : array<AsrRec>; var rec : AsrRec) {
    for (c in tests) {
        if (c.model == rec.model) {
            c <- rec
            return
        }
    }
    tests |> emplace(rec)
}

// ===== Public bench records (modules/dasLLAMA/PUBLIC_BENCH_PLAN.md) =====

//! One measured test row, keyed in `BenchRun.tests` by the llama-bench test name
//! ("pp512", "tg128", ...): mean tok/s over the timed reps ± sample stdev.
struct BenchTest {
    tok_s : double
    @optional stddev : double
}

//! One engine × backend × flavor run on one box — a first-class record. das and llama.cpp runs
//! are siblings in the same store; pairing and ratios are derived at render time, never stored.
//! `cmd` is the reproduction receipt: the exact invocation that produced the numbers.
struct BenchRun {
    engine : string          // "das" | "llama.cpp"
    backend : string         // "cpu" | "metal" | "vulkan"
    flavor : string          // das: "tuned" | "accel" (+AMX tier); llama.cpp build flavor: "stock" | "clean-cpu"
    box : string
    threads : int
    date : string            // YYYY-MM-DD
    cmd : string
    hardware : HardwareInfo
    tests : table<string; BenchTest>
    @optional source : string   // "official" | "community" — stamped by the store driver, not the tool
    @optional sha : string      // engine commit (das: git HEAD; llama.cpp: build_commit)
    @optional version : string  // das release version
    @optional tune : string     // das gen-kernel stamp provenance — required for reproducibility
    @optional exec_fmt : string // what this engine EXECUTES for this file (weight formats per site
                                // class + activation/scale treatment) — the apples-to-apples receipt;
                                // das derives it from the loaded Model, llama.cpp rows from flavor
    @optional env : string      // the arming environment `cmd` was run under — see [[bench_env_line]].
                                // Load-bearing: most GPU/tier levers arm via env, so a `cmd` alone
                                // does NOT reproduce a row and two configs can look identical
}

// Every env var the dasLLAMA engine + the tune rail consult. Capturing the WHOLE set (not a curated
// "interesting" subset) is deliberate: an unrecorded var is exactly how two different configs come
// to look like the same row.
let private BENCH_ENV_VARS = fixed_array(
    "DAS_JOBQUE_AFFINITY", "DAS_JOBQUE_TEAM_RANK_GATE", "DAS_JOBQUE_THREADS",
    "DAS_TUNE_MANIFEST", "DAS_TUNE_MODE",
    "DASLLAMA_ACCEL", "DASLLAMA_ACCEL_F16", "DASLLAMA_ACCEL_F16_STRIPS", "DASLLAMA_ACCEL_MIN_MMAC",
    "DASLLAMA_ACCEL_MIN_NTOK", "DASLLAMA_ALLOW_INTERP_LOAD", "DASLLAMA_BATCH_GRID_2D",
    "DASLLAMA_BENCH_VERBOSE", "DASLLAMA_BOX", "DASLLAMA_CALLER_PRIO", "DASLLAMA_CONV_PROF",
    "DASLLAMA_COOPMAT", "DASLLAMA_CPU", "DASLLAMA_CPU_PREFILL", "DASLLAMA_EXPERT_REUSE",
    "DASLLAMA_FUSED_DECODE", "DASLLAMA_GPU", "DASLLAMA_GPU_ATTN", "DASLLAMA_GPU_CLS",
    "DASLLAMA_GPU_COMBINE", "DASLLAMA_GPU_DENSE", "DASLLAMA_GPU_DENSE_ATTN",
    "DASLLAMA_GPU_DENSE_SHEXP", "DASLLAMA_GPU_DN", "DASLLAMA_GPU_DND", "DASLLAMA_GPU_HEAT",
    "DASLLAMA_GPU_MIN_CTX", "DASLLAMA_GPU_MOE_LAYERS", "DASLLAMA_GPU_MOE_STREAM",
    "DASLLAMA_GPU_NAME", "DASLLAMA_GPU_PROF", "DASLLAMA_GPU_QKV", "DASLLAMA_GPU_SHEXP",
    "DASLLAMA_GPU_VRAM_MB", "DASLLAMA_IMAGE", "DASLLAMA_METAL_ATTN", "DASLLAMA_METAL_ATTN_D",
    "DASLLAMA_METAL_ATTN_SINGLE", "DASLLAMA_METAL_BATCH_CONCURRENT", "DASLLAMA_METAL_BATCH_FUSE",
    "DASLLAMA_METAL_BATCH_GEMM_MIN", "DASLLAMA_METAL_BATCH_MM", "DASLLAMA_METAL_BATCH_MV",
    "DASLLAMA_METAL_BATCH_MV8", "DASLLAMA_METAL_BATCH_NCB", "DASLLAMA_METAL_BATCH_PIPE",
    "DASLLAMA_METAL_BATCH_SK", "DASLLAMA_METAL_DECODE_CONCURRENT", "DASLLAMA_METAL_DECODE_SKIP",
    "DASLLAMA_METAL_FUSE", "DASLLAMA_METAL_GEMV_TG", "DASLLAMA_METAL_HAZARD_PARANOID",
    "DASLLAMA_METAL_HAZARD_STRICT", "DASLLAMA_METAL_KQ_B8", "DASLLAMA_METAL_KV_MIRROR_MB",
    "DASLLAMA_METAL_LOGITS", "DASLLAMA_METAL_MULMM", "DASLLAMA_METAL_NCB",
    "DASLLAMA_METAL_PF_CAPTURE", "DASLLAMA_METAL_PIPE_DEBUG", "DASLLAMA_METAL_PREFILL_SKIP",
    "DASLLAMA_METAL_SCHED", "DASLLAMA_METAL_SPEC", "DASLLAMA_METAL_UNRETAINED",
    "DASLLAMA_MM_SMALL", "DASLLAMA_MODELS_DIR", "DASLLAMA_MTP_DEBUG", "DASLLAMA_NOISY",
    "DASLLAMA_OS", "DASLLAMA_PIN_BACKEND", "DASLLAMA_PIN_PREFILL", "DASLLAMA_RAM_GB",
    "DASLLAMA_THREADS", "DASLLAMA_TRUTH_REFRESH", "DASLLAMA_VK_FA", "DASLLAMA_VK_HAZARD_PARANOID",
    "DASLLAMA_VK_HAZARD_TRACE", "DASLLAMA_VK_MEMPRIO", "DASLLAMA_VK_REBAR", "DASLLAMA_VK_XFERQ")

// quote on anything a shell reads as syntax, not just spaces — full cross-shell escaping is not
// well-defined (cmd and POSIX disagree on the rules), so widen the trigger rather than pretend
def private shell_quote(s : string) : string {
    var bare = !empty(s)
    peek_data(s) $(arr) {
        for (b in arr) {
            let c = int(b)
            bare = bare && (is_alpha(c) || is_number(c) || c == '_' || c == '.' || c == '/'
                || c == '\\' || c == ':' || c == '=' || c == '-' || c == '+' || c == ',')
        }
    }
    return bare ? s : "\"{s}\""
}

//! The run's REAL invocation, from the process argv — binary, interpreter flags, script and args
//! alike. Not a reconstruction: a rebuilt `-- {parsed args}` string loses the binary actually used
//! and silently drops anything the arg parser did not model.
def bench_cmd_line() : string {
    var parts <- [for (a in get_command_line_arguments()); shell_quote(a)]
    let line = join(parts, " ")
    delete parts
    return line
}

//! The set members of [[BENCH_ENV_VARS]] that are actually set, as a copy-pasteable `K=V ...`
//! prefix. Pair with [[bench_cmd_line]] and the row reproduces exactly.
def bench_env_line() : string {
    var parts <- [for (name in BENCH_ENV_VARS);
        "{name}={shell_quote(get_env_variable(name))}";
        where has_env_variable(name)]
    let line = join(parts, " ")
    delete parts
    return line
}

//! One GGUF's identity plus every run measured against it. `gguf` (the exact filename) is the
//! store's primary key — it pins weights and quant.
struct BenchModel {
    gguf : string
    runs : array<BenchRun>
    @optional arch : string
    @optional quant : string
    @optional params_b : double    // total parameters, from llama-bench's model_n_params
    @optional active_b : double    // per-token active parameters (MoE-aware), from Config geometry
    @optional size_bytes : int64
    @optional note : string        // board caveat for this model (quant substitution, capability gap)
}

//! True on a macOS box — the gate for Metal cells in the record driver.
def is_apple_box : bool {
    return !is_windows() && run_capture("uname -s", true) == "Darwin"
}

//! The official per-box record store the site build merges.
def bench_records_path : string {
    return "modules/dasLLAMA/performance/records/{box_name()}.json"
}

def read_bench_records(path : string; var models : array<BenchModel>) : bool {
    if (!stat(path).is_valid) {
        return false
    }
    return sscan_json(fread(path), models)
}

def write_bench_records(path : string; models : array<BenchModel>) {
    let text = sprint_json(models, true)
    var wrote = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, text)
            wrote = true
        }
    }
    if (!wrote) {
        to_log(LOG_ERROR, "dasLLAMA bench: could not open {path} for writing — records NOT persisted\n")
    }
}

//! Update-or-insert one run; the identity is (box, engine, backend, flavor) — a re-measure
//! replaces its row in place (fresh date/sha), never appends a superseded copy.
def upsert_bench_run(var runs : array<BenchRun>; var r : BenchRun) {
    for (c in runs) {
        if (c.box == r.box && c.engine == r.engine && c.backend == r.backend && c.flavor == r.flavor) {
            c <- r
            return
        }
    }
    runs |> emplace(r)
}

//! Merge one incoming model (e.g. a --json-path cell from lcpp_bench) into the store: identity
//! fields refresh when the incoming side knows them, runs upsert per (box, engine, backend, flavor).
def upsert_bench_model(var models : array<BenchModel>; var incoming : BenchModel) {
    for (m in models) {
        if (m.gguf == incoming.gguf) {
            if (!empty(incoming.arch)) {
                m.arch = incoming.arch
            }
            if (!empty(incoming.quant)) {
                m.quant = incoming.quant
            }
            if (incoming.params_b > 0.0lf) {
                m.params_b = incoming.params_b
            }
            if (incoming.active_b > 0.0lf) {
                m.active_b = incoming.active_b
            }
            if (incoming.size_bytes > 0l) {
                m.size_bytes = incoming.size_bytes
            }
            if (!empty(incoming.note)) {
                m.note = incoming.note
            }
            for (i in range(length(incoming.runs))) {
                upsert_bench_run(m.runs, incoming.runs[i])
            }
            return
        }
    }
    models |> emplace(incoming)
}

// ===== llama-bench invocation (shared by lcpp_bench --ref and the establisher) =====

//! One llama-bench `-o json` result row (prefill rows have n_gen=0, decode rows n_prompt=0).
struct LlamaBenchRow {
    n_prompt : int
    n_gen : int
    avg_ts : double
    @optional stddev_ts : double
    model_n_params : int64
    build_commit : string
}

//! Run a llama-bench command line (must include `-o json`) and parse its rows. False on a run or
//! parse failure. Stdout only — llama-bench keeps progress on stderr, so the pipe stays clean.
def run_llama_bench(cmd : string; var rows : array<LlamaBenchRow>) : bool {
    var out = ""
    unsafe {
        popen(cmd) $(f) {
            if (f != null) {   // popen can hand a null handle on spawn failure (bad binary path)
                out = fread_to_eof(f)
            }
        }
    }
    return sscan_json(out, rows)
}
