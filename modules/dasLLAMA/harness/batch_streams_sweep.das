options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// The batched decode's stream sweep on the armed GPU home: one model, loaded once with as many K/V regions as
// the widest count, then one plain batched decode row a stream count - the total tok/s over every stream, the
// rows the home's N-row command steps at once, and the row's refusal where a step left the device. A total that
// falls or flattens between two counts names a cap. The home is the environment's: DASLLAMA_GPU=1 for Vulkan,
// DASLLAMA_CUDA=1 beside it for CUDA.
//
//   DASLLAMA_GPU=1 bin/daslang -jit modules/dasLLAMA/harness/batch_streams_sweep.das -- -m <model.gguf>
//       [--streams 2,4,8,12,16,32] [--plen 32] [--ngen 64] [--reps 3] [--reload]

require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama   // load_model, set_gpu_resident_regions
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_tier   // gpu_cpu_passes_
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, gpu_resident_decline_, gpu_resident_assist_rows
require dasllama/dasllama_bench   // bench_tg_batched_rep
require _prompt_files   // id_list
require ?cuda dasllama/dasllama_cuda_common   // nolint:STYLE030,LINT019 - the launch timing rail, inside the CUDA half
require ?vulkan dasllama/dasllama_vulkan_common   // nolint:STYLE030,LINT019 - the module the Vulkan half is gated on
require ?vulkan dasllama/dasllama_vulkan_decode   // nolint:STYLE030,LINT019 - the pricing rail, inside the Vulkan half
require ?cuda _cuda_time   // nolint:STYLE030,LINT019 - the profile rail's report, inside the CUDA half
require ?cuda dasllama/dasllama_cuda_decode   // nolint:STYLE030,LINT019 - set_cuda_moe_fp4_rows, inside the CUDA half
require daslib/jobque_boost
require daslib/clargs
require math

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "The model's GGUF"
    model : string
    @clarg_doc = "Stream counts, comma-separated, each 2 or more"
    streams : string = "2,4,8,12,16,32"
    @clarg_doc = "Prompt tokens a stream"
    plen : int = 32
    @clarg_doc = "Timed decode ticks a rep"
    ngen : int = 64
    @clarg_doc = "Reps a count; the row reports the best and the spread"
    reps : int = 3
    @clarg_doc = "Load the model again for every count, with exactly that count's regions"
    reload : bool
    @clarg_doc = "Every stream takes one prompt: a routed model's streams pick the same experts (llama-batched-bench feeds each stream its own tokens)"
    same : bool
    @clarg_doc = "CUDA home: the rows from which a routed layer's step takes the FP4 expert chain (0 = never; absent = the driver's rule)"
    fp4_rows : int = -1
    @clarg_doc = "CUDA home: the same for a routed layer on the f16-fed expert tiles"
    tile_rows : int = -1
    @clarg_doc = "Comma-separated kernel-name fragments whose launches are dropped from the captured commands, on either home - a run with and without prices a kernel class; the tokens are garbage"
    skip : string
    @clarg_doc = "CUDA home: after the rows, each kernel's wall a tick at the widest count, every launch waited (the prompts' window kernels ride along)"
    times : bool
    @clarg_doc = "Beside each row, the same step as a segmented window: every stream's one token a segment"
    segs : bool
    @clarg_short = "h"
    @clarg_doc = "Show this help and exit"
    help : bool
}

//! `n` sessions' step as ONE segmented window, a segment of SEG_ROWS tokens a session (a one-token chunk is the decode
//! step's by the engine's rule): rows a second over `ngen` windows, and the driver's prefills the timed windows counted
//! (n a window where every window rode the device)
let private SEG_ROWS = 2l

[cold_path]
def private segs_rep(m : Model; n, plen, ngen, seed : int64) : tuple<tps : double; prefills : int64> {
    var sessions : array<Session>
    sessions |> reserve(n)
    for (i in range64(n)) {
        sessions |> emplace(create_device_session(m, gpu_device_session_dtype()))
        var prompt <- bench_prompt(plen, seed * 64l + i + 1l)
        eval(m, sessions[i], prompt)
        delete prompt
    }
    var ptrs : array<Session?>
    ptrs |> reserve(n)
    for (s in sessions) {
        ptrs |> push(unsafe(addr(s)))
    }
    var ns = 0l
    var pf0 = 0l
    for (step in range64(ngen + 1l)) {
        var chunks <- [for (i in range64(n)); bench_prompt(SEG_ROWS, seed * 4096l + step * 64l + i)]
        if (step == 1l) {   //! step 0 is the untimed warm step
            pf0 = gpu_resident_prefills_()
        }
        let t0 = ref_time_ticks()
        eval_segments(m, ptrs, chunks)
        ns += step > 0l ? get_time_nsec(t0) : 0l
        delete chunks
    }
    let prefills = gpu_resident_prefills_() - pf0
    ptrs |> clear()   //! borrowed: the sessions array owns them
    for (s in sessions) {
        gpu_device_kv_park(s)   //! a deleted session keeps its region pinned
    }
    unsafe {
        delete ptrs
    }
    delete sessions
    return (tps = double(n * ngen * SEG_ROWS) * 1.0e9lf / double(max(ns, 1l)), prefills = prefills)
}

