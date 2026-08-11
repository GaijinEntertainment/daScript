options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require strings

// Tutorial dasLLAMA-09: Embeddings
//
// This tutorial covers:
//   - embed(model, text): a mean-pooled, L2-normalized sentence vector
//   - Cosine similarity as a plain dot product (the vectors are unit length)
//   - Semantic ranking: order candidates by closeness to a query
//   - What "RAG-grade" means — when a decoder-as-embedder is the right tool
//
// Any chat model dasLLAMA loads doubles as an embedder. embed() runs one forward
// pass, mean-pools the decoder's last-layer hidden state (post-final-norm) over all
// positions, and L2-normalizes the result to unit length. Because every vector is
// unit length, cosine similarity is just the dot product — no dividing by norms.
//
// Prerequisites: a GGUF model file on disk (any of the tutorial models works).
// These tutorials use SmolLM2-135M-Instruct Q8_0 (~145MB):
//   https://huggingface.co/bartowski/SmolLM2-135M-Instruct-GGUF
//
// Run: daslang.exe -jit tutorials/dasLLAMA/09_embeddings.das -- <path/to/model.gguf>
// (always pass -jit — interpreted inference is far too slow for model work)

// Resolve the model path: last command-line argument ending in .gguf, else $DASLLAMA_MODEL.
def model_path() : string {
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        return cmd[length(cmd) - 1]
    }
    return get_env_variable("DASLLAMA_MODEL")
}

// Cosine similarity of two embeddings. embed() returns unit-length vectors, so the
// dot product IS the cosine — 1.0 means identical direction, 0.0 means unrelated.
def cosine(a, b : array<float>) : float {
    var s = 0.0
    for (x, y in a, b) {
        s += x * y
    }
    return s
}

[export]
def main() {   // nolint:STYLE038 - the tutorial is one linear read; the section banners are its structure
    let path = model_path()
    if (empty(path)) {
        print("usage: daslang -jit tutorials/dasLLAMA/09_embeddings.das -- <model.gguf>\n")
        print("(or set DASLLAMA_MODEL — a good tiny model is SmolLM2-135M-Instruct-Q8_0.gguf,\n")
        print(" https://huggingface.co/bartowski/SmolLM2-135M-Instruct-GGUF)\n")
        return
    }
    if (!jit_enabled()) {
        print("NOTE: running interpreted — re-run with -jit, inference is far too slow without it\n")
    }

    // ──────────────────────────────────────────────────────────────────────────
    // Section 1 — Load the model
    // ──────────────────────────────────────────────────────────────────────────
    //
    // No special "embedding model" is needed: load_model gives an ordinary decoder,
    // and embed() reads its hidden state instead of its token logits. The vector width
    // is model.config.dim — the decoder's embedding dimension.

    print("loading {path}\n")
    var m <- load_model(path, QuantMode.q8)
    allow_cpu_prefill()   // a CPU tutorial, said explicitly — on a Mac, set_metal_mode(MetalMode.when_available) picks the GPU rails instead
    print("arch={m.arch} dim={m.config.dim}\n\n")

    // The forward pass splits its matmuls across the job queue, so embed() — like
    // generate() — must run inside with_job_que(). Model code outside one panics.
    with_job_que() {
        setup_dasllama_jobque()

        // ──────────────────────────────────────────────────────────────────────
        // Section 2 — One vector per sentence
        // ──────────────────────────────────────────────────────────────────────
        //
        // embed() is the whole API: text in, a fixed-width unit vector out. A vector's
        // dot product with itself is 1.0 because embed() L2-normalizes — the cheapest
        // sanity check that the vector really is unit length.

        let query = "How do I sort a list in Python?"
        let qv <- embed(m, query)
        print("embed(query) -> {length(qv)} floats, self-similarity {cosine(qv, qv)} (== 1.0)\n\n")

        // ──────────────────────────────────────────────────────────────────────
        // Section 3 — Semantic ranking
        // ──────────────────────────────────────────────────────────────────────
        //
        // This is retrieval in miniature: embed a handful of candidate sentences, score
        // each against the query by cosine similarity, and sort. The on-topic answers
        // float to the top and the unrelated ones sink — and the concept sentence
        // ("Quicksort and merge sort …") ranks high on meaning, not on shared words.

        let candidates = [
            "Use the sorted() built-in or list.sort() to order a Python list.",
            "Quicksort and merge sort are classic comparison-based algorithms.",
            "The cat curled up on the warm windowsill and fell asleep.",
            "Preheat the oven to 200C before you start baking the bread."
        ]

        var scored : array<tuple<float; string>>
        for (c in candidates) {
            let cv <- embed(m, c)
            scored |> push((cosine(qv, cv), c))
        }
        sort(scored) $(a, b) => a._0 > b._0

        print("ranked by similarity to \"{query}\":\n")
        for (s in scored) {
            print("  {s._0}  {s._1}\n")
        }
        delete scored
    }

    // ──────────────────────────────────────────────────────────────────────────
    // Section 4 — RAG-grade, and what that means
    // ──────────────────────────────────────────────────────────────────────────
    //
    // A decoder used as an embedder gives RAG-grade vectors: good enough to retrieve
    // the relevant passage from a corpus or rank candidates by meaning, which is what
    // retrieval-augmented generation needs. It is NOT a substitute for a dedicated
    // embedding model (BGE, E5, …) — those are trained with a contrastive objective and
    // score higher on similarity benchmarks. Reach for embed() when the model is
    // already loaded and "close enough" retrieval is the job; reach for a real embedder
    // when embedding quality is the product.
    print("\nembed() reuses the loaded chat model — no separate embedding model to ship.\n")
}
