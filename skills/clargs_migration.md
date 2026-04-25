# Skill: Migrate hand-rolled argv to `daslib/clargs`

**Read this skill before** editing any in-tree tool that still calls
`get_command_line_arguments()` directly. The standing rule (Boris,
2026-04-24): whenever you touch such a tool for any reason, migrate its
argv parsing to `daslib/clargs` in the same PR. Do **not** open a
dedicated "migrate every tool" PR — keep migrations opportunistic so
they ride along with whatever you were already doing.

## Why

`daslib/clargs` replaces ad-hoc `find_index("--foo")` + `args[i+1]`
machinery with:

- A typed, declarative `[CommandLineArgs]` struct — every flag is a
  field with a real type (`string`, `int`, `bool`, `array<string>`,
  enum). The compiler enforces the schema.
- A free `--help` renderer (`print_help`) that lists every flag with
  its short alias, doc string, and default — no hand-written usage text
  to drift out of sync.
- Uniform short-flag (`-v`), required-flag, and enum-validation
  handling across every tool. Less code, fewer off-by-one bugs.

## Backlog (as of 2026-04-25)

These files still call `get_command_line_arguments()` directly. Migrate
the one you happen to be editing — leave the rest:

- `utils/lint/main.das`
- `utils/aot/main.das`
- `utils/daspkg/main.das` (partly migrated; check before doing more)
- `utils/dasFormatter/main.das`
- `utils/benchctl/main.das`
- `utils/mcp/main.das`
- `utils/daslang-live/main.das`
- `daslib/debug.das`
- `daslib/ansi_colors.das`
- `examples/daStrudel/features/feature_common.das`

This list will shrink over time. Re-grep for
`get_command_line_arguments` in `utils/`, `daslib/`, and `examples/`
before adding "still pending" claims to a PR description.

## Recipe

1. **Add the require:** `require daslib/clargs`.

2. **Pick the argv accessor by host shape:**
   - **Standalone tool** (AOT-built `.exe`, `daspkg`, `lint`, `aot`,
     `das-fmt`, etc. — argv[0] is the binary, argv[1..] are flags) →
     `get_program_args()`.
   - **Daslang-script host** (run via `daslang script.das -- args...` —
     real flags live after the `--` separator) → `get_cli_arguments()`.

3. **Declare the config struct:**
   ```das
   [CommandLineArgs]
   struct Config {
       @clarg_required
       @clarg_short = "n"
       @clarg_doc = "Greeting target"
       name : string

       @clarg_short = "r"
       @clarg_doc = "Repeat count"
       repeat : int

       @clarg_short = "v"
       @clarg_doc = "Verbose"
       verbose : bool

       @clarg_short = "?"           // see "Help-flag pitfall" below
       @clarg_name = "show-help"
       @clarg_doc = "Show this help and exit"
       help : bool
   }
   ```
   - Field types map directly: `string`, `int`, `bool`, `float`,
     `array<T>` for repeatable flags, enum types (clargs validates).
   - `@clarg_required` panics if the user omits the flag.
   - `@clarg_doc` populates the `--help` text — write it.
   - `@clarg_name` overrides the auto-derived flag name (default is
     `--field-name`, hyphens replacing underscores).

4. **Parse and check help:**
   ```das
   var cfg = Config()
   let err = parse_args(cfg)            // or parse_args(cfg, args)
   if (cfg.help) {
       print_help(get_command_info(type<Config>), "tool-name")
       return
   }
   if (err != "") {
       print("error: {err}\n\n")
       print_help(get_command_info(type<Config>), "tool-name")
       return
   }
   ```
   The single-arg `parse_args(cfg)` form pulls argv via the macro's
   chosen accessor; the two-arg form lets you pass an explicit
   `array<string>` — useful for scripts that already split argv or for
   tests.

5. **Delete the hand-rolled parsing:** every `find_index("--foo")`,
   `args[i+1]`, manual short-flag check, and string-to-int coercion the
   tool used to carry. Read `cfg.foo` directly.

## Help-flag pitfall

When run under `daslang.exe` (script-host case), the host intercepts
`-h` / `--help` itself before forwarding script args. Wire your help
field to `-?` instead — see `examples/clargs/main.das` for the
canonical pattern. AOT-built standalone binaries that own argv directly
(`get_program_args()`) can use `-h` / `--help` as the natural
convention.

## Reference

- `daslib/clargs.das` — implementation (macro, `parse_args`,
  `print_help`, `get_program_args`, `get_cli_arguments`).
- `examples/clargs/main.das` — minimal end-to-end example with
  required flag, short flags, enum, array, and help wiring.
- `utils/daspkg/commands.das` — production use across multiple
  subcommands.

## Scope discipline

Migrate **only** the tool you are already editing. Don't bundle
unrelated tool migrations into the same PR — each migration is a
small, self-contained change that should ride alongside the work that
brought you to that file.
