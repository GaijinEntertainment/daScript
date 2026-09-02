options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

// The MTP ruler - one chat corpus, two engines, identical rendered prompts, one dated and sha-stamped
// JSON. Ours: the released bench exe at each depth (`--mtp-ab --prompts <corpus> --chat-prompts`),
// its per-prompt rows parsed. llama.cpp: a stock `llama-server` fed the SAME rendered thinking-off
// prompt raw through /completion (greedy, fixed n, ignore_eos), its timings read back. The identical
// continuation text is the control that makes the two acceptance columns comparable; llama-bench has
// no speculative arm and llama-cli re-wraps a raw prompt as a chat turn, which is why the server.
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration
require dasllama/dasllama_common
require dasllama/dasllama_math   // setup_dasllama_jobque_
require dasllama/dasllama_image   // load_model_cached - the .dlim rail
require dasllama/dasllama_tokenizer
require dasllama/dasllama_chat   // create_chat_renderer_ / set_thinking_ / add_user_ / render_turn_
require dasllama/dasllama_mtp_gemma   // gemma_drafter_sidecar
require ../performance/profile_common.das   // apply_box_profile_runtime
require dashv/dashv_boost   // GET / POST against the reference server
require daslib/jobque_boost
require daslib/json_boost
require daslib/clargs
require daslib/fio
require daslib/strings_boost   // join
require daslib/strings_convert   // try_to_double
require strings
require math

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "GGUF trunk (its mtp- head beside it is found the way the engine finds it)"
    model : string

    @clarg_doc = "Draft head GGUF for llama.cpp's -md (default: the mtp- sibling the engine would load; '-' = none)"
    head : string = ""

    @clarg_doc = "Stock llama-server binary (the pinned reference build); empty = skip llama.cpp"
    server : string = ""

    @clarg_doc = "Our released bench exe (default: the rig exe); empty = skip ours"
    exe : string = "modules/dasLLAMA/performance/_rig/dasllama-bench.app/Contents/MacOS/dasllama-bench"

    @clarg_doc = "Prompt corpus: %%-separated user turns, # comment lines"
    prompts : string = "modules/dasLLAMA/benchmarks/data/specbench4_prompts.txt"

    @clarg_short = "n"
    @clarg_doc = "Tokens generated per prompt (fixed; llama.cpp runs ignore_eos to match)"
    ngen : int = 128

    @clarg_short = "r"
    @clarg_doc = "Timed repetitions per prompt"
    reps : int = 3

    @clarg_doc = "Comma list of speculative depths run on both engines (off runs on both regardless)"
    depths : string = "1,2"

    @clarg_short = "t"
    @clarg_doc = "Threads for llama-server (-t); the exe takes its box profile"
    threads : int = 6

    @clarg_doc = "llama-server port"
    port : int = 18790

    @clarg_short = "o"
    @clarg_doc = "Output JSON path (default: stdout only)"
    out : string = ""

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private load_corpus(path : string) : array<string> {
    var texts : array<string>
    fopen(path, "rb") $(f) {
        if (f == null) {
            panic("mtp_ruler: can't open prompt corpus {path}")
        }
        for (p in split(fread(f), "%%")) {
            let tp = strip(p)
            if (!empty(tp) && !starts_with(tp, "#")) {
                texts |> push(clone_string(tp))
            }
        }
    }
    return <- texts
}

//! stdout of a shell command, "" when it fails to start
def private sh(cmd : string; secs : float = 600.0) : string {
    var out = ""
    unsafe(popen_timeout(cmd, secs, $(f : FILE const?) {
        if (f != null) {
            out = fread(f)
        }
    }))
    return out
}

def private first_line(s : string) : string {
    let lns <- split(s, "\n")
    return !empty(lns) ? strip(lns[0]) : ""
}

