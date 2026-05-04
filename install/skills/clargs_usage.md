# Skill: Command-line argument parsing with `daslib/clargs`

Read this skill before writing your own daslang scripts or AOT-built
tools that need to parse command-line flags. `daslib/clargs` replaces
hand-rolled `find_index("--foo")` / `args[i+1]` / string-to-int
machinery with a typed, declarative spec — fewer off-by-one bugs, a
free `--help` renderer, and uniform behavior across every tool.

## Why use it

- A single `[CommandLineArgs]` struct is the source of truth — every
  flag is a real field with a real type (`string`, `int`, `bool`,
  `array<string>`, enum). The compiler enforces the schema.
- `print_help` renders complete usage text — short flags, long flags,
  doc strings, defaults — with no duplicated string templates.
- Required flags, repeatable flags (`array<T>`), and enum validation
  all just work.

## Recipe

1. **Add the require:** `require daslib/clargs`.

2. **Pick the argv accessor by host shape:**
   - **Standalone tool** (AOT-built `.exe` you ship, or any binary
     where `argv[0]` is the program and `argv[1..]` are the flags) →
     `get_program_args()`.
   - **Daslang-script host** (run via `daslang script.das -- args...`
     — real flags live after the `--` separator) →
     `get_cli_arguments()`.

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
     `--field-name`, hyphens replacing underscores).
   - `@clarg_short` declares a short alias (`-n`, `-?`).

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

   The single-arg `parse_args(type<Config>)` form pulls argv via the
   macro's chosen accessor automatically. The two-arg
   `parse_args(type<Config>, args)` form lets you pass an explicit
   `array<string>` — useful for tests or scripts that already split
   argv.

## Help-flag pitfall

When run under `daslang` (script-host case), the host intercepts
`-h` / `--help` itself before forwarding script args. Wire your help
field to `-?` instead — see the `?` short flag in the example above.
AOT-built standalone binaries that own argv directly via
`get_program_args()` can use `-h` / `--help` as the natural
convention.

## Reference

- `daslib/clargs.das` — implementation (macro, `parse_args`,
  `print_help`, `get_program_args`, `get_cli_arguments`).
- `examples/clargs/main.das` — minimal end-to-end example with
  required flag, short flags, enum, array, and help wiring.
- `utils/daspkg/commands.das` — production use across multiple
  subcommands.
