options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// dasllama-cli — the shell front end of dasLLAMA, a sibling of dasllama-server on the same
// facade: complete, chat, transcribe, speak, talk (audio in, spoken reply out), embed,
// tokenize, and the llama-bench rows. It shares this box's tune winners with the server: NO
// scope declaration of its own — requiring dasLLAMA pulls in the [tune_scope]. The policy is
// "auto" (like the server): the first run on an untuned box tunes and relaunches; --tune forces
// a re-tune; a fat build carries no tuner and ignores both.
//
//   bin/daslang -jit utils/dasllama-server/cli.das -- chat --model <model.gguf>
//   bin/daslang -jit utils/dasllama-server/cli.das -- talk --asr <asr.gguf> --model <llm.gguf> --tts <tts.gguf> --in q.wav --out a.wav

require dasllama/dasllama       // the public facade — everything this tool touches
require daslib/tune             // [tune_policy] + log_tune_status - the shells; the framework rides in when the build has dasLLVM
require jobque                  // set_jobque_threads_cap
require daslib/jobque_boost
require daslib/clargs           // get_user_args
require daslib/module_path      // get_this_module_dir: the config beside the program
require daslib/logger           // logger_init: the engine's to_log / print land in logs/dasllama-cli.log, off stdout
require daslib/rtti             // this_context().last_exception: an uncaught panic's text, printed for the user
require daslib/defer
require daslib/fio
require daslib/json_boost       // embed --json
require daslib/strings_boost    // join
require strings
require math
require cli_args                // the plan: subcommand, flags, config file
require cli_repl                // the chat loop's logic
require serving_knobs           // the spellings shared with the server + the --gpu pick
require model_catalog           // catalog_models_dir
require dasllama/dasllama_bench // the llama-bench rows - the same rows dasllama-bench and the server's in-process bench run

let TOOL = "dasllama-cli"

//! A refusal: on stderr, exit 1. Written straight to the stream - the logger hook below diverts
//! `to_log` into the log file, and a refusal is for the user's eyes.
def private fail(why : string) : int {
    note(why)
    return 1
}

def private emit(text : string) {
    fprint(fstdout(), text)
    fflush(fstdout())
}

//! Progress and counters go to stderr, so stdout carries the answer alone.
def private note(text : string) {
    fprint(fstderr(), "{TOOL}: {text}\n")
    fflush(fstderr())
}

// ===== The engine under every subcommand =====

def private gpu_knobs_of(c : CommonArgs; cfg : CliConfig) : GpuKnobs {
    // an explicit flag sets a Vulkan detail knob; else the config's key, when it carries one
    let layers_cli = c.gpu_layers != 0
    let stream_cli = c.gpu_stream != 0
    return GpuKnobs(gpu = c.gpu, metal = c.metal,
                    layers = layers_cli ? c.gpu_layers : cfg.gpu_layers, layers_set = layers_cli || cfg.gpu_layers_set,
                    stream = stream_cli ? c.gpu_stream : cfg.gpu_stream, stream_set = stream_cli || cfg.gpu_stream_set,
                    dn = c.gpu_dn || (cfg.gpu_dn_set && cfg.gpu_dn), dn_set = c.gpu_dn || cfg.gpu_dn_set,
                    attn = c.gpu_attn || (cfg.gpu_attn_set && cfg.gpu_attn), attn_set = c.gpu_attn || cfg.gpu_attn_set,
                    dense = c.gpu_dense || cfg.gpu_dense,
                    vram_mb = c.gpu_vram_mb > 0 ? c.gpu_vram_mb : cfg.gpu_vram_mb)
}

//! What one run serves on: the lane cap, the backend, the context cap - landed before any load.
struct private Rig {
    gpu : string          // effective_gpu's answer
    vulkan_armed : bool
    ctx : int64
    quant : QuantMode
    kv : KVDtype
    models_dir : string
    max_new : int64
}

def private arm_rig(c : CommonArgs; cfg : CliConfig; sampler : SamplerArgs; default_new : int64) : Rig {
    let threads = c.threads > 0 ? c.threads : (c.threads < 0 ? 0 : 16)
    set_dispatch_worker_limit(threads)
    set_jobque_threads_cap(threads > 0 ? max(1, threads - 1) : 0)
    let knobs = gpu_knobs_of(c, cfg)
    let gpu = effective_gpu(knobs, TOOL)
    if (gpu_pick_is_auto(knobs)) {
        note("gpu backend auto-detected: {gpu} (pass --gpu off | metal | vulkan to pin)")
    }
    let armed = apply_gpu_backend(knobs, gpu, TOOL)
    let ctx = int64(max(c.ctx, 0))
    if (armed) {
        set_gpu_resident_regions(1l)   // one conversation at a time: one device-home region
        set_gpu_ctx_max(ctx)
    }
    return Rig(gpu = gpu, vulkan_armed = armed, ctx = ctx,
               quant = pick_quant(c.quant, TOOL), kv = pick_kv_dtype(c.kv_dtype, TOOL),
               models_dir = catalog_models_dir(c.models_dir),
               max_new = sampler.max_tokens > 0 ? int64(sampler.max_tokens) : default_new)
}

