# dasLLAMA Documents Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`. Planned work: `followup_general.md`, `followup_vulkan.md`,
`followup_metal.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with it.**

**A diff that adds a `followup_*.md` entry asking that one function be shortened or split - not
that its body merge with a twin's onto one template - drops that function's STYLE037/STYLE038
suppression (`// nolint:`, `options _function_length` / `_cyclomatic_complexity`) or lands the
split in the same change.**

**A diff that adds a STYLE037/STYLE038 suppression anywhere under this folder to a function a
`followup_*.md` entry asks shortened or split - not merged with a twin's body onto one template -
is a defect - land the split instead.**

**A diff that adds a file under `dasllama/` adds that file's charter line - the one line saying
what the file holds - to an `ARCHITECTURE_*.md` companion in the same change, never to
`ARCHITECTURE.md`.** `ARCHITECTURE.md`'s sec.1 routing block names the companion holding each
file's charter line.

**A `followup_*.md` row's number never changes and is never reused: a row a diff adds takes a
number higher than every number that file has ever carried, deleted rows included; a row keeps
its number while its text narrows to what remains of its own work.** Text cites rows by number.

**A diff that adds, removes, or moves a section of an `ARCHITECTURE_*.md` companion, or adds
or removes a companion, lands, in the same change, every text that states the companion's
section range or lists its sections - `ARCHITECTURE.md`'s index line, the companion's own
opening, a sibling companion's routing sentence - and every prose `sec.N` / file citation of an
added, moved, or removed section.** Prose citations are not checked by LINT026 (the lint that
resolves `[arch]` citation targets), so one naming a section that left its file sends the reader
to nothing.

**A section a diff adds to an `ARCHITECTURE*.md` in this folder takes a number no other section
in the folder's `ARCHITECTURE*.md` set carries.** A number two files carry sends a bare `sec.N`
citation to two places.
