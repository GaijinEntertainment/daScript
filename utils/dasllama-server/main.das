options gen2
options gc               // standalone JIT loop collects between update calls
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// dasllama-server — an OpenAI-compatible HTTP server over the dasLLAMA facade.
//
//   bin/daslang -jit utils/dasllama-server/main.das -- --model <model.gguf> [--port 8080] [--quant q8]
//                                                       [--kv-dtype f16] [--asr <asr.bin>] [--mmproj <mmproj.gguf>]
//                                                       [--image-mmproj <mmproj.gguf>] [--config server.toml] [--tune]
//
// --config points at a TOML file whose keys mirror the long flag names (model = "...", port = 8080,
// kv_dtype = "tq4", ...); explicit CLI flags override it. A config carrying `authoritative = true`
// (what the control page's POST /config writes) flips the top of that precedence instead:
// defaults < CLI < authoritative TOML.
//
// Then point any OpenAI client at http://127.0.0.1:<port>/v1 (opencode, Open WebUI, the `llm` CLI, …).
//
// Per-box tuning: the first start on an untuned box consults the sidecar exchange (dasllama.io —
// a verified match downloads instead of racing ~20 min; exchange_accept/exchange_submit in the
// config), else runs the dasLLAMA kernel tuner; either way it exits so the watchdog can restart
// with the winners (missing = "restart"); `--tune` forces a local re-tune. Set DAS_TUNE_POLICY=error
// while developing to skip per-start tuning (it prints the tuner command instead). The winners live
// at <das_root>/dasllama.tune.json, shared by every dasLLAMA app on this box.

require openai_server           // the server + route handlers (this directory)
require model_catalog           // catalog_models_dir — the models-dir resolution
require dasllama/dasllama       // load_model / QuantMode / load_asr_model
require llvm/daslib/llvm_tune   // [tune_policy] + tune_status (the injected guard resolves here)
require dasllama/dasllama_exchange   // the sidecar exchange: boot resolver + submit check
require jobque                  // set_jobque_threads_cap / set_jobque_affinity
require daslib/clargs
require daslib/toml
require daslib/json_boost
require daslib/module_path
require daslib/logger
require daslib/fio
require live_host               // init/update/shutdown lifecycle + standalone exit_requested
require math
require strings                 // to_lower / starts_with (pick_quant's file-quant spellings)

[CommandLineArgs]
struct ServerArgs {
    @clarg_short = "c"
    @clarg_doc = "TOML config file; keys mirror the long flag names (explicit CLI flags override)"
    config : string

    @clarg_short = "m"
    @clarg_doc = "Path to the GGUF model to serve (or model = in --config)"
    model : string

    @clarg_short = "p"
    @clarg_doc = "Port to listen on (default 8080)"
    port : int

    @clarg_short = "q"
    @clarg_doc = "Weight serving mode: fp32 | q8 (default); file-format spellings q8_0 | q4_0 | q4_k | q5_k | q6_k | mxfp4 | f16 | bf16 select q8, where those planes serve natively; q4 = legacy requant tier"
    quant : string

    @clarg_doc = "KV cache codec: f32 | f16 | q8_0 | tq4 (default f16)"
    kv_dtype : string

    @clarg_doc = "Metal (GPU) serving mode: off | auto | required (default off). auto = GPU when a path's gates pass, CPU fallback otherwise; required = a call the GPU cannot serve is a hard failure"
    metal : string

    @clarg_doc = "GPU backend: auto (default: metal/vulkan when detected, else CPU) | off | metal | metal-required | vulkan (a set --metal flag keeps the legacy env-driven path)"
    gpu : string

    @clarg_doc = "vulkan: resident MoE expert-stack layers K, offloaded from the end (default 2 under --gpu vulkan)"
    gpu_layers : int

    @clarg_doc = "vulkan: streamed prefill MoE layers below the resident set (default 35 under --gpu vulkan)"
    gpu_stream : int

    @clarg_doc = "vulkan: deltanet (recurrent) layers through the device chain (default on under --gpu vulkan)"
    gpu_dn : bool

    @clarg_doc = "vulkan: full-attention layers through the device chain (default on under --gpu vulkan)"
    gpu_attn : bool

    @clarg_doc = "vulkan: dense attention-side planes resident (default off)"
    gpu_dense : bool

    @clarg_doc = "vulkan: resident-weight VRAM cap override in MB (default: query the device)"
    gpu_vram_mb : int

    @clarg_short = "a"
    @clarg_doc = "Path to an ASR model — enables /v1/audio/transcriptions"
    asr : string

    @clarg_doc = "mmproj for the GGUF Qwen3-ASR route (paired with --asr)"
    mmproj : string

    @clarg_name = "image-mmproj"
    @clarg_doc = "Vision mmproj (gemma4uv, gemma4v, or gemma3v, sniffed) for the default model — enables image_url parts on /v1/chat/completions"
    image_mmproj : string

    @clarg_doc = "Long-lived asynchronous ASR worker threads (default 1; 2 permits two parallel transcriptions)"
    asr_workers : int

    @clarg_doc = "Context-length cap in tokens (default: the model's trained context_length)"
    ctx : int

    @clarg_doc = "Default reply token budget when a request omits max_tokens (default 16384; clamped to ctx per request)"
    max_tokens : int

    @clarg_short = "s"
    @clarg_doc = "Max concurrent generation streams (default 4)"
    streams : int

    @clarg_short = "t"
    @clarg_doc = "Worker-lane cap for the matmul dispatch (default 16; -1 = all cores)"
    threads : int

    @clarg_doc = "Jobque compute dispatch: hybrid (default; LLM team, ASR inline) | team | inline"
    team_dispatch : string

    @clarg_doc = "Worker CPU affinity: -1 = platform default (QoS on darwin, off elsewhere), 0 = off, 1 = ideal-CPU hint, 2 = hard mask (DAS_JOBQUE_AFFINITY env overrides)"
    affinity : int = -1

    @clarg_doc = "Prefill chunk in tokens: decode stalls at most this many per tick (default 64)"
    chunk : int

    @clarg_doc = "KV page size in positions for paged serving (default 64)"
    page_rows : int

    @clarg_doc = "Prefix-cache retention cap in pages (default: one full context per stream; -1 = unbounded)"
    prefix : int

    @clarg_doc = "Flat preallocated KV sessions — disables paged serving and the prefix cache"
    flat : bool

    @clarg_doc = "MTP/NextN self-speculative decode for greedy requests (needs a model with an in-file NextN head, e.g. the -MTP- GGUFs; pays on the dense qwen35 models)"
    mtp : bool