//! The LLM the plan names, loaded under the rig; "" in `why` when it loaded.
def private load_llm(rig : Rig; spec : string; var why : string&) : Model {
    let path = resolve_model_path(spec, rig.models_dir)
    if (empty(path)) {
        why = (empty(spec) ? "no model - pass --model <gguf> or configure one in dasllama-server.toml"
                           : "model not found: {spec} (a bare name resolves under {rig.models_dir})")
        return <- Model()
    }
    note("loading {path}")
    var m <- load_model(path, rig.quant)
    if (rig.ctx > 0l) {
        m.config.seq_len = min(m.config.seq_len, rig.ctx)
    }
    if (rig.vulkan_armed) {
        let decline = gpu_resident_decline()
        note(empty(decline) ? "served whole from the GPU, the cache with it" : "served from the CPU with GPU rails: {decline}")
    }
    return <- m
}

//! A tower or pair file the plan names - `--mmproj`, `--image-mmproj`, `--audio-mmproj` - resolved
//! like a model: a path as given, else a bare name under the models dir; "" with `why` when absent.
def private resolve_aux(rig : Rig; spec, what : string; var why : string&) : string {
    if (empty(spec)) {
        return ""
    }
    let path = resolve_model_path(spec, rig.models_dir)
    if (empty(path)) {
        why = "{what} not found: {spec} (a bare name resolves under {rig.models_dir})"
    }
    return path
}

//! A device-home cache where the whole-model driver serves the model, else the host cache.
def private home_session(m : Model; rig : Rig) : Session {
    if (gpu_device_sessions() > 0l) {
        return <- create_device_session(m, gpu_device_session_dtype())
    }
    return <- create_session(m, rig.kv)
}

//! The sampler with the command's defaults where a knob was left unset: greedy for a completion,
//! the chat preset for a conversation.
def private sampling_of(s : SamplerArgs; chatty : bool) : SamplingParams {
    let pick = resolve_sampler(s, chatty)
    var params = SamplingParams()
    params.temp = pick.temp
    params.top_k = int64(pick.top_k)
    params.top_p = pick.top_p
    let seed = s.seed == 0 ? "fixed" : "{s.seed}"
    note(pick.temp <= 0.0 ? "sampler: greedy" : "sampler: temp {pick.temp} | top-k {pick.top_k} | top-p {pick.top_p} | min-p {s.min_p} | seed {seed}")
    params.min_p = s.min_p
    params.penalty = s.repeat_penalty
    params.presence_penalty = s.presence_penalty
    params.frequency_penalty = s.frequency_penalty
    return params
}

def private stats_line(st : Stats) : string
    => "prompt {st.n_prompt} tok | gen {st.n_gen} tok | ttft {int(st.ttft_s * 1000.0lf)} ms | prefill {int(st.prefill_tps)} t/s | gen {int(st.gen_tps)} t/s"

def private emit_halves(show_thinking : bool; rsn, cnt : string; var thought : bool&) {
    let text = think_print(show_thinking, rsn, cnt, thought)
    if (!empty(text)) {
        emit(text)
    }
}

//! One reply streamed to stdout: the reasoning half only when asked, the content always;
//! the full reply text comes back for the caller's own bookkeeping.
def private stream_reply(m : Model; var chat : ChatSession; params : SamplingParams; show_thinking : bool) : string {
    var ts = make_think_stream(chat)
    var rsn = ""
    var cnt = ""
    var thought = false
    let reply = respond(m, chat, params) $(piece) {
        think_feed(ts, piece, rsn, cnt)
        emit_halves(show_thinking, rsn, cnt, thought)
        return true
    }
    think_finish(ts, rsn, cnt)
    emit_halves(show_thinking, rsn, cnt, thought)
    emit("\n")
    return reply
}

// ===== Towers, speech =====

//! The vision side of a chat: the CLI owns the embedder and splices the encoded rows onto the
//! turn itself, so the chat session stays free for an audio tower.
struct private Vision {
    emb : VisionEmbedder = VisionEmbedder()
    state : VisionState = VisionState()
    armed : bool
}

def private load_vision(path : string) : Vision {
    var v = Vision()
    if (empty(path)) {
        return <- v
    }
    note("loading vision mmproj {path}")
    v.emb <- load_vision_embedder(path)
    v.state <- VisionState()
    v.armed = true
    return <- v
}

