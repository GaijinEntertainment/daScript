options gen2
options indenting = 4
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// model_bench_run - the model bench's runner: for each model of the config it starts a server with that model, waits
// until it answers, runs `model_bench.das` against it, stops the server, and writes the model's card - a ladder record
// with the bench's rows as its tests. ONE model process on the box at a time.
//
//   bin/daslang modules/dasLLAMA/harness/model_bench_run.das -- [--model <entry> ...] [--server-tree <daslang tree>]
//       [--llama-server <llama-server binary>] [--server-arg <argument> ... [--server-engine <name>]] [--gpu vulkan]
//       [--models-dir <dir>] [--out-dir <dir>] [--port 8911] [--config <json>] [--quick]
//       [--max-length N] [--cell-seconds S] [--reps N] [--wide] [--boot-seconds 900]
//
// The server runs from `--server-tree` (default: this tree) with that tree's own binary, so the bench of one tree can
// read the server of another. A model's server flags are its config entry's `server` object; the environment the
// runner was started under is the server's. The server and the bench are children of the runner: it keeps the
// server's output in a log, and a server that never answers or does not stop is killed with everything it started.
// Per model, under `--out-dir` (default `logs/model_bench` of this tree): `<entry>.card.json` (the record),
// `<entry>.bench.json` (the bench's own file: every row's fields, its flags, what it refused, how it ran),
// `<entry>.requests.jsonl` (every timed request) and `<entry>.server.log`.
// `--llama-server` runs the reference in ours' place: llama.cpp's server on the same file, every layer on the GPU, the
// slots and the context a slot of the config, greedy (`--temp 0`: the bench's client cannot send a zero temperature).
// Its files carry `.ref` in their names and its card is an engine `llama.cpp` run, so the two cards of a model sit
// side by side. `--server-arg` runs any other server: its command line an argument a flag, `{gguf}`, `{port}`,
// `{ctx}` and `{streams}` filled in; an arm's flags are the config's `das` side.
// A model whose entry lists arms gets one server start an arm: `off` runs the whole ladder, every other arm starts
// the server with the arm's flags from the config's `arm_flags` table and runs the bench's `--arm` rows. Every arm's
// rows land in the model's one card; an arm's own files carry its name. A model with arms gets one more bench run at
// its plain server - the arms' long-reply rows with no drafter (`--arm off`, files `.long`) - so every arm's row has
// a plain row of the same reply length beside it.
// `--wide` adds the wide pass: one more server start under the entry's `wide_server` - the streams a wide wave takes,
// each with room for the longest width prompt - and the bench's `--wide` rows (8, 16, 32 streams at once).
// Exit 1 when a model's server did not serve, its bench reported a refusal, or its card could not be written.

require ../performance/profile_common.das   // BenchModel / BenchRun, gather_hardware, file_identity, bench_env_line, git_commit_at
require dashv/dashv_boost   // GET / POST: the server's model list and its shutdown
require daslib/clargs
require daslib/command_line   // get_das_exe: the bench child runs on this process's binary
require daslib/fio   // with_process: the server and the bench as children
require daslib/strings_boost   // join, replace, find
require strings
require math
require _model_bench  // the config's shape and the run file the bench writes

[CommandLineArgs]
struct RunnerArgs {
    @clarg_short = "m"
    @clarg_doc = "A config entry to run (repeatable; default: every model of the config)"
    model : array<string>

    @clarg_name = "server-tree"
    @clarg_doc = "The daslang tree the server runs from, with its own binary (default: this tree)"
    server_tree : string

    @clarg_doc = "The server's --gpu, and the backend the card names (default: vulkan)"
    gpu : string

    @clarg_name = "llama-server"
    @clarg_doc = "Run the reference in ours' place: the llama-server binary of a llama.cpp build"
    llama_server : string

    @clarg_name = "server-arg"
    @clarg_doc = "Run another server in ours' place: its command line, one argument a flag (repeatable); \{gguf\}, \{port\}, \{ctx\} and \{streams\} in an argument stand for the model file, the port and the entry's server plan"
    server_arg : array<string>

    @clarg_name = "server-engine"
    @clarg_doc = "The engine the card names for a --server-arg server (default: custom)"
    server_engine : string

    @clarg_name = "models-dir"
    @clarg_doc = "Where the model files are (default: DASLLAMA_MODELS_DIR)"
    models_dir : string

    @clarg_name = "out-dir"
    @clarg_doc = "Where the cards and the logs go (default: logs/model_bench under this tree)"
    out_dir : string

    @clarg_short = "p"
    @clarg_doc = "The port the server listens on (default 8911)"
    port : int

