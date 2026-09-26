options gen2

module cli_args shared

//! dasllama-cli's command line as a plan: the subcommand, the flags every subcommand shares, the
//! sampler, and each subcommand's own flags - parsed from the argv slice after the subcommand
//! and checked before anything loads. The config file (`dasllama-server.toml` beside the exe
//! or in the cwd, or `--config`) fills what the flags left empty, so a box the server's setup
//! page configured answers `dasllama-cli chat` with no flags at all.

require daslib/clargs
require daslib/enum_trait   // to_enum: the subcommand word
require daslib/toml
require daslib/json_boost
require daslib/fio
require daslib/strings_boost   // join
require strings

enum CliCommand {
    none
    complete
    chat
    transcribe
    speak
    talk
    embed
    tokenize
    bench
}

let CLI_COMMANDS : array<string> <- ["complete", "chat", "transcribe", "speak", "talk", "embed", "tokenize", "bench"]

let CLI_USAGE_LINES : array<string> <- [
    "usage: dasllama-cli <command> [flags]",
    "",
    "  complete     complete a prompt (optionally about an image)",
    "  chat         talk to a model, turn by turn, from the terminal or a script",
    "  transcribe   audio file to text with an ASR model",
    "  speak        text to a WAV file with a TTS model",
    "  talk         audio or text in, the model's reply spoken to a WAV file",
    "  embed        a text's embedding vector",
    "  tokenize     a text's token ids and pieces",
    "  bench        the llama-bench rows (pp512 / tg128, tg128@N) on a model",
    "",
    "dasllama-cli <command> --help lists that command's flags.",
    "A dasllama-server.toml beside the program or in the current directory supplies the model,",
    "the towers, the speech models and the backend when the flags do not."
]

def cli_usage() : string => "{join(CLI_USAGE_LINES, "\n")}\n"

def command_of(name : string) : CliCommand => to_enum(type<CliCommand>, name, CliCommand.none)

//! The flags every subcommand takes; the config keys of the same names fill the empty ones.
[CommandLineArgs]
struct CommonArgs {
    @clarg_short = "c"
    @clarg_doc = "TOML config file (default: dasllama-server.toml beside the program, else in the cwd); keys mirror the long flag names, explicit flags win"
    config : string

    @clarg_short = "m"
    @clarg_doc = "The LLM GGUF: a path, or a file name under --models-dir"
    model : string

    @clarg_short = "q"
    @clarg_doc = "Weight serving mode: fp32 | q8 (default; file-format spellings serve natively under q8) | q4 = legacy requant"
    quant : string

    @clarg_doc = "KV cache codec: f32 | f16 | q8_0 | tq4 (default f16)"
    kv_dtype : string

    @clarg_doc = "GPU backend: auto (default: metal/vulkan when detected, else CPU) | off | metal | metal-required | vulkan"
    gpu : string

    @clarg_doc = "Metal serving mode when --gpu is unset: off | auto | required"
    metal : string

    @clarg_doc = "vulkan: resident MoE expert-stack layers K, offloaded from the end (default auto under --gpu vulkan)"
    gpu_layers : int

    @clarg_doc = "vulkan: streamed prefill MoE layers below the resident set (default auto under --gpu vulkan)"
    gpu_stream : int

    @clarg_doc = "vulkan: deltanet (recurrent) layers through the device chain (default on under --gpu vulkan)"
    gpu_dn : bool

    @clarg_doc = "vulkan: full-attention layers through the device chain (default on under --gpu vulkan)"
    gpu_attn : bool

    @clarg_doc = "vulkan: dense attention-side planes resident (default off)"
    gpu_dense : bool

    @clarg_doc = "vulkan: resident-weight VRAM cap override in MB (default: query the device)"
    gpu_vram_mb : int

    @clarg_doc = "Context-length cap in tokens (default: the model's trained context_length)"
    ctx : int

    @clarg_short = "t"
    @clarg_doc = "Worker-lane cap for the matmul dispatch (default 16; -1 = all cores)"
    threads : int

    @clarg_doc = "Where a bare model name resolves (default ~/.dasllama/models; DASLLAMA_MODELS_DIR overrides)"
    models_dir : string

