// The CUDA resident files' forced-feed cell: the same fixed tokens through the CUDA chain and the all-CPU chain, the
// prefill logits and every step's within the bar, the one-step-off control past it, with the witnesses that the CUDA
// driver served every position, ran the windows the prompt takes and replayed the captured graph past the first step,
// a CPU run with the model off the device moving none of them (the control). One body for every carrier family the
// home serves (the `test_gpu_resident_cuda_*.das` files).
// Requiring it declares the CUDA resident files' CPU-prefill intent; its rig sets their prompt window beside each load
// (`cuda_feed_window`), and it carries the MMQ route pin and the FP4 rung's skip the FP4 cells take.

options gen2
options _dasllama_internal = true

module _cuda_feed shared public

require dasllama/dasllama_lint public
require dastest/testing_boost public
require daslib/jobque_boost
require dasllama/dasllama_math   // with_dasllama_jobque_
require dasllama/dasllama_gpu_tier   // gpu_cpu_passes_
require dasllama/dasllama_gpu_resident   // moe_gpu_resident_active, RD_UNSPLIT_POS
require dasllama/dasllama_cuda_decode   // cud_rdec_stats, set_cuda_mmq_pin / cuda_mmq_pin
require dasllama/dasllama_cuda_common   // cud_init, g_cud.sm, FP4_RUNG_SM: the FP4 rung's skip
require dasllama/dasllama_common   // allow_cpu_prefill
require dasllama/dasllama_env   // set_pf_window_rows_pin
require dasllama/dasllama_blocks   // g_moe_meet_*_experts: the stream witness's meet
require _model_tier
require _resident_feed
require daslib/defer

//! the CUDA resident files' intent: the control arm (the model off the device) prefills on the CPU
[init]
def private cuda_feed_init() {
    allow_cpu_prefill()
}

//! the prompt window at PF_WINDOW_ROWS rows where nothing pinned one, before a cell's load: the cells size their prompts
//! against it, and the plan's own pick would run the two-window prompt as one; the caller's `resident_levers` put it back
def cuda_feed_window() {
    if (pf_window_rows_pin() <= 0l) {
        set_pf_window_rows_pin(PF_WINDOW_ROWS)
    }
}

let LOGIT_BAR_REL = 0.06   //! the dense carriers' bar, of the step's max |logit|: every CUDA resident file reads it

//! the DASLLAMA_CUDA_MMQ value `v` by its name, for a cell's label
def cuda_mmq_name(v : int64) : string {
    return "CUDA_MMQ_OFF" if (v == CUDA_MMQ_OFF)
    return "CUDA_MMQ_WINNERS" if (v == CUDA_MMQ_WINNERS)
    return "CUDA_MMQ_ALL" if (v == CUDA_MMQ_ALL)
    return "CUDA_MMQ_UNPINNED"
}

//! true where the CUDA home is asked for and its card carries the FP4 tensor rung; else the cell has registered the skip
//! naming why and its caller returns
def fp4_rung_on(t : T?) : bool {
    return false if (!cuda_home_asked(t))
    let why = fp4_rung_why()
    if (!empty(why)) {
        t |> skip(why)
        return false
    }
    return true
}

//! why the card the CUDA home arms carries no FP4 tensor rung; ""
//! where it does
def fp4_rung_why() : string {
    return "no CUDA device the home arms (the cuda tier's log names why)" if (!cud_init())
    return ("the FP4 tensor rung needs compute capability {FP4_RUNG_SM}: " +
        "this card is {g_cud.sm}") if (g_cud.sm < FP4_RUNG_SM)
    return ""
}

//! `file` of family `arch` through the CUDA chain and the CPU chain over an `n`-token prompt and eight steps, at `bar` of the step's max |logit|;
//! `host_layers` past 0 asks at least that many routed layers' experts onto the host (`set_gpu_host_layers`): the split token command
//! and the cut window chain, on any card; the plan is held to exactly that count unless `host_more` admits the plan moving more
//! (a carrier the card cannot hold whole)
[cold_path]
def cuda_forced_feed(t : T?; file, arch : string; n, windows : int64; bar : float; host_layers : int64 = 0l; host_more : bool = false) {
    cuda_forced_feed(t, file, arch, n, windows, bar, host_layers, host_more) $ {}
}

//! the CUDA decoder's counters as `cud_rdec_stats` answers them
typedef CudRdecStats = tuple<tokens : int64; graph : bool; windows : int64; unsplit : int64; unsplit_form : bool>

