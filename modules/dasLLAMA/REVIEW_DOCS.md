# dasLLAMA Ledgers and Architecture Documents Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md` (the sec.1 routing block and the charter lines its companions hold).
Planned work: `followup_general.md`, `followup_vulkan.md`, `followup_metal.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with it.**

**A diff that adds a `followup_*.md` entry saying a function can be shortened or split - other
than one asking twin bodies onto one template - drops that function's STYLE037/STYLE038
suppression (`// nolint:`, `options _function_length` / `_cyclomatic_complexity`) or lands the
split in the same change; adding such a suppression to a function an entry names is a defect.**

**A diff that adds a file under `dasllama/`, or gives a file there anything its sec.1 charter
line does not cover, keeps the charters true in the same change - in an `ARCHITECTURE_*.md`
companion, never `ARCHITECTURE.md`.** `ARCHITECTURE.md`'s sec.1 routing block names the
companion holding each file's charter line.

**A `followup_*.md` row whose work landed in this change is deleted and every checked-in
citation of it repointed or dropped; no other row is renumbered (text cites rows by number),
and a row listing several items keeps its number and strikes the one that landed.**

**A row a diff adds to a `followup_*.md` takes a number higher than every number that file
carries - a deleted row's number is never reused.**

**A diff that adds, removes, or moves a section of an `ARCHITECTURE_*.md` companion, or adds
or removes a companion, lands `ARCHITECTURE.md`'s index line and section range, the
companion's own opening (its range and the sections it names), and every repointed prose
`sec.N` / file citation of the moved sections, in the same change; a new section takes a
number no other section in this folder's `ARCHITECTURE*.md` set uses.** Prose citations are
not LINT026-gated, so one naming a section that left its file sends the reader to nothing.
