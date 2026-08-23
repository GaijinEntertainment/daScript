# Claude Code hooks

Hooks for agent sessions in this repo. **A hook reads only its stdin payload - 
no daslang, no config reads - and returns in tens of milliseconds.** It must
work when the das binary is down.

There is deliberately no write-time comment hook: comments are working
scaffolding - write them freely during a PR. The gate is `make_pr`'s comment
drain: the formatter strips them per the folder's `.lint_config` policy, and a
non-empty strip goes through the `rescue-bot` agent (report-only ledger; the
session rules on every rescue). Policy: `skills/comment_style_hygiene.md`;
mechanics: `skills/internal/make_pr.md`.

## monitor_guard.jq - PreToolUse, `Monitor|Bash`

Denies `gh pr checks` inside a Monitor, and in Bash denies it whenever the
command also carries `--watch`, `while`, `until`, or `sleep` - the watcher to
use instead is `daslang utils/internal/pr-babysit/main.das -- --pr <N> --watch`,
run bare in a background Bash (exit code is the verdict). A single un-looped
`gh pr checks <N>` from Bash passes.

## Registration

The tracked `.claude/settings.json` registers hooks for every checkout;
machine-local permissions live in `.claude/settings.local.json` (gitignored) - 
the two merge. Commands run relative to the session's start directory - start
sessions at the tree root. Claude Code asks once before running a project's
hooks. Active from the next session start.
