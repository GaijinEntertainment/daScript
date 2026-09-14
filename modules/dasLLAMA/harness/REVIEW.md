# dasLLAMA harness Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_MEASUREMENT.md`. Planned work: `../followup_metal.md` for anything about
the Metal backend, `../followup_vulkan.md` for anything about the Vulkan backend,
`../PERF_LEDGER.md` for any other performance followup, `../followup_general.md` for everything
else.

**A diff that adds or moves a timing in `tune_kernels.das` keeps `dot_q8q8_laneq4x4` the LAST
row of the `benches` list, and adds no CPU timing that runs after it.** Running that bench pins one
matmul backend for the rest of the process, so a CPU timing after it runs against the pinned
backend instead of the one it would have picked.

**A diff that points a `dashv` call in this folder at the sidecar exchange - dasllama.io's
tune-sidecar service: sidecar lookup, download, or submit - is a defect; it calls the exchange
through `dasllama/dasllama_exchange` (the module `../dasllama/dasllama_exchange.das` declares)
instead.**

**A diff that declares or changes a `[vk_dispatch]` binding in this folder keeps that binding's
read in a body the binding's own file declares; never set `@role = "alias"` or `"weight"` on a
binding whose only read sits in another file - move the read into the binding's own file
instead.** The lens - the pass that collects a binding's accesses
from its declaring file alone - reports such a binding as never accessed, and `@role` silences
the report.

**A diff that adds a timed row to a file in this folder that prints timed rows, or changes which
kernel an existing row times, ships that row's alternate - a row of the same run, at the same
shape, that the new number is read against - or records in `../ARCHITECTURE_MEASUREMENT.md`
sec.2.5 the reference-engine row it is read against, with the command that produced it.** A number with no alternate beside it ranks nothing.