//! Queue `path`'s image on the next turn; false with the reason when it cannot be.
def private queue_image(m : Model; var chat : ChatSession; var v : Vision; path : string; var why : string&) : bool {
    if (!v.armed) {
        why = "no vision mmproj - pass --image-mmproj <gguf>"
        return false
    }
    var inscope img <- load_image_rgb(path)
    if (img.width == 0) {
        why = "could not decode image '{path}'"
        return false
    }
    var rows : array<float>
    let n = encode_image(v.emb, v.state, img, base_name(path), rows)
    add_user_image_rows(m, chat, rows, n, vision_mrope_grid(v.emb, v.state))
    note("image {img.width}x{img.height} -> {n} soft tokens")
    return true
}

//! The hearing side of a chat: the CLI owns the audio encoder and splices the encoded rows onto
//! the turn itself, the way the server's media worker does.
struct private Hearing {
    emb : AudioEmbedder = AudioEmbedder()
    state : AudioState = AudioState()
    armed : bool
}

def private load_hearing(path : string; var why : string&) : Hearing {
    var h = Hearing()
    if (empty(path)) {
        return <- h
    }
    if (audio_probe_proj_dim(path) == 0l) {
        why = "{path} carries no audio tower this build serves (gemma4a)"
        return <- h
    }
    note("loading audio mmproj {path}")
    h.emb <- load_audio_embedder(path)
    h.state <- AudioState()
    h.armed = true
    return <- h
}

//! Queue `path`'s audio on the next turn; false with the reason when it cannot be.
def private queue_audio(m : Model; var chat : ChatSession; var h : Hearing; path : string; var why : string&) : bool {
    if (!h.armed) {
        why = "no audio mmproj - pass --audio-mmproj <gguf>"
        return false
    }
    let samples <- load_audio_16k_mono(path)
    if (empty(samples)) {
        why = "could not decode audio '{path}'"
        return false
    }
    var rows : array<float>
    let n = encode_audio(h.emb, h.state, samples, rows)
    add_user_audio_rows(m, chat, rows, n)
    note("audio {double(length(samples)) / 16000.0lf} s -> {n} soft tokens")
    return true
}

//! The speech model a run speaks through; `armed` false when none was asked for.
struct private Speech {
    model : TtsModel = TtsModel()
    voice : string
    speed : float
    armed : bool
}

def private load_speech(rig : Rig; spec, voice : string; speed : float; var why : string&) : Speech {
    var sp = Speech(speed = speed > 0.0 ? speed : 1.0)
    if (empty(spec)) {
        return <- sp
    }
    let path = resolve_model_path(spec, rig.models_dir)
    if (empty(path)) {
        why = "TTS model not found: {spec}"
        return <- sp
    }
    let missing = tts_missing_file(path)
    if (!empty(missing)) {
        why = "TTS model {base_name(path)} is missing {missing} (the front-end packs sit beside the GGUF)"
        return <- sp
    }
    note("loading TTS {path}")
    sp.model <- load_tts_model(path)
    var inscope c <- caps(sp.model)
    if (empty(voice) && empty(c.voices)) {
        why = "{base_name(path)} offers no voice this front end can drive"
        return <- sp
    }
    sp.voice = empty(voice) ? c.voices[0] : voice
    if (!empty(voice)) {
        var known = true
        try {
            tts_voice_lang(sp.model, voice)   // resolves aliases the way synthesize does; an unknown voice panics
        } recover {
            known = false
        }
        if (!known) {
            why = "'{voice}' is not a voice of {base_name(path)} - it offers: {join(c.voices, ", ")}"
            return <- sp
        }
    }
    if (!c.speed && sp.speed != 1.0) {
        why = "{base_name(path)} takes no speed - drop --speed"
        return <- sp
    }
    sp.armed = true
    return <- sp
}

//! The directory a file is about to land in, created when absent; false with the reason when it
//! cannot be.
def private ensure_dir_for(out : string; var why : string&) : bool {
    var err = ""
    if (!mkdir_rec(dir_name(out), err)) {   // true for "" and for a directory that exists
        why = "could not create the directory {dir_name(out)} for {out}: {err}"
        return false
    }
    return true
}

//! Speak `text` into `out`; false with the reason when the file did not land.
def private speak_to(var sp : Speech; text : string; out : string; var why : string&) : bool {
    if (!ensure_dir_for(out, why)) {
        return false
    }
    var inscope audio <- synthesize(sp.model, text, sp.voice, sp.speed)
    if (!write_wav_pcm16(out, audio.pcm, audio.sample_rate)) {
        why = "could not write {out} ({stat(dir_name(out)).is_valid ? "the file is not writable" : "the directory does not exist"})"
        return false
    }
    note("wrote {out} - {sp.voice}, {timings_line(audio.timings, audio.sample_rate)}")
    return true
}

