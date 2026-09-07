# dasLLAMA Code Review Checklist - the reference build in checked-in text

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with
it.** The reference build is the third-party engine this module measures itself against - the
checkout `benchmarks/setup_lcpp_ref.das` pins.

**Checked-in text under `modules/dasLLAMA/` - docs, comments, and string data, any language -
that is not locating, patching, or reproducing work against the reference build describes an
upstream mechanism in our own terms: no "lifted/ported verbatim from", and no name belonging
to the reference build - symbol, header, constant, binary, project or organization - write
"the reference exe" or "upstream" instead.** A symbol the file carrying that text calls or
holds as a value is its own name, not attribution.

**Prose whose job is to locate, patch, or reproduce work against the reference build names
that build's binaries and symbols outright, and keeps that naming inside the sentences doing
that job.** The job decides, not the artifact kind - a regeneration path, an env-knob row, a
command line in a methodology or how-to document, a ledger row whose subject is a reading of
the reference build (the compared row, the command that reproduces it), and a source patch
applied TO the reference build all qualify. A paragraph that mixes a reading of the reference
build with a proposal of our own keeps them in separate sentences; a row that cites upstream
while proposing our own work is a proposal, not a reading, so it names no upstream symbol.
