options gen2
options persistent_heap
options stack = 524288
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_nmt
require dasllama/dasllama_spm   // load_spm_model_file: the vocabulary reader's refusals
require dasllama/dasllama_translate   // the facade's two translator kinds
require dasllama/dasllama_common   // encode_: the completion's raw continuation
require dasllama/dasllama_sampling   // generate_, SamplingParams: the same
require dasllama/dasllama_tokenizer   // eos_id: the completion's end token
require dasllama/dasllama_chat   // the chat turn the hybrid-thinking cell renders to count its prompt
require dasllama/dasllama_image   // dlim_inventory_: the LLM's baked image the facade loads back
require strings
require daslib/strings_boost   // join
require dasllama/dasllama_math   // with_dasllama_jobque_: the forward runs on the engine's configured pool
require daslib/fio
require daslib/math_bits   // float_bits_to_uint: the synthetic files' f32 words
require _model_tier
require _json_file
require _npy   // read_npy: the oracle's per-stage dumps
require _compares   // check_rows / check_exact / check_exceeds / ids_equal
require _nmt_oracle   // the build's device translation seat, nmt_log_side

// The Bergamot students against marian's own f32 graph (`../benchmarks/nmt/bergamot_oracle.py`, this repository's oracle rig,
// writes the model root's `nmt_oracle/<pair>-<tier>/<src>_<variant>/`): the tokenizer id for id against Python sentencepiece
// over the fixture lines and the normalizer fixture, the exact lane stage by stage under the oracle's own tokens (the forced
// feed) and its free run by a line floor, the q8 lane by its lines' agreement with the f32 oracle, the device seat against the
// CPU q8 chain; the loader's refusals over synthetic Marian and SentencePiece files written to the test's temp directory; the
// facade's contracts on a shell and on a synthetic student, then on a translation LLM and two chat models.
// Skips: a cell skips without its model (the five `bergamot/<dir>/` students, `milmmt/MiLMMT-46-1B-v0.1.Q4_K_M.gguf`,
// `Qwen3-4B-Instruct-2507-Q4_K_M.gguf`, `Qwen3.5-0.8B-Q8_0.gguf`) or its dumps; the forward cells when interpreted
// (`jit_model_available`); the device cell on a build with no device translation seat or a box with no device for it -
// Metal on a das_metal build, Vulkan on a build without das_metal under DASLLAMA_GPU=1 with a device the tier serves.
// The facade loads (`load_translator`) bake a `.dlim` beside the model - ledgered at
// `../ARCHITECTURE_IMAGE_LEDGER.md#suite-image-mints`; every other load mints in memory.

//! The exact lane's stage bar: max |got - want| over the stage's largest magnitude, the decoder's stages under the oracle's
//! own tokens (reads 4e-6 to 6e-6 on the M5 Max, `run.das -- --area translate`, -jit, the m5 profile).
let private STAGE_BAR = 1e-4
//! The exact lane's free-run floor of lines translated token for token as the oracle's (reads 30 of 30 on every student on the
//! M5 Max): marian's float graph and ours sum in other orders, so a near-tie may flip a line on another box's kernels.
let private EXACT_LINES_FLOOR = 28
//! The device seat's floor of lines translated token for token as the CPU q8 chain (reads 28 of 30 on ru-en on Metal, 28 and 29
//! on Vulkan: the f16 attention keys and half GEMM panels flip a near-tie on the rest).
let private DEVICE_LINES_FLOOR = 26
//! The q8 lane's floor of lines translated token for token as the f32 oracle (reads 26 to 27 of 30 on the ru/en pairs; Firefox's own
//! int8 reads 20 to 23).
let private Q8_LINES_FLOOR = 25

struct private NmtCase {
    dir : string   //!< under `bergamot/` and `nmt_oracle/`
    pair : string
    src : string
}

let private CASES = [NmtCase(dir = "ru-en-base-memory", pair = "ruen", src = "ru"),
                     NmtCase(dir = "en-ru-base-memory", pair = "enru", src = "en"),
                     NmtCase(dir = "en-ru-base", pair = "enru", src = "en"),
                     NmtCase(dir = "ru-en-tiny", pair = "ruen", src = "ru"),
                     NmtCase(dir = "en-de-base-memory", pair = "ende", src = "en")]

struct private OracleTokens {
    src : string
    src_ids : array<int64>
    ids : array<int64>
}

struct private EdgeLine {
    text : string
    ids : array<int64>
}

struct private EdgeFile {
    lines : array<EdgeLine>
}

def private model_path(c : NmtCase) : string => path_join(models_dir(), "bergamot/{c.dir}/model.{c.pair}.intgemm.alphas.bin")

def private oracle_dir(c : NmtCase) : string => path_join(models_dir(), "nmt_oracle/{c.dir}")

def private line_dir(c : NmtCase; lane : string; li : int) : string => "{oracle_dir(c)}/{c.src}_{lane}/line_{li < 10 ? "0" : ""}{li}"

def private ru_en_pair(c : NmtCase) : bool => c.dir == "ru-en-base-memory" || c.dir == "en-ru-base-memory"

//! The oracle's lines of one variant, in order; empty when the dumps are absent.
def private oracle_lines(c : NmtCase; lane : string) : array<OracleTokens> {
    var out : array<OracleTokens>
    for (li in range(100)) {
        var tok : OracleTokens
        break if (!load_json_file("{line_dir(c, lane, li)}/tokens.json", tok))
        out |> emplace(tok)
    }
    return <- out
}

def private dumps_present(t : T?; c : NmtCase; lines : array<OracleTokens>) : bool {
    if (empty(lines)) {
        t |> skip("{c.dir}: oracle dumps not present")
        return false
    }
    return true
}

def private mint(path : string; q8 : bool) : NmtModel {
    var m = NmtModel()
    var inscope st <- stage_nmt_model(path, q8)
    mint_nmt_model(st, m)
    return <- m
}

//! the oracle's target ids with the end token dropped: what a translation answers
def private oracle_target(m : NmtModel; ln : OracleTokens) : array<int64> => <- [for (v in ln.ids); v; where v != m.eos_id]

//! every line of `srcs` through the CPU chain alone, no tap
def private cpu_lines(m : NmtModel; var st : NmtState; srcs : array<array<int64>>) : array<array<int64>> => <- [for (s in srcs);
    nmt_translate_ids_cpu(m, st, s) $ [unused_argument(stage, x, rows)] (stage : string; x : array<float>; rows : int64) {}]

//! how many lines of `got` are the oracle's token for token, each pair logged both sides decoded
def private lines_agreeing(m : NmtModel; tag : string; got : array<array<int64>>; lines : array<OracleTokens>) : int {
    var same = 0
    for (li, g, ln in count(), got, lines) {
        var inscope want <- oracle_target(m, ln)
        same += ids_equal(g, want) ? 1 : 0
        nmt_log_side(m, "{tag} line {li}", "ours", g)
        nmt_log_side(m, "{tag} line {li}", "oracle", want)
    }
    to_log(LOG_INFO, "{tag}: {same}/{length(lines)} lines token for token the f32 oracle's\n")
    return same
}

