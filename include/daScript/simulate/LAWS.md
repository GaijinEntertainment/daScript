# LAWS - include/daScript/simulate rulings (append-only intent provenance; see repo CLAUDE.md)

- **2026-09-03** - `REVIEW.md` (consumer list, pin rule). The review round found the
  per-consumer rule omitted the AST serializer and the pin rule never required a pin for a
  newly added field; Boris: "same" (as "lets add now") - the serializer joins the consumer
  list, and a layout change adds an `offsetof` pin for each field it adds.