    @clarg_doc = "Re-tune this box's dasLLAMA kernels, then relaunch (a fat build carries no tuner and ignores it)"
    tune : bool

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! The sampler; a knob left unset (-1) takes the command's default - greedy for `complete`,
//! the chat preset (temp 0.7, top-k 20, top-p 0.95) for `chat` and `talk`, where greedy decoding
//! lets a thinking model loop on its own draft.
[CommandLineArgs]
struct SamplerArgs {
    @clarg_doc = "Sampling temperature, 0 = greedy (default: 0 for complete, 0.7 for chat and talk)"
    temp : float = -1.0

    @clarg_doc = "Top-k cutoff, 0 = off (default: off for complete, 20 for chat and talk)"
    top_k : int = -1

    @clarg_doc = "Top-p (nucleus) cutoff, keeps the smallest prob mass >= p, 1 = off (default: off for complete, 0.95 for chat and talk)"
    top_p : float = -1.0

    @clarg_doc = "Min-p cutoff relative to the max probability (default 0 = off)"
    min_p : float

    @clarg_doc = "Multiplicative repetition penalty over recent tokens (default 1 = off)"
    repeat_penalty : float = 1.0

    @clarg_doc = "Presence penalty: flat logit subtraction for recent tokens (default 0)"
    presence_penalty : float

    @clarg_doc = "Frequency penalty: per-occurrence logit subtraction (default 0)"
    frequency_penalty : float

    @clarg_short = "s"
    @clarg_doc = "Sampling seed, 0 = the session's fixed default (default 0)"
    seed : int

    @clarg_short = "n"
    @clarg_doc = "Max tokens per reply (default: 256 for complete; 16384 for chat and talk, the server's default - a thinking model spends the budget's head on its reasoning; the context caps it)"
    max_tokens : int
}

//! The sampler knobs resolved for a command: a knob left unset (-1) takes the command's default -
//! greedy for a completion, the chat preset for a conversation.
struct SamplerPick {
    temp : float
    top_k : int
    top_p : float
}

def resolve_sampler(s : SamplerArgs; chatty : bool) : SamplerPick
    => SamplerPick(temp = s.temp >= 0.0 ? s.temp : (chatty ? 0.7 : 0.0),
                   top_k = s.top_k >= 0 ? s.top_k : (chatty ? 20 : 0),
                   top_p = s.top_p >= 0.0 ? s.top_p : (chatty ? 0.95 : 1.0))

[CommandLineArgs]
struct CompleteArgs {
    @clarg_short = "p"
    @clarg_doc = "The prompt (or --file, or the first bare argument)"
    prompt : string

    @clarg_short = "f"
    @clarg_doc = "Read the prompt from this file"
    file : string

    @clarg_path
    @clarg_doc = "Complete about an image: the prompt becomes a chat turn carrying it (needs --image-mmproj)"
    image : string

    @clarg_doc = "The vision mmproj GGUF; required with --image"
    image_mmproj : string

    @clarg_doc = "Print the reply only - no token counts or rates after it"
    quiet : bool
}

[CommandLineArgs]
struct ChatArgs {
    @clarg_doc = "The system prompt"
    system : string

    @clarg_doc = "Read the turns from this file instead of the terminal, one message per line (slash commands work); the reply follows each"
    script : string

    @clarg_doc = "The vision mmproj GGUF: arms /image"
    image_mmproj : string

    @clarg_doc = "The audio mmproj GGUF: arms /audio (the E-series mmproj serves both, pass the same file twice)"
    audio_mmproj : string

    @clarg_doc = "A TTS model GGUF: /speak on writes every reply to a WAV file"
    tts : string

    @clarg_short = "v"
    @clarg_doc = "Voice name for --tts (default: the model's first voice)"
    voice : string

    @clarg_doc = "Speech speed multiplier for --tts (default 1.0)"
    speed : float

    @clarg_doc = "Where the spoken replies land, reply_<n>.wav each (default: the cwd)"
    out_dir : string

    @clarg_doc = "Hide a thinking model's reasoning (default: it streams before the answer, a blank line between)"
    hide_thinking : bool

    @clarg_doc = "Answer directly on a hybrid thinking model (default: the model's own default)"
    no_think : bool

    @clarg_doc = "Print the reply only - no token counts or rates after it"
    quiet : bool
}

[CommandLineArgs]
struct TranscribeArgs {
    @clarg_short = "a"
    @clarg_doc = "The ASR model (GGUF or .bin): a path, or a file name under --models-dir"
    asr : string

    @clarg_doc = "mmproj for a GGUF ASR model (paired with --asr)"
    mmproj : string

