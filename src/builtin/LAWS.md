# LAWS - src/builtin rulings (append-only intent provenance; see repo CLAUDE.md)

- **2026-08-23** - `REVIEW.md` (created), `REVIEW.das` + `review_nttp.das` (created).
  Boris, on how to decide NTTP flavor per bind: "how about agent per module. that or
  builtin\math\string 100% nttp - if we can't come up with criteria" - ruled for the
  100% option when the bounded-cost criteria were laid out ("sounds good"), "and a
  review.das which can actually check it".

- **2026-09-03** - `REVIEW.md` (serializer rule added). The review round found the checklist
  had no rule for a field added to a type the AST serializer streams; Boris: "lets add now" -
  bump `getVersion()` in the same change (the reader gates on exact version equality, so
  the bump is the whole duty).