    @clarg_doc = "The config file (default: modules/dasLLAMA/harness/model_bench.json)"
    config : string

    @clarg_doc = "Pass --quick to the bench: one rep a cell"
    quick : bool

    @clarg_name = "max-length"
    @clarg_doc = "Pass --max-length to the bench"
    max_length : int

    @clarg_name = "cell-seconds"
    @clarg_doc = "Pass --cell-seconds to the bench"
    cell_seconds : int

    @clarg_doc = "Pass --reps to the bench"
    reps : int

    @clarg_doc = "Add the wide pass: a server start under the entry's wide_server, and the bench's wide widths"
    wide : bool

    @clarg_name = "boot-seconds"
    @clarg_doc = "Seconds a server may take to answer after its start (default 900)"
    boot_seconds : int

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

let STOP_SECONDS = 120
let DEFAULT_PORT = 8911
let DEFAULT_BOOT_SECONDS = 900

//! the daslang binary a tree built: the MSVC layout, else the single-config one; "" when the tree has neither
def tree_binary(tree : string) : string {
    for (rel in ["bin/Release/daslang.exe", "bin/daslang.exe", "bin/daslang"]) {
        let p = path_join(tree, rel)
        return p if (fexist(p))
    }
    return ""
}

//! the server's model list as it answers it; empty when nothing answers on `port`
def server_models(port : int) : string {
    var body = ""
    GET("http://127.0.0.1:{port}/v1/models") $(resp) {
        if (resp != null && resp.status_code == http_status.OK) {
            body = string(resp.body)
        }
    }
    return body
}

//! a command line as one text, for the card and the log: an argument with a blank in it quoted
def line_of(argv : array<string>) : string
    => join([for (a in argv); find(a, " ") >= 0 ? "\"{a}\"" : a], " ")

//! the server a run starts for a model; `argv` is empty when its binary is not there
struct Server {
    engine : string             //! the card's engine
    flavor : string
    tree : string               //! the directory it starts in ("": the runner's)
    sha : string
    asks_stop : bool            //! it has a shutdown route; any other server's process is stopped by its handle
    versioned : bool            //! a daslang build: the card carries its version
    argv : array<string>
    env : array<string>
}

//! the fields of a server a card carries, and every arm's command line
struct CardServer {
    engine : string
    flavor : string
    sha : string
    versioned : bool
    line : string
}

//! ours for one model, from a daslang tree with that tree's own binary
def our_server(args : RunnerArgs; gguf : string; port : int; plan : ServerPlan; extra : array<string>) : Server {
    let tree = empty(args.server_tree) ? get_das_root() : args.server_tree
    let binary = tree_binary(tree)
    var s <- Server(engine = "das", flavor = "tuned", tree = tree, sha = git_commit_at(tree), asks_stop = true, versioned = true)
    return <- s if (empty(binary))
    s.argv <- [binary, "-jit", "utils/dasllama-server/main.das", "--", "-m", gguf, "-p", "{port}", "--gpu", empty(args.gpu) ? "vulkan" : args.gpu]
    if (plan.ctx > 0) {
        s.argv |> push_from(["--ctx", "{plan.ctx}"])
    }
    if (plan.streams > 0) {
        s.argv |> push_from(["--streams", "{plan.streams}"])
    }
    s.argv |> push_from(plan.flags, extra)
    s.env |> push("DAS_LOG_LEVEL=info")
    return <- s
}

//! the reference for one model: llama-server holds ONE context for all its slots, so it gets the plan's context a slot
def reference_server(args : RunnerArgs; gguf : string; port : int; plan : ServerPlan; extra : array<string>) : Server {
    let slots = max(plan.streams, 1)
    let tree = dir_name(args.llama_server)
    var s <- Server(engine = "llama.cpp", flavor = "stock", tree = tree, sha = git_commit_at(tree))
    return <- s if (!fexist(args.llama_server))
    s.argv <- [args.llama_server, "-m", gguf, "--port", "{port}", "-ngl", "99", "-np", "{slots}"]
    if (plan.ctx > 0) {
        s.argv |> push_from(["-c", "{plan.ctx * slots}"])
    }
    s.argv |> push_from(["--temp", "0"], extra)
    return <- s
}

//! `arg` with the model file, the port and the plan's context and streams in place of their placeholders
def private filled_arg(arg, gguf : string; port : int; plan : ServerPlan) : string {
    let named = replace(replace(arg, "\{gguf\}", gguf), "\{port\}", "{port}")
    return replace(replace(named, "\{ctx\}", "{plan.ctx}"), "\{streams\}", "{max(plan.streams, 1)}")
}

//! another server for one model: `--server-arg`'s command line with its placeholders filled, the arm's flags after it
def named_server(args : RunnerArgs; gguf : string; port : int; plan : ServerPlan; extra : array<string>) : Server {
    var s <- Server(engine = empty(args.server_engine) ? "custom" : args.server_engine, flavor = "stock")
    s.argv <- [for (a in args.server_arg); filled_arg(a, gguf, port, plan)]
    s.argv |> push_from(extra)
    return <- s
}

//! the server the flags ask for one model
def server_of(args : RunnerArgs; gguf : string; port : int; plan : ServerPlan; extra : array<string>) : Server {
    return <- named_server(args, gguf, port, plan, extra) if (!empty(args.server_arg))
    return <- reference_server(args, gguf, port, plan, extra) if (!empty(args.llama_server))
    return <- our_server(args, gguf, port, plan, extra)
}

//! which `ArmPlan` side the flags' server reads
def reference_side(args : RunnerArgs) : bool => !empty(args.llama_server) && empty(args.server_arg)

//! the model's card: one run of a ladder record, the bench's rows as its tests
def write_card(path, gguf, gpu : string; server : CardServer; hardware : HardwareInfo; res : RunOut) : bool {
    var run <- BenchRun(engine = server.engine, backend = gpu, flavor = server.flavor, box = box_name(),
        date = format_time(get_clock(), "%Y-%m-%d"), cmd = "{server.line} ; {res.bench.cmd}", hardware = hardware,
        sha = server.sha, version = server.versioned ? das_version() : "", env = bench_env_line())
    for (name, row in keys(res.tests), values(res.tests)) {
        run.tests |> insert(name, BenchTest(tok_s = row.tok_s, stddev = row.stddev))
    }
    run.files |> push(file_identity(gguf, "weights"))
    var model <- BenchModel(gguf = base_name(gguf))
    model.runs |> emplace(run)
    var models : array<BenchModel>
    models |> emplace(model)
    let wrote = write_bench_records(path, models)
    delete models
    return wrote
}

//! the bench child's command line against the server at `port`, the runner's own bench flags passed on
def bench_argv(args : RunnerArgs; entry, stem, arm : string; port, streams : int; long_plain : bool = false) : array<string> {
    var a <- [get_das_exe(), path_join(get_das_root(), "modules/dasLLAMA/harness/model_bench.das"), "-dasroot", get_das_root(), "--",
        "--url", "http://127.0.0.1:{port}", "--model", entry, "--out", "{stem}.bench.json", "--raw", "{stem}.requests.jsonl"]
    if (long_plain) {
        a |> push_from(["--arm", "off"])
    } elif (arm == "wide") {
        a |> push("--wide")
    } elif (arm != "off") {
        a |> push_from(["--arm", arm])
    }
    if (args.quick) {
        a |> push("--quick")
    }
    if (streams > 0) {
        a |> push_from(["--streams", "{streams}"])
    }
    if (args.max_length > 0) {
        a |> push_from(["--max-length", "{args.max_length}"])
    }
    if (args.cell_seconds > 0) {
        a |> push_from(["--cell-seconds", "{args.cell_seconds}"])
    }
    if (args.reps > 0) {
        a |> push_from(["--reps", "{args.reps}"])
    }
    if (!empty(args.config)) {
        a |> push_from(["--config", args.config])
    }
    return <- a
}

//! the entries a run covers: the ones named, else every model of the config by name
def entries_of(config : BenchConfig; named : array<string>) : array<string> {
    return clone_to_move(named) if (!empty(named))
    var names <- [for (name in keys(config.models)); name]
    sort(names)
    return <- names
}

//! the server flags of `arm` for one side, the model's drafter path in place of `\{drafter\}`; empty for `off`, for an
//! arm the config does not name, and for one with no flags on that side
def arm_flags(var config : BenchConfig; arm, drafter : string; reference : bool) : array<string> {
    return <- array<string>() if (arm == "off" || !key_exists(config.arm_flags, arm))
    let plan & = unsafe(config.arm_flags[arm])
    return <- [for (f in reference ? plan.llama : plan.das); replace(f, "\{drafter\}", drafter)]
}

//! the rows, the refusals and the command line of the bench run file at `path` joined to `into`; false when the file does not read
def take_run(path : string; var into : RunOut) : bool {
    var res : RunOut
    return false if (!sscan_json(fread(path), res))
    for (name, row in keys(res.tests), values(res.tests)) {
        into.tests |> insert(name, row)
    }
    into.refused |> push_from(res.refused)
    into.bench.cmd = empty(into.bench.cmd) ? res.bench.cmd : "{into.bench.cmd} ; {res.bench.cmd}"
    delete res
    return true
}

//! how a server's run ended: `why` names what kept it from serving ("" when it served), `rc` the bench children's exit
//! codes or-ed, `stopped` whether it exited inside its stop time once asked
struct ServerRun {
    why : string
    rc : int
    stopped : bool
}

//! the lines a child has ready, each to `log`
def private pump(var p : process; log : FILE const?) {
    unsafe(process_drain(p) $(line) {
        fprint(log, "{line}\n")
    })
}

//! `bench` run to its end as a child, its lines printed as they come and `server`'s kept drained into `log` meanwhile
//! (a child whose output nobody reads stops at a full pipe); the bench's exit code
def private run_bench(bench : array<string>; var server : process; log : FILE const?) : int {
    var rc = process_running
    with_process(bench) $(var b) {
        while (rc == process_running) {
            unsafe(process_drain(b) $(line) {
                print("{line}\n")
            })
            pump(server, log)
            rc = unsafe(process_poll(b))
            if (rc == process_running) {
                sleep(50u)
            }
        }
        unsafe(process_drain(b) $(line) {
            print("{line}\n")
        })
    }
    return rc
}

//! true once `port` names `model` in its list; false with `why` set when the child exits first, when `secs` pass, or
//! when the list that answers does not name it - another process holds the port
def private wait_serving(var p : process; log : FILE const?; port, secs : int; model : string; var why : string&) : bool {
    for (_i in range(secs)) {
        pump(p, log)
        let models = server_models(port)
        if (!empty(models)) {
            why = find(models, model) >= 0 ? "" : "the server on port {port} does not name {model}: another process holds the port"
            return empty(why)
        }
        if (unsafe(process_poll(p)) != process_running) {
            why = "the server exited before it answered"
            return false
        }
        sleep(1000u)
    }
    why = "the server did not answer in {secs} s"
    return false
}

//! `server` up as a child of this process, every command line of `benches` run against it in turn, then the server
//! down. The child is the runner's to the end: its output goes to `log`, and closing its handle stops whatever of it
//! still runs, so a server that never answers or never stops takes no port into the next start.
def run_server(server : Server; port, boot_secs : int; log, model : string; benches : array<array<string>>) : ServerRun {
    var run : ServerRun
    let flog = fopen(log, "wb")
    if (flog == null) {
        run.why = "the log {log} does not open"
        return run
    }
    with_process(server.argv, server.tree, server.env) $(var p) {
        if (wait_serving(p, flog, port, boot_secs, model, run.why)) {
            for (bench in benches) {
                run.rc |= run_bench(bench, p, flog)
            }
            if (server.asks_stop) {
                POST("http://127.0.0.1:{port}/shutdown", "") $(_resp) {
                }
            } else {
                unsafe(process_terminate(p))
            }
            for (_i in range(STOP_SECONDS * 10)) {
                pump(p, flog)
                run.stopped = unsafe(process_poll(p)) != process_running
                break if (run.stopped)
                sleep(100u)
            }
        }
        pump(p, flog)
    }
    fclose(flog)
    return run
}

//! One arm of one model: its server up, the bench against it, the server down. The arm's rows join `into`; false when
//! the server did not serve or stop, or the bench refused a cell. `card` takes the arm's server and command line.
def run_arm(args : RunnerArgs; var config : BenchConfig; entry, arm, gguf, out_dir : string; s : Settings; var card : CardServer&; var into : RunOut) : bool {
    let port = args.port > 0 ? args.port : DEFAULT_PORT
    let named = drafter_of(s, arm)
    let drafter = empty(named) ? "" : path_join(empty(args.models_dir) ? models_dir() : args.models_dir, named)
    let drafting = arm != "off" && arm != "wide"
    var inscope extra <- arm_flags(config, arm, drafter, reference_side(args))
    if (drafting && empty(extra)) {
        print("model_bench_run: {entry} arm {arm} SKIPPED - the config's arm_flags name no flags for it on this server\n")
        return false
    }
    var inscope server <- server_of(args, gguf, port, s.server, extra)
    if (empty(server.argv) || (drafting && !empty(drafter) && !fexist(drafter))) {
        print("model_bench_run: {entry} arm {arm} SKIPPED - the server's binary or the drafter {drafter} is not there\n")
        return false
    }
    if (!empty(server_models(port))) {
        print("model_bench_run: {entry} REFUSED - something already answers on port {port}: one model process a box\n")
        return false
    }
    let side = reference_side(args) ? "{entry}.ref" : entry
    let stem = path_join(out_dir, arm == "off" ? side : "{side}.{arm}")
    let line = line_of(server.argv)
    card = CardServer(engine = server.engine, flavor = server.flavor, sha = server.sha, versioned = server.versioned,
        line = empty(card.line) ? line : "{card.line} ; {line}")
    print("model_bench_run: {entry} [{arm}]: {line}\n")
    let long_plain = arm == "off" && length(s.arms) > 1
    var inscope benches : array<array<string>>
    benches |> emplace(bench_argv(args, entry, stem, arm, port, s.server.streams))
    remove("{stem}.requests.jsonl")
    remove("{stem}.bench.json")
    if (long_plain) {
        remove("{stem}.long.bench.json")
        remove("{stem}.long.requests.jsonl")
        benches |> emplace(bench_argv(args, entry, "{stem}.long", arm, port, s.server.streams, true))
    }
    let run = run_server(server, port, args.boot_seconds > 0 ? args.boot_seconds : DEFAULT_BOOT_SECONDS, "{stem}.server.log", base_name(gguf), benches)
    if (!empty(run.why)) {
        print("model_bench_run: {entry} [{arm}] FAILED - {run.why}; see {stem}.server.log\n")
        return false
    }
    if (!run.stopped) {
        print("model_bench_run: {entry} [{arm}]: the server ran {STOP_SECONDS} s past its stop and was killed\n")
    }
    var took = take_run("{stem}.bench.json", into)
    if (long_plain) {
        took = take_run("{stem}.long.bench.json", into) && took
    }
    if (!took) {
        print("model_bench_run: {entry} [{arm}] FAILED - a bench run wrote no file under {stem} (exit {run.rc})\n")
    }
    return took && run.rc == 0 && run.stopped
}

//! One model: every arm of its entry, then its card over all of them; false when an arm failed or no card was written
[arch(at = "../ARCHITECTURE_MEASUREMENT_SERVED.md#model-bench-runner")]
def run_model(args : RunnerArgs; var config : BenchConfig; entry : string; s : Settings; hardware : HardwareInfo) : bool {
    let gguf = path_join(empty(args.models_dir) ? models_dir() : args.models_dir, "{entry}.gguf")
    let out_dir = empty(args.out_dir) ? path_join(get_das_root(), "logs/model_bench") : args.out_dir
    if (!fexist(gguf)) {
        print("model_bench_run: {entry} SKIPPED - {gguf} is not there\n")
        return false
    }
    mkdir_rec(out_dir)
    var all : RunOut
    var card : CardServer
    var ok = true
    for (arm in s.arms) {
        ok = run_arm(args, config, entry, arm, gguf, out_dir, s, card, all) && ok
    }
    if (args.wide && s.wide_server.streams > 0) {
        var wide := s
        wide.server := s.wide_server
        ok = run_arm(args, config, entry, "wide", gguf, out_dir, wide, card, all) && ok
        delete wide
    }
    let stem = path_join(out_dir, reference_side(args) ? "{entry}.ref" : entry)
    let carded = !empty(all.tests) && write_card("{stem}.card.json", gguf, empty(args.gpu) ? "vulkan" : args.gpu, card, hardware, all)
    let card_note = carded ? "card {stem}.card.json" : "NO card written"
    print("model_bench_run: {entry}: {length(all.tests)} row(s) over {length(s.arms)} arm(s), {length(all.refused)} refused, {card_note}\n")
    delete all
    return ok && carded
}

//! every entry the flags name, each through `run_model`; the models that failed
def run_models(args : RunnerArgs; var config : BenchConfig) : int {
    var entries <- entries_of(config, args.model)
    let hardware = gather_hardware()
    var failed = 0
    for (entry in entries) {
        var picked <- settings_of(config, entry)
        failed += run_model(args, config, entry, picked.s, hardware) ? 0 : 1
        delete picked
    }
    print("model_bench_run: {length(entries)} model(s), {failed} failed\n")
    delete entries
    return failed
}

[export]
def main : int {
    var r <- parse_args(type<RunnerArgs>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<RunnerArgs>), "model_bench_run")
        return 1
    }
    let args <- r |> move_unwrap
    if (args.help) {
        print_help(get_command_info(type<RunnerArgs>), "model_bench_run")
        return 0
    }
    let config_path = empty(args.config) ? default_config_path() : args.config
    var config : BenchConfig
    if (!read_config(config_path, config)) {
        print("error: the config {config_path} does not read\n")
        return 1
    }
    return run_models(args, config) > 0 ? 1 : 0
}
