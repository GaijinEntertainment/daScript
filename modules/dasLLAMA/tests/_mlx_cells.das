options gen2
options _dasllama_internal = true

module _mlx_cells shared public

//! the MLX snapshots' oracles and teacher-forced rail, and the Llama-3.2-1B 8-bit snapshot's cell on its GGUF twin's form; no
//! entry loads the twin

require dastest/testing_boost public
require _model_tier   // hf_snapshot_dir, models_dir
require _compares   // argmax_of: a logits row's top-1 id
require _model_form   // with_ctx_cut: the twin's teacher-forced feed on a shorter context than the block's
require dasllama/dasllama_core
require dasllama/dasllama_common
require dasllama/dasllama_load
require dasllama/dasllama_math
require dasllama/dasllama_gguf   // gguf_file_int, gguf_file_arch: the twin file's own context, read off its header
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration for the snapshot's load
require daslib/fio   // path_join
require math

//! the teacher-forced prompt both oracles ran: "The capital of France is Paris. The capital of Germany is Berlin. The sky is blue."
let PROMPT_IDS = fixed_array(128000l, 791l, 6864l, 315l, 9822l, 374l, 12366l, 13l, 578l, 3363l, 315l, 9635l, 374l, 7295l, 13l, 578l, 10846l, 374l, 6437l, 13l)

//! the snapshot runtime's top-2 per prompt position (CPU device, the snapshot's own weights): id, logit, runner-up id, logit
let private ORACLE_1B <- [(16309l, 7.0703, 2l, 7.0664), (2768l, 10.1875, 220l, 10.1016), (315l, 20.4062, 3363l, 18.0156), (9822l, 16.5781, 279l, 16.2656), (374l, 21.3594, 198l, 19.5938), (12366l, 19.0000, 264l, 16.3594), (13l, 22.7969, 627l, 22.3594), (578l, 19.3594, 12366l, 18.9688), (469l, 17.4844, 6864l, 16.9062), (374l, 22.3438, 315l, 21.7812), (12366l, 20.0312, 3021l, 16.4531), (374l, 23.2812, 11l, 21.0625), (7295l, 18.6250, 3967l, 18.4688), (13l, 25.6406, 627l, 22.8750), (578l, 21.1406, 1628l, 18.3125), (6864l, 21.2969, 3363l, 18.2344), (14298l, 13.4766, 6498l, 13.3203), (645l, 15.5625, 264l, 14.1875), (11l, 19.3125, 323l, 19.0938), (578l, 19.0156, 3639l, 16.3281)]

//! the oracle's logit at its top-1 id against ours: the gap the activation forms leave (Q8_0 rows against the runtime's halves)
let ORACLE_LOGIT_BAR = 0.4
//! a position whose oracle top-2 sit closer than this is a near tie: either id may lead
let private ORACLE_TIE_GAP = 0.5

//! `ids` fed one a position from 0, each position's logits row
def teacher_forced(var m : Model; ids : array<int64>) : array<array<float>> {
    m.config.seq_len = min(m.config.seq_len, 512l)
    var s <- make_run_state(m.config)
    var out : array<array<float>>
    for (id, i in ids, count()) {
        forward(m, s, id, int64(i))
        out |> emplace(clone_to_move(s.logits))
    }
    delete s
    return <- out
}

def teacher_forced(var m : Model) : array<array<float>> {
    var inscope ids <- [for (id in PROMPT_IDS); id]
    return <- teacher_forced(m, ids)
}

