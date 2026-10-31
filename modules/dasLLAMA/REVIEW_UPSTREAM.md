# dasLLAMA Reference-Build Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `PERF_LEDGER.md` for a measured number,
`followup_metal.md` for the Metal tier, `followup_vulkan.md` for the Vulkan tier,
`followup_general.md` for everything else.

These rules bind checked-in text wherever a diff puts it, and that diff's commit message and PR
body. The unit every rule here judges is the independent clause - bounded by a period, a
semicolon, a colon or a dash.

The reference build is any third-party engine, library or runtime whose figure any checked-in
text of this module compares with this module's own, whatever copy a box holds - a prebuilt
release, or a checkout a setup script of this module pins. A library the build's release or
source tree bundles is part of it; a package an environment installs beside it is not. A model's
name, and a family this module names after its model, name the model, not the reference build. A
sentence does reference-build work when it reports a search through that build, a patch to it, a
run of it, a regeneration from it, or a measurement against it - planned or performed - even when
it also states what our own code or our own run does. A sentence saying what the build does inside a run
it reports, and a licence or attribution notice the third party's licence requires, do
reference-build work. A sentence reporting a run of a library the build bundles to produce our
own artifact does reference-build work; naming one of the build's symbols is not by itself such
work. A sentence stating a model file's provenance - which of the build's tools a third party ran
to make it - does reference-build work.

**A diff that names the reference build - its organization, project, binary, library, source
header, symbol, constant, or a label its output prints - outside a sentence doing reference-build
work is a defect: outside such a sentence write "the reference build", or for one of its
binaries a "<role> reference exe" phrase; the third party's own binary name appears only inside
a sentence doing reference-build work.** A name this module defines, a GGUF tensor type's name
bare or with its `GGML_TYPE_` prefix, or the filename of a file it loads does not name the
reference build.

**A diff that describes what the reference build itself does, or attributes our code's shape to
it ("in the reference build's form"), outside a sentence doing reference-build work, states the
mechanism in our own terms - what it does, never which of the build's files or symbols does it,
and with no source named for our shape.**

**A sentence stating a per-op or profiler figure of the reference build - a time or a rate one of
its kernels took, rather than one of a whole run - or a constant that appears only in the
reference build's own code names the kernel or symbol that produced it, by an identifier that
appears in the reference build's source or in this module's; a role word ("the GEMVs", "the
attention") is not such a name.** Such a sentence does reference-build work. A constant a weight
format's own definition fixes is the format's, not the reference build's.

**A sentence stating what WE will build or change next carries no number measured or read from
the reference build; that number gets its own sentence.**

**A sentence promising a run of the reference build - or a number only such a run can give, a row
of its table or a ratio against it - names that run: exe and model names, command line, and
environment settings the command line does not show.** A citation of an architecture section that
states that run's recipe in full - no placeholder in its exe, model or command line - names it.
