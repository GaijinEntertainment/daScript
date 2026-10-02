options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dasllama/dasllama_transformer   // nolint:STYLE029,STYLE030,LINT019 — umbrella fires each arch [init] registration; this file uses none of its re-exports (with dasMetal, only dasllama_common's)
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_load
require dasllama/dasllama_image   // nolint:STYLE030,LINT019 — load_model_cached, used only inside the das_metal static_if half
require dasllama/dasllama_env
require ?das_metal dasllama/dasllama_metal_prefill
require ?das_metal dasllama/dasllama_metal_common   // metal_kernel_coverage - the census arm's dispatch counts
require ../performance/profile_common.das   // tune_gate + apply_box_profile_runtime — the standing pre-measure stack
require daslib/jobque_boost
require daslib/clargs
require daslib/fio
require math   // nolint:STYLE030,LINT019 — used only inside the das_metal static_if half
require strings   // nolint:STYLE030,LINT019 — used only inside the das_metal static_if half
require ?das_metal _prefill_probe_fixture

// ATTRIBUTION SWEEP - its arms attribute the Metal prefill wall across kernel FAMILIES (gemm,
// moe, attn, the recurrent scan, ew, dense activation converts, weight staging), never selecting
// between implementations. Interleaved base/family knockout at one window size, best-of-3, base
// re-measured before every family (drift-guarded). "attn" knocks the recurrent scan out with the
// attention, "dn" the scan alone. Direction-grade; output is GARBAGE while a family is skipped.
//
// On a routed model a knocked-out family upstream of the router moves the expert GEMMs' cost with
// it, so the shares there are not additive: `--census` counts dispatches and `--depth` walks one
// session's windows with the attention's two forms alternating, both on valid rows.
//
//   bin/daslang -jit modules/dasLLAMA/harness/p0_ko_probe.das -- [-m <gguf>] [-n <npos>] [--fams a,b,c | --census | --depth <tokens> | --tall-avg a,b]

[CommandLineArgs]
struct ProbeArgs {
    @clarg_short = "m"
    @clarg_doc = "Model GGUF, a path or a name under the models dir (default Qwen3-8B-BF16.gguf)"
    model : string = "Qwen3-8B-BF16.gguf"

    @clarg_short = "n"
    @clarg_doc = "Window size in tokens (default 512)"
    npos : int = 512

    @clarg_doc = "Comma list of knockout families (default nofuse,gemm,moe,attn,dn,ew,act_cvt,devw_cvt,nongemm)"
    fams : string = "nofuse,gemm,moe,attn,dn,ew,act_cvt,devw_cvt,nongemm"

    @clarg_name = "tall-avg"
    @clarg_doc = "Race the routed tall pair's engage points instead (comma list, mean rows an expert): three interleaved rounds, each arm's logits held to the first arm's"
    tall_avg : string
    @clarg_name = "gather-min"
    @clarg_doc = "Race the window at which the routed gate and up read the gathered half panel instead (comma list, tokens), as --tall-avg races its engage points"
    gather_min : string

    @clarg_doc = "Print one window's dispatch count a kernel instead, most dispatched first, and a hash of its last-row logits"
    census : bool

    @clarg_doc = "Print one window's GPU ms a kernel instead, each dispatch timed alone in a command buffer of its own on the rows the window really computes, the costliest first"
    kprof : bool

    @clarg_doc = "Time each window of one session filled to this many tokens instead, so a window's cost reads against the context already behind it; a knockout family named by --fams (one) stays off for the walk"
    depth : int

    @clarg_doc = "The form the depth walk alternates held and serving: attn_dev (default), router_mm or dn_half"
    hold : string = "attn_dev"

    @clarg_name = "cpu-spin"
    @clarg_doc = "Keep the job queue's workers on the CPU spin window (30 ms) through the run instead of the GPU-served one - the control for what the spinning workers cost a GPU window"
    cpu_spin : bool

    @clarg_doc = "A text file whose tokens are the prompt, repeated past its end, instead of the synthetic ids - which route a MoE onto half its experts"
    text : string

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! one window's dispatches by kernel
def private census(tr : Model; npos : int64) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_kernel_coverage_reset()
        measure_one(tr, npos, 1)
        var inscope cov <- metal_kernel_coverage()
        var rows <- [for (k, n in keys(cov), values(cov)); (name = k, n = n); where n > 0l]
        sort(rows) $(a, b) => a.n > b.n
        var total = 0l
        for (r in rows) {
            total += r.n
            print("p0 census npos={npos} {r.n} {r.name}\n")
        }
        print("p0 census npos={npos} total {total} dispatches over {length(rows)} kernels\n")
        delete rows
        let engage = set_metal_moe_tall_avg(32l)
        var lg <- logits_at(tr, npos, engage, @@set_metal_moe_tall_avg)   //! restores the engage point it found, so the serving one comes back
        var h = 1469598103934665603ul
        for (v in lg) {
            h = (h ^ uint64(unsafe(reinterpret<uint>(v)))) * 1099511628211ul
        }
        print("p0 census npos={npos} last-row logits hash {h}\n")
        delete lg
    }
}

