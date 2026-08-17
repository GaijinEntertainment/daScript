options gen2
options indenting = 4

require ../chunk0/git_terminal_probe.das
require daslib/fio
require strings

struct WorkerOptions {
    repository : string
    probe : string = "status"
    log_path : string
    screen_path : string
}

def private option_value(prefix : string; fallback : string = "") : string {
    for (arg in get_command_line_arguments()) {
        if (starts_with(arg, prefix)) return slice(arg, length(prefix))
    }
    return fallback
}

def private parse_options() : WorkerOptions {
    return WorkerOptions(
        repository = option_value("--repo=", get_das_root()),
        probe = option_value("--probe=", "status"),
        log_path = option_value("--log="),
        screen_path = option_value("--screen="))
}

def private command_index(name : string) : int {
    if (name == "version") return 0
    if (name == "missing-ref") return 2
    return 1
}

[export]
def main() : int {
    let cfg = parse_options()
    if (empty(cfg.log_path) || empty(cfg.screen_path)) {
        fprint(fstderr(), "session host requires --log and --screen\n")
        return 2
    }

    // This flushed byte is the minimal IPC-ready acknowledgement. The parent
    // timestamps its arrival, so process creation, daScript compilation, and
    // worker initialization are measured rather than inferred.
    fprint(fstdout(), "DASHERD_HOST_READY\n")
    fflush(fstdout())

    let commands <- make_git_probe_commands(cfg.repository)
    let sample <- run_terminal_probe(commands[command_index(cfg.probe)],
        cfg.log_path)
    if (!empty(sample.error)) {
        fprint(fstderr(), "session host probe failed: {sample.error}\n")
        return 1
    }

    var screen = fopen(cfg.screen_path, "wb")
    if (screen == null) {
        fprint(fstderr(), "could not create screen snapshot: {cfg.screen_path}\n")
        return 1
    }
    fwrite(screen, sample.output_text)
    fflush(screen)
    fclose(screen)

    fprint(fstdout(),
        ("DASHERD_HOST_RESULT\t{sample.exit_code}\t{sample.launch_us}\t" +
        "{sample.first_output_us}\t{sample.exit_us}\t{sample.drained_us}\t" +
        "{sample.poll_feed_us}\t{sample.log_write_us}\t{sample.total_us}\t" +
        "{sample.output_bytes}\n"))
    fflush(fstdout())
    return 0
}