//! Everything a chat turn needs beside the model: the towers, the speech model, the flags.
struct private ChatRig {
    system : string
    seed : int          // 0 = the session's fixed default
    max_new : int64
    kv : KVDtype
    no_think : bool
    show_thinking : bool
    quiet : bool
    out_dir : string
}

//! A fresh session under the rig's system prompt, with `turns` replayed as known exchanges.
def private open_chat(m : Model; cr : ChatRig; turns : array<ReplTurn>) : ChatSession {
    var chat <- create_chat(m, cr.system, cr.max_new, cr.kv)
    if (gpu_device_sessions() > 0l) {
        delete chat.session
        chat.session <- create_device_session(m, gpu_device_session_dtype())
    }
    if (cr.seed != 0) {
        set_seed(chat.session, cr.seed)
    }
    if (cr.no_think) {
        set_thinking(chat, false)
    }
    for (tr in turns) {
        add_user(chat, tr.user)
        add_assistant(m, chat, tr.assistant)
    }
    return <- chat
}

// ===== complete =====

def private run_complete(plan : CliPlan; cfg : CliConfig) : int {
    var err = ""
    let prompt = text_input(plan.complete.prompt, plan.complete.file, plan.bare, err)   // validate_plan held: it reads
    let image_spec = !empty(plan.complete.image_mmproj) ? plan.complete.image_mmproj : cfg.image_mmproj
    let rig = arm_rig(plan.common, cfg, plan.sampler, 256l)
    var why = ""
    let image_mmproj = resolve_aux(rig, image_spec, "vision mmproj", why)
    if (!empty(why)) return fail(why)
    var m <- load_llm(rig, plan.common.model, why)
    if (!empty(why)) return fail(why)
    let params = sampling_of(plan.sampler, false)
    var rc = 0
    with_job_que() {
        setup_dasllama_jobque()
        if (!empty(plan.complete.image)) {
            var v <- load_vision(image_mmproj)
            // the family's own thinking default: instruct mode cuts a gemma-4 image turn at its first channel marker
            let no_turns : array<ReplTurn>
            var chat <- open_chat(m, ChatRig(seed = plan.sampler.seed, max_new = rig.max_new, kv = rig.kv), no_turns)
            if (!queue_image(m, chat, v, plan.complete.image, why)) {
                rc = fail(why)
                return
            }
            add_user(chat, prompt)
            stream_reply(m, chat, params, true)   // a completion hides nothing: the reasoning streams, then the answer
            if (!plan.complete.quiet) {
                note(stats_line(stats(chat.session)))
            }
            delete chat
            return
        }
        let toks <- encode(m, prompt, true, false)
        var s <- home_session(m, rig)
        if (plan.sampler.seed != 0) {
            set_seed(s, plan.sampler.seed)
        }
        generate(m, s, toks, params, rig.max_new) $(_id, piece) {
            emit(piece)
            return true
        }
        emit("\n")
        if (!plan.complete.quiet) {
            note(stats_line(stats(s)))
        }
    }
    return rc
}

// ===== chat =====

//! The chat loop's engine over a loaded model and the terminal (or a script): the session, the
//! towers, the speech model, the lines.
class CliEngine : ReplEngine {
    m : Model = Model()
    chat : ChatSession = ChatSession()
    v : Vision = Vision()
    h : Hearing = Hearing()
    sp : Speech = Speech()
    cr : ChatRig = ChatRig()
    params : SamplingParams = SamplingParams()
    stats_line_text : string
    script : array<string>
    at : int
    interactive : bool

    //! The lines a chat reads: the script file, else stdin; false at the end of input.
    def override read_line(var line : string&) : bool {
        if (!interactive) {
            if (at >= length(script)) {
                return false
            }
            line = script[at]
            at++
            emit("> {line}\n")
            return true
        }
        emit("> ")
        line = fgets(fstdin())
        return !empty(line)   // EOF reads as "" - a blank line is "\n"
    }

    def override emit(text : string) : void {
        emit(text)
    }

    def override say(var st : ReplState; text : string) : void {
        var why = ""
        for (path in st.images) {
            if (!queue_image(m, chat, v, path, why)) {
                note(why)
            }
        }
        for (path in st.audio) {
            if (!queue_audio(m, chat, h, path, why)) {
                note(why)
            }
        }
        st.images |> clear()
        st.audio |> clear()
        add_user(chat, text)
        let reply = stream_reply(m, chat, params, cr.show_thinking)
        let split = split_reasoning(chat, reply)
        st.turns |> push(ReplTurn(user = text, assistant = split.content))
        stats_line_text = stats_line(stats(chat.session))
        if (!cr.quiet) {
            note(stats_line_text)
        }
        if (st.speak && sp.armed) {
            let out = path_join(empty(cr.out_dir) ? "." : cr.out_dir, "reply_{length(st.turns)}.wav")
            if (!speak_to(sp, split.content, out, why)) {
                note(why)
            }
        }
    }

