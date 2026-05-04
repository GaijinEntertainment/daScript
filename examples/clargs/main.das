options gen2

require daslib/clargs

//! Example: annotate a struct with [CommandLineArgs] and call parse_args to
//! populate it from the process command line.  Demonstrates required flags,
//! short flags, doc strings, enums, array flags, and the print_help helper.
//!
//! Run:
//!   daslang examples/clargs/main.das -- -?
//!   daslang examples/clargs/main.das -- --name Alice -r 3 -v
//!   daslang examples/clargs/main.das -- -n Bob -r 2 -l Warning -t a -t b
//!
//! Note: when run under daslang.exe the host intercepts `-h` / `--help`
//! itself before forwarding script args, so this example wires the
//! help flag to `-?` instead.  AOT-compiled standalone binaries that
//! own argv directly (and call parse_args(cfg, get_program_args()))
//! can use `-h` / `--help` as the natural convention.

enum LogLevel {
    Debug
    Info
    Warning
    Error
}

[CommandLineArgs]
struct Config {
    @clarg_required
    @clarg_short = "n"
    @clarg_doc = "Greeting target"
    name : string

    @clarg_short = "r"
    @clarg_doc = "Number of greetings (default: 1)"
    repeat : int

    @clarg_short = "v"
    @clarg_doc = "Verbose output"
    verbose : bool

    @clarg_short = "l"
    @clarg_doc = "Log level filter"
    level : LogLevel

    @clarg_short = "t"
    @clarg_doc = "Optional tags"
    tag : array<string>

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

[export]
def main() {
    var r <- parse_args(type<Config>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Config>), "clargs-example")
        return
    }
    var cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Config>), "clargs-example")
        return
    }

    if (cfg.verbose) {
        print("[verbose] level = {cfg.level}, tags = {length(cfg.tag)}\n")
    }

    let n = cfg.repeat > 0 ? cfg.repeat : 1
    for (_i in range(n)) {
        print("hello, {cfg.name}!\n")
    }

    for (t in cfg.tag) {
        print("  tag: {t}\n")
    }
}