    @clarg_doc = "Path to a llama.cpp llama-bench binary — enables the control page's benchmark button"
    lcpp_bin : string

    @clarg_doc = "Directory the model catalog downloads into (default ~/.dasllama/models; DASLLAMA_MODELS_DIR overrides)"
    models_dir : string

    @clarg_doc = "Re-tune this box's dasLLAMA kernels, then relaunch (handled before arg parsing)"
    tune : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// One [[models]] config entry: a served model + its per-slot overrides. `backend` is the one
// per-model serving knob (auto = the engine's decline ladder decides; cpu = never the device;
// gpu = want the tier). The -1/-2/""/0 sentinels mean "inherit the global flat key".
struct ModelEntry {
    name : string           // route id (default: base_name(path))
    path : string
    backend : string = "auto"
    ctx : int = 0
    quant : string = ""
    kv_dtype : string = ""
    streams : int = -1
    chunk : int = -1
    page_rows : int = -1    // 0 = flat is meaningful, -1 = inherit
    prefix : int = -2       // -1 = unbounded is meaningful, -2 = inherit
    mtp : int = -1          // -1 inherit | 0 off | 1 on
    image_mmproj : string   // vision mmproj (gemma4uv); empty = this model takes text only
    is_default : bool = false
}

// registered from [init] so it lands before the tune-policy guard at the top of main: an
// untuned boot consults the exchange (verified matches only by default) before racing ~20 min
[init]
def register_exchange_resolver {
    exchange_register_resolver("dasllama-server.toml")
}

// A clearly model-free boot (no --model/-m, no --config/-c, no discoverable toml) heads for
// setup mode, where no kernel runs - suppress the tune bootstrap so the page opens NOW
// instead of after a multi-minute mint; the serve-restart's configured boot tunes normally.
[init]
def suppress_tune_for_setup {
    var args <- get_user_args()
    var configured = false
    for (a in args) {
        if (a == "--model" || a == "-m" || a == "--config" || a == "-c"
                || a |> starts_with("--model=") || a |> starts_with("--config=")) {
            configured = true
        }
    }
    delete args
    if (!configured && !stat("dasllama-server.toml").is_valid
            && !stat(path_join(get_this_module_dir(), "dasllama-server.toml")).is_valid) {
        tune_suppress_mint("no model configured - setup mode boots on fallback kernels")
    }
}

// Lifecycle state is global: JIT heap collection cannot see ordinary stack locals as GC roots.
// main therefore keeps no collectable value alive across maybe_collect_gc().
var private g_setup_mode = false   // no loadable model: serve the catalog/recovery page instead of exiting
var private g_models : array<ModelEntry>
var private g_cfg = ServerArgs()
var private g_args_parsed = false
var private g_args_ready = false
var private g_server_ready = false
var private g_exit_code = 0
var private g_last_memory_log_at = 0l
var private g_last_gc_at = 0l
let GC_MIN_INTERVAL_SECONDS = 60l

def pick_kv_dtype(s : string) : KVDtype {
    let l = to_lower(s)   // same normalization as pick_quant — F16/Q8_0/TQ4 spellings accepted
    if (l == "f32") {
        return KVDtype.f32
    } elif (l == "q8_0") {
        return KVDtype.q8_0
    } elif (l == "tq4") {
        return KVDtype.tq4
    } elif (!empty(l) && l != "f16") {
        to_log(LOG_WARNING, "dasllama-server: unknown --kv-dtype '{s}', using f16 (valid: f32 | f16 | q8_0 | tq4)\n")
    }
    return KVDtype.f16
}

def pick_metal_mode(s : string) : MetalMode {
    let l = to_lower(s)
    if (l == "required") {
        return MetalMode.required
    } elif (l == "auto" || l == "when_available") {
        return MetalMode.when_available
    } elif (!empty(l) && l != "off") {
        to_log(LOG_WARNING, "dasllama-server: unknown --metal '{s}', using off (valid: off | auto | required)\n")
    }
    return MetalMode.off
}

def private cli_has_flag(flag : string) : bool {
    var args <- get_user_args()
    let eqform = "{flag}="
    for (a in args) {
        if (a == flag || a |> starts_with(eqform)) {
            delete args
            return true
        }
    }
    delete args
    return false
}

// per-key config provenance for the GET /config surface: "cli" | "toml"; absent = default.
// Filled by mark_cli_sources after arg parsing, then by apply_config as TOML keys land.
var private g_cfg_sources : table<string; string>
var private g_cfg_authoritative = false

let CONFIG_KEYS = fixed_array("model", "port", "quant", "kv_dtype", "metal", "gpu", "gpu_layers",
    "gpu_stream", "gpu_dn", "gpu_attn", "gpu_dense", "gpu_vram_mb", "asr", "mmproj", "image_mmproj", "asr_workers",
    "ctx", "max_tokens", "streams", "threads", "team_dispatch", "affinity", "chunk", "page_rows",
    "prefix", "flat", "mtp", "lcpp_bin", "models_dir", "models")

def private from_cli(key : string) : bool
    => (g_cfg_sources?[key] ?? "") == "cli"

def private toml_has(root : JsonValue?; key : string) : bool {
    if (root == null || !(root.value is _object)) {
        return false
    }
    return key_exists(root.value as _object, key)
}

def private cfg_from_toml(root : JsonValue?; key : string; var field : string&; auth : bool) {
    if (toml_has(root, key) && (auth || !from_cli(key))) {
        field = root?[key] ?? ""
        g_cfg_sources[key] = "toml"
    }
}

def private cfg_from_toml(root : JsonValue?; key : string; var field : int&; auth : bool) {
    if (toml_has(root, key) && (auth || !from_cli(key))) {
        field = int(root?[key] ?? 0l)
        g_cfg_sources[key] = "toml"
    }
}

def private cfg_from_toml(root : JsonValue?; key : string; var field : bool&; auth : bool) {
    if (toml_has(root, key) && (auth || !from_cli(key))) {
        field = root?[key] ?? false
        g_cfg_sources[key] = "toml"
    }
}

// which keys the command line set explicitly — the fill-if-unset sentinels, plus flag-presence
// probes for the keys where 0 is a meaningful explicit value (--threads / --affinity / --prefix
// and the vulkan detail knobs)
def private mark_cli_sources(cfg : ServerArgs) {   // nolint:STYLE037,STYLE038 — one flat presence test per CLI knob; a table would hide which knobs are checked how
    if (!empty(cfg.model)) {
        g_cfg_sources["model"] = "cli"
    }
    if (cfg.port > 0) {
        g_cfg_sources["port"] = "cli"
    }
    if (!empty(cfg.quant)) {
        g_cfg_sources["quant"] = "cli"
    }
    if (!empty(cfg.kv_dtype)) {
        g_cfg_sources["kv_dtype"] = "cli"
    }
    if (!empty(cfg.metal)) {
        g_cfg_sources["metal"] = "cli"
    }
    if (!empty(cfg.gpu)) {
        g_cfg_sources["gpu"] = "cli"
    }
    if (cfg.gpu_layers != 0 || cli_has_flag("--gpu-layers")) {
        g_cfg_sources["gpu_layers"] = "cli"
    }
    if (cfg.gpu_stream != 0 || cli_has_flag("--gpu-stream")) {
        g_cfg_sources["gpu_stream"] = "cli"
    }
    // bool knobs: flag PRESENCE, not value — clargs accepts --gpu-dn=false, and an explicit
    // false must beat both the blessed vulkan default (true) and a non-authoritative TOML
    if (cli_has_flag("--gpu-dn")) {
        g_cfg_sources["gpu_dn"] = "cli"
    }
    if (cli_has_flag("--gpu-attn")) {
        g_cfg_sources["gpu_attn"] = "cli"
    }
    if (cli_has_flag("--gpu-dense")) {
        g_cfg_sources["gpu_dense"] = "cli"
    }
    if (cfg.gpu_vram_mb > 0) {
        g_cfg_sources["gpu_vram_mb"] = "cli"
    }
    if (!empty(cfg.asr)) {
        g_cfg_sources["asr"] = "cli"
    }
    if (!empty(cfg.mmproj)) {
        g_cfg_sources["mmproj"] = "cli"
    }
    if (!empty(cfg.image_mmproj)) {
        g_cfg_sources["image_mmproj"] = "cli"
    }
    if (cfg.asr_workers > 0) {
        g_cfg_sources["asr_workers"] = "cli"
    }
    if (cfg.ctx > 0) {
        g_cfg_sources["ctx"] = "cli"
    }
    if (cfg.max_tokens > 0) {
        g_cfg_sources["max_tokens"] = "cli"
    }
    if (cfg.streams > 0) {
        g_cfg_sources["streams"] = "cli"
    }
    if (cfg.threads != 0 || cli_has_flag("--threads") || cli_has_flag("-t")) {
        g_cfg_sources["threads"] = "cli"
    }
    if (!empty(cfg.team_dispatch)) {
        g_cfg_sources["team_dispatch"] = "cli"
    }
    if (cfg.affinity != 0 || cli_has_flag("--affinity")) {
        g_cfg_sources["affinity"] = "cli"
    }
    if (cfg.chunk > 0) {
        g_cfg_sources["chunk"] = "cli"
    }
    if (cfg.page_rows > 0) {
        g_cfg_sources["page_rows"] = "cli"
    }
    if (cfg.prefix != 0 || cli_has_flag("--prefix")) {
        g_cfg_sources["prefix"] = "cli"
    }
    if (cli_has_flag("--flat")) {
        g_cfg_sources["flat"] = "cli"
    }
    if (cli_has_flag("--mtp")) {
        g_cfg_sources["mtp"] = "cli"
    }
    if (!empty(cfg.lcpp_bin)) {
        g_cfg_sources["lcpp_bin"] = "cli"
    }
    if (!empty(cfg.models_dir)) {
        g_cfg_sources["models_dir"] = "cli"
    }
}

// fill fields from the TOML config. Default precedence: defaults < config file < explicit CLI
// flags. A config carrying `authoritative = true` (what the control page writes) flips the top:
// its keys override CLI flags too (defaults < CLI < authoritative TOML).
def apply_config(var cfg : ServerArgs) : bool {
    let body = fread(cfg.config)
    if (empty(body)) {
        to_log(LOG_ERROR, "dasllama-server: cannot read config '{cfg.config}'\n")
        return false
    }
    var terr = ""
    var root = read_toml(body, terr)
    if (root == null) {
        to_log(LOG_ERROR, "dasllama-server: bad TOML in '{cfg.config}': {terr}\n")
        return false
    }
    let auth = root?["authoritative"] ?? false
    g_cfg_authoritative = auth
    cfg_from_toml(root, "model", cfg.model, auth)
    cfg_from_toml(root, "port", cfg.port, auth)
    cfg_from_toml(root, "quant", cfg.quant, auth)
    cfg_from_toml(root, "kv_dtype", cfg.kv_dtype, auth)
    cfg_from_toml(root, "metal", cfg.metal, auth)
    cfg_from_toml(root, "gpu", cfg.gpu, auth)
    cfg_from_toml(root, "gpu_layers", cfg.gpu_layers, auth)
    cfg_from_toml(root, "gpu_stream", cfg.gpu_stream, auth)
    cfg_from_toml(root, "gpu_dn", cfg.gpu_dn, auth)
    cfg_from_toml(root, "gpu_attn", cfg.gpu_attn, auth)
    cfg_from_toml(root, "gpu_dense", cfg.gpu_dense, auth)
    cfg_from_toml(root, "gpu_vram_mb", cfg.gpu_vram_mb, auth)
    cfg_from_toml(root, "asr", cfg.asr, auth)
    cfg_from_toml(root, "mmproj", cfg.mmproj, auth)
    cfg_from_toml(root, "image_mmproj", cfg.image_mmproj, auth)
    cfg_from_toml(root, "asr_workers", cfg.asr_workers, auth)
    cfg_from_toml(root, "ctx", cfg.ctx, auth)
    cfg_from_toml(root, "max_tokens", cfg.max_tokens, auth)
    cfg_from_toml(root, "streams", cfg.streams, auth)
    cfg_from_toml(root, "threads", cfg.threads, auth)
    cfg_from_toml(root, "team_dispatch", cfg.team_dispatch, auth)
    cfg_from_toml(root, "affinity", cfg.affinity, auth)
    cfg_from_toml(root, "chunk", cfg.chunk, auth)
    cfg_from_toml(root, "page_rows", cfg.page_rows, auth)
    cfg_from_toml(root, "prefix", cfg.prefix, auth)
    cfg_from_toml(root, "flat", cfg.flat, auth)
    cfg_from_toml(root, "mtp", cfg.mtp, auth)
    cfg_from_toml(root, "lcpp_bin", cfg.lcpp_bin, auth)
    cfg_from_toml(root, "models_dir", cfg.models_dir, auth)
    if (toml_has(root, "models")) {
        apply_models_roster(root)
    }
    unsafe {
        delete root
    }
    return true
}

// An EXPLICIT --model wins over the config's roster (the CLI-overrides-config contract): the
// matching roster entry becomes the default, an unlisted path joins the roster AS the default.
// Warn-and-degrade: a bad path or a route-name collision leaves the roster UNTOUCHED — clearing
// the roster's default first would silently boot the wrong model on the very typo being made.
def apply_model_override(var models : array<ModelEntry>; cli_model : string) {
    if (!stat(cli_model).is_valid) {
        to_log(LOG_ERROR, "dasllama-server: ignoring --model - file not found: {cli_model} (the config roster boots unchanged)\n")
        return
    }
    var midx = -1
    for (i in range(length(models))) {
        if (same_model_path(models[i].path, cli_model)) {
            midx = i
            break
        }
    }
    let route = base_name(cli_model)
    if (midx < 0) {
        for (ent in models) {
            if ((empty(ent.name) ? base_name(ent.path) : ent.name) == route) {
                to_log(LOG_ERROR, "dasllama-server: ignoring --model - a different [[models]] entry already routes as '{route}' (rename that entry, or list this path in the roster)\n")
                return
            }
        }
    }
    for (i in range(length(models))) {
        models[i].is_default = i == midx
    }
    if (midx < 0) {
        models |> push(ModelEntry(path = cli_model, is_default = true))
    }
    to_log(LOG_INFO, "dasllama-server: --model {route} overrides the config roster's default\n")
}

// [[models]] roster — per-model path + overrides; absent keys keep the ModelEntry sentinels
// (inherit the flat keys). Hand-written TOML gets the same backend/default checks POST
// /config enforces, in the warn-and-degrade shape config mistakes take everywhere else here.
def private apply_models_roster(root : JsonValue?) {
    delete g_models
    let mv = root?["models"]
    if (mv == null || !(mv.value is _array)) {
        to_log(LOG_WARNING, "dasllama-server: 'models' in the config is not a [[models]] array-of-tables — roster ignored\n")
        return
    }
    var have_default = false
    var seen_routes : table<string>
    for (e in mv.value as _array) {
        let has_mtp = e != null && (e.value is _object) && key_exists(e.value as _object, "mtp")
        var ent = ModelEntry(
            name = e?["name"] ?? "", path = e?["path"] ?? "",
            backend = to_lower(e?["backend"] ?? "auto"),
            ctx = int(e?["ctx"] ?? 0l), quant = e?["quant"] ?? "",
            kv_dtype = e?["kv_dtype"] ?? "",
            streams = int(e?["streams"] ?? -1l), chunk = int(e?["chunk"] ?? -1l),
            page_rows = int(e?["page_rows"] ?? -1l), prefix = int(e?["prefix"] ?? -2l),
            mtp = has_mtp ? ((e?["mtp"] ?? false) ? 1 : 0) : -1,
            image_mmproj = e?["image_mmproj"] ?? "",
            is_default = e?["default"] ?? false)
        if (ent.backend != "auto" && ent.backend != "cpu" && ent.backend != "gpu") {
            to_log(LOG_WARNING, "dasllama-server: [[models]] '{base_name(ent.path)}' backend '{ent.backend}' is not auto | cpu | gpu — using auto\n")
            ent.backend = "auto"
        }
        if (ent.is_default && have_default) {
            to_log(LOG_WARNING, "dasllama-server: more than one [[models]] entry marked default = true — keeping the first\n")
            ent.is_default = false
        }
        have_default ||= ent.is_default
        if (empty(ent.path)) {
            to_log(LOG_WARNING, "dasllama-server: [[models]] entry with no path — skipped\n")
            continue
        }
        let route = empty(ent.name) ? base_name(ent.path) : ent.name
        if (key_exists(seen_routes, route)) {
            to_log(LOG_WARNING, "dasllama-server: duplicate [[models]] name '{route}' — entry skipped (requests route on the first)\n")
            continue
        }
        seen_routes |> insert(route)
        g_models |> push(ent)
    }
    g_cfg_sources["models"] = "toml"
}

def private gpu_auto_pick(metal_ok, vulkan_ok : bool) : string {
    if (metal_ok) {
        return "metal"
    }
    return vulkan_ok ? "vulkan" : "off"
}

var private g_gpu_detected = ""   // cached auto-pick ("" = not probed yet; the probes cache too)

def private detected_gpu() : string {
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
def private gpu_pick_is_auto(cfg : ServerArgs) : bool {
    let l = to_lower(cfg.gpu)
    if (l == "off" || l == "metal" || l == "metal-required" || l == "vulkan") {
        return false
    }
    return l == "auto" || empty(cfg.metal)
}

// --gpu selector: one knob over the metal mode and the vulkan tier. Defaults-first: unset
// (with no legacy --metal flag) and the explicit "auto" spelling both serve on the detected
// backend — `gpu = off` is the opt-out. A set --metal keeps the legacy env-driven path.
def private effective_gpu(cfg : ServerArgs) : string {
    let l = to_lower(cfg.gpu)
    if (l == "off" || l == "metal" || l == "metal-required" || l == "vulkan") {
        return l
    }
    if (!empty(l) && l != "auto") {
        to_log(LOG_WARNING, "dasllama-server: unknown --gpu '{cfg.gpu}', ignoring (valid: auto | off | metal | metal-required | vulkan)\n")
    }
    return gpu_pick_is_auto(cfg) ? detected_gpu() : ""
}

def private cfg_sourced(key : string) : bool
    => key_exists(g_cfg_sources, key)

// vulkan detail knobs — explicit config/CLI wins; unset gets the blessed serving shape
// (K=2 resident + S=35 streamed + DN + ATTN) under gpu = vulkan
// 0 / unset = auto (-1): the loader fills the VRAM budget and streams the rest; a positive value
// pins it exactly. The old hardcoded K=2/S=35 was tuned to one 3060 Ti, and auto measured slightly
// BETTER there (4/36, 26.30 vs 26.11 tg-real256). Treating 0 as auto also removes the control-page
// trap where saving the raw unset sentinels wrote explicit zeros and disarmed the tier on restart.
def private gpu_detail_layers(cfg : ServerArgs) : int64
    => int64(cfg_sourced("gpu_layers") && cfg.gpu_layers > 0 ? cfg.gpu_layers : -1)

def private gpu_detail_stream(cfg : ServerArgs) : int64
    => int64(cfg_sourced("gpu_stream") && cfg.gpu_stream > 0 ? cfg.gpu_stream : -1)

def private gpu_detail_dn(cfg : ServerArgs) : bool
    => cfg_sourced("gpu_dn") ? cfg.gpu_dn : true

def private gpu_detail_attn(cfg : ServerArgs) : bool
    => cfg_sourced("gpu_attn") ? cfg.gpu_attn : true

// The GET /config surface: RESOLVED effective values (what init actually uses) + per-key
// provenance. Sentinel-is-meaning keys stay raw: ctx 0 = model default, prefix 0 = auto /
// -1 = unbounded, threads -1 = all cores, gpu "" = legacy metal/env path.
def private build_config_surface(cfg : ServerArgs; save_path : string) : string {
    let gpu = effective_gpu(cfg)
    var config = JV((
        model = cfg.model,
        port = cfg.port > 0 ? cfg.port : 8080,
        quant = empty(cfg.quant) ? "q8" : cfg.quant,
        kv_dtype = empty(cfg.kv_dtype) ? "f16" : cfg.kv_dtype,
        metal = empty(cfg.metal) ? "off" : cfg.metal,
        gpu = cfg.gpu,
        gpu_layers = gpu == "vulkan" ? int(gpu_detail_layers(cfg)) : cfg.gpu_layers,
        gpu_stream = gpu == "vulkan" ? int(gpu_detail_stream(cfg)) : cfg.gpu_stream,
        gpu_dn = gpu == "vulkan" ? gpu_detail_dn(cfg) : cfg.gpu_dn,
        gpu_attn = gpu == "vulkan" ? gpu_detail_attn(cfg) : cfg.gpu_attn,
        gpu_dense = cfg.gpu_dense,
        gpu_vram_mb = cfg.gpu_vram_mb,
        asr = cfg.asr,
        mmproj = cfg.mmproj,
        image_mmproj = cfg.image_mmproj,
        asr_workers = cfg.asr_workers > 0 ? cfg.asr_workers : 1,
        ctx = cfg.ctx,
        max_tokens = cfg.max_tokens > 0 ? cfg.max_tokens : 16384,
        streams = cfg.streams > 0 ? cfg.streams : 4,
        threads = cfg.threads == 0 ? 16 : cfg.threads,
        team_dispatch = empty(cfg.team_dispatch) ? "hybrid" : cfg.team_dispatch,
        affinity = cfg.affinity,
        chunk = cfg.chunk > 0 ? cfg.chunk : 64,
        page_rows = cfg.page_rows > 0 ? cfg.page_rows : 64,
        prefix = cfg.prefix,
        flat = cfg.flat,
        mtp = cfg.mtp,
        lcpp_bin = cfg.lcpp_bin,
        models_dir = cfg.models_dir))
    var sources <- { for (k in CONFIG_KEYS); k => JV(g_cfg_sources?[k] ?? "default") }
    // the [[models]] roster — the page's roster editor reads/writes this shape (a config-less
    // single-model start serializes as a synthesized one-entry roster)
    var mentries : array<JsonValue?>
    mentries |> reserve(length(g_models))
    for (ent in g_models) {
        var etab <- {
            "name" => JV(ent.name), "path" => JV(ent.path), "backend" => JV(ent.backend),
            "ctx" => JV(int64(ent.ctx)), "quant" => JV(ent.quant), "kv_dtype" => JV(ent.kv_dtype),
            "streams" => JV(int64(ent.streams)), "chunk" => JV(int64(ent.chunk)),
            "page_rows" => JV(int64(ent.page_rows)), "prefix" => JV(int64(ent.prefix)),
            "mtp" => JV(int64(ent.mtp)), "image_mmproj" => JV(ent.image_mmproj),
            "default" => JV(ent.is_default)
        }
        mentries |> push(JV(etab))
    }
    var top <- {
        "config" => config,
        "models" => JV(mentries),
        "sources" => JV(sources),
        "authoritative" => JV(g_cfg_authoritative),
        "config_path" => JV(cfg.config),
        "save_path" => JV(save_path),
        "gpu_effective" => JV(gpu),
        "gpu_detected" => JV(detected_gpu())
    }
    var doc = JV(top)
    let text = write_json_compact(doc)
    unsafe {
        delete doc
    }
    return text
}

// Parse into global lifecycle state. The no-argument overload intentionally wraps clargs'
// parse_args(type<T>) so standalone main has no collectable config local across the GC point.
def parse_args() {
    g_args_parsed = true
    g_args_ready = false
    g_exit_code = 0
    // JIT is mandatory: the kernels are per-box JIT-tuned [llvm_code], the tune framework needs
    // a live JIT, and a baked standalone -exe additionally mis-wires shared-module [init] globals
    // (the vulkan GPU-tier arm probe bakes null -> crash). There is no correct non-JIT path here.
    if (!jit_enabled()) {
        to_log(LOG_ERROR, "dasllama-server requires the JIT — run: daslang -jit main.das -- ... (a baked -exe is unsupported)\n")
        g_exit_code = 1
        request_exit()
        return
    }
    var r <- parse_args(type<ServerArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<ServerArgs>), "dasllama-server")
        g_exit_code = 1
        request_exit()
        return
    }
    g_cfg <- r |> move_unwrap
    if (g_cfg.help) {
        print_help(get_command_info(type<ServerArgs>), "dasllama-server")
        request_exit()
        return
    }
    mark_cli_sources(g_cfg)
    if (empty(g_cfg.config)) {
        // dictation-bot convention: a same-name config next to the program (or in cwd) loads
        // automatically, so a release bundle configures itself with no flags at all
        let here = "dasllama-server.toml"
        let beside = path_join(get_this_module_dir(), "dasllama-server.toml")
        if (stat(here).is_valid) {
            g_cfg.config = here
        } elif (stat(beside).is_valid) {
            g_cfg.config = beside
        }
        if (!empty(g_cfg.config)) {
            print("using config {g_cfg.config}\n")
        }
    }
    if (!empty(g_cfg.config) && !apply_config(g_cfg)) {
        g_exit_code = 1
        request_exit()
        return
    }
    if (empty(g_cfg.model) && empty(g_models)) {   // a [[models]] roster needs no flat model key
        g_setup_mode = true
        print("no model configured - starting in setup mode (open the control page to download one from the catalog)\n")
    }
    g_args_ready = true
}

