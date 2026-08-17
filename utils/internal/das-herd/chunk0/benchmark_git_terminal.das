options gen2
options indenting = 4

require git_terminal_probe
require daslib/fio
require strings
require math

struct BenchmarkOptions {
    repository : string
    warmup : int = 5
    rounds : int = 30
}

struct LaneSummary {
    ok : bool
    total_p50_us : int64 = -1l
    total_p95_us : int64 = -1l
    exit_p50_us : int64 = -1l
    exit_p95_us : int64 = -1l
    first_p50_us : int64 = -1l
    first_p95_us : int64 = -1l
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
        }
    }
    return cfg
}

def private lane_name(lane : GitProbeLane) : string {
    if (lane == GitProbeLane.direct_capture) return "direct"
    if (lane == GitProbeLane.direct_logged) return "direct+log"
    return "terminal+log"
}

def private percentile_us(var values : array<int64>; numerator : int) : int64 {
    if (empty(values)) return -1l
    sort(values)
    let index = clamp((length(values) * numerator + 99) / 100 - 1,
        0, length(values) - 1)
    return values[index]
}

def private summarize(samples : array<GitProbeSample> const) : LaneSummary {
    var total : array<int64>
    var exit : array<int64>
    var first : array<int64>
    total |> reserve(length(samples))
    exit |> reserve(length(samples))
    first |> reserve(length(samples))
    for (sample in samples) {
        total |> push(sample.total_us)
        exit |> push(sample.exit_us)
        if (sample.first_output_us >= 0l) first |> push(sample.first_output_us)
    }
    return LaneSummary(
        ok = true,
        total_p50_us = percentile_us(total, 50),
        total_p95_us = percentile_us(total, 95),
        exit_p50_us = percentile_us(exit, 50),
        exit_p95_us = percentile_us(exit, 95),
        first_p50_us = percentile_us(first, 50),
        first_p95_us = percentile_us(first, 95))
}

def private summary_text(summary : LaneSummary const) : string {
    return ("total p50={summary.total_p50_us}us p95={summary.total_p95_us}us; " +
        "exit p50={summary.exit_p50_us}us p95={summary.exit_p95_us}us; " +
        "first p50={summary.first_p50_us}us p95={summary.first_p95_us}us")
}

def private sample_tsv(sample : GitProbeSample const; iteration : int) : string {
    return ("{sample.command}\t{lane_name(sample.lane)}\t{iteration}\t" +
        "{sample.exit_code}\t{sample.output_bytes}\t{sample.launch_us}\t" +
        "{sample.first_output_us}\t{sample.exit_us}\t{sample.drained_us}\t" +
        "{sample.poll_feed_us}\t{sample.log_write_us}\t{sample.total_us}\t" +
        "{sample.error}\n")
}

def private sample_ok(command : GitProbeCommand const;
                      sample : GitProbeSample const) : bool {
    if (!empty(sample.error)) return false
    let exit_ok = command.expected_success ? sample.exit_code == 0l : sample.exit_code != 0l
    return exit_ok && find(sample.output_text, command.expected_text) >= 0
}

def private run_lane(command : GitProbeCommand const; lane : GitProbeLane;
                     warmup, rounds : int; root : string;
                     var result_file : FILE const?) : LaneSummary {
    var samples : array<GitProbeSample>
    samples |> reserve(rounds)
    let prefix = "{command.name}_{lane_name(lane)}"
    for (iteration in range(warmup + rounds)) {
        let measured = iteration >= warmup
        let log_path = (lane == GitProbeLane.direct_capture ? "" :
            path_join(root, "logs/{prefix}_{iteration}.raw"))
        var sample <- (lane == GitProbeLane.terminal_logged ?
            run_terminal_probe(command, log_path) :
            run_direct_probe(command, log_path))
        if (!sample_ok(command, sample)) {
            print(("FAIL {command.name}/{lane_name(lane)}: rc={sample.exit_code} " +
                "error={sample.error}\n"))
            return LaneSummary()
        }
        if (measured) {
            if (result_file != null) {
                fwrite(result_file, sample_tsv(sample, iteration - warmup))
            }
            samples |> emplace(sample)
        }
        // Preserve one representative raw log per logged lane; remove warmup
        // and intermediate logs after their measured writes have completed.
        if (!empty(log_path) && iteration != warmup + rounds - 1) {
            var remove_error : string
            let _ = remove(log_path, remove_error)
        }
    }
    let summary = summarize(samples)
    print(("{command.name}/{lane_name(lane)}: {summary_text(summary)}; " +
        "bytes={samples[length(samples) - 1].output_bytes}; " +
        "rc={samples[length(samples) - 1].exit_code}\n"))
    return summary
}

def private overhead_text(base, measured : int64) : string {
    let delta = measured - base
    let ratio = base > 0l ? double(measured) / double(base) : 0.0lf
    return "{delta}us ({fmt(":.2f", ratio)}x)"
}

[export]
def main() : int {
    let cfg = parse_options()
    if (!fexist(cfg.repository)) {
        print("repository does not exist: {cfg.repository}\n")
        return 2
    }
    let temp = create_temp_directory_result("dasherd_git_terminal_chunk0_")
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
    var result_file = fopen(result_path, "wb")
    if (result_file == null) {
        print("could not create results file: {result_path}\n")
        return 2
    }
    fwrite(result_file,
        ("command\tlane\titeration\texit_code\toutput_bytes\tlaunch_us\t" +
        "first_output_us\texit_us\tdrained_us\tpoll_feed_us\tlog_write_us\t" +
        "total_us\terror\n"))

    print("Chunk 0 local Git terminal overhead\n")
    print("repository: {cfg.repository}\n")
    print("warmup={cfg.warmup}, rounds={cfg.rounds}\n")
    print("drain policy: 250000us quiet after root-process exit\n")
    var ok = true
    for (command in make_git_probe_commands(cfg.repository)) {
        var summaries : array<LaneSummary>
        summaries |> reserve(3)
        for (lane in [GitProbeLane.direct_capture,
                      GitProbeLane.direct_logged,
                      GitProbeLane.terminal_logged]) {
            let summary = run_lane(command, lane, cfg.warmup, cfg.rounds,
                root, result_file)
            summaries |> push(summary)
            if (!summary.ok) {
                ok = false
                break
            }
        }
        if (ok && length(summaries) == 3) {
            print(("{command.name}/overhead at process exit: log=" +
                overhead_text(summaries[0].exit_p50_us, summaries[1].exit_p50_us) +
                "; terminal+log=" +
                overhead_text(summaries[0].exit_p50_us, summaries[2].exit_p50_us) +
                "\n"))
        }
        break if (!ok)
    }
    fflush(result_file)
    fclose(result_file)
    print("samples: {result_path}\n")
    print("representative logs: {path_join(root, "logs")}\n")
    return ok ? 0 : 1
}