//! one session's windows in turn, each timed at the context it lands on - four walks, the form
//! held and serving alternating, the serving walk's last-row logits held to the held walk's
def private walk_depth(tr : Model; npos, depth : int64; skip, form : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var want : array<float>
        for (walk in range(4)) {
            let held = walk % 2 == 0
            let was = set_metal_prefill_held(form, held)
            var s <- make_run_state(tr.config)
            set_metal_prefill_skip(skip)
            for (at in range64(depth / npos)) {
                var toks <- probe_tokens(at * npos, npos)
                let t0 = ref_time_ticks()
                eval_(tr, s, toks)
                let ms = double(get_time_usec(t0)) / 1000.0lf
                print("p0 depth npos={npos} walk={walk} {form}={held ? "held" : "serving"} skip={skip} at={at * npos} ms={ms}\n")
                delete toks
            }
            set_metal_prefill_skip("")
            set_metal_prefill_held(form, was)
            if (walk == 0) {
                want := s.logits
            } elif (!held) {
                var diff = 0.0
                var mag = 0.0
                var top_w = 0
                var top_g = 0
                for (i, g, w in count(), s.logits, want) {
                    diff = max(diff, abs(g - w))
                    mag = max(mag, abs(w))
                    top_w = w > want[top_w] ? i : top_w
                    top_g = g > s.logits[top_g] ? i : top_g
                }
                print("p0 depth walk={walk} logits, {form} serving vs held: max difference {diff} of max {mag}, top token {top_g} vs {top_w}\n")
            }
            delete s
        }
        delete want
    }
}

//! one window's GPU ms by kernel, every dispatch alone in its own command buffer; `depth` tokens of
//! context sit behind the timed window
def private kprof(tr : Model; npos, depth : int64) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let plain = measure_one(tr, npos, 3)
        var s <- make_run_state(tr.config)
        let fill = max(npos, 2048l)   //! a short timed window's context fills at the serving quantum, not a window of its own size at a time
        var at = 0l
        while (at < depth) {
            var ctx <- probe_tokens(at, min(fill, depth - at))
            eval_(tr, s, ctx)
            at += long_length(ctx)
            delete ctx
        }
        var toks <- probe_tokens(depth, npos)
        metal_kernel_coverage_reset()
        metal_kernel_profile(true)
        metal_prefill_keep_moe_rows(true)
        eval_(tr, s, toks)
        var inscope ms <- metal_kernel_profile_ms()
        metal_kernel_profile(false)
        metal_prefill_keep_moe_rows(false)
        var inscope cnts <- metal_prefill_moe_rows()
        if (!empty(cnts)) {
            //! the routed rows as the expert tiles see them: whole 128-row tiles, the 32-row remainder tiles, their pad
            var rows = 0
            var tall = 0
            var rem = 0
            var most = 0
            var idle = 0
            for (n in cnts) {
                rows += int(n)
                tall += int(n) / 128
                rem += (int(n) % 128 + 31) / 32
                most = max(most, int(n))
                idle += n == 0u ? 1 : 0
            }
            print("p0 kprof npos={npos} at={depth} last routed layer: {rows} rows over {length(cnts)} experts ({idle} idle, the busiest {most}); {tall} tiles of 128 hold {tall * 128} rows, {rem} tiles of 32 hold the other {rows - tall * 128} in {rem * 32} padded\n")
        }
        var rows <- [for (k, v in keys(ms), values(ms)); (name = k, ms = v)]
        sort(rows) $(a, b) => a.ms > b.ms
        var total = 0.0lf
        for (r in rows) {
            total += r.ms
        }
        for (r in rows) {
            print("p0 kprof npos={npos} at={depth} {r.ms} ms {r.ms / total * 100.0lf}% {r.name}\n")
        }
        print("p0 kprof npos={npos} at={depth} total {total} ms over {length(rows)} kernels, each dispatch alone; the window at depth 0 as it serves: {plain} ms\n")
        delete rows
        delete toks
        delete s
    }
}

typedef LeverFn = function<(v : int64) : int64>   //! sets an engage point, answers the one it replaced

//! the window's last-row logits under one engage point
def private logits_at(tr : Model; npos, point : int64; lever : LeverFn) : array<float> {
    var out : array<float>
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let was = invoke(lever, point)
        var s <- make_run_state(tr.config)
        var toks <- probe_tokens(0l, npos)
        eval_(tr, s, toks)
        out := s.logits
        invoke(lever, was)
        delete toks
        delete s
    }
    return <- out
}

