# Babysit (PR review iteration)

**Babysit** an open pull request through to merge. Use this skill after the PR already exists; it covers the post-open loop: watching CI, triaging Copilot/human review comments, discussing verdicts with the user, applying fixes, replying, resolving threads, and re-requesting review.

## 0. Non-negotiable review-round invariant

A Copilot round is complete only when all of these are true:

1. Every Copilot comment in the round has an explicit accept/reject reply.
2. Every corresponding review conversation is resolved, including rejected suggestions.
3. A fresh query reports zero unresolved review threads. Paginate if GraphQL reports `hasNextPage`; never mistake the first page for the complete set.
4. Copilot's latest review `commit_id` equals the PR's current `headRefOid`.
5. That current-tip review produced no new comments.

**Any push invalidates items 4-5.** Re-request Copilot after every push without exception, including formatting, documentation, generated-file, merge-conflict, and CI-only fixes. Do not rely on `review_on_push`.

Never merge with an unresolved thread or with Copilot's latest review targeting an older commit, even when every CI check is green.

## 1. Watching the PR — the loop

**Iterate against Copilot (~5 min), not the CI matrix (~30 min).** Copilot's review is a free ruleset check that lands in ~5 minutes; the full CI matrix is free too but takes ~30. So the loop is driven by Copilot and you **never sit through a full matrix between rounds**:

1. Compare the PR's current `headRefOid` with Copilot's latest review `commit_id`.
2. If the tip is unreviewed and the prior round has no unresolved threads, **request Copilot review** manually.
3. **~5 min later, act on Copilot's review:** triage each comment (Section 3) and surface your verdicts. Wait for greenlight before applying any accepted or uncertain suggestion. A round containing only clear rejections can be replied to and resolved without blocking.
4. If fixes changed the branch, run the gates and push them.
5. Reply to every comment and resolve every conversation from that completed round. Verify unresolved-thread count = 0.
6. If step 4 pushed a new tip, re-request Copilot now. If the round was reject-only and produced no push, the existing review already covers the current tip.
7. Repeat until the invariant in Section 0 holds. **Only then** wait for CI green and merge.