    @clarg_short = "f"
    @clarg_doc = "The audio file to transcribe (wav / mp3 / flac / ogg; or the first bare argument)"
    file : string

    @clarg_short = "o"
    @clarg_doc = "Write the transcript here instead of stdout"
    out : string

    @clarg_short = "l"
    @clarg_doc = "Language hint (default: auto for a model that detects it, else en)"
    lang : string
}

[CommandLineArgs]
struct SpeakArgs {
    @clarg_doc = "The TTS model GGUF: a path, or a file name under --models-dir"
    tts : string

    @clarg_doc = "The text to speak (or --file, or the first bare argument)"
    text : string

    @clarg_short = "f"
    @clarg_doc = "Read the text from this file"
    file : string

    @clarg_short = "o"
    @clarg_doc = "The WAV file to write (16-bit PCM, the model's rate; default out.wav)"
    out : string

    @clarg_short = "v"
    @clarg_doc = "Voice name or alias (default: the model's first voice)"
    voice : string

    @clarg_doc = "Speech speed multiplier (default 1.0)"
    speed : float

    @clarg_doc = "TTS weight lane: q8 (default; the prepared quant image beside the GGUF) | f32 (the file's own planes)"
    tts_lane : string

    @clarg_doc = "Print the generator's per-op profile"
    prof : bool
}

[CommandLineArgs]
struct TalkArgs {
    @clarg_short = "a"
    @clarg_doc = "The ASR model (GGUF or .bin) that hears --in"
    asr : string

    @clarg_doc = "mmproj for a GGUF ASR model (paired with --asr)"
    mmproj : string

    @clarg_short = "i"
    @clarg_name = "in"
    @clarg_doc = "The audio file to answer (wav / mp3 / flac / ogg; or the first bare argument)"
    in_file : string

    @clarg_short = "p"
    @clarg_doc = "A text prompt to answer instead of --in"
    prompt : string

    @clarg_doc = "The system prompt"
    system : string

    @clarg_short = "l"
    @clarg_doc = "Language hint for the ASR model (default: auto for a model that detects it, else en)"
    lang : string

    @clarg_doc = "The TTS model GGUF that speaks the reply"
    tts : string

    @clarg_short = "v"
    @clarg_doc = "Voice name or alias (default: the model's first voice)"
    voice : string

    @clarg_doc = "Speech speed multiplier (default 1.0)"
    speed : float

    @clarg_short = "o"
    @clarg_doc = "The WAV file the reply is spoken to (default reply.wav)"
    out : string

    @clarg_doc = "Answer directly on a hybrid thinking model (default: the model's own default)"
    no_think : bool

    @clarg_doc = "Print the reply only - no transcript, counts or rates"
    quiet : bool
}

[CommandLineArgs]
struct EmbedArgs {
    @clarg_doc = "The text to embed (or --file, or the first bare argument)"
    text : string

    @clarg_short = "f"
    @clarg_doc = "Read the text from this file"
    file : string

    @clarg_doc = "Print the vector as a JSON array (default: one float per line)"
    json : bool
}

[CommandLineArgs]
struct TokenizeArgs {
    @clarg_doc = "The text to tokenize (or --file, or the first bare argument)"
    text : string

    @clarg_short = "f"
    @clarg_doc = "Read the text from this file"
    file : string

    @clarg_doc = "Print the ids only, space-separated (default: one `id<TAB>piece` per line)"
    ids : bool

    @clarg_doc = "Do not add the model's BOS / leading specials"
    no_special : bool
}

[CommandLineArgs]
struct BenchArgs {
    @clarg_short = "p"
    @clarg_doc = "Prompt tokens per pp rep (default 512, llama-bench -p; 0 skips the row)"
    plen : int = 512

    @clarg_short = "n"
    @clarg_doc = "Generated tokens per tg rep (default 128, llama-bench -n; 0 skips the row)"
    ngen : int = 128

    @clarg_short = "r"
    @clarg_doc = "Timed repetitions per row after an untimed warmup (default 5, llama-bench -r)"
    reps : int = 5

    @clarg_doc = "Streams of the batched tg row, tg<n>@<npl>: that many streams through the scheduler, the summed rate (default 0 = no batched row)"
    npl : int

