options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options indenting = 4

// Shared foundation for the dasLLAMA profiling suite: the model catalog, the small/large tier gate,
// the per-box platform block, and the baseline-TSV I/O that the reference establisher writes and the
// das-side generator reads. The suite is deliberately self-contained (it does not cross-require the
// tests/ tier helper) so it lives entirely under modules/dasLLAMA/performance/.

module profile_common shared public

require daslib/fio public
require dasllama/dasllama_env  // the g_env_* knob globals ([EnvConfig] declarations; ENVIRONMENT.md is generated from them)
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
    return models_dir_resolved()   // ONE resolution, in dasllama_env — the tuner's confirm ladder shares it
}

// Short box tag baked into the JSON/TSV filenames (profile_llm_<box>.json). Override with DASLLAMA_BOX.
def box_name() : string {
    let env = g_env_prof.box
    return empty(env) ? "m1" : env
}

//! The per-box shared tune manifest (`DAS_TUNE_MANIFEST` target): ONE tune per box, so every
//! rig process — every cell — stamps the same winners and their .dlim image
//! identities agree. Gitignored (machine-local; staleness = older than the binary).
def public rig_tune_manifest() : string {
    return "modules/dasLLAMA/performance/{box_name()}.tune.json"
}

//! Profiling lane/thread count: min(16, physical performance cores), same count both engines,
//! pinned — one rule for every box (M1 Max 8, M4 Pro 10, M3 Air 4, big x86 16). Fixed by policy:
//! no per-engine detection, no thread matrix (METHODOLOGY.md carries the why). Override with
//! DASLLAMA_THREADS.
def profile_threads() : int {
    if (is_some(g_env_prof.threads)) {
        return max(unwrap(g_env_prof.threads), 1)
    }
    // Intel hybrid (12th-gen+/Core Ultra) tripwire: the rig pins against the slow cores on
    // Apple but has NO E-core avoidance on x86 yet — measuring through E-cores would publish
    // garbage that looks plausible. Fail loudly until P/E pinning is implemented.
    let cpu = platform_cpu()
    if (find(cpu, "Intel") >= 0 && (find(cpu, "th Gen") >= 0 || find(cpu, "Core(TM) Ultra") >= 0)) {
        panic("profile_threads: Intel hybrid CPU detected ({cpu}) - P/E pinning is not implemented; set DASLLAMA_THREADS + affinity manually or implement the P-core mask first")
    }
    let phys = physical_perf_cores()
    return phys > 0 ? min(16, phys) : 16
}

//! True when profiling lanes get explicit scheduling treatment. Windows/x64: pin to distinct
//! physical cores (OS placement is a lottery — SMT pairs measured pp512 bimodal on the 3990X).
//! darwin: jobque mode 2 maps to QoS classification — unclassified workers demote under ambient
//! load (big-model decode measured -5% tg). The llama-bench CPU mask stays Windows-only: lcpp_mask_on.
def affinity_on() : bool {
    return is_windows() || get_platform_name() == "darwin"
}

//! True when llama-bench gets explicit CPU-mask arguments — Windows only; macOS llama-bench has
//! no thread-affinity support, and the das-side darwin treatment (QoS) has no lcpp mirror.
def lcpp_mask_on() : bool {
    return is_windows()
}