//! each line's source swapped for the next line's: the input control a line floor must fall under
def private displaced_sources(lines : array<OracleTokens>) : array<array<int64>> {
    let n = length(lines)
    return <- [for (li in range(n)); clone_to_move(lines[(li + 1) % n].src_ids)]
}

[test]
def test_nmt_spm(t : T?) {
    t |> run("the model-file tokenizer spells every fixture line as Python sentencepiece does") @(t : T?) {
        for (c in CASES) {
            let path = model_path(c)
            continue if (!model_available(t, path))
            var inscope lines <- oracle_lines(c, "f32")
            var edge : EdgeFile
            if (empty(lines) || !load_json_file("{oracle_dir(c)}/spm_edge.json", edge)) {
                t |> skip("{c.dir}: oracle dumps not present")
                continue
            }
            var inscope sp <- load_spm_model_file(nmt_vocab_path(path))
            for (ln in lines) {
                var inscope ids <- spm_file_encode(sp, ln.src)
                ids |> push(sp.eos_id)
                t |> success(ids_equal(ids, ln.src_ids), "{c.dir}: '{ln.src}' ours {ids} sentencepiece {ln.src_ids}")
            }
            for (e in edge.lines) {
                var inscope ids <- spm_file_encode(sp, e.text)
                t |> success(ids_equal(ids, e.ids), "{c.dir}: edge '{e.text}' ours {ids} sentencepiece {e.ids}")
            }
            sp.norm.charsmap |> clear()
            sp.norm.trie_units = 0
            var missed = 0
            for (e in edge.lines) {
                var inscope ids <- spm_file_encode(sp, e.text)
                missed += ids_equal(ids, e.ids) ? 0 : 1
            }
            t |> success(missed > 0, "{c.dir}: the control - with the charsmap dropped the normalizer fixture spells differently ({missed} lines)")
        }
    }
}

//! a tapped stage's rows appended to its run in `got`, in tap order (the encoder's once, the decoder's a step)
def private gather_stage(var got : table<string; array<float>>; m : NmtModel; stage : string; x : array<float>; rows : int64) {
    let n = rows * (stage == "logits" ? m.n_vocab : m.d)
    if (!key_exists(got, stage)) {
        var fresh : array<float>
        got |> emplace(stage, fresh)
    }
    unsafe(got[stage]) |> push_from(copy_span(x, 0l, n))
}

//! One line through the exact lane under the oracle's own tokens: the encode, then a step a target id with the oracle's previous
//! id fed (the forced feed), every stage gathered; answers each step's argmax.
def private forced_rows(m : NmtModel; var st : NmtState; src, ids : array<int64>; var got : table<string; array<float>>) : array<int64> {
    nmt_encode(m, st, src) $(stage : string; x : array<float>; rows : int64) {
        gather_stage(got, m, stage, x, rows)
    }
    var picks : array<int64>
    var prev = -1l
    for (step, id in count(), ids) {
        picks |> push(nmt_decode_step(m, st, prev, int64(step)) $(stage : string; x : array<float>; rows : int64) {
            gather_stage(got, m, stage, x, rows)
        })
        prev = id
    }
    return <- picks
}

//! the encoder's last layer over `src`: the stage the displaced-token control holds outside the bar
def private last_encoder_rows(m : NmtModel; var st : NmtState; src : array<int64>) : array<float> {
    var inscope got : table<string; array<float>>
    nmt_encode(m, st, src) $(stage : string; x : array<float>; rows : int64) {
        gather_stage(got, m, stage, x, rows)
    }
    return <- clone_to_move(got?["enc_l{m.n_enc}"] ?? default<array<float>>)
}

[test]
def test_nmt_exact_lane(t : T?) {
    t |> run("the exact lane holds marian's f32 graph stage by stage under its own tokens, and its free run by a line floor") @(t : T?) {
        with_dasllama_jobque_() {
            let bar = Bar(kind = BarKind.rel_of_max, rel = STAGE_BAR, poison = 1.0, poison_of_max = true)
            for (c in CASES) {
                let path = model_path(c)
                continue if (!jit_model_available(t, path))
                var inscope lines <- oracle_lines(c, "f32")
                continue if (!dumps_present(t, c, lines))
                var inscope m <- mint(path, false)
                t |> equal(m.cls_off, -1l, "{c.dir}: the exact lane carries no Q8_0 classifier copy")
                var inscope st = NmtState()
                var forced_same = 0
                for (li, ln in count(), lines) {
                    var inscope got : table<string; array<float>>
                    var inscope picks <- forced_rows(m, st, ln.src_ids, ln.ids, got)
                    forced_same += ids_equal(picks, ln.ids) ? 1 : 0
                    for (stage, rows in keys(got), values(got)) {
                        let ref <- read_npy("{line_dir(c, "f32", li)}/{stage}.npy")
                        t |> equal(long_length(rows), ref.rows * ref.cols, "{c.dir} line {li} {stage}: the element count")
                        check_rows(t, "{c.dir} line {li} {stage}", rows, ref.data, bar, int(ref.cols))
                    }
                }
                to_log(LOG_INFO, "{c.dir} exact forced feed: {forced_same}/{length(lines)} lines whose every step's argmax is the oracle's token\n")
                var inscope moved := lines[0].src_ids
                moved[0] = (moved[0] + 1l) % m.n_vocab
                var inscope enc_moved <- last_encoder_rows(m, st, moved)
                let ref <- read_npy("{line_dir(c, "f32", 0)}/enc_l{m.n_enc}.npy")
                check_exceeds(t, "{c.dir} line 0 enc_l{m.n_enc}: the control - a displaced source token", enc_moved, ref.data, bar)
                var inscope srcs <- [for (ln in lines); clone_to_move(ln.src_ids)]
                var inscope free <- cpu_lines(m, st, srcs)
                let same = lines_agreeing(m, "{c.dir} exact", free, lines)
                t |> success(same >= EXACT_LINES_FLOOR, "{c.dir}: {same} of {length(lines)} lines the oracle's, floor {EXACT_LINES_FLOOR}")
                var inscope off <- displaced_sources(lines)
                var inscope off_lines <- cpu_lines(m, st, off)
                let off_same = lines_agreeing(m, "{c.dir} exact displaced", off_lines, lines)
                t |> success(off_same < EXACT_LINES_FLOOR, "{c.dir}: the control - each line fed the next line's source agrees on {off_same}, under the floor")
            }
        }
    }
}

//! A q8 minted with its classifier's Q8_0 copy zeroed - quants and scales - so every step's logits are the output bias alone.
def private mint_cls_zeroed(path : string) : NmtModel {
    var m = NmtModel()
    var inscope st <- stage_nmt_model(path, true)
    let n = st.e.n_vocab * st.e.d
    for (i in range64(st.e.cls_off, st.e.cls_off + n)) {
        st.qblob[i] = int8(0)
    }
    for (i in range64(st.e.cls_off / 32l, (st.e.cls_off + n) / 32l)) {
        st.qscales[i] = 0.0
    }
    mint_nmt_model(st, m)
    return <- m
}

