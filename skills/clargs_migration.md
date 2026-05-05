# Skill: Migrate hand-rolled argv to `daslib/clargs`

**Read this skill before** editing any in-tree tool that still calls
`get_command_line_arguments()` directly. Standing rule: whenever you
touch such a tool for any reason, migrate its argv parsing to
`daslib/clargs` in the same PR. Do **not** open a dedicated
"migrate every tool" PR — keep migrations opportunistic so they ride
along with whatever you were already doing.

For the **how** — declaring the config struct, picking an argv
accessor, parsing, help wiring — see **`skills/clargs_usage.md`**.
This file is just the migration backlog and scope discipline.

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

## Scope discipline

Migrate **only** the tool you are already editing. Don't bundle
unrelated tool migrations into the same PR — each migration is a
small, self-contained change that should ride alongside the work that
brought you to that file.
