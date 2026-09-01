# Woodpecker - the external reviewer (repo-only)

Read this before running codex on a branch, a PR, or a diff - the external-review
dimension of `skills/internal/make_pr.md` and `skills/internal/review_round.md`.

The woodpecker is one round of the Codex CLI's native reviewer over a pinned commit. It
reads with different eyes than the in-house agents, so its value is exactly the findings
our own audits cannot predict. It is also unbounded - on real code the finding stream
never runs dry - so the damper below is part of the design, not a cost-saving measure.

## Invocation

```bash
# both paths outside the repo - the session scratchpad, or logs/
report=${SCRATCH:-/tmp}/woodpecker.md
log=${SCRATCH:-/tmp}/woodpecker.log

# a branch against its base (the normal PR shape)
codex exec --sandbox read-only -o "$report" review --base origin/master 2>&1 | tee "$log"
# one commit
sha=$(git rev-parse HEAD)
codex exec --sandbox read-only -o "$report" review --commit "$sha" 2>&1 | tee "$log"

# either of these means the round never ran - disclose it as skipped
[ -s "$report" ] || echo "EMPTY REPORT - no round"
grep -q "Review was interrupted" "$log" && echo "INTERRUPTED - no round"
```

A round writes two sinks with different jobs: the `-o` report file holds the verdict and the
findings, and the tee'd run log holds the exec trace - what the round actually read and ran.
The report answers "what did it find", the log answers "did it look".

- Global flags go BEFORE the `review` subcommand. `--base` / `--commit` cannot be combined
  with a custom prompt - a custom prompt runs the generic agent, not the native reviewer;
  prefer native.
- The base is `origin/master`, never local `master` - the `make_pr` checklist rebases
  branches onto `origin/master` and leaves local `master` stale, so a stale base sweeps
  unrelated upstream commits into the review.
- Write both sinks outside the repo (the session scratchpad) or under `logs/` (gitignored) -
  a bare in-tree filename is exactly the untracked debris the preflight gate rejects.
- Run it in a tree checked out at the tip you want reviewed, and pin that sha beside both
  sinks - findings are claims about exact bytes, and re-reviews only mean anything against a
  pinned commit. The pinned sha does not freeze what codex reads: the tree must not change
  until harvest - launch after the last mutating step, or give the run its own worktree at
  that sha.
- **Never read the exit status as the verdict: codex exits 0 on a round that never ran.**
  Expired auth exits 0 having printed `Review was interrupted` and token-refresh errors, and
  leaves the report file empty - so an empty report is a round that did not happen, never a
  round that found nothing. A round that found nothing still writes its verdict there.
- No `codex` on PATH, or a round that did not run? The round is skipped, and the PR body says
  so - the gate is the disclosure, never a silent pass.
- A round can finish in under a minute or run for tens of minutes - run it in a background
  Bash and keep working; never sit idle waiting on it.
- **Before reporting a round as no-findings, confirm the run log's exec trace covers the
  files the diff changed.** Duration says nothing about depth - a one-minute round can read
  every changed file and run the suite. A truncated capture leaves only the verdict, which
  reads exactly like a round that did nothing; the exec trace is what tells them apart.

## The findings loop

Findings come back P-ranked (P1 highest). Every finding is a hypothesis:

1. **Verify before believing** - confirm or disprove each claim against the tree with
   your own read or probe, never by trusting the report's quotes.
2. Real -> red-first: a test/corpus entry that fails at the reviewed commit, then the fix.
3. Not real -> record the rejection and its reason wherever the round reports.

## The damper - how many rounds

- **Every `make_pr` arc gets a round, trivial or not** - a round is cheap next to one
  missed defect. Each caller states its own launch and harvest points
  (`make_pr` step 0a3, `review_round` Phase 2); this file owns only the budget.
- **Non-trivial arcs get more than one round by default:** after the findings-driven fix
  batch lands, re-run against the new tip sha. Beyond the defaults, a fix batch re-arms
  a round only when it materially rewrote logic; a batch of doc edits, formatting,
  comment nits, or CI plumbing never re-arms one.
- **Never loop it to quiescence.** The raw finding rate never reaches zero; past the
  first round most of what comes back is marginal, and a fix loop chasing it introduces
  regressions of its own. Judge a round's worth by how many of its findings verify real.
- **Past the defaults, keep going only where failure is silent** - a break no user
  would report. Where failure is loud, users and CI are also detection channels;
  missing a tail bug there is survivable.