[test]
def test_nmt_q8_lane(t : T?) {
    t |> run("the q8 lane translates the ru/en fixtures close to marian's f32 graph, its classifier off the Q8_0 copy") @(t : T?) {
        with_dasllama_jobque_() {
            for (c in CASES) {
                continue if (!ru_en_pair(c))
                let path = model_path(c)
                continue if (!jit_model_available(t, path))
                var inscope lines <- oracle_lines(c, "f32")
                continue if (!dumps_present(t, c, lines))
                var inscope m <- mint(path, true)
                t |> success(m.q8 && !empty(m.qblob) && m.cls_off >= 0l, "{c.dir}: the GEMMs and the classifier serve from the q8 plane")
                var inscope st = NmtState()
                var inscope srcs <- [for (ln in lines); clone_to_move(ln.src_ids)]
                var inscope got <- cpu_lines(m, st, srcs)
                let same = lines_agreeing(m, "{c.dir} q8", got, lines)
                t |> success(same >= Q8_LINES_FLOOR, "{c.dir}: {same} of {length(lines)} lines the f32 oracle's, floor {Q8_LINES_FLOOR}")
                var inscope mz <- mint_cls_zeroed(path)
                var inscope zst = NmtState()
                var inscope zgot <- cpu_lines(mz, zst, srcs)
                var moved = 0
                for (li, a, z in count(), got, zgot) {
                    moved += ids_equal(a, z) ? 0 : 1
                    if (li == 0) {
                        nmt_log_side(m, "{c.dir} q8 line 0", "the q8 lane", a)
                        nmt_log_side(m, "{c.dir} q8 line 0", "its Q8_0 classifier zeroed", z)
                    }
                }
                t |> equal(moved, length(lines), "{c.dir}: the q8 lane's logits come off the Q8_0 copy - zeroed, every line moves")
                let zsame = lines_agreeing(mz, "{c.dir} q8 cls zeroed", zgot, lines)
                t |> success(zsame < Q8_LINES_FLOOR, "{c.dir}: the control - the classifier zeroed agrees on {zsame}, under the floor")
            }
        }
    }
}

//! A device leg: every source through the seat, the counters' rise asserted and the lines held to the CPU q8 chain's.
def private device_leg(t : T?; tag : string; m : NmtModel; var st : NmtState; srcs, cpu : array<array<int64>>) {
    let on0 = nmt_seat_stats()
    var inscope gpu <- [for (s in srcs); nmt_translate_ids(m, st, s)]
    let on1 = nmt_seat_stats()
    t |> equal(on1.sentences - on0.sentences, long_length(srcs), "{tag}: every line served on the device")
    t |> equal(on1.declines, on0.declines, "{tag}: no device decline")
    var same = 0
    for (li, a, g in count(), cpu, gpu) {
        same += ids_equal(a, g) ? 1 : 0
        nmt_log_side(m, "{tag} line {li}", "cpu q8", a)
        nmt_log_side(m, "{tag} line {li}", nmt_seat_name(), g)
    }
    to_log(LOG_INFO, "{tag}: {same}/{length(srcs)} lines the CPU q8 chain's\n")
    t |> success(same >= DEVICE_LINES_FLOOR, "{tag}: {same} of {length(srcs)} lines the CPU q8 chain's, floor {DEVICE_LINES_FLOOR}")
}

//! a synthetic d 32 student through the seat: off the 64-wide GEMM tiles, so declined `shape` and translated on the CPU
def private device_shape_decline(t : T?) {
    let pinned = nmt_seat_pin(true)
    var inscope v <- unigram_vocab()
    var inscope off_lattice <- mint(syn_student("device_shape", "zz", SynSpec(), "vocab.zz.spm", v), true)
    var inscope sst = NmtState()
    let sh0 = nmt_seat_stats()
    let shape0 = nmt_seat_declines("shape")
    var inscope sids <- nmt_translate_ids(off_lattice, sst, [2l, 3l, 2l, 1l])
    t |> equal(nmt_seat_stats().sentences, sh0.sentences, "a d {SYN_D} student is not served on the device")
    t |> success(nmt_seat_declines("shape") > shape0, "a width off the 64-wide GEMM tiles declines `shape`")
    t |> success(length(sids) <= 8, "and translates on the CPU within its length cap ({length(sids)} tokens)")
    nmt_seat_pin(pinned)
}

//! each model attached again (a swap back), its device classifier zeroed: every line then differs from the CPU q8 chain's
def private device_poison_controls(t : T?; models : array<NmtModel>; srcs, cpu : array<array<array<int64>>>; tags : array<string>;
                                   var st : NmtState) {
    for (k, m in count(), models) {
        var inscope attach <- nmt_translate_ids(m, st, srcs[k][0])
        nmt_seat_poison(m)
        var poisoned = 0
        for (li, s, a in count(), srcs[k], cpu[k]) {
            var inscope g <- nmt_translate_ids(m, st, s)
            poisoned += ids_equal(a, g) ? 0 : 1
            if (li == 0) {
                nmt_log_side(m, "{tags[k]} line 0", "the device before the poison", attach)
                nmt_log_side(m, "{tags[k]} line 0", "cpu q8", a)
                nmt_log_side(m, "{tags[k]} line 0", "device classifier zeroed", g)
            }
        }
        t |> equal(poisoned, length(srcs[k]), "{tags[k]}: the control - with the device classifier zeroed every line differs")
    }
}

//! a scratch take past the device's largest buffer declining `device` (where the seat reads that limit), a source past
//! `NMT_DEVICE_SRC_CAP` declining `length`, then the exact lane of `exact_path` declining `quant_mode`; all translate on the CPU
def private device_declines(t : T?; tag : string; m : NmtModel; var st : NmtState; src : array<int64>; exact_path : string) {
    let probe = nmt_seat_take_max_probe(512ul)
    if (probe.seam) {
        let sh0 = nmt_seat_stats()
        let d0 = nmt_seat_declines("device")
        var inscope short_ids <- nmt_translate_ids(m, st, src)
        nmt_seat_take_max_probe(probe.was)
        t |> equal(nmt_seat_stats().sentences, sh0.sentences, "{tag}: a short scratch take is not served on the device")
        t |> success(nmt_seat_declines("device") > d0, "{tag}: a scratch take past the device's largest buffer declines `device`")
        t |> success(!empty(short_ids), "{tag}: and translates on the CPU")
    }
    var long_src <- [for (i in range64(NMT_DEVICE_SRC_CAP + 1l)); src[i % (long_length(src) - 1l)]]
    long_src |> push(m.eos_id)
    let len0 = nmt_seat_stats()
    let l0 = nmt_seat_declines("length")
    var inscope long_ids <- nmt_translate_ids(m, st, long_src)
    t |> equal(nmt_seat_stats().sentences, len0.sentences, "{tag}: a {length(long_src)}-token source is not served on the device")
    t |> success(nmt_seat_declines("length") > l0, "{tag}: a source past {NMT_DEVICE_SRC_CAP} tokens declines `length`")
    t |> success(!empty(long_ids), "{tag}: and translates on the CPU")
    nmt_seat_shutdown()
    var inscope ex <- mint(exact_path, false)
    let ex0 = nmt_seat_stats()
    let q0 = nmt_seat_declines("quant_mode")
    var inscope ids <- nmt_translate_ids(ex, st, src)
    t |> equal(nmt_seat_stats().sentences, ex0.sentences, "{tag}: the exact lane is not served on the device")
    t |> success(nmt_seat_declines("quant_mode") > q0, "{tag}: the exact lane declines by name")
    t |> success(!empty(ids), "{tag}: the exact lane translates on the CPU")
    nmt_seat_shutdown()
}

