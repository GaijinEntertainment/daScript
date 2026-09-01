# LAWS.md - Boris's rulings

Append-only intent provenance for rule-document edits in this folder's documents (the
mechanism: CLAUDE.md sec. "Boris's rulings get a `LAWS.md` sidecar"). Never groomed,
compacted, or cited as rules.

- **2026-09-01** (`REVIEW.md`): after a cookie audit of daslang.io and dasllama.io found
  both sites cookie-free but nothing gating a re-introduction, Boris ruled "lets just land
  review gate" - the checklist keeps only the weakening ban and the allowed-host disclosure
  duty; the check itself lives in the new `REVIEW.das`.

- **2026-09-01** (`REVIEW.md`): the two gate rules took the site checklist's dragon wording -
  the weakening rule re-plained, the disclosure rule naming `ALLOWED_HOSTS` directly.

- **2026-09-01** (`REVIEW.md`): the dragon caught the two gate rules contradicting each
  other - adding a host makes the gate report less, so one diff was both "weakening" (a
  defect) and "adding a host" (permitted with disclosure). The weakening trigger now reads
  "with the same `ALLOWED_HOSTS`", which carves the list change out without an exception
  clause. Applied to all four served-tree checklists.

- **2026-09-01** (`REVIEW.md`): flashlight round after PR 3920, dragon findings on this
  checklist. Boris ruled "okie"/"yes" to: the page definition as the served-`.html`
  property with no file census; the two-trigger `_news` rule and the two-trigger
  `news:begin` rule each split in two; the entailed "implementation that does not exist"
  sentence cut; the number-source rule in plain form with the verdict last; the `dl-*`
  WHY tail without "chrome".

- **2026-09-01** (`REVIEW.md`): two rules retired to automation, ruled "yes" in the flashlight
  round. The regenerated-news duty is now a per-PR step in `extended_checks.yml` (the
  workflows checklist owns its weakening), so the rule went; the page-metadata rule became
  the weakening residue over `test_metadata.py`, which now asserts all four items.

- **2026-09-01** (`REVIEW.md`): flashlight round, lint candidate (c) ruled "yes" - the dl-*
  rule is now `REVIEW.das`'s census (every dl-* name used here must be one the shared sheet
  defines) and the checklist keeps the weakening residue.

- **2026-09-01** (`REVIEW.das` dl-* census, first run): the census found `dl-row--noted`
  emitted by both record renderers and styled nowhere since the ASR commit. Boris ruled
  "okie" to removing the class rather than inventing a style for it or allow-listing it.
