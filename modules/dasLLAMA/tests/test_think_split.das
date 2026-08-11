options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama   // the facade fires the arch [init] registrations arch_chat reads
require dasllama/dasllama_chat   // split_reasoning_/think_feed_/think_finish_ — the template-level seam under test
require daslib/strings_boost
require math

// The reply-side reasoning matcher, per family, against the REGISTERED chat templates — model-free
// and deterministic. Each thinking family's exact wire shape must split into (reasoning, content),
// whole-string and streamed at adversarial chunk boundaries alike.

// Stream `reply` through a fresh splitter in `chunk`-byte pieces (ASCII fixtures only — slice is
// byte-based), then finish; strip like split_reasoning_ so both paths compare equal.
def private stream_split(tmpl : ChatTemplate; reply : string; chunk : int) : ThinkSplit {
    var ts = make_think_stream_(tmpl, true)
    var rp : array<string>
    var cp : array<string>
    var pos = 0
    let n = length(reply)
    while (pos < n) {
        let e = min(pos + chunk, n)
        var r = ""
        var c = ""
        think_feed_(ts, slice(reply, pos, e), r, c)
        rp |> push(r)
        cp |> push(c)
        pos = e
    }
    var rt = ""
    var ct = ""
    think_finish_(ts, rt, ct)
    rp |> push(rt)
    cp |> push(ct)
    var res = ThinkSplit(reasoning = join(rp, ""), content = join(cp, ""))
    delete rp
    delete cp
    if (!empty(res.reasoning)) {
        res.reasoning = strip(res.reasoning)
        res.content = strip(res.content)
    }
    return res
}

// Whole-string and every chunk size down to 1 byte must agree — the streaming machine's core bar.
def private check_all_chunkings(t : T?; tmpl : ChatTemplate; reply, want_r, want_c : string) {
    let whole = split_reasoning_(tmpl, reply)
    t |> equal(whole.reasoning, want_r, "whole-string reasoning")
    t |> equal(whole.content, want_c, "whole-string content")
    for (chunk in [1, 2, 3, 5, 7, 64]) {
        let st = stream_split(tmpl, reply, chunk)
        t |> equal(st.reasoning, want_r, "chunk={chunk} reasoning")
        t |> equal(st.content, want_c, "chunk={chunk} content")
    }
}

// ===== symmetric: Qwen / GLM (<think> ... </think>) =====

[test]
def test_split_symmetric(t : T?) {
    t |> run("qwen family template declares the symmetric pair") @(t : T?) {
        var ct <- arch_chat("qwen3")
        t |> success(ct.think_mode == ThinkMode.symmetric, "qwen3 mode")
        t |> equal(ct.think_open, "<think>")
        t |> equal(ct.think_close, "</think>")
        t |> success(ct.think_default, "qwen3 thinks by default")
        delete ct
        var g <- arch_chat("glm4moe")
        t |> success(g.think_mode == ThinkMode.symmetric, "glm4moe mode")
        t |> success(g.think_default, "glm4moe thinks by default")
        delete g
    }
    t |> run("the qwen wire shape splits") @(t : T?) {
        var ct <- arch_chat("qwen3")
        check_all_chunkings(t, ct, "<think>\nLet me count.\n</think>\n\nThe answer is 42.",
                            "Let me count.", "The answer is 42.")
        delete ct
    }
    t |> run("no reasoning span passes through untouched") @(t : T?) {
        var ct <- arch_chat("qwen3")
        check_all_chunkings(t, ct, "The answer is 42.", "", "The answer is 42.")
        // a mid-reply <think> is ordinary text — only a reply-opening marker counts
        check_all_chunkings(t, ct, "use the <think> tag like this", "", "use the <think> tag like this")
        delete ct
    }
    t |> run("truncated reasoning (budget cut) classifies as reasoning") @(t : T?) {
        var ct <- arch_chat("qwen3")
        check_all_chunkings(t, ct, "<think>never closed - budget ran out", "never closed - budget ran out", "")
        // an incomplete opener at EOG was ordinary content all along
        check_all_chunkings(t, ct, "<thi", "", "<thi")
        delete ct
    }
    t |> run("leading whitespace before the opener is allowed") @(t : T?) {
        var ct <- arch_chat("qwen3")
        check_all_chunkings(t, ct, "  \n<think>r</think>answer", "r", "answer")
        delete ct
    }
    t |> run("strip_think parity on the content half") @(t : T?) {
        var ct <- arch_chat("qwen3")
        for (reply in ["<think>\nr\n</think>\n\nThe answer is 42.", "  \n<think>r</think>answer",
                       "<think>never closed", "The answer is 42.", "", "use the <think> tag like this"]) {
            t |> equal(split_reasoning_(ct, reply).content, strip_think(reply), "parity on '{reply}'")
        }
        delete ct
    }
}

