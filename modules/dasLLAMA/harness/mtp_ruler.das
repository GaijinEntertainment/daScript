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
require dasllama/dasllama_gguf   // gguf_is_assistant
require ?das_metal dasllama/dasllama_metal_common   // finish_pending_step
require ?das_metal dasllama/dasllama_metal_decode
require ?das_metal dasllama/dasllama_metal_prefill
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

    @clarg_doc = "Draft head or assistant sidecar GGUF, also llama.cpp's -md (default: the assistant sidecar beside the trunk, else its mtp- sibling; '-' = none)"
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

    @clarg_doc = "GPU layers for both engines (99 = all on the GPU; 0 = the CPU rails: our NextN depth-1 round, llama-server -ngl 0 -ngld 0)"
    ngl : int = 99

    @clarg_doc = "llama-server port"
    port : int = 18790

    @clarg_doc = "Seconds to idle before every arm (default 90) - a heat-soaked box under-reads a memory-bound decode by 10-15%; the board driver settles the same way"
    settle : int = 90

    @clarg_short = "o"
    @clarg_doc = "Output JSON path (default: stdout only)"
    out : string = ""

    @clarg_doc = "Render an existing ruler JSON as a markdown table and exit (no measurement)"
    render : string = ""

    @clarg_name = "no-greedy"
    @clarg_doc = "Skip our in-process greedy continuation (the text control) - the prompts still render through our template"
    no_greedy : bool

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

//! the first `n` bytes of a continuation - what two engines are compared on
def private head_of(s : string; n : int) : string {
    var r = ""
    peek_data(s) $(d) {
        r = clone_string(slice(d, 0, min(length(d), n)))
    }
    return r
}

def private is_bos_piece(p : string) : bool {
    return p == "<bos>" || p == "<s>" || p == "<|begin_of_text|>" || p == "<|endoftext|>"
}

def private argmax_of(s : Session; vocab : int64) : int64 {
    var best = 0l
    var bv = s.logits[0]
    for (v in range64(1l, vocab)) {
        if (s.logits[v] > bv) {
            bv = s.logits[v]
            best = v
        }
    }
    return best
}

//! Our own greedy continuation of a rendered prompt, the text the reference must reproduce for the
//! two acceptance columns to be comparable (the same tokens seen by two drafters).
def private greedy_text(t : Model; tk : Tokenizer; ids : array<int64>; ngen : int) : string {
    var s <- make_run_state(t.config, KVDtype.f16, KVDtype.f16)
    eval_(t, s, ids)
    var tok = argmax_of(s, t.config.vocab_size)
    var one <- [tok]
    let text = build_string() $(w) {
        for (_i in range(ngen)) {
            w |> write(piece(tk, tok))
            one[0] = tok
            eval_(t, s, one)
            static_if (typeinfo builtin_module_exists(das_metal)) {
                finish_pending_step()
            }
            tok = argmax_of(s, t.config.vocab_size)
        }
    }
    delete one
    delete s
    return text
}

//! Every corpus turn rendered through the model's own chat template, thinking off, as the text the
//! reference server must see raw - the leading BOS piece dropped because the server adds its own -
//! plus our greedy continuation of it, the control the reference's text is checked against.
def private render_prompts(model : string; texts : array<string>; ngen : int; gpu : bool; var rendered : array<string>; var ours : array<string>) {   // nolint:LINT012,LINT019 — gpu is read in the das_metal half
    let tk <- load_tokenizer_auto(model)
    with_job_que() {
        setup_dasllama_jobque_()
        var t <- load_model_cached(model, QuantMode.q8)
        t.config.seq_len = min(t.config.seq_len, 2048l)
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (gpu) {   //! the control text comes from the rail under measurement - CPU kernels for a CPU row
                select_prefill_override("metal")
                set_metal_prefill_min_npos(1l)
                select_decode_override("metal")
            }
        }
        rendered |> reserve(length(texts))
        ours |> reserve(length(texts))
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
            ours |> push(ngen > 0 ? greedy_text(t, tk, ids, ngen) : "")
            delete ids
        }
        static_if (typeinfo builtin_module_exists(das_metal)) {
            if (gpu) {
                select_decode_override("")
                select_prefill_override("")
                metal_decode_shutdown()
                metal_prefill_shutdown()
            }
        }
        delete t
    }
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

