options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require ../benchmarks/lcpp_bench.das   // tok_read_seed — file-relative require, explicit .das
require dasllama/dasllama_common   // Model, MtpAssistantStats and the assistant registry the --mtp-ab seams read
require _scratch_dir   // scratch_file: the corpus fixtures' files

// The --tok corpus reader's header walk, model-free: leading '#' lines are provenance headers and
// come off the seed; everything from the first non-'#' line on is corpus bytes, '#' included.
// NOT gated here: an empty result PANICS by design (an empty/missing corpus is a bench-config
// error), so an empty file and a header-only file have no assertable return.

[test]
def test_tok_seed_headers(t : T?) {
    t |> run("two '#' header lines come off, the body survives whole") @(t : T?) {
        let p = scratch_file("two_headers.txt",
                             "# source: wikitext-2\n# sha256: deadbeef\nthe body starts here\nand runs on\n")
        t |> equal(tok_read_seed(p), "the body starts here\nand runs on\n")
    }
    t |> run("one header line") @(t : T?) {
        let p = scratch_file("one_header.txt", "#h\nbody\n")
        t |> equal(tok_read_seed(p), "body\n")
    }
    t |> run("an empty header line ('#' alone) still counts") @(t : T?) {
        let p = scratch_file("bare_hash.txt", "#\n#\nbody\n")
        t |> equal(tok_read_seed(p), "body\n")
    }
}

[test]
def test_tok_seed_body_only(t : T?) {
    t |> run("a body-only corpus passes through unchanged") @(t : T?) {
        let p = scratch_file("body_only.txt", "no headers here\njust corpus bytes\n")
        t |> equal(tok_read_seed(p), "no headers here\njust corpus bytes\n")
    }
    t |> run("a body with no trailing newline passes through unchanged") @(t : T?) {
        let p = scratch_file("body_no_nl.txt", "one line, no newline")
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
}

[test]
def test_tok_seed_hash_in_body(t : T?) {
    t |> run("a '#' MID-LINE in the body is corpus, not a header") @(t : T?) {
        let p = scratch_file("mid_line_hash.txt", "# header\nlet x = 1 # not a header\ntail\n")
        t |> equal(tok_read_seed(p), "let x = 1 # not a header\ntail\n")
    }
    t |> run("a '#' line AFTER the body opens is corpus too - the walk stops at the first body line") @(t : T?) {
        let p = scratch_file("late_hash.txt", "# header\nbody\n# this one stays\nmore\n")
        t |> equal(tok_read_seed(p), "body\n# this one stays\nmore\n")
    }
}

[test]
def test_tok_seed_unterminated_header(t : T?) {
    t |> run("a '#' header with NO trailing newline is kept, not stripped") @(t : T?) {
        // the walk needs a newline to know where the header ENDS; with none it leaves the text
        // alone rather than eating the whole file (the pre-sweep loop did the same)
        let p = scratch_file("header_no_nl.txt", "# header and nothing else")
        t |> equal(tok_read_seed(p), "# header and nothing else")
    }
    t |> run("headers, then a LAST header line with no newline, keeps that one") @(t : T?) {
        let p = scratch_file("last_header_no_nl.txt", "# first\n# second with no newline")
        t |> equal(tok_read_seed(p), "# second with no newline")
    }
}

[test]
def test_mtp_ab_drafter_line(t : T?) {
    t |> run("--mtp-ab's drafter line: the rounds, the joint verifies, the draft and verify a round, the cold steps' total") @(t : T?) {
        t |> equal(mtp_ab_drafter_line(MtpAssistantStats(rounds = 4l, joint = 1l, draft_us = 8000l, verify_us = 2000l, cold_us = 1500l)),
            "  gemma drafter: 4 rounds (1 joint), draft chain 2.00 ms/round, verify+commit 0.50 ms/round, cold 1.5 ms total\n")
        t |> equal(mtp_ab_drafter_line(MtpAssistantStats(cold_us = 250l)),
            "  gemma drafter: 0 rounds (0 joint), draft chain 0.00 ms/round, verify+commit 0.00 ms/round, cold 0.2 ms total\n",
            "no round: the per-round figures read 0, never a division by zero")
    }
}

[test]
def test_mtp_ab_drafter_attach(t : T?) {
    t |> run("--mtp-ab on a headless model attaches the drafter beside it through the facade, and refuses where none attaches") @(t : T?) {
        var shell = Model()
        let trunk = scratch_path("gemma-4-26B-A4B-it-Q4_K_M.gguf")
        t |> success(!mtp_ab_drafter_attach(shell, trunk), "no sidecar beside the model: nothing attaches")
        scratch_file("mtp-gemma-4-26B-A4B-it-Q8_0.gguf", "a head file no backend can read")
        t |> equal(mtp_drafter_sidecar_(trunk), scratch_path("mtp-gemma-4-26B-A4B-it-Q8_0.gguf"), "the facade resolves the planted sidecar")
        t |> success(!mtp_ab_drafter_attach(shell, trunk), "a sidecar no backend takes attaches nothing - the bench refuses the A/B")
        t |> success(!mtp_assistant_attached(shell), "and the drafter line stays off")
        delete shell
    }
}
