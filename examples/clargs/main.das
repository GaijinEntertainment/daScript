options gen2

require daslib/clargs

//! Example: annotate a struct with [CommandLineArgs] and call parse_args to
//! populate it from the process command line.  Demonstrates required flags,
//! doc strings, enums, and array flags.
//!
//! Run:
//!   daslang examples/clargs/main.das -- --name Alice --repeat 3 --verbose
//!   daslang examples/clargs/main.das -- --name Bob --repeat 2 --level Warning --tag a --tag b

enum LogLevel {
    Debug
    Info
    Warning
    Error
}

[CommandLineArgs]
struct Config {
    @clarg_required
    @clarg_doc = "Greeting target"
    name : string

    @clarg_doc = "Number of greetings (default: 1)"
    repeat : int

    @clarg_doc = "Verbose output"
    verbose : bool

    @clarg_doc = "Log level filter"
    level : LogLevel

    @clarg_doc = "Optional tags"
    tag : array<string>
}

[export]
def main() {
    var cfg = Config()
    let err = parse_args(cfg)
    if (err != "") {
        print("error: {err}\n")
        print("usage: daslang examples/clargs/main.das -- --name <name> [--repeat <n>] [--verbose] [--level <Debug|Info|Warning|Error>] [--tag <t>]...\n")
        return
    }

    if (cfg.verbose) {
        print("[verbose] level = {cfg.level}, tags = {length(cfg.tag)}\n")
    }

    let n = cfg.repeat > 0 ? cfg.repeat : 1
    for (i in range(n)) {
        print("hello, {cfg.name}!\n")
    }

    for (t in cfg.tag) {
        print("  tag: {t}\n")
    }
}
