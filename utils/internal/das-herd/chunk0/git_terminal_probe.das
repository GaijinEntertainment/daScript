options gen2
options indenting = 4

module git_terminal_probe shared

require terminal/terminal public
require daslib/fio public
require daslib/strings_boost
require strings public

enum public GitProbeLane {
    direct_capture
    direct_logged
    terminal_logged
}

struct public GitProbeCommand {
    name : string
    argv : array<string>
    command_line : string
    expected_text : string
    expected_success : bool = true
}

struct public GitProbeSample {
    command : string
    lane : GitProbeLane
    exit_code : int64 = -1l
    output_bytes : int
    launch_us : int64 = -1l
    first_output_us : int64 = -1l
    exit_us : int64 = -1l
    drained_us : int64 = -1l
    poll_feed_us : int64
    log_write_us : int64
    total_us : int64
    log_path : string
    output_text : string
    error : string
}

def private elapsed_us(started : int64) : int64 {
    return get_time_nsec(started) / 1000l
}

def private quote_terminal_arg(arg : string) : string {
    if (empty(arg)) return "\"\""
    if (find(arg, " ") < 0 && find(arg, "\t") < 0 && find(arg, "\"") < 0) {
        return arg
    }
    // Chunk 0 targets Windows ConPTY. Windows paths cannot contain a quote;
    // reject that uncommon case instead of pretending this is a general shell
    // command-line encoder.
    if (find(arg, "\"") >= 0) return ""
    return "\"{arg}\""
}

def public make_terminal_command_line(argv : array<string> const) : string {
    var parts : array<string>
    parts |> reserve(length(argv))
    for (arg in argv) {
        let quoted = quote_terminal_arg(arg)
        if (empty(quoted)) return ""
        parts |> push(quoted)
    }
    return join(parts, " ")
}

def private make_command(name : string; argv : array<string>;
                         expected_text : string;
                         expected_success : bool = true) : GitProbeCommand {
    var owned_argv : array<string>
    owned_argv := argv
    return GitProbeCommand(
        name = name,
        command_line = make_terminal_command_line(owned_argv),
        argv <- owned_argv,
        expected_text = expected_text,
        expected_success = expected_success)
}

def public make_git_probe_commands(repository : string) : array<GitProbeCommand> {
    return <- [
        <- make_command("version", ["git", "--version"], "git version"),
        <- make_command("status", [
        "git", "-C", repository, "-c", "color.ui=always",
        "status", "--short", "--branch"], "##"),
        <- make_command("missing-ref", [
        "git", "-C", repository, "-c", "color.ui=always",
        "rev-parse", "refs/heads/__dasherd_chunk0_missing_ref__"],
        "fatal: ambiguous argument", false)]
}

def public run_direct_probe(command : GitProbeCommand const;
                            log_path : string = "") : GitProbeSample {
    var sample = GitProbeSample(
        command = command.name,
        lane = empty(log_path) ? GitProbeLane.direct_capture : GitProbeLane.direct_logged,
        log_path = log_path)
    var first_byte : array<uint8>
    first_byte |> resize(1)
    var first_count = 0
    var remainder : string
    let started = ref_time_ticks()
    let rc = unsafe(popen_argv(command.argv, 0.0, $(f) {
        sample.launch_us = elapsed_us(started)
        if (f == null) {
            sample.error = "popen_argv returned a null output stream"
            return
        }
        first_count = fread(f, first_byte)
        if (first_count > 0) {
            sample.first_output_us = elapsed_us(started)
        }
        remainder = fread(f)
    }))
    sample.exit_us = elapsed_us(started)
    sample.drained_us = sample.exit_us
    sample.exit_code = int64(rc)
    sample.output_bytes = first_count + length(remainder)
    sample.output_text = (first_count > 0 ? to_char(int(first_byte[0])) : "") + remainder
    if (!empty(log_path)) {
        let log_started = ref_time_ticks()
        var log_file = fopen(log_path, "wb")
        if (log_file == null) {
            sample.error = "could not open log: {log_path}"
        } else {
            if (first_count > 0) {
                let _ = fwrite(log_file, first_byte)
            }
            if (!empty(remainder)) {
                fwrite(log_file, remainder)
            }
            fflush(log_file)
            fclose(log_file)
        }
        sample.log_write_us = elapsed_us(log_started)
    }
    sample.total_us = elapsed_us(started)
    return <- sample
}

