options gen2

require dasllama_lib_c
require daslib/clargs

// dasLLAMA from daslang, through the same C ABI the C host uses: `daslang -lib` writes these
// bindings, so this host declares nothing by hand and never requires dasllama.
//
//   bin/daslang examples/dasLLAMA/library/main.das -- --model <model.gguf> --prompt "..." --tokens 64
//   bin/daslang examples/dasLLAMA/library/main.das -- --model <asr-model> --audio <audio-file>

[CommandLineArgs]
struct HostArgs {
    @clarg_short = "m"
    @clarg_doc = "The model to load: a GGUF to continue text, a whisper .bin to transcribe"
    model : string

    @clarg_short = "p"
    @clarg_doc = "The prompt to continue"
    prompt : string = "Once upon a time"

    @clarg_short = "n"
    @clarg_doc = "How many tokens to generate"
    tokens : int = 48

    @clarg_short = "a"
    @clarg_doc = "Transcribe this audio file instead of continuing a prompt"
    audio : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! a panic lands in the generated last_error, a refusal in the library's own reason
def private why(ctx : void?) : string {
    let panicked = dasllama_lib_last_error(ctx)
    return empty(panicked) ? dasllama_lib_reason(ctx) : panicked
}

def private transcribe_file(ctx : void?; model, audio : string) : int {
    if (!dasllama_lib_asr_open(ctx, model)) {
        print("asr_open failed: {why(ctx)}\n")
        return 1
    }
    let text = dasllama_lib_asr_transcribe(ctx, audio)
    if (empty(text)) {
        print("transcribe failed: {why(ctx)}\n")
        dasllama_lib_asr_close(ctx)
        return 1
    }
    print("{text}\n\n[{dasllama_lib_asr_speed(ctx)}x realtime]\n")
    dasllama_lib_asr_close(ctx)
    return 0
}

def private continue_prompt(ctx : void?; model, prompt : string; want : int) : int {
    if (!dasllama_lib_open(ctx, model)) {
        print("open failed: {why(ctx)}\n")
        return 1
    }
    print("{dasllama_lib_arch(ctx)}: {dasllama_lib_n_layers(ctx)} layers, ")
    print("vocab {dasllama_lib_n_vocab(ctx)}, context {dasllama_lib_context_size(ctx)}\n")
    let n_prompt = dasllama_lib_prefill(ctx, prompt)
    if (n_prompt < 0) {
        print("prefill failed: {why(ctx)}\n")
        dasllama_lib_close(ctx)
        return 1
    }
    print("{prompt}")
    var got = 0
    while (got < want) {
        // the piece points into the library instance's string heap, so it is printed before the
        // next call on that instance
        let piece = dasllama_lib_next_piece(ctx)
        break if (empty(piece))
        print("{piece}")
        got++
    }
    print("\n\n[prompt {n_prompt} tok | gen {got} tok | ")
    print("prefill {int(dasllama_lib_prefill_tps(ctx))} t/s | gen {int(dasllama_lib_gen_tps(ctx))} t/s]\n")
    dasllama_lib_close(ctx)
    return 0
}

[export]
def main() : int {
    var inscope r <- parse_args(type<HostArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<HostArgs>), "main")
        return 1
    }
    var inscope cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.model)) {
        print_help(get_command_info(type<HostArgs>), "main")
        return cfg.help ? 0 : 1
    }
    var ctx = dasllama_lib_create()
    if (ctx == null) {
        print("create failed: {dasllama_lib_last_error(null)}\n")
        return 1
    }
    let rc = empty(cfg.audio) ? continue_prompt(ctx, cfg.model, cfg.prompt, cfg.tokens) : transcribe_file(ctx, cfg.model, cfg.audio)
    dasllama_lib_destroy(ctx)
    dasllama_lib_shutdown_runtime()
    return rc
}
