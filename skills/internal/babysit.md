# Babysit (PR review iteration)

**Babysit** an open pull request through to merge. Use this skill after the PR already exists; it covers the post-open loop: watching CI, triaging Copilot/human review comments, discussing verdicts with the user, applying fixes, replying, resolving threads, and re-requesting review.

## 0. Non-negotiable review-round invariant

A Copilot round is complete only when all of these are true:

1. Every Copilot comment in the round has an explicit accept/reject reply.
2. Every corresponding review conversation is resolved, including rejected suggestions.
3. A fresh query reports zero unresolved review threads.
4. Copilot's latest review `commit_id` equals the PR's current `headRefOid`.
5. Every comment of that current-tip review was handled — the terminal state is a judged
   round that produced no push: zero comments, or every comment rejected or ledgered per
   `skills/internal/review_triage.md`, replied, resolved. Merging on Copilot running out of
   comments via accept-and-repush cycles is the anti-pattern this rule exists to kill;
   merge on a round you judged and closed.

**Any push invalidates items 4-5.** Re-request Copilot after every push without exception. Do not rely on `review_on_push`.

Never merge with an unresolved thread or with Copilot's latest review targeting an older commit, even when every CI check is green.

## 1. Watching the PR — the loop

**The session babysits the build; the user does not babysit the session.** A ready,
verified fix gets committed and pushed immediately. "Batch fixes into one push" means: fix
every *distinct* failure currently on the table, then push once — not: wait for the rest of
the matrix to finish. Straggler lanes reproducing an already-fixed failure are zero
information (the post-push run re-tests everything); idling on them while holding ready
fixes is the anti-pattern. Two consecutive status updates saying "still waiting" mean the
loop has stalled — act.

**Iterate against Copilot (~5 min), not the CI matrix (~30 min).** Copilot's review lands in ~5 minutes; the full CI matrix takes ~30. The loop is driven by Copilot — **never sit through a full matrix between rounds**:

1. Compare the PR's current `headRefOid` with Copilot's latest review `commit_id`.
2. If the tip is unreviewed and the prior round has no unresolved threads, **request Copilot review** manually.
3. **~5 min later, act on Copilot's review:** triage each comment and apply its disposition per `skills/internal/review_triage.md` (fetch mechanics in Section 3 below).
4. If fixes changed the branch, run focused gates for the affected surface and push them.
5. Reply to every comment and resolve every conversation from that completed round. Verify unresolved-thread count = 0.
6. If step 4 pushed a new tip, re-request Copilot now. If the round produced no push (all rejected or ledgered), the existing review already covers the current tip.
7. Repeat until the invariant in Section 0 holds. **Only then** wait for CI green and merge.

