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

## 2026-08-31 - skills/tune.md (shipped defaults profiles, DAS_TUNE_POLICY=reference)

Boris, on the tune mint being the workflow blocker (20+ minutes per app per box): "we prune the
families. we save M1/zen2 defaults. and if winners are available (vnni), some magic M5 stuff ...
then there is a race. if nothing available - no race." Profiles are keyed by CPU feature class
("profile classes key by the feature set that gates seats" - he asked to elaborate and accepted
it), and separate CPU from GPU: "do u want to separate CPU and GPU tunes?" - resolved as
kernels-only profiles, runtime knobs never travel. He will remove the dasllama.io tune service
if the new-silicon race drops to a couple of minutes.
## 2026-09-01 - skills/tune.md, the `[tune_companion]` section

The gemv companion gets its own manifest entry, raced among the tile winner's same-layout rows, top two by tile time
only: "i don't see the point of racing anything but top 2"; "im fine with more mints. performance matters more";
"we split x64-amx from x86-vnni512 for sure".
