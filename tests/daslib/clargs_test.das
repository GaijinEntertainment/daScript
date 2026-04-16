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

[test]
def test_string_flag(t : T?) {
    var cfg = Config()
    var a <- ["--name", "Alice"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.name, "Alice")
}

[test]
def test_string_flag_equals(t : T?) {
    var cfg = Config()
    var a <- ["--name=Bob"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.name, "Bob")
}

[test]
def test_string_array_flag(t : T?) {
    var cfg = Config()
    var a <- ["--tags=aaa", "--tags", "bbb", "--tags=ccc"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.tags, ["aaa", "bbb", "ccc"])
}

[test]
def test_int_flag(t : T?) {
    var cfg = Config()
    var a <- ["--count", "42"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.count, 42)
}

[test]
def test_float_flag(t : T?) {
    var cfg = Config()
    var a <- ["--timeout", "45.1"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(cfg.timeout, 45.1)
}

[test]
def test_bool_flag(t : T?) {
    var cfg = Config()
    var a <- ["--verbose"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> success(cfg.verbose, "verbose is true")
}

[test]
def test_bool_flag_explicit_false(t : T?) {
    var cfg = Config()
    var a <- ["--verbose=false"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> success(!cfg.verbose, "verbose is false")
}

[test]
def test_enum_flag(t : T?) {
    var cfg = Config()
    var a <- ["--color", "Green"]
    t |> success(parse_args(cfg, a) == "", "parse succeeds")
    t |> equal(int(cfg.color), int(Color.Green))
}

[test]
def test_invalid_int(t : T?) {
    var cfg = Config()
    var a <- ["--count", "abc"]
    t |> success(parse_args(cfg, a) != "", "parse fails on bad int")
}

[test]
def test_invalid_float(t : T?) {
    var cfg = Config()
    var a <- ["--timeout", "abc"]
    t |> success(parse_args(cfg, a) != "", "parse fails on bad floats")
}

[test]
def test_invalid_enum(t : T?) {
    var cfg = Config()
    var a <- ["--color", "Purple"]
    t |> success(parse_args(cfg, a) != "", "parse fails on unknown enum value")
}

[test]
def test_defaults_unchanged(t : T?) {
    var cfg = Config()
    var a : array<string>
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
