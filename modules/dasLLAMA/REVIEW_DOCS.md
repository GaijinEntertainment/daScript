# dasLLAMA Documents Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE.md`. Planned work: `followup_general.md`, `followup_vulkan.md`,
`followup_metal.md`.

**A diff that adds a `followup_*.md` entry asking that one function be shortened or split (not
merged with a twin onto one template) drops that function's STYLE037/STYLE038 suppression
(`// nolint:`, `options _function_length` / `_cyclomatic_complexity`) or lands the split in the
same change.**

**Never add a STYLE037/STYLE038 suppression under this folder to a function a `followup_*.md`
entry asks to shorten or split - land the split instead.**

**A diff that adds a file under `dasllama/` adds that file's charter line - the one line saying
what the file holds - to an `ARCHITECTURE_*.md` companion in the same change, never to
`ARCHITECTURE.md`.** `ARCHITECTURE.md#file-charters`'s routing block names the companion holding
each file's charter line.

**A `followup_*.md` row's number never changes and is never reused: a row a diff adds takes a
number higher than every number that file has ever carried, deleted rows included.** Text cites
rows by number.

**A diff that adds, removes, or moves a section of an `ARCHITECTURE_*.md` companion, or adds or
removes a companion, updates, in the same change, the companion's line in `ARCHITECTURE.md`'s
routing block (under `File charters` or `Mechanisms`), the companion's own opening, every sibling
companion's routing sentence that names it, and every citation that names the companion file
alone for a moved or removed section.** LINT026 resolves a `<doc>.md#<anchor>` citation but not
one that names a file alone, so a file-only citation of a section that left its file sends the
reader to nothing.
