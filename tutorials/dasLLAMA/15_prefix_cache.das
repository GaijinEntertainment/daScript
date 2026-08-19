options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity; drop for bit-exact work

require dasllama/dasllama
require daslib/jobque_boost
require strings

// Tutorial dasLLAMA-15: The Prefix Cache by Hand
//
// This tutorial covers:
//   - create_prefix_cache over one create_kv_pool pool
//   - prefix_insert: a finished stream donates its KV pages
//   - prefix_attach: the next request skips the donated prefill
//   - prefix_chain_list + prefix_held_groups: what the cache holds
//   - prefix_release: give every page back before the pool dies
//
// Tutorial 13's scheduler ran a prefix cache for us. Here we drive the same
// cache by hand — the shape you need when you build your own scheduler, or
// when you want to see exactly where the skipped prefill goes.
//
// The idea in one line: two requests that start with the same system prompt
// compute the same KV rows for it, so the first request can DONATE those rows
// and the second can ATTACH them instead of recomputing. The cache works in
// whole KV pages (the pool's page_rows), so the match is page-granular.
//
// Prerequisites: any small instruct GGUF (SmolLM2-135M works).
//
// Run: daslang.exe -jit tutorials/dasLLAMA/15_prefix_cache.das -- <model.gguf>

let SYSTEM = "You are a terse assistant for a hardware store. Answer in one sentence. Our catalog: hammers, nails, screws, drills, saws, sandpaper, glue, tape, rope, ladders. Store hours: 9 to 18, Monday to Saturday."
let Q1 = " Do you sell drills?"
let Q2 = " When do you open?"
let PAGE_ROWS = 16l

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — Donate, then attach
// ──────────────────────────────────────────────────────────────────────────
//
// Stream A prefills the full prompt, answers, and donates its pages with
// prefix_insert. `tokens` is A's full evaled history — the cache keeps every
// full page of it. The preview string is only a label for dashboards.
//
// Stream B starts FRESH from the same pool. prefix_attach walks B's prompt
// against the cached chains: matched pages join B's page table, B's n_past
// jumps past them, and we prefill only the tail. The match is capped one
// token short of the prompt — the model still needs one eval to make logits.

def section_donate_attach(m : Model; var pool : KVPool; var cache : PrefixCache) {
    let p1 <- encode(m, "{SYSTEM}{Q1}")
    var s1 = create_session(m, pool)
    eval(m, s1, p1)
    print("A prefilled {s1.n_past} tokens; answer: '{piece(m, sample(s1, SamplingParams()))}...'\n")
    prefix_insert(cache, pool, s1, p1, "hardware-store system prompt")
    release_kv_pages(s1)
    delete s1

    let p2 <- encode(m, "{SYSTEM}{Q2}")
    var s2 = create_session(m, pool)
    let matched = prefix_attach(cache, pool, s2, p2)
    let total = long_length(p2)
    print("B matched {matched} of {total} tokens from the cache ({matched / PAGE_ROWS} pages)\n")
    let tail <- [for (i in range64(matched, total)); p2[i]]
    eval(m, s2, tail)   // only the tail computes; the matched pages are already KV
    print("B prefilled {long_length(tail)} tokens instead of {total}; answer: '{piece(m, sample(s2, SamplingParams()))}...'\n\n")
    release_kv_pages(s2)
    delete s2
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — What the cache holds, and giving it back
// ──────────────────────────────────────────────────────────────────────────
//
// prefix_chain_list snapshots every donation: how many tokens it covers, how
// many of its pages are still alive, how often it was hit, and the preview we
// labeled it with. prefix_held_groups counts the pool pages the cache is
// keeping for reuse — pages a plain release would have returned.
//
// The cache BORROWS from the pool, so teardown order matters: prefix_release
// hands every cached page back, and only then is the pool safe to delete.

def section_observe_release(var pool : KVPool; var cache : PrefixCache) {
    var inscope chains <- prefix_chain_list(cache)
    for (c in chains) {
        print("chain {c.id}: {c.n_tokens} tokens, {c.pages_live} live pages, {c.hits} hit(s), '{c.preview}'\n")
    }
    print("cache holds {prefix_held_groups(cache)} pool page(s)\n")
    prefix_release(cache, pool)
    print("after prefix_release: {prefix_held_groups(cache)} page(s)\n")
}

[export]
def main() {
    let cmd <- get_command_line_arguments()
    var path = ""
    for (a in cmd) {
        if (ends_with(a, ".gguf")) {
            path = a
        }
    }
    if (empty(path)) {
        print("usage: daslang.exe -jit tutorials/dasLLAMA/15_prefix_cache.das -- <model.gguf>\n")
        return
    }
    var m <- load_model(path, QuantMode.q8)
    allow_cpu_prefill()   // a CPU tutorial, said explicitly — on a Mac, set_metal_mode(MetalMode.when_available) picks the GPU rails instead
    var pool <- create_kv_pool(m, PAGE_ROWS)
    var cache <- create_prefix_cache()
    with_job_que() {
        setup_dasllama_jobque()
        section_donate_attach(m, pool, cache)
        section_observe_release(pool, cache)
    }
    delete cache
    delete pool
    delete m
}