//! the first token of `<path>.sha` beside a model, "" when the sidecar is absent
def private sha_of(path : string) : string {
    let sp = "{path}.sha"
    if (!fexist(sp)) {
        return ""
    }
    let ws <- split(strip(fread(sp)), " ")
    return !empty(ws) ? ws[0] : ""
}

def private is_bos_piece(p : string) : bool {
    return p == "<bos>" || p == "<s>" || p == "<|begin_of_text|>" || p == "<|endoftext|>"
}

//! Every corpus turn rendered through the model's own chat template, thinking off, as the text the
//! reference server must see raw - the leading BOS piece dropped because the server adds its own.
def private render_prompts(model : string; texts : array<string>) : array<string> {
    var rendered : array<string>
    let tk <- load_tokenizer_auto(model)
    with_job_que() {
        setup_dasllama_jobque_()
        var t <- load_model_cached(model, QuantMode.q8)
        for (text in texts) {
            var cs <- create_chat_renderer_(t, "", 0l)
            set_thinking_(cs, false)
            add_user_(cs, text)
            var ids <- render_turn_(t, cs)
            finalize(cs)
            let sb = build_string() $(w) {
                var first = true
                for (id in ids) {
                    let p = piece(tk, id)
                    if (!(first && is_bos_piece(p))) {
                        w |> write(p)
                    }
                    first = false
                }
            }
            rendered |> push(sb)
            delete ids
        }
        delete t
    }
    return <- rendered
}

def private parse_depths(s : string) : array<int> {
    var ds : array<int>
    for (p in split(s, ",")) {
        let tp = strip(p)
        if (!empty(tp)) {
            ds |> push(to_int(tp))
        }
    }
    return <- ds
}

// ===== ours: the released exe, one --mtp-ab run per depth =====

def private num_before(s : string; stopAt : string) : double {
    let ix = find(s, stopAt)
    return try_to_double(strip(ix >= 0 ? slice(s, 0, ix) : s)) ?? 0.0lf
}

//! `prompt i: off A -> on B tok/s (xR), accept a/d (p%)` -> one row; "" when the line is not one
def private parse_das_prompt_row(ln : string; depth : int) : string {
    let ix = find(ln, "prompt ")
    return "" if (ix < 0 || find(ln, "accept ") < 0)
    let ws <- split(strip(slice(ln, ix)), " ")
    return "" if (length(ws) < 11)
    let w1 = ws[1]
    let pi = to_int(ends_with(w1, ":") ? slice(w1, 0, length(w1) - 1) : w1)
    let off = try_to_double(ws[3]) ?? 0.0lf
    let on = try_to_double(ws[6]) ?? 0.0lf
    let ad <- split(ws[10], "/")
    let acc = length(ad) == 2 ? to_int(ad[0]) : 0
    let dr = length(ad) == 2 ? to_int(ad[1]) : 0
    return write_json(JV((engine = "das", depth = depth, prompt = pi, off_tps = off, on_tps = on,
        drafted = dr, accepted = acc)))
}

def private run_das(cfg : Args; depth : int; var rows : array<string>) {
    var terr = ""
    let tmpo = "{temp_directory(terr)}/mtp_ruler_das_{depth}.txt"
    let cmd = ("DAS_TUNE_MANIFEST= \"{cfg.exe}\" -- -m \"{cfg.model}\" --mtp-ab --mtp-depth {depth} -n {cfg.ngen} -r {cfg.reps}"
        + " --ngl 99 --prompts \"{cfg.prompts}\" --chat-prompts > \"{tmpo}\" 2>&1")
    print("ruler: das depth {depth}: {cmd}\n")
    sh(cmd, 3600.0)
    var outp = ""
    if (fexist(tmpo)) {
        outp = fread(tmpo)
        remove(tmpo)
    }
    var off_mean = 0.0lf
    var on_mean = 0.0lf
    var accept_line = ""
    for (ln in split(outp, "\n")) {
        if (find(ln, "mtp=off: ") >= 0) {
            off_mean = num_before(slice(ln, find(ln, "mtp=off: ") + 9), " ")
        } elif (find(ln, "mtp=on: ") >= 0) {
            on_mean = num_before(slice(ln, find(ln, "mtp=on: ") + 8), " ")
        } elif (find(ln, "  mtp: ") >= 0) {
            accept_line = strip(ln)
        } else {
            let row = parse_das_prompt_row(ln, depth)
            if (!empty(row)) {
                rows |> push(row)
            }
        }
    }
    rows |> push(write_json(JV((engine = "das", depth = depth, prompt = -1, off_tps = off_mean, on_tps = on_mean,
        note = accept_line))))
    print("ruler: das depth {depth}: off {off_mean:.1f} on {on_mean:.1f} tok/s; {accept_line}\n")
}

