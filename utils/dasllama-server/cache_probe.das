options gen2
options indenting = 4
options persistent_heap   // the server thread loads and frees a multi-GB model once
options stack = 524288    // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity - a serving probe, not the bit-exact suite
options _dasllama_internal = true

// A quick read of the prefix cache on any model: sixteen short chat requests in a fixed cycle against an
// OpenAI-style server, one line per request with its cached prompt tokens and wall time, then the shares.
//   bin/daslang -jit utils/dasllama-server/cache_probe.das -- --url http://127.0.0.1:8911
//   bin/daslang -jit utils/dasllama-server/cache_probe.das -- -m <model.gguf>      (boots a CPU server itself)
require cache_probe_cycle   // the cycle and its figures (sibling)
require _server_rig         // serve_clients: the booted server's ready poll and shutdown (sibling)
require openai_server       // run_server / set_model / allow_cpu_prefill (sibling)
require dasllama/dasllama   // load_model / QuantMode
require daslib/clargs
require daslib/jobque_boost
require daslib/json_boost
require daslib/fio          // base_name
require dashv/dashv_boost   // GET / POST
require openai/openai_models   // list_models: the served model id
require openai/openai_common   // openai_client

[CommandLineArgs]
struct Args {
    @clarg_short = "u"
    @clarg_doc = "A running dasllama-server, like http://127.0.0.1:8911; without it the probe boots one from --model"
    url : string
    @clarg_short = "m"
    @clarg_doc = "GGUF to serve on a server the probe boots itself: CPU, q8 weights, the server's default knobs"
    model : string
    @clarg_short = "p"
    @clarg_doc = "Port of the booted server (default 18199)"
    port : int = 18199
    @clarg_short = "n"
    @clarg_doc = "Completion budget per request (default 16)"
    max_tokens : int = 16
    @clarg_doc = "Send preserve_thinking: a replayed assistant turn renders with its think block, as the generation prompt did - what a client that keeps reasoning_content sends"
    preserve_thinking : bool
}

//! the first model id the server lists; "" when the route did not answer
def private served_model(base_url : string) : string {
    var listed <- list_models(openai_client("{base_url}/v1"))
    let id = listed.ok && !empty(listed.models) ? listed.models[0].id : ""
    delete listed
    return id
}

let private DEFAULT_PAGE_ROWS = 64   // run_server's page_rows default

//! the cache page the server reports on /config, `DEFAULT_PAGE_ROWS` when the route or the key is absent (a server the
//! probe boots itself registers no config surface)
def private served_page_rows(base_url : string) : int {
    var rows = DEFAULT_PAGE_ROWS
    GET("{base_url}/config") $(resp) {
        if (resp != null && resp.status_code == http_status.OK) {
            var err : string
            var js = read_json(string(resp.body), err)
            if (js != null) {
                let v = int(js?["config"]?["page_rows"] ?? -1l)
                if (v > 0) {
                    rows = v
                }
                unsafe {
                    delete js
                }
            }
        }
    }
    return rows
}

//! one request of the cycle sent and read: the usage counts, the reply, the wall time
def private send_row(base_url : string; sweep, index : int; body : string) : ProbeRow {
    var r = ProbeRow(sweep = sweep, index = index, kind = probe_kind(index), conversation = probe_conversation(index))
    let t0 = ref_time_ticks()
    POST("{base_url}/v1/chat/completions", body) $(resp) {
        r.ms = double(get_time_usec(t0)) / 1000.0lf
        if (resp == null) {
            return
        }
        r.status = int(resp.status_code)
        var err : string
        var js = read_json(string(resp.body), err)
        if (js != null) {
            r.reply = js?["choices"]?[0]?["message"]?["content"] ?? ""
            r.prompt = int(js?["usage"]?["prompt_tokens"] ?? -1l)
            r.completion = int(js?["usage"]?["completion_tokens"] ?? -1l)
            r.cached = int(js?["usage"]?["prompt_tokens_details"]?["cached_tokens"] ?? -1l)
            unsafe {
                delete js
            }
        }
    }
    return r
}

//! the cycle against `base_url`, printed as it runs; the count of requests the server did not answer 200
def private run_cycle(base_url : string; max_tokens : int; preserve_thinking : bool) : int {
    let model = served_model(base_url)
    if (empty(model)) {
        print("{base_url}/v1/models did not answer - is the server up?\n")
        return PROBE_REQUESTS
    }
    let page_rows = served_page_rows(base_url)
    print("model {model}, {PROBE_REQUESTS} requests twice, {max_tokens} completion tokens each, cache page {page_rows} rows{preserve_thinking ? ", preserve_thinking" : ""}\n")
    var rows : array<ProbeRow>
    rows |> reserve(2 * PROBE_REQUESTS)
    var replies : array<string>
    replies |> resize(PROBE_FIRST)
    for (sweep in range(1, 3)) {
        for (index in range(1, PROBE_REQUESTS + 1)) {
            let body = probe_body(index, replies, model, max_tokens, preserve_thinking)
            var r <- send_row(base_url, sweep, index, body)
            if (sweep == 1 && r.kind == "first" && r.status == 200) {
                replies[r.conversation - 1] = r.reply
            }
            print("{probe_row_line(r)}\n")
            rows |> emplace(r)
        }
    }
    print("\n{probe_report(rows, page_rows)}")
    let failed = probe_summary(rows, page_rows).failed
    delete rows
    delete replies
    return failed
}

//! a server on `port` serving `model` from a thread of its own, the cycle run against it, the server shut down after
def private boot_and_probe(model : string; port, max_tokens : int; preserve_thinking : bool) : int {
    var failed = PROBE_REQUESTS
    with_stream() $(var handoff : Stream?) {
        handoff |> push_archive((path = model, id = base_name(model), port = port))
        with_job_status(1) $(finished) {
            new_thread() @() {
                handoff |> pop_archive() $(var job : tuple<path : string; id : string; port : int>) {
                    allow_cpu_prefill()
                    var m <- load_model(job.path, QuantMode.q8)
                    set_model(m, job.id)
                    run_server(job.port)   // blocks until POST /shutdown
                }
                handoff |> release
                finished |> notify_and_release
            }
            let ready = serve_clients(port, 1200, "/v1/models") $(base_url) {
                failed = run_cycle(base_url, max_tokens, preserve_thinking)
            }
            finished |> join
            if (!ready) {
                print("the server on port {port} never answered /v1/models\n")
            }
        }
    }
    return failed
}

[export]
def main : int {
    var args = Args()
    let rc = parse_args_with_help(args, "cache_probe")
    return rc if (rc >= 0)
    if (empty(args.url) && empty(args.model)) {
        print("give --url of a running server or -m <model.gguf> to boot one\n")
        return 1
    }
    var failed = 0
    if (empty(args.url)) {
        failed = boot_and_probe(args.model, args.port, args.max_tokens, args.preserve_thinking)
    } else {
        failed = run_cycle(args.url, args.max_tokens, args.preserve_thinking)
    }
    return failed == 0 ? 0 : 2
}
