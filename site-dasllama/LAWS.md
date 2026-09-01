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
