# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `CLAUDE.md`. Planned work: `../THINKING.md`.

**Every PR runs `run.das -- --suite model-free`, plus every test here the change reaches — never
the whole directory.** A change reaches a test when it alters what code does at runtime — through
the test file, a shared helper, or engine code the test exercises; an identifier- or
comment-only edit reaches none.

**A test file that runs (or skips its model arms) with no model file present is listed in
`run.das`'s `model-free` suite in the same change it is added.**

**A test file in a `run.das` model suite (every suite but `model-free`) runs only through
`run.das`, scoped with `--arm` (the runner refuses `--full`); dastest invoked directly on such
a file is a defect. A `model-free` file runs through the runner or under plain dastest.**

**Every test RUN runs under `-jit`** — never the interpreter, never AOT. A compile-only CI lane
passes dastest's `--compile-only`; a model-gated suite run interpreted, with skips standing in
for the guard, is a defect.

**A `[test]` file that requires any `dasllama/*` module lives in this folder.** Out-of-folder
instances are ledgered in `CLAUDE.md`'s "Out-of-folder test files" note.

**A test file in this folder is registered in no `CMakeLists.txt`.**

**A suite-less file's `CLAUDE.md` entry is accurate in the same change** — added when the file
is added, corrected when what it covers is renamed or re-scoped.

**Weakening `test_program_roots.das` — dropping a root from its sweep, loosening its
`options stack = 524288` assert, or relaxing its prefill-intent assert — is a defect.**

**Weakening `test_env_registry.das` is a defect.** It enforces the knob contract that
`../ENVIRONMENT.md` describes.

**A test passes or skips explicitly on every platform.** A skip goes through a capability or
model gate; a test that silently vanishes on one platform is a defect.

**A skip gate keys on a device capability or mode predicate, never on the existence of a runtime
artifact — a file a build or a previous run produced (a dump, a mint, a generated binary); a
stocked model file is a model gate.** An artifact gate goes permanently false when its producer
moves.

**A test loading a model over 6 GiB runs only under `DASLLAMA_PARITY_FULL=1`** — a final pre-PR
gate, not the iteration loop. Here the spelling is `model_available` (`_model_tier.das`); a
serving leg, which cannot require this folder's fixtures, open-codes the same gate. Check what a
test loads first.

**A suite loads decoders with `load_model_`, never the image rail** (towers and embedders load
through their family loaders). Image-rail coverage belongs to the image suites alone.

**A signature widening with an unchanged body ships its test — the new receivers are the new
bit; a platform-fixed predicate's observable is the argv it gates or the mode it selects; a
moved or edited registration's observable is reachability.** Feed the function and check the
bytes; "the model still runs" is not that test.

**A new pre-tokenizer family or backend ships its `corpus_case` arm in `test_tokenizer.das`,
naming the `ggml-vocab-*.gguf` fixture; a corpus case asserts exact reference ids AND lossless
round-trip.**

**Every test that compares generated tokens, ids, or logits logs the decoded text for BOTH
sides (`log_gen_texts` in `_model_tier.das`, or one line per side).** A red, or a suspicious
green, must be readable as text in the log, not only as an id or float difference.

**A new GPU kernel ships with a small model in the kernel coverage suite** that dispatches it.

**A kernel-unit arm compares its kernel against a CPU oracle.** A GPU-vs-GPU arm is allowed only
for cross-dispatch bit-identity no CPU oracle can witness, and then its output buffers are
prefilled with a sentinel.

**A kernel-unit arm whose output plane is its input plane, and whose CPU oracle does not
differ from the input by construction, pairs its compare with an assert that the output
differs from the input at a known index** — an in-place kernel that never ran leaves the
input, which can wrongly satisfy a tolerant compare.

**Every ASR family has a token-for-token oracle cell** — one comparing a transcript against a
reference leg, external dump or CPU control alike.

**A real model file, mmproj, or image fixture a test in this folder loads has a row in
`../performance/model_specs.das` (or `asr_catalog` in `../performance/profile_common.das`),
rides a row's `companions` list, or has a convert script checked in beside the table in
`../performance/`.**

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
smallest GGUF (the file homes are `CLAUDE.md`'s "Model-free / no-arm tests" and
"Out-of-folder test files" notes). A family whose vocab lacks the markers has no format to
test; with no small-enough local model the remote leg goes to `../THINKING.md`.

