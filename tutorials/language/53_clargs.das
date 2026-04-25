// Tutorial 53: Command-Line Argument Parsing (clargs)
//
// This tutorial covers:
//   - Declaring a CLI args struct with [CommandLineArgs]
//   - Supported field types: string, int, float, bool, enum, array<string>
//   - Bool flag shorthand (--flag sets true; --flag=false sets false)
//   - Required flags with @clarg_required
//   - Custom flag names with @clarg_name
//   - Short flags with @clarg_short = "v"
//   - Doc strings with @clarg_doc
//   - Skipping fields with @clarg_skip
//   - Error handling (parse_args returns "" on success, error message on failure)
//   - Introspection with get_command_info
//   - Help rendering with print_help / format_help
//   - Reading process arguments: get_cli_arguments() (after "--") vs
//     get_program_args() (standalone tools, no separator)
//
// Run: daslang.exe tutorials/language/53_clargs.das

options gen2

require daslib/clargs

// ============================================================
// Section 1: Defining a CLI args struct
// ============================================================
// Annotate any struct with [CommandLineArgs] to auto-generate:
//   parse_args(var dst; args : array<string>) : string
//   parse_args(var dst) : string          (reads process CLI args)
//   get_command_info(type<T>) : CommandInfo
//
// Field names map to flag names with underscores replaced by dashes:
//   output_file -> --output-file

[CommandLineArgs]
struct Config {
    name    : string  // --name
    count   : int     // --count
    verbose : bool    // --verbose
    timeout : float   // --timeout
}

def test_basic_parsing() {
    print("=== Basic flag parsing ===\n")
    var cfg = Config()
    let err = parse_args(cfg, ["--name", "Alice", "--count=42", "--verbose", "--timeout=1.5"])
    if (err != "") {
        print("  error: {err}\n")
        return
    }
    print("  name    = {cfg.name}\n")
    print("  count   = {cfg.count}\n")
    print("  verbose = {cfg.verbose}\n")
    print("  timeout = {cfg.timeout}\n")
    // output:
    //   name    = Alice
    //   count   = 42
    //   verbose = true
    //   timeout = 1.5
}

// ============================================================
// Section 2: Bool flags
// ============================================================
// --verbose           sets verbose = true
// --verbose=true      same as above
// --verbose=false     sets verbose = false

def test_bool_flags() {
    print("\n=== Bool flags ===\n")
    var cfg = Config()

    parse_args(cfg, ["--verbose"])
    print("  bare --verbose: {cfg.verbose}\n")

    parse_args(cfg, ["--verbose=false"])
    print("  --verbose=false: {cfg.verbose}\n")
    // output:
    //   bare --verbose: true
    //   --verbose=false: false
}

// ============================================================
// Section 3: Enum flags
// ============================================================
// Enum fields accept the entry name as a string.
// Passing an unknown value returns an error.

enum LogLevel {
    Debug
    Info
    Warning
    Error
}

[CommandLineArgs]
struct LogConfig {
    level : LogLevel  // --level  (accepts "Debug", "Info", "Warning", "Error")
}

def test_enum_flags() {
    print("\n=== Enum flags ===\n")
    var cfg = LogConfig()
    let err = parse_args(cfg, ["--level", "Warning"])
    if (err != "") {
        print("  error: {err}\n")
        return
    }
    print("  level is Warning: {cfg.level == LogLevel.Warning}\n")

    // Invalid enum value returns an error
    var bad = LogConfig()
    let err2 = parse_args(bad, ["--level", "Verbose"])
    print("  unknown enum error: '{err2}'\n")
    // output:
    //   level is Warning: true
    //   unknown enum error: '--level: invalid enum value 'Verbose''
}

// ============================================================
// Section 4: Array flags
// ============================================================
// array<string> fields accept the flag multiple times.
// --tag=a --tag b --tag=c  results in  ["a", "b", "c"]

[CommandLineArgs]
struct BuildConfig {
    tags : array<string>
}

def test_array_flags() {
    print("\n=== Array flags ===\n")
    var cfg = BuildConfig()
    let err = parse_args(cfg, ["--tags=debug", "--tags", "release", "--tags=profile"])
    if (err != "") {
        print("  error: {err}\n")
        return
    }
    for (tag in cfg.tags) {
        print("  tag: {tag}\n")
    }
    // output:
    //   tag: debug
    //   tag: release
    //   tag: profile
}