**CI runs the whole time — watch it, but don't block on it:**
- **Red EARLY (while you're still looping on Copilot) is a win** — a free early signal. Jump on it immediately: fix, push, re-request Copilot (the fix is new code Copilot should see anyway).
- **Red AFTER Copilot is dry → fix → back into the Copilot loop.** A CI fix is new code, so re-request Copilot on the fix commit; don't merge until Copilot is dry again *and* CI is green.
- **Green early means nothing to do** — keep iterating on Copilot until it's dry.

The two heavy Windows toolchain builds (`build_windows_mingw`, `build_windows_clangcl`) run **nightly**, not per-PR — a per-PR red is never one of those.

**Manual re-request is mandatory.** `review_on_push: true` is set on the default-branch ruleset, but in testing it did NOT auto-trigger a review on any push (force-push *or* normal commit) — Copilot only auto-reviews once, on PR open. Every subsequent round needs an explicit `request_copilot_review`. (CI itself *does* auto-run on every PR commit via the `pull_request` trigger, free on standard runners, so it gates honestly without any manual nudge.)

**Polling cadence (use `/loop`, dynamic mode):** ~5 min (≤270s, keeps the prompt cache warm) while iterating on Copilot; switch to ~20 min once Copilot is dry and you're only waiting on the matrix.

**Status commands per tick:**
- `gh pr checks <PR>` — CI status (pending / pass / fail), or `gh api repos/<owner>/<repo>/commits/<tip>/check-runs` filtered to `status!="completed"` (pending) and `conclusion` ∈ {`failure`, `cancelled`, `timed_out`, `action_required`} (reds — don't match only `failure`; a cancelled or timed-out lane is red too)
- `gh api repos/<owner>/<repo>/pulls/<PR>/reviews` — Copilot's latest review; check its `commit_id` matches your tip (confirms it reviewed the latest code, not a stale commit)
- unresolved threads via GraphQL `reviewThreads(first:100){pageInfo{hasNextPage endCursor} nodes{isResolved …}}`, paginated when needed and filtered to `isResolved==false`

Stop polling and surface as soon as: (a) Copilot left new comments (react), (b) a CI check went red (fix), or (c) CI green AND Copilot dry (ready to merge).

> **Pure-prose tail.** If a fresh review contains only prose/wording nits, reject them, reply, resolve, and let the PR proceed to merge. Do not edit, push, or start another review cycle. A small related wording cleanup may ride along when the same round already requires a substantive fix, but prose never justifies a push by itself. Factually wrong or materially misleading text is not a nit.

## 2. CI failure handling

If a check goes red:
1. Identify the failing job: `gh pr checks <PR>` shows the URL; `gh run view <runID> --log-failed` fetches the log.
2. Apply the same fix policy as Step 2 in `skills/make_pr.md`: own change → fix it; obvious pre-existing → fix it; non-obvious pre-existing → ask the user.
3. After the fix, **re-gate to the fix's blast radius** (Section 4, step 3) — compile/format/lint always, plus whatever the change class touches — and let CI re-run the full matrix. A CI failure often comes bundled (a missing format triggers a lint, a removed function triggers an AOT hash mismatch), so read the *whole* failing job log, not just the first error. A red that the local compile/format/lint couldn't have caught (AOT hash, a JIT verifier, a Sphinx warning on untouched files) is exactly what CI is there to catch — fix and push again; don't respond by re-running full preflight every round.
4. Push the fix. CI re-runs automatically. The push invalidates Copilot-dry state, so go **back into the Copilot loop** (Section 1), re-request Copilot on the fix commit, and don't merge until Copilot is dry again and CI is green. There are no exceptions for trivial or CI-only fixes.

## 3. Triaging review comments — discuss BEFORE acting

When a review lands (especially Copilot — fast, often verbose), **don't auto-execute**. Pull the comments, classify each one, and surface to the user with a verdict per comment. Get user sign-off before changing code or text for an accepted/uncertain suggestion; clear rejections need no edit and may be closed immediately.

Fetch the comments:
```bash
gh api repos/<owner>/<repo>/pulls/<PR>/comments | jq '.[] | {id, path, line: (.line // .original_line), body}'
```

**Be conservative — default to reject.** Only ACCEPT a comment when it is a **real bug**, a **real issue**, or **factually incorrect** doc/comment text. REJECT everything else:
- a failure with no realistic path in shipped use
- an overflow/resource limit that cannot be reached within plausible hardware and process lifetime
- an over-defensive guard (protecting a case that can't occur)
- a suggestion that diverges from how the rest of the repo already does it
- prose / wording nits — wording has to be **way off or factually wrong** to be worth a change

Rejecting is the common case, not the exception. Give the user a concise per-comment verdict:

| Verdict | Means | Action |
|---|---|---|
| 🔴 **Real bug / real issue** | reachable in supported use at realistic scale, with meaningful impact | accept — establish the path, then fix |
| 🟡 **Factually wrong text** | a doc/comment/diagnostic states something untrue (e.g. names `_distinct()` when the public API is `distinct()`) | accept — correct it |
| ⚪ **Theoretical / nit / over-defensive / against-convention** | no realistic execution path, wording/style, a can't-happen guard, or repo-divergent suggestion | **reject** with a one-line reason |

### Reality test — require a plausible failure, not mathematical possibility

Before accepting a bug claim, establish all three:

1. **Reachable:** name a shipped caller and an input/state allowed by its contract.
2. **Plausible scale:** show the threshold fits real hardware, data volume, and process lifetime.
3. **Meaningful consequence:** identify the incorrect result, crash, corruption, security exposure, or user-visible failure.

Reject when the claim exists only because an integer is technically finite or an impossible state lacks a guard. Example: a 64-bit game-entity counter wrapping only after `2^64` creations is not a real bug on plausible hardware or within the program's lifetime. Do not add saturation, warnings, assertions, or branches for it.

Probability alone is not the test: a rare race, data-loss path, or attacker-controlled input can be real when the reachability chain is concrete. A reasoned proof is enough when reproduction is impractical, but speculative “could theoretically happen” is not.

Real-bug verdicts deserve a probe before declaring fix direction — the obvious fix isn't always the right fix (e.g. aligning the bind-push wasn't enough for the `take(n) |> sum()` bug because the underlying SQL was degenerate; the right answer was compile-time rejection).

**"Cheap to fix" is NOT a reason to accept — BE CONSERVATIVE.** The
trap: an over-defensive/can't-happen comment arrives, the fix is 3 harmless lines, so you accept
"for contract tightness" — and every such accept hands Copilot a fresh diff, which produces the
next marginal comment; #3492 ran 16 rounds and rounds 12–16 were pure hygiene drip that changed
no reachable behavior. Hold the line the table already draws:
- **Over-defensive / can't-happen guards → reject**, even when the guard is one line. Cite why
  the case can't occur (call-site inventory, an upstream guard, a type that makes it
  unrepresentable). Do not add warnings/asserts for states no shipped caller can produce.
- **Wording/comment nits → reject; don't create a prose-only changeset.** If a substantive fix
  is already being pushed, a directly related cleanup may ride along. Otherwise resolve the
  prose threads and land. Only standalone-fix text that is factually wrong or materially misleading.
- Accepting a marginal comment to "keep the reviewer happy" costs a commit + a full CI matrix +
  a new review round. Rejecting costs one evidence sentence. Reject.

A rejected comment still gets a reply (one-line reason, evidence if the reviewer is wrong) + a resolved thread (Section 5). Surface the verdicts and wait for the user's greenlight before applying accepted or uncertain suggestions. If every comment is a clear rejection under the rules above, close the round without a new push and continue toward merge.

## 4. Apply fixes + re-run gates

After greenlight:
1. Edit the code per the agreed verdicts.
2. **Watch for contradictory comments.** When fixing a bug, scan inline comments that describe the affected surface — they often need updating in the same pass. (If you forget, the next review round will flag it.)
3. **Gate the fix to its blast radius — do NOT re-run full preflight per round.** The branch already passed a full preflight before the PR opened; a review-fix commit is almost always a few localized lines, and **CI runs the whole ~30-min matrix on every push for free** — that is the gate for these commits, not a repeated local matrix. Per round, run only the gates the change can actually break:
   - **Always:** MCP `compile_check` + `format_file` + `lint` (the CI `extended_checks` gate) on the changed files. These are seconds, and a lint/format miss is the one class CI is unforgiving about.
   - **Touched a `//!` doc-comment or RST:** the docs gate (step 4 below).
   - **Touched runtime struct layout / `SimNode_*` / dasLLVM:** a JIT smoke (`skills/make_pr.md` step 2.5).
   - Everything else — the interpreter/AOT/Sphinx/sequence matrix — **let CI run it.** Re-running it locally per round is the wall-clock waste this note exists to kill (a 5-round babysit = ~100 min of redundant local preflight for changes CI already covers).
   - **Full preflight only when the fix is genuinely large** — a new module, a broad refactor across many files, a C++/AST-layout change, or anything you'd hesitate to land on CI alone. That is the "changes are massive" exception, not the default.
4. **`//!` doc-comment changes:** re-run `bin/Release/daslang.exe -documentation doc/reflections/das2rst.das`, delete `doc/sphinx-build`, then run both Sphinx builders. Preflight's docs gate deletes that cache unconditionally so stale doctrees cannot hide warnings. Generated `doc/source/stdlib/generated/*.rst` are gitignored; Sphinx picks them up at build time.
5. Commit (squashed branch: **`git commit --amend --no-edit`** + **`git push --force-with-lease`**, NOT `--force`; multi-commit branch: a fresh commit + `git push`). **The pre-push hook wants a fresh full-preflight token bound to HEAD, which the skip above won't have minted — `git push --no-verify` is the sanctioned bypass here** (the make_pr rule against silent `--no-verify` is about *hiding* a skipped gate; skipping the per-round full matrix during babysit is deliberate and CI-covered, so it isn't silent). CI still runs the full matrix on the push and gates the merge.

