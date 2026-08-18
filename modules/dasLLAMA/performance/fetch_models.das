options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options indenting = 4

// fetch_models — the fetch/verify driver over the provenance view (BRINGUP.md §2). The data
// lives in model_specs.das (the model-set table) and profile_common's asr_catalog;
// models_provenance renders both. Per carrier: the exact HF source (revision-pinned where
// upstream re-uploads bit-different files under the same name), canonical bytes + sha256, and
// the on-box conversion recipe for the files no registry serves. Verify is the default;
// --fetch downloads what is absent. Roots
// come from the usual env: DASLLAMA_MODELS_DIR ("llm") and WHISPER_CPP_MODELS ("whisper" —
// point it at the llm dir to keep ONE dir per box).
//   bin/daslang modules/dasLLAMA/performance/fetch_models.das -- [--fetch] [--dry-run] [-o <substr>]

require daslib/clargs
require daslib/fio
require strings
require profile_common
require llvm/daslib/llvm_tune   // [tune_policy]: provisioning only — never auto-tune
require dasllama/dasllama_env

[CommandLineArgs]
struct FetchArgs {
    @clarg_short = "f"
    @clarg_doc = "Download absent fetchable models (default: verify-only — report, touch nothing)"
    fetch : bool

    @clarg_short = "n"
    @clarg_doc = "Print what --fetch would download, download nothing"
    dry_run : bool

    @clarg_doc = "Run the on-box conversion recipes for absent converted entries (parakeet v2/v3, canary) — needs the setup_asr_rig nemo venv + refs; POSIX boxes only"
    convert : bool

