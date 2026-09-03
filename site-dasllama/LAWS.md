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

- **2026-09-01** (`REVIEW.md`, round 2): the dragon showed the per-PR news step covers a
  hand-edit inside the markers too (the generator rewrites the whole span), so that rule
  went with its neighbour; the `dio-` tail hung off a ban the gate replaced and went with
  it; "today" became "before the change" in the weakening rule, same word in all four
  served-tree checklists.

- **2026-09-01** (`REVIEW.md`, round 2 rulings, "all as suggested"): the files/ staging rule
  could never fire (the step copies files/ whole) and a new page was registered nowhere;
  both became one `REVIEW.das` census over the deploy copy lines, the sitemap list and the
  metadata test's page list. The comparative-claim tail and the "not raced" metaphor took
  their plain forms.

- **2026-09-01** (`REVIEW.md`, round 3): the number-source rule leads with its verdict again
  (the round-2 form had become a statement); the dl-* residue names the id subtraction and
  the selector-only definition; the page-census WHY says what each miss costs. The metadata
  test's three page lists became one `PAGES` census the gate reads.

- **2026-09-02** (`REVIEW.md`): Boris ruled the site gets a "stories" page - long-form:
  "this is how we measured, this is what it looks like, these are the acceptance numbers,
  this is where the prompts are from ... and a date and sha of the build. this is when we
  tried, this is what it was on that day" - with llama.cpp named as the yardstick, "not a
  competition". The two `_news` truth rules extend to `_stories`, and a story figure without
  its date and sha is a defect. (Entry re-worded 2026-09-02 on Boris's ruling that the
  project's authorship stays out of the public repo's documents.)