**When CI does go red on a push, that IS the signal the local skip missed something** — fix it, re-gate the same way (compile/format/lint + whatever the fix touches), push, re-request Copilot. The point is to let CI find the rare bundled failure, not to pre-empt every one locally. (Distinguish a real red from an infra one: a runner "shutdown signal" / "operation was canceled" with **no `error:` line** in the build log is a canceled/reclaimed runner, not your code — a fresh push's CI run supersedes it.)

## 5. Reply to each comment + resolve all threads

Replying and resolving are **separate API surfaces**. Both are required.

**Reply** uses the REST API via the MCP tool (takes the comment's `id`):
```
mcp__github__add_reply_to_pull_request_comment(commentId=<id>, body="…")
```
Reply content should:
- For accepts: state what changed and why (e.g. "kept your suggestion but tightened the wording because the original exposed macro-author internals").
- For accepts with sibling fixes: mention them ("also caught a sibling diagnostic on line ~996 with the same wrong example").
- For rejects: include evidence — runtime output, file content, a probe script result. Don't just say "I disagree."

**Resolve** uses GraphQL — REST doesn't expose `resolveReviewThread`. Two-step:

```bash
# Get thread node IDs (each thread wraps one or more comments). Inspect
# pageInfo.hasNextPage and paginate when true.
gh api graphql -f query='query { repository(owner:"O", name:"R") { pullRequest(number:N) { reviewThreads(first:100) { pageInfo { hasNextPage endCursor } nodes { id isResolved comments(first:1) { nodes { databaseId path line } } } } } } }'

# Resolve each
gh api graphql -f query="mutation { resolveReviewThread(input:{threadId:\"$thread\"}) { thread { id isResolved } } }"
```

**Resolve every thread you replied on** — including rejections (the discussion is closed; the reply explains why). Verify at the end:

```bash
gh api graphql -f query='query { repository(owner:"O", name:"R") { pullRequest(number:N) { reviewThreads(first:100) { pageInfo { hasNextPage } nodes { id isResolved } } } } }' | jq 'if .data.repository.pullRequest.reviewThreads.pageInfo.hasNextPage then error("paginate review threads") else [.data.repository.pullRequest.reviewThreads.nodes[] | select(.isResolved == false)] | length end'
```
Expect `0`.

## 6. Re-request Copilot review

**Resolve (and reply to) every prior thread BEFORE re-requesting review.** Re-requesting while old threads are still open leaves the PR ambiguous — you can't tell a stale comment (already addressed, from a superseded commit) from one that still needs action. Resolve first, and any thread that's still open after the next review is unambiguously *new*.

After all replies + resolves + any push, re-request. **Every push requires a new request; no change is too trivial to review.**
```
mcp__github__request_copilot_review(owner=…, repo=…, pullNumber=…)
```
Record the current `headRefOid` when requesting. When the review arrives, verify its `commit_id` equals that recorded tip. A review on an older SHA does not satisfy the round, even if it arrived after the latest push.

If a round was reject-only and no push occurred, do not request an identical review merely because threads were resolved: Copilot already reviewed the current tip. The mandatory trigger is a changed PR tip.

Copilot reviews the new commit's diff and may flag:
- Same-class issues you missed elsewhere in the file (it's worth scanning the affected file for each accepted pattern before pushing — saves a round).
- New issues introduced by the fix itself (e.g. a comment block that contradicts the new behavior — happened in chunk-4 round 2).
- **Zero new comments** = Copilot is done from its perspective. Move to human review.

## 7. Loop until both Copilot and humans are done

Each fix iteration: triage → discuss → fix → gate → amend → force-push → reply → resolve → verify zero unresolved → re-request → verify reviewed tip. Convergence is normal — most PRs need 1-3 rounds. Don't get fatigued; the discipline is what keeps the review-comment vs. force-push history clean.

## Quick reference

| Step | Tool/Command | Fix policy |
|---|---|---|
| Watch PR | `gh pr checks`, `gh api .../comments`, `gh api .../reviews` | Surface as soon as Copilot comments, CI fails, or both CI + Copilot are done |
| CI fail | `gh pr checks`, `gh run view --log-failed` | Fix own, fix obvious pre-existing, ask about unclear |
| Triage comments | `gh api .../pulls/<PR>/comments` | **Default reject**; require realistic reachability, scale, and impact. Prose-only round → resolve and land |
| Re-gate a fix | compile/format/lint on changed files (+ docs/JIT smoke only if that class was touched) | **Not** full preflight per round — CI runs the matrix on every push. Full preflight only for a massive change |
| Push a review fix | `git push --no-verify` (multi-commit) / `--force-with-lease --no-verify` (squashed) | Sanctioned skip of the per-round full-preflight token — deliberate + CI-covered, not silent |
| Amend/push | `git commit --amend --no-edit`, `git push --force-with-lease` | Keep squashed branch squashed |
| Reply | `mcp__github__add_reply_to_pull_request_comment` | Every addressed comment gets a reply |
| Resolve | `gh api graphql ... resolveReviewThread` | Every addressed thread gets resolved; paginate and verify unresolved = 0 |
| Re-request | `mcp__github__request_copilot_review` | Mandatory after EVERY push, after prior threads are resolved |
| Merge gate | Compare latest Copilot `commit_id` to PR `headRefOid` | CI green + matching reviewed tip + zero unresolved threads |
