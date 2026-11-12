// The block drafter under the scheduler: N streams, each a chat turn of its own off --prompts (cycled), walked plain and
// then speculating on the Metal home. Reports each run's wall rate over every stream, the drafts accepted, the widest joint
// round, and whether every stream's ids match its plain walk.
//
//   bin/daslang -jit modules/dasLLAMA/harness/dflash_sched_probe.das -- -m <trunk.gguf> -r <drafter.gguf> --prompts <file>
//       [--streams 1,2,4] [--steps 128] [--depth 3]

options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true
require dasllama/dasllama
require dasllama/dasllama_common   // set_metal_mode, allow_cpu_prefill, set_mtp_depth, the overrides
require dasllama/dasllama_dflash   // attach_dflash_drafter
require dasllama/dasllama_chat   // create_chat_renderer_, render_turn_
require dasllama/dasllama_scheduler
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - the Metal decode driver, inside the das_metal half
require ?das_metal dasllama/dasllama_metal_dflash   // nolint:STYLE030,LINT019 - the drafter's device pass registers at init
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - metal_kernel_coverage, inside the das_metal half
require ../performance/profile_common.das   // nolint:STYLE029,LINT019 - tune_gate, inside the das_metal half
require ../tests/_scheduler_rig.das   // nolint:STYLE029,LINT019 - make_req, drain, toks_of
require ../tests/_compares.das   // nolint:STYLE029,LINT019 - first_mismatch: where a drafted stream parts from its plain walk
require _prompt_files   // read_lines
require daslib/clargs
require math
require daslib/strings_boost
require daslib/strings_convert

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "Trunk GGUF path"
    model : string
    @clarg_short = "r"
    @clarg_doc = "DFlash / DFlash2 drafter GGUF path"
    drafter : string
    @clarg_doc = "a file of user questions, one a line: stream i walks question i mod the count as a chat turn (thinking off)"
    prompts : string
    @clarg_doc = "the stream counts to run, a comma list (default 1,2,4)"
    streams : string = "1,2,4"
    @clarg_doc = "generated tokens a stream (default 128)"
    steps : int = 128
    @clarg_doc = "drafts a round (default: the engine's depth)"
    depth : int = 0
    @clarg_doc = "print the Metal kernel census of each drafting run, dispatches a decode tick"
    census : bool
    @clarg_short = "h"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct private Run {
    toks : array<array<int64>>
    ms : double
    drafted : int64
    accepted : int64
    joined_max : int64
    mtp : bool
    ticks : int64          //! decode ticks
    gen_ms : double        //! the wall from the first decode tick to idle
    pass_gpu_ms : double   //! the drafter's passes on the GPU
    passes : int64
    split_ms : double[5]   //! a joint round's draft, verify, rollback, context commit and whole, per round
    rounds : int64
}

def private chat_ids(m : Model; question : string) : array<int64> {
    var inscope c <- create_chat_renderer_(m, "")
    set_thinking_(c, false)
    add_user_(c, question)
    return <- render_turn_(m, c)
}

[cold_path]
def private sched_run(m : Model; prompts : array<array<int64>>; budget : int64; spec : bool; census : bool = false) : Run {
    var run : Run
    var ev : array<SchedEvent>
    var sch <- create_scheduler(m, long_length(prompts), long_length(prompts), 0l, 64l, 0l, KVDtype.f16, spec)   //! every request queues: a queue under the stream count drops the rest
    run.mtp = sch.mtp
    for (i in range64(long_length(prompts))) {
        submit(m, sch, make_req(i + 1l, prompts[i], budget))
    }
    let t0 = ref_time_ticks()
    var t_dec = t0
    var steps = 0
    var gpu0, passes0 : int64
    var sp0 : DflashTickSplit
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let p0 = metal_dflash_stats()
        gpu0 = p0.gpu_us
        passes0 = p0.passes
        sp0 = metal_dflash_tick_split()
    }
    var census_armed = false
    while (scheduler_step(m, sch, ev)) {
        if (sch.decode_steps == 0l) {
            t_dec = ref_time_ticks()
        } elif (census && !census_armed) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                metal_kernel_coverage_reset()   //! from the first decode tick on: the prefill's kernels stay out
            }
            census_armed = true
        }
        steps++
        break if (steps > 65536)
    }
    run.ms = double(get_time_usec(t0)) / 1000.0lf
    run.gen_ms = double(get_time_usec(t_dec)) / 1000.0lf
    run.ticks = sch.decode_steps
    if (census) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            var inscope rows <- metal_kernel_coverage_served()
            var total = 0l
            for (row in rows) {
                total += row.n
                print("  census {row.name}: {double(row.n) / double(max(run.ticks - 1l, 1l)):.1f} a tick\n")
            }
            print("  census total: {double(total) / double(max(run.ticks - 1l, 1l)):.0f} dispatches a tick\n")
        }
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let p1 = metal_dflash_stats()
        run.pass_gpu_ms = double(p1.gpu_us - gpu0) / 1000.0lf
        run.passes = p1.passes - passes0
        let sp1 = metal_dflash_tick_split()
        run.rounds = sp1.rounds - sp0.rounds
        let per = 1.0lf / (1000.0lf * double(max(run.rounds, 1l)))
        run.split_ms[0] = double(sp1.draft_us - sp0.draft_us) * per
        run.split_ms[1] = double(sp1.verify_us - sp0.verify_us) * per
        run.split_ms[2] = double(sp1.rollback_us - sp0.rollback_us) * per
        run.split_ms[3] = double(sp1.commit_us - sp0.commit_us) * per
        run.split_ms[4] = double(sp1.round_us - sp0.round_us) * per
    }
    run.toks |> reserve(long_length(prompts))
    for (i in range64(long_length(prompts))) {
        run.toks |> emplace(toks_of(ev, i + 1l))
    }
    run.drafted = sch.mtp_drafted
    run.accepted = sch.mtp_accepted
    run.joined_max = sch.mtp_joined_max
    delete sch
    delete ev
    return <- run
}

