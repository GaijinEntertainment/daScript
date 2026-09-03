options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

require dasllama/dasllama
require dasllama/dasllama_math
require daslib/jobque_boost
require daslib/json_boost
require daslib/clargs
require daslib/defer          // defer() { reset_tts_q8() } — the weight-lane pin never outlives the run
require daslib/fio
require math

// The rig's synthesis half: every sentence of the 200-sentence fixture through one TTS model
// and voice, one WAV per sentence, one JSON of timings beside them. harness/tts_rig.py drives
// it once per model and scores the folder.
//   daslang -jit harness/tts_synth.das -- --model <gguf> --voice <name> --out <dir> [--speed 1.0] [--limit N] [--fixture <json>]

[CommandLineArgs]
struct SynthArgs {
    @clarg_doc = "the TTS model GGUF"
    @clarg_required
    model : string = ""
    @clarg_doc = "voice name or alias"
    @clarg_required
    voice : string = ""
    @clarg_doc = "output directory for <id>.wav and timings.json"
    @clarg_required
    out : string = ""
    @clarg_doc = "speech speed multiplier"
    speed : float = 1.0
    @clarg_doc = "synthesize only the first N sentences (0 = all)"
    limit : int = 0
    @clarg_doc = "the sentence fixture (default: tests/_tts_fixtures/g2p_corpus.json)"
    fixture : string = ""
    @clarg_doc = "pin the rows GEMMs to Q8_0 quants (the serving default; the q8 lane's own prepared image)"
    q8 : bool = false
    @clarg_doc = "pin the rows GEMMs to the file's f32 planes (the reference lane)"
    f32 : bool = false
    @clarg_doc = "print the generator's per-op profile over the whole run (PROF rows)"
    prof : bool = false
}

struct private SentenceTiming {
    id : string
    audio_s : float
    gen_ms : float
    rtf : float
}

[export]
def main() : int {
    var args = SynthArgs()
    let rc = parse_args_with_help(args, "modules/dasLLAMA/harness/tts_synth.das")
    return rc if (rc >= 0)
    if (args.q8 && args.f32) {
        to_log(LOG_ERROR, "tts synth: --q8 and --f32 pin the rows GEMMs to opposite lanes - pass one, not both\n")
        return 2
    }
    let fixture = empty(args.fixture) ? path_join(get_das_root(), "modules/dasLLAMA/tests/_tts_fixtures/g2p_corpus.json") : args.fixture
    var err = ""
    let rows = read_json(fread(fixture), err)
    if (rows == null || !(rows.value is _array)) {
        to_log(LOG_ERROR, "tts synth: {fixture} does not parse: {err}\n")
        return 2
    }
    mkdir(args.out)
    var inscope timings : array<SentenceTiming>
    var audio_total = 0.0lf
    var gen_total = 0.0lf
    var failed = ""
    defer() {   // the pin must not outlive this run, on any path out
        reset_tts_q8()
    }
    if (args.q8 || args.f32) {
        set_tts_q8(args.q8)
    }
    with_job_que() {
        setup_dasllama_jobque_()
        set_asr_prof(args.prof)
        var inscope m <- load_tts_model(args.model)
        var n = 0
        for (r in rows.value as _array) {
            break if ((args.limit > 0 && n >= args.limit) || !empty(failed))
            let id = r?["id"] ?? ""
            let text = r?["text"] ?? ""
            continue if (empty(id) || empty(text))
            var inscope a <- synthesize(m, text, args.voice, args.speed)
            let wav = path_join(args.out, "{id}.wav")
            if (!write_wav_pcm16(wav, a.pcm, a.sample_rate)) {
                failed = "cannot write {wav}"
                continue
            }
            let audio_s = float(length(a.pcm)) / float(a.sample_rate)
            let gen_ms = float(a.timings.total_us) / 1000.0
            timings |> push(SentenceTiming(id = id, audio_s = audio_s, gen_ms = gen_ms, rtf = gen_ms / 1000.0 / audio_s))
            audio_total += double(audio_s)
            gen_total += double(gen_ms) / 1000.0lf
            n++
        }
        to_log(LOG_INFO, "tts synth: {n} sentences, {audio_total} s of audio in {gen_total} s (rtf {gen_total / max(audio_total, 1e-9lf)}) -> {args.out}\n")
        if (args.prof) {
            asr_prof_report()
        }
    }
    if (!empty(failed)) {
        to_log(LOG_ERROR, "tts synth: {failed}\n")
        return 1
    }
    fwrite(path_join(args.out, "timings.json"), sprint_json(timings, true))
    return 0
}
