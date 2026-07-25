options gen2
options stack = 65536   // dasLLAMA program-root convention (options stack does not unify up from libs)
options _jit_fast_math = true   // ggml-parity FP laxity — matches the board cell's flags

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_math   // setup_dasllama_jobque_ + apply_box_profile_runtime (engine-level, not re-exported)
require dasllama/dasllama_image   // load_model_cached — the .dlim rail, without the full facade's compile weight
require ?das_metal dasllama/dasllama_metal_prefill   // nolint:STYLE030 — the skip setter + stage stats, Apple builds only
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030 — metal_dispatch_calls, Apple builds only
require ../performance/profile_common.das   // profile_threads + affinity_on — the standing per-box methodology
require daslib/jobque_boost
require daslib/clargs
require daslib/strings_boost   // nolint:STYLE030 — used only inside the Apple static_if half
require math   // nolint:STYLE030 — used only inside the Apple static_if half

// The Metal PREFILL chase harness — the pp-side twin of decode_metal_chase. Runs the same batched
// forward_prefill repeatedly with the resident GPU driver switched between knockout arms
// (interleaved across reps, so box noise hits every arm equally) and prints the driver's per-stage
// split next to the wall t/s. Arms are DASLLAMA_METAL_PREFILL_SKIP families ("full" = no skip):
//   full        the production path — the headline arm
//   gemm        minus every dense GEMM (qkv/wo/ffn; also gates the MoE mm block)
//   attn        minus attention (qk + softmax + av); finer: attn_qk | attn_sm | attn_av
//   ew          minus the elementwise set (rms/rope/bias/qknorm/swiglu/...)
//   nongemm     minus attention AND elementwise — GEMMs only
//   moe         minus the whole MoE FFN block
//   moe_mm      minus the gathered expert mul_mms only (route chain + act + reduce stay live)
//   moe_act     minus the MoE activation stage only
//   moe_reduce  minus the MoE combine/reduce stage only
//   moe_only_router | moe_only_sel | moe_only_route   isolation arms — ONLY that route-chain
//               prefix runs inside the MoE block
// Knockout arms compute GARBAGE tokens by design; timing is shape-bound, not data-bound. The
// warmup absorbs the fresh-process PSO-compile/first-touch stall (the sweep-skew culprit).
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/prefill_metal_chase.das -- \
//       -m <model.gguf> [-p 512] [-r 3] [--arms full,moe_mm,attn]

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string

    @clarg_short = "p"
    @clarg_doc = "Prefill window size in tokens (default: 512)"
    npos : int = 512

    @clarg_short = "r"
    @clarg_doc = "Interleaved repetitions of the arm set (default: 3)"
    reps : int = 3

    @clarg_doc = "Comma list of arms: full or any DASLLAMA_METAL_PREFILL_SKIP family (default: full,gemm,attn,ew,nongemm,moe,moe_mm,moe_act,moe_reduce)"
    arms : string = "full,gemm,attn,ew,nongemm,moe,moe_mm,moe_act,moe_reduce"

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// best (fastest-wall) window of an arm, whole-prefill figures in ms
struct private ArmBest {
    tps : double
    wall : double
    setup : double
    encode : double
    wait : double
    gpu : double
    rb : double
    disp : int64
}

def private build_prompt(n : int64) : array<int64> {
    return <- [for (i in range64(n)); 1000l + (i % 5000l)]
}

