# LAWS.md - Boris's rulings

Append-only intent provenance for rule-document edits in this folder's documents (the
mechanism: CLAUDE.md sec. "Boris's rulings get a `LAWS.md` sidecar"). Never groomed,
compacted, or cited as rules.

| Date | Document | The ask |
|---|---|---|
| 2026-08-25 | comment_style_hygiene.md (guide self-review boundary) | "DONT PROPOSE MINOR CHANGES TO THIS ONE" - said while triaging comment findings on the vecmath backend PR; the guide invites findings against itself, and he wants that read narrowly |
| 2026-08-25 | comment_style_hygiene.md (vendored code) | "3rd party libraries go as is" - on the vendored include/vecmath copy: house comment rules do not reach code owned by an upstream project |
| 2026-08-26 | comment_style_hygiene.md (.das kept-set boundary), CLAUDE.md (repo root), install/CLAUDE.md (shipped twin), das_formatting.md | "lets fix. prorposed wording is good" - on the audit finding that the kept-set ban read as opt-out when `force_clean_comments` is opt-in; state the boundary by the mechanism, not by an example list, in every document that carried the old reading |

- 2026-08-31 - `review_md.md`: review-round flashlight item 4. Boris ruled ("yes") to bless a
  charter-carrying `README.md` as an architecture doc rather than splitting one out -
  `site/README.md` is the precedent; both playground checklists keep their pointers.
