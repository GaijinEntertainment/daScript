options gen2

require dastest/testing_boost
require daslib/clargs

enum Color {
    Red
    Green
    Blue
}

[CommandLineArgs]
struct ConfigWithRequired {
    foo : string

    @clarg_required
    bar_required : string
}

[CommandLineArgs]
struct Config {
    name : string
    count : int
    verbose : bool
    color : Color
    timeout : float
    tags : array<string> = ["default"]
}

[CommandLineArgs]
struct ShortFlagsConfig {
    @clarg_short = "n"
    @clarg_doc = "user name"
    name : string

    @clarg_short = "c"
    @clarg_doc = "iteration count"
    count : int

    @clarg_short = "v"
    @clarg_doc = "verbose output"
    verbose : bool

    @clarg_short = "t"
    @clarg_doc = "tag to apply"
    tags : array<string>

    @clarg_short = "p"
    @clarg_doc = "palette"
    color : Color

    @clarg_short = "r"
    @clarg_doc = "request timeout in seconds"
    timeout : float

    @clarg_short = "h"
    @clarg_doc = "show this help and exit"
    help : bool
}

[CommandLineArgs]
struct ShortFlagsRequiredConfig {
    @clarg_short = "f"
    @clarg_required
    file : string
}

[test]
def test_string_flag(t : T?) {
    var cfg = Config()
    let a <- ["--name", "Alice"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.name, "Alice")
}

[test]
def test_string_flag_equals(t : T?) {
    var cfg = Config()
    let a <- ["--name=Bob"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.name, "Bob")
}

[test]
def test_string_array_flag(t : T?) {
    var cfg = Config()
    let a <- ["--tags=aaa", "--tags", "bbb", "--tags=ccc"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.tags, ["aaa", "bbb", "ccc"])
}

[test]
def test_int_flag(t : T?) {
    var cfg = Config()
    let a <- ["--count", "42"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.count, 42)
}

[test]
def test_float_flag(t : T?) {
    var cfg = Config()
    let a <- ["--timeout", "45.1"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.timeout, 45.1)
}

