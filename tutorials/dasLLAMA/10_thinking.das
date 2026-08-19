options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require strings

// Tutorial dasLLAMA-10: Thinking Models
//
// This tutorial covers:
//   - Hybrid thinking models: the reply carries a reasoning span before the answer
//   - split_reasoning — split a complete reply into (reasoning, content)
//   - Streaming the split live: make_think_stream + think_feed / think_finish
//   - think_drain — the same splitter over a reply already in hand
//   - set_thinking(false) — answer directly; effective_stop_ids for schedulers
//
// Prerequisites: tutorial 02; a thinking-family GGUF (Qwen3-0.6B-Q8_0 works:
// https://huggingface.co/Qwen/Qwen3-0.6B-GGUF).
//
// Run: daslang.exe -jit tutorials/dasLLAMA/10_thinking.das -- <path/to/model.gguf>

let SYSTEM = "You are a concise assistant."

// Resolve the model path: last command-line argument ending in .gguf, else $DASLLAMA_MODEL.
def model_path() : string {
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        return cmd[length(cmd) - 1]
    }
    return get_env_variable("DASLLAMA_MODEL")
}

// First characters of a long span, for compact printing.
def head(s : string; n : int) : string {
    return length(s) <= n ? s : slice(s, 0, n) + "..."
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — The reply carries its reasoning; Section 2 — split_reasoning
// ──────────────────────────────────────────────────────────────────────────
//
// A thinking model works through the question before answering, and the reply's
// wire format carries that span: Qwen3 and GLM wrap it in <think>...</think>,
// gpt-oss speaks Harmony channels, gemma-4 a thought channel. respond() streams
// the RAW pieces — markers included — while chat.history stores the reply
// already reasoning-stripped (tutorial 02).
//
// split_reasoning splits a complete reply at the family's reasoning boundary.
// Both halves come back stripped when a reasoning span was found; a reply with
// no reasoning passes through untouched — so it is safe to run on every reply,
// thinking model or not.

def section_buffered_split(m : Model; var chat : ChatSession) {
    add_user(chat, "Which is larger: 17 * 24 or 20 * 20? One sentence.")
    let full = respond(m, chat, SamplingParams()) $(_piece) => true   // buffered: collect, no live output
    print("raw reply ({length(full)} chars): {head(full, 80)}\n")
    print("history stores: \"{chat.history[length(chat.history) - 1].content}\"\n\n")

    let sp = split_reasoning(chat, full)
    print("reasoning ({length(sp.reasoning)} chars): {head(sp.reasoning, 120)}\n")
    print("content: {sp.content}\n\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — Streaming the split: think_feed / think_finish
// ──────────────────────────────────────────────────────────────────────────
//
// A live UI wants the reasoning marked WHILE it streams, not after. The
// ThinkStream splitter turns each streamed piece into (reasoning, content)
// deltas, holding partial markers across piece boundaries. Create it for the
// turn, feed every piece, and flush at the end — an unclosed span (token
// budget cut mid-thought) classifies as reasoning.

def section_streamed_split(m : Model) {
    // a fresh conversation: a small model's LATER turns sometimes end inside the think
    // block (the fallback below shows how that streams); the first turn closes cleanly
    var chat = create_chat(m, SYSTEM, 1024l)
    add_user(chat, "What is the capital of Australia? One sentence.")
    var ts = make_think_stream(chat)
    var shown_think = false
    var shown_answer = false
    respond(m, chat, SamplingParams()) $(piece) {
        var r = ""
        var c = ""
        think_feed(ts, piece, r, c)
        if (!empty(r)) {
            if (!shown_think) {
                print("[thinking] ")
                shown_think = true
            }
            fprint(fstdout(), r)
        }
        if (!empty(c)) {
            if (!shown_answer) {
                print("\n[answer] ")
                shown_answer = true
            }
            fprint(fstdout(), c)
        }
        fflush(fstdout())
        return true
    }
    var tail_r = ""
    var tail_c = ""
    think_finish(ts, tail_r, tail_c)
    fprint(fstdout(), tail_r)
    if (!empty(tail_c)) {
        if (!shown_answer) {
            print("\n[answer] ")
        }
        fprint(fstdout(), tail_c)
    } elif (!shown_answer) {
        // small models sometimes end the turn inside the think block; the unclosed
        // tail classifies as reasoning — exactly the truncated-tail rule above
        print("\n(no content half: the reply ended inside its think block)")
    }
    print("\n\n")

    // think_drain is the buffered twin: the reply is already whole, so one call
    // runs feed + finish + the strip rule. A server's non-streaming route uses
    // this — same splitter, no loop.
    add_user(chat, "What is 2 + 2? One sentence.")
    var ts2 = make_think_stream(chat)
    let full = respond(m, chat, SamplingParams()) $(_piece) {
        return true   // capture only; the drain below does the splitting
    }
    let sp = think_drain(ts2, full)
    print("think_drain: {length(sp.reasoning)} chars of reasoning, answer: {sp.content}\n\n")
}

// ──────────────────────────────────────────────────────────────────────────
// Section 4 — Turning thinking off
// ──────────────────────────────────────────────────────────────────────────
//
// Hybrid families answer directly when the turn opens with the template's
// empty think block. set_thinking(false) renders exactly that (a no-op for
// models with no think specials in the vocab); the default is on. Schedulers
// that cut streams themselves read effective_stop_ids — the template's stops
// plus the thinking-off extras while thinking is off — rather than the
// template's raw stop list.

def section_thinking_off(m : Model; var chat : ChatSession) {
    let stops_on <- effective_stop_ids(chat)
    set_thinking(chat, false)
    let stops_off <- effective_stop_ids(chat)
    print("stop ids: {length(stops_on)} thinking on -> {length(stops_off)} thinking off\n")

    add_user(chat, "And 12 * 12 vs 11 * 13? One sentence.")
    print("Bot (no thinking): ")
    respond(m, chat, SamplingParams()) $(piece) {
        fprint(fstdout(), piece)
        fflush(fstdout())
        return true
    }
    print("\n")
}

[export]
def main() {
    let path = model_path()
    if (empty(path)) {
        print("usage: daslang -jit tutorials/dasLLAMA/10_thinking.das -- <model.gguf>\n")
        return
    }
    var m <- load_model(path, QuantMode.q8)
    allow_cpu_prefill()   // a CPU tutorial, said explicitly — on a Mac, set_metal_mode(MetalMode.when_available) picks the GPU rails instead
    with_job_que() {
        setup_dasllama_jobque()
        var chat = create_chat(m, SYSTEM, 1024l)   // thinking burns budget; the 256 default is too tight
        section_buffered_split(m, chat)
        section_streamed_split(m)
        section_thinking_off(m, chat)
    }
}
