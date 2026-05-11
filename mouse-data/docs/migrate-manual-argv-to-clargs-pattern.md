---
slug: migrate-manual-argv-to-clargs-pattern
title: What is the minimal pattern to migrate a daslang script from manual get_command_line_arguments parsing to daslib/clargs while keeping --help behavior under daslang host?
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

Use `require daslib/clargs`, define a `[CommandLineArgs]` struct with fields for each flag, then parse via `parse_args(type<Args>)`.

Pattern:
1) `var parsed <- parse_args(type<Args>)`
2) On error: print `parsed |> unwrap_err` and `print_help(get_command_info(type<Args>), "tool-name")`
3) `var args <- parsed |> move_unwrap`
4) Handle help with a dedicated field wired as:
   - `@clarg_short = "?"`
   - `@clarg_name = "show-help"`
   This avoids collision with daslang host handling of `-h/--help`.

For boolean flags, use `bool` fields (`html`, `log`, `nomain` etc). For optional value flags, use `string` (empty means absent) or `Option<string>` if you want explicit option semantics.

This replacement removes manual index walking and keeps generated usage text in sync with struct annotations.

## Questions
- What is the minimal pattern to migrate a daslang script from manual get_command_line_arguments parsing to daslib/clargs while keeping --help behavior under daslang host?
