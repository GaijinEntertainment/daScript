# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (this folder's — the suite map, arm lists, and runner mechanics live there).
Planned work: `../THINKING.md` (deferred remote legs).

**Every PR runs the model-free tests here, plus each suite the change reaches — never the whole
directory.** A change reaches a suite through a suite file, a shared helper's behavior, or engine
code the suite exercises; a comment- or prose-only change reaches none. Suites run only through
`run.das`, `--arm`-scoped (it refuses `--full`); dastest run directly on a suite file is a defect.
Every other test, vulkan included, runs under dastest.

**Every test RUN runs under `-jit`** — never the interpreter, never AOT. A compile-only CI lane
passes dastest's `--compile-only`; a model-gated suite run interpreted, with skips standing in
for the guard, is a defect.

**dasLLAMA `[test]` files live here**, except a test whose subject lives beside it under `utils/`
(the serving legs and the exchange client, which require their sibling by bare name) and the one
bench self-check `benchmarks/matmul/test_matmul_par.das`; no dasLLAMA test appears in any
`CMakeLists.txt`.

**A test passes or skips explicitly on every platform.** A skip goes through a capability or
model gate; a test that silently vanishes on one platform is a defect.

**A skip gate keys on a device capability or mode predicate, never on the existence of a runtime
artifact.** An artifact gate goes permanently false when its producer moves.

**A test loading a model over 6 GiB runs only under `DASLLAMA_PARITY_FULL=1`** — a final pre-PR
gate, not the iteration loop. Here the spelling is `model_available` (`_model_tier.das`); a
serving leg, which cannot require this folder's fixtures, open-codes the same gate. Check what a
test loads first.

**A suite loads decoders with `load_model_`, never the image rail** (towers and embedders load
through their family loaders). Image-rail coverage belongs to the image suites alone.

**Every new or changed piece of functionality — a function or a registration, its signature,
body, or wiring — ships its test in the same change: feed the function, check the bytes; "the
model still runs" is not that test.** The folder's sharpenings of `REVIEW_COMMON.md`'s
constitutional rule: a signature widening with an unchanged body counts (the new receivers are
the new bit); a platform-fixed predicate's observable is the argv it gates or the mode it
selects; a moved or edited registration's observable is reachability.

**Every test that compares logits also logs decoded text for both sides.** A red, or a
suspicious green, must be readable as text in the log, not only as an id or float difference.

**A new GPU kernel ships with a small model in the kernel coverage suite** that dispatches it.

**A kernel-unit arm compares its kernel against a CPU oracle.** A GPU-vs-GPU arm is allowed only
for cross-dispatch bit-identity no CPU oracle can witness, and then its output buffers are
prefilled with a sentinel; an in-place output plane instead pairs its compare with a liveness
check.

**A harness that prints output for another tool to compare fails loudly when it has nothing to
print.** A run that ends without its comparison lines — wrong flags, failed load — exits
non-zero.

**Every ASR family has a token-for-token oracle cell, and every oracle cell logs its transcript
as an `eyeball:` line.** An id-only comparison is a defect.

**A real image fixture or mmproj a vision test loads has its `performance/fetch_models.das`
entry.**

**A vision test that needs no model builds its image procedurally and pins its expectations
in-repo; any image a test feeds an embedder is a fixture the test builds, or previewable via
`DASLLAMA_VISION_DUMP`** — a red never requires adding instrumentation before a human can see
what the model saw.

**A tier-1 vision fixture — one an embedder-parity cell regenerates in-test and compares
against an oracle dump — has an exact-value generator.** A generator running libm
transcendentals is not float-portable and its cell is a defect; orientation coverage uses
shaped exact fixtures.

**A vision embedding-parity cell names its fixture and logs the measured maxdiff on green as
well as red.**

**A family that gains a live thinking or tool format ships its recognition tests in the same
change** — the wire-shape pins, the render pins, and a live server leg gated on the family's
smallest GGUF (the file homes are `CLAUDE.md`'s suite map). A family whose vocab lacks the
markers has no format to test; with no small-enough local model the remote leg goes to
`THINKING.md`.

**A change reaching `dasllama_tokenizer.das`, `dasllama_spm.das`, `dasllama_bpe.das`, or
`dasllama_pretok.das` records a `tests/test_tokenizer.das` run with its cases EXECUTED, not
skipped.** A new pre-tokenizer family or backend ships its `corpus_case` arm naming the
`ggml-vocab-*.gguf` fixture; a corpus case asserts exact reference ids AND lossless round-trip.
