# Claude Code hooks

Pure-python hooks for agent sessions in this repo. No daslang and no config
reads — a hook must work when the das binary is down, and must stay cheap
(~45 ms, interpreter startup dominates).

## comment_guard.py — PostToolUse, `Edit|Write`

Scans the edit payload for `//` and `/* */` comments outside the kept set and,
on a hit, exits 2 with an advisory the agent sees immediately. It never blocks
or undoes the edit — the slap lands at the moment the habit fires, which is the
whole point.

The scanner is a char-level state machine over the comment/string grammar:
strings and bounded char literals mask comment markers, das strings and block
comments span lines (das blocks nest, per `src/parser/ds2_lexer.lpp`), C raw
strings `R"delim(...)delim"` span lines, backslash-continued C strings persist,
and `//` immediately after `:` is a URL.

Kept sets:

- `.das` — exact mirror of `is_kept_comment` in `daslib/das_source_formatter.das`:
  `//!` docs, `//fmt:` (no space), `// nolint:` (colon required) / `// @nolint`
  (case-sensitive), plus the file's leading header block, which may sit below
  the `options`/`module`/`require` preamble (trailing comments on preamble
  lines are not header). Message: the formatter will strip it.
- `.cpp .cc .cxx .c .h .hpp .hxx` — `//!`, `NOLINT*` (any case), `clang-format`
  directives, `/*!`/`/**` doxygen blocks, and the leading header block, which
  may sit below leading preprocessor lines. Message: house rule, no new C/C++
  comments.

Semantics:

- `Edit` fires only on a NET comment increase, quoting the comments not present
  verbatim in the replaced text — moves, reindents, rewords, and deletions are
  all silent. The rule is "no new comments", not "no touching old ones".
- `Write` scans the full content minus the header block; a full-file rewrite of
  a not-yet-swept file therefore lists pre-existing comments too — the message
  says so, the agent proceeds.
- Teaching code (tutorials/examples) legitimately keeps prose; the advisory
  says so — there is no path exemption, the agent just proceeds.
- Fail-open: bad or non-dict JSON, unknown tools, missing fields, and failed
  tool calls (`tool_response.success == false`) all exit 0 silently. stdin is
  decoded as UTF-8 with replacement; a leading BOM is stripped.

Known limits (advisory tool, false negatives acceptable): an Edit fragment
starting inside a block comment or string literal scans as code, and a pure
addition of a header-block line via Edit is slapped (Edit fragments carry no
positional context).

## Tests

    python3 .claude/hooks/test_comment_guard.py

84 cases over the grammar matrix; also run by CI in `extended_checks.yml`
beside the other python gates.

## Registration

Wired in `.claude/settings.json` per tree (see that file's `hooks` block; on
this machine the command is `python` — the `python3` Store stub is not
guaranteed on Windows). Until the settings split lands, registration is
manual — copy the `hooks` block into the tree's `.claude/settings.json`.
