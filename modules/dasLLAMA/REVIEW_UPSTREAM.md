# dasLLAMA Reference-Build Naming Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `followup_general.md`, `followup_vulkan.md`,
`followup_metal.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with
it.** The reference build is the third-party engine this module measures itself against, whatever
copy a box holds - a prebuilt release or the checkout `benchmarks/setup_lcpp_ref.das` pins.
A sentence does reference-build work when it reports a search through that build, a patch to it,
a run of it, a regeneration from it, or a measurement against it - planned or performed; naming
one of its symbols is not by itself such work. A library the build ships is part of it, and running that
library to mint our own artifact is reference-build work. The text this list binds is checked-in
text under `modules/dasLLAMA/`, a commit message that changes it, and the body of the PR that
changes it.

**A diff that names the reference build - its organization, project, binary, header, symbol or
constant - outside a sentence doing reference-build work is a defect: write "the reference exe" or
"upstream" instead, and describe its mechanism in our own terms.** A symbol this module's code
calls or holds as a value is its own name, not attribution; attributing our code's shape to
upstream ("in upstream's form") names it too.

**A sentence stating a number read from a part of the reference build rather than from a whole
run of it - a time, a rate, or a constant of its code - names the kernel or symbol that produced
it, by the reference build's name for it or by ours.**

**A sentence stating what WE will build or change next carries no reading of the reference
build; the reading it sits beside gets its own sentence.** A sentence stating a reference-build
run we will make does reference-build work, not such a proposal - an exit criterion stated as a
comparison to the reference is such a run.

**A sentence stating a reference-build run we will make carries that run's names, knobs and
commands.**
