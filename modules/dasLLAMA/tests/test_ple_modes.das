options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: each test frees its model before the next loads
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — load_model_/create_chat/generate/decode
require dasllama/dasllama_gguf
require dasllama/dasllama_convert   // dequant_k5_plane_superblock — the kq plane oracle
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier

// E-series serving-mode rails for the PLE token table (eseries_bringup_plan.md slice A): the
// table is pinned to its DISK-format plane in EVERY serving mode — fp32 keeps the plane bits
// verbatim (gather-time dequant == eager expand, so the fp32 rail stays exact), q4 serves with
// the same pin instead of the old refusal.

def private e2b_q8_path() : string => path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
def private e2b_kq_path() : string => path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")

// file-truth dequant of table row `r` (all = layers*ple elements) straight off the gguf mapping
def private file_row(path : string; r, all : int64) : array<float> {
    var dst : array<float>
    dst |> resize(all)
    let f = fopen(path, "rb")
    if (f == null) {
        panic("cannot open {path}")
    }
    fmap(f) $(var bytes : array<uint8>#) {
        var inscope m <- parse_gguf_meta(bytes)
        gguf_read_tensor_f32(m, bytes, "per_layer_token_embd.weight", dst, 0l, all, r * all)
    }
    fclose(f)
    return <- dst
}

// one short greedy chat turn; returns ONLY the continuation, decoded, for the eyeball log
// (generate's return is prompt echo minus BOS, then continuation, capped at `steps` total)
def private greedy_turn(var m : Model; user : string; n : int64) : string {
    var text = ""
    with_job_que() {
        var s <- make_run_state(m.config)
        var chat = create_chat(m, "")
        add_user(chat, user)
        let prompt <- render_turn(m, chat)
        let np = long_length(prompt)
        let ids <- generate(m, s, prompt, int(np + n))
        var gen : array<int64>
        gen |> reserve(n)
        for (i in range64(np - 1l, long_length(ids))) {
            gen |> push(ids[i])
        }
        text = decode(m, gen)
        delete gen
        delete chat
        delete s
    }
    return text
}

def private letters(s : string) : int {
    var n = 0
    for (ch in s) {
        if (is_alpha(ch)) {
            n++
        }
    }
    return n
}

[test]
def test_ple_fp32_rail(t : T?) {
    t |> run("fp32 serving pins the Q8_0 PLE table to its own q8 plane, bit-exact (E2B Q8_0)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = e2b_q8_path()
        if (!model_available(t, path)) {
            return
        }
        var tr <- load_gguf(path, QuantMode.fp32)
        let c = tr.config
        let all = c.n_layers * c.n_embd_per_layer
        let tab = c.vocab_size * all
        t |> success(tr.ple_emb_fmt == KqFmt.q8 && tr.ple_emb_off == 0l, "table tagged q8 at plane offset 0")
        t |> success(long_length(tr.qblob) == tab, "the q8 plane holds EXACTLY the table ({long_length(tr.qblob)} of {tab} elements)")
        // no expansion: the fp32 blob no longer has room for the 4x-expanded 9.4 GB table
        t |> success(long_length(tr.wblob) < tab, "fp32 wblob ({long_length(tr.wblob)}) is smaller than the table it no longer carries ({tab})")
        // gather bit-match: dequant off the pinned plane == the file's own dequant, first/second/last row
        var have : array<float>
        have |> resize(all)
        var eq = true
        var worst = 0.0lf
        for (r in [0l, 1l, c.vocab_size - 1l]) {
            unsafe {
                dequant_q8_row(tr, tr.ple_emb_off + r * all, all, addr(have[0]))
            }
            var want <- file_row(path, r, all)
            for (i in range64(all)) {
                if (have[i] != want[i]) {
                    eq = false
                    worst = max(worst, double(abs(have[i] - want[i])))
                }
            }
            delete want
        }
        t |> success(eq, "gather rows bit-match the file dequant (worst |diff| {worst})")
        delete have
        delete tr
    }
}

[test]
def test_ple_fp32_kq_table(t : T?) {
    t |> run("fp32 serving pins a K-quant PLE table to its native plane (E2B Q4_K_M)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = e2b_kq_path()
        if (!model_available(t, path)) {
            return
        }
        var tr <- load_gguf(path, QuantMode.fp32)
        t |> success(tr.ple_emb_fmt == KqFmt.k5, "per_layer_token_embd pinned to its disk k5 plane under fp32")
        t |> success(empty(tr.qblob), "no q8 side plane (the table rides the kq plane)")
        // whole-row placement vs the file dequant (f16-pair rounding only — test_kquant's bar)
        let all = tr.config.n_layers * tr.config.n_embd_per_layer
        var have : array<float>
        have |> resize(all)
        for (blk in range64(all / 256l)) {
            let sb = (tr.ple_emb_off + blk * 256l) / 256l
            dequant_k5_plane_superblock(tr.k5q, sb * K5_QSB, tr.k5s, sb * K5_SSB, have, blk * 256l)
        }
        var want <- file_row(path, 0l, all)
        var num = 0.0lf
        var den = 0.0lf
        for (h, w in have, want) {
            num += double(h - w) * double(h - w)
            den += double(w) * double(w)
        }
        let rr = float(sqrt(num / max(den, 1e-30lf)))
        t |> success(rr < 1e-3, "PLE table row 0 matches the file dequant (rel rms {rr})")
        delete want
        delete have
        delete tr
    }
}

[test]
def test_ple_mode_forwards(t : T?) {
    t |> run("fp32 and q4 E2B chat turns run end to end (E2B Q8_0)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = e2b_q8_path()
        if (!model_available(t, path)) {
            return
        }
        var q8_text = ""
        {
            // q8 control: the known-good serving mode, same raw turn — the shape the other two
            // modes' continuations are eyeballed against
            var m <- load_model_(path, QuantMode.q8)
            m.config.seq_len = min(m.config.seq_len, 512l)   // KV sizing only; the turn is short
            q8_text = greedy_turn(m, "Name three colors.", 24l)
            to_log(LOG_INFO, "PLE q8 greedy (control): {q8_text}\n")
            t |> success(letters(q8_text) >= 3, "q8 control continuation is text: '{q8_text}'")
            delete m
        }
        {
            var m <- load_model_(path, QuantMode.fp32)
            m.config.seq_len = min(m.config.seq_len, 512l)
            let text = greedy_turn(m, "Name three colors.", 24l)
            to_log(LOG_INFO, "PLE fp32 greedy: {text}\n")
            t |> success(letters(text) >= 3, "fp32 greedy continuation is text: '{text}'")
            delete m
        }
        {
            // pre-rail this load PANICKED ("q4 serving mode has no per-layer-embedding rail")
            var m <- load_model_(path, QuantMode.q4_0)
            m.config.seq_len = min(m.config.seq_len, 512l)
            let text = greedy_turn(m, "Name three colors.", 24l)
            to_log(LOG_INFO, "PLE q4 greedy: {text}\n")
            t |> success(letters(text) >= 3, "q4 greedy continuation is text: '{text}'")
            delete m
        }
    }
}