[test]
def test_nmt_device(tst : T?) {
    tst |> run("the build's device seat translates the ru/en fixtures as the CPU q8 chain does, across a model swap with no shutdown") @(t : T?) {
        return if (nmt_seat_skip(t, "nmt device"))
        with_dasllama_jobque_() {
            device_shape_decline(t)
            var models : array<NmtModel>
            var inscope srcs, cpu : array<array<array<int64>>>
            var tags, paths : array<string>
            let was = nmt_seat_pin(false)
            for (c in CASES) {
                continue if (!ru_en_pair(c))
                let path = model_path(c)
                continue if (!jit_model_available(t, path))
                var inscope lines <- oracle_lines(c, "f32")
                continue if (!dumps_present(t, c, lines))
                var mm <- mint(path, true)
                var inscope st = NmtState()
                var cs <- [for (ln in lines); clone_to_move(ln.src_ids)]
                let off0 = nmt_seat_stats()
                var cl <- cpu_lines(mm, st, cs)
                t |> equal(nmt_seat_stats().sentences, off0.sentences, "{c.dir}: the seat pinned off serves no sentence on the device")
                models |> emplace(mm)
                srcs |> emplace(cs)
                cpu |> emplace(cl)
                tags |> push("{c.dir} {nmt_seat_name()}")
                paths |> push(path)
            }
            if (!empty(models)) {
                nmt_seat_pin(true)
                var inscope st = NmtState()
                for (k, m in count(), models) {
                    device_leg(t, k == 0 ? tags[k] : "{tags[k]} (swapped in, no shutdown)", m, st, srcs[k], cpu[k])
                }
                device_poison_controls(t, models, srcs, cpu, tags, st)
                device_declines(t, tags[0], models[0], st, srcs[0][0], paths[0])
            }
            nmt_seat_pin(was)
            unsafe {
                delete models
            }
        }
    }
}

// ===== synthetic files: a SentencePiece vocabulary and a Marian student written to the test's temp directory =====

def private put_varint(var b : array<uint8>; v : uint64) {
    var x = v
    while (x >= 0x80ul) {
        b |> push(uint8((x & 0x7Ful) | 0x80ul))
        x >>= 7ul
    }
    b |> push(uint8(x))
}

def private put_key(var b : array<uint8>; num, wire : int) {
    put_varint(b, uint64((num << 3) | wire))
}

def private put_u32(var b : array<uint8>; v : uint) {
    b |> reserve(length(b) + 4)
    for (k in range(4)) {
        b |> push(uint8((v >> uint(8 * k)) & 0xFFu))
    }
}

def private put_u64(var b : array<uint8>; v : uint64) {
    b |> reserve(length(b) + 8)
    for (k in range(8)) {
        b |> push(uint8((v >> uint64(8 * k)) & 0xFFul))
    }
}

def private put_text(var b : array<uint8>; s : string) {
    peek_data(s) $(d) {
        b |> reserve(length(b) + length(d))
        for (c in d) {
            b |> push(c)
        }
    }
}

def private pb_bytes(var b : array<uint8>; num : int; payload : array<uint8>) {
    put_key(b, num, 2)
    put_varint(b, uint64(long_length(payload)))
    b |> push_from(payload)
}

def private pb_uint(var b : array<uint8>; num : int; v : uint64) {
    put_key(b, num, 0)
    put_varint(b, v)
}

//! One `SentencePiece` message: the piece, its score, its type (1 normal, 2 unknown, 3 control).
def private pb_piece_msg(piece : string; score : float; kind : int) : array<uint8> {
    var m, p : array<uint8>
    put_text(p, piece)
    pb_bytes(m, 1, p)
    put_key(m, 2, 5)
    put_u32(m, float_bits_to_uint(score))
    pb_uint(m, 3, uint64(kind))
    return <- m
}

//! A ModelProto of four pieces - `<unk>` (id 0), `</s>` (id 1), `▁a`, `b` - and a trainer spec of `model_type`
//! (1 unigram, 2 BPE); `tail` is appended to the trainer spec's fields.
def private spm_bytes(model_type : int; tail : array<uint8>) : array<uint8> {
    var b : array<uint8>
    pb_bytes(b, 1, pb_piece_msg("<unk>", 0.0, 2))
    pb_bytes(b, 1, pb_piece_msg("</s>", 0.0, 3))
    pb_bytes(b, 1, pb_piece_msg("▁a", -1.0, 1))   // nolint:STYLE039 (U+2581 is the SentencePiece space marker - vocabulary data, not prose)
    pb_bytes(b, 1, pb_piece_msg("b", -2.0, 1))
    var ts : array<uint8>
    pb_uint(ts, 3, uint64(model_type))
    pb_uint(ts, 40, 0ul)
    pb_uint(ts, 42, 1ul)
    ts |> push_from(tail)
    pb_bytes(b, 2, ts)
    var ns : array<uint8>
    pb_uint(ns, 3, 1ul)
    pb_uint(ns, 4, 1ul)
    pb_bytes(b, 3, ns)
    return <- b
}

let private SYN_VOCAB = 4
let private SYN_D = 32
let private SYN_FF = 32
let private INTGEMM8 = 0x4101ul
let private FLOAT32 = 0x0404ul
let private INT8 = 0x0101ul

//! One item of a synthetic Marian file: its name, type, shape and bytes.
struct private SynItem {
    name : string
    kind : uint64
    shape : array<int>
    data : array<uint8>
}

//! the int8 value of element `i` of the item salted `salt`, off zero and inside the int8 range
def private syn_q(i, salt : int) : int => ((i * 37 + salt * 101) % 251) - 125

//! An [out][in] GEMM: intgemm8 [out][in] int8 over a multiplier of 2, or its float32 twin [in][out] holding each int8 / 2;
//! `untransposed` lays the twin's floats [out][in] - the control a transpose-blind compare would miss.
def private syn_gemm(name : string; nout, nin, salt : int; f32, untransposed : bool) : SynItem {
    var it = SynItem(name = name)
    if (!f32) {
        it.kind = INTGEMM8
        it.shape <- [nout, nin]
        it.data |> reserve(nout * nin + 4)
        for (i in range(nout * nin)) {
            it.data |> push(uint8(syn_q(i, salt) & 0xFF))
        }
        put_u32(it.data, float_bits_to_uint(2.0))
        return <- it
    }
    it.kind = FLOAT32
    it.shape <- [nin, nout]
    for (r in range(nin)) {
        for (c in range(nout)) {
            let i = untransposed ? r * nout + c : c * nin + r
            put_u32(it.data, float_bits_to_uint(float(syn_q(i, salt)) * 0.5))
        }
    }
    return <- it
}

//! A [1][n] float32 row.
def private syn_row(name : string; n, salt : int) : SynItem {
    var it = SynItem(name = name, kind = FLOAT32)
    it.shape <- [1, n]
    for (i in range(n)) {
        put_u32(it.data, float_bits_to_uint(float(syn_q(i, salt)) * 0.01))
    }
    return <- it
}