//! ... with `witness`, the caller's own reads of the driver's counters, run after the fed steps while the model still holds the card
def cuda_forced_feed(t : T?; file, arch : string; n, windows : int64; bar : float; host_layers : int64; host_more : bool; witness : block<void>) {
    cuda_feed_rig(t, file, arch, host_layers, host_more) $(tr : Model; _path : string; before : CudRdecStats) {
        forced_feed_over(t, tr, "{file} cuda ", n, 8l, bar) $ {
            cuda_feed_served(t, before, n, windows, witness)
        }
    }
}

//! `cuda_forced_feed` on the FP4 tensor leaf, whose e2m1 activations the CPU chain does not take: the rows held by the f32-chain
//! witness at `ratio` of the CPU chain's own distance from the file's f32 chain (`forced_feed_f32_witness`) in place of a bar of the
//! step's max |logit| - one mantissa bit scatters single logits past any bar a one-step-off control still clears
[cold_path]
def cuda_forced_feed_f32_witness(t : T?; file, arch : string; n, windows : int64; ratio : float) {
    cuda_feed_rig(t, file, arch, 0l, false) $(tr : Model; path : string; before : CudRdecStats) {
        forced_feed_f32_witness(t, tr, path, "{file} cuda ", n, 8l, ratio) $ {
            cuda_feed_served(t, before, n, windows) $ {}
        }
    }
}

//! the driver's counters after a fed run: every call served on the device, every position counted, `windows` window chains, the
//! captured graph, the attention's unsplit form where the keys sit under its position; then `witness`, the caller's own reads
def private cuda_feed_served(t : T?; before : CudRdecStats; n, windows : int64; witness : block<void>) {
    held_on(t) $(var h) {
        cuda_feed_served(h, before, n, windows)
    }
    invoke(witness)
}

//! `cuda_feed_served`'s reads, held into `h`
def private cuda_feed_served(var h : Held; before : CudRdecStats; n, windows : int64) {
    var passes <- gpu_cpu_passes_()
    h |> held_equal(length(passes), 0, "the device served every call of the resident run: {passes}")
    delete passes
    let st = cud_rdec_stats()
    h |> held_fact(st.tokens >= n + 8l,
        "the CUDA driver served every position of the prompt and every fed token ({st.tokens} " +
        "positions)")
    h |> held_equal(st.windows - before.windows, windows,
        "the prompt ran as {windows} window chain(s) on the CUDA driver")
    h |> held_fact(st.graph, "the token step replays as a captured graph past the first step")
    let unsplit_want = st.unsplit_form && n + 8l <= RD_UNSPLIT_POS ? 8l : 0l
    h |> held_equal(st.unsplit - before.unsplit, unsplit_want,
        "the attention's unsplit form served {unsplit_want} of the fed steps (the keys " +
        "{n + 8l <= RD_UNSPLIT_POS ? "under" : "past"} the unsplit " +
        "position{st.unsplit_form ? "" : "; the device splits no keys"})")
}

//! `feed`, an `n`-token prompt and eight fed steps through the armed
//! CUDA driver, with the driver's counters `cuda_feed_served` reads
//! held into `h` (`windows` the window chains the prompt takes)
def with_cuda_feed_served(var h : Held; n, windows : int64; feed : block<void>) {
    let before = cud_rdec_stats()
    invoke(feed)
    cuda_feed_served(h, before, n, windows)
}

//! the counters' control around `run`, a CPU run with the model off
//! the device: it serves no position on the CUDA driver, runs no
//! window chain on it and holds no captured graph, held into `h`
def with_cuda_counters_still(var h : Held; run : block<void>) {
    let off0 = cud_rdec_stats()
    invoke(run)
    let off1 = cud_rdec_stats()
    h |> held_equal(off1.tokens, off0.tokens,
        "the counters' control: a CPU run with the model off the device serves no position on " +
        "the CUDA driver")
    h |> held_equal(off1.windows, off0.windows,
        "the counters' control: ... and runs no window chain on it")
    h |> held_fact(!off1.graph, "the counters' control: ... and holds no captured graph")
}

//! the knob's value a red names: the meet runs under DASLLAMA_GPU_MOE_SPLIT=1, the whole-layer rail under 0
def private split_knob : string => "DASLLAMA_GPU_MOE_SPLIT={gpu_want_moe_split() ? 1 : 0}"

//! the streamed prompt's counters before a feed (`stream_witness`
//! reads what moved): the chain's windows, the experts the meet took
//! on the device and handed the host
struct StreamAt {
    windows : int64
    gpu_experts : int64
    cpu_experts : int64
}

def stream_at() : StreamAt => StreamAt(windows = cud_stream_stats().windows,
    gpu_experts = g_moe_meet_gpu_experts, cpu_experts = g_moe_meet_cpu_experts)

