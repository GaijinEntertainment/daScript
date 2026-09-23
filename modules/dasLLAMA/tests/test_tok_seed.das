options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require ../benchmarks/lcpp_bench.das   // tok_read_seed — file-relative require, explicit .das
require dasllama/dasllama_common
require dasllama/dasllama_load
require dasllama/dasllama_bench
require dasllama/dasllama_math   // setup_dasllama_jobque_
require dasllama/dasllama_env
require daslib/jobque_boost
require daslib/fio

def private llama2c_dir() : string {
    let env = g_env_test.llama2c_dir
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")

// a pp row on a session sized to `seq_len`: under the prompt it must refuse before it writes,
// at or over it the row runs
def private pp_row_panics(var tr : Model; seq_len, plen : int64; warm : bool) : bool {
    tr.config.seq_len = seq_len
    var s <- make_run_state(tr.config)
    var tripped = false
    try {
        if (warm) {
            bench_pp_warm(tr, s, plen)
        } else {
            bench_pp_rep(tr, s, plen, 1l)
        }
    } recover {
        tripped = true
    }
    delete s
    return tripped
}

// The --tok corpus reader's header walk, model-free: leading '#' lines are provenance headers and
// come off the seed; everything from the first non-'#' line on is corpus bytes, '#' included.
// NOT gated here: an empty result PANICS by design (an empty/missing corpus is a bench-config
// error), so an empty file and a header-only file have no assertable return.

// Per-process temp directory, created on first use and removed by [finalize] — writing fixtures
// into the worktree would leave untracked state, and a shared path races parallel dastest runs.
var private _tmp_dir : string
var private _tmp_dir_inited = false

def private write_corpus(name : string; content : string) : string {
    if (!_tmp_dir_inited) {
        let r = create_temp_directory_result("dasllama_tok_seed")
        if (!(r is value)) {   // "" would resolve to the CWD and defeat the hermetic goal
            panic("could not create temp dir for the tok_read_seed test: {unsafe(r.error)}")
        }
        _tmp_dir = unsafe(r.value)
        _tmp_dir_inited = true
    }
    let p = path_join(_tmp_dir, name)
    fopen(p, "wb") $(f) {
        if (f != null) {
            fwrite(f, content)
        }
    }
    return p
}

[finalize]
def cleanup_temp_dir {
    if (_tmp_dir_inited && !empty(_tmp_dir)) {
        rmdir_rec(_tmp_dir)
    }
}

[test]
def test_tok_seed_headers(t : T?) {
    t |> run("two '#' header lines come off, the body survives whole") @(t : T?) {
        let p = write_corpus("two_headers.txt",
                             "# source: wikitext-2\n# sha256: deadbeef\nthe body starts here\nand runs on\n")
        t |> equal(tok_read_seed(p), "the body starts here\nand runs on\n")
    }
    t |> run("one header line") @(t : T?) {
        let p = write_corpus("one_header.txt", "#h\nbody\n")
        t |> equal(tok_read_seed(p), "body\n")
    }
    t |> run("an empty header line ('#' alone) still counts") @(t : T?) {
        let p = write_corpus("bare_hash.txt", "#\n#\nbody\n")
        t |> equal(tok_read_seed(p), "body\n")
    }
}

[test]
def test_tok_seed_body_only(t : T?) {
    t |> run("a body-only corpus passes through unchanged") @(t : T?) {
        let p = write_corpus("body_only.txt", "no headers here\njust corpus bytes\n")
        t |> equal(tok_read_seed(p), "no headers here\njust corpus bytes\n")
    }
    t |> run("a body with no trailing newline passes through unchanged") @(t : T?) {
        let p = write_corpus("body_no_nl.txt", "one line, no newline")
        t |> equal(tok_read_seed(p), "one line, no newline")
    }
}

[test]
def test_flat_row_plen(t : T?) {
    t |> run("the flat session's prompt rows never shrink the context under the tg-real row's prompts") @(t : T?) {
        // -p 0 -n 32 beside a real row whose longest prompt capped the context at 270 + 32 + 8
        t |> equal(flat_row_plen(0l, 32l, 310l, true), 278l, "the real row's cap stands: the flat rows read it back as prompt rows")
        t |> equal(flat_row_plen(512l, 128l, 310l, true), 512l, "a flat prompt longer than the real row's cap keeps its own length")
        t |> equal(flat_row_plen(0l, 32l, 4096l, false), 0l, "no real row: the flat session sizes as llama-bench does, plen + ngen")
    }
    t |> run("the rows' context cap covers the flat rows whatever the batched streams' prompts") @(t : T?) {
        t |> equal(rows_ctx_need(512l, 128l, 512l, 4, 1l), 652l, "the batched streams' need: prompt + window + a slot a stream + the margin")
        t |> equal(rows_ctx_need(512l, 128l, 128l, 4, 1l), 512l, "short batched prompts never cap the context under the pp row's prompt")
        t |> equal(rows_ctx_need(64l, 4l, 8l, 4, 1l), 64l, "the flat rows' need wins at every scale")
        t |> equal(rows_ctx_need(512l, 128l, 512l, 0, 1l), 512l, "no batched row: the flat rows alone")
        t |> equal(rows_ctx_need(0l, 128l, 128l, 4, 3l), 548l, "the self-speculative streams take depth + 1 rows a tick")
    }
    t |> run("the pp rows refuse a session sized under their prompt (needs stories15M)") @(t : T?) {
        if (!stat(MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present at {MODEL_PATH} (set DASLLAMA_LLAMA2C_DIR)")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        with_job_que() {
            setup_dasllama_jobque_()
            t |> success(!pp_row_panics(tr, 16l, 8l, true), "a session that fits runs the warmup")
            t |> success(pp_row_panics(tr, 4l, 8l, true), "the warmup prefill checks the cap before it writes")
            t |> success(pp_row_panics(tr, 4l, 8l, false), "a timed rep checks the cap before it writes")
        }
    }
}

[test]
def test_tok_seed_hash_in_body(t : T?) {
    t |> run("a '#' MID-LINE in the body is corpus, not a header") @(t : T?) {
        let p = write_corpus("mid_line_hash.txt", "# header\nlet x = 1 # not a header\ntail\n")
        t |> equal(tok_read_seed(p), "let x = 1 # not a header\ntail\n")
    }
    t |> run("a '#' line AFTER the body opens is corpus too - the walk stops at the first body line") @(t : T?) {
        let p = write_corpus("late_hash.txt", "# header\nbody\n# this one stays\nmore\n")
        t |> equal(tok_read_seed(p), "body\n# this one stays\nmore\n")
    }
}

[test]
def test_tok_seed_unterminated_header(t : T?) {
    t |> run("a '#' header with NO trailing newline is kept, not stripped") @(t : T?) {
        // the walk needs a newline to know where the header ENDS; with none it leaves the text
        // alone rather than eating the whole file (the pre-sweep loop did the same)
        let p = write_corpus("header_no_nl.txt", "# header and nothing else")
        t |> equal(tok_read_seed(p), "# header and nothing else")
    }
    t |> run("headers, then a LAST header line with no newline, keeps that one") @(t : T?) {
        let p = write_corpus("last_header_no_nl.txt", "# first\n# second with no newline")
        t |> equal(tok_read_seed(p), "# second with no newline")
    }
}
