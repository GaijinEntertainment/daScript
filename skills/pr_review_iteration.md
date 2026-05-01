# PR Review Iteration

Use this skill after the pull request already exists. It covers the post-open loop: watching CI, triaging Copilot/human review comments, discussing verdicts with the user, applying fixes, replying, resolving threads, and re-requesting review.

## 1. Watching the PR

Pick a cadence — either ping the user when something interesting happens, or use `/loop` (without an interval, dynamic mode) to self-pace polls. On each tick:

- `gh pr checks <PR>` — CI status (pending / pass / fail)
- `gh api repos/<owner>/<repo>/pulls/<PR>/comments` — inline review comments
- `gh api repos/<owner>/<repo>/pulls/<PR>/reviews` — review-level state (`COMMENTED` / `CHANGES_REQUESTED` / `APPROVED`)

**Wait for whichever comes first** — Copilot review OR CI completion. Copilot usually returns in ~5 minutes; the full CI matrix takes 20-30 minutes. Don't wait for the slower one when the faster one already gives you something to act on.

Cadence guidance:
- **First poll: ~5 minutes.** Copilot review almost always lands by then. If both Copilot has left `COMMENTED` AND CI has visible progress, surface and stop.
- **If Copilot done, CI still pending:** triage the comments and start fixing while CI grinds on the rest of the matrix. Re-poll CI on amend/force-push.
- **If Copilot still pending after 5 min:** poll again at ~10 min. Rare, but happens on long diffs.

Stop polling and surface as soon as: (a) Copilot has left a `COMMENTED`-state review (something to react to), (b) any CI check has gone red (something to fix), or (c) CI is fully green AND Copilot is done.

## 2. CI failure handling

If a check goes red:
1. Identify the failing job: `gh pr checks <PR>` shows the URL; `gh run view <runID> --log-failed` fetches the log.
2. Apply the same fix policy as Step 2 in `skills/make_pr.md`: own change → fix it; obvious pre-existing → fix it; non-obvious pre-existing → ask the user.
3. After the fix, **re-run the gates from Step 1-5 in `skills/make_pr.md`** (lint + interpreted + AOT + Sphinx if `//!` or RST touched + format) — don't trust spot-checks. CI failures often come bundled (a missing format triggers a lint, a removed function triggers an AOT hash mismatch).
4. Amend the squashed commit + force-push. CI re-runs automatically on push. No need to re-request review for a CI-only fix unless the diff is large.

## 3. Triaging review comments — discuss BEFORE acting

When a review lands (especially Copilot — fast, often verbose), **don't auto-execute**. Pull the comments, classify each one, and surface to the user with a verdict per comment. Only act after the user signs off.

Fetch the comments:
```bash
gh api repos/<owner>/<repo>/pulls/<PR>/comments | jq '.[] | {id, path, line: (.line // .original_line), body}'
```

For each comment, give the user a concise verdict:

| Verdict | Means | Example |
|---|---|---|
| 🔴 **Real bug** | Reviewer found a correctness issue | "take(n) before aggregate emits unbound `LIMIT ?` placeholder" |
| 🟡 **Cosmetic accept** | Wording / consistency / doc-drift fix | "diagnostic says `_distinct()` but public API is `distinct()`" |
| 🔴 **Reject with evidence** | Reviewer is wrong; reply with proof | "RST already matches actual emitter output (paste runtime line)" |

Real-bug verdicts deserve a probe before declaring fix direction — the obvious fix isn't always the right fix. (e.g., aligning the bind-push wasn't enough for the `take(n) |> sum()` bug because the underlying SQL is degenerate; the right answer was compile-time rejection.)

Wait for the user to greenlight before applying. They may push back on individual verdicts ("reject this one too" or "let's not bundle this fix into chunk 4").

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
| Triage comments | `gh api .../pulls/<PR>/comments` | Discuss verdicts with user before acting |
| Re-run gates | Follow Step 1-5 in `skills/make_pr.md` | Full rerun after every amend |
| Amend/push | `git commit --amend --no-edit`, `git push --force-with-lease` | Keep squashed branch squashed |
| Reply | `mcp__github__add_reply_to_pull_request_comment` | Every addressed comment gets a reply |
| Resolve | `gh api graphql ... resolveReviewThread` | Every addressed thread gets resolved |
| Re-request | `mcp__github__request_copilot_review` | After replies, resolves, and push |