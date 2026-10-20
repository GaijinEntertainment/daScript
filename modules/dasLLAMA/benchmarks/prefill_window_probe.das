options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dasllama/dasllama
require dasllama/dasllama_common   // apply_box_profile_runtime, set_metal_mode, the route knee setter
require ?das_metal dasllama/dasllama_metal_prefill   // the driver's per-window stage report
require ?das_metal dasllama/dasllama_metal_common    // the decode stage report, the per-kernel profile
require ../performance/profile_common.das   // nolint:STYLE029,LINT019 - tune_gate, used only inside the das_metal static_if halves
require daslib/clargs
require daslib/cmp   // nolint:STYLE030,LINT019 - cmp_by, used only inside the das_metal static_if halves
require daslib/strings_boost   // nolint:STYLE030,LINT019 - split, used only inside the das_metal static_if halves
require strings   // nolint:STYLE030,LINT019 - to_int, used only inside the das_metal static_if halves
require math   // nolint:STYLE030,LINT019 - min, used only inside the das_metal static_if halves

// The short-window prefill probe: the Metal prefill driver's own stage report (GPU timestamps, the encode and
// wait walls, the readback) for windows of 2 to 512 tokens appended to a warm prefix, each a median over the
// reps after the first (the warmup rep), beside a decode step at the same position and, on request, the
// per-kernel GPU split of chosen window sizes (every captured node timed alone). The numbers are the driver's
// report, not a clock of this file's; `ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-gemv-route` reads them.
//
//   bin/daslang -jit modules/dasLLAMA/benchmarks/prefill_window_probe.das -- -m <model.gguf> [--prefix 2000] [-r 4]
//       [--kprof 2,8,64] [--gemv-max N] [--cold --windows 2967]
//
// The sidecar beside this file (`prefill_window_probe.tune.json`, copied from the box's) applies the box's Metal
// crowns; without one every tensor twin runs its base form, and the report says so. A window the driver declines
// prints its refusal and no number. The run exits non-zero when no window row was served.
// `tune_gate()` runs before the first rep: a run on untuned CPU
// winners exits non-zero (DASLLAMA_ALLOW_UNTUNED=1 measures anyway,
// stamped not publishable).
// ATTRIBUTION SWEEP --ab: window GPU ms to each lever arm, no compare
// ATTRIBUTION SWEEP --cold: GPU ms to a cold first window, no compare
// `--ab devw` prints a window whose A arm ran no dev-W site as inert,
// with no number, and exits non-zero when every window was inert.

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "Model GGUF path"
    model : string
    @clarg_doc = "warm prefix tokens ahead of the windows (default 2000)"
    prefix : int = 2000
    @clarg_short = "r"
    @clarg_doc = "reps per window size, the first a warmup (default 4)"
    reps : int = 4
    @clarg_doc = "per-kernel profile of these window sizes, comma list (default none)"
    kprof : string = ""
    @clarg_doc = "kernel rows to print per profiled window (default 24)"
    top : int = 24
    @clarg_doc = "the routed block's gathered-route knee in tokens (default -1: the box profile's)"
    gemv_max : int = -1
    @clarg_doc = "an interleaved A/B over every window size, one process: `knee` = the gathered route (knee 512) against the tiles (knee 0), `tail` = the K-quant GEMV-tail peel on against off, `devw` = the dev-W route on against off (the K-quant sites' scratch f16 panels against their own bytes); reps a window per arm, alternating"
    ab : string = ""
    @clarg_doc = "window sizes to sweep, comma list (default 2,3,4,8,12,16,24,32,48,64,96,128,256,512)"
    windows : string = ""
    @clarg_doc = "cold windows: every rep and every profiled window runs on a fresh session from position 0 - the served first prefill - and no prefix is evaluated"
    cold : bool
    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let WINDOWS <- [2, 3, 4, 8, 12, 16, 24, 32, 48, 64, 96, 128, 256, 512]

def private median(var xs : array<double>) : double {
    sort(xs)
    let n = length(xs)
    return n % 2 == 1 ? xs[n / 2] : 0.5lf * (xs[n / 2 - 1] + xs[n / 2])
}

//! the samples' coefficient of variation in percent - a row past 3% is noise, not a reading
def private cv_pct(xs : array<double>) : double {
    let n = double(length(xs))
    var mean = 0.0lf
    for (x in xs) {
        mean += x / n
    }
    var vsum = 0.0lf
    for (x in xs) {
        vsum += (x - mean) * (x - mean) / n
    }
    return mean > 0.0lf ? 100.0lf * sqrt(vsum) / mean : 0.0lf
}