// one timed window: arm the skip, run one batched prefill, read the driver's stage split
def private run_window(t; var s : Session; prompt : array<int64>; npos : int64; arm : string; var best : ArmBest&) {
    set_metal_prefill_skip(arm == "full" ? "" : arm)
    metal_prefill_stage_reset()
    let d0 = metal_dispatch_calls()
    let t0 = ref_time_ticks()
    forward_prefill(t, s, prompt, npos, 0l)
    let us = get_time_usec(t0)
    set_metal_prefill_skip("")   // restore before anything else runs
    let st = metal_prefill_stage_stats()
    if (st.prefills != 1l) {
        print("  ⚠ {arm}: {st.prefills} gpu prefills in the window (expected 1) — declined to CPU?\n")
        return
    }
    let disp = metal_dispatch_calls() - d0
    let tps = double(npos) * 1.0e6lf / double(us)
    let w = ArmBest(tps = tps, wall = double(us) / 1000.0lf,
        setup = double(st.setup_us) / 1000.0lf, encode = double(st.encode_us) / 1000.0lf,
        wait = double(st.wait_us) / 1000.0lf, gpu = st.gpu_us / 1000.0lf,
        rb = double(st.readback_us) / 1000.0lf, disp = disp)
    print("  {arm}: {w.wall}ms {tps} t/s | setup {w.setup} encode {w.encode} wait {w.wait} (gpu {w.gpu}) rb {w.rb} ms  disp {disp}\n")
    if (w.tps > best.tps) {
        best = w
    }
}

def private run_chase(cfg) {   // untyped param = generic — non-Apple builds never compile the Metal-typed body
    // the standing per-box methodology (lcpp_bench's pattern): cap the future que's workers and
    // hard-pin lanes; explicit DAS_JOBQUE_THREADS / DAS_JOBQUE_AFFINITY envs still win
    let threads = has_env_variable("DAS_JOBQUE_THREADS") ? to_int(get_env_variable("DAS_JOBQUE_THREADS")) : profile_threads()
    if (!has_env_variable("DAS_JOBQUE_THREADS")) {
        set_jobque_threads_cap(max(threads - 1, 1))
    }
    if (affinity_on() && !has_env_variable("DAS_JOBQUE_AFFINITY")) {
        set_jobque_affinity(2)   // 2 = hard mask, the llama-bench --cpu-strict mirror
    }
    apply_box_profile_runtime()
    set_metal_mode(MetalMode.required)   // before load_model — pins portable, activates the metal overrides
    let arms <- [for (a in split(cfg.arms, ",")); a]
    let npos = int64(cfg.npos)
    print("loading {cfg.model}\n")
    with_job_que() {
        setup_dasllama_jobque_()
        var t <- load_model_cached(cfg.model, QuantMode.q8)
        t.config.seq_len = min(t.config.seq_len, npos + 8l)   // the board cell's KV cap
        let c = t.config
        print("config: dim={c.dim} layers={c.n_layers} heads={c.n_heads}/{c.n_kv_heads} vocab={c.vocab_size} | backend {active_kernel_backend()}\n")
        var s <- make_run_state(c)
        // warmup x2 — PSO compile + first-touch on the first, steady-state check on the second
        let prompt <- build_prompt(npos)
        forward_prefill(t, s, prompt, npos, 0l)
        forward_prefill(t, s, prompt, npos, 0l)
        var best : table<string; ArmBest>
        print("=== N={npos}: {cfg.reps} reps x {length(arms)} arms ===\n")
        for (rep in range(cfg.reps)) {
            print(" rep {rep}\n")
            for (arm in arms) {
                run_window(t, s, prompt, npos, arm, best[arm])
            }
        }
        print("--- best per arm @N={npos} (ms) ---\n")
        for (arm in arms) {
            let b = best[arm]
            print("  {arm}: {b.tps} t/s | wall {b.wall} setup {b.setup} encode {b.encode} wait {b.wait} (gpu {b.gpu}) rb {b.rb}  disp {b.disp}\n")
        }
        let fb = best["full"]
        if (fb.tps > 0.0lf) {
            print("--- Δ vs full (best windows; subtraction arms read as the class's GPU cost) ---\n")
            for (arm in arms) {
                continue if (arm == "full")
                let b = best[arm]
                continue if (b.tps <= 0.0lf)
                print("  {arm}: Δgpu {fb.gpu - b.gpu}ms Δwall {fb.wall - b.wall}ms Δdisp {fb.disp - b.disp}\n")
            }
        }
        delete s
        delete t
    }
}

[export]
def main() {
    if (!jit_enabled()) {
        print("This harness is JIT-only — run with: daslang -jit <file>\n")
        return
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "prefill_metal_chase")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "prefill_metal_chase")
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        run_chase(cfg)
    } else {
        print("das_metal is not built into this binary\n")
    }
}