//! The exe finds a gemma assistant head by its quant-stem rule; a NextN split head rides the shard
//! walk only under its exact trunk basename or `DASLLAMA_MTP_HEAD`, so a `-Q8_0` head is pinned.
def private run_das(cfg : Args; head : string; depth : int; var rows : array<string>) {
    var terr = ""
    let tmpo = "{temp_directory(terr)}/mtp_ruler_das_{depth}.txt"
    let pin = !empty(head) && !gguf_is_assistant(head) ? " DASLLAMA_MTP_HEAD=\"{head}\"" : ""
    let cmd = ("DAS_TUNE_MANIFEST={pin} \"{cfg.exe}\" -- -m \"{cfg.model}\" --mtp-ab --mtp-depth {depth} -n {cfg.ngen} -r {cfg.reps}"
        + " --ngl {cfg.ngl} --prompts \"{cfg.prompts}\" --chat-prompts > \"{tmpo}\" 2>&1")
    print("ruler: das depth {depth}: {cmd}\n")
    sleep(uint(max(cfg.settle, 0)) * 1000u)
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

def private run_lcpp_arm(cfg : Args; head : string; kmax : int; rendered, ours : array<string>; var rows : array<string>) {
    var spec = ""
    if (kmax > 0) {
        spec = " --spec-type draft-mtp --spec-draft-n-max {kmax}"
        if (!empty(head)) {
            spec = "{spec} -md \"{head}\" -ngld {cfg.ngl}"
        }
    }
    var terr = ""
    let log = "{temp_directory(terr)}/mtp_ruler_lcpp_{kmax}.log"
    let cmd = ("\"{cfg.server}\" --port {cfg.port} --host 127.0.0.1 -ngl {cfg.ngl} -fa on -c 2048 -t {cfg.threads} -np 1 --no-warmup -fit off"
        + " -m \"{cfg.model}\"{spec} > \"{log}\" 2>&1 & echo $!")
    print("ruler: lcpp n_max {kmax}: {cmd}\n")
    sleep(uint(max(cfg.settle, 0)) * 1000u)
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
        let same = pi < length(ours) && text == head_of(ours[pi], 96)
        rows |> push(write_json(JV((engine = "llama.cpp", depth = kmax, prompt = pi, tps = tps, predicted_n = npred,
            prompt_n = nprompt, drafted = dn, accepted = da, text = text, same_text_as_das = same))))
        print("ruler: lcpp n_max {kmax} prompt {pi}: {tps:.1f} tok/s over {npred} ({nprompt} prompt), accept {da}/{dn}{same ? "" : " - TEXT DIFFERS from ours"}\n")
    }
    sh("kill {pid}", 10.0)
    wait_server(cfg.port, false, 60)
    sleep(2000u)
}

// ===== --render: one markdown table per ruler JSON =====

def private pct(acc, dr : int64) : string {
    return dr > 0l ? "{double(acc) * 100.0lf / double(dr):.1f}%" : "-"
}

//! Row (engine, depth, prompt) -> cell text: "tok/s (accept%)" for an ON arm, "tok/s" for OFF
def private cell(rows : array<JsonValue?>; engine : string; depth, prompt : int64) : string {
    for (r in rows) {
        if ((r?["engine"] ?? "") != engine || (r?["depth"] ?? -9l) != depth || (r?["prompt"] ?? -9l) != prompt) {
            continue
        }
        if (engine == "das") {
            return depth == 0l ? "" : "{r?["on_tps"] ?? 0.0lf:.1f} ({pct(r?["accepted"] ?? 0l, r?["drafted"] ?? 0l)})"
        }
        let tps = r?["tps"] ?? 0.0lf
        return depth == 0l ? "{tps:.1f}" : "{tps:.1f} ({pct(r?["accepted"] ?? 0l, r?["drafted"] ?? 0l)})"
    }
    return ""
}

