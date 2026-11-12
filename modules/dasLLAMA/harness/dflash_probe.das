options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dasllama/dasllama
require dasllama/dasllama_common   // set_metal_mode, allow_cpu_prefill, generate, generate_mtp_greedy, the overrides
require dasllama/dasllama_dflash   // attach_dflash_drafter, set_dflash_draft_temp
require dasllama/dasllama_math   // parallel_argmax
require dasllama/dasllama_sampling   // generate_, generate_mtp_sampled, set_seed_, set_mtp_draft_temp
require dasllama/dasllama_chat   // create_chat_renderer_, set_tools_, render_turn_, turn_stop_ids_: the tool-call arm's prompts
require ?das_metal dasllama/dasllama_metal_kernels   // nolint:STYLE030,LINT019 - the Metal decode driver, inside the das_metal half
require ?das_metal dasllama/dasllama_metal_dflash   // nolint:STYLE030,LINT019 - the drafter's device pass registers at init
require ?das_metal dasllama/dasllama_metal_common   // nolint:STYLE030,LINT019 - set_metal_decode_speculate, inside the das_metal half
require ../performance/profile_common.das   // nolint:STYLE029,LINT019 - tune_gate, inside the das_metal half
require _prompt_files   // read_ids, read_lines
require daslib/clargs
require daslib/fio
require math
require strings
require daslib/strings_boost

// The speculative round - the DFlash2 block drafter with -r, the trunk's own NextN head without - against the plain walk of
// the same sampler: the same prompt, the same budget, the same seed. Three accept modes
// (ARCHITECTURE_GPU_MTP_ACCEPT.md#dflash-sampled-drafts, #nextn-draft-temp): greedy (ids equal by construction), sample
// (the exact draw-and-compare walk; ids equal where the verify's rows and the plain steps' give the draws one distribution)
// and residual (rejection sampling, the drafter at --draft-temp; distributed as the plain stream, not token-equal to it).
// Each prints tokens a round, drafts accepted of drafted, both walks' decode tok/s and both texts. --prompts runs one
// short chat turn a line (the tools of --tools declared, thinking off, each walk ending on the turn's stop) and pools them;
// --context-ids / --context-text put a long context ahead of every prompt.
//
//   bin/daslang -jit modules/dasLLAMA/harness/dflash_probe.das -- -m <trunk.gguf> [-r <drafter.gguf>] [--steps 48]
//       [--mode greedy|sample|residual] [--temp 0.7] [--top-p 1] [--top-k 0] [--draft-temp <temp>] [--seed 4242]
//       [--prompt-ids <file> | --prompts <file> [--tools <file>]] [--context-text <file> [--context-tokens 3000]]

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "Trunk GGUF path"
    model : string
    @clarg_short = "r"   //! not -d: after the `--` the JIT runner reads -d as its own --jit-dump and prints every module's IR
    @clarg_doc = "DFlash2 drafter GGUF path (absent: the trunk's NextN head drafts)"
    drafter : string
    @clarg_doc = "generated tokens a walk (default 48)"
    steps : int = 48
    @clarg_doc = "decode and verify on the Metal driver (the prompt still prefills on the CPU route, whose layer hook feeds the drafter's context)"
    metal : bool
    @clarg_doc = "print every round's drafts and accepted count"
    trace : bool
    @clarg_doc = "run the drafter's pass on the CPU reference arm even where the device serves it"
    cpu_draft : bool
    @clarg_doc = "drafts a round (1 .. 7; default: the engine's depth)"
    depth : int = 0
    @clarg_doc = "a file of comma-separated token ids as the prompt instead of the built-in text (a bench's prompt file, its chat template inside)"
    prompt_ids : string
    @clarg_doc = "dump the engine's profile sections over the speculative walk (needs JOBQUE_PROFILING=1 and -jit-no-cache)"
    prof : bool
    @clarg_doc = "the Metal single-row driver's greedy pre-encode chain: -1 adaptive (the engine's default), 0 off, 1 forced"
    chain : int = -1
    @clarg_doc = "print the Metal kernel census of the traced walk's third round (every kernel the round dispatched, with its count)"
    census : bool
    @clarg_doc = "the accept mode: greedy (argmax match), sample (draw each verify row, accept while it equals the draft), residual (rejection sampling with the residual)"
    mode : string = "greedy"
    @clarg_doc = "the sampler's temperature under sample / residual (default 0.7)"
    temp : float = 0.7
    @clarg_doc = "the sampler's nucleus under sample / residual (default 1: off)"
    top_p : float = 1.0
    @clarg_doc = "the sampler's top-k under sample / residual (default 0: off)"
    top_k : int = 0
    @clarg_doc = "the drafter's own temperature under residual (default: --temp; 0 = greedy drafts, a point-mass q)"
    draft_temp : float = -1.0
    @clarg_doc = "the seed both walks' rngs start from (default 4242)"
    seed : int = 4242
    @clarg_doc = "a file of user questions, one a line: each a chat turn of its own (thinking off), both walks pooled over them"
    prompts : string
    @clarg_doc = "a file of tool definitions (OpenAI tools[] entries), one JSON object a line, declared on every --prompts turn"
    tools : string
    @clarg_doc = "a file of comma-separated token ids put ahead of every prompt"
    context_ids : string
    @clarg_doc = "a text file whose first --context-tokens tokens go ahead of every prompt"
    context_text : string
    @clarg_doc = "the tokens --context-text contributes (default 3000)"
    context_tokens : int = 3000
    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let PROMPT = "Write a complete, well-commented Python module that implements a token-bucket rate limiter with per-key buckets, a sliding-window variant, a thread-safe wrapper, and a pytest test suite. Write at least 600 lines. Do not stop early."

