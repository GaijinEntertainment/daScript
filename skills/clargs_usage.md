# Skill: Command-line argument parsing with `daslib/clargs`

Read this skill before writing or editing any tool that parses
command-line flags. `daslib/clargs` replaces hand-rolled
`find_index("--foo")` / `args[i+1]` / string-to-int machinery with a
typed, declarative spec — fewer off-by-one bugs, a free `--help`
renderer, and uniform behavior across every tool.

## Why use it

- A single `[CommandLineArgs]` struct is the source of truth — every
  flag is a real field with a real type (`string`, `int`, `bool`,
  `array<string>`, enum). The compiler enforces the schema.
- `print_help` renders complete usage text — short flags, long flags,
  doc strings, env-twin/repeat/mutex markers — with no duplicated string
  templates. Field defaults are NOT rendered (`default_doc` is populated
  by `[EnvConfig]` only).
- Required flags, repeatable flags (`array<T>`), and enum validation
  all just work.

## Recipe

1. **Add the require:** `require daslib/clargs`.

2. **Don't pick an argv accessor.** The macro-generated
   `parse_args(type<Config>)` pulls argv through `get_user_args()`,
   which is mode-aware: `argv[1..]` for a standalone `daslang -exe`
   binary, the post-`--` slice under the interpreter or JIT. The same
   source works in all three. Reach for `get_program_args()` /
   `get_cli_arguments()` only when you need one specific slice
   regardless of host, or when you're feeding an explicit
   `array<string>` to the two-arg form.

3. **Declare the config struct** (every flag is a field; metadata is
   `@clarg_*` annotations on the field):

   ```das
   require daslib/clargs

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
       @clarg_doc = "Optional tags (repeatable)"
       tag : array<string>

       @clarg_short = "?"           // see "Help-flag pitfall" below
       @clarg_name = "show-help"
       @clarg_doc = "Show this help and exit"
       help : bool
   }
   ```

   - Field types map directly: `string`, `int`, `bool`, `float`,
     `array<T>` for repeatable flags, enum types (clargs validates).
   - `@clarg_required` makes `parse_args` return an error string if
     the flag is missing.
   - `@clarg_doc` populates the `--help` text — write it.
   - `@clarg_name` overrides the auto-derived flag name (default is
     `--field-name`, hyphens replacing underscores). The value is
     always emitted as a `--long-flag` — clargs auto-prepends the
     double dash.
   - `@clarg_short` declares a short alias (`-n`, `-?`).

   Beyond the five above, five more forms carry the cases a
   hand-rolled parser usually gets wrong:

   | Annotation / form | Behaviour |
   |---|---|
   | `@clarg_positional` | Positional args, consumed in declaration order. `string`, `Option<string>`, and `array<string>` only; an `array<string>` positional must be last; a required positional can't follow an optional one; can't combine with `@clarg_short` or `@clarg_count`. |
   | `@clarg_count` | Plain `int` field; sums every occurrence, so `-v -v -v` → `3`. **Short-flag bundling is not implemented** — `-vvv` parses as nothing and leaves the field `0`. |
   | `@clarg_mutex_group = "name"` | Mutually exclusive group. A violation reports `clargs: --fast, --slow: mutually exclusive (group 'mode')`. |
   | `@clarg_skip` | Excludes the field from the CLI schema entirely — for state the struct carries but the user never sets. |
   | `Option<T>` field type | Distinguishes "not supplied" from the zero value (`require daslib/option`). |

4. **Parse and check help:**

   ```das
   [export]
   def main() {
       var r <- parse_args(type<Config>)
       if (r |> is_err) {
           print("error: {r |> unwrap_err}\n\n")
           print_help(get_command_info(type<Config>), "tool-name")
           return
       }
       let cfg <- r |> move_unwrap
       if (cfg.help) {
           print_help(get_command_info(type<Config>), "tool-name")
           return
       }

       // ... use cfg.name, cfg.repeat, cfg.verbose, cfg.level, cfg.tag ...
   }
   ```

   `parse_args` returns `Result<Config, string>`. Use `is_err` /
   `unwrap_err` to inspect failure and `move_unwrap` to take the
   populated struct on success.

   **Shorter, and correct by default: `parse_args_with_help`.** The
   macro generates it for every `[CommandLineArgs]` struct that does
   not declare its own `--help` / `-h` field. It intercepts those two
   flags, prints `format_help_with_auto_help`, and returns `0` = help
   printed, `-1` = clean parse (your struct is populated), `1` = parse
   error (already logged at `LOG_ERROR`). That collapses the twelve
   lines above to four:

   ```das
   [export]
   def main : int {
       var cfg : Config
       let rc = parse_args_with_help(cfg, "tool-name")
       return rc if (rc >= 0)

       // ... use cfg.name, cfg.repeat, cfg.verbose, cfg.level, cfg.tag ...
       return 0
   }
   ```

   Its auto `--help` / `-h` is only reachable from a standalone
   `daslang -exe` binary — under the script host those two flags never
   reach your script (see the pitfall below).

   The single-arg `parse_args(type<Config>)` form pulls argv via
   `get_user_args()` automatically. The two-arg
   `parse_args(type<Config>, args)` form lets you pass an explicit
   `array<string>` — useful for tests, scripts that already split
   argv, or callers that need to share argv with another consumer
   (clargs silently ignores unknown flag-shaped tokens, so other
   tooling's flags pass through without errors).

## Environment twins

Any option can also be settable from an environment variable — the
command line always wins, env beats the default. Declare per field with
`@clarg_env = "NAME"`, or struct-wide with
`[CommandLineArgs(env_prefix = "TOOL")]`, which derives `TOOL_LONG_NAME`
from each option's long name (hyphens become underscores;
`@clarg_env = ""` opts a field out). `--help` shows the twin as
`(env: NAME)`.