//! our teacher-forced logits against the snapshot runtime's top-2 rows
def oracle_gate(t : T?; tag : string; lg : array<array<float>>; oracle : array<tuple<int64; float; int64; float>>) {
    var worst = 0.0
    var lead_miss = 0
    for (row, o in lg, oracle) {
        worst = max(worst, nan_wide_diff(row[o._0], o._1))
        if (o._1 - o._3 >= ORACLE_TIE_GAP && argmax_of(row) != o._0) {
            lead_miss++
        }
    }
    to_log(LOG_INFO, "{tag} vs the snapshot runtime: worst top-1 logit gap {worst}, {lead_miss} lead misses\n")
    t |> success(worst < ORACLE_LOGIT_BAR, "{tag}: the oracle's top-1 logit within {ORACLE_LOGIT_BAR} at every position (worst {worst})")
    t |> equal(lead_miss, 0, "{tag}: every position the oracle leads by {ORACLE_TIE_GAP}+ keeps its top-1")
}

//! the context the GGUF file at `path` declares (`{arch}.context_length`), read off its header: a block's form caps its
//! model's context, the file's stays the twin's
def private file_seq_len(path : string) : int64 => gguf_file_int(path, "{gguf_file_arch(path)}.context_length", 0l)

//! the Llama-3.2-1B MLX 8-bit snapshot's cell against its Q8_0 GGUF twin `b`, a CPU form of the twin's block; a skip where the snapshot is not stocked
def mlx_llama_1b_8bit_cell(t : T?; var b : Model) {
    unsafe {
        t |> run("Llama-3.2-1B 8-bit snapshot: config = the Q8_0 twin's, mlx8 planes at every dense site, logits against both oracles") @capture(& b)(t : T ?) {
            let snap = hf_snapshot_dir("mlx-community/Llama-3.2-1B-Instruct-8bit")
            if (empty(snap)) {
                t |> skip("the 1B snapshot not present")
                return
            }
            var a <- load_model_(snap, QuantMode.q8)
            t |> success(a.config.rope_neox && !b.config.rope_neox, "the checkpoint's half-split q/k rows rope as NEOX pairs, the converter's permuted rows as adjacent ones")
            var ca = a.config
            ca.rope_neox = b.config.rope_neox
            var cb = b.config
            cb.seq_len = file_seq_len(path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf"))
            t |> equal(sprint_json(ca, false), sprint_json(cb, false), "every other config field equals the twin's")
            t |> equal(a.emb_fmt, KqFmt.mlx8, "the token table (tied classifier) serves its own mlx8 planes")
            for (l in range64(a.config.n_layers)) {
                for (f in [fmt_at(a.wq_fmt, l), fmt_at(a.wk_fmt, l), fmt_at(a.wv_fmt, l), fmt_at(a.wo_fmt, l), fmt_at(a.w1_fmt, l), fmt_at(a.w2_fmt, l), fmt_at(a.w3_fmt, l)]) {
                    if (f != KqFmt.mlx8) {
                        t |> failure("layer {l}: a dense plane tagged {f}, not its disk type's mlx8")
                    }
                }
            }
            t |> equal(long_length(a.qblob), 0l, "no q8 copy of any plane")
            var inscope la <- teacher_forced(a)
            var inscope lb : array<array<float>>
            with_ctx_cut(b, 512l) {
                delete lb
                lb <- teacher_forced(b)
            }
            var worst = 0.0
            var agree = 0
            for (ra, rb in la, lb) {
                let rel = logits_maxdiff(ra, rb) / logits_maxabs(rb)   //! inf / inf where a side holds a NaN: a NaN, read as the worst
                worst = max(worst, is_nan(rel) ? INFINITE_DIFF : rel)
                agree += argmax_of(ra) == argmax_of(rb) ? 1 : 0
            }
            to_log(LOG_INFO, "1B vs the Q8_0 twin: worst max|d|/max|logit| {worst}, top-1 agree {agree}/20\n")
            t |> success(worst < 0.08, "the twin's teacher-forced logits within 8% of their range (worst {worst}): two quantizers of one checkpoint")
            t |> success(agree >= 17, "top-1 equal to the twin's at {agree} of 20 positions")
            oracle_gate(t, "1B", la, ORACLE_1B)
            delete a
        }
    }
}