//! the greedy speculative walk round by round: each round's drafts and the count accepted, printed - the `steps` ids and
//! the counts as `generate_mtp_sampled` returns them
var g_census_round = -1

def private walk_traced(m : Model; var s : Session; prompt : array<int64>; steps : int64) : MtpSampledWalk {
    let np = long_length(prompt)
    var w : MtpSampledWalk
    w.ids |> reserve(steps)
    set_mtp_spec(true)
    forward_prefill(m, s, prompt, np, 0l)
    s.n_past = np
    let t_gen = ref_time_ticks()
    var tok = argmax_row(s.logits)
    var round = 0
    while (long_length(w.ids) < steps) {
        w.ids |> push(tok)
        break if (long_length(w.ids) >= steps)
        let tr = ref_time_ticks()
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (round == g_census_round) {
                metal_kernel_coverage_reset()
            }
        }
        let d0 = s.mtp_drafted
        let a = mtp_spec_round(m, s, tok)
        let drafted = s.mtp_drafted - d0
        print("round {round} at {s.n_past - a - 1l}: anchor {tok} drafted {drafted} accepted {a} in {get_time_usec(tr) / 1000} ms\n")
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (round == g_census_round) {
                var inscope rows <- metal_kernel_coverage_served()
                for (row in rows) {
                    print("  census {row.name}: {row.n}\n")
                }
            }
        }
        w.drafted += drafted
        w.accepted += a
        w.rounds++
        round++
        for (j in range64(a)) {
            break if (long_length(w.ids) >= steps)
            w.ids |> push(s.mtp_vbatch[j + 1l])
        }
        break if (long_length(w.ids) >= steps)
        tok = argmax_row(s.logits)
    }
    w.gen_us = int64(get_time_usec(t_gen))
    return <- w
}

//! the sampler a mode runs: a bare argmax under greedy, else the asked temperature, nucleus and top-k
def private mode_params(a : Args) : SamplingParams {
    return SamplingParams() if (a.mode == "greedy")
    return SamplingParams(temp = a.temp, top_p = a.top_p, top_k = int64(a.top_k))
}