// ============================================================
// Section 5: Required flags
// ============================================================
// @clarg_required means parse_args returns an error if the
// flag is absent from the argument list.

[CommandLineArgs]
struct DeployConfig {
    host : string

    @clarg_required
    token : string  // --token must always be provided
}

def test_required_flags() {
    print("\n=== Required flags ===\n")
    var cfg = DeployConfig()

    // Missing --token
    let err1 = parse_args(cfg, ["--host=prod.example.com"])
    print("  missing required: {err1 != "" ? "error (expected)" : "ok (unexpected)"}\n")

    // Both present
    let err2 = parse_args(cfg, ["--host=prod.example.com", "--token=secret123"])
    print("  both present: {err2 == "" ? "ok" : err2}\n")
    print("  token = {cfg.token}\n")
    // output:
    //   missing required: error (expected)
    //   both present: ok
    //   token = secret123
}

// ============================================================
// Section 6: Field-level attributes
// ============================================================
// @clarg_name = "flag"  overrides the auto-generated --flag-name
// @clarg_doc = "text"   description used by introspection / help
// @clarg_skip           excludes the field from CLI parsing

[CommandLineArgs]
struct AppConfig {
    @clarg_name = "output-dir"
    @clarg_doc = "Directory to write output files"
    out_path : string  // flag is --output-dir, not --out-path

    @clarg_doc = "Number of parallel workers (default: 1)"
    workers : int

    @clarg_skip
    internal_id : int  // not a CLI flag; set in code only
}

def test_field_attributes() {
    print("\n=== Field attributes ===\n")
    var cfg = AppConfig()
    cfg.internal_id = 99  // direct assignment, not via CLI
    let err = parse_args(cfg, ["--output-dir=/tmp/out", "--workers=4"])
    if (err != "") {
        print("  error: {err}\n")
        return
    }
    print("  out_path    = {cfg.out_path}\n")
    print("  workers     = {cfg.workers}\n")
    print("  internal_id = {cfg.internal_id}\n")
    // output:
    //   out_path    = /tmp/out
    //   workers     = 4
    //   internal_id = 99
}

// ============================================================
// Section 7: Error handling
// ============================================================
// parse_args returns "" on success.
// On failure it returns a descriptive error string, e.g.:
//   "--count: invalid int value 'abc'"
//   "--token: missing required flag"
//   "--level: invalid enum value 'Verbose'"

[CommandLineArgs]
struct TypedConfig {
    count : int
}

def test_error_handling() {
    print("\n=== Error handling ===\n")
    var cfg = TypedConfig()
    let err = parse_args(cfg, ["--count", "not_a_number"])
    print("  error: '{err}'\n")
    // output:
    //   error: '--count: invalid int value 'not_a_number''
}

// ============================================================
// Section 8: Short flags
// ============================================================
// @clarg_short = "X" attaches a single-character short flag,
// callable as -X / -X value / -X=value.  All field types support
// short flags; declaring two fields with the same short character
// is a compile-time error.

[CommandLineArgs]
struct ServerConfig {
    @clarg_short = "p"
    @clarg_doc = "listen port"
    port : int

    @clarg_short = "v"
    @clarg_doc = "verbose logging"
    verbose : bool

    @clarg_short = "t"
    @clarg_doc = "tag (repeated)"
    tags : array<string>
}

def test_short_flags() {
    print("\n=== Short flags ===\n")

    // Long form, short form, and mixed form all parse identically.
    var a = ServerConfig()
    parse_args(a, ["--port", "8080", "--verbose", "--tags=alpha", "--tags=beta"])

    var b = ServerConfig()
    parse_args(b, ["-p", "8080", "-v", "-t=alpha", "-t=beta"])

    var c = ServerConfig()
    parse_args(c, ["-p=8080", "--verbose", "-t", "alpha", "--tags=beta"])

    print("  long-only port    = {a.port}, verbose = {a.verbose}, tags = {a.tags}\n")
    print("  short-only port   = {b.port}, verbose = {b.verbose}, tags = {b.tags}\n")
    print("  mixed port        = {c.port}, verbose = {c.verbose}, tags = {c.tags}\n")
    // output:
    //   long-only port    = 8080, verbose = true, tags = [alpha, beta]
    //   short-only port   = 8080, verbose = true, tags = [alpha, beta]
    //   mixed port        = 8080, verbose = true, tags = [alpha, beta]
}

