# dasLLAMA tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (this folder's). Planned work: `../THINKING.md`.

**Every PR runs the model-free tests here, plus every test here the change reaches — never the
whole directory.** A change reaches a test when it alters what code does at runtime — through
the test file, a shared helper, or engine code the test exercises; an identifier- or
comment-only edit reaches none.

**A test file listed in `run.das` runs only through `run.das`, scoped with `--arm` (the runner
refuses `--full`); dastest invoked directly on such a file is a defect. Every other test in
this folder runs under dastest.**

**Every test RUN runs under `-jit`** — never the interpreter, never AOT. A compile-only CI lane
passes dastest's `--compile-only`; a model-gated suite run interpreted, with skips standing in
for the guard, is a defect.

**A `[test]` file that requires any `dasllama/*` module lives in this folder.** Out-of-folder
instances are ledgered in `CLAUDE.md`'s "Out-of-folder test files" note.

**No dasLLAMA test appears in any `CMakeLists.txt`.**

**A suite-less file's `CLAUDE.md` entry is accurate in the same change** — added when the file
is added, corrected when what it covers is renamed or re-scoped.

**Weakening `test_program_roots.das` — dropping a root from its sweep, loosening its
`options stack = 524288` assert, or relaxing its prefill-intent assert — is a defect.**

**Weakening `test_env_registry.das` is a defect.** It enforces the knob contract: no raw
environment access outside `dasllama_env.das` (declare an `[EnvConfig]` field and read
`g_env_*.<field>`; dynamic names go through `env_is_set` / `env_value_of`), no re-declared
env helper, and a checked-in `ENVIRONMENT.md` matching what the declarations render
(regenerate with `harness/gen_env_doc.das`).

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

**A signature widening with an unchanged body ships its test — the new receivers are the new
bit; a platform-fixed predicate's observable is the argv it gates or the mode it selects; a
moved or edited registration's observable is reachability.** Feed the function and check the
bytes; "the model still runs" is not that test.

**Every test that compares generated tokens, ids, or logits logs the decoded text for both
sides.** A red, or a suspicious green, must be readable as text in the log, not only as an id
or float difference.

**A new GPU kernel ships with a small model in the kernel coverage suite** that dispatches it.

**A kernel-unit arm compares its kernel against a CPU oracle.** A GPU-vs-GPU arm is allowed only
for cross-dispatch bit-identity no CPU oracle can witness, and then its output buffers are
prefilled with a sentinel.

**A kernel-unit arm whose output plane is its input plane pairs its compare with a liveness
check** — an in-place kernel that never ran leaves the input, which can wrongly satisfy a
tolerant compare.

**Every ASR family has a token-for-token oracle cell — one comparing a transcript against a
reference leg, external dump or CPU control alike — and every such cell logs its transcript as
an `eyeball:` line.** An id-only comparison is a defect.

**A real model file, mmproj, or image fixture a test in this folder loads has its
`../performance/fetch_models.das` entry**, or a convert script checked in beside that
manifest in `../performance/`.

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