//! the process-wide acceptance and the drafters' temperature a mode asks; false = no such mode
def private arm_mode(a : Args) : bool {
    if (a.mode == "greedy" || a.mode == "sample") {
        set_mtp_accept(MtpAccept.exact)
        set_dflash_draft_temp(0.0)
        set_mtp_draft_temp(0.0)
        return true
    }
    if (a.mode == "residual") {
        set_mtp_accept(MtpAccept.rejection)
        let temp = a.draft_temp >= 0.0 ? a.draft_temp : a.temp
        set_dflash_draft_temp(temp)
        set_mtp_draft_temp(temp)
        return true
    }
    return false
}

//! the decode rate of `n` ids over `us` microseconds
def private tok_per_s(n, us : int64) : double => us > 0l ? double(n) * 1.0e6lf / double(us) : 0.0lf

def private argmax_row(v : array<float>) : int64 => parallel_argmax(v, long_length(v))

//! the context every prompt carries ahead of it: --context-ids, else the first --context-tokens tokens of --context-text
def private context_of(a : Args; m : Model) : array<int64> {
    return <- read_ids(a.context_ids) if (!empty(a.context_ids))
    let text = empty(a.context_text) ? "" : fread(a.context_text)
    var ids <- empty(text) ? array<int64>() : encode(m, text, false)
    if (long_length(ids) > int64(a.context_tokens)) {
        ids |> resize(a.context_tokens)
    }
    return <- ids
}

//! the plain walk of one prompt and the speculative walk of the same sampler from the same seed
struct PairWalk {
    plain : array<int64>
    plain_us : int64   //! the plain walk's decode after its prefill
    spec : MtpSampledWalk
}

//! both walks over `prompt`, each ending on the first id among `stops` or at --steps ids
def private walk_pair(a : Args; m : Model; prompt : array<int64>; var stops : array<int64>) : PairWalk {
    let steps = int64(a.steps)
    let params = mode_params(a)
    var p : PairWalk
    var s1 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    set_seed_(s1, a.seed)
    p.plain |> reserve(steps)
    generate_(m, s1, prompt, params, steps) $(id : int64; _piece : string) : bool {
        p.plain |> push(id)
        return long_length(p.plain) < steps && find_index(stops, id) < 0
    }
    p.plain_us = s1.stat_gen_us
    var s2 = make_run_state(m.config, KVDtype.f16, KVDtype.f16)
    set_seed_(s2, a.seed)
    if (a.prof) {
        forward_profile_reset()
    }
    if (a.trace && a.mode == "greedy" && empty(stops)) {
        p.spec <- walk_traced(m, s2, prompt, steps)
    } else {
        p.spec <- generate_mtp_sampled(m, s2, prompt, params, steps, stops)
    }
    if (a.prof) {
        print("--- forward_profile: the speculative walk ---\n")
        forward_profile_report()
    }
    delete s1
    delete s2
    return <- p
}

//! the leading ids both walks share
def private ids_equal(p : PairWalk) : int64 {
    var same = 0l
    while (same < long_length(p.plain) && same < long_length(p.spec.ids) && p.plain[same] == p.spec.ids[same]) {
        same++
    }
    return same
}

def private tokens_a_round(accepted, rounds : int64) : double => rounds > 0l ? double(accepted + rounds) / double(rounds) : 0.0lf

def private mode_line(a : Args) {
    let params = mode_params(a)
    let draft_temp = empty(a.drafter) ? get_mtp_draft_temp() : get_dflash_draft_temp()
    let depth = a.depth > 0 ? "{a.depth}" : (empty(a.drafter) ? "{get_mtp_depth()}" : "the block")
    print("mode {a.mode}: temp {params.temp} top-p {params.top_p} top-k {params.top_k}, draft temp {draft_temp}, seed {a.seed}, depth {depth}\n")
}