//! llama-bench affinity arguments — an empty array when lcpp_mask_on() is false: the first
//! `threads` physical cores as a logical-CPU mask — SMT siblings are adjacent on Windows, so even
//! bits (16 -> 0x55555555) — plus --cpu-strict so the pins hold. The das-side mirror is jobque affinity.
def lcpp_affinity_argv(threads : int) : array<string> {
    var args : array<string>
    if (lcpp_mask_on()) {
        var mask = 0ul
        for (i in range(min(threads, 32))) {
            mask |= 1ul << uint64(2 * i)
        }
        args <- ["--cpu-mask", "0x{mask:x}", "--cpu-strict", "1"]
    }
    return <- args
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

//! Run a child STREAMING: every line it emits (stdout+stderr, merged by popen_argv) prints to OUR
//! stdout the moment it arrives — the visibility contract: stdout alone tells how far the process
//! is and what it does, at every stage, without waiting for the child to exit. A rolling tail is
//! retained for failure reporting. Returns the exit code.
def run_and_stream(args : array<string>; var tail_out : string&; keep_tail : int = 4096) : int {
    var acc = ""
    let rc = unsafe(popen_argv(args, 0.0, $(f) {
        if (f == null) {
            return
        }
        while (true) {
            let line = fgets(f)
            if (empty(line)) {   // fgets keeps the newline, so "" only at EOF
                break
            }
            print(line)
            acc = "{acc}{line}"
            if (length(acc) > keep_tail * 2) {
                acc = slice(acc, length(acc) - keep_tail)
            }
        }
    }))
    tail_out = length(acc) > keep_tail ? slice(acc, length(acc) - keep_tail) : acc
    return rc
}

//! Capture twin of [[run_and_stream]] — argv spawn (no shell parse, the Windows cmd quoting
//! traps can't bite), stdout+stderr merged by popen_argv, whole output returned trimmed.
//! Reads via the same fgets loop as run_and_stream — predates fread's pipe-safe EOF
//! fallback, and the line loop has never dropped output, so it stays.
def run_capture_argv(args : array<string>) : string {
    var acc = ""
    unsafe {
        popen_argv(args, 0.0, $(f) {
            if (f == null) {   // popen_argv can hand a null handle on spawn failure (bad binary path)
                return
            }
            while (true) {
                let line = fgets(f)
                if (empty(line)) {   // fgets keeps the newline, so "" only at EOF
                    break
                }
                acc = "{acc}{line}"
            }
        })
    }
    return strip(acc)
}

//! The display form of an argv for receipts — one line, elements with spaces quoted.
def argv_line(args : array<string>) : string {
    let line = build_string() $(w) {
        var first = true
        for (a in args) {
            if (!first) {
                w |> write(" ")
            }
            first = false
            w |> write(find(a, " ") >= 0 ? "\"{a}\"" : a)
        }
    }
    return "{line}"
}

// Run `cmd`, return its trimmed stdout (first line only when `first_line`). fread reads a
// pipe to EOF (the fio fallback for non-regular files), so the whole stream arrives.
def run_capture(cmd : string; first_line : bool) : string {
    var out = ""
    unsafe {
        popen(cmd) $(f) {
            if (f != null) {   // popen can hand a null handle on spawn failure (bad binary path)
                out = fread(f)
            }
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
    // shell-free spawn (fio's run_and_capture → popen_argv), so no cmd and no quote/option traps;
    // CreateProcess still wants backslashes in the program path (probe-verified: `bin/daslang`
    // fails to spawn, `bin\daslang` runs), hence the normalize
    var bin = g_env_prof.daslang_bin
    if (is_windows()) {
        bin = replace(bin, "/", "\\")
    }
    var out = ""
    if (run_and_capture([bin, "--version"], out) != 0) {
        return "unknown"
    }
    var line = strip(out)
    let nl = find(line, "\n")
    if (nl >= 0) {
        line = slice(line, 0, nl)
    }
    let toks <- split(line, " ")
    return empty(toks) ? "unknown" : toks[length(toks) - 1]
}

// True on a Windows box (cmd sets OS=Windows_NT). macOS/Linux leave it empty.
def private is_windows() : bool {
    return g_env_prof.win_os == "Windows_NT"
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
    let ov = g_env_prof.cpu
    if (!empty(ov)) {
        return ov
    }
    if (is_windows()) {
        return win_cpu_name()
    }
    if (is_macos()) {
        return run_capture("sysctl -n machdep.cpu.brand_string", true)
    }
    // linux: after-colon tail of `model name : AMD Ryzen 7 PRO 8700GE ...`
    return strip(line_value(run_capture("grep -m1 \"model name\" /proc/cpuinfo", true), "model name"))
}

// The OS string: env override (DASLLAMA_OS) → OS-native probe (`ver` / sw_vers / os-release+kernel).
def private platform_os() : string {
    let ov = g_env_prof.os_name
    if (!empty(ov)) {
        return ov
    }
    if (is_windows()) {
        return run_capture("ver", true)
    }
    if (is_macos()) {
        return strip("{run_capture("sw_vers -productName", true)} {run_capture("sw_vers -productVersion", true)}")
    }
    // linux: PRETTY_NAME from os-release + kernel, e.g. `Debian GNU/Linux 12 (bookworm) 6.1.0-49-amd64`
    return strip("{run_capture(". /etc/os-release && echo $PRETTY_NAME", true)} {run_capture("uname -r", true)}")
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

//! The bench cells' provenance gate: REFUSE to measure on fallback kernel winners. A
//! stale/absent sidecar silently downgrades every ``[tune]`` kernel with only [I]-level
//! warnings — the m4 pilot shipped a full board that way. Three worlds, because
//! ``tune_status()`` populates in exactly one of them:
//! * **standalone exe** (the board rig): winners are BAKED at build time, so the status table
//!   is empty by design — provenance is the sidecar the release shipped beside the binary.
//! * **``DAS_TUNE_MANIFEST`` pinned**: stamps apply, status stays empty (framework gap,
//!   ledgered) — gate on the FILE: present, newer than the binary, carrying kernels.
//! * **plain script**: the policy rail ran, so every row must stamp a manifest winner.
//! ``DASLLAMA_ALLOW_UNTUNED=1`` is the dev-run escape in all three (it also suppresses the
//! auto-mint at ``[init]`` — see ``dasllama_math_gen_register`` — so the gate is reached at all).
def tune_gate() : bool {
    var why = ""
    let mpath = g_env_core.tune_manifest
    if (is_standalone_exe()) {
        // the release ships <exe>.tune.json from the same mint that baked the winners in
        let sc = tune_manifest_path()
        if (!stat(sc).is_valid) {
            why = "no sidecar beside the exe ({sc}) - the release did not ship its mint"
        } elif (find(fread(sc), "\"kernels\"") < 0) {
            why = "the exe's sidecar ({sc}) carries no kernels section - not a minted manifest"
        }
    } elif (!empty(mpath)) {
        var mst : FStat
        var bst : FStat
        let args <- get_command_line_arguments()
        if (!stat(mpath, mst)) {
            why = "DAS_TUNE_MANIFEST='{mpath}' does not exist - every kernel runs its fallback"
        } elif (stat(args[0], bst) && bst.mtime > mst.mtime) {
            why = "DAS_TUNE_MANIFEST='{mpath}' is OLDER than the binary ({args[0]}) - the staleness rule drops every kernel to its fallback"
        } elif (find(fread(mpath), "\"kernels\"") < 0) {
            why = "DAS_TUNE_MANIFEST='{mpath}' carries no kernels section - not a minted manifest"
        }
    } else {
        // per-script sidecar world: tune_status populates — require all-manifest stamps
        var st <- tune_status()
        var total = 0
        var bad = 0
        var sample = ""
        for (r in st) {
            total++
            if (r.source != "manifest") {
                bad++
                if (empty(sample)) {
                    sample = tune_row_str(r)
                }
            }
        }
        delete st
        if (total == 0) {
            why = "tune_status is EMPTY - the cell cannot prove its kernel provenance"
        } elif (bad > 0) {
            why = "{bad}/{total} kernels stamp non-manifest winners (first: {sample}) - stale or absent sidecar"
        }
    }
    if (empty(why)) {
        return true
    }
    if (g_env_bench.allow_untuned) {
        to_log(LOG_WARNING, "tune gate: {why} - measuring anyway (DASLLAMA_ALLOW_UNTUNED=1, numbers are NOT publishable)\n")
        return true
    }
    to_log(LOG_ERROR, "tune gate: {why}. Mint with: DAS_TUNE_MODE=tune bin/daslang -jit modules/dasLLAMA/harness/dasllama_tuner.das -dasroot <repo>; DASLLAMA_ALLOW_UNTUNED=1 overrides for dev runs\n")
    return false
}

//! True when a das run cannot name the winners it measured under — an empty ``tune`` or
//! ``tune_sha`` stamp. The store-time gate reads this: the M4 board stored 7 such rows under a
//! mis-shipped stale sidecar (the file-level gate passed — the sidecar WAS a valid old mint;
//! the staleness drop to fallback happened later, leaving stamps empty and nothing checking).
def das_run_unstamped(r : BenchRun) : bool {
    return r.engine == "das" && (empty(r.tune) || empty(r.tune_sha))
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

// ===== Hardware block (public bench records — schema in modules/dasLLAMA/METHODOLOGY.md) =====

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
    @optional model_id : string   // machine model (mac: hw.model e.g. "MacBookPro18,2")
    @optional ram_config : string // module layout ("4×32 GB DDR4-3200") — bandwidth identity on PC
    @optional power_plan : string // Windows active power scheme — throttling identity
    @optional smt : string        // "on" | "off" when determinable; "" elsewhere
    @optional remote_desktop : string  // measurement-time probe: "parsec" when parsecd ran,
                                       // "off" = probed clean, "" = never probed (legacy row)
}

// The remote-desktop probe: parsecd's GPU encoder moved Metal cell times 1.7x between runs
// (tune-hardening measurements) — "Parsec off" is a standing bench rule, so the row records
// whether it actually was. Process-list probe; "off" is a probed verdict, "" means unprobed.
def private platform_remote_desktop() : string {
    if (is_windows()) {
        let out = run_capture("tasklist /FI \"IMAGENAME eq parsecd.exe\" 2>nul", false)
        return find(out, "parsecd") >= 0 ? "parsec" : "off"
    }
    // macOS ships its OWN daemon named parsecd (CoreParsec.framework, a Spotlight helper) —
    // only the Parsec app's daemon counts, so list full command lines and drop the system one
    let out = run_capture("pgrep -fl parsecd 2>/dev/null | grep -v CoreParsec.framework", true)
    return empty(out) ? "off" : "parsec"
}

def private is_macos() : bool {
    return !is_windows() && run_capture("uname -s", true) == "Darwin"
}

def private platform_arch() : string {
    if (is_windows()) {
        return g_env_prof.win_arch
    }
    return run_capture("uname -m", true)
}

def private platform_ram_gb() : int {
    if (is_some(g_env_prof.ram_gb)) {
        return unwrap(g_env_prof.ram_gb)
    }
    let gib = 1024.0 * 1024.0 * 1024.0
    if (is_windows()) {
        // wmic emits "TotalPhysicalMemory=68632934400"; clean Windows 11 24H2+ installs ship
        // without wmic (it survives in-place upgrades), so fall through to CIM when it yields
        // nothing — and 2>nul keeps the miss off the console
        let raw = run_capture("wmic ComputerSystem get TotalPhysicalMemory /value 2>nul", false)
        for (ln in split(raw, "\n")) {
            let i = find(ln, "=")
            if (i >= 0 && find(ln, "TotalPhysicalMemory") >= 0) {
                return int(to_float(strip(slice(ln, i + 1))) / gib + 0.5)
            }
        }
        let ps = run_capture("powershell -NoProfile -Command \"(Get-CimInstance Win32_ComputerSystem).TotalPhysicalMemory\"", true)
        return empty(ps) ? 0 : int(to_float(ps) / gib + 0.5)
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

// display-adapter names that belong to a virtual/remote stack (Parsec, RDP, Hyper-V, VMware,
// Citrix, the driverless fallback), not the physical GPU; matched case-insensitively
def private is_virtual_gpu(name : string) : bool {
    let low = to_lower(name)
    for (marker in fixed_array("virtual", "remote display", "basic display", "hyper-v", "vmware", "citrix")) {
        if (find(low, marker) >= 0) {
            return true
        }
    }
    return false
}

// first non-virtual adapter, else the first at all — remote-desktop stacks enumerate their
// virtual display ahead of the physical GPU
def private pick_gpu_name(names : array<string>) : string {
    for (name in names) {
        if (!is_virtual_gpu(name)) {
            return name
        }
    }
    return empty(names) ? "" : names[0]
}

def private platform_gpu() : string {
    let ov = g_env_prof.gpu_name
    if (!empty(ov)) {
        return ov
    }
    if (is_windows()) {
        // wmic emits "Name=..." lines where present; clean Windows 11 24H2+ installs ship without
        // it (survives in-place upgrades), so fall through to the same probe via CIM
        var inscope names : array<string>
        let raw = run_capture("wmic path win32_VideoController get name /value 2>nul", false)
        for (ln in split(raw, "\n")) {
            let i = find(ln, "Name=")
            if (i >= 0) {
                names |> push(strip(slice(ln, i + length("Name="))))
            }
        }
        if (empty(names)) {
            let ps = run_capture("powershell -NoProfile -Command \"(Get-CimInstance Win32_VideoController).Name\"", false)
            for (ln in split(ps, "\n")) {
                let name = strip(ln)
                if (!empty(name)) {
                    names |> push(name)
                }
            }
        }
        return pick_gpu_name(names)
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
        return (perf = 0, total = g_env_prof.win_ncpu)
    }
    if (is_macos()) {
        return (
            perf = to_int(run_capture("sysctl -n hw.perflevel0.physicalcpu", true)),
            total = to_int(run_capture("sysctl -n hw.physicalcpu", true)))
    }
    return (perf = 0, total = to_int(run_capture("nproc", true)))
}

//! Physical core count — the thread-policy input ([[profile_threads]]): performance cores on
//! Apple, `wmic cpu get NumberOfCores` on Windows (logical count would double-count SMT), lscpu
//! math on Linux. 0 when the box can't answer (policy then falls back to its cap).
def physical_perf_cores() : int {
    if (is_windows()) {
        let raw = run_capture("wmic cpu get NumberOfCores /value", false)
        for (ln in split(raw, "\n")) {
            let i = find(ln, "=")
            if (i >= 0 && find(ln, "NumberOfCores") >= 0) {
                return to_int(strip(slice(ln, i + 1)))
            }
        }
        return 0
    }
    if (is_macos()) {
        return to_int(run_capture("sysctl -n hw.perflevel0.physicalcpu", true))
    }
    // cores per socket × sockets; lscpu -p would need parsing, the two -e fields are stable
    let cps = to_int(run_capture("lscpu | grep \"Core(s) per socket\" | grep -oE \"[0-9]+\"", true))
    let sk = to_int(run_capture("lscpu | grep \"Socket(s)\" | grep -oE \"[0-9]+\"", true))
    return cps * max(sk, 1)
}

// mac machine model ("MacBookPro18,2" / "Mac16,11") — the two-different-M1-Pros disambiguator
def private platform_model_id() : string {
    return is_macos() ? run_capture("sysctl -n hw.model", true) : ""
}

// RAM module layout — the bandwidth identity on PC ("4×32 GB DDR4-3200"). Apple unified memory
// has no modules; the chip name already implies the bandwidth class there.
def private platform_ram_config() : string {
    if (!is_windows()) {
        return ""
    }
    // wmic emits one block per module: "Capacity=34359738368\n...\nSpeed=3200"
    let raw = run_capture("wmic memorychip get Capacity,Speed /value", false)
    var count = 0
    var cap_gb = 0
    var speed = 0
    for (ln in split(raw, "\n")) {
        let i = find(ln, "=")
        if (i < 0) {
            continue
        }
        if (find(ln, "Capacity") >= 0) {
            count++
            cap_gb = int(to_float(strip(slice(ln, i + 1))) / (1024.0 * 1024.0 * 1024.0) + 0.5)
        } elif (find(ln, "Speed") >= 0) {
            speed = to_int(strip(slice(ln, i + 1)))
        }
    }
    if (count == 0) {
        return ""
    }
    return speed > 0 ? "{count}x{cap_gb} GB @ {speed} MT/s" : "{count}x{cap_gb} GB"
}

// Windows active power scheme name — a "Power saver" sweep is not a "High performance" sweep
def private platform_power_plan() : string {
    if (!is_windows()) {
        return ""
    }
    let ln = run_capture("powercfg /getactivescheme", true)
    let i = find(ln, "(")
    let j = find(ln, ")")
    return (i >= 0 && j > i) ? slice(ln, i + 1, j) : ""
}

// SMT state, where logical vs physical counts can answer it (Windows/Linux; Apple has no SMT)
def private platform_smt() : string {
    if (is_windows()) {
        let phys = physical_perf_cores()
        let logical = g_env_prof.win_ncpu
        if (phys > 0 && logical > 0) {
            return logical > phys ? "on" : "off"
        }
        return ""
    }
    if (is_macos()) {
        return ""
    }
    let tpc = to_int(run_capture("lscpu | grep \"Thread(s) per core\" | grep -oE \"[0-9]+\"", true))
    return tpc > 1 ? "on" : (tpc == 1 ? "off" : "")
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
        gpu = platform_gpu(),
        model_id = platform_model_id(),
        ram_config = platform_ram_config(),
        power_plan = platform_power_plan(),
        remote_desktop = platform_remote_desktop(),
        smt = platform_smt())
}

// ===== Input-file identity (.sha sidecars) =====

// process-lifetime cache: path -> "sha256\tsize\tmtime" (the sidecar payload)
var private g_sha_cache : table<string; string>

// hex sha256 of a file via the OS tool; "" on failure
def private compute_sha256(path : string) : string {
    var out = ""
    if (is_windows()) {
        // certutil: line 2 is the digest (spaced hex on older builds — strip non-hex)
        let raw = run_capture("certutil -hashfile \"{path}\" SHA256", false)
        var lines <- split(raw, "\n")
        if (length(lines) >= 2) {
            out = to_lower(replace(strip(lines[1]), " ", ""))
        }
        delete lines
    } else {
        let cmd = is_macos() ? "shasum -a 256 \"{path}\"" : "sha256sum \"{path}\""
        let ln = run_capture(cmd, true)
        var toks <- split(ln, " ")
        if (!empty(toks)) {
            out = to_lower(toks[0])
        }
        delete toks
    }
    return length(out) == 64 ? out : ""
}

//! Content identity of one input file, served from a `{path}.sha` sidecar so a 22 GB gguf is
//! hashed once per box, not once per run. The sidecar is `sha256\tsize\tmtime`; a size or mtime
//! mismatch (re-download, re-quantize) recomputes and rewrites it. `sha256` is "" only when the
//! file is missing or the box has no hash tool — callers store the InputFile regardless, so the
//! gap is visible in the record rather than silently absent.
    //! sha256 of this process's tune sidecar — the sidecar GENERATION a measurement runs under.
    //! Rows minted under different generations are not comparable; "" when the sidecar is absent.
def tune_manifest_sha() : string {
    return sidecar_generation_sha(tune_manifest_path())
}

    //! sha256 of an explicit sidecar file — the oracle's current-generation probe; "" when absent.
def sidecar_generation_sha(path : string) : string {
    return stat(path).is_valid ? compute_sha256(path) : ""
}

def file_identity(path : string; role : string) : InputFile {
    var fi = InputFile(role = role, name = base_name(path))
    let st = stat(path)
    if (!st.is_valid) {
        return fi
    }
    fi.bytes = int64(st.size)
    let stamp = "{st.size}\t{format_time(st.mtime, "%Y-%m-%dT%H:%M:%S")}"
    if (key_exists(g_sha_cache, path)) {
        let hit = g_sha_cache?[path] ?? ""
        if (ends_with(hit, stamp)) {
            fi.sha256 = slice(hit, 0, find(hit, "\t"))
            return fi
        }
    }
    let side = "{path}.sha"
    if (stat(side).is_valid) {
        let txt = strip(fread(side))
        var cols <- split(txt, "\t")
        if (length(cols) == 3 && "{cols[1]}\t{cols[2]}" == stamp && length(cols[0]) == 64) {
            fi.sha256 = cols[0]
            g_sha_cache[path] = "{cols[0]}\t{stamp}"
            delete cols
            return fi
        }
        delete cols
    }
    let sha = compute_sha256(path)
    if (empty(sha)) {
        to_log(LOG_WARNING, "dasLLAMA bench: no sha256 for {path} - identity recorded without hash\n")
        return fi
    }
    fi.sha256 = sha
    g_sha_cache[path] = "{sha}\t{stamp}"
    fopen(side, "wb") $(f) {
        if (f != null) {
            fwrite(f, "{sha}\t{stamp}\n")
        }
    }
    return fi
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
    let env = g_env_prof.whisper_cpp_models
    if (empty(env)) {
        return "{whisper_cpp_root()}/models/"
    }
    // callers concat "{whisper_models_dir()}{file}" — guarantee the separator (a slashless
    // override silently skipped every whisper/parakeet catalog entry on the m4 pilot)
    return ends_with(env, "/") || ends_with(env, "\\") ? env : "{env}/"
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
    @optional exec_fmt : string   // the reference's precision, stamped onto its BenchRun rows
    // non-empty = this reference times ONLY these clips (a slow tool capped below the
    // spec's ladder); empty = the spec's buckets
    @optional buckets : array<string>
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
    // DECLARED, not derived from file size the way the LLM tier is: ASR cost is clip-duration x
    // model-speed, and bytes mis-sort it — large-v3-turbo is 1.51 GB against Parakeet's 2.33 GB
    // yet costs 97.5 s per rep against Parakeet's 38.6 s (m1, the five corpus buckets).
    tier : Tier = Tier.small
}

//! Full path to the spec's das-side carrier — the one source for what four call sites spelled
//! inline as "{spec.root}{spec.das_model}".
def asr_model_path(spec : AsrModelSpec) : string => "{spec.root}{spec.das_model}"

//! True when the model's carrier is on disk AND it is in the requested tier — the ASR twin of
//! model_selected. The tier is the spec's own field; only presence is probed here.
def asr_model_selected(spec : AsrModelSpec; tier : Tier) : bool {
    if (!stat(asr_model_path(spec)).is_valid) {
        return false
    }
    return tier == Tier.all || tier == spec.tier
}

// The audio models the suite profiles. Absent files are skipped at run time. The das side is uniform
// (load_asr_model + transcribe drives every entry); the heterogeneity is which reference tool(s) apply.
def asr_catalog() : array<AsrModelSpec> {
    let wroot = whisper_models_dir()
    let lroot = models_dir()
    return <- [
        // libri_short only on Parakeet v3 — the LibriSpeech aggregate is deliberately scoped to
        // the fastest model (one-model dictation stats; anything wider is box-hours, not signal)
        // cli references run the q8_0 siblings (setup_asr_rig --refs quantizes them from the
        // fp bins via the upstream quantize tools): apples with the das q8 default AND the
        // references' own measured-best config on-box
        AsrModelSpec(display = "Parakeet-TDT v2", das_model = "ggml-parakeet-tdt-0.6b-v2-f32.bin",
            das_mmproj = "", root = wroot,
            refs <- [AsrRef(tool = "parakeet-cli", handle = "ggml-parakeet-tdt-0.6b-v2-q8_0.bin", exec_fmt = "q8_0"),
                     AsrRef(tool = "onnx", handle = "v2-int8", exec_fmt = "int8")],
            buckets <- corpus_buckets(), libri_short = "", lang = "en"),
        AsrModelSpec(display = "Parakeet-TDT v3", das_model = "ggml-parakeet-tdt-0.6b-v3-f32.bin",
            das_mmproj = "", root = wroot,
            refs <- [AsrRef(tool = "parakeet-cli", handle = "ggml-parakeet-tdt-0.6b-v3-q8_0.bin", exec_fmt = "q8_0"),
                     AsrRef(tool = "onnx", handle = "int8", exec_fmt = "int8")],
            buckets <- corpus_buckets(), libri_short = "v3", lang = "en"),
        AsrModelSpec(display = "Whisper tiny", das_model = "ggml-tiny.bin",
            das_mmproj = "", root = wroot,
            refs <- [AsrRef(tool = "whisper-cli", handle = "ggml-tiny-q8_0.bin", exec_fmt = "q8_0"),
                     AsrRef(tool = "onnx", handle = "whisper-tiny-int8", exec_fmt = "int8")],
            buckets <- corpus_buckets(), libri_short = "", lang = "en"),
        AsrModelSpec(display = "Whisper large-v3-turbo", das_model = "ggml-large-v3-turbo.bin",
            das_mmproj = "", root = wroot,
            refs <- [AsrRef(tool = "whisper-cli", handle = "ggml-large-v3-turbo-q8_0.bin", exec_fmt = "q8_0"),
                     AsrRef(tool = "onnx", handle = "whisper-turbo-int8", exec_fmt = "int8")],
            buckets <- corpus_buckets(), libri_short = "", lang = "en", tier = Tier.large),
        // canary caps at gb1: the model's trained envelope is 40 s audio / 1024 total tokens
        // (model card) — hp0-class single-window greedy decode runaway-loops on repetitive
        // audio in BOTH engines, the model's limitation, not a budget choice; 40 s chunked
        // long-form (the NVIDIA-sanctioned usage) is ledgered
        AsrModelSpec(display = "Canary-Qwen 2.5B", das_model = "canary-qwen-2.5b-decoder-f16.gguf",
            das_mmproj = "canary-qwen-2.5b-encoder-f32.bin", root = lroot,
            refs <- [AsrRef(tool = "nemo", handle = "nvidia/canary-qwen-2.5b", exec_fmt = "bf16")],
            buckets <- ["jfk.wav", "jfk3.wav", "gb1.wav"], libri_short = "", lang = "auto", tier = Tier.large),
        // jfk3 (33 s) replaced gemma4a_test2.wav — the one corpus clip with no traceable
        // provenance; every published input must be re-fetchable (B2/B3)
        // gemma4a caps at hp0: its audio tokenizer runs ~25 tok/s, so hp0x2 (547 s) is 13.7k
        // tokens against the 8192 decoder window — a single-window architectural limit
        // (asr_ctx_guard panics), the onnx-rel-pos class of honest cap, not a budget choice
        AsrModelSpec(display = "Gemma-4 E2B audio", das_model = "gemma-4-E2B-it-Q8_0.gguf",
            das_mmproj = "mmproj-gemma-4-E2B-it-bf16.gguf", root = lroot,
            // mtmd's CPU Conformer encode runs near xRT 1 — same per-rep budget cap as omni's
            refs <- [AsrRef(tool = "mtmd", handle = "mmproj-gemma-4-E2B-it-bf16.gguf",
                            exec_fmt = "bf16/q8_0", buckets <- ["jfk.wav", "jfk3.wav", "gb1.wav"])],
            buckets <- ["jfk.wav", "jfk3.wav", "gb1.wav", "hp0.wav"], libri_short = "", lang = "auto", tier = Tier.large),
        AsrModelSpec(display = "Qwen3-Omni-30B audio", das_model = "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf",
            das_mmproj = "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf", root = lroot,
            // mtmd's CPU-only AuT encode runs near xRT 1 (and far worse on zen2) — the long
            // clips would blow the per-rep budget, so this reference stays on the short ladder
            refs <- [AsrRef(tool = "mtmd", handle = "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf",
                            exec_fmt = "bf16/q8_0", buckets <- ["jfk.wav", "jfk3.wav", "gb1.wav"])],
            buckets <- corpus_buckets(), libri_short = "", lang = "auto", tier = Tier.large)
    ]
}

// ===== ASR: reference tools (paths, parsers, runners, versions) =====

//! "audio-chat" when the model's reference is llama.cpp's multimodal CLI, else "asr" — the
//! `BenchRun.workload` classifier, derived from the catalog rather than a hand-kept model list.
def asr_workload(spec : AsrModelSpec) : string {
    for (rf in spec.refs) {
        if (rf.tool == "mtmd") {
            return "audio-chat"
        }
    }
    return "asr"
}

//! The `BenchRun.engine` name for a reference tool — the tool IS the engine identity here
//! (parakeet-cli lives in the whisper.cpp tree, but the row names what actually ran).
def asr_engine_name(tool : string) : string {
    return tool == "mtmd" ? "llama-mtmd-cli" : tool
}

// Box-neutral defaults: every reference lands where setup_asr_rig puts it — the checkouts as
// siblings of the repo root, the venvs inside the repo — so a fresh box needs NO exports
// (the env vars remain the overrides). A laptop-absolute default here is a BRINGUP bug.
def whisper_cpp_root() : string {
    return g_env_prof.whisper_cpp
}

def whisper_cpp_bin(cli : string) : string {
    let root = whisper_cpp_root()
    // Locate the built binary across cmake layouts: MSVC multi-config (build\bin\Release\x.exe),
    // Ninja/single-config (build/bin/x.exe), Mac/Make (build/bin/x). stat picks the one that exists.
    for (c in [normalize("{root}/build/bin/Release/{cli}.exe"),
               normalize("{root}/build/bin/{cli}.exe"),
               normalize("{root}/build/bin/{cli}")]) {
        if (stat(c).is_valid) {
            return c
        }
    }
    return normalize("{root}/build/bin/{cli}")
}

//! A venv's interpreter across layouts — POSIX puts it in bin/, Windows in Scripts/.
def venv_python(venv : string) : string {
    // native separators: cmd parses a bare `foo/Scripts/python.exe` as command `foo` + switches
    let posix = "{venv}/bin/python"
    return normalize(stat(posix).is_valid ? posix : "{venv}/Scripts/python.exe")
}

def onnx_py() : string {
    return normalize(empty(g_env_prof.onnx_py) ? venv_python("modules/dasLLAMA/benchmarks/asr/.venv-onnx") : g_env_prof.onnx_py)
}

def nemo_py() : string {
    return normalize(empty(g_env_prof.nemo_py) ? venv_python("modules/dasLLAMA/benchmarks/asr/.venv-nemo") : g_env_prof.nemo_py)
}

// The clean CPU-only build (llama.cpp-clean, ebd048fc5, Accelerate ON, Metal OFF, no dasprof)
// patched with patches/mtmd-timing.patch. CPU-only is deliberate: the fast historical mtmd numbers
// rode Metal (GPU) despite the "CPU-only" label — ggml's Conformer audio ops don't route through
// BLAS/AMX, so a genuine CPU encode is ~16 s (and das's optimized scalar Conformer beats it).
def mtmd_bin() : string {
    let env = g_env_prof.mtmd_bin
    if (!empty(env)) {
        return normalize(env)
    }
    // same cmake-layout probe as whisper_cpp_bin: MSVC multi-config, Ninja, Mac/Make
    let root = "../llama.cpp-mtmd-ref/build-cpu/bin"
    for (c in [normalize("{root}/Release/llama-mtmd-cli.exe"),
               normalize("{root}/llama-mtmd-cli.exe"),
               normalize("{root}/llama-mtmd-cli")]) {
        if (stat(c).is_valid) {
            return c
        }
    }
    return normalize("{root}/llama-mtmd-cli")
}

// The python bench scripts live beside the corpus (asr/); these tools run from the repo-root cwd.
def asr_dir() : string {
    return "modules/dasLLAMA/benchmarks/asr"
}

// The numeric token immediately before an "ms" token on a whitespace line (whisper.cpp timing lines:
// "whisper_print_timings:  total time =  1234.56 ms"). -1 if none.
def private ms_before_ms_token(line : string) : double {
    let toks <- [for (t in split(line, " ")); t; where !empty(t)]
    for (i in range(length(toks))) {
        if (toks[i] == "ms" && i > 0) {
            return to_double(toks[i - 1])
        }
    }
    return -1.0lf
}

//! whisper.cpp transcribe ms = total time - load time, from the *_print_timings lines. -1 on failure.
def parse_cli_ms(out : string) : double {
    var load = -1.0lf
    var total = -1.0lf
    for (line in split(out, "\n")) {
        if (find(line, "_print_timings") < 0) {
            continue
        }
        if (find(line, "load time") >= 0) {
            load = ms_before_ms_token(line)
        }
        if (find(line, "total time") >= 0) {
            total = ms_before_ms_token(line)
        }
    }
    return (load >= 0.0lf && total >= 0.0lf) ? total - load : -1.0lf
}

//! Best (min) ms over the BENCH lines whose clip basename == `wav_base` (onnx_bench / canary_qwen_bench
//! emit "BENCH\t<model>\t<base>\t<audio_s>\t<rep>\t<ms>\t<speed>"). Also reports the audio seconds. -1 if none.
def parse_bench_best(out, wav_base : string; var audio_s : double&) : double {
    var best = -1.0lf
    audio_s = -1.0lf
    for (line in split(out, "\n")) {
        let c <- split(line, "\t")
        if (length(c) < 6 || c[0] != "BENCH" || c[2] != wav_base) {
            continue
        }
        audio_s = to_double(c[3])
        let ms = to_double(c[5])
        if (ms > 0.0lf && (best < 0.0lf || ms < best)) {
            best = ms
        }
    }
    return best
}

//! The patched llama-mtmd-cli emits "MTMD_TIMING encode_prefill_ms E decode_ms D total_ms T". Returns
//! (total, encode) ms; (-1, -1) if the line is absent.
def parse_mtmd_ms(out : string) : tuple<total : double; encode : double> {
    for (line in split(out, "\n")) {
        if (find(line, "MTMD_TIMING") < 0) {
            continue
        }
        let toks <- [for (t in split(line, " ")); t; where !empty(t)]
        var enc = -1.0lf
        var tot = -1.0lf
        for (i in range(length(toks))) {
            if (toks[i] == "encode_prefill_ms" && i + 1 < length(toks)) {
                enc = to_double(toks[i + 1])
            }
            if (toks[i] == "total_ms" && i + 1 < length(toks)) {
                tot = to_double(toks[i + 1])
            }
        }
        return (total = tot, encode = enc)
    }
    return (total = -1.0lf, encode = -1.0lf)
}

//! whisper.cpp parakeet-cli / whisper-cli — one process per clip (load is ~ms; parse_cli_ms
//! subtracts it regardless). Best over `reps`. Returns the winning command line too — the receipt.
def run_asr_cli(cli, model_path, wav_path : string; threads, reps : int; var cmd_out : string&) : double {
    var args <- [whisper_cpp_bin(cli), "-m", model_path, "-f", wav_path, "-t", "{threads}"]
    if (cli == "whisper-cli") {
        args |> push_from(["-bs", "1", "-bo", "1", "-nf"])
    }
    args |> push("-ng")
    var best = -1.0lf
    cmd_out = argv_line(args)
    for (_rep in range(reps)) {
        let ms = parse_cli_ms(run_capture_argv(args))
        if (ms > 0.0lf && (best < 0.0lf || ms < best)) {
            best = ms
        }
    }
    return best
}

//! One parakeet-cli process over MANY clips (`file0 file1 ...`): the per-file timing blocks
//! repeat the load and ACCUMULATE `total time`, so parse_cli_ms on the full output (last
//! total − load) is the whole-set engine time. A block-count mismatch (an unreadable clip
//! mid-list) discards the rep — a partial cumulative total would mispair. Best over `reps`.
def run_asr_cli_multi(cli, model_path : string; wav_paths : array<string>; threads, reps : int; var cmd_out : string&) : double {
    var args <- [whisper_cpp_bin(cli), "-m", model_path, "-t", "{threads}", "-ng"]
    args |> push_from(wav_paths)
    cmd_out = argv_line(args)
    var best = -1.0lf
    for (_rep in range(reps)) {
        let out = run_capture_argv(args)
        var blocks = 0
        for (line in split(out, "\n")) {
            if (find(line, "_print_timings") >= 0 && find(line, "total time") >= 0) {
                blocks++
            }
        }
        if (blocks != length(wav_paths)) {
            to_log(LOG_WARNING, "run_asr_cli_multi: {blocks}/{length(wav_paths)} files timed - rep discarded\n")
            continue
        }
        let ms = parse_cli_ms(out)
        if (ms > 0.0lf && (best < 0.0lf || ms < best)) {
            best = ms
        }
    }
    return best
}

//! patched llama-mtmd-cli — one process per clip (30B load is heavy; MTMD_TIMING excludes it).
//! Best total over `reps`.
def run_asr_mtmd(model_path, mmproj_path, wav_path : string; threads, reps : int;
                 var cmd_out : string&; var encode_out : double&; var text_out : string&) : double {
    var best = -1.0lf
    encode_out = 0.0lf
    text_out = ""
    let args <- [mtmd_bin(), "-m", model_path, "--mmproj", mmproj_path, "--audio", wav_path,
                 "-p", "Transcribe the audio.", "--temp", "0", "--jinja", "--reasoning", "off",
                 "-ngl", "0", "-t", "{threads}"]
    cmd_out = argv_line(args)
    for (_rep in range(reps)) {
        let out = run_capture_argv(args)
        let r = parse_mtmd_ms(out)
        if (r.total > 0.0lf && (best < 0.0lf || r.total < best)) {
            best = r.total
            encode_out = max(r.encode, 0.0lf)   // the best rep's already-parsed encode split
            text_out = mtmd_response_text(out)
        }
    }
    return best
}

//! The response body out of a captured llama-mtmd-cli run. The capture MERGES stderr
//! (popen_argv does, by design — same hazard json_array_tail guards for llama-bench), so
//! engine log lines ride the stream; they are dropped by their tag shape (`identifier:` as
//! the first token), the transcript prose never starts that way.
def mtmd_response_text(out : string) : string {
    var inscope parts : array<string>
    for (line in split(out, "\n")) {
        let s = strip(line)
        continue if (empty(s) || find(s, "MTMD_TIMING") >= 0 || mtmd_log_line(s))
        parts |> push(s)
    }
    return join(parts, " ")
}

// the engine-log tag shape: a lowercase identifier ending in ':' as the first token
// (llama_model_loader:, print_info:, main:, ...), or a bare ggml/llama/clip/mtmd prefix
def private mtmd_log_line(s : string) : bool {
    for (p in ["ggml", "llama_", "clip_", "mtmd_", "common_", "build:", "system_info", "encoding audio", "audio slice"]) {
        return true if (starts_with(s, p))
    }
    let sp = find(s, " ")
    let head = sp > 0 ? slice(s, 0, sp) : s
    if (!ends_with(head, ":")) {
        return false
    }
    var tag_shape = true
    peek_data(head) $(bytes) {
        for (i in range(length(bytes) - 1)) {
            let c = int(bytes[i])
            if (!((c >= 'a' && c <= 'z') || is_number(c) || c == '_' || c == '.')) {
                tag_shape = false
                break
            }
        }
    }
    return tag_shape
}

//! The onnx/nemo bench invocation over a clip set — ONE process for all clips (python model load
//! is expensive, amortize it); the caller parses per-clip rows out with [[parse_bench_best]].
//! The onnx rows ALSO land in [[asr_pybench_out_path]] via --out: parakeet-variant loaders
//! wreck the child's stdout pipe on Windows (~1 byte then EOF while the run continues), so
//! the caller re-reads the file whenever the captured stream carries no rows.
def asr_pybench_args(tool, handle : string; clips : array<string>; threads, reps : int) : array<string> {
    var args <- (tool == "onnx"
        ? [onnx_py(), "{asr_dir()}/onnx_bench.py", "--variant", handle, "--out", asr_pybench_out_path()]
        : [nemo_py(), "{asr_dir()}/canary_qwen_bench.py"])
    args |> push_from(["--threads", "{threads}", "--reps", "{reps}"])
    for (c in clips) {
        args |> push_from(["--wav", "{asr_corpus_dir()}{c}"])
    }
    return <- args
}

def asr_pybench_out_path() : string {
    return "{asr_dir()}/_pybench_rows.txt"
}

// ===== Reference-engine version capture (B4 — never hand-typed) =====

// 9-hex short commit of the git checkout containing `path`; "" when not a checkout
def private git_commit_at(path : string) : string {
    let out = run_capture("git -C \"{path}\" rev-parse --short=9 HEAD 2>&1", true)
    if (length(out) != 9) {
        return ""
    }
    var hex = true
    peek_data(out) $(arr) {
        for (b in arr) {
            let c = int(b)
            hex = hex && (is_number(c) || (c >= 'a' && c <= 'f'))
        }
    }
    return hex ? out : ""
}

// -uno: tracked modifications only — untracked files (.dlim caches, our .sha sidecars) don't
// change what was built, and counting them would taint every reference tree as dirty
def private git_dirty_at(path : string) : bool {
    return !empty(strip(run_capture("git -C \"{path}\" status --porcelain -uno 2>&1", false)))
}

//! Version line for the whisper.cpp checkout (whisper-cli AND parakeet-cli — one tree). A dirty
//! tree taints the identity loudly: nobody can rebuild "+dirty".
def whisper_cpp_versions() : string {
    let root = whisper_cpp_root()
    let commit = git_commit_at(root)
    if (empty(commit)) {
        return ""
    }
    if (git_dirty_at(root)) {
        to_log(LOG_WARNING, "dasLLAMA bench: {root} has uncommitted changes - reference identity is DIRTY\n")
        return "whisper.cpp @ {commit}+dirty"
    }
    return "whisper.cpp @ {commit}"
}

//! Version line for the patched mtmd build: base commit + the patch file's content hash. The
//! checkout is EXPECTED dirty (the patch is applied); the patch sha is what makes that ok.
def mtmd_versions() : string {
    let commit = git_commit_at(dir_name(mtmd_bin()))
    let patch = file_identity("{asr_dir()}/patches/mtmd-timing.patch", "patch")
    let ptag = empty(patch.sha256) ? "mtmd-timing.patch(unhashed)" : "mtmd-timing.patch@{slice(patch.sha256, 0, 8)}"
    return empty(commit) ? "llama.cpp + {ptag}" : "llama.cpp @ {commit} + {ptag}"
}

def onnx_versions() : string {
    return run_capture("{onnx_py()} -c \"import onnx_asr, onnxruntime; print('onnx-asr', getattr(onnx_asr, '__version__', '?'), '/ onnxruntime', onnxruntime.__version__)\" 2>&1", true)
}

def nemo_versions() : string {
    return run_capture("{nemo_py()} -c \"import nemo, torch; print('nemo', nemo.__version__, '/ torch', torch.__version__)\" 2>&1", true)
}

//! The version line for one reference tool — dispatch by catalog tool name.
def asr_tool_versions(tool : string) : string {
    if (tool == "parakeet-cli" || tool == "whisper-cli") {
        return whisper_cpp_versions()
    }
    if (tool == "mtmd") {
        return mtmd_versions()
    }
    if (tool == "onnx") {
        return onnx_versions()
    }
    if (tool == "nemo") {
        return nemo_versions()
    }
    return ""
}

// ===== ASR: the LibriSpeech dictation set (setup_asr_rig --libri) =====

//! The dictation set's corpus-relative wav paths, sorted — exactly 25 or empty (a partial
//! fetch would sum a different set on each side of a pair; the legs skip with a warning).
def libri_wavs() : array<string> {
    var out : array<string>
    dir("{asr_corpus_dir()}librispeech") $(name : string) {
        if (name |> ends_with(".wav")) {
            out |> push("librispeech/{name}")
        }
    }
    if (length(out) != 25) {
        out |> clear()
        return <- out
    }
    sort(out)
    return <- out
}

//! The one BenchTest key both sides of the LibriSpeech aggregate record under: total
//! best-of-reps ms over the 25 clips (+ total audio seconds) — the dictation stat is a
//! whole-set number, never 25 table rows.
def libri_test_key() : string {
    return "asr:librispeech25"
}

// ===== Public bench records (schema: modules/dasLLAMA/METHODOLOGY.md "The record") =====

//! One measured test row, keyed in `BenchRun.tests` by the llama-bench test name
//! ("pp512", "tg128", ...) for LLM runs, or by clip ("asr:jfk.wav") for ASR runs.
//! LLM: mean tok/s over the timed reps ± sample stdev (higher is better).
//! ASR: best-of-reps transcribe wall ms + the clip's audio seconds (lower ms is better).
struct BenchTest {
    @optional tok_s : double
    @optional stddev : double
    @optional ms : double
    @optional audio_s : double
    @optional encode_ms : double   // ASR encode split of the best rep, where the engine reports one
    // the ASR transcript. A duration alone cannot distinguish "faster" from "decoded fewer
    // tokens", and under fast-math that is exactly the ambiguity — so the row carries the text
    // the timing came from, and two rows can be diffed rather than trusted.
    @optional text : string
}

//! One input file a run read, pinned by content hash. `sha256` comes from the `.sha` sidecar
//! rail ([[file_identity]]) so two boxes claiming "the same" gguf/clip are provably the same
//! bytes — or provably not.
struct InputFile {
    role : string            // "weights" | "mmproj" | "clip"
    name : string            // basename as the catalog knows it
    sha256 : string
    bytes : int64
}

//! One engine × backend × flavor run on one box — a first-class record. das and reference runs
//! are siblings in the same store; pairing and ratios are derived at render time, never stored.
//! `cmd` is the reproduction receipt: the exact invocation that produced the numbers.
struct BenchRun {
    engine : string          // "das" | "llama.cpp" | ASR references ("whisper.cpp", "onnx", "nemo", ...)
    backend : string         // "cpu" | "metal" | "vulkan"
    flavor : string          // das: "tuned" | "accel" (+AMX tier); llama.cpp build flavor: "stock" | "clean-cpu"
    box : string
    threads : int
    date : string            // YYYY-MM-DD
    cmd : string
    hardware : HardwareInfo
    tests : table<string; BenchTest>
    @optional workload : string // "" = text LLM; "asr" | "audio-chat" — selects the tests-key shape
    @optional source : string   // "official" | "community" — stamped by the store driver, not the tool
    @optional sha : string      // engine commit (das: git HEAD; llama.cpp: build_commit)
    @optional version : string  // das release version
    @optional versions : string // reference-tool version line (venv package pins, patch sha) — see B4
    @optional tune : string     // das gen-kernel stamp provenance — required for reproducibility
    @optional tune_sha : string // sha256 of the tune sidecar the exe shipped with — the sidecar
                                // GENERATION; rows across generations are not comparable
    @optional exec_fmt : string // what this engine EXECUTES for this file (weight formats per site
                                // class + activation/scale treatment) — the apples-to-apples receipt;
                                // das derives it from the loaded Model, llama.cpp rows from flavor
    @optional env : string      // the arming environment `cmd` was run under — see [[bench_env_line]].
                                // Load-bearing: most GPU/tier levers arm via env, so a `cmd` alone
                                // does NOT reproduce a row and two configs can look identical
    @optional files : array<InputFile> // every input the run read (weights, mmproj, clips)
    @optional comment : string  // merged from records/annotations.json by gen_site_records —
                                // never written by sweep tools, so re-sweeps cannot wipe it
    @optional void_clips : string // merged from annotations like `comment`: clips whose
                                  // cross-engine ratio is declared meaningless (no-win render)
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
    "DASLLAMA_OS", "DASLLAMA_PIN_BACKEND", "DASLLAMA_PIN_PREFILL", "DASLLAMA_PREFETCH",
    "DASLLAMA_RAM_GB", "DASLLAMA_THREADS", "DASLLAMA_TRIM", "DASLLAMA_TRUTH_REFRESH",
    "DASLLAMA_VK_FA", "DASLLAMA_VK_HAZARD_PARANOID", "DASLLAMA_VK_HAZARD_TRACE",
    "DASLLAMA_VK_IMPORT", "DASLLAMA_VK_MEMPRIO", "DASLLAMA_VK_REBAR", "DASLLAMA_VK_XFERQ")

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
        "{name}={shell_quote(env_value_of(name))}";
        where env_is_set(name)]
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

//! One hand-authored annotation (records/annotations.json): an honest note merged onto matching
//! records at site-gen time. Lives OUTSIDE the sweep output on purpose — re-profiling regenerates
//! the stores, and a comment explaining an anomaly must survive that. `gguf` alone annotates the
//! model (its `note`); any selector present narrows it to matching runs (their `comment`).
struct BenchAnnotation {
    gguf : string
    @optional box : string
    @optional engine : string
    @optional backend : string
    @optional flavor : string
    @optional workload : string
    comment : string
    @optional void_clips : string  // comma-separated clip names whose cross-engine ratio this
                                   // note declares meaningless (ref behavior difference) — the
                                   // site renders no win for them
}

def annotations_path() : string {
    return "modules/dasLLAMA/performance/records/annotations.json"
}

def read_annotations(var out : array<BenchAnnotation>) : bool {
    let path = annotations_path()
    if (!stat(path).is_valid) {
        return false
    }
    return sscan_json(fread(path), out)
}

def private anno_is_model_level(a : BenchAnnotation) : bool {
    return empty(a.box) && empty(a.engine) && empty(a.backend) && empty(a.flavor) && empty(a.workload)
}

def private anno_matches_run(a : BenchAnnotation; r : BenchRun) : bool {
    return ((empty(a.box) || a.box == r.box) && (empty(a.engine) || a.engine == r.engine)
        && (empty(a.backend) || a.backend == r.backend) && (empty(a.flavor) || a.flavor == r.flavor)
        && (empty(a.workload) || a.workload == r.workload))
}

//! Merge annotations onto the models they describe: model-level entries set `note`, run-level
//! entries set `comment` on every matching run. In-order application — a later, more specific
//! entry overwrites an earlier one where both match.
def apply_annotations(var models : array<BenchModel>; annos : array<BenchAnnotation>) {
    for (a in annos) {
        for (m in models) {
            if (m.gguf != a.gguf) {
                continue
            }
            if (anno_is_model_level(a)) {
                m.note = a.comment
            } else {
                for (r in m.runs) {
                    if (anno_matches_run(a, r)) {
                        r.comment = a.comment
                        if (!empty(a.void_clips)) {
                            r.void_clips = a.void_clips
                        }
                    }
                }
            }
        }
    }
}

//! True on a macOS box — the gate for Metal cells in the record driver.
def is_apple_box : bool {
    return !is_windows() && run_capture("uname -s", true) == "Darwin"
}

//! The official per-box record store the site build merges.
def bench_records_path : string {
    return "modules/dasLLAMA/performance/records/{box_name()}.json"
}

//! Every per-box store under records/, STRICT: ``records/{box}.json`` only — a dotted stem is
//! a tune archive, ``annotations.json`` is the hand-note overlay, ``_``-prefixed is scratch.
//! The ONE lister both generators use; two copies drifted on the ``_`` rule once already.
def list_record_stores(root : string = "modules/dasLLAMA/performance/records") : array<string> {
    var out : array<string>
    dir(root) $(fn) {
        if (ends_with(fn, ".json") && !starts_with(fn, "_")) {
            let stem = slice(fn, 0, length(fn) - length(".json"))
            if (stem != "annotations" && find(stem, ".") < 0) {
                out |> push("{root}/{fn}")
            }
        }
    }
    sort(out)
    return <- out
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
        to_log(LOG_ERROR, "dasLLAMA bench: could not open {path} for writing - records NOT persisted\n")
    }
}

//! Update-or-insert one run; the identity is (box, engine, backend, flavor, workload) — a
//! re-measure replaces its row in place (fresh date/sha), never appends a superseded copy.
def upsert_bench_run(var runs : array<BenchRun>; var r : BenchRun) {
    for (c in runs) {
        if (c.box == r.box && c.engine == r.engine && c.backend == r.backend
                && c.flavor == r.flavor && c.workload == r.workload) {
            delete c
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

// ===== Oracle mode (gen_bench_records --oracle: the stored rows re-verify against fresh cells) =====

//! Worst-case cell verdict of an oracle comparison — ``fail`` drives the nonzero exit.
enum OracleVerdict {
    ok
    warn
    fail
}

def oracle_verdict_str(v : OracleVerdict) : string {
    return v == OracleVerdict.fail ? "FAIL" : (v == OracleVerdict.warn ? "WARN" : "OK")
}

//! Gate a fresh das run against its stored row, one line per stored tok/s test key. One-sided:
//! a drop past ``fail_pct`` fails, past ``warn_pct`` warns; a gain reports (flagged as
//! suspicious past ``fail_pct``, but never failed). A stored key missing from the fresh run —
//! and a stored row with no tok/s tests at all — is a fail: the stored row is the coverage
//! contract. Returns the worst verdict across keys.
def oracle_compare(fresh, stored : BenchRun; warn_pct, fail_pct : double; var lines : array<string>) : OracleVerdict {
    var worst = OracleVerdict.ok
    var compared = 0
    var tkeys <- [for (k in keys(stored.tests)); k]
    sort(tkeys)
    for (k in tkeys) {
        let s_tok = stored.tests?[k]?.tok_s ?? 0.0lf
        if (s_tok <= 0.0lf) {
            // ms-shaped (ASR): the SAME one-sided gate, mirrored — for a duration, worse is HIGHER,
            // so the regression side is +delta. A row with neither metric stays uncomparable.
            let s_ms = stored.tests?[k]?.ms ?? 0.0lf
            if (s_ms <= 0.0lf) {
                continue
            }
            compared++
            let f_ms = fresh.tests?[k]?.ms ?? 0.0lf
            if (f_ms <= 0.0lf) {
                lines |> push("{k} MISSING from the fresh run (stored {s_ms:.1f} ms)")
                worst = OracleVerdict.fail
                continue
            }
            let d_ms = (f_ms - s_ms) * 100.0lf / s_ms
            var vm = OracleVerdict.ok
            if (d_ms > fail_pct) {
                vm = OracleVerdict.fail
            } elif (d_ms > warn_pct) {
                vm = OracleVerdict.warn
            }
            let note_ms = (vm == OracleVerdict.ok && d_ms < -fail_pct) ? " (suspicious gain - verify the cell)" : ""
            let sign_ms = d_ms < 0.0lf ? "" : "+"
            lines |> push("{k} {f_ms:.1f} vs {s_ms:.1f} ms ({sign_ms}{d_ms:.1f}%) {oracle_verdict_str(vm)}{note_ms}")
            if (int(vm) > int(worst)) {
                worst = vm
            }
            continue
        }
        compared++
        let f_tok = fresh.tests?[k]?.tok_s ?? 0.0lf
        if (f_tok <= 0.0lf) {
            lines |> push("{k} MISSING from the fresh run (stored {s_tok:.1f})")
            worst = OracleVerdict.fail
            continue
        }
        let s_sd = stored.tests?[k]?.stddev ?? 0.0lf
        let delta = (f_tok - s_tok) * 100.0lf / s_tok
        var v = OracleVerdict.ok
        if (delta < -fail_pct) {
            v = OracleVerdict.fail
        } elif (delta < -warn_pct) {
            v = OracleVerdict.warn
        }
        let note = (v == OracleVerdict.ok && delta > fail_pct) ? " (suspicious gain - verify the cell)" : ""
        let sign = delta < 0.0lf ? "" : "+"
        lines |> push("{k} {f_tok:.1f} vs {s_tok:.1f}±{s_sd:.1f} ({sign}{delta:.1f}%) {oracle_verdict_str(v)}{note}")   // nolint:STYLE039 (plus-minus sign in the human-facing oracle report row)
        if (int(v) > int(worst)) {
            worst = v
        }
    }
    delete tkeys
    if (compared == 0) {
        lines |> push("no comparable tok/s or ms tests in the stored row")
        worst = OracleVerdict.fail
    }
    return worst
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

// The capture merges stderr (popen_argv does, by design), and every reference build that logs a
// line — a Metal build announcing its device, say — therefore poisons the JSON. Cut the trailing
// `-o json` array back out: llama-bench prints exactly one, last, opening with `[` on its own
// line. "" when there is none, which is the honest signal that the child printed no result.
def private json_array_tail(raw : string) : string {
    var lines <- split(raw, "\n")
    var start = -1
    for (i in range(length(lines))) {
        if (strip(lines[i]) == "[") {
            start = i
        }
    }
    if (start < 0) {
        delete lines
        return ""
    }
    var keep <- [for (i in range(start, length(lines))); lines[i]]
    let out = join(keep, "\n")
    delete keep
    delete lines
    return out
}

//! Run a llama-bench command line (must include `-o json`) and parse its rows. False on a run or
//! parse failure. The child's stdout and stderr arrive merged, so the JSON array is cut back out
//! before parsing — a reference build that logs is normal, not a failure.
//! `raw` keeps what the child actually printed: a failure here is otherwise indistinguishable
//! from a silent one, and the caller has nothing to report but elapsed time.
def run_llama_bench(args : array<string>; var rows : array<LlamaBenchRow>; var raw : string&) : bool {
    raw = run_capture_argv(args)
    let js = json_array_tail(raw)
    if (empty(js)) {
        return false   // no result array at all: the child died or printed only diagnostics
    }
    return sscan_json(js, rows)
}

def run_llama_bench(args : array<string>; var rows : array<LlamaBenchRow>) : bool {
    var raw = ""
    return run_llama_bench(args, rows, raw)
}