//! the stream's counters since `at`, logged
def private log_stream(file : string; at : StreamAt) {
    let st = cud_stream_stats()
    to_log(LOG_INFO, "{file}: the chain ran {st.windows - at.windows} streamed windows, " +
        "{st.bundles} bundles of {st.experts} experts since the device came up; the meet took " +
        "{g_moe_meet_gpu_experts - at.gpu_experts} experts on the device and handed the host " +
        "{g_moe_meet_cpu_experts - at.cpu_experts} ({split_knob()})\n")
}

//! the control's reads since `at` with the prompt stream off, held
//! into `h`: no window streamed, no expert met on either side
def stream_off_witness(var h : Held; file : string; at : StreamAt) {
    log_stream(file, at)
    h |> held_equal(cud_stream_stats().windows - at.windows, 0l,
        "control: with the prompt stream off the chain ran no window - the host summed " +
        "every routed layer on the host")
    h |> held_equal(g_moe_meet_gpu_experts - at.gpu_experts, 0l,
        "control: with the prompt stream off the meet took no expert on the device " +
        "({split_knob()})")
    h |> held_equal(g_moe_meet_cpu_experts - at.cpu_experts, 0l,
        "control: ... and handed the host none")
}

//! `stream_off_witness` on the cell
def stream_off_witness(t : T?; file : string; at : StreamAt) {
    held_on(t) $(var h) {
        stream_off_witness(h, file, at)
    }
}

//! the streamed prompt's witnesses since `at`, held into `h`: the
//! chain ran the window(s) it was asked (`cud_stream_stats`), the
//! meet split the experts between the device and the host
//! (`host_first` the meet's host share the cell pinned)
def stream_witness(var h : Held; file : string; at : StreamAt; host_first : int64) {
    log_stream(file, at)
    let windows = cud_stream_stats().windows - at.windows
    h |> held_fact(windows > 0l,
        "the {gpu_want_moe_split() ? "per-expert chain" : "whole-layer rail"} ran " +
        "{windows} streamed windows over the prompt's host layers ({split_knob()})")
    return if (!gpu_want_moe_split())   // DASLLAMA_GPU_MOE_SPLIT=0: the whole-layer rail takes every region, no meet
    let met_gpu = g_moe_meet_gpu_experts - at.gpu_experts
    let met_cpu = g_moe_meet_cpu_experts - at.cpu_experts
    h |> held_fact(met_gpu > 0l, "the meet took {met_gpu} experts on the device ({split_knob()})")
    h |> held_fact(met_cpu > 0l,
        "the meet handed the host {met_cpu} experts " +
        "(set_moe_host_first({host_first}), {split_knob()})")
}

//! `stream_witness` on the cell
def stream_witness(t : T?; file : string; at : StreamAt; host_first : int64) {
    held_on(t) $(var h) {
        stream_witness(h, file, at, host_first)
    }
}

//! the fed cells' rig: `file` loaded on the driver with `host_layers` asked onto the host, admitted and checked, `feed` run under
//! the jobque with the counters as the load left them, then the counters' control - a CPU run with the model off the device
def private cuda_feed_rig(t : T?; file, arch : string; host_layers : int64; host_more : bool; feed : block<(tr : Model; path : string; before : CudRdecStats) : void>) {
    let path = cuda_model_path(t, file)
    if (empty(path)) return
    let levers = resident_levers()
    defer() {
        restore_resident_levers(levers)
    }
    cuda_feed_window()
    if (!assert_pf_window(t)) return
    var tr <- load_resident_host(path, 1l, 0l, host_layers)
    t |> equal(tr.arch, arch, "the file is a {arch}-family model")
    return if (!resident_admitted(t, "the whole-model driver armed on the model (the load log names any decline)", ""))
    if (host_layers > 0l && host_more) {
        t |> success(gpu_resident_host_layers() >= host_layers, "at least the first {host_layers} routed layers' experts serve on the host as asked (set_gpu_host_layers): {gpu_resident_host_layers()} of {tr.config.n_layers} do")
    } elif (host_layers > 0l) {
        t |> equal(gpu_resident_host_layers(), host_layers, "exactly the first {host_layers} routed layers' experts serve on the host as asked (set_gpu_host_layers; the card holds the rest): {gpu_resident_host_layers()} of {tr.config.n_layers} do")
    }
    let before = cud_rdec_stats()
    with_dasllama_jobque_() {
        invoke(feed, tr, path, before)
        var prompt <- resident_prompt(40l)
        var toks <- resident_feed(8l)
        held_on(t) $(var h) {
            with_cuda_counters_still(h) {
                var inscope cpu_rows <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
            }
        }
        delete prompt
        delete toks
    }
    delete tr
}