//! one prompt's two walks reported; 2 where a greedy walk's ids part from the plain walk's
def private walk_one(a : Args; m : Model; prompt : array<int64>) : int {
    var none : array<int64>
    var inscope p <- walk_pair(a, m, prompt, none)
    let w & = p.spec
    let same = ids_equal(p)
    mode_line(a)
    print("prompt {length(prompt)} ids\n")
    print("tokens a round {fmt(":.3f", tokens_a_round(w.accepted, w.rounds))} over {w.rounds} rounds; drafts accepted {w.accepted} of {w.drafted}\n")
    print("speculative {length(w.ids)} ids at {fmt(":.2f", tok_per_s(long_length(w.ids), w.gen_us))} tok/s; plain {length(p.plain)} ids at {fmt(":.2f", tok_per_s(long_length(p.plain), p.plain_us))} tok/s (decode alone, both)\n")
    print("ids equal for {same} of {min(length(p.plain), length(w.ids))}\n")
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (a.metal) {
            let sel = metal_dflash_selected()
            print("device passes: {sel.selected} selected, {sel.held} held ({metal_verify_ahead_rounds()} sampled), {metal_dflash_accepted()} accepted on the device\n")
        }
    }
    print("plain: {decode(m, p.plain)}\n")
    print("spec:  {decode(m, w.ids)}\n")
    var rc = 0
    if (same < min(long_length(p.plain), long_length(w.ids))) {
        print("{a.mode == "residual" ? "diverged (rejection sampling's draws are not the plain stream's)" : "MISMATCH"} at {same}: plain {p.plain[same]} vs speculative {w.ids[same]}\n")
        if (a.mode == "greedy") {
            rc = 2
        }
    }
    return rc
}

//! a rendered chat turn and the ids that end its reply
struct PromptTurn {
    ids : array<int64>
    stops : array<int64>
}

//! one --prompts line rendered as a chat turn with the --tools declared and thinking off
def private chat_turn(m : Model; question : string; tools : array<string>) : PromptTurn {
    var inscope c <- create_chat_renderer_(m, "")
    var declared := tools
    set_tools_(c, declared)
    set_thinking_(c, false)
    add_user_(c, question)
    return <- PromptTurn(ids <- render_turn_(m, c), stops <- effective_stop_ids_(c))
}

def private one_line(text : string) : string => replace(text, "\n", "\\n")

//! every --prompts line's two walks, `ctx` ahead of each, pooled: tokens a round, drafts accepted, the mean decode a request
//! (the first draw to the last id) and the pooled rate of both walks; 2 where a greedy walk's ids part from the plain walk's
def private walk_pool(a : Args; m : Model; ctx : array<int64>) : int {
    var inscope questions <- read_lines(a.prompts)
    var inscope tools <- empty(a.tools) ? array<string>() : read_lines(a.tools)
    if (empty(questions)) {
        print("no prompts in '{a.prompts}'\n")
        return 1
    }
    mode_line(a)
    print("{length(questions)} prompts, {length(tools)} tools, {length(ctx)} context ids ahead, at most {a.steps} ids a reply\n")
    var plain_ids = 0l
    var plain_us = 0l
    var spec_ids = 0l
    var spec_us = 0l
    var rounds = 0l
    var drafted = 0l
    var accepted = 0l
    var parted = 0
    for (q, i in questions, count()) {
        var turn <- chat_turn(m, q, tools)
        var prompt := ctx
        prompt |> push_from(turn.ids)
        var inscope p <- walk_pair(a, m, prompt, turn.stops)
        let same = ids_equal(p)
        parted += same < min(long_length(p.plain), long_length(p.spec.ids)) ? 1 : 0
        print("prompt {i}: {length(prompt)} ids; plain {length(p.plain)} ids in {fmt(":.1f", double(p.plain_us) / 1000.0lf)} ms, spec {length(p.spec.ids)} ids in {fmt(":.1f", double(p.spec.gen_us) / 1000.0lf)} ms, {p.spec.rounds} rounds, accepted {p.spec.accepted} of {p.spec.drafted}, ids equal {same}\n")
        print("  spec: {one_line(decode(m, p.spec.ids))}\n")
        plain_ids += long_length(p.plain)
        plain_us += p.plain_us
        spec_ids += long_length(p.spec.ids)
        spec_us += p.spec.gen_us
        rounds += p.spec.rounds
        drafted += p.spec.drafted
        accepted += p.spec.accepted
        delete turn
        delete prompt
    }
    let n = double(length(questions))
    let pct = drafted > 0l ? double(accepted) * 100.0lf / double(drafted) : 0.0lf
    print("POOLED tokens a round {fmt(":.3f", tokens_a_round(accepted, rounds))} over {rounds} rounds; drafts accepted {accepted} of {drafted} ({fmt(":.1f", pct)}%)\n")
    print("POOLED speculative {spec_ids} ids, {fmt(":.1f", double(spec_us) / 1000.0lf / n)} ms a request, {fmt(":.2f", tok_per_s(spec_ids, spec_us))} tok/s; plain {plain_ids} ids, {fmt(":.1f", double(plain_us) / 1000.0lf / n)} ms a request, {fmt(":.2f", tok_per_s(plain_ids, plain_us))} tok/s\n")
    print("POOLED walks whose ids part: {parted} of {length(questions)}\n")
    return parted > 0 && a.mode == "greedy" ? 2 : 0
}

