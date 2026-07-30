options gen2
options _comment_hygiene = true
options indenting = 4
options rtti                                 // dasOPENAI needs rtti so @optional fields serialize correctly

// prepare_recording — pre-render the voiceover track for a tutorial recording.
//
// Compiles the given record_*.das driver (same module resolution as running it: if it won't
// compile, there is no prepare and no recording), walks its AST to collect every say() line
// plus the APNG basename, synthesizes each unique line to a wav via Kokoro TTS, measures the
// durations, and writes voiceover/<stem>.manifest.json beside where the APNG will land.
// with_recording_app auto-detects that manifest at record time (see imgui_playwright.das).

require imgui                              // umbrella — drivers `require imgui`; load it (shared) so compile_file reuses it
require imgui/imgui_playwright              // VoEntry / VoManifest / say_hash / RECORD_ASSET_REL / dasimgui_module_root
require openai/openai_audio                 // speech / SpeechRequest / openai_client
require daslib/ast_boost                    // compile_file / VisitorAdapter / visit
require audio/audio_wav                     // read_wav
require daslib/fio
require daslib/json
require daslib/json_boost
require daslib/clargs
require strings
require daslib/strings_boost

[CommandLineArgs]
struct Config {
    @clarg_doc = "Path to the record_*.das driver to scan for say() lines"
    driver : string

    @clarg_doc = "Kokoro TTS voice"
    voice : string = "bf_emma"

    @clarg_doc = "TTS model id"
    model : string = "kokoro"

    @clarg_doc = "OpenAI-compatible base URL (Kokoro)"
    base_url : string = "http://127.0.0.1:8880/v1"

    @clarg_doc = "Silence appended after each spoken line, milliseconds"
    gap_ms : int = 500

    @clarg_doc = "Asset root whose doc/source/_static/tutorials/voiceover/ receives the manifest + wavs (default: dasImgui's module root). Pass the consumer package root for a non-dasImgui driver (e.g. a dasImguiNodeEditor recording) so the manifest lands beside where its APNG will be written."
    asset_root : string

    @clarg_short = "?"
    @clarg_doc = "Show help and exit"
    help : bool
}

// Collected by the AST scan (module globals so the visitor appends directly).
struct SayLine {
    caption : string    // say() text arg — the on-screen caption
    spoken  : string    // say() voice arg, or caption when voice is empty — the TTS input
}
var g_lines : array<SayLine>
var g_apng : string

def const_str(e : Expression?) : string {
    if (e == null || !(e is ExprConstString)) return ""
    return string((e as ExprConstString).value)
}

class ScanVisitor : AstVisitor {
    def override preVisitExprCall(expr : ExprCall?) : void {
        let name = string(expr.name)
        if (name == "say" || name == "say_begin") {
            // say[_begin](app, text, target, read_ms, voice) — by compile time all 5 are positional
            // (defaults materialized): caption at [1], voice at [4] (empty "" when omitted). Both are
            // scanned because a click-under-voice driver calls say_begin directly with literal args.
            if (length(expr.arguments) >= 5) {
                let caption = const_str(expr.arguments[1])
                let voice = const_str(expr.arguments[4])
                if (!empty(caption)) {
                    g_lines |> push(SayLine(caption = caption,
                        spoken = !empty(voice) ? voice : caption))
                }
            }
        } elif (ends_with(name, "_recording_app")) {
            // with_recording_app or any consumer wrapper (with_node_editor_recording_app,
            // with_implot_recording_app, ...) — 2nd positional argument is the APNG basename
            if (length(expr.arguments) >= 2) {
                let s = const_str(expr.arguments[1])
                if (!empty(s)) {
                    g_apng = s
                }
            }
        }
    }
}

def scan_driver(driver : string) : bool {
    //! Compile the driver and collect say() texts + the APNG basename. False on compile failure.
    var ok_compiled = false
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.no_optimizations = true   // imgui shared modules are reused from prepare's own require chain — do NOT ignore them
            compile_file(driver, access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    to_log(LOG_ERROR, "[prepare] driver won't compile (no prepare, no recording):\n{issues}\n")
                    return
                }
                var astVisitor = new ScanVisitor()
                make_visitor(*astVisitor) $(adapter) {
                    visit(program, adapter)
                }
                unsafe {
                    delete astVisitor
                }
                ok_compiled = true
            }
        }
    }
    return ok_compiled
}

