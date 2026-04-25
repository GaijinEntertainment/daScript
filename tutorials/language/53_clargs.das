// Tutorial 53: Command-Line Argument Parsing (clargs)
//
// This tutorial covers:
//   - Declaring a CLI args struct with [CommandLineArgs]
//   - Supported field types: string, int, float, bool, enum, array<string>
//   - Bool flag shorthand (--flag sets true; --flag=false sets false)
//   - Required flags with @clarg_required
//   - Custom flag names with @clarg_name
//   - Doc strings with @clarg_doc
//   - Skipping fields with @clarg_skip
//   - Error handling (parse_args returns "" on success, error message on failure)
//   - Introspection with get_command_info for help generation
//   - Reading process arguments with get_cli_arguments()
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
// Section 8: Introspection with get_command_info
// ============================================================
// get_command_info(type<T>) returns a CommandInfo value
// describing every parsed flag: name, type, doc string,
// required status, and valid enum choices.
// Use it to implement --help output.

def print_help(info : CommandInfo) {
    for (arg in info.args) {
        let req = arg.is_required ? " (required)" : ""
        let doc = !empty(arg.doc_string) ? "  — {arg.doc_string}" : ""
        print("  {arg.flag_name}{req}{doc}\n")
        if (!empty(arg.enum_values)) {
            for (v in arg.enum_values) {
                print("      {v}\n")
            }
        }
    }
}

def test_introspection() {
    print("\n=== Introspection ===\n")
    let info <- get_command_info(type<AppConfig>)
    print("  AppConfig exposes {length(info.args)} flags:\n")
    print_help(info)
    // output:
    //   AppConfig exposes 2 flags:
    //     --output-dir  — Directory to write output files
    //     --workers  — Number of parallel workers (default: 1)
}

// ============================================================
// Section 9: Reading process arguments
// ============================================================
// In a real program call parse_args(cfg) (no second argument)
// to parse from the process command line.
//
// Arguments must come after "--" on the command line:
//
//   daslang.exe my_script.das -- --name Alice --count 5
//
// get_cli_arguments() returns only the slice after "--".
// Without "--" it returns an empty array.

def test_process_args() {
    print("\n=== Process CLI arguments ===\n")
    let args = get_cli_arguments()
    print("  CLI args passed to this tutorial: {length(args)}\n")
    // Run as:
    //   daslang.exe tutorials/language/53_clargs.das -- --name foo
    // to see: CLI args passed to this tutorial: 2
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
    test_introspection()
    test_process_args()
}
