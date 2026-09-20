# dasLLAMA Reference-Build Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `followup_general.md`, `followup_vulkan.md`,
`followup_metal.md`.

These rules bind checked-in text under `modules/dasLLAMA/` and the commit message or PR body of
a diff to it.

The reference build is the third-party engine this module measures itself against, whatever
copy a box holds - a prebuilt release or the checkout `benchmarks/setup_lcpp_ref.das` pins. A
sentence does reference-build work when it reports a search through that build, a patch to it,
a run of it, a regeneration from it, or a measurement against it - planned or performed; naming
one of its symbols is not by itself such work. A library the build ships is part of it, and
running that library to mint our own artifact is reference-build work.

**A diff that names the reference build - its organization, project, binary, header, symbol or
constant - outside a sentence doing reference-build work is a defect: write "the reference
build", or for its binary "the reference exe", qualified when the module runs more than one
reference exe ("the batched reference exe"), instead.** A name this module's own code, files or
artifacts carry is its own name, not attribution; attributing our code's shape to the reference
build ("in the reference build's form") names it too.

**A diff that describes what the reference build itself does, outside a sentence doing
reference-build work, describes it in our own terms - what the mechanism does, never which of its
files or symbols does it.** A sentence about our own code's shape is not such a description; a
reference-build name inside such a sentence is a defect just the same.

**A sentence stating a number read from a part of the reference build rather than from a whole
run of it - a time, a rate, or a constant that appears only in the reference build's own code -
names the kernel or symbol that produced it, by an identifier that appears in the reference
build's source or in this module's; a role word ("the GEMVs", "the attention") is not such a
name.** Such a sentence does reference-build work. A constant a weight format's own definition
fixes is the format's, not the reference build's.

**A sentence stating what WE will build or change next carries no reading of the reference
build; the reading it sits beside gets its own sentence.**

**A sentence stating a reference-build run we will make carries that run's names, knobs and
commands.** Such a sentence does reference-build work, not a proposal; a sentence that promises a
measurement we have not made - a table row, a ratio, or a comparison against the reference - states
such a run.