// Log das-managed memory separately from the watchdog's process RSS/private-byte report. This
// function never collects; its formatted-string temporary is dead before maybe_collect_gc runs.
def private log_memory_if_due() {
    let now = int64(get_clock())
    if (g_last_memory_log_at != 0l && now - g_last_memory_log_at < 60l) {
        return
    }
    g_last_memory_log_at = now
    let heap_used = heap_bytes_allocated()
    let heap_total = heap_total_allocated()
    let string_used = string_heap_bytes_allocated()
    let string_total = string_heap_total_allocated()
    to_log(
        LOG_INFO,
        "dasllama-server: memory das_heap={heap_used}/{heap_total} string_heap={string_used}/{string_total} allocations={heap_allocation_count()} string_allocations={string_heap_allocation_count()}\n")
}

// Standalone-loop GC. Every collectable request-local value lived inside update() and is dead by
// the time main calls this function. daslang-live already collects between update calls.
def maybe_collect_gc() {
    let forced = take_gc_request()
    return if (is_live_mode() && !forced)
    let now = int64(get_clock())
    if (!forced && g_last_gc_at != 0l && now - g_last_gc_at < GC_MIN_INTERVAL_SECONDS) {
        return
    }
    if (forced) {
        g_last_gc_at = now
        let gc_started = ref_time_ticks()
        to_log(
            LOG_INFO,
            "dasllama-server: GC begin reason=forced_endpoint validate=true das_heap={heap_bytes_allocated()}/{heap_total_allocated()} string_heap={string_heap_bytes_allocated()}/{string_heap_total_allocated()}\n")
        unsafe(heap_collect(true, true))
        to_log(
            LOG_INFO,
            "dasllama-server: GC end reason=forced_endpoint elapsed_ms={int64(get_time_usec(gc_started)) / 1000l} das_heap={heap_bytes_allocated()}/{heap_total_allocated()} string_heap={string_heap_bytes_allocated()}/{string_heap_total_allocated()}\n")
        return
    }
    let string_used = string_heap_bytes_allocated()
    let string_total = string_heap_total_allocated()
    if (string_total > 0ul && string_used * 3ul < string_total * 2ul) {
        g_last_gc_at = now
        let gc_started = ref_time_ticks()
        to_log(
            LOG_INFO,
            "dasllama-server: GC begin reason=string_fragmentation validate=true das_heap={heap_bytes_allocated()}/{heap_total_allocated()} string_heap={string_used}/{string_total}\n")
        unsafe(heap_collect(true, true))
        to_log(
            LOG_INFO,
            "dasllama-server: GC end reason=string_fragmentation elapsed_ms={int64(get_time_usec(gc_started)) / 1000l} das_heap={heap_bytes_allocated()}/{heap_total_allocated()} string_heap={string_heap_bytes_allocated()}/{string_heap_total_allocated()}\n")
        return
    }
    let heap_used = heap_bytes_allocated()
    let heap_total = heap_total_allocated()
    if (heap_total > 0ul && heap_used * 3ul < heap_total) {
        g_last_gc_at = now
        let gc_started = ref_time_ticks()
        to_log(
            LOG_INFO,
            "dasllama-server: GC begin reason=heap_fragmentation validate=true das_heap={heap_used}/{heap_total} string_heap={string_heap_bytes_allocated()}/{string_heap_total_allocated()}\n")
        unsafe(heap_collect(true, true))
        to_log(
            LOG_INFO,
            "dasllama-server: GC end reason=heap_fragmentation elapsed_ms={int64(get_time_usec(gc_started)) / 1000l} das_heap={heap_bytes_allocated()}/{heap_total_allocated()} string_heap={string_heap_bytes_allocated()}/{string_heap_total_allocated()}\n")
    }
}