// ===== llama.cpp: a stock llama-server, the rendered prompt raw through /completion =====

def private server_up(port : int) : bool {
    var ok = false
    GET("http://127.0.0.1:{port}/health") $(resp) {
        if (resp != null && resp.status_code == http_status.OK) {
            peek(resp.body) $(bs) {
                ok = find(bs, "\"ok\"") >= 0
            }
        }
    }
    return ok
}

def private wait_server(port : int; up : bool; secs : int) : bool {
    for (_i in range(secs)) {
        return true if (server_up(port) == up)
        sleep(1000u)
    }
    return false
}

struct LcppTimings {
    tps : double
    n_pred : int64
    n_prompt : int64
    draft_n : int64
    draft_acc : int64
    text : string
}

def private lcpp_completion(port : int; prompt : string; ngen : int) : LcppTimings {
    var r : LcppTimings
    let body = ("\{\"prompt\":{write_json_compact(JV(prompt))},\"n_predict\":{ngen},\"temperature\":0,\"top_k\":1,"
        + "\"ignore_eos\":true,\"cache_prompt\":false,\"stream\":false}")
    POST("http://127.0.0.1:{port}/completion", body) $(resp) {
        if (resp == null || resp.status_code != http_status.OK) {
            print("ruler: /completion failed ({resp == null ? -1 : int(resp.status_code)})\n")
            return
        }
        var err = ""
        var js : JsonValue?
        peek(resp.body) $(bs) {
            js = read_json(bs, err)
        }
        if (js == null) {
            print("ruler: /completion body is not JSON ({err})\n")
            return
        }
        let tm = js?["timings"]
        r.tps = tm?["predicted_per_second"] ?? 0.0lf
        r.n_pred = tm?["predicted_n"] ?? 0l
        r.n_prompt = tm?["prompt_n"] ?? 0l
        r.draft_n = tm?["draft_n"] ?? 0l
        r.draft_acc = tm?["draft_n_accepted"] ?? 0l
        let content = js?["content"] ?? ""
        r.text = clone_string(slice(content, 0, min(length(content), 96)))
        delete_json(js)
    }
    return r
}