    def override reopen(var st : ReplState) : void {
        cr.system = st.system
        delete chat
        chat <- open_chat(m, cr, st.turns)
    }

    def override last_stats : string {
        return stats_line_text
    }

    def override can_speak : bool {
        return sp.armed
    }
}

//! The audio tower a chat arms: the flag, else the served mmproj when it carries an audio encoder
//! (the E-series file serves both towers).
def private chat_audio_mmproj(plan : CliPlan; cfg : CliConfig) : string {
    if (!empty(plan.chat.audio_mmproj)) {
        return plan.chat.audio_mmproj
    }
    return (!empty(cfg.image_mmproj) && stat(cfg.image_mmproj).is_valid && audio_probe_proj_dim(cfg.image_mmproj) > 0l) ? cfg.image_mmproj : ""
}

def private run_chat(plan : CliPlan; cfg : CliConfig) : int {
    let rig = arm_rig(plan.common, cfg, plan.sampler, 16384l)
    var why = ""
    var eng = new CliEngine()
    eng.m <- load_llm(rig, plan.common.model, why)
    if (!empty(why)) return fail(why)
    let image_mmproj = resolve_aux(rig, !empty(plan.chat.image_mmproj) ? plan.chat.image_mmproj : cfg.image_mmproj, "vision mmproj", why)
    if (!empty(why)) return fail(why)
    let audio_mmproj = resolve_aux(rig, chat_audio_mmproj(plan, cfg), "audio mmproj", why)
    if (!empty(why)) return fail(why)
    eng.interactive = empty(plan.chat.script)
    let body = eng.interactive ? "" : fread(plan.chat.script)   // validate_plan held: it reads
    eng.script <- split(body, "\n")
    while (!empty(eng.script) && empty(strip(eng.script[length(eng.script) - 1]))) {
        eng.script |> pop()
    }
    var rc = 0
    with_job_que() {
        setup_dasllama_jobque()
        eng.params = sampling_of(plan.sampler, true)
        eng.cr = ChatRig(system = plan.chat.system, seed = plan.sampler.seed, max_new = rig.max_new, kv = rig.kv,
                         no_think = plan.chat.no_think, show_thinking = !plan.chat.hide_thinking, quiet = plan.chat.quiet,
                         out_dir = plan.chat.out_dir)
        eng.v <- load_vision(image_mmproj)
        eng.h <- load_hearing(audio_mmproj, why)
        if (!empty(why)) {
            rc = fail(why)
            return
        }
        eng.sp <- load_speech(rig, !empty(plan.chat.tts) ? plan.chat.tts : cfg.tts, plan.chat.voice, plan.chat.speed, why)
        if (!empty(why)) {
            rc = fail(why)
            return
        }
        var st = ReplState(system = eng.cr.system, speak = eng.sp.armed)
        eng.chat <- open_chat(eng.m, eng.cr, st.turns)
        if (eng.interactive) {
            eng->emit("{TOOL}: {base_name(plan.common.model)} ready - /help lists the commands, /exit quits\n")
        }
        repl_run(st, eng)
        delete eng.chat
    }
    unsafe {
        delete eng
    }
    return rc
}

// ===== transcribe =====

//! The ASR model a plan names with its mmproj pairing (the flag, else the config's when the ASR
//! is the config's), loaded; "" in `why` when it loaded.
def private load_asr(rig : Rig; asr_spec, mmproj_flag : string; cfg : CliConfig; var why : string&) : AsrModel {
    let asr = resolve_model_path(asr_spec, rig.models_dir)
    if (empty(asr)) {
        why = "ASR model not found: {asr_spec}"
        return <- AsrModel()
    }
    let mmproj = resolve_aux(rig, !empty(mmproj_flag) ? mmproj_flag : (asr_spec == cfg.asr ? cfg.mmproj : ""), "ASR mmproj", why)
    if (!empty(why)) {
        return <- AsrModel()
    }
    note("loading ASR {asr}")
    return <- (empty(mmproj) ? load_asr_model(asr) : load_asr_model(asr, mmproj))
}

//! The language hint a session takes: the flag, else "auto" for a model that detects the
//! language itself (its caps list no codes), else English. A hint the model cannot take is
//! refused with what it does take, in `why`.
def private asr_lang(m : AsrModel; flag : string; var why : string&) : string {
    var inscope c <- caps(m)
    if (empty(flag)) {
        return empty(c.languages) ? "auto" : "en"
    }
    if (flag == "auto") {
        return flag
    }
    if (empty(c.languages)) {
        why = "this ASR model detects the language itself - drop --lang (or pass --lang auto)"
        return ""
    }
    if (find_index(c.languages, flag) < 0) {
        why = "'{flag}' is not a language this ASR model takes - it takes: {join(c.languages, ", ")}"
        return ""
    }
    return flag
}