//! the token pool the prefix and every window read from: house notes, encoded as many times as the run needs
def private token_pool(m : Model; need : int) : array<int64> {
    let text = ("The kitchen light is on, the bedroom lights are off, the sprinkler runs at seven in the morning and again at dusk. " +
        "The thermostat holds twenty-one degrees while anyone is home and drops to eighteen overnight. " +
        "The garage door reports closed; the front door camera saw a courier at ten past two. " +
        "Groceries arrive on Thursday; the dishwasher finished its cycle an hour ago and wants emptying. ")
    var pool : array<int64>
    while (length(pool) < need) {
        pool |> push_from(encode(m, text, empty(pool)))
    }
    return <- pool
}

//! one window of `n` tokens off the pool at `cursor` with every captured node timed alone, the top rows printed
def private kernel_profile(m : Model; var s : Session; pool : array<int64>; var cursor : int&; n, top : int) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope win <- [for (i in range(n)); pool[cursor + i]]
        cursor += n
        metal_kernel_profile(true)
        eval(m, s, win)
        var inscope prof <- metal_kernel_profile_ms()
        metal_kernel_profile(false)
        var inscope rows <- [for (k, v in keys(prof), values(prof)); (name = k, ms = v)]
        var total = 0.0lf
        for (r in rows) {
            total += r.ms
        }
        sort(rows) <| cmp_by(-_.ms)
        print("\n--- per-kernel profile, npos={n}: {length(rows)} distinct dispatch shapes, {total} ms summed (each node alone)\n")
        for (i in range(min(top, length(rows)))) {
            print("{rows[i].ms}  {rows[i].name}\n")
        }
    }
}

//! twelve decode steps off the pool at `cursor`, the driver's stage report a step
def private decode_reference(m : Model; var s : Session; pool : array<int64>; cursor : int) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_decode_stage_reset()
        for (i in range(12)) {
            eval(m, s, [pool[cursor + i]])
        }
        let ds = metal_decode_stage_stats()
        if (ds.steps != 12l) {
            print("\ndecode step at pos {s.n_past}: REFUSED - the Metal driver served {ds.steps} of 12 steps (the rest fell to the CPU rails), no number\n")
            return
        }
        print("\ndecode step x{ds.steps} at pos {s.n_past}: gpu {ds.gpu_us / double(ds.steps) / 1000.0lf} ms  encode {double(ds.encode_us) / double(ds.steps) / 1000.0lf} ms  wait {double(ds.wait_us) / double(ds.steps) / 1000.0lf} ms\n")
    }
}