// ===== asymmetric: gemma-4 (<|channel>thought ... <channel|>) =====

[test]
def test_split_gemma4(t : T?) {
    t |> run("gemma4 template declares the asymmetric pair, thinking off by default") @(t : T?) {
        var ct <- arch_chat("gemma4")
        t |> success(ct.think_mode == ThinkMode.asymmetric, "mode")
        t |> equal(ct.think_open, "<|channel>thought")
        t |> equal(ct.think_close, "<channel|>")
        t |> success(!ct.think_default, "E-series thinking is opt-in")
        t |> success(!empty(ct.think_gate.parts), "the <|think|> system gate is declared")
        t |> success(!empty(ct.assistant_open_think.parts), "the bare thinking-ON opener is declared")
        delete ct
    }
    t |> run("the gemma-4 wire shape splits (opener's trailing newline optional)") @(t : T?) {
        var ct <- arch_chat("gemma4")
        check_all_chunkings(t, ct, "<|channel>thought\nLet me ponder.\n<channel|>\nAnswer here.",
                            "Let me ponder.", "Answer here.")
        check_all_chunkings(t, ct, "<|channel>thoughtpondering<channel|>direct", "pondering", "direct")
        delete ct
    }
    t |> run("thinking-off replies (no channel markers) pass through") @(t : T?) {
        var ct <- arch_chat("gemma4")
        check_all_chunkings(t, ct, "Just the answer.", "", "Just the answer.")
        delete ct
    }
    t |> run("truncated thought classifies as reasoning; a bare opener at EOG too") @(t : T?) {
        var ct <- arch_chat("gemma4")
        check_all_chunkings(t, ct, "<|channel>thought\nran out of budget", "ran out of budget", "")
        check_all_chunkings(t, ct, "<|channel>thought", "", "")
        delete ct
    }
}

// ===== channel-switch: gpt-oss (Harmony channels) =====

[test]
def test_split_gptoss(t : T?) {
    t |> run("gpt-oss template declares channel_switch") @(t : T?) {
        var ct <- arch_chat("gpt-oss")
        t |> success(ct.think_mode == ThinkMode.channel_switch, "mode")
        t |> success(ct.think_default, "thinks by default")
        delete ct
    }
    t |> run("analysis -> final splits") @(t : T?) {
        var ct <- arch_chat("gpt-oss")
        check_all_chunkings(t, ct,
            "<|channel|>analysis<|message|>Deep thought.<|end|><|start|>assistant<|channel|>final<|message|>The answer.",
            "Deep thought.", "The answer.")
        delete ct
    }
    t |> run("straight-to-final replies have no reasoning") @(t : T?) {
        var ct <- arch_chat("gpt-oss")
        check_all_chunkings(t, ct, "<|channel|>final<|message|>Hi there.", "", "Hi there.")
        delete ct
    }
    t |> run("multiple analysis segments accumulate; commentary counts as reasoning") @(t : T?) {
        var ct <- arch_chat("gpt-oss")
        check_all_chunkings(t, ct,
            "<|channel|>analysis<|message|>one.<|end|><|start|>assistant<|channel|>commentary<|message|>two.<|end|><|start|>assistant<|channel|>final<|message|>done",
            "one.two.", "done")
        delete ct
    }
    t |> run("truncated analysis classifies as reasoning") @(t : T?) {
        var ct <- arch_chat("gpt-oss")
        check_all_chunkings(t, ct, "<|channel|>analysis<|message|>cut mid-thou", "cut mid-thou", "")
        delete ct
    }
    t |> run("a reply with no channel framing passes through") @(t : T?) {
        var ct <- arch_chat("gpt-oss")
        check_all_chunkings(t, ct, "bare text reply", "", "bare text reply")
        delete ct
    }
}

// ===== unarmed / non-thinking =====

[test]
def test_split_unarmed(t : T?) {
    t |> run("non-thinking families (mode none) pass everything through") @(t : T?) {
        var ct <- arch_chat("llama")
        t |> success(ct.think_mode == ThinkMode.none, "llama declares no reply format")
        check_all_chunkings(t, ct, "<think>not a thinking model</think> text", "",
                            "<think>not a thinking model</think> text")
        delete ct
    }
    t |> run("an unarmed stream (thinking off) passes everything through") @(t : T?) {
        var ct <- arch_chat("qwen3")
        var ts = make_think_stream_(ct, false)
        var r = ""
        var c = ""
        think_feed_(ts, "<think>suppressed anyway</think>x", r, c)
        t |> equal(r, "")
        t |> equal(c, "<think>suppressed anyway</think>x")
        think_finish_(ts, r, c)
        t |> equal(r, "")
        t |> equal(c, "")
        delete ct
    }
}
