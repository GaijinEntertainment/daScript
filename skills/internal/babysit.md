# Babysit (PR review iteration)

The post-open loop: watch CI, triage review comments, fix, reply, resolve, re-request, merge.
PR creation is `skills/internal/make_pr.md`; comment verdicts are
`skills/internal/review_triage.md`.

GitHub mechanics live in the tool:
`daslang utils/internal/pr-babysit/main.das -- --pr <N> --watch`. Exit codes: `0` ready to
merge, `1` request/API failure, `2` CI red (failing checks named), `3` a review targets the
tip with unresolved threads, `5` nothing actionable within the timeout. Run it BARE in the
background, never piped — a pipeline reports the last command's exit code, and the exit code
IS the verdict. When GitHub changes, fix the tool, not this file.

## 0. Stop rule

**The last round is a round that changed nothing.** The loop exits at the first round you
close without pushing: Copilot's latest review targets the current tip
(`commit_id == headRefOid`), handling it required no code change — zero comments, or every
comment rejected or ledgered — and every thread, this round's and all earlier, has an
accept/reject reply and is resolved. A round that changed code can never be the last one:
the change itself is unreviewed, so re-request and go again.

**"Good enough" is the gate, not perfection** — otherwise the fixpoint is unreachable (code
can be endlessly hardened). Known bugs merge; known prose defects merge or ledger. A
finding's bar is not "is it real?" but "is it worth a push?", and the price shifts with the
batch: a push already carrying a code fix takes non-nit prose fixes along nearly free; a
prose-only finding rarely buys a round of its own. Per-finding verdicts:
`skills/internal/review_triage.md`.

Merge = this stop rule + CI green on that same tip + no pending human review. Green CI on a
tip Copilot has not seen fails the rule; so does a quiet Copilot with old threads still open.

**Any push invalidates the review.** Re-request after every push, without exception:
`review_on_push: true` fires nothing after PR open — Copilot auto-reviews exactly once, at
open. CI *does* auto-run on every commit via the `pull_request` trigger.

`--watch` exit `0` means CI green, tip reviewed, zero unresolved; the replies and the
changed-nothing judgment are yours.

## 1. The loop

**The session babysits the build; the user does not babysit the session.** A verified fix is
committed and pushed immediately. "Batch fixes into one push" means: fix every *distinct*
failure on the table, then push once — not wait for the rest of the matrix. Straggler lanes
reproducing an already-fixed failure are zero information; the post-push run re-tests
everything. Two consecutive "still waiting" updates mean the loop stalled — act.

**Iterate against Copilot (~5 min), not the CI matrix (~30 min)** — never sit through a full
matrix between rounds.

1. Tip unreviewed and no unresolved threads from the prior round → re-request (Section 4).
2. ~5 min later, triage every comment and apply its disposition per `review_triage.md`.
3. If fixes changed the branch, run focused gates for the affected surface and push.
4. Reply to and resolve every thread of that round (Section 3).
5. Pushed a new tip → re-request now. No push → the existing review already covers the tip.
6. Repeat until Section 0 holds; **only then** wait for CI green and merge.

CI runs the whole time — watch it, don't block on it. Red early is a free signal: fix, push,
re-request. Red after Copilot is dry: fix, then back into the loop. Green early means nothing
until Copilot is dry.

`build_windows_mingw` and `build_windows_clangcl` run **nightly**, not per-PR — a per-PR red
is never one of those. A detached `workflow_dispatch` run has no PR association, so the tool
cannot see it — poll that one with `gh run view <runID> --json jobs`.

## 2. CI red

1. Infra or real? A runner "shutdown signal" / "operation was canceled" with **no `error:`
   line** in the log is a reclaimed runner: nothing to fix, and a fresh push supersedes it.
2. `gh pr checks <PR>` gives the URL; `gh run view <runID> --log-failed` the log.
3. Fix policy is Step 2 of `skills/internal/make_pr.md`: own change → fix; obvious
   pre-existing → fix; non-obvious pre-existing → ask the user.
4. Reproduce the lane locally when practical, run focused gates for the changed surface —
   never the full preflight; CI is the authoritative complete rerun.
5. Push, then back into the Copilot loop.

## 3. Fix, reply, resolve

Fetch comments with `gh api repos/<owner>/<repo>/pulls/<PR>/comments` (id, path, line, body).
Verdict, disposition, and what the user is asked versus told are all `review_triage.md`'s.

Applying the accepted fixes:

- **Scan for contradictory comments** describing the affected surface — a bug fix often
  strands the prose next to it.
- Focused gates + `git diff --check` + any directly applicable formatter/generator check. A
  round touching `//!` doc-comments re-runs the docs gates per
  `skills/internal/documentation_rst.md`.
- Amend and `git push --force-with-lease` (never `--force`) to keep a squashed branch
  squashed.

Reply and resolve are separate steps; both are required.
`mcp__github__add_reply_to_pull_request_comment(commentId=<id>, body="…")` replies — an
accept states what changed and why; a reject's content is `review_triage.md`'s.
`--list-unresolved` prints each open thread's id and path; `--resolve <id>` (repeatable)
closes them. **Resolve every thread you replied on, rejections included** — GitHub collapses
resolved threads, so the human scrolls past only what still needs action.

## 4. Re-request

**Reply to and resolve every prior thread BEFORE re-requesting.** Then any thread still open
after the next review is unambiguously new; otherwise a stale comment from a superseded
commit is indistinguishable from a live one.

```
daslang utils/internal/pr-babysit/main.das -- --pr <N> --request-review --watch
```

Requests, then blocks until something needs action. A review on an older SHA never satisfies
the round. If the round produced no push, do not re-request merely because threads got
resolved — the trigger is a changed tip.

Zero new comments = Copilot is done; the merge gate stays Section 0's.
