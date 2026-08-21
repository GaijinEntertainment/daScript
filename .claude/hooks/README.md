# Claude Code hooks

Pure-python hooks for agent sessions in this repo. No daslang and no config
reads — a hook must work when the das binary is down, and must stay cheap
(~45 ms on a normal edit; a full-file Write of the largest sources reaches
~90 ms).

## comment_guard.py — PostToolUse, `Edit|Write`

Scans the edit payload for `//` and `/* */` comments outside the kept set and,
on a hit, exits 2 with an advisory the agent sees immediately. It never blocks
or undoes the edit — the slap lands at the moment the habit fires, which is the
whole point.

The scanner is a char-level state machine over the comment/string grammar:
strings and bounded char literals mask comment markers; das strings span lines
and interpolate (`"{expr}"` may hold nested plain strings); das block comments
nest (`src/parser/ds2_lexer.lpp`); das reader macros (`%name~ ... %%`) are
opaque bodies; C raw strings `R"delim(...)delim"` (with `u8`/`L` prefixes) and
backslash-continued C strings span lines; `scheme://` after a real URI scheme
is a URL, not a comment. Control characters are not line breaks.

Kept sets:

- `.das` — exact mirror of `is_kept_comment` in `daslib/das_source_formatter.das`:
  `//!` docs, `//fmt:` (no space), `// nolint:` (colon required) / `// @nolint`
  (case-sensitive), plus the leading header block — every full-line comment
  before the first code line, where the `options`/`module`/`require` preamble
  does not count as code (trailing comments on preamble lines are not header,
  and a leading `/* license */` before same-line code is).
- `.cpp .cc .cxx .c .h .hpp .hxx .inc` — `//!`, `NOLINT*` (any case),
  `clang-format` directives, `/*!` and `/** ` doxygen blocks (star-run banners
  are not doxygen), and the leading header block, where preprocessor directives
  do not count as code. Message: house rule, no new C/C++ comments.

Semantics:

- `Edit` fires only on a NET comment increase, quoting the comments not present
  (whitespace-normalized) in the replaced text — moves, reindents, rewords, and
  deletions are all silent. Quoted line numbers refer to the replacement text.
- `Write` scans the full content minus the header block; a full-file rewrite of
  a not-yet-swept file therefore lists pre-existing comments too — the message
  says so, the agent proceeds.
- Teaching code (tutorials/examples) legitimately keeps prose; the advisory
  says so — there is no path exemption, the agent just proceeds.
- Fail-open: bad or non-dict JSON, unknown tools or events, missing fields, and
  failed tool calls (`tool_response.success == false`) all exit 0 silently.
  stdin is decoded as UTF-8 with replacement; a leading BOM is stripped.

Known limits (advisory tool, false negatives acceptable): an Edit fragment
starting inside a block comment, string literal, or reader-macro body scans as
code; a pure addition of a header-block line via Edit is slapped (fragments
carry no positional context); a comment right after a scheme-shaped label
(`done://x`) is skipped as a URL.

## Tests

    python3 .claude/hooks/test_comment_guard.py

Cases over the grammar matrix plus a corpus invariant over known-tricky
tracked files (reader macros, generated flex output, 250KB+ sources). Also run
by CI in `extended_checks.yml` beside the other python gates.

## Registration

The tracked `.claude/settings.json` registers the hook for every checkout;
machine-local permissions live in `.claude/settings.local.json` (gitignored) —
the two merge. The command runs relative to the session's start directory —
start sessions at the tree root. It invokes `python` (not `python3` — the
Windows Store stub is not guaranteed); Claude Code asks once before running a
project's hooks. Active from the next session start.
