options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dasllama/dasllama   // nolint:STYLE029,LINT019 - the facade's Metal entries, used only inside the das_metal half
require dasllama/dasllama_common   // apply_box_profile_runtime, set_metal_mode, the overrides, eval_
require dasllama/dasllama_math   // parallel_argmax
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - set_metal_kq_fat, inside the das_metal half
require ../performance/profile_common.das   // nolint:STYLE029,LINT019 - tune_gate, inside the das_metal half
require daslib/clargs
require daslib/jobque_boost
require math

// The served single row under DASLLAMA_METAL_KQ_FAT 0 against the same row under the asked mode, teacher-forced: the plain
// chain's greedy tokens over a short prompt are fed to both, and every step's logits are compared - the instrument for a
// mode-level drift on a model whose CPU chain does not fit the box.
//
//   bin/daslang -jit modules/dasLLAMA/harness/fat_mode_ab_probe.das -- -m <model.gguf> [--steps 32] [--mode 2]

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string
    @clarg_doc = "forced decode steps after the prompt (default 32)"
    steps : int = 32
    @clarg_doc = "the mode compared against the plain chain's mode 0 (default 2)"
    mode : int = 2
    @clarg_doc = "prefill the prompt on the CPU rails (the device prefill left out of the compare)"
    cpu_prefill : bool
    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let PROMPT = "The kitchen light is on, the bedroom lights are off, and the sprinkler runs at seven in the morning. Describe the house's evening routine in detail."

def private argmax_row(v : array<float>) : int64 => parallel_argmax(v, long_length(v))

def private maxdiff(a, b : array<float>) : float {
    var d = 0.0
    for (i in range64(min(long_length(a), long_length(b)))) {
        d = max(d, abs(a[i] - b[i]))
    }
    return d
}

//! the prompt and `steps` forced steps on the device under `mode`: the logits a step, the forced tokens read from `forced` where
//! it holds them, else the step's own argmax appended to it
def private walk(m : Model; mode, steps : int; cpu_prefill : bool; prompt : array<int64>; var forced : array<int64>) : array<array<float>> {
    var out : array<array<float>>
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_kq_fat(mode)
        var s = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
        if (cpu_prefill) {
            allow_cpu_prefill(true)
            eval_(m, s, prompt)
        } else {
            select_prefill_override("metal")
            eval_(m, s, prompt)
            select_prefill_override("")
        }
        select_decode_override("metal")
        var one <- [0l]
        out |> reserve(steps + 1)
        var pl : array<float>   //! the prompt's last-row logits first: the prefill's own compare
        pl := s.logits
        out |> emplace(pl)
        for (i in range(steps)) {
            if (i < length(forced)) {
                one[0] = forced[i]
            } else {
                one[0] = argmax_row(s.logits)
                forced |> push(one[0])
            }
            eval_(m, s, one)
            var l : array<float>
            l := s.logits
            out |> emplace(l)
        }
        select_decode_override("")
        delete one
        delete s
    }
    return <- out
}

def private run(a : Args) : int {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_mode(MetalMode.required)
        apply_box_profile_runtime()
        return 1 if (!tune_gate())
        with_dasllama_jobque() {
            var m <- load_model(a.model, QuantMode.q8)
            let prompt <- encode(m, PROMPT, true)
            var forced : array<int64>
            var ref <- walk(m, 0, a.steps, a.cpu_prefill, prompt, forced)
            var got <- walk(m, a.mode, a.steps, a.cpu_prefill, prompt, forced)
            var worst = 0.0
            var flips = 0
            print("prefill: last-row logits maxdiff {maxdiff(ref[0], got[0])} (argmax {argmax_row(ref[0])} vs {argmax_row(got[0])})\n")
            for (i in range(a.steps)) {
                let d = maxdiff(ref[i + 1], got[i + 1])
                let a0 = argmax_row(ref[i + 1])
                let a1 = argmax_row(got[i + 1])
                worst = max(worst, d)
                if (a0 != a1) {
                    flips++
                }
                print("step {i}: fed {forced[i]} | mode 0 argmax {a0} '{decode(m, [a0])}' | mode {a.mode} argmax {a1} '{decode(m, [a1])}' | maxdiff {d}\n")
            }
            print("mode {a.mode} vs mode 0 over {a.steps} forced steps: worst maxdiff {worst}, argmax flips {flips}\n")
            delete ref
            delete got
            delete m
        }
        return 0
    } else {
        print("das_metal is not built into this binary\n")
        return 1
    }
}

[export]
def main : int {
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        print_help(get_command_info(type<Args>), "fat_mode_ab_probe")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help || empty(a.model)) {
        print_help(get_command_info(type<Args>), "fat_mode_ab_probe")
        return a.help ? 0 : 2
    }
    return run(a)
}