//! The embedded config, `swap_key`'s value replaced by `swap_value` (none for an empty key).
def private syn_yml(d : int; swap_key, swap_value : string) : string {
    let keys <- ["type", "transformer-decoder-autoreg", "dec-cell", "transformer-postprocess", "transformer-preprocess",
                 "tied-embeddings-all", "transformer-ffn-activation", "layer-normalization", "dim-emb", "transformer-dim-ffn",
                 "transformer-heads", "enc-depth", "dec-depth"]
    let vals <- ["transformer", "rnn", "ssru", "dan", "\"\"", "true", "relu", "false", "{d}", "{SYN_FF}", "1", "1", "1"]
    return build_string() $(var w) {
        for (k, v in keys, vals) {
            w |> write("{k}: {k == swap_key ? swap_value : v}\n")
        }
    }
}

//! What a synthetic student varies: the GEMMs' form, the embedding's rows and width, one yml value, an item dropped or grown.
struct private SynSpec {
    f32 : bool
    untransposed : bool
    vocab : int = SYN_VOCAB
    d : int = SYN_D
    emb_d : int = SYN_D
    emb_kind : uint64 = 0ul       //!< 0: the GEMMs' form
    swap_key : string
    swap_value : string
    no_yml : bool
    drop : string                 //!< an item left out
    grow : string                 //!< a row item one element long
    short_mult : bool             //!< the embedding's intgemm8 bytes missing their multiplier
}

//! One layer's GEMMs and rows under `prefix`, each salted on from `salt`.
def private syn_layer(var items : array<SynItem>; prefix : string; gs : array<tuple<string; int; int>>; rs : array<tuple<string; int>>;
                      var salt : int&; spec : SynSpec) {
    items |> reserve(length(items) + length(gs) + length(rs))
    for (g in gs) {
        items |> emplace(syn_gemm("{prefix}_{g._0}", g._1, g._2, salt++, spec.f32, spec.untransposed))
    }
    for (r in rs) {
        let name = "{prefix}_{r._0}"
        items |> emplace(syn_row(name, name == spec.grow ? r._1 + 1 : r._1, salt++))
    }
}

//! Every item of a one-encoder-layer, one-decoder-layer student at `spec`.
def private syn_items(spec : SynSpec) : array<SynItem> {
    let d = spec.d
    let ff = SYN_FF
    var items : array<SynItem>
    if (!spec.no_yml) {
        var yml = SynItem(name = "special:model.yml", kind = INT8)
        put_text(yml.data, syn_yml(d, spec.swap_key, spec.swap_value))
        yml.data |> push(uint8(0))
        yml.shape <- [length(yml.data)]
        items |> emplace(yml)
    }
    var emb <- syn_gemm("Wemb", spec.vocab, spec.emb_d, 1, spec.f32, true)
    if (spec.f32) {
        emb.shape <- [spec.vocab, spec.emb_d]
    }
    if (spec.emb_kind != 0ul) {
        emb.kind = spec.emb_kind
    }
    if (spec.short_mult) {
        emb.data |> resize(spec.vocab * spec.emb_d)
    }
    items |> emplace(emb)
    items |> emplace(syn_row("decoder_ff_logit_out_b", spec.vocab, 2))
    var salt = 3
    let enc_g <- [("self_Wq", d, d), ("self_Wk", d, d), ("self_Wv", d, d), ("self_Wo", d, d), ("ffn_W1", ff, d), ("ffn_W2", d, ff)]
    let enc_r <- [("self_bq", d), ("self_bk", d), ("self_bv", d), ("self_bo", d), ("self_Wo_ln_scale", d), ("self_Wo_ln_bias", d),
                  ("ffn_b1", ff), ("ffn_b2", d), ("ffn_ffn_ln_scale", d), ("ffn_ffn_ln_bias", d)]
    let dec_g <- [("rnn_W", d, d), ("rnn_Wf", d, d), ("context_Wq", d, d), ("context_Wk", d, d), ("context_Wv", d, d),
                  ("context_Wo", d, d), ("ffn_W1", ff, d), ("ffn_W2", d, ff)]
    let dec_r <- [("rnn_bf", d), ("rnn_ffn_ln_scale", d), ("rnn_ffn_ln_bias", d), ("context_bq", d), ("context_bk", d),
                  ("context_bv", d), ("context_bo", d), ("context_Wo_ln_scale", d), ("context_Wo_ln_bias", d), ("ffn_b1", ff),
                  ("ffn_b2", d), ("ffn_ffn_ln_scale", d), ("ffn_ffn_ln_bias", d)]
    syn_layer(items, "encoder_l1", enc_g, enc_r, salt, spec)
    syn_layer(items, "decoder_l1", dec_g, dec_r, salt, spec)
    return <- [for (it in items); it; where it.name != spec.drop]
}

//! The Marian binary of `items`: version, count, four u64 an item, the names, the shapes, a pad to 256, the blobs;
//! `claim_extra` bytes claimed past the last blob, `name_extra` past the first name, `version` in the version word.
def private marian_bytes(items : array<SynItem>; version : uint64 = 1ul; claim_extra : int = 0; name_extra : int = 0) : array<uint8> {
    var b : array<uint8>
    put_u64(b, version)
    put_u64(b, uint64(long_length(items)))
    for (i, it in count(), items) {
        put_u64(b, uint64(length(it.name) + 1 + (i == 0 ? name_extra : 0)))
        put_u64(b, it.kind)
        put_u64(b, uint64(long_length(it.shape)))
        put_u64(b, uint64(length(it.data) + (i == length(items) - 1 ? claim_extra : 0)))
    }
    b |> reserve(length(b) + length(items) * 64)
    for (it in items) {
        put_text(b, it.name)
        b |> push(uint8(0))
    }
    for (it in items) {
        for (s in it.shape) {
            put_u32(b, uint(s))
        }
    }
    let pad = (256 - (length(b) + 8) % 256) % 256
    put_u64(b, uint64(pad))
    b |> reserve(length(b) + pad)
    for (_i in range(pad)) {
        b |> push(uint8(0))
    }
    for (it in items) {
        b |> push_from(it.data)
    }
    return <- b
}

def private write_bytes(path : string; b : array<uint8>) {
    let f = fopen(path, "wb")
    fwrite(f, b)
    fclose(f)
}

//! A student's directory under the temp directory: `model.<pair>.intgemm.alphas.bin` and its vocabulary (`vocab_name`, "" for
//! none) beside it; answers the model's path.
def private syn_student(dir, pair : string; spec : SynSpec; vocab_name : string; vocab : array<uint8>) : string {
    let root = path_join(test_temp_dir(), dir)
    mkdir_rec(root)
    if (!empty(vocab_name)) {
        write_bytes(path_join(root, vocab_name), vocab)
    }
    var inscope items <- syn_items(spec)
    let path = path_join(root, "model.{pair}.intgemm.alphas.bin")
    var inscope b <- marian_bytes(items)
    write_bytes(path, b)
    return path
}

def private unigram_vocab : array<uint8> {
    let none : array<uint8>
    return <- spm_bytes(1, none)
}

