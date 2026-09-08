# dasLLAMA Upstream-Naming Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_MEASUREMENT.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with
it.** The reference build is the third-party engine this module measures itself against - the
checkout `benchmarks/setup_lcpp_ref.das` pins. The text this list binds is checked-in text
under `modules/dasLLAMA/`; a repo-root `plans/` document a dasLLAMA change writes reaches it
through `plans/REVIEW.md`.

**Text whose job is not work performed ON the reference build or measured AGAINST it -
locating a site in it, patching it, running it, regenerating from it - describes an upstream
mechanism in our own terms: no "lifted/ported verbatim from", and no name belonging to the
reference build - symbol, header, constant, binary, project or organization - write "the
reference exe" or "upstream" instead.** Mirroring the reference build's behavior in our own
code is not that job. A symbol this module's code calls or holds as a value is its own name,
not attribution.

**Text whose job is work performed on the reference build or measured against it names that
build's names - symbol, header, constant, binary, project or organization - outright, and keeps
the naming inside the sentences doing that job.** The job decides, not the artifact kind; a
knob or command needed to reproduce a compared reading stays licensed inside a planned-work row.

**A paragraph that mixes a reading of the reference build with a proposal of our own keeps
them in separate sentences.** The proposal sentence names no upstream name; the reading
sentences beside it name what they read, kernel and file included.
