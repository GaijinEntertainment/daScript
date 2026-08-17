# Command-Line Arguments and Environment Config

`daslib/clargs` replaces hand-rolled `find_index("--foo")` / `args[i+1]` / string-to-number
machinery with a declarative spec: one annotated struct is the source of truth, every flag is a
real field with a real type, and `--help` renders itself.

## The recipe

```das
require daslib/clargs

enum LogLevel { Debug, Info, Warning, Error }

[CommandLineArgs]
struct Config {
    @clarg_required
    @clarg_short = "n"
    @clarg_doc = "Greeting target"
    name : string

    @clarg_short = "v"
    @clarg_doc = "Verbose output"
    verbose : bool

    @clarg_short = "l"
    @clarg_doc = "Log level filter"
    level : LogLevel

    @clarg_short = "t"
    @clarg_doc = "Optional tags (repeatable)"
    tag : array<string>
}

[export]
def main : int {
    var cfg : Config
    let rc = parse_args_with_help(cfg, "tool-name")
    return rc if (rc >= 0)

    // ... use cfg.name, cfg.verbose, cfg.level, cfg.tag ...
    return 0
}
```

**Don't pick an argv accessor.** The generated `parse_args` pulls argv through `get_user_args()`,
which is mode-aware: `argv[1..]` for a standalone `-exe` binary, the post-`--` slice under the
interpreter or the JIT. One spelling works in all three; `get_program_args()` /
`get_cli_arguments()` are for the rare case wanting one specific slice regardless of host.

Field types map directly — `string`, `int`, `float`, `bool`, an enum (clargs validates the value),
`array<T>` for a repeatable flag. The flag name derives from the field name with underscores
becoming hyphens, and clargs always emits the long form with its own `--` prefix.

| Annotation / form | Behaviour |
|---|---|
| `@clarg_required` | Missing flag makes `parse_args` return an error string. |
| `@clarg_doc` | The `--help` text. Write it. |
| `@clarg_short = "n"` | A short alias (`-n`). |
| `@clarg_name = "show-help"` | Overrides the derived long name. |
| `@clarg_env = "NAME"` | An environment twin for this option. |
| `@clarg_positional` | Positional, consumed in declaration order. `string`, `Option<string>`, `array<string>` only; an array positional must be last; a required positional cannot follow an optional one; not combinable with `@clarg_short` or `@clarg_count`. |
| `@clarg_count` | Plain `int`; sums occurrences, so `-v -v -v` gives `3`. **Short-flag bundling is not implemented** — `-vvv` parses as nothing and leaves the field `0`. |
| `@clarg_mutex_group = "name"` | Mutually exclusive group; a violation reports `--fast, --slow: mutually exclusive (group 'mode')`. |
| `@clarg_skip` | Excludes the field from the CLI schema — state the struct carries but the user never sets. |
| `Option<T>` field type | Distinguishes "not supplied" from the zero value (`require daslib/option`). |

## Parsing and help

`parse_args_with_help(cfg, "tool-name")` is generated for every `[CommandLineArgs]` struct that
does not declare its own `--help` / `-h` field. It intercepts those two flags, prints the rendered
help, and returns **`0`** = help printed, **`-1`** = clean parse (your struct is populated), **`1`**
= parse error (already logged at error level). Hence the four-line `main` above.

The longer form is worth it only when you want to control the failure path:

```das
var r <- parse_args(type<Config>)
if (r |> is_err) {
    report("error: {r |> unwrap_err}")
    print_help(get_command_info(type<Config>), "tool-name")
    return
}
let cfg <- r |> move_unwrap
```

`parse_args` returns `Result<Config, string>`. The two-argument form
`parse_args(type<Config>, args)` takes an explicit `array<string>` — useful for tests, or for a
caller that shares argv with another consumer (clargs ignores unknown flag-shaped tokens, so the
other tool's flags pass through without error). `format_help` is the string-returning sibling of
`print_help`. Field defaults are **not** rendered in help text.

**The help-flag pitfall.** Under the script host, `daslang` intercepts `-h` / `--help` itself
before forwarding script arguments — even after the `--` separator. A script that must offer help
under the interpreter wires it to `-?` instead:

```das
@clarg_short = "?"
@clarg_name = "show-help"
@clarg_doc = "Show this help and exit"
help : bool
```

A standalone `-exe` binary owns argv directly, so it can use `-h` / `--help` — and that is the only
mode where `parse_args_with_help`'s automatic help flag is reachable.

## Environment twins

Any option can also come from an environment variable: **the command line wins, environment beats
the default.** Declare it per field with `@clarg_env = "NAME"`, or struct-wide with
`[CommandLineArgs(env_prefix = "TOOL")]`, which derives `TOOL_LONG_NAME` from each option's long
name (hyphens become underscores; `@clarg_env = ""` opts a field out). Help shows the twin as
`(env: NAME)`.

Semantics: a boolean reads `""`, `0`, `false`, `off`, `no` (any case) as false and anything else as
true; a set-but-empty variable counts as unset; garbage in an int / float / enum variable is a
parse **error**, exactly as on the command line — and so is a bare value-less `--flag` even when
the variable is set, because argv wins including its errors. `@clarg_required` is satisfied by
either carrier. Positional, `@clarg_count`, and repeatable fields have no environment form. Mutex
groups are checked on the command line only, so an environment-supplied value never conflicts.

## `[EnvConfig]` — ambient library knobs

A library has no argv, so its knobs use the sibling annotation. A struct marked
`[EnvConfig(env_prefix = "MYLIB")]` — same `@clarg_doc` / `@clarg_env` vocabulary, same name
derivation, `bool` / `int` / `int64` / `float` / `string` fields, field initializers as defaults —
generates `env_config(type<T>) : T`, plus `get_env_config_info(type<T>)` for documentation
generators.

```das
let g_cfg = env_config(type<MyLibConfig>)      // global initializer: loads at context init,
                                               // before any requirer's globals or [init]
```

Call it **once** and let hot code read plain struct fields; never re-read the environment per call.
Same semantics as the twins above, with one deliberate difference: garbage numeric text logs a
warning and keeps the default, because a library load must not die on a stray variable. The
command-line-only annotations (positional, count, required, short, mutex) are rejected here.

For a tri-state knob — where presence itself matters, or the effective default is computed at
runtime — declare the field `Option<T>`: it stays `none` when the variable is unset, so the read
site writes `cfg.layers ?? computed_default()` or branches on `is_some`. Two extras feed
documentation generators through `CommandArgumentInfo`: `@clarg_default_doc = "probed"` overrides
the rendered default when the initializer literal would mislead, and `@clarg_path` marks a string
field as a filesystem path.

`--long-flag` is the canonical long form across daslang's own CLI and its modules, and it is what
clargs auto-prepends — follow it for new flags so everything lines up.
