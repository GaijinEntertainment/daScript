options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math   // setup_dasllama_jobque_
require dasllama/dasllama_env

// The CPU-prefill tripwire — this file deliberately never calls allow_cpu_prefill()
// (test_attn_span arms it in [init], which is why this cell cannot live there): on a
// Metal-capable build a >32-row undeclared batched prefill panics, media span included —
// the metal rail serves spans, so a CPU-served one is a silent fallback like any other.

def private llama2c_dir() : string {
    let env = g_env_test.llama2c_dir
    return empty(env) ? "/Users/borisbatkin/Work/llama2.c" : env
}
let MODEL_PATH = path_join(llama2c_dir(), "stories15M.bin")
let NROWS = 40l

[test]
def test_cpu_prefill_tripwire(t : T?) {
    t |> run("an undeclared prefill trips - span and causal alike (needs stories15M)") @(t : T?) {
        if (!stat(MODEL_PATH).is_valid) {
            t |> skip("stories15M.bin not present at {MODEL_PATH} (set DASLLAMA_LLAMA2C_DIR)")
            return
        }
        if (!has_prefill_override("metal")) {
            t |> success(true, "skipped: not a Metal-capable build - the tripwire never arms")
            return
        }
        var tr <- load_checkpoint(MODEL_PATH)
        var rows : array<float>
        rows |> reserve(NROWS * tr.config.dim)
        rows |> resize(NROWS * tr.config.dim)
        with_job_que() {
            setup_dasllama_jobque_()
            var s <- make_run_state(tr.config)
            var span_tripped = false
            try {
                forward_prefill_embd(tr, s, rows, NROWS, 0l, true)
            } recover {
                span_tripped = true
            }
            t |> success(span_tripped, "an undeclared {NROWS}-row SPAN prefill panics like any other")
            var tripped = false
            try {
                var s2 <- make_run_state(tr.config)
                forward_prefill_embd(tr, s2, rows, NROWS, 0l, false)
                delete s2
            } recover {
                tripped = true
            }
            t |> success(tripped, "an undeclared {NROWS}-row causal prefill still panics")
            // an mrope quantum is no longer exempt: metal serves it (the per-row-table
            // capability), so a CPU-served one is the same silent fallback as any other
            tr.config.rope_sections = int4(8, 8, 8, 0)   // stamp: the toy is not an mrope arch (rotary 48 -> half 24)
            tr.config.mrope_interleaved = true
            var mrope_tripped = false
            try {
                var s3 <- make_run_state(tr.config)
                eval_embd_span_mrope(tr, s3, rows, NROWS, 8l, 32l, int2(6, 4))
                delete s3
            } recover {
                mrope_tripped = true
            }
            t |> success(mrope_tripped, "an undeclared {NROWS}-row MROPE quantum panics like any other")
            delete s
        }
        delete rows
        delete tr
    }
}