Semantics (shared with `daslib/build_const` and the dasLLAMA env
readers): booleans read `""`, `0`, `false`, `off`, `no` (any case) as
false and anything else as true; a set-but-empty variable counts as
unset; a garbage int/float/enum value is a parse ERROR, same as on the
command line — and so is a bare value-less `--flag` even when the
variable is set (argv wins, including its errors). `@clarg_required` is
satisfied by either carrier. Positional, `@clarg_count`, and repeatable
(array) fields have no environment form. Mutex groups check the command
line only — an env-supplied default never conflicts with an explicit
flag.

## `[EnvConfig]` — ambient library knobs

Libraries have no argv, so their knobs use the sibling annotation: a struct
marked `[EnvConfig(env_prefix = "MYLIB")]` (same `@clarg_doc` / `@clarg_env`
vocabulary, same name derivation, bool/int/int64/float/string fields, field
initializers as defaults) generates `env_config(type<T>) : T` — call it once
(a global initializer `let g_cfg = env_config(type<T>)` loads at context
init, before any requirer's globals or `[init]`s) and hot code then reads
plain struct fields, never the environment — plus
`get_env_config_info(type<T>)` for doc-generation and coverage rails.
Same env semantics as the twins above, except garbage numeric text logs a
warning and keeps the default (a library load must not die on a stray
variable). Positional/count/required/short/mutex annotations are rejected —
those are command-line concepts.

Tri-state knobs — presence matters, or the effective default is computed at
runtime — declare the field `Option<T>`: it stays `none` when the variable
is unset, so a read site writes `cfg.layers ?? computed_default()` or
branches on `is_some`. Two doc-rail extras: `@clarg_default_doc = "probed"`
overrides the rendered default text when the initializer literal would
mislead, and `@clarg_path` marks a string field as a filesystem path — both
land in `CommandArgumentInfo` (`default_doc`, `is_path`) for generators.
The worked example is dasLLAMA's `dasllama_env.das` (146 knobs, eleven
area structs, its `ENVIRONMENT.md` generated from the info).

## Help-flag pitfall

When run under `daslang` (script-host case), the host intercepts
`-h` / `--help` itself before forwarding script args — even after the
`--` separator. Wire your help field to `-?` instead — see the `?`
short flag in the example above. Standalone `daslang -exe` binaries own
argv directly and can use `-h` / `--help` as the natural convention,
which is also the only mode where `parse_args_with_help`'s auto help
flag is reachable.

## Daslang convention for long flags

Daslang's own CLI (handled in `utils/daScript/main.cpp`) treats
`--long-flag` as the canonical long form — main.cpp strips one
leading dash and matches `cmd=="-long-flag"`. New flags added to the
daslang core or modules should follow this same `--long-flag`
convention so they line up with clargs's auto-prepended `--` and
existing flags like `--track-smart-ptr`, `--das-profiler-log-file`.

## Reference

- `daslib/clargs.das` — implementation. Macro-generated per struct:
  `parse_args` (one- and two-arg), `parse_args_with_help`,
  `get_command_info`. Hand-written public API: `print_help`,
  `format_help` (the string-returning sibling),
  `format_help_with_auto_help`, `make_auto_help_arg`,
  `get_user_args`, `get_program_args`, `get_cli_arguments`,
  `collect_positional_args`, `count_flag`, `find_mutex_violation`.
- `examples/clargs/main.das` — minimal end-to-end example with
  required flag, short flags, enum, array, and help wiring.
- `utils/daspkg/commands.das` — production use across multiple
  subcommands.
- `modules/dasLLVM/daslib/llvm_exe.das` (`ReleaseDepsArgs`) — sharing
  argv with another consumer (daslang's own flag set).

## Migrating hand-rolled `get_command_line_arguments()` callers

Standing rule: when you edit any in-tree tool that still parses `get_command_line_arguments()` directly, migrate its argv handling to `daslib/clargs` in the same PR. Don't open a dedicated "migrate every tool" PR — keep migrations opportunistic so they ride along with whatever you were already doing. Migrate **only** the tool you're already editing.

To find remaining callers, grep `get_command_line_arguments` under `utils/`, `daslib/`, and `examples/` before adding "still pending" claims to a PR description. The migration is a small, self-contained change: declare the config struct, call `parse_args_with_help` (or `parse_args`), wire help. The reference example is `examples/clargs/main.das`; production patterns are in `utils/daspkg/commands.das`.