def wav_duration(path : string) : float {
    //! Duration in seconds. read_wav for 16-bit PCM; ffprobe fallback for anything else.
    var samples : array<float>
    var rate = 0
    var channels = 0
    if (read_wav(path, samples, rate, channels) && rate > 0 && channels > 0)
        return float(length(samples)) / float(rate) / float(channels)
    let args <- ["ffprobe", "-v", "error", "-show_entries", "format=duration",
        "-of", "default=nokey=1:noprint_wrappers=1", path]
    var out : string
    run_and_capture(args, out)
    return to_float(strip(out))
}

[export]
def main : int {
    var args_r <- parse_args(type<Config>)
    if (args_r |> is_err) {
        to_log(LOG_ERROR, "[prepare] {args_r |> unwrap_err}\n")
        print_help(get_command_info(type<Config>), "prepare_recording")
        return 1
    }
    let cfg <- args_r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "prepare_recording")
        return 0
    }
    if (empty(cfg.driver)) {
        to_log(LOG_ERROR, "[prepare] --driver is required\n")
        return 1
    }
    if (!scan_driver(cfg.driver)) return 1
    if (empty(g_apng)) {
        to_log(LOG_ERROR, "[prepare] no with_recording_app(...) APNG basename found in {cfg.driver}\n")
        return 1
    }
    if (empty(g_lines)) {
        to_log(LOG_WARNING, "[prepare] no say() lines found in {cfg.driver}\n")
        return 0
    }
    to_log(LOG_INFO, "[prepare] scanned {cfg.driver}: apng={g_apng}, {length(g_lines)} say() line(s)\n")
    let apng_stem = stem(g_apng)
    // Manifest + wavs must land beside where the APNG will be written (arm_voiceovers reads
    // dir_name(apng)/voiceover at record time). dasImgui drivers root it under dasImgui's module
    // root; a consumer package roots it under its own and passes --asset-root to redirect there.
    let root = empty(cfg.asset_root) ? dasimgui_module_root() : cfg.asset_root
    let vo_dir = path_join(path_join(root, RECORD_ASSET_REL), "voiceover")
    mkdir_rec(vo_dir)
    let client = openai_client(cfg.base_url)
    var manifest = VoManifest(voice = cfg.voice, gap_ms = cfg.gap_ms)
    var seen : table<string>
    for (line in g_lines) {
        // Hash + TTS on the spoken line; the caption rides along in VoEntry.text for reference.
        let h = say_hash(line.spoken)
        continue if (key_exists(seen, h))
        seen |> insert(h)
        let wav_name = "{apng_stem}_{h}.wav"
        let wav_path = path_join(vo_dir, wav_name)
        let st = stat(wav_path)
        if (!st.is_valid) {
            let res = speech(client, SpeechRequest(model = cfg.model, voice = cfg.voice,
                input = line.spoken, response_format = "wav"))
            if (!res.ok) {
                to_log(LOG_ERROR, "[prepare] TTS failed for: \"{line.spoken}\"\n")
                return 1
            }
            fopen(wav_path, "wb") $(f) {
                if (f != null) {
                    fwrite(f, res.audio)
                }
            }
        }
        let dur = wav_duration(wav_path)
        manifest.entries |> push(VoEntry(hash = h, text = line.caption,
            voice = line.spoken, wav = wav_name, dur_s = dur))
        to_log(LOG_INFO, "[prepare] {wav_name}  {dur}s  \"{line.spoken}\"\n")
    }
    let manifest_path = path_join(vo_dir, "{apng_stem}.manifest.json")
    if (!fwrite(manifest_path, write_json(JV(manifest)))) {
        to_log(LOG_ERROR, "[prepare] failed to write {manifest_path}\n")
        return 1
    }
    to_log(LOG_INFO, "[prepare] wrote {manifest_path} ({length(manifest.entries)} entries)\n")
    return 0
}