// ============================================================
// Section 9: Introspection with get_command_info
// ============================================================
// get_command_info(type<T>) returns a CommandInfo value
// describing every parsed flag: name, short name, type, doc string,
// required status, array status, and valid enum choices.

def test_introspection() {
    print("\n=== Introspection ===\n")
    let info <- get_command_info(type<ServerConfig>)
    print("  ServerConfig exposes {length(info.args)} flags:\n")
    for (arg in info.args) {
        print("    {arg.short_flag_name}, {arg.flag_name}  ({arg.value_type})  {arg.doc_string}\n")
    }
    // output:
    //   ServerConfig exposes 3 flags:
    //     -p, --port  (tInt)  listen port
    //     -v, --verbose  (tBool)  verbose logging
    //     -t, --tags  (tString)  tag (repeated)
}

// ============================================================
// Section 10: Help rendering with print_help / format_help
// ============================================================
// The library renders a Usage / Flags block from CommandInfo.
//   - print_help writes to stdout.
//   - format_help returns the same text as a string (testable,
//     redirectable into a logger or buffer).
//
// The macro intentionally does NOT auto-handle --help.  Add a
// help : bool field with @clarg_short = "h" and check it after
// parse_args; you control the exit policy.

[CommandLineArgs]
struct DemoConfig {
    @clarg_short = "n"
    @clarg_doc = "user's display name"
    name : string

    @clarg_doc = "iteration count"
    count : int

    @clarg_short = "v"
    @clarg_doc = "verbose logging"
    verbose : bool

    @clarg_short = "h"
    @clarg_doc = "show this help and exit"
    help : bool
}

def test_help_rendering() {
    print("\n=== Help rendering ===\n")
    let info <- get_command_info(type<DemoConfig>)
    print(format_help(info, "demo"))
    // output:
    //   Usage: demo [flags]
    //
    //   Flags:
    //     -n, --name=STRING    user's display name
    //         --count=INT      iteration count
    //     -v, --verbose        verbose logging
    //     -h, --help           show this help and exit
}

// ============================================================
// Section 11: Reading process arguments
// ============================================================
// Two helpers feed argv into parse_args, depending on how the
// program is invoked:
//
//   get_cli_arguments()    — script-style, returns the slice AFTER "--"
//                            in argv.  Used when daslang itself is the
//                            host:  daslang script.das -- --name Alice
//                            parse_args(cfg) (the no-argument overload)
//                            calls this internally.
//
//   get_program_args()     — standalone-tool style, returns argv[1..]
//                            (everything after the program name).  Use
//                            this for AOT'd binaries that own the full
//                            argv themselves (no "--" separator).
//
// Both helpers also accept an explicit argv parameter, which makes the
// "--" splitting and argv0 skipping testable in unit tests.

def test_process_args() {
    print("\n=== Process CLI arguments ===\n")

    // Script-style: arguments after "--".
    let script_args = get_cli_arguments()
    print("  get_cli_arguments() returned {length(script_args)} entries\n")
    // Run as:
    //   daslang.exe tutorials/language/53_clargs.das -- --name foo
    // to see: get_cli_arguments() returned 2 entries

    // Standalone-tool style: full argv minus argv[0].
    let prog_args = get_program_args()
    print("  get_program_args() returned {length(prog_args)} entries\n")

    // The split helpers are testable directly without poking the
    // process state:
    let scripted <- get_cli_arguments(["host", "script.das", "--", "--foo", "bar"])
    print("  get_cli_arguments(synthetic) -> {length(scripted)} entries (foo, bar)\n")

    let standalone <- get_program_args(["fmt.exe", "--write", "file.das"])
    print("  get_program_args(synthetic) -> {length(standalone)} entries (--write, file.das)\n")
}

// ============================================================
// Main
// ============================================================

[export]
def main() {
    test_basic_parsing()
    test_bool_flags()
    test_enum_flags()
    test_array_flags()
    test_required_flags()
    test_field_attributes()
    test_error_handling()
    test_short_flags()
    test_introspection()
    test_help_rendering()
    test_process_args()
}