def public run_terminal_probe(command : GitProbeCommand const;
                              log_path : string;
                              drain_quiet_us : int64 = 250000l;
                              timeout_us : int64 = 10000000l) : GitProbeSample {
    var sample = GitProbeSample(
        command = command.name,
        lane = GitProbeLane.terminal_logged,
        log_path = log_path)
    var log_file = fopen(log_path, "wb")
    if (log_file == null) {
        sample.error = "could not open log: {log_path}"
        return <- sample
    }

    let started = ref_time_ticks()
    var inscope terminal <- terminal_launch(command.command_line, "", 100, 30)
    sample.launch_us = elapsed_us(started)
    if (terminal_process_id(terminal) == 0) {
        sample.error = terminal_transport_error(terminal)
        sample.total_us = elapsed_us(started)
        fclose(log_file)
        return <- sample
    }

    var exit_seen = false
    var last_data_us = 0l
    while (true) {
        let poll_started = ref_time_ticks()
        let poll_status = terminal_poll(terminal)
        sample.poll_feed_us += elapsed_us(poll_started)
        let now_us = elapsed_us(started)
        if (poll_status == TerminalPollStatus.data) {
            if (sample.first_output_us < 0l) {
                sample.first_output_us = now_us
            }
            sample.output_bytes += length(terminal.read_buffer)
            last_data_us = now_us
            let log_started = ref_time_ticks()
            let _ = fwrite(log_file, terminal.read_buffer)
            sample.log_write_us += elapsed_us(log_started)
        } elif (poll_status == TerminalPollStatus.error) {
            sample.error = terminal_transport_error(terminal)
            break
        }

        if (!exit_seen && terminal_process_exited(terminal)) {
            exit_seen = true
            sample.exit_us = now_us
            sample.exit_code = terminal_exit_code(terminal)
            if (last_data_us == 0l) {
                last_data_us = now_us
            }
        }

        if (poll_status == TerminalPollStatus.closed) {
            sample.drained_us = now_us
            break
        }
        // ConPTY keeps its output pipe open after the root process exits. A
        // bounded quiet interval after the last observed bytes is the Chunk 0
        // drain policy. ConHost can flush noticeably after root exit when the
        // UI polls once per frame; exit_us remains the apples-to-apples primary
        // metric and drained_us reports this conservative logging policy.
        if (exit_seen && now_us - last_data_us >= drain_quiet_us) {
            sample.drained_us = now_us
            break
        }
        if (now_us >= timeout_us) {
            sample.error = "terminal probe timed out"
            if (!exit_seen) {
                let _ = terminal_terminate(terminal)
            }
            break
        }
        sleep(0u)
    }

    finish_probe_sample(sample, terminal, log_file, started)
    return <- sample
}

def private finish_probe_sample(var sample : GitProbeSample; var terminal : Terminal; log_file : file; started : int64) {
    fflush(log_file)
    fclose(log_file)
    sample.total_us = elapsed_us(started)
    if (sample.drained_us < 0l) {
        sample.drained_us = sample.total_us
    }
    if (sample.exit_code < 0l && terminal_process_exited(terminal)) {
        sample.exit_code = terminal_exit_code(terminal)
        sample.exit_us = elapsed_us(started)
    }
    var viewport = TerminalViewportSnapshot()
    terminal_viewport_snapshot(terminal, 0, viewport)
    sample.output_text = viewport.screen_text
}
