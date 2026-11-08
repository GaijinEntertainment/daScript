options gen2

require dastest/testing_boost
require daslib/clargs
require daslib/fio
require daslib/rtti

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
struct RequiredWithHelp {
    @clarg_required
    file : string

    @clarg_short = "h"
    help : bool
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

[CommandLineArgs]
struct OptionalConfig {
    @clarg_doc = "verbose flag (None when absent)"
    verbose : Option<bool>

    @clarg_doc = "iteration count (None when absent)"
    count : Option<int>

    @clarg_doc = "user name (None when absent)"
    name : Option<string>
}

[CommandLineArgs]
struct PositionalConfig {
    @clarg_positional
    @clarg_doc = "subcommand name"
    command : string

    @clarg_positional
    @clarg_doc = "subcommand argument"
    command_arg : Option<string>

    @clarg_doc = "project root"
    root : string = "."

    @clarg_doc = "force"
    force : bool

    @clarg_short = "v"
    @clarg_doc = "verbose"
    verbose : bool
}

[CommandLineArgs]
struct OptionalValueConfig {
    @clarg_positional
    @clarg_doc = "input file"
    input : string

    @clarg_short = "d"
    @clarg_optional_value
    @clarg_doc = "dump target"
    dump : Option<string>
}

[CommandLineArgs]
struct PositionalArrayTailConfig {
    @clarg_positional
    @clarg_doc = "first arg"
    head : string

    @clarg_positional
    @clarg_doc = "remaining args"
    rest : array<string>
}

[CommandLineArgs]
struct AutoHelpConfig {
    @clarg_doc = "user name"
    name : string
}

[CommandLineArgs]
struct MutexConfig {
    @clarg_mutex_group = "color"
    color : bool

    @clarg_name = "no-color"
    @clarg_mutex_group = "color"
    no_color : bool

    @clarg_mutex_group = "verbosity"
    quiet : bool

    @clarg_mutex_group = "verbosity"
    loud : bool

