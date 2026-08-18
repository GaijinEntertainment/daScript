options gen2
options persistent_heap
options _dasllama_internal = true

// Diagnostic twin of test_parity_gptoss_20b: same pinned-oracle path (classic prefill attention +
// scalar libm act + raw greedy generate), but prints got-vs-want with the divergence index, and
// honors DASLLAMA_PIN_BACKEND (pin BEFORE load, so select_matmul_backend_for_load_ obeys).
// Run: daslang -jit gptoss_parity_probe.das   [DASLLAMA_PIN_BACKEND=portable|x64-gen]

require dasllama/dasllama
require dasllama/dasllama_math
require daslib/jobque_boost
require strings
require dasllama/dasllama_env

let GPTOSS_PROMPT <- [16l, 11l, 220l, 17l, 11l, 220l, 18l, 11l, 220l, 19l,
                      11l, 220l, 20l, 11l, 220l, 21l, 11l, 220l, 22l, 11l]
let GPTOSS_GEN <- [220l, 23l, 11l, 220l, 24l, 11l, 220l, 702l, 11l, 220l,
                   994l, 11l, 220l, 899l, 11l, 220l, 1311l, 11l, 220l, 1265l,
                   11l, 220l, 1055l, 11l, 220l, 1125l, 11l, 220l, 1422l, 11l,
                   220l, 1157l, 11l, 220l, 858l, 11l, 220l, 455l, 11l, 220l]
let GPTOSS_LONG_GEN <- [220l, 17395l, 11l, 220l, 17336l, 11l, 220l, 17773l, 11l, 220l,
                        19145l, 11l, 220l, 17385l, 11l, 220l, 18369l, 11l, 220l, 19676l,
                        11l, 220l, 19548l, 11l, 220l, 19218l, 11l, 220l, 9444l, 11l,
                        220l, 18881l, 11l, 220l, 18694l, 11l, 220l, 17666l, 11l, 220l]

def counting_prompt() : string {
    return build_string() $(var w) {
        for (i in range(1, 151)) {
            w |> write(i)
            w |> write(i < 150 ? ", " : ",")
        }
    }
}

def gen_and_compare(tr : Model; prompt, want : array<int64>; tag : string) {
    let n_gen = length(want)
    var s = make_run_state(tr.config)
    let ids <- generate(tr, s, prompt, length(prompt) + n_gen)
    let echo = length(prompt) - 1
    let got <- [for (i in range(n_gen)); ids[echo + i]; where echo + i < length(ids)]
    var first = -1
    for (i in range(length(want))) {
        if (i >= length(got) || got[i] != want[i]) {
            first = i
            break
        }
    }
    if (first < 0) {
        print("{tag}: MATCH ({n_gen} tokens)\n")
    } else {
        print("{tag}: DIVERGES at token {first} (got {first < length(got) ? got[first] : -1l} want {want[first]})\n")
        print("  got:  {got}\n")
        print("  want: {want}\n")
    }
    delete s
}

[export]
def main {
    let pin = g_env_engine.pin_backend
    if (!empty(pin)) {
        pin_kernel_backend(pin)
        print("pinned backend: {pin}\n")
    }
    let path = "D:\\Work\\llama.cpp\\models\\gpt-oss-20b-mxfp4.gguf"
    var tr <- load_model(path, QuantMode.q8)
    if (tr.config.seq_len > 2048l) {
        tr.config.seq_len = 2048l
    }
    print("active backend: {active_kernel_backend()}\n")
    let prev_attn = get_prefill_attn_mode()
    set_prefill_attn_mode(AttnPrefillMode.classic)
    let prev_act = get_act_vec()
    set_act_vec(false)
    with_job_que() {
        setup_dasllama_jobque()
        gen_and_compare(tr, GPTOSS_PROMPT, GPTOSS_GEN, "short (counting)")
        let long_prompt <- encode(tr, counting_prompt(), true, false)
        gen_and_compare(tr, long_prompt, GPTOSS_LONG_GEN, "window-engaged (449 tok)")
    }
    set_prefill_attn_mode(prev_attn)
    set_act_vec(prev_act)
    delete tr
}