def private das_off(rows : array<JsonValue?>; prompt : int64) : string {
    for (r in rows) {
        if ((r?["engine"] ?? "") == "das" && (r?["depth"] ?? 0l) == 1l && (r?["prompt"] ?? -9l) == prompt) {
            return "{r?["off_tps"] ?? 0.0lf:.1f}"
        }
    }
    return ""
}

def private render_table(path : string) {
    var err = ""
    var doc = read_json(fread(path), err)
    if (doc == null) {
        print("mtp_ruler: {path} does not parse ({err})\n")
        return
    }
    let meta = doc?["meta"]
    print("{meta?["date"] ?? ""} - {meta?["cpu"] ?? ""} - das {meta?["das_sha"] ?? ""} - {meta?["lcpp_version"] ?? ""} - ngl {meta?["ngl"] ?? 99l}\n")
    print("model {base_name(meta?["model"] ?? "")} - head {base_name(meta?["head"] ?? "")} - corpus {base_name(meta?["corpus"] ?? "")} - n {meta?["ngen"] ?? 0l} - reps {meta?["reps"] ?? 0l}\n\n")
    let rn = doc?["rows"]
    var rows : array<JsonValue?>
    if (rn != null && rn.value is _array) {
        rows := rn.value as _array
    }
    let depths <- split(meta?["depths"] ?? "1,2", ",")
    let head = build_string() $(w) {
        w |> write("| prompt | das off | ")
        for (d in depths) {
            w |> write("das depth {strip(d)} | ")
        }
        w |> write("llama.cpp off | ")
        for (d in depths) {
            w |> write("llama.cpp n_max {strip(d)} | ")
        }
        w |> write("same text |\n|---|---|")
        for (_d in range(2 * length(depths) + 2)) {
            w |> write("---|")
        }
    }
    print("{head}\n")
    var nprompts = 0l
    for (r in rows) {
        nprompts = max(nprompts, (r?["prompt"] ?? -1l) + 1l)
    }
    for (pi in range64(nprompts)) {
        let line = build_string() $(w) {
            w |> write("| {pi} | {das_off(rows, pi)} | ")
            for (d in depths) {
                w |> write("{cell(rows, "das", to_int64(strip(d), false), pi)} | ")
            }
            w |> write("{cell(rows, "llama.cpp", 0l, pi)} | ")
            for (d in depths) {
                w |> write("{cell(rows, "llama.cpp", to_int64(strip(d), false), pi)} | ")
            }
        }
        var same = "-"
        for (r in rows) {
            if ((r?["engine"] ?? "") == "llama.cpp" && (r?["depth"] ?? -9l) == 0l && (r?["prompt"] ?? -9l) == pi) {
                same = (r?["same_text_as_das"] ?? false) ? "yes" : "NO"
            }
        }
        print("{line}{same} |\n")
    }
    for (r in rows) {
        if ((r?["engine"] ?? "") == "das" && (r?["prompt"] ?? 0l) == -1l) {
            print("\ndas depth {r?["depth"] ?? 0l} pooled: off {r?["off_tps"] ?? 0.0lf:.1f} on {r?["on_tps"] ?? 0.0lf:.1f} tok/s; {r?["note"] ?? ""}\n")
        }
    }
    delete_json(doc)
}

//! --head names the draft head or assistant sidecar; "-" runs without one; empty resolves the
//! assistant sidecar first, then the `mtp-<model>` sibling beside the trunk
def private resolve_head(cfg : Args) : string {
    if (cfg.head == "-") {
        return ""
    }
    if (!empty(cfg.head)) {
        return cfg.head
    }
    let side = gemma_drafter_sidecar(cfg.model)
    if (!empty(side)) {
        return side
    }
    let sib = path_join(dir_name(cfg.model), "mtp-{base_name(cfg.model)}")
    return fexist(sib) ? sib : ""
}