//! every engage point timed once a round in turn, its logits' largest difference from the first arm's beside it
def private race_lever(tr : Model; npos : int64; arms : array<string>; name : string; lever : LeverFn) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var want <- logits_at(tr, npos, to_int64(arms[0], false), lever)
        for (round in range(3)) {
            for (a in arms) {
                let avg = to_int64(a, false)
                let was = invoke(lever, avg)
                let ms = measure_one(tr, npos, 3)
                invoke(lever, was)
                var got <- logits_at(tr, npos, avg, lever)
                var diff = 0.0
                var mag = 0.0
                for (g, w in got, want) {
                    diff = max(diff, abs(g - w))
                    mag = max(mag, abs(w))
                }
                print("p0 race npos={npos} round={round} {name}={a} ms={ms} | logits vs {name}={arms[0]}: max difference {diff} of max {mag}\n")
                delete got
            }
        }
        delete want
    }
}

def private race_arms(tr : Model; npos : int64; list, name : string; lever : LeverFn) {
    return if (empty(list))
    var arms <- split(list, ",")
    race_lever(tr, npos, arms, name, lever)
    delete arms
}

[export, arch(at="../ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig")]
def main {
    var r <- parse_args(type<ProbeArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<ProbeArgs>), "p0_ko_probe")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<ProbeArgs>), "p0_ko_probe")
        return
    }
    let env = g_env_harness.models_dir
    let mdir = empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
    let path = stat(cfg.model).is_valid ? cfg.model : path_join(mdir, cfg.model)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_mode(MetalMode.required)   //! before load - pins the metal serving, the bench's --ngl leg
        var tr <- load_model_cached(path, QuantMode.q8)
        if (!tr.metal_blob) {
            panic("p0: the mapped flavor is not metal_blob - wrong leg")
        }
        let npos = int64(cfg.npos)
        tr.config.seq_len = max(2048l, max(npos, int64(cfg.depth) + (cfg.kprof ? npos : 0l)))
        apply_box_profile_runtime()
        if (!tune_gate()) {
            panic("p0: tune_gate refused - tune first (or point DAS_TUNE_MANIFEST at a complete sidecar)")
        }
        if (!empty(cfg.text)) {
            var src = ""
            fopen(cfg.text, "rb") $(f) {
                if (f == null) {
                    panic("p0: cannot read --text {cfg.text}")
                }
                src = fread(f)
            }
            var toks <- encode_(tr, src, true, false)
            print("p0 prompt: {long_length(toks)} tokens of {cfg.text}\n")
            probe_set_text(toks)
            delete toks
        }
        with_dasllama_jobque_() {
            measure_one(tr, npos, 1)   //! warm: JIT + blob upload + first graphs
            if (cfg.cpu_spin) {
                set_dispatch_gpu_served(false)   //! the served window above switched it; no load follows to switch it back
            }
            //! "nofuse" matches no skip family: every kernel encodes with full math, so this arm
            //! is the null control (base-as-candidate) - and any empty-skip-gated fused route's
            //! on/off A/B, output VALID either way
            if (cfg.census) {
                census(tr, npos)
            }
            race_arms(tr, npos, cfg.tall_avg, "tall_avg", @@set_metal_moe_tall_avg)
            race_arms(tr, npos, cfg.gather_min, "gather_min", @@set_metal_moe_gather_min)
            if (cfg.kprof) {
                kprof(tr, npos, int64(cfg.depth))
            } elif (cfg.depth > 0) {
                walk_depth(tr, npos, int64(cfg.depth), find(cfg.fams, ",") >= 0 ? "" : cfg.fams, cfg.hold)
            }
            var fams <- split(empty(cfg.tall_avg) && empty(cfg.gather_min) && !cfg.census && !cfg.kprof && cfg.depth == 0 ? cfg.fams : "nofuse", ",")
            for (fam in fams) {
                let base = measure_one(tr, npos, 3)
                set_metal_prefill_skip(fam)
                let ms = measure_one(tr, npos, 3)
                set_metal_prefill_skip("")
                print("p0 ko npos={npos} base={base} skip={fam} ms={ms} delta_ms={base - ms} share={(base - ms) / base} (output GARBAGE - timing attribution only)\n")
            }
            delete fams
            let base_post = measure_one(tr, npos, 3)
            print("p0 ko base(post)={base_post}ms\n")
        }
        delete tr
    } else {
        panic("p0: no das_metal in this build - nothing to knock out for {path}, and a 0-row run must not read as a clean one")
    }
}
