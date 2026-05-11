# blind-mouse — practitioner manual

Read this BEFORE asking, adding, or curating mouse cards. The hard rule (MOUSE FIRST) lives in `CLAUDE.md`; this file is the operational detail behind it.

## Triggers

| Situation | Action |
|---|---|
| About to research a "how do I X?" / "what's the pattern for Y?" / "why does Z behave this way?" question | `mouse__ask` first |
| Just answered such a question through your own research | `mouse__add` before moving on |
| `mouse__ask` returned cards but none of them address what you actually asked | `mouse__bad(queryId)` immediately, then research and `mouse__add` if the answer's worth caching |
| Wrap-up after a meaningful chunk of work | See `skills/task_wrap_up.md` (review `mouse log --misses` for zero-result asks, `mouse log --review` for unrated hits) |
| Symbol/field/type lookup ("where is X defined?", "all references to Y") | NOT mouse — use daslang MCP (`find_symbol`, `grep_usage`, `find_references`) |
| Categorical convention (gen2 syntax, build flags, formatting) | NOT mouse — `CLAUDE.md` / `skills/*.md` |
| Project state (in-progress branches, who's doing what) | NOT mouse — `git log` / memory |

## Setup

The mouse MCP server is deferred — the call dance is `ToolSearch select:mcp__mouse__<tool>` → invoke. The corpus at `mouse-data/docs/*.md` is git-tracked (shared across machines); `mouse-data/index.db` is gitignored and per-machine. First call to any mouse entry point auto-rebuilds the index via the per-tree git-staleness signature — no manual `rebuild` needed.

## Asking

- **Don't ask the meta-question.** "Should I use mouse?" / "When should I consult mouse?" are not real asks — they're traffic. The decision is encoded in the trigger table above. Skip to the actual question. (Telemetry: the `when-to-consult-mouse` card was the #1 retrieval target by a 2× margin; almost all of those calls were noise.)
- **Free-form natural language is fine.** The retriever ORs words and ranks by BM25 + Jaccard title-similarity. Don't write FTS5 syntax unless you specifically need phrase matching (then pass `rawQuery=true`).
- **One question per call.** Three sub-questions → three asks. They hit different cards; compressing them into one query dilutes BM25 scoring.
- **Plan-mode sweep.** During planning, ask the mouse early and often — design questions, prior-art questions, gotcha-recall, trade-off recall. Each cached answer saves a research detour; each cache miss is one `mouse__add` away from being free next time.
- **Mark no-match (false-positive hits).** The response begins with `query_id: N`. If you scan the returned cards and **none** of them address the question — BM25 matched on shared tokens but the corpus has no real answer yet — call `mouse__bad(queryId=N)` before moving on. That converts a false-positive hit into the same actionable signal as `match_count = 0`. Skip when you're unsure whether a card kind-of helped — only the clear-negative signal carries information; we never mark hits as good (default-positive bias would make `useful=true` ratings noise).

## Adding

Title is the question form, ending with `?`. Body structure:

1. **Lead with the answer in one sentence** — bold the verdict if it's surprising. Don't bury the lede.
2. **Citations** — `file:line` references for compiler/runtime sources, so the next reader can verify against current state.
3. **Practical implication** — when does this matter, what should I do differently?
4. **`## Questions` block** — paraphrasings that should retrieve this card. Add the original ask as the first bullet; add 1–3 alternates that a future agent might phrase differently.
5. **`## See also` block** (optional) — cross-links to related slugs, one per line, slug + 1-line hook.

Frontmatter:
- `slug` — kebab-case, derived from title; MUST match the filename (basename mismatch makes the card unretrievable via `get`).
- `title` — the original question.
- `created` / `last_verified` — both today's date on add (ISO `YYYY-MM-DD`).
- `links` — list of related slugs; mirrors the `## See also` section. Drives the reverse-link footer.

**Dupe-on-add gate.** Jaccard ≥ 0.7 hard-blocks the create. If blocked: `mouse__get` the existing card, decide whether to (a) edit it — add a paraphrase to `## Questions`, extend the body with the new angle — or (b) override with `force=true`. Edit beats override unless you genuinely have a different question with overlapping wording.

**Don't add a card when:**
- The miss is symbol-shaped (`ExprOp3 fields cond left right`, `where is X defined`). Use daslang MCP and don't card the lookup — symbol searches resolve against the live codebase, so a frozen card is worse than the live tool.
- The answer is already in `skills/*.md` or `CLAUDE.md`. Categorical knowledge has one home; doubling content guarantees drift.
- The fact is about in-progress project state. Memory and `git log` are the right channels — those are designed to decay; cards aren't.

## Editing

Cards go stale. Fix in place:
- `Edit` the `.md` directly under `mouse-data/docs/`.
- Bump `last_verified` to today.
- No rebuild needed — the next mouse call detects the change via the staleness signature.

When `mouse__ask` returns a card with `last_verified` more than ~30 days old AND the answer is load-bearing for what you're about to do (not just background context): spot-check before quoting. Quick `find_symbol` on the cited file:line, or a `compile_check` on the snippet, takes seconds and catches drift. If the card is wrong, edit it on the spot.

## See also

- `utils/mouse/OVERVIEW.md` — design vision, FTS5 / staleness internals, dupe-on-add details, schema
- `skills/task_wrap_up.md` — end-of-task curation pass (`mouse log --misses`, un-asked questions you researched the long way)
- `CLAUDE.md` "MOUSE FIRST" — the hard rule itself
