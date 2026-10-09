# dasLLAMA Documents Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`. Planned work: `followup_general.md`, `followup_vulkan.md`,
`followup_metal.md`.

**A diff that adds a `followup_*.md` entry asking that one function be shortened or split (not
merged with a twin onto one template), or adds a STYLE037/STYLE038 suppression (`// nolint:`,
`options _function_length` / `_cyclomatic_complexity`) under this folder to a function such an
entry names, drops the suppression or lands the split in the same change.**

**A diff that adds a file under `dasllama/` adds that file's charter line - the one line saying
what the file holds - to an `ARCHITECTURE_*.md` companion in the same change, never to
`ARCHITECTURE.md`.** `ARCHITECTURE.md#file-charters`'s routing block names the companion holding
each file's charter line.

**A `followup_*.md` row's number never changes and is never reused: a row a diff adds takes a
number higher than every number that file has carried on the base branch or in the diff's own
commits, deleted rows included.** Text cites rows by number.

**A diff that changes the problem a `followup_*.md` row states deletes the row, repoints every
checked-in citation that names it to the new row's number or drops it, and adds the new problem
under a new number.** A row rewritten under its number turns every citation of that number into a
citation of something else.

**A diff files planned work in the ledger that owns it: `followup_metal.md` for work that would
change a Metal kernel, driver or dispatch and no Vulkan one, or CPU engine work whose problem
reproduces only on macOS; `followup_vulkan.md` for work that would change a Vulkan kernel, driver
or dispatch and no Metal one; `followup_general.md` for everything else, work on both backends and
rig and instrument rows included.**

**A measured figure a diff records lands in `PERF_LEDGER.md`; a `followup_*.md` row that carries
one cites the entry it comes from.**

**A diff that adds, removes, or moves a `##` or `###` heading of an `ARCHITECTURE_*.md` companion,
or adds or removes a companion, updates every text that lists a companion's contents - the
companion's line in `ARCHITECTURE.md`'s routing block (under `File charters` or `Mechanisms`), the
companion's own opening, a sibling companion's routing sentence that names it - so that it names
each added heading's subject and no removed one's, in the same change.**

**A diff that moves or removes a heading of an `ARCHITECTURE_*.md` companion leaves no citation
that names the companion file alone for that heading's subject - it repoints the citation to the
heading's new home as `<doc>.md#<anchor>`, or drops it.** LINT026 checks a `<doc>.md#<anchor>`
citation, never one that names a file alone.
