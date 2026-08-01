options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// demo_load — a small load generator for dasllama-server, dogfooding the dashv client: worker
// threads loop varied chat completions against a RUNNING server. The default STAGED RAMP
// (1 -> 2 -> 4 -> 8 workers, a few requests each) fills the control page's stream swimlane and
// stair-steps the total tok/s; -n pins a constant worker count instead.
//
//   bin/daslang utils/dasllama-server/demo_load.das -- [--url http://127.0.0.1:8080] [-n 4] [-r 3]

require dashv/dashv_boost
require daslib/clargs
require daslib/jobque_boost

[CommandLineArgs]
struct DemoArgs {
    @clarg_short = "u"
    @clarg_doc = "Server base URL (default http://127.0.0.1:8080)"
    url : string

    @clarg_short = "n"
    @clarg_doc = "Constant worker count (default 0 = staged ramp 1 -> 2 -> 4 -> 8)"
    workers : int

    @clarg_short = "r"
    @clarg_doc = "Requests per worker per stage (default 3)"
    requests : int

    @clarg_doc = "Reply token budget per request (default 64)"
    max_tokens : int

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let PROMPTS = fixed_array(
    "Explain continuous batching in two sentences.",
    "Write a four-line poem about memory bandwidth.",
    "List three uses for a prefix cache in an LLM server.",
    "What is speculative decoding? Keep it short.",
    "Describe a transformer's KV cache to a beginner.",
    "Give two reasons quantized weights speed up inference.",
    "Summarize what a scheduler does in a token server.",
    "Why does prefill parallelize better than decode?")

def private run_worker(base : string; wid, nreq, max_new : int64) {
    for (i in range64(nreq)) {
        let prompt = PROMPTS[int((wid * 7l + i) % int64(length(PROMPTS)))]
        let body = "\{\"messages\":[\{\"role\":\"user\",\"content\":\"{prompt}\"}],\"max_tokens\":{max_new}}"
        var status = 0
        POST("{base}/v1/chat/completions", body) $(resp) {
            status = resp != null ? int(resp.status_code) : 0
        }
        let mark = status == int(http_status.OK) ? "" : "  <- FAILED"
        print("[w{wid}] {i + 1l}/{nreq} -> {status}{mark}\n")
    }
}

[export]
def main : int {
    var r <- parse_args(type<DemoArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<DemoArgs>), "demo_load")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<DemoArgs>), "demo_load")
        return 0
    }
    let base = empty(cfg.url) ? "http://127.0.0.1:8080" : cfg.url
    let nreq = int64(cfg.requests > 0 ? cfg.requests : 3)
    let max_new = int64(cfg.max_tokens > 0 ? cfg.max_tokens : 64)
    var up = false
    GET("{base}/v1/models") $(resp) {
        up = resp != null && resp.status_code == http_status.OK
    }
    if (!up) {
        print("error: no dasllama-server at {base} — start it first, then watch its control page while this runs\n")
        return 1
    }
    var stages : array<int>
    if (cfg.workers > 0) {
        stages |> push(cfg.workers)
    } else {
        stages <- [1, 2, 4, 8]
    }
    for (stage in stages) {
        print("=== stage: {stage} worker(s) x {nreq} request(s), max_tokens {max_new}\n")
        with_job_status(stage) $(finished) {
            for (w in range(stage)) {
                let wid = int64(w)
                new_thread() @() {
                    run_worker(base, wid, nreq, max_new)
                    finished |> notify_and_release
                }
            }
            finished |> join
        }
    }
    print("done — every stage drained\n")
    delete stages
    return 0
}
