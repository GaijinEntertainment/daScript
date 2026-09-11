# dasLLAMA Reference-Build Naming Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`. Planned work: `followup_general.md`, `followup_vulkan.md`,
`followup_metal.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with
it.** The reference build is the third-party engine this module measures itself against - the
checkout `benchmarks/setup_lcpp_ref.das` pins. Reference-build work is locating a site in that
build, patching it, running it, regenerating from it, or measuring against it - planned or
performed; a library the build ships is part of it, and running that library to mint our own
artifact is reference-build work. The text this list binds is checked-in text under
`modules/dasLLAMA/` and the body of the PR that changes that text.

**A sentence whose job is not reference-build work describes an upstream mechanism in our own
terms: no "lifted/ported verbatim from", and no name belonging to the reference build - symbol,
header, constant, binary, project or organization - write "the reference exe" or "upstream"
instead.** A symbol this module's code calls or holds as a value is its own name, not
attribution.

**A number read from a part of the reference build rather than from a whole run of it - a
time, a rate, or a constant of its code - names the kernel or symbol it was read from, in the
sentence that states it.**

**A sentence stating what WE will build or change next carries no reading of the reference
build and no name belonging to it - symbol, header, constant, binary, project or organization;
the reading it sits beside gets its own sentence.** A sentence stating a
reference-build run we will make is reference-build work, not such a proposal, and carries
that run's names, knobs and commands.
