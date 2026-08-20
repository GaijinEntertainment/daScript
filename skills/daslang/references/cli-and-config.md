# Command-Line Arguments and Environment Config

`daslib/clargs` turns one annotated struct into an argv parser that also renders its own `--help`.

## The recipe

```das
require daslib/clargs

[CommandLineArgs]
struct Config {
    @clarg_required
    @clarg_short = "n"
    @clarg_doc = "Greeting target"
    name : string

    @clarg_short = "t"
    @clarg_doc = "Tags (repeatable)"
    tag : array<string>
}

[export]
def main : int {
    var cfg : Config
    let rc = parse_args_with_help(cfg, "tool-name")
    return rc if (rc >= 0)
    return 0
}
```

**Don't pick an argv accessor.** `parse_args` pulls argv through `get_user_args()`: `argv[1..]` for
a standalone `-exe` binary, the post-`--` slice under the interpreter or the JIT.
`get_program_args()` / `get_cli_arguments()` force one slice regardless of host.

Field types: `string`, `int`, `float`, `bool`, an enum (clargs validates the value), `array<T>` for
a repeatable flag. The long name is the field name with underscores as hyphens; clargs adds the
`--`.

| Annotation / form | Behaviour |
|---|---|
| `@clarg_required` | Missing flag ⇒ `parse_args` returns an error string. |
| `@clarg_doc` | The `--help` text. |
| `@clarg_short = "n"` | Short alias (`-n`). |
| `@clarg_name = "show-help"` | Overrides the derived long name. |
| `@clarg_env = "NAME"` | Environment twin. |
| `@clarg_positional` | Consumed in declaration order. `string`, `Option<string>`, `array<string>` only; an array positional must be last; a required positional cannot follow an optional one; not combinable with `@clarg_short` or `@clarg_count`. |
| `@clarg_count` | Plain `int`; sums occurrences, so `-v -v -v` gives `3`. **Short-flag bundling is not implemented** — `-vvv` parses as nothing and leaves the field `0`. |
| `@clarg_mutex_group = "name"` | Mutually exclusive group. |
| `@clarg_skip` | Excludes the field from the CLI schema. |
| `Option<T>` field type | Distinguishes "not supplied" from the zero value (`require daslib/option`). |

## Parsing and help

`parse_args_with_help(cfg, "tool-name")` is generated for every `[CommandLineArgs]` struct that
declares no `--help` / `-h` field of its own; it intercepts those flags, prints the help, and
returns **`0`** = help printed, **`-1`** = clean parse (struct populated), **`1`** = parse error
(already logged at error level).

Controlling the failure path:

```das
var r <- parse_args(type<Config>)
if (r |> is_err) {
    report("error: {r |> unwrap_err}")
    print_help(get_command_info(type<Config>), "tool-name")
    return
}
let cfg <- r |> move_unwrap
```

`parse_args` returns `Result<Config, string>`; `parse_args(type<Config>, args)` takes an explicit
`array<string>`. Unknown flag-shaped tokens are ignored, not errors. `format_help` returns the help
as a string instead of printing it. Field defaults are **not** rendered in help text.

**The help-flag pitfall.** Under the script host, `daslang` intercepts `-h` / `--help` itself before
forwarding script arguments, even after the `--` separator. A script that must offer help under the
interpreter wires it to `-?` instead:

```das
@clarg_short = "?"
@clarg_name = "show-help"
@clarg_doc = "Show this help and exit"
help : bool
```

Only an `-exe` binary owns argv, so `-h` / `--help` — and `parse_args_with_help`'s automatic help
flag — are reachable only there.

## Environment twins

An option can also come from an environment variable: **command line beats environment beats
default.** Declare the twin per field with `@clarg_env = "NAME"`, or struct-wide with
`[CommandLineArgs(env_prefix = "TOOL")]`, deriving `TOOL_LONG_NAME` from the long name (hyphens
become underscores; `@clarg_env = ""` opts a field out). Help shows it as `(env: NAME)`.

A boolean reads `""`, `0`, `false`, `off`, `no` (any case) as false and anything else as true; a
set-but-empty variable counts as unset; garbage in an int / float / enum variable is a parse
**error**, exactly as on the command line — and so is a bare value-less `--flag` even when the
variable is set. Either carrier satisfies `@clarg_required`. Positional, `@clarg_count`, and
repeatable fields have no environment form; mutex groups are checked on the command line only, so an
environment-supplied value never conflicts.

## `[EnvConfig]` — ambient library knobs

`[EnvConfig(env_prefix = "MYLIB")]` — same `@clarg_doc` / `@clarg_env` vocabulary and name
derivation, `bool` / `int` / `int64` / `float` / `string` fields, field initializers as defaults —
generates `env_config(type<T>) : T` plus `get_env_config_info(type<T>)` for documentation
generators.

```das
let g_cfg = env_config(type<MyLibConfig>)   // at context init, before requirers' globals and [init]
```

Call it **once**, never per call. Same semantics as the twins, except that garbage numeric text logs
a warning and keeps the default instead of erroring. The command-line-only annotations (positional,
count, required, short, mutex) are rejected here.

An `Option<T>` field stays `none` when the variable is unset — read it with `??` or `is_some`.
`CommandArgumentInfo` also exposes, for documentation generators, `@clarg_default_doc = "probed"`
(overrides the rendered default, for when the initializer literal would mislead) and `@clarg_path`
(marks a string field as a filesystem path).
