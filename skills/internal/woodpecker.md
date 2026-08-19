# Woodpecker — the external reviewer (repo-only)

Read this before running codex on a branch, a PR, or a diff — the external-review
dimension of `skills/internal/make_pr.md` and `skills/internal/review_round.md`.

The woodpecker is one round of the Codex CLI's native reviewer over a pinned commit. It
reads with different eyes than the in-house agents, so its value is exactly the findings
our own audits cannot predict. It is also unbounded — any real code reviews non-empty
forever — so the damper below is part of the design, not a cost saving.

## Invocation

```bash
# a branch against its base (the normal PR shape)
codex exec --sandbox read-only -o <report.md> review --base master
# one commit
codex exec --sandbox read-only -o <report.md> review --commit <sha>
```

- Global flags go BEFORE the `review` subcommand. `--base` / `--commit` exclude a custom
  prompt — a custom prompt runs the generic agent, not the native reviewer; prefer native.
- Run it in a tree checked out at the tip you want reviewed, and record that sha with the
  report — findings are claims about exact bytes, and re-reviews only mean anything
  against a pinned commit.
- A round runs tens of minutes (~25 measured 2026-08): launch it in a background Bash at
  the start of the caller's round — the review round or the `make_pr` checklist pass —
  and harvest at its end; never sit idle waiting on it.

## The findings loop

Findings come back P-ranked (P1 highest). Every finding is a hypothesis:

1. **Verify before believing** — confirm or disprove each claim against the tree with
   your own read or probe, never by trusting the report's quotes.
2. Real → red-first: a test/corpus entry that fails at the reviewed commit, then the fix.
3. Not real → record the rejection and its reason wherever the round reports.

## The damper — how many rounds

- **Every `make_pr` arc gets a round, trivial or not** — a round is cheap next to one
  missed defect. Each caller states its own launch point (`make_pr` step 0a3,
  `review_round` Phase 2); this file owns only the budget.
- **Non-trivial arcs get more than one round by default:** after the findings-driven fix
  batch lands, re-run against the new tip sha. Beyond the defaults, a fix batch re-arms
  a round only when it materially rewrote logic; a batch of doc edits, formatting,
  comment nits, or CI plumbing never re-arms one.
- **Never loop it to quiescence.** The raw finding rate never reaches zero; past the
  first round the stream turns tail-heavy and a fix loop chasing it starts minting its
  own regressions. Judge a round's worth by how many of its findings verify real —
  never by whether the stream empties.
- **Past the defaults, keep going only where failure is silent** — a break no user
  would report. Where failure is loud, users and CI are also detection channels;
  missing a tail bug there is survivable.