    @clarg_short = "o"
    @clarg_doc = "Output: txt (default) | md (a llama-bench-style table)"
    output : string
}

//! The parsed command line: which subcommand, the shared flags, the sampler, the subcommand's
//! own flags, the bare arguments after the flags, and the refusal when it did not parse.
struct CliPlan {
    cmd : CliCommand
    common : CommonArgs
    sampler : SamplerArgs = SamplerArgs()
    complete : CompleteArgs
    chat : ChatArgs
    transcribe : TranscribeArgs
    speak : SpeakArgs
    talk : TalkArgs
    embed : EmbedArgs
    tokenize : TokenizeArgs
    bench : BenchArgs = BenchArgs()
    rest : array<string>     // the subcommand's argv slice, verbatim
    bare : array<string>     // the non-flag tokens of that slice, in order
    help : bool
    error : string
}

//! One struct parsed from the subcommand's slice; false with the parser's reason.
def private parse_into(var target : auto(TT); args : array<string>; var err : string&) : bool {
    var r <- parse_args(type<TT -const -&>, args)
    if (r |> is_err) {
        err = r |> unwrap_err
        return false
    }
    target <- r |> move_unwrap
    return true
}

//! The tokens of `args` that are neither flags nor a value-flag's value, in order: the prompt,
//! the file, the text a subcommand accepts bare. clargs' own walk, over the structs' flags merged.
def bare_tokens(args : array<string>; infos : array<CommandInfo>) : array<string> {
    var merged = CommandInfo()
    for (info in infos) {
        merged.args |> push_clone_from(info.args)   // the info holds arrays: a clone per entry
    }
    return <- collect_positional_args(args, merged)
}

def private command_infos(cmd : CliCommand) : array<CommandInfo> {
    var infos : array<CommandInfo>
    infos |> emplace(get_command_info(type<CommonArgs>))
    if (cmd == CliCommand.complete || cmd == CliCommand.chat || cmd == CliCommand.talk) {
        infos |> emplace(get_command_info(type<SamplerArgs>))
    }
    if (cmd == CliCommand.complete) {
        infos |> emplace(get_command_info(type<CompleteArgs>))
    } elif (cmd == CliCommand.chat) {
        infos |> emplace(get_command_info(type<ChatArgs>))
    } elif (cmd == CliCommand.transcribe) {
        infos |> emplace(get_command_info(type<TranscribeArgs>))
    } elif (cmd == CliCommand.speak) {
        infos |> emplace(get_command_info(type<SpeakArgs>))
    } elif (cmd == CliCommand.talk) {
        infos |> emplace(get_command_info(type<TalkArgs>))
    } elif (cmd == CliCommand.embed) {
        infos |> emplace(get_command_info(type<EmbedArgs>))
    } elif (cmd == CliCommand.tokenize) {
        infos |> emplace(get_command_info(type<TokenizeArgs>))
    } elif (cmd == CliCommand.bench) {
        infos |> emplace(get_command_info(type<BenchArgs>))
    }
    return <- infos
}

//! The help for one subcommand: its own flags, the sampler where it samples, then the shared flags.
def command_help(cmd : CliCommand) : string {
    let name = "dasllama-cli {cmd}"
    var text = ""
    if (cmd == CliCommand.complete) {
        text = format_help(get_command_info(type<CompleteArgs>), name)
    } elif (cmd == CliCommand.chat) {
        text = format_help(get_command_info(type<ChatArgs>), name)
    } elif (cmd == CliCommand.transcribe) {
        text = format_help(get_command_info(type<TranscribeArgs>), name)
    } elif (cmd == CliCommand.speak) {
        text = format_help(get_command_info(type<SpeakArgs>), name)
    } elif (cmd == CliCommand.talk) {
        text = format_help(get_command_info(type<TalkArgs>), name)
    } elif (cmd == CliCommand.embed) {
        text = format_help(get_command_info(type<EmbedArgs>), name)
    } elif (cmd == CliCommand.tokenize) {
        text = format_help(get_command_info(type<TokenizeArgs>), name)
    } elif (cmd == CliCommand.bench) {
        text = format_help(get_command_info(type<BenchArgs>), name)
    }
    if (cmd == CliCommand.complete || cmd == CliCommand.chat || cmd == CliCommand.talk) {
        text = "{text}\nsampling:\n{format_help(get_command_info(type<SamplerArgs>), name)}"
    }
    return "{text}\nshared flags:\n{format_help(get_command_info(type<CommonArgs>), name)}"
}

//! `argv` is the program's own arguments (after the daslang `--` under the JIT): the subcommand
//! first, its flags after. Nothing loads here; a plan whose `error` is non-empty is refused
//! with that text, `help` asks for the usage or the subcommand's help.
def parse_cli(argv : array<string>) : CliPlan {
    var plan = CliPlan()
    if (empty(argv)) {
        plan.help = true
        return <- plan
    }
    let first = argv[0]
    if (first == "--help" || first == "-?" || first == "help" || first == "--show-help") {
        plan.help = true
        return <- plan
    }
    plan.cmd = command_of(first)
    if (plan.cmd == CliCommand.none) {
        plan.error = "unknown command '{first}' (one of {join(CLI_COMMANDS, " | ")})"
        return <- plan
    }
    plan.rest <- [for (i in range(1, length(argv))); argv[i]]
    for (a in plan.rest) {   // `<command> --help` in every spelling: the shipped exe owns argv, and clargs would drop an unknown --help
        if (a == "--help" || a == "-h" || a == "-?" || a == "--show-help") {
            plan.help = true
            return <- plan
        }
    }
    var err = ""
    if (!parse_into(plan.common, plan.rest, err)) {
        plan.error = err
        return <- plan
    }
    if (plan.common.help) {
        plan.help = true
        return <- plan
    }
    var ok = true
    if (plan.cmd == CliCommand.complete || plan.cmd == CliCommand.chat || plan.cmd == CliCommand.talk) {
        ok = parse_into(plan.sampler, plan.rest, err)
    }
    if (ok) {
        if (plan.cmd == CliCommand.complete) {
            ok = parse_into(plan.complete, plan.rest, err)
        } elif (plan.cmd == CliCommand.chat) {
            ok = parse_into(plan.chat, plan.rest, err)
        } elif (plan.cmd == CliCommand.transcribe) {
            ok = parse_into(plan.transcribe, plan.rest, err)
        } elif (plan.cmd == CliCommand.speak) {
            ok = parse_into(plan.speak, plan.rest, err)
        } elif (plan.cmd == CliCommand.talk) {
            ok = parse_into(plan.talk, plan.rest, err)
        } elif (plan.cmd == CliCommand.embed) {
            ok = parse_into(plan.embed, plan.rest, err)
        } elif (plan.cmd == CliCommand.tokenize) {
            ok = parse_into(plan.tokenize, plan.rest, err)
        } elif (plan.cmd == CliCommand.bench) {
            ok = parse_into(plan.bench, plan.rest, err)
        }
    }
    if (!ok) {
        plan.error = err
        return <- plan
    }
    let infos <- command_infos(plan.cmd)
    plan.bare <- bare_tokens(plan.rest, infos)
    return <- plan
}

// ===== The config file =====

//! What `dasllama-server.toml` supplies when a flag is empty: the same keys the server reads,
//! plus the default `[[models]]` roster entry where the file carries a roster.
struct CliConfig {
    path : string            // "" when no file was found
    model : string
    image_mmproj : string
    asr : string
    mmproj : string
    tts : string
    tts_lane : string
    tts_voices_dir : string
    quant : string
    kv_dtype : string
    gpu : string
    metal : string
    gpu_layers : int
    gpu_layers_set : bool
    gpu_stream : int
    gpu_stream_set : bool
    gpu_dn : bool
    gpu_dn_set : bool
    gpu_attn : bool
    gpu_attn_set : bool
    gpu_dense : bool
    gpu_vram_mb : int
    ctx : int
    threads : int
    threads_set : bool
    models_dir : string
    error : string           // the read or parse failure of an explicit --config
}

//! The config a program reads with no `--config`: `dasllama-server.toml` in the cwd, else beside
//! the program (`program_dir`); "" when neither exists. The one lookup both programs share.
def discover_config(program_dir : string) : string {
    let here = "dasllama-server.toml"
    if (stat(here).is_valid) {
        return here
    }
    let beside = path_join(program_dir, "dasllama-server.toml")
    return stat(beside).is_valid ? beside : ""
}

//! The roster's default entry (`default = true`, else the first): its path and per-model
//! overrides land on the config where the flat keys left them empty.
def private apply_roster(root : JsonValue?; var cfg : CliConfig) {
    let mv = root?["models"]
    if (mv == null || !(mv.value is _array)) {
        return
    }
    var pick_at = -1
    var idx = 0
    for (e in mv.value as _array) {
        if (pick_at < 0 || (e?["default"] ?? false)) {
            pick_at = idx
        }
        break if (e?["default"] ?? false)
        idx++
    }
    if (pick_at < 0) {
        return
    }
    let pick = (mv.value as _array)[pick_at]
    if (empty(cfg.model)) {
        cfg.model = pick?["path"] ?? ""
    }
    if (empty(cfg.image_mmproj)) {
        cfg.image_mmproj = pick?["image_mmproj"] ?? ""
    }
    if (empty(cfg.quant)) {
        cfg.quant = pick?["quant"] ?? ""
    }
    if (empty(cfg.kv_dtype)) {
        cfg.kv_dtype = pick?["kv_dtype"] ?? ""
    }
    if (cfg.ctx <= 0) {
        cfg.ctx = int(pick?["ctx"] ?? 0l)
    }
}

//! Parse a config file's text into the keys the CLI understands; unknown keys are ignored.
def parse_cli_config(body : string; var cfg : CliConfig) : bool {
    var terr = ""
    var root = read_toml(body, terr)
    if (root == null) {
        cfg.error = "bad TOML: {terr}"
        return false
    }
    cfg.model = root?["model"] ?? ""
    cfg.image_mmproj = root?["image_mmproj"] ?? ""
    cfg.asr = root?["asr"] ?? ""
    cfg.mmproj = root?["mmproj"] ?? ""
    cfg.tts = root?["tts"] ?? ""
    cfg.tts_lane = root?["tts_lane"] ?? ""
    cfg.tts_voices_dir = root?["tts_voices_dir"] ?? ""
    cfg.quant = root?["quant"] ?? ""
    cfg.kv_dtype = root?["kv_dtype"] ?? ""
    cfg.gpu = root?["gpu"] ?? ""
    cfg.metal = root?["metal"] ?? ""
    cfg.gpu_layers_set = key_exists(root, "gpu_layers")
    cfg.gpu_layers = int(root?["gpu_layers"] ?? 0l)
    cfg.gpu_stream_set = key_exists(root, "gpu_stream")
    cfg.gpu_stream = int(root?["gpu_stream"] ?? 0l)
    cfg.gpu_dn_set = key_exists(root, "gpu_dn")
    cfg.gpu_dn = root?["gpu_dn"] ?? false
    cfg.gpu_attn_set = key_exists(root, "gpu_attn")
    cfg.gpu_attn = root?["gpu_attn"] ?? false
    cfg.gpu_dense = root?["gpu_dense"] ?? false
    cfg.gpu_vram_mb = int(root?["gpu_vram_mb"] ?? 0l)
    cfg.ctx = int(root?["ctx"] ?? 0l)
    cfg.threads_set = key_exists(root, "threads")
    cfg.threads = int(root?["threads"] ?? 0l)
    cfg.models_dir = root?["models_dir"] ?? ""
    apply_roster(root, cfg)
    unsafe {
        delete root
    }
    return true
}

//! Read the config the plan names, or the discovered one; a missing explicit `--config` is the
//! error, a missing discovered file is no config at all.
def load_cli_config(named : string; program_dir : string) : CliConfig {
    var cfg : CliConfig
    cfg.path = !empty(named) ? named : discover_config(program_dir)
    if (empty(cfg.path)) {
        return cfg
    }
    let body = fread(cfg.path)
    if (empty(body)) {
        cfg.error = "cannot read config '{cfg.path}'"
        return cfg
    }
    parse_cli_config(body, cfg)
    return cfg
}

//! The config's values land on every shared flag the command line left empty.
def fill_common(var c : CommonArgs; cfg : CliConfig) {
    if (empty(c.model)) {
        c.model = cfg.model
    }
    if (empty(c.quant)) {
        c.quant = cfg.quant
    }
    if (empty(c.kv_dtype)) {
        c.kv_dtype = cfg.kv_dtype
    }
    if (empty(c.gpu)) {
        c.gpu = cfg.gpu
    }
    if (empty(c.metal)) {
        c.metal = cfg.metal
    }
    if (c.ctx <= 0) {
        c.ctx = cfg.ctx
    }
    if (c.threads == 0 && cfg.threads_set) {
        c.threads = cfg.threads
    }
    if (empty(c.models_dir)) {
        c.models_dir = cfg.models_dir
    }
}

//! A model spelling to the file it names: a path as given when it exists, else a bare name
//! under `models_dir`; "" when neither exists.
def resolve_model_path(spec : string; models_dir : string) : string {
    if (empty(spec)) {
        return ""
    }
    if (stat(spec).is_valid) {
        return spec
    }
    if (find(spec, "/") < 0 && find(spec, "\\") < 0 && !empty(models_dir)) {
        let under = path_join(models_dir, spec)
        if (stat(under).is_valid) {
            return under
        }
    }
    return ""
}

//! What a subcommand refuses before any model loads - a missing input, a knob pair that cannot
//! both hold, a spelling that is not one of the choices - as the refusal text, "" when the plan
//! holds. The config has already filled the flags it fills. File existence and model loading
//! stay with the run: they need the rig.
def validate_plan(plan : CliPlan; cfg : CliConfig) : string {   // nolint:STYLE037 - one flat ladder, an arm per command
    var err = ""
    if (plan.cmd == CliCommand.complete) {
        let prompt = text_input(plan.complete.prompt, plan.complete.file, plan.bare, err)
        return err if (!empty(err))
        return "nothing to complete - pass --prompt, --file, or the prompt as the first argument" if (empty(prompt))
        let image_spec = !empty(plan.complete.image_mmproj) ? plan.complete.image_mmproj : cfg.image_mmproj
        return "--image needs --image-mmproj <vision mmproj gguf>" if (!empty(plan.complete.image) && empty(image_spec))
    } elif (plan.cmd == CliCommand.chat) {
        if (!empty(plan.chat.script) && empty(fread(plan.chat.script))) {
            return "could not read --script {plan.chat.script}"
        }
    } elif (plan.cmd == CliCommand.transcribe) {
        let file = !empty(plan.transcribe.file) ? plan.transcribe.file : (empty(plan.bare) ? "" : plan.bare[0])
        return "nothing to transcribe - pass --file <audio> or the file as the first argument" if (empty(file))
        return "no ASR model - pass --asr <gguf|bin> or configure one" if (empty(plan.transcribe.asr) && empty(cfg.asr))
    } elif (plan.cmd == CliCommand.speak) {
        let text = text_input(plan.speak.text, plan.speak.file, plan.bare, err)
        return err if (!empty(err))
        return "nothing to say - pass --text, --file, or the text as the first argument" if (empty(text))
        return "no TTS model - pass --tts <gguf> or configure one" if (empty(plan.speak.tts) && empty(cfg.tts))
    } elif (plan.cmd == CliCommand.talk) {
        let in_file = !empty(plan.talk.in_file) ? plan.talk.in_file : (empty(plan.bare) ? "" : plan.bare[0])
        return "nothing to answer - pass --in <audio> or --prompt <text>" if (empty(in_file) && empty(plan.talk.prompt))
        return "no ASR model to hear --in - pass --asr <gguf|bin> or configure one" if (!empty(in_file) && empty(plan.talk.asr) && empty(cfg.asr))
        return "no TTS model to speak the reply - pass --tts <gguf> or configure one" if (empty(plan.talk.tts) && empty(cfg.tts))
    } elif (plan.cmd == CliCommand.embed) {
        let text = text_input(plan.embed.text, plan.embed.file, plan.bare, err)
        return err if (!empty(err))
        return "nothing to embed - pass --text, --file, or the text as the first argument" if (empty(text))
    } elif (plan.cmd == CliCommand.tokenize) {
        let text = text_input(plan.tokenize.text, plan.tokenize.file, plan.bare, err)
        return err if (!empty(err))
        return "nothing to tokenize - pass --text, --file, or the text as the first argument" if (empty(text))
    } elif (plan.cmd == CliCommand.bench) {
        let fmt = to_lower(plan.bench.output)
        return "--output is txt | md" if (!empty(fmt) && fmt != "txt" && fmt != "md")
        return "nothing to measure - -p and -n are both 0" if (plan.bench.plen <= 0 && plan.bench.ngen <= 0)
    }
    return ""
}

//! The text a subcommand reads: the flag, else the file, else the first bare token; "" when none.
def text_input(flag, file : string; bare : array<string>; var err : string&) : string {
    if (!empty(flag)) {
        return flag
    }
    if (!empty(file)) {
        let body = fread(file)
        if (empty(body)) {
            err = "could not read {file}"
        }
        return body
    }
    return empty(bare) ? "" : bare[0]
}