def private run_lcpp_arm(cfg : Args; head : string; kmax : int; rendered : array<string>; var rows : array<string>) {
    var spec = ""
    if (kmax > 0) {
        spec = " --spec-type draft-mtp --spec-draft-n-max {kmax}"
        if (!empty(head)) {
            spec = "{spec} -md \"{head}\" -ngld 99"
        }
    }
    var terr = ""
    let log = "{temp_directory(terr)}/mtp_ruler_lcpp_{kmax}.log"
    let cmd = ("\"{cfg.server}\" --port {cfg.port} --host 127.0.0.1 -ngl 99 -fa on -c 2048 -t {cfg.threads} -np 1 --no-warmup -fit off"
        + " -m \"{cfg.model}\"{spec} > \"{log}\" 2>&1 & echo $!")
    print("ruler: lcpp n_max {kmax}: {cmd}\n")
    let pid = first_line(sh(cmd, 30.0))
    if (!wait_server(cfg.port, true, 300)) {
        print("ruler: llama-server did not come up in 300 s - log {log}\n")
        sh("kill {pid}", 10.0)
        return
    }
    rows |> reserve(length(rows) + length(rendered))
    for (pi in range(length(rendered))) {
        var tps_sum = 0.0lf
        var dn = 0l
        var da = 0l
        var npred = 0l
        var nprompt = 0l
        var text = ""
        for (_rep in range(cfg.reps)) {
            let r = lcpp_completion(cfg.port, rendered[pi], cfg.ngen)
            tps_sum += r.tps
            dn += r.draft_n
            da += r.draft_acc
            npred = r.n_pred
            nprompt = r.n_prompt
            text = r.text
        }
        let tps = tps_sum / double(max(cfg.reps, 1))
        rows |> push(write_json(JV((engine = "llama.cpp", depth = kmax, prompt = pi, tps = tps, predicted_n = npred,
            prompt_n = nprompt, drafted = dn, accepted = da, text = text))))
        print("ruler: lcpp n_max {kmax} prompt {pi}: {tps:.1f} tok/s over {npred} ({nprompt} prompt), accept {da}/{dn}\n")
    }
    sh("kill {pid}", 10.0)
    wait_server(cfg.port, false, 60)
    sleep(2000u)
}

[export]
def main() {
    if (!jit_enabled()) {
        print("This ruler is JIT-only - run with: daslang -jit <file>\n")
        return
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "mtp_ruler")
        return
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.model)) {
        print_help(get_command_info(type<Args>), "mtp_ruler")
        return
    }
    var head = cfg.head
    if (empty(head)) {
        head = gemma_drafter_sidecar(cfg.model)
        if (empty(head)) {
            let sib = path_join(dir_name(cfg.model), "mtp-{base_name(cfg.model)}")
            head = fexist(sib) ? sib : ""
        }
    } elif (head == "-") {
        head = ""
    }
    let texts <- load_corpus(cfg.prompts)
    print("ruler: {length(texts)} prompts from {cfg.prompts}; head = {empty(head) ? "(none)" : head}\n")
    var rows : array<string>
    let depths <- parse_depths(cfg.depths)
    if (!empty(cfg.exe)) {
        for (d in depths) {
            run_das(cfg, d, rows)
        }
    }
    if (!empty(cfg.server)) {
        apply_box_profile_runtime()
        set_metal_mode(MetalMode.when_available)
        let rendered <- render_prompts(cfg.model, texts)
        run_lcpp_arm(cfg, head, 0, rendered, rows)
        for (d in depths) {
            run_lcpp_arm(cfg, head, d, rendered, rows)
        }
    }
    let meta = JV((date = format_time(get_clock(), "%Y-%m-%d %H:%M"), host = first_line(sh("hostname")),
        cpu = first_line(sh("sysctl -n machdep.cpu.brand_string 2>/dev/null || uname -m")),
        das_sha = first_line(sh("git rev-parse --short HEAD")), das_exe = cfg.exe,
        lcpp_server = cfg.server, lcpp_version = first_line(sh("\"{cfg.server}\" --version 2>&1")),
        model = cfg.model, model_sha = sha_of(cfg.model), head = head, head_sha = sha_of(head),
        corpus = cfg.prompts, ngen = cfg.ngen, reps = cfg.reps, threads = cfg.threads, depths = cfg.depths))
    let text = "\{\"meta\":{write_json_compact(meta)},\"rows\":[{join(rows, ",")}]}"
    var err = ""
    var doc = read_json(text, err)
    if (doc == null) {
        print("ruler: assembled JSON does not parse ({err}) - raw follows\n{text}\n")
        return
    }
    let pretty = write_json(doc)
    if (!empty(cfg.out)) {
        fwrite(cfg.out, pretty)
        print("ruler: wrote {cfg.out}\n")
    } else {
        print("{pretty}\n")
    }
    delete_json(doc)
}