[test]
def test_bool_flag(t : T?) {
    var cfg = Config()
    let a <- ["--verbose"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> success(cfg.verbose, "verbose is true")
}

[test]
def test_bool_flag_explicit_false(t : T?) {
    var cfg = Config()
    let a <- ["--verbose=false"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> success(!cfg.verbose, "verbose is false")
}

[test]
def test_enum_flag(t : T?) {
    var cfg = Config()
    let a <- ["--color", "Green"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(int(cfg.color), int(Color.Green))
}

[test]
def test_invalid_int(t : T?) {
    var cfg = Config()
    let a <- ["--count", "abc"]
    t |> success(parse_args(cfg, a) != "", "parse fails on bad int")
}

[test]
def test_invalid_float(t : T?) {
    var cfg = Config()
    let a <- ["--timeout", "abc"]
    t |> success(parse_args(cfg, a) != "", "parse fails on bad floats")
}

[test]
def test_invalid_enum(t : T?) {
    var cfg = Config()
    let a <- ["--color", "Purple"]
    t |> success(parse_args(cfg, a) != "", "parse fails on unknown enum value")
}

[test]
def test_defaults_unchanged(t : T?) {
    var cfg = Config()
    let a : array<string>
    t |> success(parse_args(cfg, a) == "", "parse succeeds with no args")
    t |> equal(cfg.name, "")
    t |> equal(cfg.count, 0)
    t |> equal(cfg.tags, ["default"])
    t |> success(!cfg.verbose)
}

[test]
def test_required(t : T?) {
    // Required flag present: parse succeeds.
    var cfg = ConfigWithRequired()
    let err = parse_args(cfg, ["--foo=1", "--bar-required=hello"])
    t |> equal(err, "")
    t |> equal(cfg.foo, "1")
    t |> equal(cfg.bar_required, "hello")
}

[test]
def test_required_failure(t : T?) {
    // Required flag missing: parse returns a missing-flag error.
    var cfg = ConfigWithRequired()
    let err = parse_args(cfg, ["--foo=1"])
    t |> equal(err, "--bar-required: missing required flag")
}

// --- Missing-value detection ----------------------------------------------
// A bare "--flag" (last token, no following value, no "=value" form) is
// malformed input for any value-bearing flag.  parse_args must report it
// instead of silently leaving the field at default.

[test]
def test_missing_value_string(t : T?) {
    var cfg = Config()
    let err = parse_args(cfg, ["--name"])
    t |> equal(err, "--name: missing value")
}

[test]
def test_missing_value_int(t : T?) {
    var cfg = Config()
    let err = parse_args(cfg, ["--count"])
    t |> equal(err, "--count: missing value")
}

[test]
def test_missing_value_float(t : T?) {
    var cfg = Config()
    let err = parse_args(cfg, ["--timeout"])
    t |> equal(err, "--timeout: missing value")
}

[test]
def test_missing_value_enum(t : T?) {
    var cfg = Config()
    let err = parse_args(cfg, ["--color"])
    t |> equal(err, "--color: missing value")
}

[test]
def test_missing_value_array(t : T?) {
    var cfg = Config()
    let err = parse_args(cfg, ["--tags"])
    t |> equal(err, "--tags: missing value")
}

[test]
def test_missing_value_short_flag(t : T?) {
    // Same path via the short-flag form.
    var cfg = ShortFlagsConfig()
    let err = parse_args(cfg, ["-n"])
    t |> equal(err, "--name: missing value")
}

[test]
def test_bare_bool_still_sets_true(t : T?) {
    // Bool is the one type where bare "--flag" IS the canonical "set true"
    // form, not a missing value.  Make sure the new detection didn't break it.
    var cfg = Config()
    let err = parse_args(cfg, ["--verbose"])
    t |> equal(err, "")
    t |> success(cfg.verbose, "verbose set true by bare flag")
}

// --- Duplicate-flag policy: last occurrence wins -------------------------
// Matches GNU getopt, argparse, clap, etc. — lets users override an earlier
// flag (typically from an alias / wrapper) by appending a later one.

[test]
def test_duplicate_string_last_wins(t : T?) {
    var cfg = Config()
    parse_args(cfg, ["--name=Alice", "--name=Bob"])
    t |> equal(cfg.name, "Bob")
}

[test]
def test_duplicate_int_last_wins(t : T?) {
    var cfg = Config()
    parse_args(cfg, ["--count", "1", "--count=42"])
    t |> equal(cfg.count, 42)
}

[test]
def test_duplicate_bool_last_wins(t : T?) {
    var cfg = Config()
    parse_args(cfg, ["--verbose", "--verbose=false"])
    t |> success(!cfg.verbose, "explicit false after bare true overrides")
}

[test]
def test_short_long_alias_last_wins(t : T?) {
    var cfg = ShortFlagsConfig()
    parse_args(cfg, ["-n", "Alice", "--name", "Bob"])
    t |> equal(cfg.name, "Bob")
}

[test]
def test_long_short_alias_last_wins(t : T?) {
    var cfg = ShortFlagsConfig()
    parse_args(cfg, ["--name", "Bob", "-n", "Alice"])
    t |> equal(cfg.name, "Alice")
}

[test]
def test_get_command_info(t : T?) {
    let info <- get_command_info(type<Config>)
    t |> equal(length(info.args), 6)
    t |> equal(info.args[0].flag_name, "--name")
    t |> equal(info.args[1].flag_name, "--count")
    t |> equal(info.args[2].flag_name, "--verbose")
    t |> equal(info.args[3].flag_name, "--color")
    t |> equal(info.args[4].flag_name, "--timeout")
    t |> equal(info.args[5].flag_name, "--tags")
    t |> equal(length(info.args[3].enum_values), 3)
}

// --- Short flags ----------------------------------------------------------

[test]
def test_short_flag_string(t : T?) {
    var cfg = ShortFlagsConfig()
    let a <- ["-n", "Alice"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.name, "Alice")
}

[test]
def test_short_flag_string_equals(t : T?) {
    var cfg = ShortFlagsConfig()
    let a <- ["-n=Bob"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.name, "Bob")
}

[test]
def test_short_flag_int(t : T?) {
    var cfg = ShortFlagsConfig()
    let a <- ["-c", "42"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.count, 42)
}

[test]
def test_short_flag_float(t : T?) {
    var cfg = ShortFlagsConfig()
    let a <- ["-r=2.5"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.timeout, 2.5)
}

[test]
def test_short_flag_bool(t : T?) {
    var cfg = ShortFlagsConfig()
    let a <- ["-v"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> success(cfg.verbose, "verbose is true")
}

[test]
def test_short_flag_enum(t : T?) {
    var cfg = ShortFlagsConfig()
    let a <- ["-p", "Blue"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(int(cfg.color), int(Color.Blue))
}

[test]
def test_short_flag_array(t : T?) {
    var cfg = ShortFlagsConfig()
    let a <- ["-t", "a", "-t=b", "-t", "c"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.tags, ["a", "b", "c"])
}

[test]
def test_short_and_long_array_preserve_order(t : T?) {
    // Mixing long and short occurrences must preserve command-line order.
    var cfg = ShortFlagsConfig()
    let a <- ["--tags=a", "-t", "b", "--tags=c", "-t=d"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.tags, ["a", "b", "c", "d"])
}

[test]
def test_short_flag_required_present(t : T?) {
    var cfg = ShortFlagsRequiredConfig()
    let err = parse_args(cfg, ["-f", "in.txt"])
    t |> equal(err, "")
    t |> equal(cfg.file, "in.txt")
}

[test]
def test_short_flag_required_missing(t : T?) {
    var cfg = ShortFlagsRequiredConfig()
    let err = parse_args(cfg, [])
    t |> equal(err, "--file: missing required flag")
}

[test]
def test_short_flag_metadata(t : T?) {
    let info <- get_command_info(type<ShortFlagsConfig>)
    t |> equal(info.args[0].flag_name, "--name")
    t |> equal(info.args[0].short_flag_name, "-n")
    t |> equal(info.args[2].flag_name, "--verbose")
    t |> equal(info.args[2].short_flag_name, "-v")
    t |> equal(info.args[6].flag_name, "--help")
    t |> equal(info.args[6].short_flag_name, "-h")
}

// --- get_cli_arguments / get_program_args overloads -----------------------

[test]
def test_get_cli_arguments_after_double_dash(t : T?) {
    let result <- get_cli_arguments(["host", "script.das", "--", "--name=Alice", "--verbose"])
    t |> equal(length(result), 2)
    t |> equal(result[0], "--name=Alice")
    t |> equal(result[1], "--verbose")
}

[test]
def test_get_cli_arguments_no_separator(t : T?) {
    let result <- get_cli_arguments(["host", "script.das", "--name=Alice"])
    t |> equal(length(result), 0)
}

[test]
def test_get_cli_arguments_empty_after_separator(t : T?) {
    let result <- get_cli_arguments(["host", "script.das", "--"])
    t |> equal(length(result), 0)
}

[test]
def test_get_program_args_skips_argv0(t : T?) {
    let result <- get_program_args(["fmt.exe", "--write", "file.das"])
    t |> equal(length(result), 2)
    t |> equal(result[0], "--write")
    t |> equal(result[1], "file.das")
}

[test]
def test_get_program_args_only_argv0(t : T?) {
    let result <- get_program_args(["fmt.exe"])
    t |> equal(length(result), 0)
}

// --- print_help / format_help --------------------------------------------

[test]
def test_format_help_smoke(t : T?) {
    let info <- get_command_info(type<ShortFlagsConfig>)
    let help = format_help(info, "demo")
    t |> success(starts_with(help, "Usage: demo [flags]\n"), "starts with usage line")
    t |> success(find(help, "-n, --name=STRING") != -1, "renders short+long flag spec")
    t |> success(find(help, "-v, --verbose") != -1, "bool flag has no =placeholder")
    t |> success(find(help, "-p, --color=ENUM") != -1, "enum flag uses ENUM placeholder")
    t |> success(find(help, "(Red|Green|Blue)") != -1, "enum values rendered inline")
    t |> success(find(help, "(repeated)") != -1, "array flags marked (repeated)")
}

[test]
def test_format_help_required_marker(t : T?) {
    let info <- get_command_info(type<ShortFlagsRequiredConfig>)
    let help = format_help(info, "upload")
    t |> success(find(help, "(required)") != -1, "required flags marked (required)")
}

[test]
def test_format_help_no_short_alignment(t : T?) {
    let info <- get_command_info(type<Config>)
    let help = format_help(info, "demo")
    // No fields have short flags — every flag line should start with "      --"
    // (two spaces of margin + four spaces where "-X, " would go).
    t |> success(find(help, "      --name") != -1, "long-only flags align with short slot blank")
}
