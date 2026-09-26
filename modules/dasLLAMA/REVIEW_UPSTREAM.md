# dasLLAMA Reference-Build Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `PERF_LEDGER.md` for a measured number,
`followup_metal.md` for the Metal tier, `followup_vulkan.md` for the Vulkan tier,
`followup_general.md` for everything else.

These rules bind checked-in text under `modules/dasLLAMA/` and the commit message or PR body of
a diff to it.

The reference build is any third-party engine, library or runtime whose figure a sentence
compares with this module's own, whatever copy a box holds - a prebuilt release, or a checkout a
setup script of this module pins. A
sentence does reference-build work when it reports a search through that build, a patch to it,
a run of it, a regeneration from it, or a measurement against it - planned or performed; naming
one of its symbols is not by itself such work. A library the build ships is part of it, and
running that library to produce our own artifact is reference-build work. A sentence does this
work even when it also states what our own code or our own run does.

**A diff that names the reference build - its organization, project, binary, library, source
header, symbol, constant, or a label its output prints - outside a sentence doing reference-build
work is a defect: outside such a sentence write "the reference build", or for one of its
binaries a "<role> reference exe" phrase; the third party's own binary name appears only inside
a sentence doing reference-build work.** A name that this module's own code, files or
artifacts carry is its own name, not attribution; attributing our code's shape to the reference
build ("in the reference build's form") names it too - state the shape in our own terms, with
no source named for it.

**A diff that describes what the reference build itself does, outside a sentence doing
reference-build work, describes it in our own terms - what the mechanism does, never which of its
files or symbols does it.**

**A sentence stating a number read from a part of the reference build rather than from a whole
run of it - a time, a rate, or a constant that appears only in the reference build's own code -
names the kernel or symbol that produced it, by an identifier that appears in the reference
build's source or in this module's; a role word ("the GEMVs", "the attention") is not such a
name.** Such a sentence does reference-build work. A constant a weight format's own definition
fixes is the format's, not the reference build's.

**A sentence stating what WE will build or change next carries no number measured or read from
the reference build; that number gets its own sentence.** The unit every rule here judges is
the independent clause - bounded by a period, a semicolon, a colon or a dash.

**A sentence stating a figure from a reference-build run that no checked-in script of this module
produces names that run: its exe and model names, its command line, and any environment settings
the command line does not show.**

**A sentence stating a reference-build run we will make names that run the same way: its exe and
model names, its command line, and any environment settings the command line does not show.** Such
a sentence does reference-build work, not a proposal; a sentence that promises a measurement whose
number can only be had by running the reference build - a row of its table, a ratio against it -
states such a run; a promise whose number this module's own runs alone can produce is not one.
