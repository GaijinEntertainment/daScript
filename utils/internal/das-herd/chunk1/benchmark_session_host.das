options gen2
options indenting = 4

require ../chunk0/git_terminal_probe.das
require daslib/fio
require daslib/strings_boost
require strings
require math

struct BenchmarkOptions {
    repository : string
    warmup : int = 5
    rounds : int = 30
    discard : bool
}

struct HostedSample {
    ok : bool
    exit_code : int64 = -1l
    ready_us : int64 = -1l
    hosted_exit_us : int64 = -1l
    inner_exit_us : int64 = -1l
    inner_total_us : int64 = -1l
    outer_total_us : int64 = -1l
    output_bytes : int
    output_text : string
    error : string
}

struct ComparisonSummary {
    direct_exit_p50 : int64
    direct_exit_p95 : int64
    terminal_exit_p50 : int64
    terminal_exit_p95 : int64
    host_ready_p50 : int64
    host_ready_p95 : int64
    hosted_exit_p50 : int64
    hosted_exit_p95 : int64
    hosted_inner_p50 : int64
    hosted_outer_p50 : int64
}

def private elapsed_us(started : int64) : int64 {
    return get_time_nsec(started) / 1000l
}

def private parse_options() : BenchmarkOptions {
    var cfg = BenchmarkOptions(repository = get_das_root())
    for (arg in get_command_line_arguments()) {
        if (starts_with(arg, "--repo=")) {
            cfg.repository = slice(arg, length("--repo="))
        } elif (starts_with(arg, "--warmup=")) {
            cfg.warmup = max(0, to_int(slice(arg, length("--warmup="))))
        } elif (starts_with(arg, "--rounds=")) {
            cfg.rounds = max(1, to_int(slice(arg, length("--rounds="))))
        } elif (arg == "--discard") {
            cfg.discard = true
        }
    }
    return cfg
}

def private percentile_us(var values : array<int64>; numerator : int) : int64 {
    if (empty(values)) return -1l
    sort(values)
    let index = clamp((length(values) * numerator + 99) / 100 - 1,
        0, length(values) - 1)
    return values[index]
}

def private read_file(path : string; var error : string&) : string {
    var file = fopen(path, "rb")
    if (file == null) {
        error = "could not read {path}"
        return ""
    }
    let result = fread(file)
    fclose(file)
    return result
}

def private parse_host_result(output : string; var sample : HostedSample) {
    for (line in split_by_chars(output, "\n")) {
        if (!starts_with(line, "DASHERD_HOST_RESULT\t")) continue
        let fields <- split_by_chars(strip(line), "\t")
        if (length(fields) != 10) {
            sample.error = "malformed host result: {line}"
            return
        }
        sample.exit_code = to_int64(fields[1])
        sample.inner_exit_us = to_int64(fields[4])
        sample.inner_total_us = to_int64(fields[8])
        sample.output_bytes = to_int(fields[9])
        sample.ok = true
        return
    }
    sample.error = "session host produced no result: {output}"
}

def private run_hosted_probe(command : GitProbeCommand const;
                             repository, log_path, screen_path : string) : HostedSample {
    var sample = HostedSample()
    let root = get_das_root()
    let executable = path_join(root, "bin/Release/daslang.exe")
    let worker = path_join(root,
        "utils/internal/das-herd/chunk1/session_host_worker.das")
    let argv = [executable, worker, "--", "--repo={repository}",
        "--probe={command.name}", "--log={log_path}", "--screen={screen_path}"]
    var first_byte : array<uint8>
    first_byte |> resize(1)
    var first_count = 0
    var remainder : string
    let started = ref_time_ticks()
    let worker_rc = unsafe(popen_argv(argv, 0.0, $(f) {
        if (f == null) {
            sample.error = "session host returned a null output stream"
            return
        }
        first_count = fread(f, first_byte)
        if (first_count > 0) {
            sample.ready_us = elapsed_us(started)
        }
        remainder = fread(f)
    }))
    sample.outer_total_us = elapsed_us(started)
    let output = (first_count > 0 ? to_char(int(first_byte[0])) : "") + remainder
    if (worker_rc != 0) {
        sample.error = "session host exited {worker_rc}: {output}"
        return <- sample
    }
    parse_host_result(output, sample)
    if (!sample.ok) return <- sample
    sample.hosted_exit_us = sample.ready_us + sample.inner_exit_us
    sample.output_text = read_file(screen_path, sample.error)
    sample.ok = empty(sample.error)
    return <- sample
}