[test]
def test_nmt_float32_student(t : T?) {
    t |> run("a float32 Marian student widens to the planes its intgemm8 twin does, every [in][out] GEMM transposed") @(t : T?) {
        var inscope v <- unigram_vocab()
        let a = syn_student("twin_int8", "zz", SynSpec(), "vocab.zz.spm", v)
        let b = syn_student("twin_f32", "zz", SynSpec(f32 = true), "vocab.zz.spm", v)
        let c = syn_student("twin_f32_flat", "zz", SynSpec(f32 = true, untransposed = true), "vocab.zz.spm", v)
        var inscope sa <- stage_nmt_model(a, false)
        var inscope sb <- stage_nmt_model(b, false)
        var inscope sc <- stage_nmt_model(c, false)
        t |> equal(sa.e.n_vocab, int64(SYN_VOCAB), "the twins' vocabulary")
        check_exact(t, "the float32 twin's f32 planes against the intgemm8 file's", sb.blob, sa.blob)
        let off = mismatch_exact(sc.blob, sa.blob)
        to_log(LOG_INFO, "the control - the float32 GEMMs laid [out][in]: {off} of {length(sa.blob)} elements differ, the first at {first_mismatch(sc.blob, sa.blob)}\n")
        t |> success(off > 0, "the control - a twin whose GEMMs are not transposed widens to other planes ({off} elements)")
        var inscope q <- stage_nmt_model(a, true)
        t |> success(q.e.cls_off >= 0l && long_length(q.qblob) == q.e.cls_off + q.e.n_vocab * q.e.d, "the q8 lane lays the classifier's Q8_0 copy after the GEMMs")
    }
}

//! the panic `stage_nmt_model` raises on `path` ("" where it stages)
def private stage_panic(path : string) : string {
    return panic_text() $ {
        var inscope st <- stage_nmt_model(path, false)
    }
}

[test]
def test_nmt_loader_refusals(t : T?) {
    t |> run("the student loader refuses each malformed file by name") @(t : T?) {
        var inscope v <- unigram_vocab()
        let ok = syn_student("good", "zz", SynSpec(), "vocab.zz.spm", v)
        t |> equal(stage_panic(ok), "", "the control - the well-formed student stages")
        let cases <- [("type", "s2s", "model type 's2s', not transformer"),
                      ("dec-cell", "lstm", "the decoder is not an SSRU student"),
                      ("transformer-postprocess", "da", "the layers are not post-norm"),
                      ("tied-embeddings-all", "false", "the embeddings are not tied"),
                      ("transformer-ffn-activation", "swish", "the FFN activation is not relu"),
                      ("layer-normalization", "true", "the SSRU carries layer norms"),
                      ("transformer-heads", "3", "hparams d 32 ff 32 heads 3")]
        for (i, cs in count(), cases) {
            let p = syn_student("yml_{i}", "zz", SynSpec(swap_key = cs._0, swap_value = cs._1), "vocab.zz.spm", v)
            let why = stage_panic(p)
            t |> success(find(why, cs._2) >= 0, "{cs._0}: {cs._1} refused: '{why}'")
        }
        let specs <- [(SynSpec(no_yml = true), "no special:model.yml item"),
                      (SynSpec(emb_d = 64), "no [vocab][32] Wemb"),
                      (SynSpec(d = 48, emb_d = 48), "d 48 / ff 32 off the Q8_0 lattice"),
                      (SynSpec(vocab = 5), "the model's 5 embeddings and the vocabulary's 4 pieces"),
                      (SynSpec(emb_kind = INT8), "'Wemb' has Marian type 0x101, neither float32 nor intgemm8"),
                      (SynSpec(drop = "encoder_l1_ffn_b2"), "no item 'encoder_l1_ffn_b2'"),
                      (SynSpec(grow = "decoder_l1_context_bq"), "'decoder_l1_context_bq' holds 33 elements, the layout 32"),
                      (SynSpec(short_mult = true), "'Wemb' holds 128 bytes, its int8 shape 128 and a multiplier")]
        for (i, cs in count(), specs) {
            let p = syn_student("spec_{i}", "zz", cs._0, "vocab.zz.spm", v)
            let why = stage_panic(p)
            t |> success(find(why, cs._1) >= 0, "'{cs._1}' refused: '{why}'")
        }
        var inscope items <- syn_items(SynSpec())
        let root = path_join(test_temp_dir(), "framing")
        mkdir_rec(root)
        write_bytes(path_join(root, "vocab.zz.spm"), v)
        let p = path_join(root, "model.zz.intgemm.alphas.bin")
        for (fr in [(2ul, 0, 0, "not a Marian binary model (version 0x2)"),
                    (1ul, 64, 0, "the item blobs end at"),
                    (1ul, 0, 100000000, "item 0's name runs past the file")]) {
            var inscope b <- marian_bytes(items, fr._0, fr._1, fr._2)
            write_bytes(p, b)
            let why = stage_panic(p)
            t |> success(find(why, fr._3) >= 0, "'{fr._3}' refused: '{why}'")
        }
        let split = syn_student("split", "zz", SynSpec(), "srcvocab.zz.spm", v)
        t |> equal(nmt_vocab_path(split), path_join(dir_name(split), "srcvocab.zz.spm"), "a split pair's source vocabulary is the one found")
        t |> equal(nmt_vocab_path(ok), path_join(dir_name(ok), "vocab.zz.spm"), "a shared vocabulary is the one found")
        let why_split = stage_panic(split)
        t |> success(find(why_split, "is a split-vocabulary pair") >= 0, "a split-vocabulary pair refused: '{why_split}'")
        let bare = syn_student("bare", "zz", SynSpec(), "", v)
        let why_bare = stage_panic(bare)
        t |> success(find(why_bare, "no vocabulary beside") >= 0, "a student with no vocabulary refused: '{why_bare}'")
        let none : array<uint8>
        var inscope bpe <- spm_bytes(2, none)
        let p_bpe = syn_student("bpe", "zz", SynSpec(), "vocab.zz.spm", bpe)
        let why_bpe = stage_panic(p_bpe)
        t |> success(find(why_bpe, "is not a unigram SentencePiece model") >= 0, "a BPE vocabulary refused: '{why_bpe}'")
        let gone = path_join(dir_name(ok), "model.zz.intgemm.alphas.bin.gone")
        write_bytes(path_join(dir_name(ok), "vocab.zz.spm"), v)
        let why_gone = stage_panic(gone)
        t |> success(find(why_gone, "no vocabulary beside") >= 0 || find(why_gone, "cannot open model") >= 0, "a missing model refused: '{why_gone}'")
    }
}

//! the panic `load_spm_model_file` raises on `bytes` written to the temp directory ("" where it loads)
def private spm_panic(name : string; bytes : array<uint8>) : string {
    let p = path_join(test_temp_dir(), name)
    write_bytes(p, bytes)
    return panic_text() $ {
        var inscope sp <- load_spm_model_file(p)
    }
}