[cold_path]
def private sweep_row(m : Model; a : Args; n : int64) {
    var best = 0.0lf
    var worst = 0.0lf
    var why = ""
    for (rep in range(a.reps)) {
        let r = bench_tg_batched_rep(m, n, int64(a.plen), int64(a.ngen), int64(rep) + 1l, false, KVDtype.f16, a.same)
        if (!empty(r.why)) {
            why = r.why
            break
        }
        best = max(best, r.tps)
        worst = rep == 0 ? r.tps : min(worst, r.tps)
    }
    if (!empty(why)) {
        print("streams {n}: refused - {why}\n")
        return
    }
    let spread = best > 0.0lf ? (best - worst) * 100.0lf / best : 0.0lf
    print("streams {n}: {best:.1f} tok/s total, {best / double(n):.1f} a stream, spread {spread:.1f}% over {a.reps} reps\n")
    static_if (typeinfo module_exists(dasllama_cuda_decode)) {
        let sh = cud_rdec_moe_share()
        if (sh.slots > 0l) {
            print("  routed: {sh.distinct} experts over {sh.slots} slots at the last counted step, the expert chain {sh.wide ? "on" : "off"} ({sh.ne} experts, {sh.k} picks a row, {sh.nfe} x {sh.dim} a stack)\n")
        }
    }
    if (a.segs) {
        var sbest = 0.0lf
        var counted = 0l
        for (rep in range(a.reps)) {
            let r = segs_rep(m, n, int64(a.plen), int64(a.ngen), int64(rep) + 1l)
            sbest = max(sbest, r.tps)
            counted = r.prefills
        }
        print("  as segments: {sbest:.1f} rows/s over windows of {n * SEG_ROWS} rows, {counted} device prefill(s) over {a.ngen} windows of {n} segments\n")
    }
}

//! one rep at `n` streams under the launch timing rail: each kernel's wall a timed tick, widest first
[cold_path]
def private times_row(m : Model; a : Args; n : int64) {
    static_if (typeinfo module_exists(_cuda_time)) {
        cud_launch_timing(true)
        cuda_times_drop()
        bench_tg_batched_rep(m, n, int64(a.plen), int64(a.ngen), 1l, false, KVDtype.f16, a.same)
        cud_launch_timing(false)
        cuda_times_report("{n} streams", int64(a.ngen + 1), 0.0lf, "ms a tick in the timed launches")
    }
}

[cold_path]
def private sweep_load(a : Args; counts : array<int64>; regions : int64) : bool {
    set_gpu_resident_regions(regions)
    static_if (typeinfo module_exists(dasllama_cuda_decode)) {
        if (a.fp4_rows >= 0) {
            set_cuda_moe_fp4_rows(int64(a.fp4_rows))
        }
        if (!empty(a.skip)) {
            set_cuda_kernel_skip(a.skip)
        }
        if (a.tile_rows >= 0) {
            set_cuda_moe_tile_rows(int64(a.tile_rows))
        }
    }
    static_if (typeinfo module_exists(dasllama_vulkan_common)) {
        if (!empty(a.skip)) {
            set_vulkan_kernel_skip(a.skip)
        }
    }
    var m <- load_model(a.model, QuantMode.q8)
    let ok = moe_gpu_resident_active()
    if (!ok) {
        print("the whole-model driver declined the model at {regions} regions: {gpu_resident_decline_()}\n")
    } else {
        print("regions {regions}: the device homes {gpu_device_sessions()} stream(s), the N-row command steps {gpu_resident_assist_rows()} row(s) at once\n")
        for (n in counts) {
            sweep_row(m, a, n)
        }
        if (a.times) {
            times_row(m, a, counts[length(counts) - 1])
        }
        var passes <- gpu_cpu_passes_()
        for (p in passes) {
            print("  passed to the CPU: {p.reason} {p.words} x{p.count}\n")
        }
        delete passes
    }
    delete m
    return ok
}

[cold_path]
def private run(a : Args) : int {
    var counts <- id_list(a.streams, 2l)
    if (empty(counts)) {
        print("no stream count of 2 or more in --streams {a.streams}\n")
        return 2
    }
    var rc = 0
    with_dasllama_jobque_() {
        if (a.reload) {
            for (n in counts) {
                rc = sweep_load(a, [n], n) ? rc : 1
            }
        } else {
            var widest = 0l
            for (n in counts) {
                widest = max(widest, n)
            }
            rc = sweep_load(a, counts, widest) ? 0 : 1
        }
    }
    delete counts
    return rc
}

[export, cold_path]
def main : int {
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        print_help(get_command_info(type<Args>), "batch_streams_sweep")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help || empty(a.model)) {
        print_help(get_command_info(type<Args>), "batch_streams_sweep")
        return a.help ? 0 : 2
    }
    return run(a)
}
