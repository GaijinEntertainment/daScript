options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require ../benchmarks/lcpp_bench.das   // tok_read_seed — file-relative require, explicit .das
require daslib/fio

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