[export]
def main() : int {
    if (!jit_enabled()) {
        print("This ruler is JIT-only - run with: daslang -jit <file>\n")
        return 1
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "mtp_ruler")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (!empty(cfg.render)) {
        render_table(cfg.render)
        return 0
    }
    if (cfg.help || empty(cfg.model)) {
        print_help(get_command_info(type<Args>), "mtp_ruler")
        return cfg.help ? 0 : 1
    }
    let head = resolve_head(cfg)
    let texts <- load_corpus(cfg.prompts)
    print("ruler: {length(texts)} prompts from {cfg.prompts}; head = {empty(head) ? "(none)" : head}\n")
    var rows : array<string>
    let depths <- parse_depths(cfg.depths)
    if (!empty(cfg.exe)) {
        for (d in depths) {
            run_das(cfg, head, d, rows)
        }
    }
    apply_box_profile_runtime()
    set_metal_mode(cfg.ngl > 0 ? MetalMode.when_available : MetalMode.off)
    if (cfg.ngl == 0) {
        allow_cpu_prefill()   //! a CPU row prefills its control text on the CPU by design
    }
    var rendered : array<string>
    var ours : array<string>
    render_prompts(cfg.model, texts, cfg.no_greedy ? 0 : cfg.ngen, cfg.ngl > 0, rendered, ours)
    rows |> reserve(length(rows) + length(ours))
    for (pi in range(length(ours))) {
        continue if (empty(ours[pi]))
        let head96 = head_of(ours[pi], 96)
        rows |> push(write_json(JV((engine = "das", depth = 0, prompt = pi, text = head96))))
        print("ruler: das greedy prompt {pi}: {head96}\n")
    }
    if (!empty(cfg.server)) {
        run_lcpp_arm(cfg, head, 0, rendered, ours, rows)
        for (d in depths) {
            run_lcpp_arm(cfg, head, d, rendered, ours, rows)
        }
    }
    if (empty(rows)) {
        print("ruler: no result rows - nothing was measured (check --exe, --server, --no-greedy and the model)\n")
        return 1
    }
    return write_report(cfg, head, rows) ? 0 : 1
}

//! the dated, sha-stamped record: provenance of both engines, the model and its head, then the rows
[arch(at="../ARCHITECTURE_MEASUREMENT.md#ruler-records")]
def private write_report(cfg : Args; head : string; rows : array<string>) : bool {
    let meta = JV((date = format_time(get_clock(), "%Y-%m-%d %H:%M"),
        cpu = first_line(sh("sysctl -n machdep.cpu.brand_string 2>/dev/null || uname -m")),
        das_sha = first_line(sh("git rev-parse --short HEAD")), das_exe = cfg.exe,
        lcpp_server = cfg.server, lcpp_version = first_line(sh("\"{cfg.server}\" --version 2>&1")),
        model = cfg.model, model_sha = sha_of(cfg.model), head = head, head_sha = sha_of(head),
        corpus = cfg.prompts, ngen = cfg.ngen, reps = cfg.reps, threads = cfg.threads, ngl = cfg.ngl, depths = cfg.depths))
    let text = "\{\"meta\":{write_json_compact(meta)},\"rows\":[{join(rows, ",")}]}"
    var err = ""
    var doc = read_json(text, err)
    if (doc == null) {
        print("ruler: assembled JSON does not parse ({err}) - raw follows\n{text}\n")
        return false
    }
    let pretty = write_json(doc)
    if (!empty(cfg.out)) {
        fwrite(cfg.out, pretty)
        print("ruler: wrote {cfg.out}\n")
    } else {
        print("{pretty}\n")
    }
    delete_json(doc)
    return true
}