    flag_outside_groups : bool
}

[CommandLineArgs]
struct MutexValueConfig {
    name : string
    @clarg_required
    mode : string
    @clarg_mutex_group = "verbosity"
    quiet : bool
    @clarg_mutex_group = "verbosity"
    loud : bool
}

[CommandLineArgs]
struct PositionalValueConfig {
    @clarg_positional
    command : string
    name : string
    @clarg_required
    mode : string
}

[CommandLineArgs]
struct PositionalMutexValueConfig {
    @clarg_positional
    command : string
    name : string
    @clarg_required
    mode : string
    @clarg_mutex_group = "verbosity"
    quiet : bool
    @clarg_mutex_group = "verbosity"
    loud : bool
}

[CommandLineArgs]
struct CountValueConfig {
    name : string
    @clarg_count
    @clarg_short = "v"
    verbose : int
}

[CommandLineArgs]
struct CountConfig {
    @clarg_count
    @clarg_short = "v"
    @clarg_doc = "verbosity"
    verbose : int
}

[test(description = "a token consumed as a flag's value is never read as a boolean, another occurrence or any other flag")]
def test_consumed_flag_values(t : T?) {
    t |> run("a string value cannot enable a boolean") @(t : T?) {
        var r <- parse_args(type<Config>, ["--name", "--verbose"])
        if (r |> is_err) { t |> failure(r |> unwrap_err); return }
        let cfg <- r |> move_unwrap
        t |> equal(cfg.name, "--verbose")
        t |> equal(cfg.verbose, false)
    }
    t |> run("a value is not another occurrence of its flag") @(t : T?) {
        var r <- parse_args(type<Config>, ["--name", "--name=Bob"])
        if (r |> is_err) { t |> failure(r |> unwrap_err); return }
        let cfg <- r |> move_unwrap
        t |> equal(cfg.name, "--name=Bob")
    }
    t |> run("an array value cannot enable a boolean") @(t : T?) {
        var r <- parse_args(type<Config>, ["--tags", "--verbose"])
        if (r |> is_err) { t |> failure(r |> unwrap_err); return }
        let cfg <- r |> move_unwrap
        t |> equal(cfg.tags, ["--verbose"])
        t |> equal(cfg.verbose, false)
    }
    t |> run("a value cannot set an optional flag") @(t : T?) {
        var r <- parse_args(type<OptionalConfig>, ["--name", "--verbose"])
        if (r |> is_err) { t |> failure(r |> unwrap_err); return }
        let cfg <- r |> move_unwrap
        t |> equal(cfg.name |> unwrap_or(""), "--verbose")
        t |> success(cfg.verbose |> is_none)
    }
    t |> run("a value cannot set an enum flag") @(t : T?) {
        var r <- parse_args(type<Config>, ["--name", "--color=Green"])
        if (r |> is_err) { t |> failure(r |> unwrap_err); return }
        let cfg <- r |> move_unwrap
        t |> equal(cfg.name, "--color=Green")
        t |> equal(cfg.color, Color.Red)
    }
    t |> run("only unconsumed flags contribute to a count") @(t : T?) {
        var r <- parse_args(type<CountValueConfig>,
            ["--name", "-v", "-v"])
        if (r |> is_err) { t |> failure(r |> unwrap_err); return }
        let cfg <- r |> move_unwrap
        t |> equal(cfg.name, "-v")
        t |> equal(cfg.verbose, 1)
    }
    t |> run("a value does not violate a count flag") @(t : T?) {
        var r <- parse_args(type<CountValueConfig>,
            ["--name", "--verbose=2"])
        if (r |> is_err) { t |> failure(r |> unwrap_err); return }
        let cfg <- r |> move_unwrap
        t |> equal(cfg.name, "--verbose=2")
        t |> equal(cfg.verbose, 0)
    }
}

[test(description = "with a schema, consumed flag values stay out of positionals, mutex groups and required checks")]
def test_consumed_flag_values_with_schema(t : T?) {
    t |> run("positionals keep consumed flag values separate") @(t : T?) {
        var r <- parse_args(type<PositionalConfig>,
            ["--root", "--force", "run"])
        if (r |> is_err) { t |> failure(r |> unwrap_err); return }
        let cfg <- r |> move_unwrap
        t |> equal(cfg.root, "--force")
        t |> equal(cfg.command, "run")
        t |> equal(cfg.force, false)
    }
    t |> run("consumed values do not enter mutex groups") @(t : T?) {
        var r <- parse_args(type<MutexValueConfig>,
            ["--mode=a", "--name", "--loud", "--quiet"])
        if (r |> is_err) { t |> failure(r |> unwrap_err); return }
        let cfg <- r |> move_unwrap
        t |> equal(cfg.name, "--loud")
        t |> success(cfg.quiet && !cfg.loud)
    }
    t |> run("positional mutex parsing keeps values separate") @(t : T?) {
        var r <- parse_args(type<PositionalMutexValueConfig>,
            ["run", "--mode=a", "--name", "--loud", "--quiet"])
        if (r |> is_err) { t |> failure(r |> unwrap_err); return }
        let cfg <- r |> move_unwrap
        t |> equal(cfg.command, "run")
        t |> equal(cfg.name, "--loud")
        t |> success(cfg.quiet && !cfg.loud)
    }
    t |> run("a value cannot satisfy another required flag") @(t : T?) {
        let r <- parse_args(type<ConfigWithRequired>,
            ["--foo", "--bar-required=x"])
        t |> success(r |> is_err)
    }
    t |> run("mutex required checks skip flag values") @(t : T?) {
        let r <- parse_args(type<MutexValueConfig>,
            ["--name", "--mode=a"])
        t |> success(r |> is_err)
    }
    t |> run("positional required checks skip flag values") @(t : T?) {
        let r <- parse_args(type<PositionalValueConfig>,
            ["run", "--name", "--mode=a"])
        t |> success(r |> is_err)
    }
    t |> run("positional mutex required checks skip flag values") @(t : T?) {
        let r <- parse_args(type<PositionalMutexValueConfig>,
            ["run", "--name", "--mode=a"])
        t |> success(r |> is_err)
    }
}

[test(description = "a string flag takes the next token as its value")]
def test_string_flag(t : T?) {
    let a <- ["--name", "Alice"]
    var r <- parse_args(type<Config>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.name, "Alice")
}

[test(description = "a string flag accepts the --name=value spelling")]
def test_string_flag_equals(t : T?) {
    let a <- ["--name=Bob"]
    var r <- parse_args(type<Config>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.name, "Bob")
}

[test(description = "an array flag collects repeated occurrences in both spellings in order")]
def test_string_array_flag(t : T?) {
    let a <- ["--tags=aaa", "--tags", "bbb", "--tags=ccc"]
    var r <- parse_args(type<Config>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.tags, ["aaa", "bbb", "ccc"])
}

[test(description = "an int flag parses its value")]
def test_int_flag(t : T?) {
    let a <- ["--count", "42"]
    var r <- parse_args(type<Config>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.count, 42)
}

[test(description = "a float flag parses its value")]
def test_float_flag(t : T?) {
    let a <- ["--timeout", "45.1"]
    var r <- parse_args(type<Config>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.timeout, 45.1)
}

[test(description = "a bare bool flag reads true")]
def test_bool_flag(t : T?) {
    let a <- ["--verbose"]
    var r <- parse_args(type<Config>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> success(cfg.verbose, "verbose is true")
}

[test(description = "--flag=false sets a bool flag to false")]
def test_bool_flag_explicit_false(t : T?) {
    let a <- ["--verbose=false"]
    var r <- parse_args(type<Config>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> success(!cfg.verbose, "verbose is false")
}

[test(description = "an enum flag parses a value name into the enum")]
def test_enum_flag(t : T?) {
    let a <- ["--color", "Green"]
    var r <- parse_args(type<Config>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(int(cfg.color), int(Color.Green))
}

[test(description = "a non-numeric int value is a parse error")]
def test_invalid_int(t : T?) {
    let a <- ["--count", "abc"]
    t |> success(parse_args(type<Config>, a) |> is_err, "parse fails on bad int")
}

[test(description = "a non-numeric float value is a parse error")]
def test_invalid_float(t : T?) {
    let a <- ["--timeout", "abc"]
    t |> success(parse_args(type<Config>, a) |> is_err, "parse fails on bad floats")
}

[test(description = "an unknown enum value is a parse error")]
def test_invalid_enum(t : T?) {
    let a <- ["--color", "Purple"]
    t |> success(parse_args(type<Config>, a) |> is_err, "parse fails on unknown enum value")
}

[test(description = "with no arguments every field keeps its declared default")]
def test_defaults_unchanged(t : T?) {
    let a : array<string>
    var r <- parse_args(type<Config>, a)
    t |> success(r |> is_ok, "parse succeeds with no args")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.name, "")
    t |> equal(cfg.count, 0)
    t |> equal(cfg.tags, ["default"])
    t |> success(!cfg.verbose)
}

[test(description = "a required flag that is present parses")]
def test_required(t : T?) {
    // Required flag present: parse succeeds.
    let r <- parse_args(type<ConfigWithRequired>, ["--foo=1", "--bar-required=hello"])
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.foo, "1")
    t |> equal(cfg.bar_required, "hello")
}

[test(description = "a help request succeeds with the required flag absent, while without it the missing-flag error stands")]
def test_required_yields_to_help(t : T?) {
    // `--help` answers with the required flag absent
    for (asked in [["--help"], ["-h"], ["--help", "--file=x.txt"]]) {
        let r <- parse_args(type<RequiredWithHelp>, asked)
        t |> success(r |> is_ok, "help parses with no required flag: {asked}")
        t |> success((r |> unwrap).help, "the help flag reads set: {asked}")
    }
    // the control: no help flag, the required flag's error
    let r <- parse_args(type<RequiredWithHelp>, [])
    t |> equal(r |> unwrap_err, "--file: missing required flag")
}

[test(description = "a missing required flag returns the missing-flag error")]
def test_required_failure(t : T?) {
    // Required flag missing: parse returns a missing-flag error.
    let r <- parse_args(type<ConfigWithRequired>, ["--foo=1"])
    t |> equal(r |> unwrap_err, "--bar-required: missing required flag")
}

// --- Missing-value detection ----------------------------------------------
// A bare "--flag" (last token, no value, no "=value" form) is malformed for any
// value-bearing flag; parse_args must report it, not default silently.

[test(description = "a string flag with no value reports a missing value")]
def test_missing_value_string(t : T?) {
    let r <- parse_args(type<Config>, ["--name"])
    t |> equal(r |> unwrap_err, "--name: missing value")
}

[test(description = "an int flag with no value reports a missing value")]
def test_missing_value_int(t : T?) {
    let r <- parse_args(type<Config>, ["--count"])
    t |> equal(r |> unwrap_err, "--count: missing value")
}

[test(description = "a float flag with no value reports a missing value")]
def test_missing_value_float(t : T?) {
    let r <- parse_args(type<Config>, ["--timeout"])
    t |> equal(r |> unwrap_err, "--timeout: missing value")
}

[test(description = "an enum flag with no value reports a missing value")]
def test_missing_value_enum(t : T?) {
    let r <- parse_args(type<Config>, ["--color"])
    t |> equal(r |> unwrap_err, "--color: missing value")
}

[test(description = "an array flag with no value reports a missing value")]
def test_missing_value_array(t : T?) {
    let r <- parse_args(type<Config>, ["--tags"])
    t |> equal(r |> unwrap_err, "--tags: missing value")
}

[test(description = "the short spelling of a flag with no value reports the same missing-value error under the long name")]
def test_missing_value_short_flag(t : T?) {
    // Same path via the short-flag form.
    let r <- parse_args(type<ShortFlagsConfig>, ["-n"])
    t |> equal(r |> unwrap_err, "--name: missing value")
}

[test(description = "a bare bool flag is a set, not a missing value")]
def test_bare_bool_still_sets_true(t : T?) {
    // Bool is the one type where bare "--flag" IS the canonical "set true"
    // form, not a missing value.  Make sure the new detection didn't break it.
    var r <- parse_args(type<Config>, ["--verbose"])
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> success(cfg.verbose, "verbose set true by bare flag")
}

// --- Duplicate-flag policy: last occurrence wins -------------------------
// Matches GNU getopt, argparse, clap, etc. — lets users override an earlier
// flag (typically from an alias / wrapper) by appending a later one.

[test(description = "the last occurrence of a repeated string flag wins")]
def test_duplicate_string_last_wins(t : T?) {
    let cfg <- parse_args(type<Config>, ["--name=Alice", "--name=Bob"]) |> move_unwrap
    t |> equal(cfg.name, "Bob")
}

[test(description = "the last occurrence of a repeated int flag wins")]
def test_duplicate_int_last_wins(t : T?) {
    let cfg <- parse_args(type<Config>, ["--count", "1", "--count=42"]) |> move_unwrap
    t |> equal(cfg.count, 42)
}

[test(description = "an explicit false after a bare true overrides it")]
def test_duplicate_bool_last_wins(t : T?) {
    let cfg <- parse_args(type<Config>, ["--verbose", "--verbose=false"]) |> move_unwrap
    t |> success(!cfg.verbose, "explicit false after bare true overrides")
}

[test(description = "a long spelling after a short one wins")]
def test_short_long_alias_last_wins(t : T?) {
    let cfg <- parse_args(type<ShortFlagsConfig>, ["-n", "Alice", "--name", "Bob"]) |> move_unwrap
    t |> equal(cfg.name, "Bob")
}

[test(description = "a short spelling after a long one wins")]
def test_long_short_alias_last_wins(t : T?) {
    let cfg <- parse_args(type<ShortFlagsConfig>, ["--name", "Bob", "-n", "Alice"]) |> move_unwrap
    t |> equal(cfg.name, "Alice")
}

[test(description = "get_command_info lists the flags in declaration order with their enum values")]
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

[test(description = "a short flag takes the next token as a string value")]
def test_short_flag_string(t : T?) {
    let a <- ["-n", "Alice"]
    var r <- parse_args(type<ShortFlagsConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.name, "Alice")
}

[test(description = "a short flag accepts the -n=value spelling")]
def test_short_flag_string_equals(t : T?) {
    let a <- ["-n=Bob"]
    var r <- parse_args(type<ShortFlagsConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.name, "Bob")
}

[test(description = "a short int flag parses its value")]
def test_short_flag_int(t : T?) {
    let a <- ["-c", "42"]
    var r <- parse_args(type<ShortFlagsConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.count, 42)
}

[test(description = "a short float flag parses its value with the = spelling")]
def test_short_flag_float(t : T?) {
    let a <- ["-r=2.5"]
    var r <- parse_args(type<ShortFlagsConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.timeout, 2.5)
}

[test(description = "a bare short bool flag reads true")]
def test_short_flag_bool(t : T?) {
    let a <- ["-v"]
    var r <- parse_args(type<ShortFlagsConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> success(cfg.verbose, "verbose is true")
}

[test(description = "a short enum flag parses a value name")]
def test_short_flag_enum(t : T?) {
    let a <- ["-p", "Blue"]
    var r <- parse_args(type<ShortFlagsConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(int(cfg.color), int(Color.Blue))
}

[test(description = "a short array flag collects repeated occurrences")]
def test_short_flag_array(t : T?) {
    let a <- ["-t", "a", "-t=b", "-t", "c"]
    var r <- parse_args(type<ShortFlagsConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.tags, ["a", "b", "c"])
}

[test(description = "mixed long and short array occurrences keep command-line order")]
def test_short_and_long_array_preserve_order(t : T?) {
    // Mixing long and short occurrences must preserve command-line order.
    let a <- ["--tags=a", "-t", "b", "--tags=c", "-t=d"]
    var r <- parse_args(type<ShortFlagsConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.tags, ["a", "b", "c", "d"])
}

[test(description = "a required flag is satisfied by its short spelling")]
def test_short_flag_required_present(t : T?) {
    let r <- parse_args(type<ShortFlagsRequiredConfig>, ["-f", "in.txt"])
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.file, "in.txt")
}

[test(description = "a missing required flag with a short spelling reports its long name")]
def test_short_flag_required_missing(t : T?) {
    let r <- parse_args(type<ShortFlagsRequiredConfig>, [])
    t |> equal(r |> unwrap_err, "--file: missing required flag")
}

[test(description = "command info records the short flag name beside the long one")]
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

[test(description = "get_cli_arguments returns the tokens after the first double dash")]
def test_get_cli_arguments_after_double_dash(t : T?) {
    let result <- get_cli_arguments(["host", "script.das", "--", "--name=Alice", "--verbose"])
    t |> equal(length(result), 2)
    t |> equal(result[0], "--name=Alice")
    t |> equal(result[1], "--verbose")
}

[test(description = "get_cli_arguments returns nothing when there is no double dash")]
def test_get_cli_arguments_no_separator(t : T?) {
    let result <- get_cli_arguments(["host", "script.das", "--name=Alice"])
    t |> equal(length(result), 0)
}

[test(description = "get_cli_arguments returns nothing when the double dash ends the list")]
def test_get_cli_arguments_empty_after_separator(t : T?) {
    let result <- get_cli_arguments(["host", "script.das", "--"])
    t |> equal(length(result), 0)
}

[test(description = "get_program_args drops the program name and keeps the rest")]
def test_get_program_args_skips_argv0(t : T?) {
    let result <- get_program_args(["fmt.exe", "--write", "file.das"])
    t |> equal(length(result), 2)
    t |> equal(result[0], "--write")
    t |> equal(result[1], "file.das")
}

[test(description = "get_program_args returns nothing when only the program name is present")]
def test_get_program_args_only_argv0(t : T?) {
    let result <- get_program_args(["fmt.exe"])
    t |> equal(length(result), 0)
}

// --- print_help / format_help --------------------------------------------

[test(description = "format_help renders the usage line, short and long specs, enum values and the repeated marker")]
def test_format_help_smoke(t : T?) {
    let info <- get_command_info(type<ShortFlagsConfig>)
    let help = format_help(info, "demo")
    // Test runs under the interpreter, so format_help emits the daslang-script invocation form.
    t |> success(starts_with(help, "Usage: daslang demo -- [flags]\n"), "starts with usage line")
    t |> success(find(help, "-n, --name=STRING") != -1, "renders short+long flag spec")
    t |> success(find(help, "-v, --verbose") != -1, "bool flag has no =placeholder")
    t |> success(find(help, "-p, --color=ENUM") != -1, "enum flag uses ENUM placeholder")
    t |> success(find(help, "(Red|Green|Blue)") != -1, "enum values rendered inline")
    t |> success(find(help, "(repeated)") != -1, "array flags marked (repeated)")
}

[test(description = "format_help marks required flags")]
def test_format_help_required_marker(t : T?) {
    let info <- get_command_info(type<ShortFlagsRequiredConfig>)
    let help = format_help(info, "upload")
    t |> success(find(help, "(required)") != -1, "required flags marked (required)")
}

[test(description = "flags without short spellings align with the blank short slot")]
def test_format_help_no_short_alignment(t : T?) {
    let info <- get_command_info(type<Config>)
    let help = format_help(info, "demo")
    // No fields have short flags — every flag line should start with "      --"
    // (two spaces of margin + four spaces where "-X, " would go).
    t |> success(find(help, "      --name") != -1, "long-only flags align with short slot blank")
}

// --- Option<T> fields ----------------------------------------------------

[test(description = "absent optional flags stay none")]
def test_optional_bool_absent_is_none(t : T?) {
    let a : array<string>
    let r <- parse_args(type<OptionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds with no args")
    let cfg <- r |> unwrap
    t |> success(cfg.verbose |> is_none, "verbose stays None when flag absent")
    t |> success(cfg.count |> is_none, "count stays None when flag absent")
    t |> success(cfg.name |> is_none, "name stays None when flag absent")
}

[test(description = "a bare optional bool flag becomes some(true)")]
def test_optional_bool_set_true(t : T?) {
    let a <- ["--verbose"]
    let r <- parse_args(type<OptionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> success(cfg.verbose |> is_some, "verbose becomes Some")
    t |> equal(cfg.verbose |> unwrap, true)
}

[test(description = "--flag=false on an optional bool becomes some(false)")]
def test_optional_bool_set_false_explicit(t : T?) {
    let a <- ["--verbose=false"]
    let r <- parse_args(type<OptionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> success(cfg.verbose |> is_some, "explicit --verbose=false produces Some(false)")
    t |> equal(cfg.verbose |> unwrap, false)
}

[test(description = "an optional int flag becomes some of its value")]
def test_optional_int_set(t : T?) {
    let a <- ["--count", "7"]
    let r <- parse_args(type<OptionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> success(cfg.count |> is_some, "count Some")
    t |> equal(cfg.count |> unwrap, 7)
}

[test(description = "an optional string flag becomes some of its value")]
def test_optional_string_set(t : T?) {
    let a <- ["--name=Alice"]
    let r <- parse_args(type<OptionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> success(cfg.name |> is_some, "name Some")
    t |> equal(cfg.name |> unwrap, "Alice")
}

[test(description = "a bad optional int value is an error naming the flag")]
def test_optional_int_invalid_value(t : T?) {
    let a <- ["--count=notanumber"]
    let r <- parse_args(type<OptionalConfig>, a)
    t |> success(r |> is_err, "parse fails on bad int")
    t |> success(find(r |> unwrap_err, "--count") != -1, "error mentions flag name")
}

// --- Positional arguments -------------------------------------------------
// @clarg_positional fills fields from non-flag tokens in declaration order: plain
// string = required, Option<string> = optional, array<string> collects the rest.

[test(description = "required and optional positionals fill in order")]
def test_positional_basic(t : T?) {
    let a <- ["install", "dasImgui"]
    let r <- parse_args(type<PositionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.command, "install")
    t |> success(cfg.command_arg |> is_some, "command_arg present")
    t |> equal(cfg.command_arg |> unwrap, "dasImgui")
}

[test(description = "an absent optional positional stays none")]
def test_positional_only_required(t : T?) {
    let a <- ["list"]
    let r <- parse_args(type<PositionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.command, "list")
    t |> success(cfg.command_arg |> is_none, "command_arg absent")
}

[test(description = "a missing required positional is an error naming it")]
def test_positional_missing_required(t : T?) {
    let a : array<string>
    let r <- parse_args(type<PositionalConfig>, a)
    t |> success(r |> is_err, "parse fails when required positional missing")
    t |> success(find(r |> unwrap_err, "command") != -1, "error mentions positional name")
}

[test(description = "positionals and flags can interleave in any order")]
def test_positional_mixed_with_flags(t : T?) {
    let a <- ["--root", "/tmp", "install", "dasImgui", "--force"]
    let r <- parse_args(type<PositionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.command, "install")
    t |> equal(cfg.command_arg |> unwrap, "dasImgui")
    t |> equal(cfg.root, "/tmp")
    t |> success(cfg.force, "force flag set")
}

[test(description = "a --flag=value spelling does not consume the following positional")]
def test_positional_equals_form_flag_no_consumption(t : T?) {
    // `--root=/tmp` MUST NOT consume `install` as the flag value.
    let a <- ["--root=/tmp", "install"]
    let r <- parse_args(type<PositionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.root, "/tmp")
    t |> equal(cfg.command, "install")
    t |> success(cfg.command_arg |> is_none, "no second positional")
}

[test(description = "a space-separated short flag consumes the next token and later tokens are positionals")]
def test_positional_short_flag_with_value(t : T?) {
    // Space-separated short flag form takes the next token as value.
    let a <- ["-v", "install", "foo"]
    let r <- parse_args(type<PositionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> success(cfg.verbose, "verbose set")
    t |> equal(cfg.command, "install")
    t |> equal(cfg.command_arg |> unwrap, "foo")
}

[test(description = "an unknown flag is dropped, not read as a positional")]
def test_positional_unknown_flag_skipped(t : T?) {
    // Unknown `--unknown` is silently dropped, NOT treated as positional.
    let a <- ["--unknown", "install"]
    let r <- parse_args(type<PositionalConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.command, "install")
}

[test(description = "an array positional collects the remaining tokens")]
def test_positional_array_tail(t : T?) {
    let a <- ["a", "b", "c", "d"]
    var r <- parse_args(type<PositionalArrayTailConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.head, "a")
    t |> equal(cfg.rest, ["b", "c", "d"])
}

[test(description = "an array positional may be empty")]
def test_positional_array_tail_empty(t : T?) {
    let a <- ["only"]
    var r <- parse_args(type<PositionalArrayTailConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal(cfg.head, "only")
    t |> equal(length(cfg.rest), 0)
}

[test(description = "help shows required and optional positionals in the usage line and a positionals section")]
def test_positional_help_renders_usage(t : T?) {
    let info = get_command_info(type<PositionalConfig>)
    let help = format_help(info, "test")
    t |> success(find(help, "<command>") != -1, "required positional in usage")
    t |> success(find(help, "[<command_arg>]") != -1, "optional positional bracketed in usage")
    t |> success(find(help, "Positional arguments:") != -1, "positionals section header")
}

// --- Auto --help / -h -----------------------------------------------------
// parse_args_with_help is generated when the user declared no --help/-h flag;
// returns -1 on clean parse, 0 on help, 1 on parse error.

[test(description = "format_help_with_auto_help injects --help and -h when the struct declares none")]
def test_auto_help_long(t : T?) {
    // format_help_with_auto_help injects --help / -h into the help table when
    // the user did not declare their own (AutoHelpConfig has none).
    let info = get_command_info(type<AutoHelpConfig>)
    let help = format_help_with_auto_help(info, "test")
    t |> success(find(help, "--help") != -1, "auto-injected --help row")
    t |> success(find(help, "-h, --help") != -1, "auto-injected -h short alias")
}

[test(description = "auto help does not add a second row when the struct declares its own help flag")]
def test_auto_help_user_help_no_inject(t : T?) {
    // ShortFlagsConfig (defined above) declares its OWN help field; auto-help
    // must NOT add a duplicate row.
    let info = get_command_info(type<ShortFlagsConfig>)
    let help = format_help_with_auto_help(info, "test")
    // count occurrences of "--help" — should be exactly 1 (the user's one).
    var n = 0
    var search_from = 0
    while (true) {
        let idx = find(help, "--help", search_from)
        if (idx == -1) {
            break
        }
        n++
        search_from = idx + 1
    }
    t |> equal(n, 1, "exactly one --help row")
}

// --- Mutex groups ---------------------------------------------------------

[test(description = "one member of a mutex group parses")]
def test_mutex_group_one_member(t : T?) {
    let a <- ["--color"]
    let r <- parse_args(type<MutexConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> success(cfg.color, "color set")
    t |> success(!cfg.no_color, "no_color clear")
}

[test(description = "two members of one mutex group are an error naming both flags and the group")]
def test_mutex_group_violation(t : T?) {
    let a <- ["--color", "--no-color"]
    let r <- parse_args(type<MutexConfig>, a)
    t |> success(r |> is_err, "mutex violation reported")
    t |> success(find(r |> unwrap_err, "--color") != -1, "error mentions --color")
    t |> success(find(r |> unwrap_err, "--no-color") != -1, "error mentions --no-color")
    t |> success(find(r |> unwrap_err, "color") != -1, "error mentions group name")
}

[test(description = "a mutex group with no member set parses")]
def test_mutex_group_none_set(t : T?) {
    let a : array<string>
    t |> success(parse_args(type<MutexConfig>, a) |> is_ok, "parse succeeds")
}

[test(description = "members of different mutex groups may be set together")]
def test_mutex_different_groups_coexist(t : T?) {
    let a <- ["--color", "--quiet"]
    let r <- parse_args(type<MutexConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> success(cfg.color, "color set")
    t |> success(cfg.quiet, "quiet set")
}

[test(description = "help annotates each flag with its mutex group")]
def test_mutex_help_decoration(t : T?) {
    let info = get_command_info(type<MutexConfig>)
    let help = format_help(info, "test")
    t |> success(find(help, "(mutex: color)") != -1, "color group annotated")
    t |> success(find(help, "(mutex: verbosity)") != -1, "verbosity group annotated")
}

// --- Count action ---------------------------------------------------------

[test(description = "a count flag defaults to zero")]
def test_count_default(t : T?) {
    let a : array<string>
    let r <- parse_args(type<CountConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.verbose, 0)
}

[test(description = "one long occurrence of a count flag counts one")]
def test_count_long_once(t : T?) {
    let a <- ["--verbose"]
    let r <- parse_args(type<CountConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.verbose, 1)
}

[test(description = "three short occurrences count three")]
def test_count_short_thrice(t : T?) {
    let a <- ["-v", "-v", "-v"]
    let r <- parse_args(type<CountConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.verbose, 3)
}

[test(description = "long and short occurrences of a count flag add up")]
def test_count_mixed_long_short(t : T?) {
    let a <- ["-v", "--verbose"]
    let r <- parse_args(type<CountConfig>, a)
    t |> success(r |> is_ok, "parse succeeds")
    let cfg <- r |> unwrap
    t |> equal(cfg.verbose, 2)
}

[test(description = "a count flag rejects the =value spelling")]
def test_count_rejects_value_form(t : T?) {
    let a <- ["--verbose=2"]
    let r <- parse_args(type<CountConfig>, a)
    t |> success(r |> is_err, "value form rejected")
    t |> success(find(r |> unwrap_err, "--verbose") != -1, "error mentions flag")
}

[test(description = "help marks a count flag as repeating")]
def test_count_help_decoration(t : T?) {
    let info = get_command_info(type<CountConfig>)
    let help = format_help(info, "test")
    t |> success(find(help, "(repeats)") != -1, "count flag annotated as repeats")
}

// ===== environment twins =====

[CommandLineArgs(env_prefix = "CLET")]
struct EnvTwinConfig {
    @clarg_doc = "name"
    name : string

    @clarg_doc = "level"
    level : int

    @clarg_doc = "verbose"
    verbose : bool

    @clarg_doc = "palette"
    color : Color

    @clarg_env = "CLET_MODE_X"
    @clarg_doc = "explicit env name"
    mode : string

    @clarg_env = ""
    @clarg_doc = "opted out of the prefix derivation"
    quiet : bool

    @clarg_name = "show-mode"
    @clarg_doc = "renamed flag; env derives from the long name"
    renamed : bool
}

[CommandLineArgs(env_prefix = "CLETR")]
struct EnvRequiredConfig {
    @clarg_required
    @clarg_doc = "must be supplied"
    must : string
}

[CommandLineArgs(env_prefix = "CLETM")]
struct EnvMutexConfig {
    @clarg_mutex_group = "fmt"
    as_json : bool
    @clarg_mutex_group = "fmt"
    as_text : bool
}

def clear_clet_env() {
    // "" counts as unset by contract, so cleanup never needs a true unset
    for (name in ["CLET_NAME", "CLET_LEVEL", "CLET_VERBOSE", "CLET_COLOR", "CLET_MODE_X",
                  "CLET_SHOW_MODE", "CLETR_MUST", "CLETM_AS_JSON", "CLETM_AS_TEXT"]) {
        set_env_variable(name, "")
    }
}

[test(description = "each flag's environment twin is named from its field, an override, or none")]
def test_env_twin_names(t : T?) {
    let info = get_command_info(type<EnvTwinConfig>)
    let by_field <- { for (a in info.args); a.field_name => a.env_name }
    t |> equal("CLET_NAME", by_field?["name"] ?? "?")
    t |> equal("CLET_LEVEL", by_field?["level"] ?? "?")
    t |> equal("CLET_MODE_X", by_field?["mode"] ?? "?")
    t |> equal("CLET_SHOW_MODE", by_field?["renamed"] ?? "?")
    t |> equal("", by_field?["quiet"] ?? "?")
}

[test(description = "an environment value fills a flag that argv leaves out, and argv wins when both are present")]
def test_env_fallback_and_precedence(t : T?) {
    clear_clet_env()
    set_env_variable("CLET_NAME", "envn")
    set_env_variable("CLET_LEVEL", "9")
    set_env_variable("CLET_MODE_X", "m1")
    let no_args : array<string>
    var r <- parse_args(type<EnvTwinConfig>, no_args)
    t |> success(r |> is_ok, "env-only parse succeeds")
    let cfg <- r |> move_unwrap
    t |> equal("envn", cfg.name)
    t |> equal(9, cfg.level)
    t |> equal("m1", cfg.mode)
    let a <- ["--name", "cli", "--level", "3"]
    var r2 <- parse_args(type<EnvTwinConfig>, a)
    t |> success(r2 |> is_ok, "mixed parse succeeds")
    let cfg2 <- r2 |> move_unwrap
    t |> equal("cli", cfg2.name)
    t |> equal(3, cfg2.level)
    t |> equal("m1", cfg2.mode)
    clear_clet_env()
}

[test(description = "an environment bool reads false only for the false spellings, and an argv bool beats it")]
def test_env_bool_rule(t : T?) {
    clear_clet_env()
    let no_args : array<string>
    for (p in [("1", true), ("yes", true), ("banana", true), ("0", false), ("off", false), ("No", false), ("FALSE", false)]) {
        set_env_variable("CLET_VERBOSE", p._0)
        var r <- parse_args(type<EnvTwinConfig>, no_args)
        t |> success(r |> is_ok, "bool parse succeeds for '{p._0}'")
        let cfg <- r |> move_unwrap
        t |> success(cfg.verbose == p._1, "'{p._0}' reads as {p._1}")
    }
    set_env_variable("CLET_VERBOSE", "0")
    let a <- ["--verbose"]
    var r <- parse_args(type<EnvTwinConfig>, a)
    t |> success(r |> is_ok, "argv bool beats env")
    let cfg <- r |> move_unwrap
    t |> success(cfg.verbose, "--verbose wins over CLET_VERBOSE=0")
    clear_clet_env()
}

[test(description = "a garbage environment int is a parse error naming the bad int")]
def test_env_int_garbage_is_loud(t : T?) {
    clear_clet_env()
    set_env_variable("CLET_LEVEL", "fifty")
    let no_args : array<string>
    let r <- parse_args(type<EnvTwinConfig>, no_args)
    t |> success(r |> is_err, "garbage env int refuses")
    t |> success(find(r |> unwrap_err, "invalid int") != -1, "error names the bad int")
    clear_clet_env()
}

[test(description = "an environment enum parses a value name and rejects an unknown one")]
def test_env_enum(t : T?) {
    clear_clet_env()
    let no_args : array<string>
    set_env_variable("CLET_COLOR", "Red")
    var r <- parse_args(type<EnvTwinConfig>, no_args)
    t |> success(r |> is_ok, "enum via env parses")
    let cfg <- r |> move_unwrap
    t |> success(cfg.color == Color.Red, "enum value taken from env")
    set_env_variable("CLET_COLOR", "Banana")
    let r2 <- parse_args(type<EnvTwinConfig>, no_args)
    t |> success(r2 |> is_err, "invalid enum via env refuses")
    t |> success(find(r2 |> unwrap_err, "invalid enum value") != -1, "error names the enum problem")
    clear_clet_env()
}

[test(description = "an environment value satisfies a required flag while a bare value-less argv flag keeps its error")]
def test_env_required_and_edges(t : T?) {
    clear_clet_env()
    let no_args : array<string>
    let r <- parse_args(type<EnvRequiredConfig>, no_args)
    t |> success(r |> is_err, "missing required still refuses")
    set_env_variable("CLETR_MUST", "x")
    var r2 <- parse_args(type<EnvRequiredConfig>, no_args)
    t |> success(r2 |> is_ok, "env satisfies @clarg_required")
    let cfg <- r2 |> move_unwrap
    t |> equal("x", cfg.must)
    // a bare value-less flag keeps its argv error even with the env twin set
    set_env_variable("CLET_NAME", "envn")
    let bare <- ["--name"]
    let r3 <- parse_args(type<EnvTwinConfig>, bare)
    t |> success(r3 |> is_err, "value-less argv flag stays an error")
    t |> success(find(r3 |> unwrap_err, "missing value") != -1, "error is missing value")
    // set-but-empty counts as unset
    set_env_variable("CLET_NAME", "")
    var r4 <- parse_args(type<EnvTwinConfig>, no_args)
    t |> success(r4 |> is_ok, "empty env parse succeeds")
    let cfg4 <- r4 |> move_unwrap
    t |> equal("", cfg4.name)
    clear_clet_env()
}

[test(description = "an environment-supplied mutex member does not trip the group")]
def test_env_mutex_is_argv_only(t : T?) {
    clear_clet_env()
    set_env_variable("CLETM_AS_JSON", "1")
    let a <- ["--as-text"]
    var r <- parse_args(type<EnvMutexConfig>, a)
    t |> success(r |> is_ok, "env-supplied member does not trip the mutex group")
    let cfg <- r |> move_unwrap
    t |> success(cfg.as_text, "argv member set")
    t |> success(cfg.as_json, "env member still populates its field")
    clear_clet_env()
}

[test(description = "help names each flag's environment twin")]
def test_env_help_decoration(t : T?) {
    let info = get_command_info(type<EnvTwinConfig>)
    let help = format_help(info, "test")
    t |> success(find(help, "(env: CLET_NAME)") != -1, "help names the env twin")
}

// ===== [EnvConfig] — ambient library knobs =====

[EnvConfig(env_prefix = "CLEC")]
struct EngineKnobs {
    @clarg_doc = "flag knob"
    fast_math : bool = true

    @clarg_doc = "int knob"
    lanes : int = 8

    @clarg_doc = "float knob"
    scale : float = 1.5

    @clarg_env = "CLEC_MODEL_PATH"
    @clarg_doc = "string knob"
    model : string = "default.gguf"
}

def clear_clec_env() {
    for (name in ["CLEC_FAST_MATH", "CLEC_LANES", "CLEC_SCALE", "CLEC_MODEL_PATH"]) {
        set_env_variable(name, "")
    }
}

[test(description = "env_config keeps declared defaults and reads overrides from the environment")]
def test_env_config_defaults_and_overrides(t : T?) {
    clear_clec_env()
    let d = env_config(type<EngineKnobs>)
    t |> success(d.fast_math, "bool default holds")
    t |> equal(8, d.lanes)
    t |> success(d.scale > 1.49 && d.scale < 1.51, "float default holds")
    t |> equal("default.gguf", d.model)
    set_env_variable("CLEC_FAST_MATH", "off")
    set_env_variable("CLEC_LANES", "24")
    set_env_variable("CLEC_SCALE", "0.25")
    set_env_variable("CLEC_MODEL_PATH", "m.gguf")
    let k = env_config(type<EngineKnobs>)
    t |> success(!k.fast_math, "bool knob reads")
    t |> equal(24, k.lanes)
    t |> success(k.scale > 0.24 && k.scale < 0.26, "float knob reads")
    t |> equal("m.gguf", k.model)
    clear_clec_env()
}

[test(description = "a garbage environment value keeps the default and a set-but-empty one counts as unset")]
def test_env_config_edges(t : T?) {
    clear_clec_env()
    set_env_variable("CLEC_LANES", "banana")   // garbage logs a warning and keeps the default
    let k = env_config(type<EngineKnobs>)
    t |> equal(8, k.lanes)
    set_env_variable("CLEC_LANES", "")         // set-but-empty is unset
    let k2 = env_config(type<EngineKnobs>)
    t |> equal(8, k2.lanes)
    clear_clec_env()
}

[test(description = "environment config info lists each field with its variable name")]
def test_env_config_info(t : T?) {
    let info = get_env_config_info(type<EngineKnobs>)
    t |> equal(4, length(info.args))
    let by_field <- { for (a in info.args); a.field_name => a.env_name }
    t |> equal("CLEC_FAST_MATH", by_field?["fast_math"] ?? "?")
    t |> equal("CLEC_MODEL_PATH", by_field?["model"] ?? "?")
}

// ===== [EnvConfig] — int64 and Option-wrapped (tri-state) fields =====

[EnvConfig(env_prefix = "CLEO")]
struct TriKnobs {
    @clarg_doc = "int64 knob"
    big : int64 = 42l

    @clarg_doc = "tri-state flag"
    want_gpu : Option<bool>

    @clarg_doc = "tri-state int"
    layers : Option<int>

    @clarg_doc = "tri-state int64"
    vram : Option<int64>

    @clarg_doc = "tri-state string"
    pin : Option<string>

    @clarg_default_doc = "probed"
    @clarg_path
    @clarg_doc = "path knob with doc-default override"
    root : string = "bin/daslang"
}

def clear_cleo_env() {
    for (name in ["CLEO_BIG", "CLEO_WANT_GPU", "CLEO_LAYERS", "CLEO_VRAM", "CLEO_PIN", "CLEO_ROOT"]) {
        set_env_variable(name, "")
    }
}

[test(description = "optional env fields stay none when unset and read int, int64, bool and string values when set")]
def test_env_config_tri_state(t : T?) {
    clear_cleo_env()
    let d = env_config(type<TriKnobs>)
    t |> equal(42l, d.big)
    t |> success(is_none(d.want_gpu), "unset tri-state flag stays none")
    t |> success(is_none(d.layers), "unset tri-state int stays none")
    t |> success(is_none(d.vram), "unset tri-state int64 stays none")
    t |> success(is_none(d.pin), "unset tri-state string stays none")
    t |> equal("bin/daslang", d.root)
    t |> equal(7, d.layers ?? 7)                     // ?? falls through on none
    set_env_variable("CLEO_BIG", "9000000000")       // above 2^31 — the int64 lane is real
    set_env_variable("CLEO_WANT_GPU", "off")
    set_env_variable("CLEO_LAYERS", "24")
    set_env_variable("CLEO_VRAM", "8589934592")
    set_env_variable("CLEO_PIN", "metal")
    let k = env_config(type<TriKnobs>)
    t |> equal(9000000000l, k.big)
    t |> success(is_some(k.want_gpu) && !unwrap(k.want_gpu), "set tri-state flag reads its value")
    t |> equal(24, k.layers ?? -1)
    t |> equal(8589934592l, k.vram ?? -1l)
    t |> equal("metal", k.pin ?? "?")
    clear_cleo_env()
}

[test(description = "garbage and empty values leave optional env fields none or at their default")]
def test_env_config_tri_state_edges(t : T?) {
    clear_cleo_env()
    set_env_variable("CLEO_LAYERS", "banana")   // garbage warns and keeps the default (none)
    set_env_variable("CLEO_BIG", "banana")
    let k = env_config(type<TriKnobs>)
    t |> success(is_none(k.layers), "garbage tri-state int stays none")
    t |> equal(42l, k.big)
    set_env_variable("CLEO_LAYERS", "")         // set-but-empty is unset
    set_env_variable("CLEO_VRAM", "")
    let k2 = env_config(type<TriKnobs>)
    t |> success(is_none(k2.layers) && is_none(k2.vram), "empty tri-state stays none")
    clear_cleo_env()
}

[test(description = "environment config info records default docs, path marks and optional wraps per field")]
def test_env_config_default_doc_and_path(t : T?) {
    let info = get_env_config_info(type<TriKnobs>)
    t |> equal(6, length(info.args))
    for (a in info.args) {
        if (a.field_name == "big") {
            t |> equal("42", a.default_doc)
            t |> success(!a.is_optional_wrap && !a.is_path, "plain int64 field flags")
            t |> success(a.value_type == Type.tInt64, "int64 value_type recorded")
        } elif (a.field_name == "root") {
            t |> equal("probed", a.default_doc)   // @clarg_default_doc wins over the initializer
            t |> success(a.is_path, "@clarg_path recorded")
        } elif (a.field_name == "vram") {
            t |> equal("", a.default_doc)
            t |> success(a.is_optional_wrap, "Option wrap recorded")
            t |> success(a.value_type == Type.tInt64, "Option inner type recorded")
        }
    }
    let engine_info = get_env_config_info(type<EngineKnobs>)
    for (a in engine_info.args) {
        if (a.field_name == "model") {
            t |> equal("default.gguf", a.default_doc)   // initializer renders when no override
        }
    }
}

[CommandLineArgs]
struct RenamedHelpConfig {
    @clarg_name = "show-help"
    help : bool
    name : string
}

[CommandLineArgs]
struct ClaimedShortHConfig {
    @clarg_short = "h"
    host : string

    @clarg_name = "show-help"
    help : bool
}

[test(description = "--help and -h set the help field even when the struct names its flag differently")]
def test_help_reaches_a_renamed_help_flag(t : T?) {
    for (asked in fixed_array("--help", "-h", "--show-help")) {
        var r <- parse_args(type<RenamedHelpConfig>, [asked])
        t |> success(r |> is_ok, "{asked} parses")
        let cfg <- r |> move_unwrap
        t |> success(cfg.help, "{asked} sets the help field the struct named --show-help")
    }
}

[test(description = "a help alias is not taken when it is a flag's value, follows a bare double dash, or is claimed by another flag")]
def test_help_alias_leaves_claimed_tokens_alone(t : T?) {
    var as_value <- parse_args(type<RenamedHelpConfig>, ["--name", "--help"])
    let cfg_value <- as_value |> move_unwrap
    t |> success(!cfg_value.help && cfg_value.name == "--help", "a --help that is a flag's value is that value, not a help request")
    var passed_on <- parse_args(type<RenamedHelpConfig>, ["--name", "x", "--", "--help"])
    let cfg_passed <- passed_on |> move_unwrap
    t |> success(!cfg_passed.help && cfg_passed.name == "x", "a --help after a bare -- belongs to whoever takes the rest")
    var claimed <- parse_args(type<ClaimedShortHConfig>, ["-h", "box"])
    let cfg_claimed <- claimed |> move_unwrap
    t |> success(!cfg_claimed.help && cfg_claimed.host == "box", "a -h the struct declares for another flag stays that flag")
    var plain <- parse_args(type<RenamedHelpConfig>, ["--name", "y"])
    let cfg_plain <- plain |> move_unwrap
    t |> success(!cfg_plain.help, "control: no help request leaves the field false")
    var no_field <- parse_args(type<Config>, ["--help", "--name", "z"])
    t |> success(no_field |> is_ok, "a struct with no help field still ignores --help")
    let cfg_no_field <- no_field |> move_unwrap
    t |> equal(cfg_no_field.name, "z")
}

[test(description = "get_host_binary returns an absolute path when it found the file and a bare name when the shell did")]
def test_get_host_binary(t : T?) {
    let host = get_host_binary()
    if (fexist(host)) {
        t |> equal(get_full_file_name(host), host, "a host found by path is absolute")
    } else {
        t |> equal(host, base_name(host), "a host the shell found on PATH stays a bare name: {host}")
    }
    let bare = "no-such-binary-7f37b1e9"
    t |> success(!fexist(get_full_file_name(bare)), "a bare name resolves beside the cwd, where nothing sits - get_host_binary keeps such a name bare for PATH")
}

[test(description = "a host started by a relative path finds itself by an absolute path", timeout = 1200)]
def test_get_host_binary_relative_start(t : T?) {
    let host = get_host_binary()
    if (!fexist(host)) {
        t |> skip("the host was found on PATH, so there is no path to start it by")
        return
    }
    var err : string
    let rel = relative(host, getcwd(), err)
    if (!empty(err) || is_absolute(rel)) {
        t |> skip("the host is not under the current directory: {err}")
        return
    }
    let by_name = get_platform_name() == "windows" ? replace_extension(rel, "") : rel
    // a bare name is a PATH lookup on POSIX, not a start by path: from the host's own directory the relative path needs its `./`
    let argv0 = base_name(by_name) == by_name ? path_join(".", by_name) : by_name
    var output : string
    let rc = run_and_capture([argv0, "-dasroot", get_das_root(),
            path_join(get_das_root(), "tests/daslib/_host_binary_probe.das")], output, 120.0)
    t |> equal(rc, 0, "the host starts by '{argv0}': {output}")
    let child = strip(output)
    t |> success(is_absolute(child) && fexist(child), "a host started by '{argv0}' finds itself by an absolute path, not '{child}'")
}


// --- @clarg_optional_value -----------------------------------------

[test(description = "an optional-value flag gives an empty value bare and its value with = and in the short form, and help renders it")]
def test_optional_value_flag(t : T?) {
    t |> run("the bare flag gives an empty value and leaves the next token positional") @(t : T?) {
        var r <- parse_args(type<OptionalValueConfig>, ["--dump", "main.das"])
        t |> success(r |> is_ok, "parse succeeds")
        let cfg <- r |> move_unwrap
        t |> equal(cfg.dump |> unwrap_or("absent"), "")
        t |> equal(cfg.input, "main.das")
    }
    t |> run("--flag=value gives the value") @(t : T?) {
        var r <- parse_args(type<OptionalValueConfig>, ["main.das", "--dump=out.ll"])
        let cfg <- r |> move_unwrap
        t |> equal(cfg.dump |> unwrap_or(""), "out.ll")
    }
    t |> run("the short bare flag gives an empty value") @(t : T?) {
        var r <- parse_args(type<OptionalValueConfig>, ["-d", "main.das"])
        let cfg <- r |> move_unwrap
        t |> equal(cfg.dump |> unwrap_or("absent"), "")
        t |> equal(cfg.input, "main.das")
    }
    t |> run("short -d=value gives the value") @(t : T?) {
        var r <- parse_args(type<OptionalValueConfig>, ["main.das", "-d=x.ll"])
        let cfg <- r |> move_unwrap
        t |> equal(cfg.dump |> unwrap_or(""), "x.ll")
    }
    t |> run("absent flag stays none") @(t : T?) {
        var r <- parse_args(type<OptionalValueConfig>, ["main.das"])
        let cfg <- r |> move_unwrap
        t |> success(cfg.dump |> is_none, "dump is none")
    }
    t |> run("help renders the optional value") @(t : T?) {
        let help = format_help(get_command_info(type<OptionalValueConfig>), "demo")
        t |> success(find(help, "-d, --dump[=STRING]") != -1, "optional-value placeholder")
    }
}