def private sweep(a : Args) : int {   // nolint:STYLE038 — one flat sweep: the warm prefix, the window ladder with its A/B arms, the profile and the decode row
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope windows <- empty(a.windows) ? clone_to_move(WINDOWS) : [for (w in split(a.windows, ",")); to_int(w); where to_int(w) > 0]
        var widest = 0
        for (w in windows) {
            widest = max(widest, w)
        }
        for (ks in split(a.kprof, ",")) {
            widest = max(widest, to_int(ks))
        }
        set_metal_mode(MetalMode.required)
        set_gpu_resident_regions(1l)
        set_gpu_ctx_max(a.cold ? int64(max(8192, widest + 64)) : 8192l)
        set_gpu_ctx_strict(true)
        set_gpu_kv_dtype(KVDtype.f16)
        apply_box_profile_runtime()
        return 1 if (!tune_gate())
        if (a.gemv_max >= 0) {
            set_metal_moe_gemv_max(int64(a.gemv_max))
        }
        if (!empty(a.ab) && a.ab != "knee" && a.ab != "tail" && a.ab != "devw") {
            print("prefill_window_probe: --ab takes `knee`, `tail` or `devw`, not `{a.ab}`\n")
            return 2
        }
        let knee_before = metal_moe_gemv_max()
        let tail_before = get_metal_prefill_mm_tail()
        let devw_before = get_metal_prefill_dev_w()
        var rows_served = 0
        var rows_inert = 0
        with_dasllama_jobque() {   // nolint:STYLE037,LINT019 — the sweep's one block: the ladder and its A/B arms
            var m <- load_model(a.model, QuantMode.q8)
            var s <- create_session(m)
            let pool <- token_pool(m, a.cold ? widest + 16 : a.prefix + 6000)   //! the decode row reads twelve tokens past the last window
            print("model {m.config.dim}d x {m.config.n_layers}L vocab {m.config.vocab_size}; metal mode {get_metal_mode()}; gathered-route knee {pf_moe_gemv_knee()} tokens\n")
            if (!a.cold) {
                var prefix <- [for (i in range(a.prefix)); pool[i]]
                eval(m, s, prefix)
                print("warm prefix {s.n_past} tokens done\n")
            }
            var cursor = a.cold ? 0 : a.prefix
            let from = a.cold ? "each on a fresh session from 0" : "windows appended in sequence from {a.prefix}"
            if (empty(a.ab)) {
                print("\nnpos  gpu_ms  encode_ms  wait_ms  rb_ms  ms/tok  gpu_cv%  (medians of {a.reps - 1} after the warmup rep; {from})\n")
            } else {
                let arms = a.ab == "knee" ? "route (knee 512) vs tiles (knee 0)" : (a.ab == "tail" ? "peel on vs off" : "scratch panels vs own bytes")
                print("\nnpos  A_ms  B_ms  A/B  A_cv%  B_cv%  ({a.ab}: {arms}; medians of {a.reps} a arm, the arms alternating after one warmup rep of A; {from})\n")
            }
            for (n in windows) {
                var inscope gpu, enc, wait, rb, gpu_b : array<double>
                var declined = 0
                var devw_sites = 0l   //! the dev-W sites arm A's timed reps ran
                let reps = empty(a.ab) ? a.reps : 1 + 2 * a.reps
                for (r in range(reps)) {
                    let arm_b = !empty(a.ab) && r > 0 && (r - 1) % 2 == 1
                    if (a.ab == "knee") {
                        set_metal_moe_gemv_max(arm_b ? 0l : 512l)
                    } elif (a.ab == "tail") {
                        set_metal_prefill_mm_tail(!arm_b)
                    } elif (a.ab == "devw") {
                        set_metal_prefill_dev_w(!arm_b)
                    }
                    if (a.cold) {
                        delete s
                        s <- create_session(m)
                        cursor = 0
                    }
                    var inscope win <- [for (i in range(n)); pool[cursor + i]]
                    cursor += n
                    metal_prefill_stage_reset()
                    let declines0 = metal_prefill_stats().declines
                    let devw0 = metal_prefill_devw_scratch_stats().sites
                    eval(m, s, win)
                    let st = metal_prefill_stage_stats()
                    if (r > 0 && !arm_b) {
                        devw_sites += metal_prefill_devw_scratch_stats().sites - devw0
                    }
                    if (st.prefills != 1l) {
                        declined++
                        print("{n}  declined ({metal_prefill_stats().declines - declines0} decline(s) counted)\n")
                        continue
                    }
                    if (r == 0) {
                        continue
                    }
                    if (arm_b) {
                        gpu_b |> push(st.gpu_us / 1000.0lf)
                        continue
                    }
                    gpu |> push(st.gpu_us / 1000.0lf)
                    enc |> push(double(st.encode_us) / 1000.0lf)
                    wait |> push(double(st.wait_us) / 1000.0lf)
                    rb |> push(double(st.readback_us) / 1000.0lf)
                }
                if (declined > 0 || empty(gpu)) {
                    continue
                }
                if (a.ab == "devw" && devw_sites == 0l) {
                    print("{n}  inert: arm A ran no dev-W site, the devw lever changed nothing\n")
                    rows_inert++
                    continue
                }
                let g = median(gpu)
                if (empty(a.ab)) {
                    print("{n}  {g}  {median(enc)}  {median(wait)}  {median(rb)}  {g / double(n)}  {cv_pct(gpu)}\n")
                } else {
                    let gb = median(gpu_b)
                    print("{n}  {g}  {gb}  {g / gb}  {cv_pct(gpu)}  {cv_pct(gpu_b)}\n")
                }
                rows_served++
            }
            if (!empty(a.ab)) {   //! the ladder ends on an arm-B rep: the profiles and the decode row below run at the settings the run started with
                set_metal_moe_gemv_max(knee_before)
                set_metal_prefill_mm_tail(tail_before)
                set_metal_prefill_dev_w(devw_before)
            }
            for (ks in split(a.kprof, ",")) {
                let n = to_int(ks)
                if (n > 0) {
                    if (a.cold) {
                        delete s
                        s <- create_session(m)
                        cursor = 0
                    }
                    kernel_profile(m, s, pool, cursor, n, a.top)
                }
            }
            decode_reference(m, s, pool, cursor)
            delete s
            delete m
        }
        if (rows_served == 0) {
            let why = rows_inert > 0 ? "the devw lever was inert" : "every window declined"
            print("no window row served - {why}\n")
            return 1
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
        print_help(get_command_info(type<Args>), "prefill_window_probe")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help || empty(a.model)) {
        print_help(get_command_info(type<Args>), "prefill_window_probe")
        return a.help ? 0 : 2
    }
    return sweep(a)
}