**CI runs the whole time — watch it, but don't block on it:**
- **Red EARLY (while you're still looping on Copilot) is a win** — a free early signal. Jump on it immediately: fix, push, re-request Copilot (the fix is new code Copilot should see anyway).
- **Red AFTER Copilot is dry → fix → back into the Copilot loop.** A CI fix is new code, so re-request Copilot on the fix commit; don't merge until Copilot is dry again *and* CI is green.
- **Green early means nothing to do** — keep iterating on Copilot until it's dry.

The two heavy Windows toolchain builds (`build_windows_mingw`, `build_windows_clangcl`) run **nightly**, not per-PR — a per-PR red is never one of those.

**Manual re-request is mandatory.** `review_on_push: true` on the default-branch ruleset fires nothing after PR open — Copilot auto-reviews exactly once, at open. Every subsequent round needs an explicit request (Section 6). CI itself *does* auto-run on every PR commit via the `pull_request` trigger, so it gates honestly without any manual nudge.

**Watching is a tool call, not a polling script.** Run
`daslang utils/internal/pr-babysit/main.das -- --pr <N> --watch` BARE in the background —
never piped (`| tail` etc.): a pipeline reports the last command's exit code, and the exit
code IS the verdict. React to it: `2` = CI red (failing checks named), `3` = a review targets the tip and threads
are unresolved — triage it, `0` = CI green + reviewed tip + zero unresolved — ready to
merge, `5` = nothing actionable within the timeout. The tool owns the GitHub mechanics
(job-granular check reads that see matrix reds, the bot-reviewer blind spots, thread
counting); when GitHub changes, fix the tool, not this file. One gap stays manual: a
detached `workflow_dispatch` run has no PR association, so the tool cannot see it — poll it
with `gh run view <runID> --json jobs`.

## 2. CI failure handling

If a check goes red:
1. Distinguish a real red from an infra one: a runner "shutdown signal" / "operation was canceled" with **no `error:` line** in the build log is a canceled/reclaimed runner, not your code — a fresh push's CI run supersedes it, and there is nothing to fix.
2. Identify the failing job: `gh pr checks <PR>` shows the URL; `gh run view <runID> --log-failed` fetches the log.
3. Apply the same fix policy as Step 2 in `skills/internal/make_pr.md`: own change → fix it; obvious pre-existing → fix it; non-obvious pre-existing → ask the user.
4. After the fix, reproduce the failing lane locally when practical and run focused gates for the affected surface. Do not repeat the full preflight; CI is the authoritative complete rerun for the new tip.
5. Push the fix. CI re-runs automatically. The push invalidates Copilot-dry state, so go **back into the Copilot loop** (Section 1) and re-request per Section 0; don't merge until Copilot is dry again and CI is green.

## 3. Triage review comments — `skills/internal/review_triage.md`

Fetch each comment's id, path, line, and body — the GitHub MCP comment listing when
connected, `gh api repos/<owner>/<repo>/pulls/<PR>/comments` otherwise.

Every comment gets a verdict, a disposition, a reply, and a resolved thread per
`skills/internal/review_triage.md` — the verdict tests, the fix-now vs ledgered rule, and what the
user is asked versus told all live there.

## 4. Apply fixes + run focused gates

After the verdicts, and any confirmations the triage rules required:
1. Edit the code per the agreed verdicts.
2. **Watch for contradictory comments.** When fixing a bug, scan inline comments that describe the affected surface — they often need updating in the same pass.
3. Run focused local gates that exercise the changed surface, plus `git diff --check` and any directly applicable formatter/generator check — never the full preflight (Section 2's rule; it ran once per `skills/internal/make_pr.md`).
4. A fix round touching `//!` doc-comments re-runs the docs gates per `skills/internal/documentation_rst.md` (das2rst regen + Sphinx).
5. Commit the fix and push once its focused gates pass; CI remains mandatory before merge. If amending, use **`git push --force-with-lease`** (never `--force`).

## 5. Reply to each comment + resolve all threads

Replying and resolving are **separate API surfaces**. Both are required.

**Reply** uses the REST API via the MCP tool (takes the comment's `id`):
```
mcp__github__add_reply_to_pull_request_comment(commentId=<id>, body="…")
```
An accept's reply states what changed and why; reject-reply content is
`skills/internal/review_triage.md`'s.

**Resolve** is GraphQL-only — REST has no thread resolution — and the tool carries it:
`--list-unresolved` prints each open thread's id and path,
`--resolve <id>` (repeatable) resolves them. **Resolve every thread you replied on,
including rejections** — GitHub collapses resolved threads, so the human reviewer scrolls
past only what still needs action; an unresolved pile of answered comments makes the PR
unreadable. The end state, zero unresolved, is what `--watch` exit `0`/`3` already
distinguishes.

## 6. Re-request Copilot review

**Resolve (and reply to) every prior thread BEFORE re-requesting review.** Re-requesting while old threads are still open leaves the PR ambiguous — you can't tell a stale comment (already addressed, from a superseded commit) from one that still needs action. Resolve first, and any thread that's still open after the next review is unambiguously *new*.

After all replies + resolves + any push, re-request (every push requires one — Section 0).
```
daslang utils/internal/pr-babysit/main.das -- --pr <N> --request-review --watch
```
This requests the review and blocks until something needs action — the same exit codes as
Section 1, plus `1` when the request itself fails. The tool owns the request transport and
its verification — REST vs GraphQL rails, the requested-reviewers blind spot, tip matching
— and a review on an older SHA never satisfies the round.
`mcp__github__request_copilot_review` also works when the GitHub MCP is connected; verify
through the tool's `--watch` either way.

If a round produced no push, do not request an identical review merely because threads were resolved: Copilot already reviewed the current tip. The mandatory trigger is a changed PR tip.

Copilot reviews the new commit's diff and may flag:
- The same defect pattern elsewhere in the file — `skills/internal/review_triage.md`'s post-accept sweep exists to catch these before the push.
- New issues introduced by the fix itself, such as a comment block that now contradicts the new behavior.
- **Zero new comments** = Copilot is done from its perspective. Move to human review.

## 7. Loop until both Copilot and humans are done

Copilot dry is not the exit: the loop ends only when human reviewers are done too — a pending human review holds the merge exactly like an unreviewed tip. The Quick reference below is the per-iteration recap.

## Quick reference

| Step | Tool/Command | Fix policy |
|---|---|---|
| Watch PR | `daslang utils/internal/pr-babysit/main.das -- --pr <N> --watch` | React to the exit code: 2 CI red, 3 triage review, 0 ready to merge, 5 timeout |
| CI fail | `gh pr checks`, `gh run view --log-failed` | Fix own, fix obvious pre-existing, ask about unclear |
| Triage comments | `gh api .../pulls/<PR>/comments` | Verdict + disposition + reply per `skills/internal/review_triage.md` |
| Terminal state | a judged round that produced no push (zero comments, or all rejected/ledgered + resolved) | Never merge on "Copilot ran out" via accept-and-repush cycles |
| Re-run gates | Focused tests + directly applicable format/generator/doc checks | Full preflight once before initial PR push; CI handles complete reruns after review fixes |
| Amend/push | `git commit --amend --no-edit`, `git push --force-with-lease` | Keep squashed branch squashed |
| Reply | `mcp__github__add_reply_to_pull_request_comment` | Every addressed comment gets a reply |
| Resolve | `daslang utils/internal/pr-babysit/main.das -- --pr <N> --list-unresolved` / `--resolve <id>` | Every addressed thread gets resolved; end state is zero unresolved |
| Re-request | `daslang utils/internal/pr-babysit/main.das -- --pr <N> --request-review --watch` | Mandatory after every push, after prior threads are resolved |
| Merge gate | Compare latest Copilot `commit_id` to PR `headRefOid` | CI green + matching reviewed tip + zero unresolved threads |
