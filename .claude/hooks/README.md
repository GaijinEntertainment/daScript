# Claude Code hooks

Pure-python hooks for agent sessions in this repo. No daslang, no filesystem or
config reads — a hook must work when the das binary is down, and must cost
nothing per invocation.

## comment_guard.py — PostToolUse, `Edit|Write`

Scans the edit payload for `//` and `/* */` comments outside the kept set and,
on a hit, exits 2 with an advisory the agent sees immediately. It never blocks
or undoes the edit — the slap lands at the moment the habit fires, which is the
whole point.

- `.das` — kept set: `//!` public-API docs, `// nolint` suppressions, `//fmt:`
  directives, the file's leading header block (which may sit below the
  `options`/`module`/`require` preamble). Message: the formatter will strip it.
- `.cpp .cc .cxx .c .h .hpp .hxx` — kept set: `//!` API docs, `NOLINT`
  (any case), the leading header block. Message: house rule, no new C/C++
  comments.
- `Edit` compares old vs new: comments present verbatim in the replaced text
  (moves, reindents) never fire. `Write` scans the full content, minus the
  header block.
- Teaching code (tutorials/examples) legitimately keeps prose; the advisory
  says so — there is no path exemption, the agent just proceeds.
- Fail-open: bad JSON, unknown tools, missing fields all exit 0 silently.

Known scanner limits (advisory tool, kept simple on purpose): raw strings
(`R"(...)"`) and unpaired apostrophes in code can mask a comment on the same
line; both are rare and cost a false negative, never a false slap.

## Tests

    python3 .claude/hooks/test_comment_guard.py

Also run by CI in `extended_checks.yml` beside the other python gates.

## Registration

Hooks are wired in `.claude/settings.json` (checked in). Commands use paths
relative to the session's start directory — start sessions at the tree root.
