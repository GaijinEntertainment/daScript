# PR Review Iteration

Use this skill after the pull request already exists. It covers the post-open loop: watching CI, triaging Copilot/human review comments, discussing verdicts with the user, applying fixes, replying, resolving threads, and re-requesting review.

## 1. Watching the PR — the loop

**Iterate against Copilot (~5 min), not the CI matrix (~30 min).** Copilot's review is a free ruleset check that lands in ~5 minutes; the full CI matrix is free too but takes ~30. So the loop is driven by Copilot and you **never sit through a full matrix between rounds**:

1. **Push** a commit.
2. **Re-request Copilot** — `request_copilot_review` (or the PR's "re-request review" button). You MUST do this manually after *every* push (see the review_on_push note below).
3. **~5 min later, act on Copilot's review:** triage each comment (Section 3), **surface your verdicts to the user and wait for greenlight**, then fix what's agreed, push, re-request. (Discuss-before-fix is non-negotiable — see Section 3.) Repeat until Copilot is **dry** — a fresh review on your current tip with no new comments and no unresolved threads.
4. **Only once Copilot is dry, wait for CI green** (~30 min), then merge.

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
- unresolved threads via GraphQL `reviewThreads(first:50){nodes{isResolved …}}` filtered to `isResolved==false`

Stop polling and surface as soon as: (a) Copilot left new comments (react), (b) a CI check went red (fix), or (c) CI green AND Copilot dry (ready to merge).

> **Pure-prose tail.** Prose/wording nits are **reject by default** (Section 3) — don't fix them, so the drip never starts. Each reword only hands Copilot a fresh paragraph to nit; it's self-perpetuating. Accept a wording change only when it's way off or factually wrong. (Real example: nightly-CI PR #3237 took 1 functional round then 6 prose rounds that, under this bar, should have been rejected.)

## 2. CI failure handling

If a check goes red:
1. Identify the failing job: `gh pr checks <PR>` shows the URL; `gh run view <runID> --log-failed` fetches the log.
2. Apply the same fix policy as Step 2 in `skills/make_pr.md`: own change → fix it; obvious pre-existing → fix it; non-obvious pre-existing → ask the user.
3. After the fix, **re-run the gates from Step 1-5 in `skills/make_pr.md`** (lint + interpreted + AOT + Sphinx if `//!` or RST touched + format) — don't trust spot-checks. CI failures often come bundled (a missing format triggers a lint, a removed function triggers an AOT hash mismatch).
4. Push the fix. CI re-runs automatically. If Copilot was already dry, this is a "red after Copilot" — go **back into the Copilot loop** (Section 1): re-request Copilot on the fix commit and don't merge until it's dry again *and* CI is green. (A trivial CI-only fix — a format tweak with no logic change — can skip the re-request, but when in doubt, re-request; it's free and ~5 min.)

## 3. Triaging review comments — discuss BEFORE acting

When a review lands (especially Copilot — fast, often verbose), **don't auto-execute**. Pull the comments, classify each one, and surface to the user with a verdict per comment. Only act after the user signs off.

Fetch the comments:
```bash
gh api repos/<owner>/<repo>/pulls/<PR>/comments | jq '.[] | {id, path, line: (.line // .original_line), body}'
```

**Be conservative — default to reject.** Only ACCEPT a comment when it is a **real bug**, a **real issue**, or **factually incorrect** doc/comment text. REJECT everything else:
- a bug that can never happen, or is very unlikely
- an over-defensive guard (protecting a case that can't occur)
- a suggestion that diverges from how the rest of the repo already does it
- prose / wording nits — wording has to be **way off or factually wrong** to be worth a change

Rejecting is the common case, not the exception. Give the user a concise per-comment verdict:

| Verdict | Means | Action |
|---|---|---|
| 🔴 **Real bug / real issue** | a correctness or design problem that can actually occur | accept — probe first, then fix |
| 🟡 **Factually wrong text** | a doc/comment/diagnostic states something untrue (e.g. names `_distinct()` when the public API is `distinct()`) | accept — correct it |
| ⚪ **Nit / unlikely / over-defensive / against-convention** | wording, style, a can't-happen guard, repo-divergent suggestion | **reject** with a one-line reason |

Real-bug verdicts deserve a probe before declaring fix direction — the obvious fix isn't always the right fix (e.g. aligning the bind-push wasn't enough for the `take(n) |> sum()` bug because the underlying SQL was degenerate; the right answer was compile-time rejection).

A rejected comment still gets a reply (one-line reason, evidence if the reviewer is wrong) + a resolved thread (Section 5). Surface the verdicts and wait for the user's greenlight before applying — they may reject more ("reject this one too") or defer a fix to a later chunk.

## 4. Apply fixes + re-run gates

After greenlight:
1. Edit the code per the agreed verdicts.
2. **Watch for contradictory comments.** When fixing a bug, scan inline comments that describe the affected surface — they often need updating in the same pass. (If you forget, the next review round will flag it.)
3. **Re-run the full gates** from Step 1-5 in `skills/make_pr.md`. Yes, full. CI is unforgiving; every amend goes back through the whole matrix.
4. **`//!` doc-comment changes:** re-run `bin/Release/daslang.exe doc/reflections/das2rst.das`, then a clean Sphinx build (`rm -rf doc/sphinx-build site/doc` first — cached builds hide errors). Generated `doc/source/stdlib/generated/*.rst` are gitignored; Sphinx picks them up at build time.
5. **`git commit --amend --no-edit`** + **`git push --force-with-lease`** (NOT `--force` — protects against racing pushes).

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
# Get thread node IDs (each thread wraps one or more comments)
gh api graphql -f query='query { repository(owner:"O", name:"R") { pullRequest(number:N) { reviewThreads(first:30) { nodes { id isResolved comments(first:1) { nodes { databaseId path line } } } } } } }'

# Resolve each
gh api graphql -f query="mutation { resolveReviewThread(input:{threadId:\"$thread\"}) { thread { id isResolved } } }"
```

**Resolve every thread you replied on** — including rejections (the discussion is closed; the reply explains why). Verify at the end:

```bash
gh api graphql -f query='query { repository(owner:"O", name:"R") { pullRequest(number:N) { reviewThreads(first:30) { nodes { id isResolved } } } } }' | jq '[.data.repository.pullRequest.reviewThreads.nodes[] | select(.isResolved == false)] | length'
```
Expect `0`.

## 6. Re-request Copilot review

After all replies + resolves + force-push, re-request:
```
mcp__github__request_copilot_review(owner=…, repo=…, pullNumber=…)
```
Copilot reviews the new commit's diff and may flag:
- Same-class issues you missed elsewhere in the file (it's worth scanning the affected file for each accepted pattern before pushing — saves a round).
- New issues introduced by the fix itself (e.g. a comment block that contradicts the new behavior — happened in chunk-4 round 2).
- **Zero new comments** = Copilot is done from its perspective. Move to human review.

## 7. Loop until both Copilot and humans are done

Each iteration: triage → discuss → fix → gate → amend → force-push → reply → resolve → re-request. Convergence is normal — most PRs need 1-3 rounds. Don't get fatigued; the discipline is what keeps the review-comment vs. force-push history clean.

## Quick reference

| Step | Tool/Command | Fix policy |
|---|---|---|
| Watch PR | `gh pr checks`, `gh api .../comments`, `gh api .../reviews` | Surface as soon as Copilot comments, CI fails, or both CI + Copilot are done |
| CI fail | `gh pr checks`, `gh run view --log-failed` | Fix own, fix obvious pre-existing, ask about unclear |
| Triage comments | `gh api .../pulls/<PR>/comments` | **Default reject**; accept only real bugs/issues or factually wrong text. Discuss verdicts with user before acting |
| Re-run gates | Follow Step 1-5 in `skills/make_pr.md` | Full rerun after every amend |
| Amend/push | `git commit --amend --no-edit`, `git push --force-with-lease` | Keep squashed branch squashed |
| Reply | `mcp__github__add_reply_to_pull_request_comment` | Every addressed comment gets a reply |
| Resolve | `gh api graphql ... resolveReviewThread` | Every addressed thread gets resolved |
| Re-request | `mcp__github__request_copilot_review` | After replies, resolves, and push |