def private expected(command : GitProbeCommand const; exit_code : int64;
                     output : string; error : string) : bool {
    if (!empty(error)) return false
    let exit_ok = command.expected_success ? exit_code == 0l : exit_code != 0l
    return exit_ok && find(output, command.expected_text) >= 0
}

def private remove_if_present(path : string) {
    if (empty(path) || !fexist(path)) return
    var error : string
    let _ = remove(path, error)
}

def private collect_exit(samples : array<GitProbeSample> const) : array<int64> {
    var values : array<int64>
    values |> reserve(length(samples))
    for (sample in samples) {
        values |> push(sample.exit_us)
    }
    return <- values
}

def private collect_host(samples : array<HostedSample> const;
                         selector : int) : array<int64> {
    var values : array<int64>
    values |> reserve(length(samples))
    for (sample in samples) {
        if (selector == 0) {
            values |> push(sample.ready_us)
        } elif (selector == 1) {
            values |> push(sample.hosted_exit_us)
        } elif (selector == 2) {
            values |> push(sample.inner_exit_us)
        } else {
            values |> push(sample.outer_total_us)
        }
    }
    return <- values
}

def private summarize(direct, terminal : array<GitProbeSample> const;
                      hosted : array<HostedSample> const) : ComparisonSummary {
    var direct_exit <- collect_exit(direct)
    var terminal_exit <- collect_exit(terminal)
    var host_ready <- collect_host(hosted, 0)
    var hosted_exit <- collect_host(hosted, 1)
    var hosted_inner <- collect_host(hosted, 2)
    var hosted_outer <- collect_host(hosted, 3)
    return ComparisonSummary(
        direct_exit_p50 = percentile_us(direct_exit, 50),
        direct_exit_p95 = percentile_us(direct_exit, 95),
        terminal_exit_p50 = percentile_us(terminal_exit, 50),
        terminal_exit_p95 = percentile_us(terminal_exit, 95),
        host_ready_p50 = percentile_us(host_ready, 50),
        host_ready_p95 = percentile_us(host_ready, 95),
        hosted_exit_p50 = percentile_us(hosted_exit, 50),
        hosted_exit_p95 = percentile_us(hosted_exit, 95),
        hosted_inner_p50 = percentile_us(hosted_inner, 50),
        hosted_outer_p50 = percentile_us(hosted_outer, 50))
}