def private run_transcribe(plan : CliPlan; cfg : CliConfig) : int {
    let file = !empty(plan.transcribe.file) ? plan.transcribe.file : plan.bare[0]   // validate_plan held: one of them is set
    let rig = arm_rig(plan.common, cfg, plan.sampler, 0l)
    var why = ""
    var m <- load_asr(rig, !empty(plan.transcribe.asr) ? plan.transcribe.asr : cfg.asr, plan.transcribe.mmproj, cfg, why)
    if (!empty(why)) return fail(why)
    let t_decode = ref_time_ticks()
    let samples <- load_audio_16k_mono(file)
    if (empty(samples)) return fail("could not decode audio: {file}")
    let decode_s = double(get_time_usec(t_decode)) / 1.0e6lf
    let audio_s = double(length(samples)) / 16000.0lf
    var lerr = ""
    let lang = asr_lang(m, plan.transcribe.lang, lerr)
    if (!empty(lerr)) return fail(lerr)
    var text = ""
    let t_tr = ref_time_ticks()
    with_job_que() {
        setup_dasllama_jobque()
        var s = create_session(m, lang)
        text = transcribe(m, s, samples)
    }
    let transcribe_s = double(get_time_usec(t_tr)) / 1.0e6lf
    var werr = ""
    if (empty(plan.transcribe.out)) {
        emit("{text}\n")
    } elif (!ensure_dir_for(plan.transcribe.out, werr)) {
        return fail(werr)
    } elif (fwrite(plan.transcribe.out, text)) {
        note("wrote {plan.transcribe.out}")
    } else {
        return fail("could not write {plan.transcribe.out}")
    }
    let rtf = audio_s > 0.0lf ? transcribe_s / audio_s : 0.0lf
    note("{audio_s} s audio | decode {decode_s} s | transcribe {transcribe_s} s | rtf {rtf}")
    return 0
}

// ===== speak =====

def private run_speak(plan : CliPlan; cfg : CliConfig) : int {
    var err = ""
    let text = text_input(plan.speak.text, plan.speak.file, plan.bare, err)   // validate_plan held: it reads
    let rig = arm_rig(plan.common, cfg, plan.sampler, 0l)
    let lane_spelling = !empty(plan.speak.tts_lane) ? plan.speak.tts_lane : cfg.tts_lane
    defer() {   // the lane pin must not outlive this run, on any path out
        reset_tts_q8()
    }
    if (!empty(lane_spelling)) {
        set_tts_q8(pick_tts_lane(lane_spelling, TOOL) == "q8")
    }
    let t_load = ref_time_ticks()
    var why = ""
    var sp <- load_speech(rig, !empty(plan.speak.tts) ? plan.speak.tts : cfg.tts, plan.speak.voice, plan.speak.speed, why)
    if (!empty(why)) return fail(why)
    note("TTS load {double(get_time_usec(t_load)) / 1.0e6lf} s")
    let out = empty(plan.speak.out) ? "out.wav" : plan.speak.out
    var rc = 0
    with_job_que() {
        setup_dasllama_jobque()
        set_asr_prof(plan.speak.prof)
        if (!speak_to(sp, text, out, why)) {
            rc = fail(why)
        }
        if (plan.speak.prof) {
            asr_prof_report()
        }
    }
    return rc
}

// ===== talk: audio or text in, the reply spoken =====

//! The clip to answer, or nothing when the prompt is text.
def private talk_audio(in_file : string) : array<float> {
    if (empty(in_file)) {
        var none : array<float>
        return <- none
    }
    return <- load_audio_16k_mono(in_file)
}

