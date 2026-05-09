# blind-mouse — personal Q&A cache

A separate MCP server (and CLI) that stores curated `.md` answers to "how do I X?" / "what's the pattern for Y?" / "why does Z behave this way?" questions and retrieves them via SQLite-FTS5 BM25 ranking. The agent both consumes and curates the corpus.

## Problem

Across sessions, agents repeatedly re-derive the same answers. Existing knowledge channels miss the long tail:

- `CLAUDE.md` and `skills/*.md` cover **categorical** knowledge — rules, tables, conventions. Authored deliberately, not accumulated.
- The daslang MCP (`find_symbol`, `grep_usage`, `find_references`) covers **symbol lookups** in the current codebase.
- `git log` covers **what changed** and **why** in commit prose.
- Memory covers **persistent facts about the user / project** that don't fit the codebase.

None of those covers "the agent figured this out once and lost it." That's blind-mouse's slot.

## Non-goals

- Not a docs replacement. RST tutorials, skill files, and CLAUDE.md remain the source of truth for categorical knowledge.
- Not project memory. Memory is for who/what/why/when about ongoing work; mouse is for technical-fact recall.
- Not a vector store. v0 ships BM25-only. Embeddings are a vNext layer once we know what the corpus actually looks like.
- Not a search engine. The corpus is small, hand-curated, and lives next to the project.

## Mental model

**Atomic docs with cross-refs.** A doc answers one specific question. Related docs link via stable slugs. Aggregating "everything about X" into one mega-doc kills retrieval precision.

**Question aliases live in the doc.** When `add` records a Q&A, the original question becomes the first bullet in a `## Questions` section. Future paraphrased asks that match this section will retrieve the doc — the doc accumulates its own retrieval surface as it gets hit.

**The agent both consumes and curates.** Retrieval is the easy part. The killer feature is the edit-when-wrong loop: agent reads an answer, finds it stale or incomplete, edits the `.md` directly, bumps `last_verified`. No separate maintenance tool — just `Edit`.

## Doc anatomy

```markdown
---
slug: typefunction-return-type
title: How do I declare a [typefunction] return type?
created: 2026-05-08
last_verified: 2026-05-08
links: [typefunction-overview, macro-types]
---

(answer body — code blocks, prose, examples)

## Questions
- How do I declare a [typefunction] return type?
- typefunction return value
- what's the type for a typefunction macro
```

Frontmatter fields: `slug` (stable ID, used for cross-refs), `title` (1-line description), `created` / `last_verified` (ISO dates), `links` (inline list of slugs this doc references).

`## Questions` section: bullet list of natural-language phrasings that should retrieve this doc. Auto-grows over time as different paraphrases hit the same answer (vNext: `confirm` tool to grow it on successful retrieval; v0: edit by hand).

## Operations

| Operation | CLI | MCP tool | Notes |
|---|---|---|---|
| Retrieve | `mouse ask "<q>"` | `mouse__ask` | Top-K BM25 ranked. Words OR-joined. |
| Add Q&A | `mouse add "<q>" --body "..."` | `mouse__add` | Dupe-gated by default; pass `--force` / `force=true` to override. |
| Get doc | `mouse get <slug>` | `mouse__get` | Body + frontmatter + reverse-link footer. |
| Rebuild | `mouse rebuild` | `mouse__rebuild` | Rescans `<root>/docs/`; idempotent. |
| Serve MCP | `mouse serve` | (this _is_ the server) | stdio JSON-RPC. |

`add`'s **dupe-on-add gate** is the corpus-hygiene mechanism. With `force=false` (default), `add` first runs retrieval on the new question and returns the similar docs without writing if any match. The agent decides: extend an existing doc (edit the `.md`) or create a new one (re-call with `force=true`).

## Storage model

The `.md` files under `<root>/docs/` are the **source of truth**. The SQLite index at `<root>/index.db` is rebuildable — `mouse rebuild` repopulates it from disk. Implications:

- The corpus is `git`-friendly. Check it in if you want a shared corpus; `git pull` followed by `mouse rebuild` syncs.
- Hand-edits work. `Edit` an answer, run `mouse rebuild`, the index reflects the change.
- The DB is disposable. Lose it, regenerate it.

The SQLite schema (managed via `[sql_migration]` from `sqlite/sqlite_migrate`):

- `docs` — slug PK, path, title, created, last_verified, body_hash.
- `links` — composite-PK pair `(from_slug, to_slug)` for cross-refs.
- `search_idx` — FTS5 virtual table; per-doc concatenation of title + question aliases + body. BM25 ranks via the `@sql_fts_rank` column.

Rebuild is whole-corpus delete+repopulate — simple, correct, fast for small corpora. Incremental update (re-index only changed `body_hash`) is a vNext optimization once the corpus is large enough that whole-rebuild matters.

## Curation discipline

**Extend an existing doc** when:
- The new question is a paraphrase of one already covered.
- The fix is "add one more bullet to the Questions section" or "clarify one paragraph in the body."

**Create a new doc** when:
- The new question is a different facet that deserves separate retrieval (different slug, separable answer).
- Cross-link the related doc via `links:` if there's relationship.

**Bump `last_verified`** when you confirm an answer is still correct. The displayed date in `mouse get` lets future agents calibrate trust.

**Before deleting or renaming a doc**, check `mouse get <slug>`'s "linked from:" footer. Any incoming links would dangle.

## v0 → vNext backlog

1. **Embeddings**. Replace BM25 with cosine over a small embedding model. Better synonym handling.
2. **LLM rerank.** Top-K BM25 → Haiku rerank → final K. Confidence threshold filters obvious misses.
3. **Two-tier corpus.** `~/.mouse/` (cross-project) + workspace overlay.
4. **Auto-staleness.** Periodic re-verification; flag answers whose external refs no longer resolve.
5. **`confirm(slug, question)`** tool — grow the question alias list automatically after a successful retrieval.
6. **External-ref validators** — given a doc cites a file path / PR / symbol, check it still exists.
7. **Schema validation** — when the corpus is shared via git, a CI step that runs `mouse rebuild` and checks for parse errors.

## What this is testing

The hypothesis: **the curate-on-use loop is more valuable than the retrieval algorithm.** If after a week the corpus is empty, the design failed for reasons no amount of embedding cleverness fixes — the agent never reached for the tool, or never recorded what it learned. If it fills up with garbage, the dupe-on-add gate wasn't strict enough. If it fills up cleanly, the next move is layering on retrieval improvements.

v0 ships the smallest thing that exercises the whole loop end-to-end. Decisions get made from real corpus shape, not architecture diagrams.
