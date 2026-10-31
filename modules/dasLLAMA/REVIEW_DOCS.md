# dasLLAMA Documents Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md`, `ARCHITECTURE_DOCS.md`. Planned work: `followup_general.md`,
`followup_vulkan.md`, `followup_metal.md`.

**A diff that adds a `followup_*.md` entry asking that a function carrying a STYLE037/STYLE038
suppression (`// nolint:`, `options _function_length` / `_cyclomatic_complexity`) be shortened or
split (an entry asking to replace its repeated, hand-copied code with a loop counts; one asking to
merge it with a twin onto one template does not) removes that suppression, or lands the shortening
or split, in the same change.**

**A diff that adds a STYLE037/STYLE038 suppression under this folder to a function that carried no
suppression of that STYLE code on the base branch, in any file, where a `followup_*.md` entry
asks that function to be shortened or split (an entry asking to replace its repeated, hand-copied
code with a loop counts; one asking to merge it with a twin onto one template does not), is a
defect - it lands the shortening or split instead.**

**A diff that adds a file under `dasllama/` adds that file's charter line - the one line saying
what the file holds - to an `ARCHITECTURE_*.md` companion in the same change, never to
`ARCHITECTURE.md`.** `ARCHITECTURE.md#file-charters`'s routing block names the companion holding
each file's charter line.

**A `followup_*.md` row's number, and an item's label inside a row, never changes and is never
reused: a row a diff adds takes a number higher than every number its file has carried, and an
item a diff adds takes a label higher than every label its row has carried, on the base branch or
in the diff's own commits, deleted ones included.** Text cites rows and items by number.

**A diff that removes the problem a `followup_*.md` row states deletes the row - or, when the row
lists several items and the diff resolves only some, deletes those items alone and the row keeps
its number - and repoints every checked-in citation of what it deleted to where the resolution now
lives (the architecture doc or the code), or drops the citation.**

**A diff that deletes a `followup_*.md` row or item rewrites every checked-in text that states the
deleted problem as still open - a comment, a doc sentence - to state the current behavior, or
deletes the sentence.**

**A diff that changes the problem a surviving `followup_*.md` row or item states - beyond deleting
what it resolves or adding an item under a new label - deletes it, repoints every checked-in
citation that names it to the new row's number or drops it, and adds the new problem under a new
number.** A row rewritten under its number turns every citation of
that number into a citation of something else.

**A diff files planned work in the ledger that owns it: `followup_metal.md` for work that would
change a Metal kernel, driver or dispatch and no Vulkan or CUDA one, or CPU engine work whose
problem reproduces only on macOS; `followup_vulkan.md` for work that would change a Vulkan kernel,
driver or dispatch, outside every template the CUDA home also stamps, and no Metal or CUDA one;
`followup_general.md` for everything else, rig and instrument rows included.**

**A time, rate, ratio or error a box measured, written by a diff into an `ARCHITECTURE*.md` or a
`followup_*.md`, names each box it holds on, and the `PERF_LEDGER.md` entry it comes from shows it
on each of them; a `followup_*.md` row that carries one cites that entry.** A parity claim - X runs
at Y's time - is a ratio.

**A diff that adds or removes an `ARCHITECTURE_*.md` companion adds or drops its line in
`ARCHITECTURE.md`'s routing block (under `File charters` or `Mechanisms`) in the same change.**

**A diff that adds, removes, or moves a `##` or `###` heading of an `ARCHITECTURE_*.md` companion
updates every text that lists the companion's contents - its own opening, any other
`ARCHITECTURE*.md` sentence that lists its subjects - so that it names each added heading's
subject and no removed one's, in the same change.**

**A diff that moves or removes a heading of an `ARCHITECTURE_*.md` companion leaves no citation
that names the companion file alone for that heading's subject - it repoints the citation to the
heading's new home as `<doc>.md#<anchor>`, or drops it.** LINT026 checks a `<doc>.md#<anchor>`
citation, never one that names a file alone.

**A diff that adds or rewrites text in a `HOW_TO_*.md` under this folder writes each step as an
imperative or a specification of the code to write - never a narrative - citing the
`ARCHITECTURE*.md#<anchor>` section that owns the mechanism the step touches, and never restating
that section's reasoning.**

**A trap a diff writes into a `HOW_TO_*.md` under this folder - a place where the procedure does
not hold - is one sentence at the step it applies to, naming the consequence the author meets.**

**A HOW_TO trap that is a defect of this repository's own code cites that defect's `followup_*.md`
row by number; a diff that writes one with no row files the row in the same change.**

**A sentence a diff adds to a `HOW_TO_*.md` under this folder that tells of an event - a run, a
probe, a fix, who found what - is a defect; git keeps what happened.**