[export]
def init() {   // nolint:STYLE037,STYLE038 — boot sequence: the knobs are order-coupled (dispatch/affinity before the jobque, backend before the load), so the phases cannot move behind a function boundary
    if (!g_args_parsed) {
        parse_args()   // daslang-live calls init directly; standalone main parsed already
    }
    if (!g_args_ready) {
        return
    }
    let port = g_cfg.port > 0 ? g_cfg.port : 8080     // non-positive (incl. a stray negative) -> default
    let ctx = g_cfg.ctx > 0 ? int64(g_cfg.ctx) : 0l   // 0 = keep the model's trained context_length
    let streams = g_cfg.streams > 0 ? int64(g_cfg.streams) : 4l
    let chunk = g_cfg.chunk > 0 ? int64(g_cfg.chunk) : 64l
    let page_rows = g_cfg.flat ? 0l : (g_cfg.page_rows > 0 ? int64(g_cfg.page_rows) : 64l)
    let prefix = int64(g_cfg.prefix)   // 0 = auto (init_server sizes it), < 0 = unbounded
    let dispatch_mode = to_lower(g_cfg.team_dispatch)
    if (!empty(dispatch_mode) && dispatch_mode != "hybrid"
        && dispatch_mode != "team" && dispatch_mode != "inline") {
        print("error: --team-dispatch must be 'hybrid', 'team', or 'inline'\n")
        g_exit_code = 2
        request_exit()
        return
    }
    let team_dispatch = dispatch_mode != "inline"
    let asr_team_dispatch = dispatch_mode == "team"
    // matmul dispatch lane cap: LLM decode saturates memory bandwidth long before big core
    // counts, and an uncapped dispatch fights every other process on the box. A negative config
    // value maps to 0 here, which set_dispatch_worker_limit reads as "no limit" = all cores.
    let threads = g_cfg.threads > 0 ? g_cfg.threads : (g_cfg.threads < 0 ? 0 : 16)
    set_dispatch_worker_limit(threads)
    // Cap the jobque WORKER count at construction from the config (total lanes = workers + the
    // computing main, so `threads` lanes -> threads-1 workers, floored at 1 worker). Without this
    // the queue spawns cores-1 workers on a big box and parks the surplus — which is why
    // DAS_JOBQUE_THREADS used to be mandatory. Must land before the jobque exists, same as affinity
    // below; the env still overrides. threads<=0 means "all cores" -> cap 0 (off, the stock default).
    set_jobque_threads_cap(threads > 0 ? max(1, threads - 1) : 0)
    // worker CPU affinity — must land before the jobque exists (applied at worker spawn);
    // -1 keeps the engine's platform default (darwin QoS via dasllama_math [init])
    if (g_cfg.affinity < -1 || g_cfg.affinity > 2) {
        print("error: --affinity must be -1 (platform default), 0 (off), 1 (hint), or 2 (mask; QoS on darwin)\n")
        g_exit_code = 2
        request_exit()
        return
    }
    if (g_cfg.affinity >= 0) {
        set_jobque_affinity(g_cfg.affinity)
    }
    let lanes_desc = threads > 0 ? "{threads}" : "all"
    let aff_desc = g_cfg.affinity >= 0 ? "{g_cfg.affinity}" : "platform default"
    print("dispatch: {lanes_desc} worker lanes, affinity {aff_desc}\n")
    // tee every to_log/print to logs/dasllama-server.log (timestamped ndjson) AND the console
    logger_init_tee("dasllama-server")
    to_log(LOG_INFO, "dasllama-server: logging to {get_das_root()}/logs/dasllama-server.log\n")

    log_tune_status("dasllama-server")
    // a fresh local tune that has not been shared: submit now (always) or surface the offer
    // (ask — the watchdog balloon + the control page's exchange card); quiet otherwise
    exchange_boot_submit_check(exchange_policy_from_config(g_cfg.config))
    // GPU backend lands BEFORE the load: the metal mode pins the row-major backend the GPU
    // kernels read, and the vulkan tier want must be armed before the load walk uploads stacks
    let gpu = effective_gpu(g_cfg)
    if (gpu_pick_is_auto(g_cfg)) {
        to_log(LOG_INFO, "dasllama-server: gpu backend auto-detected: {gpu} (set gpu = off | metal | vulkan to pin)\n")
    }
    var metal_mode = pick_metal_mode(g_cfg.metal)
    if (!empty(gpu) && !from_cli("gpu") && from_cli("metal")) {
        // gpu is the master backend switch, but an explicit CLI --metal losing to a config-file
        // key silently would invert the documented precedence — say so
        to_log(LOG_WARNING, "dasllama-server: config key gpu = '{gpu}' overrides the explicit --metal flag (drop one of them)\n")
    }
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
        to_log(LOG_INFO, "dasllama-server: prefill runs on the CPU arm - pass --gpu metal for the Metal rails\n")
    }
    to_log(LOG_INFO, "dasllama-server: metal mode {get_metal_mode()}\n")
    if (gpu == "vulkan") {
        // shexp rides the blessed shape: the resident shared expert measured +8.3% on decode
        // (decode-arc S2) and was missing here. The booleans stay explicit rather than riding
        // auto_tier, so an explicit `gpu_dn = false` still turns its rail off.
        set_gpu_tier_want(GpuTierWant(
            moe_layers = gpu_detail_layers(g_cfg), moe_stream = gpu_detail_stream(g_cfg),
            vram_mb = int64(g_cfg.gpu_vram_mb), dn = gpu_detail_dn(g_cfg),
            attn = gpu_detail_attn(g_cfg), dense = g_cfg.gpu_dense, shexp = true))
        if (moe_gpu_tier_arm()) {
            let ktag = gpu_detail_layers(g_cfg) < 0l ? "auto" : "{gpu_detail_layers(g_cfg)}"
            let stag = gpu_detail_stream(g_cfg) < 0l ? "auto" : "{gpu_detail_stream(g_cfg)}"
            to_log(LOG_INFO, "dasllama-server: vulkan GPU tier armed (K={ktag}, S={stag}, dn={gpu_detail_dn(g_cfg)}, attn={gpu_detail_attn(g_cfg)})\n")
        } else {
            to_log(LOG_WARNING, "dasllama-server: gpu = vulkan requested but the tier did not arm (no vulkan device, or this build lacks the vulkan module)\n")
        }
    }
    // resolve the model roster: [[models]] entries, else the single legacy model key
    if (empty(g_models) && !empty(g_cfg.model)) {
        g_models |> push(ModelEntry(path = g_cfg.model, is_default = true))
    } elif (!empty(g_models) && !empty(g_cfg.model) && (g_cfg_sources?["model"] ?? "") == "cli") {
        apply_model_override(g_models, g_cfg.model)
    }
    // A path typo in an AUTHORITATIVE config must not brick the boot into a crash loop: every
    // roster path is checked up front and degrades — skip the slot, drop the arm — instead of
    // dying in a loader panic before the port (and the config page, the recovery UI) opens.
    var roster : array<ModelEntry>
    for (ent in g_models) {
        var e = ent
        if (!stat(e.path).is_valid) {
            to_log(LOG_ERROR, "dasllama-server: skipping model '{empty(e.name) ? base_name(e.path) : e.name}' - file not found: {e.path} (fix the config, then POST /restart)\n")
            continue
        }
        if (!empty(e.image_mmproj) && !stat(e.image_mmproj).is_valid) {
            to_log(LOG_ERROR, "dasllama-server: dropping the vision arm of '{empty(e.name) ? base_name(e.path) : e.name}' - mmproj not found: {e.image_mmproj}\n")
            e.image_mmproj = ""
        }
        roster |> push(e)
    }
    delete g_models
    g_models <- roster
    if (empty(g_models) && !g_setup_mode) {
        // not an exit: a crash-looping boot under the watchdog never gets a page to fix the config from
        g_setup_mode = true
        to_log(LOG_ERROR, "dasllama-server: no loadable model - every roster path failed its file check; starting in setup mode\n")
    }
    if (!empty(g_cfg.image_mmproj) && !stat(g_cfg.image_mmproj).is_valid) {
        to_log(LOG_ERROR, "dasllama-server: dropping --image-mmproj - file not found: {g_cfg.image_mmproj}\n")
        g_cfg.image_mmproj = ""
    }
    if (!g_setup_mode) {
        var def_idx = 0
        for (i in range(length(g_models))) {
            if (g_models[i].is_default) {
                def_idx = i
                break
            }
        }
        // At load time exactly ONE slot arms the GPU tier — the default slot unless its backend pins
        // cpu, else the first gpu/auto entry. The want is toggled per load (the tier arms at each
        // load tail); every non-cpu slot RECORDS the want so the switch-on-demand path (P2b) can
        // re-arm it when the tick loop or POST /v1/models/activate moves the tier.
        var gpu_idx = -1
        var slot_want = GpuTierWant()
        if (gpu == "vulkan") {
            slot_want = GpuTierWant(
                moe_layers = gpu_detail_layers(g_cfg), moe_stream = gpu_detail_stream(g_cfg),
                vram_mb = int64(g_cfg.gpu_vram_mb), dn = gpu_detail_dn(g_cfg),
                attn = gpu_detail_attn(g_cfg), dense = g_cfg.gpu_dense, shexp = true)
            if (g_models[def_idx].backend != "cpu") {
                gpu_idx = def_idx
            } else {
                for (i in range(length(g_models))) {
                    if (g_models[i].backend != "cpu") {
                        gpu_idx = i
                        break
                    }
                }
            }
        }
        set_runtime_load_policy(slot_want, ctx)
        // A serving process prefers degrading (warn + nearest working mode) over dying on an
        // unsupported model/quant combo; API-facing tools keep the strict fail-loud default.
        set_resilient_load(true)
        for (i in range(length(g_models))) {
            var ent & = unsafe(g_models[i])
            if (empty(ent.name)) {
                ent.name = base_name(ent.path)
            }
            if (gpu == "vulkan") {
                set_gpu_tier_want(i == gpu_idx ? slot_want : GpuTierWant())   // zero want: loads CPU-only
            }
            print("loading {ent.path} ...\n")
            var m <- load_model(ent.path, pick_quant(!empty(ent.quant) ? ent.quant : g_cfg.quant))
            let ectx = ent.ctx > 0 ? int64(ent.ctx) : ctx
            if (ectx > 0l) {
                m.config.seq_len = min(m.config.seq_len, ectx)
            }
            let ctx_note = ectx > 0l ? "" : " (model default)"
            to_log(LOG_INFO, "dasllama-server: {empty(ent.name) ? base_name(ent.path) : ent.name} ctx {m.config.seq_len}{ctx_note}\n")
            var opts = SlotOpts(backend = empty(ent.backend) ? "auto" : ent.backend)
            if (ent.streams > 0) {
                opts.streams = int64(ent.streams)
            }
            if (ent.chunk > 0) {
                opts.chunk = int64(ent.chunk)
            }
            if (ent.page_rows >= 0) {
                opts.page_rows = int64(ent.page_rows)
            }
            if (ent.prefix != -2) {
                opts.prefix = int64(ent.prefix)
            }
            if (!empty(ent.kv_dtype)) {
                opts.kv_dtype = pick_kv_dtype(ent.kv_dtype)
                opts.kv_dtype_set = true
            }
            opts.mtp = ent.mtp
            add_model(m, ent.name, opts, ent.path)   // captures this load's arm outcome + marks onto the slot
            if (gpu == "vulkan" && ent.backend != "cpu") {
                set_slot_gpu_want(ent.name, slot_want)   // the want switch-in re-arms with
            }
        }
        set_default_model(g_models[def_idx].name)
        // 16384, not a chat-sized 256: thinking-default families spend the budget's head on the
        // thought channel, and a request that omits max_tokens must still get its whole answer
        set_default_max_tokens(g_cfg.max_tokens > 0 ? int64(g_cfg.max_tokens) : 16384l)
        for (i in range(length(g_models))) {
            let path = (!empty(g_models[i].image_mmproj) ? g_models[i].image_mmproj
                                                         : (i == def_idx ? g_cfg.image_mmproj : ""))
            if (!empty(path)) {
                set_model_vision(g_models[i].name, path)
                // the E-series mmproj carries BOTH towers: the same file arms native audio, so
                // one download gives text + vision + audio off the one serving slot
                if (gemma4a_probe_proj_dim(path) > 0l) {
                    set_model_audio(g_models[i].name, path)
                }
            }
        }

        // the dlim GC rides server start (and every bake): only never-loadable images go
        for (ent in g_models) {
            let gc = dlim_clean(ent.path, true)
            if (gc.removed > 0) {
                to_log(LOG_INFO, "dasllama-server: dlim GC reclaimed {gc.freed >> 20l} MB from {gc.removed} stale image(s) beside {base_name(ent.path)}\n")
            }
        }

        if (!empty(g_cfg.asr)) {
            configure_asr_workers(g_cfg.asr, g_cfg.mmproj, g_cfg.asr_workers > 0 ? g_cfg.asr_workers : 1)
        }
    }

    // the /config surface: a config-less start saves beside the program (the same spot the
    // auto-discovery in parse_args looks)
    let save_path = !empty(g_cfg.config) ? g_cfg.config : path_join(get_this_module_dir(), "dasllama-server.toml")
    // the served-model anchor for the page's model_files picker: the flat key, else the roster default
    var picker_model = g_cfg.model
    if (empty(picker_model)) {
        for (ent in g_models) {
            if (ent.is_default) {
                picker_model = ent.path
                break
            }
        }
    }
    if (empty(picker_model) && !empty(g_models)) {
        picker_model = g_models[0].path   // roster with no surviving default: first entry anchors the picker
    }
    set_config_surface(build_config_surface(g_cfg, save_path), save_path, picker_model)
    let catalog_dir = catalog_models_dir(g_cfg.models_dir)
    set_catalog_dir(catalog_dir)
    if (!empty(catalog_env_override())) {
        // override-announce rule: the env wins the catalog resolution over the config key
        to_log(LOG_INFO, "dasllama-server: models dir {catalog_dir} (DASLLAMA_MODELS_DIR overrides the models_dir config key)\n")
    }
    set_setup_mode(g_setup_mode)
    // the bench button runs OUR bench as a child daslang process (lcpp_bench.das) — a
    // standalone-exe deployment has no script runner, so the button stays disabled there
    var daslang_bin = ""
    if (!is_standalone_exe()) {
        for (a in get_command_line_arguments()) {   // argv[0] = the running daslang binary (locked temp — no delete)
            daslang_bin = a
            break
        }
    }
    set_bench_config(g_cfg.lcpp_bin, daslang_bin)

    g_server_ready = init_server(
        port, streams,
        [chunk_tokens = chunk, page_rows = page_rows, prefix_groups = prefix,
         kv_dtype = pick_kv_dtype(g_cfg.kv_dtype), team_dispatch = team_dispatch,
         asr_team_dispatch = asr_team_dispatch, mtp = g_cfg.mtp])
    if (!g_server_ready) {
        g_exit_code = 2
        request_exit()
    }
}

[export]
def update() {
    if (!g_server_ready || !update_server()) {
        request_exit()
    }
    log_memory_if_due()
}

[export]
def shutdown() {
    shutdown_server()
    g_server_ready = false
    logger_flush()
}

// missing = "restart": an untuned first start writes the tune sidecar and returns 3 before
// parse_args/init can create the JobQue, load the model, or bind the port. The watchdog relaunches.
[export, tune_policy(missing = "restart")]
def main : int {
    parse_args()
    init()
    while (!exit_requested()) {
        update()
        maybe_collect_gc()
    }
    shutdown()
    if (g_exit_code == 0 && server_restart_requested()) {
        g_exit_code = 4   // watchdog contract: config-restart relaunch (3 stays tune-restart)
    }
    return g_exit_code
}