def private run_talk(plan : CliPlan; cfg : CliConfig) : int {
    let in_file = !empty(plan.talk.in_file) ? plan.talk.in_file : (empty(plan.bare) ? "" : plan.bare[0])   // validate_plan held: this or the prompt
    let rig = arm_rig(plan.common, cfg, plan.sampler, 16384l)
    var why = ""
    var asr <- empty(in_file) ? AsrModel() : load_asr(rig, !empty(plan.talk.asr) ? plan.talk.asr : cfg.asr, plan.talk.mmproj, cfg, why)
    if (!empty(why)) return fail(why)
    let samples <- talk_audio(in_file)
    if (!empty(in_file) && empty(samples)) return fail("could not decode audio: {in_file}")
    var m <- load_llm(rig, plan.common.model, why)
    if (!empty(why)) return fail(why)
    var sp <- load_speech(rig, !empty(plan.talk.tts) ? plan.talk.tts : cfg.tts, plan.talk.voice, plan.talk.speed, why)
    if (!empty(why)) return fail(why)
    let out = empty(plan.talk.out) ? "reply.wav" : plan.talk.out
    let params = sampling_of(plan.sampler, true)
    var rc = 0
    with_job_que() {
        setup_dasllama_jobque()
        var text = plan.talk.prompt
        var heard_s = 0.0lf
        if (!empty(in_file)) {
            var lerr = ""
            let lang = asr_lang(asr, plan.talk.lang, lerr)
            if (!empty(lerr)) {
                rc = fail(lerr)
                return
            }
            let t0 = ref_time_ticks()
            var s = create_session(asr, lang)
            text = transcribe(asr, s, samples)
            heard_s = double(get_time_usec(t0)) / 1.0e6lf
            if (!plan.talk.quiet) {
                emit("you: {text}\n")
            }
            if (empty(strip(text))) {
                rc = fail("the ASR model heard nothing in {in_file}")
                return
            }
        }
        let no_turns : array<ReplTurn>
        var chat <- open_chat(m, ChatRig(system = plan.talk.system, seed = plan.sampler.seed, max_new = rig.max_new, kv = rig.kv, no_think = plan.talk.no_think), no_turns)
        add_user(chat, text)
        let t1 = ref_time_ticks()
        let reply = stream_reply(m, chat, params, false)
        let reply_s = double(get_time_usec(t1)) / 1.0e6lf
        let split = split_reasoning(chat, reply)
        let t2 = ref_time_ticks()
        if (!speak_to(sp, split.content, out, why)) {
            rc = fail(why)
        }
        let spoke_s = double(get_time_usec(t2)) / 1.0e6lf
        if (!plan.talk.quiet) {
            note("{stats_line(stats(chat.session))}")
            note("heard {heard_s} s | replied {reply_s} s | spoke {spoke_s} s")
        }
        delete chat
    }
    return rc
}

// ===== embed, tokenize =====

def private run_embed(plan : CliPlan; cfg : CliConfig) : int {
    var err = ""
    let text = text_input(plan.embed.text, plan.embed.file, plan.bare, err)   // validate_plan held: it reads
    let rig = arm_rig(plan.common, cfg, plan.sampler, 0l)
    var why = ""
    var m <- load_llm(rig, plan.common.model, why)
    if (!empty(why)) return fail(why)
    with_job_que() {
        setup_dasllama_jobque()
        let vec <- embed(m, text)
        if (plan.embed.json) {
            var doc = JV(vec)
            emit("{write_json_compact(doc)}\n")
            unsafe {
                delete doc
            }
        } else {
            for (x in vec) {
                emit("{x}\n")
            }
        }
    }
    return 0
}

def private run_tokenize(plan : CliPlan; cfg : CliConfig) : int {
    var err = ""
    let text = text_input(plan.tokenize.text, plan.tokenize.file, plan.bare, err)   // validate_plan held: it reads
    let rig = arm_rig(plan.common, cfg, plan.sampler, 0l)
    var why = ""
    let m <- load_llm(rig, plan.common.model, why)
    if (!empty(why)) return fail(why)
    let toks <- encode(m, text, !plan.tokenize.no_special, false)
    if (plan.tokenize.ids) {
        let ids <- [for (id in toks); "{id}"]
        emit("{join(ids, " ")}\n")
    } else {
        for (id in toks) {
            emit("{id}\t{piece(m, id)}\n")
        }
    }
    note("{length(toks)} token(s)")
    return 0
}

// ===== bench: the llama-bench rows =====

//! One row's line, txt or a markdown table row.
def private say_row(md : bool; model, backend : string; row : BenchRow; reps : int) {
    if (md) {
        emit("| {model} | {backend} | {row.test} | {row.mean:.2f} ± {row.sd:.2f} |\n")
    } else {
        emit("{row.test}: {row.mean:.2f} ± {row.sd:.2f} tok/s ({reps} reps)\n")
    }
}