    @clarg_short = "o"
    @clarg_doc = "Only entries whose local name contains this substring"
    only : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private entry_path(e : ProvEntry) : string {
    let root = e.root == "whisper" ? whisper_models_dir() : models_dir()
    return "{root}{e.name}"
}

// fetch straight to the destination: -C - resumes a partial file, and the size+sha gates below
// ===== the on-box conversion drivers (--convert) =====
// The recipes as executable steps, needing setup_asr_rig's rig: NEMO_PY (the venv python),
// WHISPER_CPP (the pinned checkout, parakeet converter + models dir), MTMD_BIN (whose own
// llama.cpp checkout serves the canary decoder's gguf writer — no extra env).

def private convert_nemo_py() : string {
    let p = g_env_prof.nemo_py
    return empty(p) ? "modules/dasLLAMA/benchmarks/asr/.venv-nemo/bin/python" : p
}

def private convert_hub_root() : string {
    let hf = g_env_core.hf_home
    return "{hf}/hub" if (!empty(hf))
    let home = g_env_core.home
    if (empty(home)) {
        to_log(LOG_ERROR, "convert: neither HF_HOME nor HOME is set - cannot locate the huggingface hub cache\n")
        return ""
    }
    return "{home}/.cache/huggingface/hub"
}

def private hub_download(py, expr : string) : string {
    let out = run_capture("{py} -c \"from huggingface_hub import {expr}\"", true)
    var last = ""
    for (ln in split(out, "\n")) {
        if (!empty(strip(ln))) {
            last = strip(ln)
        }
    }
    return last
}

def private convert_parakeet(e : ProvEntry; path : string) {
    let py = convert_nemo_py()
    let v = find(e.name, "-v3-") >= 0 ? "v3" : "v2"
    let src = hub_download(py, "hf_hub_download; print(hf_hub_download('nvidia/parakeet-tdt-0.6b-{v}','parakeet-tdt-0.6b-{v}.nemo'))")
    if (empty(src) || !stat(src).is_valid) {
        to_log(LOG_ERROR, "fetch_models: --convert {e.name} - the .nemo download failed\n")
        return
    }
    // the converter's default output name is ggml-model-f32.bin — --out-name is load-bearing
    // (the zen4 bringup lost v2 to a v3 overwrite without it)
    run_capture("{py} \"{g_env_prof.whisper_cpp}/models/convert-parakeet-to-ggml.py\" --model \"{src}\" --out-dir \"{dir_name(path)}\" --use-f32 --out-name {e.name}", false)
}

def private convert_canary(path : string) {
    let py = convert_nemo_py()
    let hub = convert_hub_root()
    return if (empty(hub))
    hub_download(py, "snapshot_download; print(snapshot_download('nvidia/canary-qwen-2.5b'))")
    hub_download(py, "snapshot_download; print(snapshot_download('Qwen/Qwen3-1.7B', allow_patterns=['tokenizer*','*.json']))")
    // MTMD_BIN sits at <checkout>/build-cpu/bin/llama-mtmd-cli — its checkout carries the
    // gguf writer the decoder conversion drives
    let lcpp = dir_name(dir_name(dir_name(g_env_prof.mtmd_bin)))
    run_capture("{py} modules/dasLLAMA/harness/convert_canary_to_ggml.py --src \"{hub}/models--nvidia--canary-qwen-2.5b\" --qwen3-tokenizer \"{hub}/models--Qwen--Qwen3-1.7B\" --llama-cpp \"{lcpp}\" --out-dir \"{dir_name(path)}\" --decoder-outtype f16", false)
}

// one canary invocation produces both the encoder and the decoder
def private try_convert(e : ProvEntry; path : string; var canary_done : bool&) {
    if (get_platform_name() == "windows") {
        to_log(LOG_ERROR, "fetch_models: --convert is POSIX-only (the venv/converter paths differ on Windows) - run the recipes by hand: {e.recipe}\n")
        return
    }
    if (find(e.name, "parakeet") >= 0) {
        convert_parakeet(e, path)
    } elif (find(e.name, "canary") >= 0) {
        if (!canary_done) {
            convert_canary(path)
            canary_done = true
        }
    }
}

// are the acceptance test — curl's exit status is not (run_capture swallows it anyway)
def private fetch_one(e : ProvEntry; path : string) {
    to_log(LOG_INFO, "fetch_models: START {e.name} ({e.bytes / 1000000l} MB)\n")
    run_capture("curl -sSL -C - --retry 5 --retry-delay 10 -o \"{path}\" \"{e.url}\"", false)
}

[export, tune_policy(missing = "fallback")]
def main() : int {
    var r <- parse_args(type<FetchArgs>)
    if (r |> is_err) {
        to_log(LOG_ERROR, "fetch_models: {r |> unwrap_err}\n")
        print_help(get_command_info(type<FetchArgs>), "fetch_models")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<FetchArgs>), "fetch_models")
        return 0
    }
    var ok = 0
    var fetched = 0
    var pending = 0
    var failed = 0
    var canary_done = false
    for (e in models_provenance()) {
        if (!empty(cfg.only) && find(e.name, cfg.only) < 0) {
            continue
        }
        let path = entry_path(e)
        var st = stat(path)
        if (!st.is_valid && empty(e.url) && cfg.convert && !empty(e.recipe) && e.recipe != REF_Q8_RECIPE) {
            try_convert(e, path, canary_done)
            st = stat(path)
        }
        if (!st.is_valid && !empty(e.url)) {
            if (!cfg.fetch || cfg.dry_run) {
                to_log(LOG_WARNING, "fetch_models: {e.name} - ABSENT{cfg.dry_run ? ", would fetch" : ""} ({e.url})\n")
                pending++
                continue
            }
            fetch_one(e, path)
            st = stat(path)
            if (st.is_valid) {   // run_capture swallows curl's exit code — the file IS the receipt
                fetched++
            }
        }
        if (!st.is_valid) {
            if (empty(e.recipe)) {
                to_log(LOG_ERROR, "fetch_models: {e.name} - ABSENT and the fetch produced nothing ({e.url})\n")
                failed++
            } else {
                to_log(LOG_WARNING, "fetch_models: {e.name} - ABSENT, converted on-box: {e.recipe}\n")
                pending++
            }
            continue
        }
        if (empty(e.sha256)) {
            to_log(LOG_INFO, "fetch_models: {e.name} - present; {e.recipe}\n")
            ok++
            continue
        }
        if (int64(st.size) != e.bytes) {
            to_log(LOG_ERROR, "fetch_models: {e.name} - SIZE MISMATCH (have {int64(st.size)}, provenance {e.bytes}) - different bytes would silently bench a different model\n")
            failed++
            continue
        }
        let fi = file_identity(path, "weights")
        if (fi.sha256 != e.sha256) {
            to_log(LOG_ERROR, "fetch_models: {e.name} - sha MISMATCH (have {slice(fi.sha256, 0, 12)}..., provenance {slice(e.sha256, 0, 12)}...) - wrong upstream build, find the recorded repo/revision, never waive\n")
            failed++
            continue
        }
        to_log(LOG_INFO, "fetch_models: {e.name} - ok\n")
        ok++
    }
    to_log(failed > 0 ? LOG_ERROR : LOG_INFO,
        "fetch_models: {ok} ok, {fetched} fetched, {pending} pending, {failed} failed\n")
    return failed > 0 || pending > 0 ? 1 : 0
}
