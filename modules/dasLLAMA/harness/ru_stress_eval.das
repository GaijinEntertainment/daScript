options gen2
options stack = 524288
options _dasllama_internal = true
options _ascii_strings = false

// Score the Russian stress-context model through the engine: every row of a jsonl of {id, text, form,
// ordinal} has its word found (ru_stress_find_word), picked among the catalog's ordinals (ru_stress_pick)
// and timed; prints the accuracy, the pieces and [UNK] rate, the agreement with a reference's picks (--torch,
// rows of {id, pick}) and the milliseconds a sentence by piece-count bucket, and writes one jsonl row a
// sentence; --subset scores only the rows a second jsonl lists (those it marks `agrees`). --text times the marking
// of plain sentences, one a line, through --pack - in context with --model, by the pack alone without it.
// --lane-race picks the pass's lane work and lane floor: every candidate's forward once a round, 30 rounds
// after 2 warmup rounds, at 30 and at 96 pieces.
// ATTRIBUTION SWEEP: --stages times one sentence cut to 30 and to 96 pieces stage by stage over the model's own
// weights - where a pass's time goes, 20 rounds after 2 warmup rounds; its rows pick nothing.
// The per-sentence rows (MS, RATE, FRONT_END) drop 0 warmup rounds: each sentence is timed once, the first included.
//
//   bin/daslang -jit modules/dasLLAMA/harness/ru_stress_eval.das -- --model <models dir>/ru-stress-rubert-base-q4.gguf \
//       --lane q8 --data <sentences.jsonl> [--subset <rows.jsonl>] [--torch <reference picks.jsonl>] [--out <picks.jsonl>]

require dasllama/dasllama_g2p_ru_stress
require dasllama/dasllama_g2p_ru
require dasllama/dasllama_textnorm_ru
require dasllama/dasllama_unicode
require dasllama/dasllama_tts_blocks
require dasllama/dasllama_tower
require dasllama/dasllama_math
require dasllama/dasllama_convert
require dasllama/dasllama_gemm_schema
require ../performance/profile_common.das   // tune_gate + apply_box_profile_runtime, the standing pre-measure stack
require _prompt_files   // read_lines
require daslib/clargs
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require math

[CommandLineArgs]
struct EvalArgs {
    @clarg_short = "m"
    @clarg_doc = "ru_stress GGUF (harness/convert_ru_stress.py); only --text runs without one"
    model : string

    @clarg_doc = "Plain sentences, one a line: time the marking of each through --pack, with --model in context and without it by the pack alone"
    text : string

    @clarg_doc = "Weight lane: q8 | f32 (default q8)"
    lane : string = "q8"

    @clarg_doc = "jsonl of rows with id, text, form and ordinal"
    data : string

    @clarg_doc = "jsonl of a reference's picks (id and pick a row) to count the disagreements against"
    torch : string

    @clarg_doc = "jsonl of (text, form) rows: score only the rows it lists - the rows the file marks agrees"
    subset : string

    @clarg_short = "o"
    @clarg_doc = "jsonl the picks go to, one row a sentence"
    out : string

    @clarg_doc = "Stop after this many rows (0 = all)"
    limit : int

    @clarg_doc = "Time one sentence cut to 30 and 96 pieces stage by stage instead of scoring"
    stages : bool

    @clarg_doc = "Race the pass's lane work and lane floor: one forward a candidate a round at 30 and 96 pieces, the shipped pair among them"
    lane_race : bool