def private run_bench(plan : CliPlan; cfg : CliConfig) : int {
    let md = to_lower(plan.bench.output) == "md"   // validate_plan held: txt or md
    let plen = int64(max(plan.bench.plen, 0))
    let ngen = int64(max(plan.bench.ngen, 0))
    let reps = max(plan.bench.reps, 1)
    let npl = int64(max(plan.bench.npl, 0))
    let rig = arm_rig(plan.common, cfg, plan.sampler, 0l)
    var why = ""
    var m <- load_llm(rig, plan.common.model, why)
    if (!empty(why)) return fail(why)
    let model_name = base_name(resolve_model_path(plan.common.model, rig.models_dir))
    let device_home = gpu_device_sessions() > 0l
    let backend = device_home ? "{rig.gpu}:resident" : (empty(rig.gpu) || rig.gpu == "off" ? "cpu" : rig.gpu)
    var rc = 0
    with_job_que() {
        setup_dasllama_jobque()
        let served_ctx = m.config.seq_len   //! bench_session narrows it to the rows; the batched row takes the served width back
        if (md) {
            emit("| model | backend | test | t/s |\n|---|---|---|---|\n")
        }
        var s <- device_home ? create_device_session(m, gpu_device_session_dtype()) : bench_session(m, plen, ngen, rig.kv, rig.kv)
        if (plen > 0l) {
            bench_pp_warm(m, s, plen)   // untimed full-size run, as llama-bench does
            let sane = bench_warm_sanity(m, s)
            if (!sane.ok) {
                rc = fail("pp{plen} refused: {sane.detail} - the serving pipeline is broken and its timings would be meaningless")
                delete s
                return
            }
            let tps <- [for (rep in range(reps)); bench_pp_rep(m, s, plen, int64(rep))]
            say_row(md, model_name, backend, bench_row("pp{plen}", tps), reps)
        }
        if (ngen > 0l) {
            bench_tg_warm(m, s)   // one untimed token, llama-bench's test_gen(ctx, 1)
            let tps <- [for (rep in range(reps)); bench_tg_rep(m, s, ngen, int64(rep))]
            say_row(md, model_name, backend, bench_row("tg{ngen}", tps), reps)
        }
        delete s
        if (ngen > 0l && npl > 1l) {
            m.config.seq_len = served_ctx
            let bplen = plen > 0l ? plen : BENCH_PP_TOKENS
            var tps : array<double>
            var refused = ""
            for (rep in range(reps)) {
                let br = bench_tg_batched_rep(m, npl, bplen, ngen, int64(rep), false, rig.kv)
                if (!empty(br.why)) {
                    refused = br.why
                    break
                }
                tps |> push(br.tps)
            }
            if (empty(refused)) {
                say_row(md, model_name, backend, bench_row("tg{ngen}@{npl}", tps), reps)
            } else {
                emit("tg{ngen}@{npl}: refused - {refused}\n")
            }
        }
    }
    return rc
}

// ===== main =====

[export, tune_policy(missing = "auto")]
def main : int {
    if (!jit_enabled() && !is_standalone_exe()) {
        // the kernels are [llvm_code]: the interpreter would run them far too slowly to be useful
        return fail("run under the JIT - bin/daslang -jit utils/dasllama-server/cli.das -- <command> ...")
    }
    // stdout carries the answer alone: the engine's notices (a missing Metal profile, a decline)
    // go to logs/dasllama-cli.log under the das root, the CLI's own lines to stderr. Not an
    // [init]: a host that loads this file as a module (the MCP tools) carries its own hook
    // agent, and two collide. The tune policy guard speaks before main, so from the source
    // tree its one status line still lands on stdout; the shipped exe has no guard.
    logger_init(TOOL)
    let argv <- get_user_args()
    var plan <- parse_cli(argv)
    if (!empty(plan.error)) {
        emit("error: {plan.error}\n\n")
        emit(plan.cmd == CliCommand.none ? cli_usage() : command_help(plan.cmd))
        return 2
    }
    if (plan.help) {
        emit(plan.cmd == CliCommand.none ? cli_usage() : command_help(plan.cmd))
        return 0
    }
    log_tune_status(TOOL)
    let cfg = load_cli_config(plan.common.config, get_this_module_dir())
    if (!empty(cfg.error)) {
        return fail(cfg.error)
    }
    if (!empty(cfg.path)) {
        note("using config {cfg.path}")
    }
    fill_common(plan.common, cfg)
    let refusal = validate_plan(plan, cfg)
    if (!empty(refusal)) {
        return fail(refusal)
    }
    var rc = 1
    try {
        rc = dispatch(plan, cfg)
    } recover {
        // the logger hook keeps the engine's report in the log file; the user gets the text too
        rc = fail("{this_context().last_exception} (the engine's report is in logs/{TOOL}.log)")
    }
    return rc
}

def private dispatch(plan : CliPlan; cfg : CliConfig) : int {
    if (plan.cmd == CliCommand.complete) return run_complete(plan, cfg)
    if (plan.cmd == CliCommand.chat) return run_chat(plan, cfg)
    if (plan.cmd == CliCommand.transcribe) return run_transcribe(plan, cfg)
    if (plan.cmd == CliCommand.speak) return run_speak(plan, cfg)
    if (plan.cmd == CliCommand.talk) return run_talk(plan, cfg)
    if (plan.cmd == CliCommand.embed) return run_embed(plan, cfg)
    if (plan.cmd == CliCommand.bench) return run_bench(plan, cfg)
    return run_tokenize(plan, cfg)
}