def private total_toks(r : Run) : int64 {
    var n = 0l
    for (t in r.toks) {
        n += long_length(t)
    }
    return n
}

def private run_streams(a : Args; m : Model; questions : array<string>; n : int64) {
    var prompts : array<array<int64>>
    prompts |> reserve(n)
    for (i in range64(n)) {
        prompts |> emplace(chat_ids(m, questions[int(i % long_length(questions))]))
    }
    var plain <- sched_run(m, prompts, int64(a.steps), false)
    var spec <- sched_run(m, prompts, int64(a.steps), true, a.census)
    var same = 0l
    for (i in range64(n)) {
        var eq = long_length(plain.toks[i]) == long_length(spec.toks[i])
        for (j in range64(min(long_length(plain.toks[i]), long_length(spec.toks[i])))) {
            eq = eq && plain.toks[i][j] == spec.toks[i][j]
        }
        same += eq ? 1l : 0l
        if (!eq) {
            let at = first_mismatch(plain.toks[i], spec.toks[i])
            print("  stream {i} parts from plain at id {at} of {long_length(plain.toks[i])} plain, {long_length(spec.toks[i])} drafted\n")
        }
    }
    let pt = total_toks(plain)
    let st = total_toks(spec)
    print("  decode rate: plain {double(pt) * 1000.0lf / plain.gen_ms:.1f} tok/s | drafting {double(st) * 1000.0lf / spec.gen_ms:.1f} tok/s, from the first decode tick\n")
    print("  decode ticks: plain {plain.ticks} at {plain.gen_ms / double(max(plain.ticks, 1l)):.2f} ms | drafting {spec.ticks} at {spec.gen_ms / double(max(spec.ticks, 1l)):.2f} ms, {spec.passes} passes at {spec.pass_gpu_ms / double(max(spec.passes, 1l)):.2f} ms GPU each ({spec.pass_gpu_ms / double(max(spec.ticks, 1l)):.2f} ms a tick)\n")
    if (spec.rounds > 0l) {
        print("  a joint round {spec.split_ms[4]:.2f} ms: drafts {spec.split_ms[0]:.2f}, verify {spec.split_ms[1]:.2f}, rollbacks {spec.split_ms[2]:.2f}, context commits {spec.split_ms[3]:.2f}, the rest {spec.split_ms[4] - spec.split_ms[0] - spec.split_ms[1] - spec.split_ms[2] - spec.split_ms[3]:.2f}; outside the round {spec.gen_ms / double(max(spec.ticks, 1l)) - spec.split_ms[4]:.2f}\n")
    }
    print("streams {n}: plain {pt} ids {plain.ms:.1f} ms {double(pt) * 1000.0lf / plain.ms:.1f} tok/s | drafting {st} ids {spec.ms:.1f} ms {double(st) * 1000.0lf / spec.ms:.1f} tok/s (scheduler speculating: {spec.mtp}), accepted {spec.accepted} of {spec.drafted}, widest joint round {spec.joined_max} | streams equal to plain: {same} of {n}\n")
    delete prompts
    delete plain
    delete spec
}

def private run(a : Args) : int {
    allow_cpu_prefill(true)
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_mode(MetalMode.required)
        apply_box_profile_runtime()
        return 1 if (!tune_gate())
        select_decode_override("metal")
    } else {
        print("das_metal is not built into this binary\n")
        return 1
    }
    var rc = 0
    with_dasllama_jobque() {
        var m <- load_model(a.model, QuantMode.q8)
        if (a.depth > 0) {
            set_mtp_depth(int64(a.depth))
        }
        let why = attach_dflash_drafter(m, a.drafter)
        if (!empty(why)) {
            print("drafter refused: {why}\n")
            rc = 1
        } else {
            var questions <- read_lines(a.prompts)
            for (tok in split(a.streams, ",")) {
                let n = try_to_int(tok)
                if (n is ok && (n as ok) > 0 && !empty(questions)) {
                    run_streams(a, m, questions, int64(n as ok))
                }
            }
            delete questions
        }
        delete m
    }
    return rc
}

[export, cold_path]
def main : int {
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        print_help(get_command_info(type<Args>), "dflash_sched_probe")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help || empty(a.model) || empty(a.drafter) || empty(a.prompts)) {
        print_help(get_command_info(type<Args>), "dflash_sched_probe")
        return a.help ? 0 : 2
    }
    return run(a)
}