    @clarg_doc = "tts_g2p_ru.bin: score through the front end instead - the normalized sentence's context picks and the marked word, beside the pack alone"
    pack : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct private EvalRow {
    id : string
    text : string
    form : string
    ordinal : int
}

struct private SubsetRow {
    text : string
    form : string
    verdict : string
}

struct private RefPick {
    id : string
    pick : int
}

struct private PickRow {
    id : string
    form : string
    ordinal : int
    pick : int
    pieces : int
    ms : double
    logits : array<float>
}

//! the first `limit` non-empty lines of `path` (all of them at 0); a file that cannot be read panics naming it
def private lines_of(path : string; limit : int) : array<string> {
    panic("ru_stress_eval: cannot read {path}") if (!stat(path).is_valid)
    return <- read_lines(path, limit)
}

def private read_rows(path : string; limit : int; var rows : array<auto(RT)>) {
    var inscope lines <- lines_of(path, limit)
    rows |> reserve(length(rows) + length(lines))
    for (line in lines) {
        var row : RT
        if (!sscan_json(line, row)) {
            panic("ru_stress_eval: {path}: a row does not parse: {line}")
        }
        rows |> emplace(row)
    }
}

def private percentile(var xs : array<double>; p : double) : double {
    return 0.0lf if (empty(xs))
    sort(xs)
    return xs[min(length(xs) - 1, int(p * double(length(xs))))]
}

let private BUCKET_EDGES = fixed_array(24, 48, 96)   //! the largest piece count of each timing bucket

def private bucket_of(pieces : int) : int {
    for (edge, bucket in BUCKET_EDGES, count()) {
        return bucket if (pieces <= edge)
    }
    return length(BUCKET_EDGES) - 1
}

def private bucket_name(bucket : int) : string => bucket == 0 ? "<={BUCKET_EDGES[0]}" : "{BUCKET_EDGES[bucket - 1] + 1}-{BUCKET_EDGES[bucket]}"

def private subset_only(path : string; var rows : array<EvalRow>) {
    var listed : array<SubsetRow>
    read_rows(path, 0, listed)
    let agrees <- {for (s in listed); "{s.text}\t{s.form}"; where s.verdict == "agrees"}
    let before = length(rows)
    var kept <- [for (r in rows); r; where key_exists(agrees, "{r.text}\t{r.form}")]
    delete rows
    rows <- kept
    print("SUBSET\t{length(rows)} of {before} rows kept\n")
}

struct private FrontTally {
    rows : int
    lost : int
    same_text : int
    ctx_right : int
    pack_right : int
    direct_right : int
    same_ctx_right : int
    asked : int
    fallbacks : int
    beyond_pack : int
    single_reading : int
    single_right : int
}

//! for the word `lower` at `span` among `words`: the front end's answer (the context pass's pick, else the pack's default) and the pack's default
def private front_ordinal(p : RuStressPack; picks : RuContextPicks; words : array<int2>; lower : string; span : int2) : tuple<front : int; pack : int> {
    let pack = ru_pack_default(p, lower)
    for (w, k in words, count()) {
        return (front = picks.picked?[k] ?? pack, pack = pack) if (w == span)
    }
    return (front = pack, pack = pack)
}

let private UNTIMED_MS = -1.0lf     //! the time of a row whose word the sentence does not hold: no pick, so no timing

//! the rows of --data, cut to --subset's where one is named; none, said on a line, where nothing is left to score
def private scored_rows(cfg : EvalArgs) : array<EvalRow> {
    var rows : array<EvalRow>
    read_rows(cfg.data, cfg.limit, rows)
    if (!empty(cfg.subset)) {
        subset_only(cfg.subset, rows)
    }
    if (empty(rows)) {
        print("error: no row of {cfg.data} is left to score\n")
    }
    return <- rows
}

def private run_front_end(cfg : EvalArgs; m : RuStressModel) : int {
    var rows <- scored_rows(cfg)
    return 1 if (empty(rows))
    var p <- load_ru_stress_pack(cfg.pack)
    var sc = RuStressScratch()
    var tally = FrontTally()
    var all_ms : array<double>
    all_ms |> reserve(length(rows))
    for (r in rows) {
        tally.rows++
        let norm = normalize_text_ru(r.text)
        let span = ru_stress_find_word(norm, r.form)
        if (span.x < 0) {
            tally.lost++
            continue
        }
        let t0 = ref_time_ticks()
        var inscope picks <- ru_stress_context_picks(m, p, norm, sc)
        all_ms |> push(double(get_time_usec(t0)) / 1000.0lf)
        tally.asked += picks.asked
        tally.fallbacks += picks.fallbacks
        tally.beyond_pack += picks.beyond_pack
        var inscope words <- ru_text_words(utf8_to_cpts(norm))
        let got = front_ordinal(p, picks, words, r.form, span)
        tally.ctx_right += got.front == r.ordinal ? 1 : 0
        tally.pack_right += got.pack == r.ordinal ? 1 : 0
        var inscope readings <- ru_stress_variants(p, r.form)
        if (length(readings) < 2) {
            tally.single_reading++
            tally.single_right += got.front == r.ordinal ? 1 : 0
        }
        if (norm == r.text) {
            tally.same_text++
            tally.same_ctx_right += got.front == r.ordinal ? 1 : 0
            let direct = ru_stress_pick(m, r.text, ru_stress_find_word(r.text, r.form), ru_stress_allowed(m, r.form), sc)
            tally.direct_right += direct.ordinal == r.ordinal ? 1 : 0
            if (direct.ordinal != got.front) {
                print("GAP\t{r.id}\t{r.form}\tgold {r.ordinal}\tfront end {got.front}\tdirect {direct.ordinal}\tpack {got.pack}\treadings {length(readings)}\t{r.text}\n")
            }
        }
    }
    let n = tally.rows - tally.lost
    if (n == 0) {
        print("error: no sentence of {cfg.data} keeps its word through normalization\n")
        delete p
        return 1
    }
    print("FRONT_END\t{base_name(cfg.data)}\t{n} of {tally.rows} sentences keep the word through normalization\tcontext {tally.ctx_right} = {100.0lf * double(tally.ctx_right) / double(max(n, 1))}%\tpack default {tally.pack_right} = {100.0lf * double(tally.pack_right) / double(max(n, 1))}%\n")
    print("FRONT_END\t{tally.same_text} sentences the normalizer leaves as they are\tfront end {tally.same_ctx_right}\tdirect pick {tally.direct_right}\n")
    print("FRONT_END\t{tally.single_reading} target words the pack holds one reading for\t{tally.single_right} of them right\n")
    print("FRONT_END\twords asked {tally.asked}\tpicks beyond the pack {tally.beyond_pack}\tfallbacks {tally.fallbacks}\tcontext pass ms a sentence median {percentile(all_ms, 0.5lf)} p95 {percentile(all_ms, 0.95lf)}\n")
    delete p
    return 0
}

def private rate_report(cfg : EvalArgs; marked_by : string; sentences : int; total_us : int64; marks : int; var all_ms : array<double>) {
    let seconds = double(total_us) / 1000000.0lf
    print("RATE\t{base_name(cfg.text)}\t{marked_by}\t{sentences} sentences in {seconds} s = {double(sentences) / max(seconds, 0.000001lf)} sentences/s\tstress marks {marks}\tms a sentence median {percentile(all_ms, 0.5lf)} p95 {percentile(all_ms, 0.95lf)}\n")
}

//! every line of --text marked by `mark` and timed, the rate row named `marked_by`; 1 where the file holds no sentence
def private run_text(cfg : EvalArgs; marked_by : string; mark : block<(p : RuStressPack; line : string) : string>) : int {
    var lines <- lines_of(cfg.text, cfg.limit)
    if (empty(lines)) {
        print("error: {cfg.text} holds no sentence\n")
        return 1
    }
    var p <- load_ru_stress_pack(cfg.pack)
    var all_ms : array<double>
    all_ms |> reserve(length(lines))
    var marks = 0
    let t_all = ref_time_ticks()
    for (line in lines) {
        let t0 = ref_time_ticks()
        let marked = invoke(mark, p, line)
        all_ms |> push(double(get_time_usec(t0)) / 1000.0lf)
        marks += count(marked, "+")
    }
    rate_report(cfg, marked_by, length(lines), int64(get_time_usec(t_all)), marks, all_ms)
    delete p
    return 0
}

def private run_text_pack(cfg : EvalArgs) : int =>
    run_text(cfg, "pack alone") $(p, line) => ru_mark_stress(p, line)

def private run_text_model(cfg : EvalArgs; m : RuStressModel) : int {
    var sc = RuStressScratch()
    return run_text(cfg, "{base_name(cfg.model)} {cfg.lane}") $(p, line) => ru_mark_stress_in_context(m, p, line, sc)
}

def private run_eval(cfg : EvalArgs; m : RuStressModel) : int {
    var rows <- scored_rows(cfg)
    return 1 if (empty(rows))
    var picks : array<RefPick>
    if (!empty(cfg.torch)) {
        read_rows(cfg.torch, 0, picks)
    }
    let refs <- {for (r in picks); r.id => r.pick}
    var sc = RuStressScratch()
    var right = 0
    var unfound = 0
    var differ = 0
    var shown = 0
    var word_pieces = 0
    var unk_pieces = 0
    var all_ms : array<double>
    var out : array<PickRow>
    all_ms |> reserve(length(rows))
    out |> reserve(length(rows))
    for (r in rows) {
        let span = ru_stress_find_word(r.text, r.form)
        if (span.x < 0) {
            unfound++
        }
        let t0 = ref_time_ticks()
        let pick = ru_stress_pick(m, r.text, span, ru_stress_allowed(m, r.form), sc)
        let ms = span.x < 0 ? UNTIMED_MS : double(get_time_usec(t0)) / 1000.0lf
        let pieces = length(sc.pieces)
        word_pieces += pieces - 2
        for (p in sc.pieces) {
            if (p.id == m.tok.unk_id) {
                unk_pieces++
            }
        }
        if (span.x >= 0) {
            all_ms |> push(ms)
        }
        if (pick.ordinal == r.ordinal) {
            right++
        }
        let ref = refs?[r.id] ?? -1
        if (ref >= 0 && ref != pick.ordinal) {
            differ++
            if (shown < 5) {
                print("DIFFER\t{r.id}\t{r.form}\tgold {r.ordinal}\ttorch {ref}\tengine {pick.ordinal}\tlogits {pick.logits}\t{r.text}\n")
                shown++
            }
        }
        out |> emplace(PickRow(id = r.id, form = r.form, ordinal = r.ordinal, pick = pick.ordinal, pieces = pieces, ms = ms,
            logits <- [for (o in range(RU_STRESS_ORDINALS)); pick.logits[o]]))
    }
    let n = length(rows)
    print("ACCURACY\t{base_name(cfg.data)}\t{cfg.lane}\t{right} / {n} = {100.0lf * double(right) / double(max(n, 1))}%\tunfound {unfound}\n")
    print("PIECES\tmean {double(word_pieces) / double(max(n, 1))} a sentence (specials aside)\t[UNK] {unk_pieces} of {word_pieces} = {100.0lf * double(unk_pieces) / double(max(word_pieces, 1))}%\tvocabulary {wordpiece_count(m.tok)}\n")
    if (!empty(cfg.torch)) {
        print("AGREEMENT\t{n - differ} / {n} picks the reference's, {differ} differ\n")
    }
    for (bucket in range(length(BUCKET_EDGES))) {
        var inscope b <- [for (row in out); row.ms; where bucket_of(row.pieces) == bucket && row.ms != UNTIMED_MS]
        print("MS\t{bucket_name(bucket)} pieces\t{length(b)} sentences\tmedian {percentile(b, 0.5lf)}\tp95 {percentile(b, 0.95lf)}\n")
    }
    print("MS\tall\t{length(all_ms)} sentences\tmedian {percentile(all_ms, 0.5lf)}\tp95 {percentile(all_ms, 0.95lf)}\n")
    if (!empty(cfg.out)) {
        fopen(cfg.out, "wb") $(f) {
            panic("ru_stress_eval: cannot write {cfg.out}") if (f == null)
            for (row in out) {
                fwrite(f, "{sprint_json(row, false)}\n")
            }
        }
    }
    return 0
}

struct private StageClock {
    us : table<string; int64>
}

def private clock_add(var clk : StageClock; name : string; us : int64) {
    clk.us[name] = (clk.us?[name] ?? 0l) + us
}

def private timed(var clk : StageClock; name : string; blk : block<() : void>) {
    let t0 = ref_time_ticks()
    invoke(blk)
    clock_add(clk, name, int64(get_time_usec(t0)))
}

//! the report's line for each stage `bert_layer_rows` names
let private LAYER_STAGE_LINES <- {
    "qkv" => "attn q/k/v GEMMs", "attention" => "attn softmax(QK)V", "attn_out" => "attn output GEMM",
    "attn_norm" => "residuals + LayerNorms", "ffn_up" => "ffn up GEMM", "gelu" => "ffn gelu",
    "ffn_down" => "ffn down GEMM", "out_norm" => "residuals + LayerNorms"
}

//! the engine's own layer with its stages on the clock
def private staged_layer(var clk : StageClock; lw : RuStressLayer; m : RuStressModel; var h : array<float>; t : int64; var sc : TtsBertScratch) {
    var mark = ref_time_ticks()
    bert_layer_rows(lw, m.heads, m.eps, m.gelu_form, h, t, sc) $(name) {
        clock_add(clk, LAYER_STAGE_LINES?[name] ?? name, int64(get_time_usec(mark)))
        mark = ref_time_ticks()
    }
}

def private stage_report(m : RuStressModel; text : string; pieces : int; reps : int) {
    var sc = RuStressScratch()
    var clk = StageClock()
    var ids : array<int>
    for (r in range(reps + 2)) {
        let keep = r >= 2
        var inscope full : array<WordPiece>
        timed(clk, "tokenize") {
            delete full
            full <- ru_stress_pieces(m.tok, text, m.max_pieces)
        }
        delete ids
        ids <- [for (p in full); p.id]
        ids |> resize(pieces)
        ids[pieces - 1] = m.tok.sep_id
        let t = long_length(ids)
        timed(clk, "embeddings") {
            ru_stress_embed_hidden(m, ids, sc)
        }
        for (lw in m.layers) {
            staged_layer(clk, lw, m, sc.hidden, t, sc.layer)
        }
        delete sc.pieces
        sc.pieces <- full
        sc.pieces |> resize(pieces)
        timed(clk, "pool + head") {
            ru_stress_pick_span(m, int2(0, 1000), 0u, sc)
        }
        timed(clk, "ru_stress_forward (whole, the control)") {
            ru_stress_forward(m, ids, sc)
        }
        if (!keep) {
            delete clk.us
        }
    }
    let ffn & = unsafe(m.layers[0].ffn)
    print("STAGES\t{pieces} pieces\t{m.q8 ? "q8" : "f32"}\tlayer 0 ffn: q8 {ffn.q8} kq format {ffn.kq_fmt} repacked {ffn.kq_repacked}\tbackend {active_kernel_backend()}\tdispatch lanes {get_dispatch_lanes()}\n")
    for (name in fixed_array("tokenize", "embeddings", "attn q/k/v GEMMs", "attn softmax(QK)V", "attn output GEMM", "residuals + LayerNorms",
                             "ffn up GEMM", "ffn gelu", "ffn down GEMM", "pool + head", "ru_stress_forward (whole, the control)")) {
        print("STAGE\t{name}\t{double(clk.us?[name] ?? 0l) / double(reps) / 1000.0lf} ms\n")
    }
    let t = int64(pieces)
    let hidden = int(m.hidden_dim)
    let ffn_dim = ffn.nout
    for (shape in fixed_array(int2(hidden, hidden), int2(hidden, int(ffn_dim)), int2(int(ffn_dim), hidden))) {
        let work = int64(shape.x) * int64(shape.y) * t
        print("LANES\tGEMM {shape.x}->{shape.y} at {pieces} rows\tlanes_for_work {lanes_for_work(work, get_batch_lane_cap())}\tchunks {matmul_chunks(shape.y, get_q8_batch_chunks_per_job(), work)}\n")
    }
    var inscope xq : array<int8>
    var inscope xs, x : array<float>
    x |> resize(t * ffn_dim)
    xq |> resize(t * ffn_dim)
    xs |> resize(t * ffn_dim / Q8_BLOCK_ELEMS)
    let tq = ref_time_ticks()
    for (_r in range(reps)) {
        quantize_q8_0_into(x, t * ffn_dim, xq, xs, 0l, 0l)
    }
    print("STAGE\tq8 requant of one [{pieces}][{ffn_dim}] activation (inside each ffn down GEMM)\t{double(get_time_usec(tq)) / double(reps) / 1000.0lf} ms\n")
}

let private LANE_RACE_ROUNDS = 30
let private LANE_RACE_WARMUP = 2
let private MIB = 1048576l

//! One process, every candidate once a round: the pass's forward at `pieces` pieces under each (lane work, lane floor) pair through the probe seam, the shipped pair among them; a row a candidate.
def private lane_race(m : RuStressModel; text : string; pieces : int) {
    let arms = fixed_array(
        (work = 4l * MIB, floor = RU_STRESS_MIN_LANES), (work = 8l * MIB, floor = RU_STRESS_MIN_LANES),
        (work = RU_STRESS_LANE_WORK, floor = RU_STRESS_MIN_LANES), (work = 32l * MIB, floor = RU_STRESS_MIN_LANES),
        (work = 64l * MIB, floor = RU_STRESS_MIN_LANES), (work = RU_STRESS_LANE_WORK, floor = 1l),
        (work = RU_STRESS_LANE_WORK, floor = 4l), (work = RU_STRESS_LANE_WORK, floor = 16l))
    var sc = RuStressScratch()
    var inscope full <- ru_stress_pieces(m.tok, text, m.max_pieces)
    var inscope ids <- [for (p in full); p.id]
    ids |> resize(pieces)
    ids[pieces - 1] = m.tok.sep_id
    var ms : array<double>[8]
    for (r in range(LANE_RACE_ROUNDS + LANE_RACE_WARMUP)) {
        for (arm, k in arms, count()) {
            set_ru_stress_lanes_probe_(arm.work, arm.floor)
            let t0 = ref_time_ticks()
            ru_stress_forward(m, ids, sc)
            if (r >= LANE_RACE_WARMUP) {
                ms[k] |> push(double(get_time_usec(t0)) / 1000.0lf)
            }
        }
    }
    for (arm, k in arms, count()) {
        set_ru_stress_lanes_probe_(arm.work, arm.floor)
        let shipped = arm.work == RU_STRESS_LANE_WORK && arm.floor == RU_STRESS_MIN_LANES
        sort(ms[k])
        print("LANE_RACE\t{pieces} pieces\tlane work {arm.work} floor {arm.floor}{shipped ? " (shipped)" : ""}\tlanes {ru_stress_lanes(m, int64(pieces))} of {get_dispatch_lanes()}\tforward ms median {percentile(ms[k], 0.5lf)} min {ms[k][0]}\n")
        delete ms[k]
    }
    set_ru_stress_lanes_probe_(0l, 0l)
}

//! the sentence --stages and --lane-race cut to their piece counts
let private STAGE_TEXT = "Старый замок стоял на холме над рекой, и в его стенах жили люди, которые помнили войну, голод и долгие зимы, когда дороги заметало снегом и никто не приходил в деревню до самой весны. Каждое утро звонили колокола, а вечером в окнах горел свет, и старики рассказывали детям о том, как строили этот замок."

[export, cold_path]
def main() : int {
    var r <- parse_args(type<EvalArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<EvalArgs>), "ru_stress_eval")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<EvalArgs>), "ru_stress_eval")
        return 0
    }
    if (cfg.lane != "q8" && cfg.lane != "f32") {
        print("error: --lane is q8 or f32\n")
        return 1
    }
    apply_box_profile_runtime()
    if (!tune_gate()) {
        print("tune_gate refused - point DAS_TUNE_MANIFEST at a complete sidecar\n")
        return 1
    }
    if (!empty(cfg.text) && empty(cfg.pack)) {
        print("error: --text needs --pack\n")
        return 1
    }
    if (empty(cfg.model)) {
        return run_text_pack(cfg) if (!empty(cfg.text))
        print("error: --model is required without --text\n")
        return 1
    }
    var rc = 0
    with_dasllama_jobque_() {
        let t_load = ref_time_ticks()
        var m <- load_ru_stress(cfg.model, cfg.lane == "q8")
        print("LOAD\t{base_name(cfg.model)}\t{cfg.lane}\t{double(get_time_usec(t_load)) / 1000.0lf} ms\n")
        if (!empty(cfg.text)) {
            rc = run_text_model(cfg, m)
        } elif (!empty(cfg.pack)) {
            rc = run_front_end(cfg, m)
        } elif (cfg.stages) {
            stage_report(m, STAGE_TEXT, 30, 20)
            stage_report(m, STAGE_TEXT, 96, 20)
        } elif (cfg.lane_race) {
            lane_race(m, STAGE_TEXT, 30)
            lane_race(m, STAGE_TEXT, 96)
        } else {
            rc = run_eval(cfg, m)
        }
        delete m
    }
    return rc
}
