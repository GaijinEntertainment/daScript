options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// Transcribe a FLEURS-layout corpus (a <split>.tsv beside a <split>/ folder of wavs) with one ASR model on one
// weight lane, and optionally translate every hypothesis and every reference with a Bergamot student. Writes one
// TSV row a clip: id, wav, audio seconds, transcribe ms, hypothesis[, hypothesis translated, reference translated].
// Scoring (WER, chrF) is the caller's.
//
//   bin/daslang -jit modules/dasLLAMA/harness/asr_corpus_eval.das -- --model F:/models/gigaam-v3-e2e-rnnt-f16.gguf \
//       --lane exact --corpus F:/corpora/fleurs/ru_ru --out F:/corpora/fleurs/ru_ru/hyp/f16-exact.tsv

require dasllama/dasllama
require daslib/clargs
require daslib/jobque_boost
require daslib/fio
require jobque   // set_jobque_threads_cap
require strings
require daslib/strings_boost   // split
require math

[CommandLineArgs]
struct EvalArgs {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "ASR model path"
    model : string

    @clarg_doc = "GigaAM weight lane: q8 | exact (default: the family's policy)"
    lane : string

    @clarg_required
    @clarg_doc = "Corpus folder holding <split>.tsv and <split>/<wav>"
    corpus : string

    @clarg_doc = "Corpus split (default test)"
    split : string = "test"

    @clarg_required
    @clarg_short = "o"
    @clarg_doc = "Output TSV"
    out : string

    @clarg_doc = "Bergamot student to translate the hypotheses and references with"
    nmt : string

    @clarg_doc = "Stop after this many clips (0 = all)"
    limit : int

    @clarg_doc = "Engine threads (default 16)"
    threads : int = 16

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct private Clip {
    id : string
    wav : string
    ref : string
}

def private read_clips(path : string; limit : int) : array<Clip> {
    var clips : array<Clip>
    fopen(path, "rb") $(f) {
        panic("asr_corpus_eval: cannot read {path}") if (f == null)
        while (!feof(f) && (limit <= 0 || length(clips) < limit)) {
            let line = fgets(f)
            continue if (empty(line))
            var inscope cols <- split(strip_right(line), "\t")
            continue if (length(cols) < 3)
            clips |> push(Clip(id = cols[0], wav = cols[1], ref = cols[2]))
        }
    }
    return <- clips
}

def private one_line(s : string) : string => replace_multiple(s, [(text = "\t", replacement = " "), (text = "\r", replacement = " "), (text = "\n", replacement = " ")])

[export, cold_path]
def main() : int {
    var r <- parse_args(type<EvalArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<EvalArgs>), "asr_corpus_eval")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<EvalArgs>), "asr_corpus_eval")
        return 0
    }
    if (cfg.lane == "q8" || cfg.lane == "exact") {
        set_asr_tower_q8(cfg.lane == "q8")
    } elif (!empty(cfg.lane)) {
        print("error: --lane is q8 or exact\n")
        return 1
    }
    set_jobque_threads_cap(max(1, cfg.threads - 1))
    var rc = 0
    with_job_que() {
        setup_dasllama_jobque()
        rc = run_eval(cfg)
    }
    return rc
}

def private run_eval(cfg : EvalArgs) : int {
    let clips <- read_clips(path_join(cfg.corpus, "{cfg.split}.tsv"), cfg.limit)
    let t_load = ref_time_ticks()
    var m <- load_asr_model(cfg.model)
    print("LOAD\t{base_name(cfg.model)}\t{cfg.lane}\t{double(get_time_usec(t_load)) / 1000.0lf} ms\n")
    let has_nmt = !empty(cfg.nmt)
    var inscope tr <- has_nmt ? load_translator(cfg.nmt) : Translator()
    mkdir_rec(dir_name(cfg.out))
    var total_audio = 0.0lf
    var total_ms = 0.0lf
    fopen(cfg.out, "wb") $(f) {
        panic("asr_corpus_eval: cannot write {cfg.out}") if (f == null)
        for (c, i in clips, count()) {
            var inscope samples <- load_audio_16k_mono(path_join(path_join(cfg.corpus, cfg.split), c.wav))
            let audio_s = double(length(samples)) / double(AUDIO_SAMPLE_RATE)
            var s = create_session(m)
            let t0 = ref_time_ticks()
            let hyp = one_line(transcribe(m, s, samples))
            let ms = double(get_time_usec(t0)) / 1000.0lf
            total_audio += audio_s
            total_ms += ms
            var row = "{c.id}\t{c.wav}\t{audio_s}\t{ms}\t{hyp}"
            if (has_nmt) {
                row += "\t{one_line(translate(tr, hyp))}\t{one_line(translate(tr, c.ref))}"
            }
            fwrite(f, "{row}\n")
            if ((i + 1) % 100 == 0) {
                print("PROGRESS\t{i + 1}/{length(clips)}\n")
            }
        }
    }
    print("DONE\t{length(clips)} clips\t{total_audio} s audio\t{total_ms / 1000.0lf} s transcribe\t{total_audio / (total_ms / 1000.0lf)} x realtime\n")
    return 0
}