[test]
def test_nmt_spm_file_refusals(t : T?) {
    t |> run("the SentencePiece model-file reader refuses a malformed protobuf by name, and skips a fixed64 field") @(t : T?) {
        var inscope good <- unigram_vocab()
        t |> equal(spm_panic("good.spm", good), "", "the control - the well-formed vocabulary loads")
        var fixed : array<uint8>
        put_key(fixed, 50, 1)
        put_u64(fixed, 0x0123456789ABCDEFul)
        var inscope with64 <- spm_bytes(1, fixed)
        let p64 = path_join(test_temp_dir(), "fixed64.spm")
        write_bytes(p64, with64)
        var inscope sp <- load_spm_model_file(p64)
        t |> equal(length(sp.tk.vocab), SYN_VOCAB, "a trainer spec carrying a fixed64 field: every piece read")
        t |> success(sp.unigram && sp.eos_id == 1l && sp.unk_id == 0l, "and the fields after it")
        var inscope cut := good
        cut |> push(uint8(0x08))
        cut |> push(uint8(0x80))
        let why_cut = spm_panic("cut.spm", cut)
        t |> success(find(why_cut, "a protobuf varint runs past the end of the model file") >= 0, "a varint cut off at the end: '{why_cut}'")
        var inscope wire3 := good
        put_key(wire3, 9, 3)
        let why_w3 = spm_panic("wire3.spm", wire3)
        t |> success(find(why_w3, "protobuf wire type 3") >= 0, "a group wire type: '{why_w3}'")
        var inner, long_text : array<uint8>
        put_text(long_text, "abcdefgh")
        pb_bytes(inner, 1, long_text)
        var past : array<uint8>
        put_key(past, 1, 2)
        put_varint(past, uint64(length(inner) - 4))
        past |> push_from(inner)
        let why_past = spm_panic("past.spm", past)
        t |> success(find(why_past, "a protobuf field runs past its message") >= 0, "a piece field longer than its message: '{why_past}'")
    }
}

[test]
def test_translate_facade_contracts(t : T?) {
    t |> run("the facade's direction, coverage and language calls on a shell of each kind and a synthetic student") @(t : T?) {
        var inscope llm = Translator(kind = TranslatorKind.llm)
        let why = panic_text() $ {
            translate(llm, "Где ближайшая аптека?")   // nolint:STYLE039 - the source language's own text
        }
        t |> success(find(why, "pick the direction with set_translator_langs") >= 0, "an LLM with no direction refuses to translate: '{why}'")
        t |> success(!set_translator_langs(llm, "ru", "ru"), "an LLM refuses a direction from a language to itself")
        t |> equal(translator_langs(llm).src, "", "and keeps its direction unpicked")
        t |> success(translator_serves(llm, "ru", "en") && translator_serves(llm, "zh-Hant", "yue"), "an LLM serves any two of its languages")
        t |> success(!translator_serves(llm, "ru", "ru"), "but not a language to itself")
        t |> success(!translator_serves(llm, "ru", "xx") && !translator_serves(llm, "xx", "ru"), "nor a code off its list")
        var inscope codes <- translator_languages(llm)
        t |> equal(length(codes), 46, "an LLM's languages: the card's 46")
        t |> equal(codes[0], "ar", "sorted, the first")
        t |> success(find_index(codes, "zh-Hant") >= 0 && find_index(codes, "ru") >= 0, "the codes the prompts spell")
        t |> equal(translator_language_name("ru"), "Russian", "a code's English name")
        t |> equal(translator_language_name("xx"), "xx", "an unknown code names itself")
        var inscope mt = Translator(kind = TranslatorKind.marian, src = "ru", trg = "en")
        var inscope two <- translator_languages(mt)
        t |> equal(join(two, ","), "ru,en", "a student's languages: its pair, source first")
        t |> success(translator_serves(mt, "ru", "en"), "a student serves its own pair")
        t |> success(!translator_serves(mt, "en", "ru") && !set_translator_langs(mt, "en", "ru"), "and not the way back")
        t |> equal(translator_langs(mt).src, "ru", "its direction unchanged by the refusal")
        var inscope v <- unigram_vocab()
        let path = syn_student("facade", "zz", SynSpec(), "vocab.zz.spm", v)
        with_dasllama_jobque_() {
            var inscope q <- load_translator(path)
            t |> equal(q.kind, TranslatorKind.marian, "a Marian file loads as a student")
            t |> success(q.m.q8, "on the q8 lane by default")
            t |> equal(translator_langs(q).src, "zz", "a pair name that is not two codes: the whole name is the source")
            t |> equal(translator_langs(q).trg, "", "and the target is unnamed")
            var inscope ex <- load_translator(path, true)
            t |> success(!ex.m.q8 && ex.m.cls_off == -1l, "exact = true serves the f32 lane")
        }
    }
}

//! The prompt the facade's completion form feeds MiLMMT-46 (the card's form), spelled here to run it with no line stop.
def private completion_prompt(src, trg, text : string) : string {
    let s = translator_language_name(src)
    let d = translator_language_name(trg)
    return "Translate this from {s} to {d}:\n{s}: {text}\n{d}:"
}

//! The same prompt's greedy continuation to the end token or `cap` tokens, with no stop at the end of the line.
def private raw_completion(var tr : Translator; prompt_text : string; cap : int64) : string {
    let params = SamplingParams()
    tr.ls.n_past = 0l
    let prompt <- encode_(tr.lm, prompt_text, true, false)
    let eos = eos_id(tr.lm.tok)
    return build_string() $(var w) {
        generate_(tr.lm, tr.ls, prompt, params, cap) $(id, piece) {
            return false if (id == eos)
            w |> write(piece)
            return true
        }
    }
}

