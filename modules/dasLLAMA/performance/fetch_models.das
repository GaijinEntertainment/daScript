options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options indenting = 4

// fetch_models — the committed provenance manifest + fetch/verify rig for the model set
// (BRINGUP.md §2). One entry per catalog carrier: the exact HF source (revision-pinned where
// upstream re-uploads bit-different files under the same name), canonical bytes + sha256, and
// the on-box conversion recipe for the files no registry serves. Verify is the default; --fetch
// downloads what is absent. Roots come from the usual env: DASLLAMA_MODELS_DIR ("llm") and
// WHISPER_CPP_MODELS ("whisper" — point it at the llm dir to keep ONE dir per box).
//   bin/daslang modules/dasLLAMA/performance/fetch_models.das -- [--fetch] [--dry-run] [-o <substr>]

require daslib/clargs
require daslib/fio
require strings
require profile_common
require llvm/daslib/llvm_tune   // [tune_policy]: provisioning only — never auto-tune

[CommandLineArgs]
struct FetchArgs {
    @clarg_short = "f"
    @clarg_doc = "Download absent fetchable models (default: verify-only — report, touch nothing)"
    fetch : bool

    @clarg_short = "n"
    @clarg_doc = "Print what --fetch would download, download nothing"
    dry_run : bool

    @clarg_short = "o"
    @clarg_doc = "Only entries whose local name contains this substring"
    only : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! One provenance row: local catalog `name` under `root` ("llm" = DASLLAMA_MODELS_DIR,
//! "whisper" = WHISPER_CPP_MODELS), the exact HF `url` ("" = converted on-box per `recipe`),
//! canonical `bytes` + `sha256` ("" = the conversion does real math, so bytes are
//! per-architecture — the gate is the recipe's output-parity command, never a hash).
struct ProvEntry {
    name : string
    root : string
    url : string
    bytes : int64
    sha256 : string
    recipe : string
}

let private HF = "https://huggingface.co"
let private PARAKEET_RECIPE = "convert: <pinned whisper.cpp>/models/convert-parakeet-to-ggml.py --use-f32 over the nvidia/parakeet-tdt-0.6b .nemo (nemo venv; repack — sha canonical everywhere)"
let private CANARY_ENC_RECIPE = "convert: modules/dasLLAMA/harness/convert_canary_to_ggml.py over nvidia/canary-qwen-2.5b (nemo venv; encoder is a repack — sha canonical everywhere)"
let private CANARY_DEC_RECIPE = "convert: modules/dasLLAMA/harness/convert_canary_to_ggml.py over nvidia/canary-qwen-2.5b (nemo venv); bytes are per-arch (fp16 LoRA merge) — gate via asr_bench --text over jfk/jfk3/gb1 vs benchmarks/asr/canary_transcripts.expected"

//! The full model set with provenance — LLM board carriers (`pub_catalog` names) first, then
//! the ASR carriers (`asr_catalog` names). Local name differs from the upstream one where the
//! catalog renamed (UD/MTP stems, the E2B mmproj's lowercase -bf16); the url keeps the remote
//! spelling. gpt-oss is revision-pinned; every unpinned repo was byte-verified 2026-07-26.
def models_provenance() : array<ProvEntry> {
    return <- [
        // ===== LLM board (pub_catalog), ascending size =====
        ProvEntry(name = "gemma-4-12B-it-Q4_K_M.gguf", root = "llm",
            url = "{HF}/lmstudio-community/gemma-4-12B-it-GGUF/resolve/main/gemma-4-12B-it-Q4_K_M.gguf",
            bytes = 7381382944l, sha256 = "95d83ba36642b1f385fb906b5962a71763361be3bac930a709945f72d97473f8"),
        ProvEntry(name = "gemma-4-E4B-it-Q8_0.gguf", root = "llm",
            url = "{HF}/ggml-org/gemma-4-E4B-it-GGUF/resolve/main/gemma-4-E4B-it-Q8_0.gguf",
            bytes = 8031242688l, sha256 = "34be82b17b4942d389b9b527170c4b058027abdd32531fda063d3d97dd8ce80a"),
        ProvEntry(name = "gpt-oss-20b-mxfp4.gguf", root = "llm",
            url = "{HF}/ggml-org/gpt-oss-20b-GGUF/resolve/e1dc459feff949ff451ce107337a2026daa80df8/gpt-oss-20b-mxfp4.gguf",
            bytes = 12109566560l, sha256 = "be37a636aca0fc1aae0d32325f82f6b4d21495f06823b5fbc1898ae0303e9935"),
        ProvEntry(name = "Mistral-Small-3.1-24B-Instruct-2503-Q4_K_M.gguf", root = "llm",
            url = "{HF}/unsloth/Mistral-Small-3.1-24B-Instruct-2503-GGUF/resolve/main/Mistral-Small-3.1-24B-Instruct-2503-Q4_K_M.gguf",
            bytes = 14333910592l, sha256 = "6d670773c3908584349d41a5048d1472226b593c881fd394e8ac196c802e81e2"),
        ProvEntry(name = "gemma-4-26B-A4B-it-Q4_K_M.gguf", root = "llm",
            url = "{HF}/unsloth/gemma-4-26B-A4B-it-GGUF/resolve/main/gemma-4-26B-A4B-it-UD-Q4_K_M.gguf",
            bytes = 16947541728l, sha256 = "f2c28b3dc4776931ac6f879e11f203dec637ea0f14267a86ec8f6165f63f293f"),
        ProvEntry(name = "Qwen3.6-27B-MTP-Q4_K_M.gguf", root = "llm",
            url = "{HF}/unsloth/Qwen3.6-27B-MTP-GGUF/resolve/main/Qwen3.6-27B-Q4_K_M.gguf",
            bytes = 17106773120l, sha256 = "a7cbd3ecc0e3f9b333edee61ae66bc87ed713c5d49587a8355814722ed329e0f"),
        ProvEntry(name = "Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf", root = "llm",
            url = "{HF}/unsloth/Qwen3-30B-A3B-Instruct-2507-GGUF/resolve/main/Qwen3-30B-A3B-Instruct-2507-Q4_K_M.gguf",
            bytes = 18556686752l, sha256 = "6c997b8af17debdfb01d890214400ccbab00db6acc0ba8da5de1cc906c4774d0"),
        ProvEntry(name = "Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf", root = "llm",
            url = "{HF}/unsloth/Qwen3.6-35B-A3B-MTP-GGUF/resolve/main/Qwen3.6-35B-A3B-UD-Q4_K_M.gguf",
            bytes = 22663387424l, sha256 = "0b21525e972670ed59e1812e170b27c26355381f0656ecc4e25617ece7dac58b"),
        // ===== ASR carriers (asr_catalog) =====
        ProvEntry(name = "ggml-tiny.bin", root = "whisper",
            url = "{HF}/ggerganov/whisper.cpp/resolve/main/ggml-tiny.bin",
            bytes = 77691713l, sha256 = "be07e048e1e599ad46341c8d2a135645097a538221678b7acdd1b1919c6e1b21"),
        ProvEntry(name = "ggml-large-v3-turbo.bin", root = "whisper",
            url = "{HF}/ggerganov/whisper.cpp/resolve/main/ggml-large-v3-turbo.bin",
            bytes = 1624555275l, sha256 = "1fc70f774d38eb169993ac391eea357ef47c88757ef72ee5943879b7e8e2bc69"),
        ProvEntry(name = "ggml-parakeet-tdt-0.6b-v2-f32.bin", root = "whisper", url = "",
            bytes = 2471664028l, sha256 = "9dde716823d0dfe587cb7396a7cc251708c30a8475f6fccc8af36ee769fcd808",
            recipe = PARAKEET_RECIPE),
        ProvEntry(name = "ggml-parakeet-tdt-0.6b-v3-f32.bin", root = "whisper", url = "",
            bytes = 2508463079l, sha256 = "b17956ca991a7f084c978a8a9d4c9785864260b6dd9b72f884344759d913ae7f",
            recipe = PARAKEET_RECIPE),
        ProvEntry(name = "gemma-4-E2B-it-Q8_0.gguf", root = "llm",
            url = "{HF}/ggml-org/gemma-4-E2B-it-GGUF/resolve/main/gemma-4-E2B-it-Q8_0.gguf",
            bytes = 4967497152l, sha256 = "996d08777aadc6bfd3c7375ef70ba25a0f55240075860754fdb18d6d860aa63a"),
        ProvEntry(name = "mmproj-gemma-4-E2B-it-bf16.gguf", root = "llm",
            url = "{HF}/ggml-org/gemma-4-E2B-it-GGUF/resolve/main/mmproj-gemma-4-E2B-it-BF16.gguf",
            bytes = 986833664l, sha256 = "711e1e8f43fa0664adbac493129be1e6c25b81af4b4cdea97c7d798b25c0a3a4"),
        ProvEntry(name = "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf", root = "llm",
            url = "{HF}/ggml-org/Qwen3-Omni-30B-A3B-Instruct-GGUF/resolve/main/Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf",
            bytes = 32484494336l, sha256 = "8a50e5a7d29ae6a28fea9ca45e3bb0a142e76ec07e6787a7703cd498eb08ffaa"),
        ProvEntry(name = "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf", root = "llm",
            url = "{HF}/ggml-org/Qwen3-Omni-30B-A3B-Instruct-GGUF/resolve/main/mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf",
            bytes = 2207465568l, sha256 = "f0dfe825fb692d426362b1ac79678fc08daa4758f7151526cad110515f122883"),
        ProvEntry(name = "canary-qwen-2.5b-encoder-f32.bin", root = "llm", url = "",
            bytes = 3252768256l, sha256 = "37d1057ced3150cfde2fb996c1fc0a3d702d23a97f9347aedb52795ebec03e5c",
            recipe = CANARY_ENC_RECIPE),
        ProvEntry(name = "canary-qwen-2.5b-decoder-f16.gguf", root = "llm", url = "",
            bytes = 0l, sha256 = "", recipe = CANARY_DEC_RECIPE)
    ]
}

def private entry_path(e : ProvEntry) : string {
    let root = e.root == "whisper" ? whisper_models_dir() : models_dir()
    return "{root}{e.name}"
}

// fetch straight to the destination: -C - resumes a partial file, and the size+sha gates below
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
    for (e in models_provenance()) {
        if (!empty(cfg.only) && find(e.name, cfg.only) < 0) {
            continue
        }
        let path = entry_path(e)
        var st = stat(path)
        if (!st.is_valid && !empty(e.url)) {
            if (!cfg.fetch || cfg.dry_run) {
                to_log(LOG_WARNING, "fetch_models: {e.name} — ABSENT{cfg.dry_run ? ", would fetch" : ""} ({e.url})\n")
                pending++
                continue
            }
            fetch_one(e, path)
            st = stat(path)
            fetched++
        }
        if (!st.is_valid) {
            to_log(LOG_WARNING, "fetch_models: {e.name} — ABSENT, converted on-box: {e.recipe}\n")
            pending++
            continue
        }
        if (empty(e.sha256)) {
            to_log(LOG_INFO, "fetch_models: {e.name} — present; {e.recipe}\n")
            ok++
            continue
        }
        if (int64(st.size) != e.bytes) {
            to_log(LOG_ERROR, "fetch_models: {e.name} — SIZE MISMATCH (have {st.size}, provenance {e.bytes}) — different bytes would silently bench a different model\n")
            failed++
            continue
        }
        let fi = file_identity(path, "weights")
        if (fi.sha256 != e.sha256) {
            to_log(LOG_ERROR, "fetch_models: {e.name} — sha MISMATCH (have {slice(fi.sha256, 0, 12)}…, provenance {slice(e.sha256, 0, 12)}…) — wrong upstream build, find the recorded repo/revision, never waive\n")
            failed++
            continue
        }
        to_log(LOG_INFO, "fetch_models: {e.name} — ok\n")
        ok++
    }
    to_log(failed > 0 ? LOG_ERROR : LOG_INFO,
        "fetch_models: {ok} ok, {fetched} fetched, {pending} pending, {failed} failed\n")
    return failed > 0 || pending > 0 ? 1 : 0
}