def private run_command(command : GitProbeCommand const; cfg : BenchmarkOptions const;
                        root : string; var results : FILE const?) : bool {
    var direct : array<GitProbeSample>
    var terminal : array<GitProbeSample>
    var hosted : array<HostedSample>
    direct |> reserve(cfg.rounds)
    terminal |> reserve(cfg.rounds)
    hosted |> reserve(cfg.rounds)
    for (iteration in range(cfg.warmup + cfg.rounds)) {
        let measured = iteration >= cfg.warmup
        let suffix = "{command.name}_{iteration}"
        let direct_log = path_join(root, "logs/{suffix}_direct.raw")
        let terminal_log = path_join(root, "logs/{suffix}_terminal.raw")
        let hosted_log = path_join(root, "logs/{suffix}_hosted.raw")
        let screen_path = path_join(root, "{suffix}.screen")
        var direct_sample <- run_direct_probe(command, direct_log)
        var terminal_sample <- run_terminal_probe(command, terminal_log)
        var hosted_sample <- run_hosted_probe(command, cfg.repository,
            hosted_log, screen_path)
        if (!expected(command, direct_sample.exit_code,
                direct_sample.output_text, direct_sample.error)
                || !expected(command, terminal_sample.exit_code,
                    terminal_sample.output_text, terminal_sample.error)
                || !expected(command, hosted_sample.exit_code,
                    hosted_sample.output_text, hosted_sample.error)) {
            print(("FAIL {command.name} iteration {iteration}: " +
                "direct='{direct_sample.error}' terminal='{terminal_sample.error}' " +
                "hosted='{hosted_sample.error}'\n"))
            return false
        }
        if (measured) {
            direct |> emplace(direct_sample)
            terminal |> emplace(terminal_sample)
            hosted |> emplace(hosted_sample)
            if (results != null) {
                let index = iteration - cfg.warmup
                fwrite(results, ("{command.name}\t{index}\t" +
                    "{direct[length(direct) - 1].exit_us}\t" +
                    "{terminal[length(terminal) - 1].exit_us}\t" +
                    "{hosted[length(hosted) - 1].ready_us}\t" +
                    "{hosted[length(hosted) - 1].inner_exit_us}\t" +
                    "{hosted[length(hosted) - 1].hosted_exit_us}\t" +
                    "{hosted[length(hosted) - 1].outer_total_us}\n"))
            }
        }
        remove_if_present(screen_path)
        if (iteration != cfg.warmup + cfg.rounds - 1) {
            remove_if_present(direct_log)
            remove_if_present(terminal_log)
            remove_if_present(hosted_log)
        }
    }

    let summary = summarize(direct, terminal, hosted)
    let terminal_overhead = summary.terminal_exit_p50 - summary.direct_exit_p50
    let host_overhead = summary.hosted_exit_p50 - summary.terminal_exit_p50
    print(("{command.name}: direct={summary.direct_exit_p50}us " +
        "(p95 {summary.direct_exit_p95}); in-process terminal=" +
        "{summary.terminal_exit_p50}us (p95 {summary.terminal_exit_p95}); " +
        "session host={summary.hosted_exit_p50}us " +
        "(p95 {summary.hosted_exit_p95})\n"))
    print(("{command.name}: terminal delta={terminal_overhead}us; " +
        "fresh-host delta={host_overhead}us; host ready=" +
        "{summary.host_ready_p50}us (p95 {summary.host_ready_p95}); " +
        "inner terminal={summary.hosted_inner_p50}us; " +
        "worker total including drain={summary.hosted_outer_p50}us\n"))
    return true
}

[export]
def main() : int {
    let cfg = parse_options()
    let temp = create_temp_directory_result("dasherd_session_host_chunk1_")
    if (!(temp is value)) {
        print("could not create benchmark directory: {temp as error}\n")
        return 2
    }
    let root = temp as value
    var mkdir_error : string
    if (!mkdir(path_join(root, "logs"), mkdir_error)) {
        print("could not create log directory: {mkdir_error}\n")
        return 2
    }
    let result_path = path_join(root, "samples.tsv")
    var results = fopen(result_path, "wb")
    if (results == null) {
        print("could not create result file: {result_path}\n")
        return 2
    }
    fwrite(results, ("command\titeration\tdirect_exit_us\tterminal_exit_us\t" +
        "host_ready_us\thost_inner_exit_us\thosted_exit_us\thost_total_us\n"))
    print("Chunk 1 fresh process-per-session overhead\n")
    print("repository: {cfg.repository}\n")
    print("warmup={cfg.warmup}, rounds={cfg.rounds}\n")
    var ok = true
    for (command in make_git_probe_commands(cfg.repository)) {
        if (!run_command(command, cfg, root, results)) {
            ok = false
            break
        }
    }
    fflush(results)
    fclose(results)
    if (cfg.discard && ok) {
        let removed = rmdir_rec_result(root)
        if (!(removed is value)) {
            print("could not discard benchmark directory: {removed as error}\n")
            return 2
        }
        print("benchmark artifacts discarded\n")
    } else {
        print("samples: {result_path}\n")
        print("representative logs: {path_join(root, "logs")}\n")
    }
    return ok ? 0 : 1
}