[test]
def test_nmt_llm_translator(tst : T?) {
    tst |> run("a translation LLM through the facade: the direction picked, one sentence translated to the end of its line, a Bergamot pair kept") @(t : T?) {
        let path = path_join(models_dir(), "milmmt/MiLMMT-46-1B-v0.1.Q4_K_M.gguf")
        return if (!jit_model_available(t, path))
        let text = "Где ближайшая аптека?"   // nolint:STYLE039 - the source language's own text
        with_dasllama_jobque_() {
            var inscope tr <- load_translator(path)
            t |> equal(tr.kind, TranslatorKind.llm)
            t |> equal(translator_langs(tr).src, "", "no source before the direction is picked")
            t |> success(translator_serves(tr, "ru", "en"), "it serves ru -> en")
            t |> success(!set_translator_langs(tr, "ru", "xx"), "an unknown code refused")
            t |> success(set_translator_langs(tr, "ru", "en"), "ru -> en picked")
            let out = translate(tr, text)
            to_log(LOG_INFO, "milmmt ru->en translate: {replace(out, "\n", "\\n")}\n")
            t |> success(find(to_lower(out), "pharmacy") >= 0, "the translation: '{out}'")
            t |> success(find(out, "\n") < 0, "the translation ends at its line: '{out}'")
            let two = "Привет.\nКак дела?"   // nolint:STYLE039 - the source language's own text
            let first = translate(tr, two)
            let raw = raw_completion(tr, completion_prompt("ru", "en", two), 64l)
            to_log(LOG_INFO, "milmmt ru->en of a two-line source: translate '{replace(first, "\n", "\\n")}', no line stop '{replace(raw, "\n", "\\n")}'\n")
            t |> success(!empty(first) && find(first, "\n") < 0, "a two-line source: the completion stops at the end of its first line: '{first}'")
            t |> success(find(strip(raw), "\n") >= 0, "the control - with no stop at the line's end the completion runs on past it: '{replace(raw, "\n", "\\n")}'")
            t |> success(set_translator_langs(tr, "en", "ru"), "en -> ru picked")
            let back = translate(tr, "Where is the nearest pharmacy?")
            to_log(LOG_INFO, "milmmt en->ru: {replace(back, "\n", "\\n")}\n")
            t |> success(find(back, "аптек") >= 0, "the translation back: '{back}'")   // nolint:STYLE039 - the source language's own text
        }
        var inscope imgs <- dlim_inventory_(path)
        let img = find_index_if(imgs) $(i : DlimImageInfo) => i.verdict == "CURRENT"
        t |> success(img >= 0, "the GGUF load baked a current .dlim beside it")
        if (img >= 0) {
            with_dasllama_jobque_() {
                var inscope ti <- load_translator(imgs[img].path)
                t |> equal(ti.kind, TranslatorKind.llm, "the LLM's prepared image loads as an LLM: {imgs[img].file}")
                t |> success(set_translator_langs(ti, "ru", "en"), "ru -> en picked on the image")
                let out = translate(ti, text)
                t |> success(find(to_lower(out), "pharmacy") >= 0, "the image translates: '{out}'")
            }
        }
        let mpath = path_join(models_dir(), "bergamot/ru-en-base-memory/model.ruen.intgemm.alphas.bin")
        return if (!model_available(t, mpath))
        with_dasllama_jobque_() {
            var inscope mt <- load_translator(mpath)
            t |> equal(translator_langs(mt).src, "ru")
            t |> success(set_translator_langs(mt, "ru", "en"), "its own pair")
            t |> success(!set_translator_langs(mt, "en", "ru"), "a Bergamot student keeps its one direction")
            var inscope ex <- load_translator(mpath, true)
            t |> success(!ex.m.q8, "exact = true loads the f32 lane")
            let line = translate(ex, text)
            to_log(LOG_INFO, "bergamot exact ru->en: {line}\n")
            t |> success(find(to_lower(line), "pharmacy") >= 0, "the f32 lane translates: '{line}'")
        }
    }
}

[test]
def test_nmt_chat_translator(tst : T?) {
    tst |> run("an instruct GGUF through the facade: its own chat template, the translation alone") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3-4B-Instruct-2507-Q4_K_M.gguf")
        return if (!jit_model_available(t, path))
        with_dasllama_jobque_() {
            var inscope tr <- load_translator(path)
            t |> equal(tr.kind, TranslatorKind.llm)
            t |> success(!empty(tr.lm.chat_template_str), "the file carries a chat template - the chat form serves it")
            t |> success(set_translator_langs(tr, "ru", "en"), "ru -> en picked")
            let out = translate(tr, "Где ближайшая аптека?")   // nolint:STYLE039 - the source language's own text
            to_log(LOG_INFO, "qwen3-4b ru->en: {replace(out, "\n", "\\n")}\n")
            t |> success(find(to_lower(out), "pharmacy") >= 0, "the translation: '{out}'")
            t |> success(find(out, "\n") < 0 && find(out, "<think>") < 0, "one line, no reasoning: '{out}'")
        }
    }
}

//! The facade's chat instruction, spelled here to count the prompt its turn renders.
def private chat_system(src, trg : string) : string => "You are a translator. Translate the user's message from {translator_language_name(src)} to {translator_language_name(trg)}. Reply with the translation only - no notes, no quotes, no explanations."

//! the tokens the facade's thinking-off turn renders for `text`
def private chat_prompt_len(tr : Translator; text : string) : int64 {
    var inscope chat <- create_chat_renderer_(tr.lm, chat_system(tr.src, tr.trg))
    set_thinking_(chat, false)
    add_user_(chat, text)
    var inscope prompt <- render_turn_(tr.lm, chat)
    return long_length(prompt)
}

//! the reply budget the hybrid-thinking cell holds: a translation of one short line fits; a reasoning turn spends hundreds
let private CHAT_REPLY_TOKENS = 48l

[test]
def test_nmt_chat_thinking_off(tst : T?) {
    tst |> run("a hybrid-thinking chat model through the facade answers with thinking off: the translation alone, in a short reply") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        return if (!jit_model_available(t, path))
        let text = "Где ближайшая аптека?"   // nolint:STYLE039 - the source language's own text
        with_dasllama_jobque_() {
            var inscope tr <- load_translator(path)
            t |> success(find(tr.lm.chat_template_str, "enable_thinking") >= 0, "the carrier's template carries the thinking switch")
            t |> success(set_translator_langs(tr, "ru", "en"), "ru -> en picked")
            let out = translate(tr, text)
            let reply = tr.ls.n_past - chat_prompt_len(tr, text)
            to_log(LOG_INFO, "qwen3.5-0.8b ru->en: '{replace(out, "\n", "\\n")}', {reply} reply tokens\n")
            t |> success(!empty(out) && find(out, "<think>") < 0, "the translation, no reasoning: '{out}'")
            t |> success(find(to_lower(out), "pharmac") >= 0, "the translation: '{out}'")
            t |> success(reply > 0l && reply <= CHAT_REPLY_TOKENS, "the reply took {reply} tokens, within {CHAT_REPLY_TOKENS}: no reasoning turn ran")
        }
    }
}

[test]
def test_nmt_step_lanes_and_device_gate(t : T?) {
    t |> run("a decode step's lanes follow its widest GEMM through the shared lane rule; the device homes' shape gate") @(t : T?) {
        let lanes = get_dispatch_lanes()
        t |> equal(nmt_step_lanes(NmtModel(d = 512l, ff = 2048l, n_head = 8l)), min(8, lanes), "d 512 ff 2048: eight lanes' share of work, at most the pool")
        t |> equal(nmt_step_lanes(NmtModel(d = 32l, ff = 32l, n_head = 1l)), 1, "a tiny step: one lane")
        let was = set_step_lane_work_probe_(1l << 40l)
        t |> equal(nmt_step_lanes(NmtModel(d = 512l, ff = 2048l, n_head = 8l)), 1, "the seam's lane work past every step: one lane")
        let mine = set_step_lane_work_probe_(0l)
        t |> equal(mine, 1l << 40l, "the seam answers the work it replaced")
        t |> equal(nmt_step_lanes(NmtModel(d = 512l, ff = 2048l, n_head = 8l)), min(8, lanes), "0 restores the default")
        set_step_lane_work_probe_(was)
        t |> success(nmt_device_shape_ok(NmtModel(d = 512l, ff = 2048l, n_head = 8l)), "the students' geometry: served")
        t |> success(!nmt_device_shape_ok(NmtModel(d = 32l, ff = 64l, n_head = 1l)), "a width off the 64-wide tiles: declined")
        t |> success(!nmt_device_shape_ok(NmtModel(d = 512l, ff = 2016l, n_head = 8l)), "an FFN width off the tiles: declined")
        t |> success(!nmt_device_shape_ok(NmtModel(d = 512l, ff = 2048l, n_head = 256l)), "a head of two floats: declined")
        t |> success(!nmt_device_shape_ok(NmtModel(d = 512l, ff = 2048l, n_head = 3l)), "a width no head count divides: declined")
    }
}