//! the arm the flags ask: the pooled --prompts walk, else one prompt (--prompt-ids, else the built-in text), `ctx` ahead
def private walk_both(a : Args; m : Model) : int {
    var inscope ctx <- context_of(a, m)
    return walk_pool(a, m, ctx) if (!empty(a.prompts))
    var prompt := ctx
    var body <- empty(a.prompt_ids) ? encode(m, PROMPT, true) : read_ids(a.prompt_ids)
    prompt |> push_from(body)
    let rc = walk_one(a, m, prompt)
    delete body
    delete prompt
    return rc
}

def private run(a : Args) : int {
    if (!arm_mode(a)) {
        print("unknown --mode '{a.mode}': greedy, sample or residual\n")
        return 2
    }
    allow_cpu_prefill(true)
    if (a.metal) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            set_metal_mode(MetalMode.required)
            apply_box_profile_runtime()
            return 1 if (!tune_gate())
            select_decode_override("metal")
            if (a.chain >= 0) {
                set_metal_decode_speculate(a.chain)
            }
            if (a.census) {
                g_census_round = 2
            }
        } else {
            print("das_metal is not built into this binary\n")
            return 1
        }
    } else {
        set_metal_mode(MetalMode.off)
    }
    var rc = 0
    with_dasllama_jobque() {
        var m <- load_model(a.model, QuantMode.q8)
        set_dflash_device(!a.cpu_draft)
        if (a.depth > 0) {
            set_mtp_depth(int64(a.depth))
        }
        if (empty(a.drafter)) {
            if (m.config.n_layer_nextn <= 0) {
                print("{base_name(a.model)} carries no NextN head and no -r drafter is named\n")
                rc = 1
            } else {
                print("the NextN head drafts\n")
                rc = walk_both(a, m)
            }
        } else {
            let why = attach_dflash_drafter(m, a.drafter)
            set_dflash_trace(a.trace)
            if (!empty(why)) {
                print("drafter refused: {why}\n")
                rc = 1
            } else {
                print("drafter {base_name(a.drafter)} attached, pass on the {dflash_on_device() ? "device" : "CPU"}\n")
                rc = walk_both(a, m)
            }
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
        print_help(get_command_info(type<Args>), "dflash_probe")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help || empty(a.model)) {
        print_help(get_command_info(type<Args>), "dflash_probe")
        return a.help ? 0 : 2
    }
    return run(a)
}
