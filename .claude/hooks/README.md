# Claude Code hooks

Hooks for agent sessions in this repo. **A hook reads only its stdin payload, the
file that payload names, and the documents that file cites - no daslang, no config
reads - and returns in tens of milliseconds.** It must work when the das binary is
down.

There is deliberately no write-time comment hook: comments are working
scaffolding - write them freely during a PR. The gate is `make_pr`'s comment
drain: the formatter strips them per the folder's `.lint_config` policy, and a
non-empty strip goes through the `harvester` agent (proposal ledger; the
session rules on every entry). Policy: `skills/comment_style_hygiene.md`;
mechanics: `skills/internal/make_pr.md`.

## monitor_guard.jq - PreToolUse, `Monitor|Bash`

Denies `gh pr checks` inside a Monitor, and in Bash denies it whenever the
command also carries `--watch`, `while`, `until`, or `sleep` - the watcher to
use instead is `daslang utils/internal/pr-babysit/main.das -- --pr <N> --watch`,
run bare in a background Bash (exit code is the verdict). A single un-looped
`gh pr checks <N>` from Bash passes.

## arch_inject.py - PostToolUse, `Edit|Write|MultiEdit`

When an edit lands inside a `.das` function carrying `[arch(at="<doc>.md#<anchor>")]`,
the hook returns the cited section as `additionalContext` - the 40 lines that govern the
function arrive when the function is touched, instead of a 300-line document being read
first. Once per anchor per session: a memo in the temp directory
(`claude-arch-memo-<session_id>.json`) keeps each anchor's section hash, so twelve citers
of one section cost one injection and an edited section re-injects. `--reset` (registered
on `PreCompact` and `SessionEnd`) drops the memo, because after a compaction the earlier
copy has left the context. Sections cap at 60 lines; `arch_of` returns the rest. Reads and
`arch_of` stay quiet - the moment that matters is writing.

## Registration

The tracked `.claude/settings.json` registers hooks for every checkout;
machine-local permissions live in `.claude/settings.local.json` (gitignored) -
the two merge. Commands run relative to the session's start directory - start
sessions at the tree root. Claude Code asks once before running a project's
hooks. Active from the next session start.
