# dasLLAMA module instructions

dasLLAMA is the daslang LLM / ASR / vision engine, in-tree at `modules/dasLLAMA/`. **How it is
built and why is the `ARCHITECTURE*.md` set beside this file** (`ARCHITECTURE.md` routes to the
engine, image, GPU, Vulkan, Metal, measurement and media companions) - read the section you
are about to work in before writing code here. The rules binding a diff are `REVIEW*.md`;
`ENVIRONMENT.md` lists every knob; `TEST_CATEGORIES.md` lists the model categories - the large models a run opens
only when it names them, and the work each one covers; `followup_general.md` / `followup_metal.md` /
`followup_vulkan.md` are the ledgers;
`PERF_LEDGER.md` is the measured record; `tests/CLAUDE.md` is the test discipline (run suites
ONLY through `tests/run.das`).

## Test workflow

Suites run through `tests/run.das` under `-jit`. Pick the smallest run that covers the edit (`R` = `./bin/daslang -jit modules/dasLLAMA/tests/run.das --`):

| When | Run |
|---|---|
| after an edit | `R --changed` - the tests the changed files reach, by file |
| working in one area | `R --area llm` - `audio`, `vision`, `tts`, `translate`, `llm`, `infra`, a comma list |
| a box with no models | `R --suite model-free` - the per-PR gate, a 30 s wall a test |
| the work touches a large model's topic | `R --category <name>` (`TEST_CATEGORIES.md`) - no other run opens a model past 6 GiB, the full suite included |
| one file of a run | add `--only <substr,...>`, or `--exclude <file>` to drop one |
| an arc's final run | the runs above under the full suite (`DASLLAMA_PARITY_FULL=1`), and over the fleet |

Before a PR: `skills/internal/dasllama_testing.md` (repo root) says what the PR runs by its kind.

**A run skips what is unchanged - iterate that way.** A child whose program loaded whole from its compile record
reports the files that passed last time, on this box and in this environment, and runs the rest; an edit re-runs the
children it reaches. Every run does this, the full suite's included, and every run records its passes for the next.
`--rerun` runs every file and records nothing: it is for a defect in the pass records, never for evidence. Do not
delete `.cache/daslang` either - the compile records and the passes live there. The `REPORT` line counts the files
not run - quote the line whole where a run is evidence.

**What a box sets to run faster** (`ENVIRONMENT.md` lists every knob; a set knob is part of a pass's key, the scratch folder aside):

- `DASLLAMA_DISABLE_MODULES=dasVulkan` (or `dasCUDA`, a comma list) leaves a GPU module out of every child and of
  every process a test starts: a box working on one GPU home compiles that home's kernel classes alone. The fleet
  sets it a home; a run under it says so on its `MODULES` line, ahead of its first child.
- `DASLLAMA_SCRATCH_DIR=<folder on a local disk>` is the children's temp folder - for a box whose own temp folder is
  a network volume.
- On Linux a media run's children keep their heap; the runner sets it, nothing to do.

**What the runner prepares ahead of its first child**, each said on one line, because no test's wall admits it: the
test child's exe and the entry program's compile record (`CHILD`), and - on a Metal box, in a run that names no
`DAS_TUNE_MANIFEST`, where the per-box crown file does not match this tree's Metal kernel sources - the crown race
(`BOXRACE`, once a change to those sources).

**How a run is cut.** Ten files go to a child as one program - the whole suite on the model-free suite - (`--batch N`;
`--batch 1` is one child a file): the engine's parse and JIT are paid once a batch, each file in a context of its own.
Outside the full suite a model-free test past 30 s ends the run where it stands (`WALL`). On every run a batch that
ends without every file's verdict - its program did not compile, or it died midway - ends the run too (`LOST`, the
first compiler error on the line): no run starts that batch's files again one child each. `--skip N` resumes at the
child the `RESUME` line names, `--test-wall N` sets the wall for a run (0 = none) and `--stop-file <path>` shares the
stop between a run's boxes. `--suite gpu-kernels` is the Vulkan and
CUDA device kernel files - no model, no wall; the stocked suite runs them first.

**What `--changed` reaches.** A test file itself, a fixture the tests requiring it, an engine module its
`MODULE_WITNESSES` row or - a core module - every model-free `llm` file and one stocked witness a family and home,
and its other areas whole. A changed file in a model suite (decode | mtp | prefill | matrix | image) reaches
nothing - `--changed` names it; run that suite with `--arm` scoped to the change. `--area examples` runs the browser
examples' smoke tests, which no other run plans.

**The fleet** (`harness/pod_fleet.das -- up | sync | run | status | times | down`) is two pods, one a GPU home: the
Vulkan home runs every file but the CUDA-only ones (`CUDA_ONLY_EXCLUDE`), the CUDA home its pass's, each with the
other home's module left out. `run` is the
per-PR run, `run --full` an arc's final one; `down` deletes the pods once the session's runs are done.

**A test that loads a model** loads it once a form in its area, on the area's one block for the file
(`tests/REVIEW_LOAD_ONCE.md`); a model-free test fits the 30 s wall, and `tests/REVIEW_FILES.md` says which tests may
declare a `timeout`. **A test that starts the daslang binary** builds the command with `daslang_argv`
(`tests/_daslang_argv.das`), so the child leaves out the modules the run does.

Other arms of the runner: `--no-tune` runs any suite but the image suites on the reference kernel bodies
(`DAS_TUNE_POLICY=reference`, the image rail off) - the portable tier's arm. `--poison-kv` starts every child's K/V
caches as NaN, so a read of a row no store wrote reds its cell - the run a change to a K/V store or reader cites.
`--reap-images` deletes the images a run's children mint, for a box whose run alone writes its models directory (a
fleet pod's own folder, where `harness/pod_fleet.das` passes it). The runner's `REPORT` and `NEXT` lines say what
ran, what the run skipped (a filtered arm, an absent device, or a missing model), and what is still owed. The rest:
`tests/CLAUDE.md`.

Follow the daslang **gen2** conventions - the root `CLAUDE.md` rules apply to every `.das` file
here.

## HOW_TO documents (REQUIRED for the task they name)

What a `HOW_TO_*.md` is - its steps, traps and tense - is `ARCHITECTURE_DOCS.md#howto-documents`. Read
the one that matches your task before the first edit, and fix it in the same session when a step
turns out wrong.

| Document | Read BEFORE... |
|---|---|
| `HOW_TO_ADD_A_FORMAT.md` | Adding a weight format (a new `KqFmt`), a served form of one, or a backend's (CPU, Vulkan, Metal) kernels for one at a site (a weight plane a backend's serving call reads) that reads it through a converted copy (any other form of its weights that outlives the kernel call or reaches the `.dlim`) |
| `HOW_TO_ADD_ASR.md` | Adding a speech-recognition family (an encoder plus a CTC, transducer or LLM decoder head) |
| `HOW_TO_ADD_FORMAT_TO_TTS.md` | Serving a text-to-speech weight group (a StyleTTS2 recipe group, a Pocket tensor) in another weight format - the mint, the kernel form, the GPU seats' read, and the rig run that sets the recipe |
| `HOW_TO_GET_SIDECAR.md` | Taking an untuned box to a committed CPU-class profile (`performance/defaults/<class>.tune-defaults.json`) |

Read `BRINGUP.md` before bringing a profiling box up from zero (the records rig;
`METHODOLOGY.md` is the published method); it is not a `HOW_TO_*.md`.

## Skill files (REQUIRED)

| Skill file | Read BEFORE... |
|---|---|
| `skills/tune.md` | Touching any `[tune]` / `[tune_perm]` kernel family or the sidecar |
| `skills/internal/llvm_tune_internals.md` | Editing the tune framework itself |
| `skills/perf_lint.md` / `skills/style_lint.md` | Suppressing any lint finding here |
| `skills/internal/tests_in_repo.md` | Adding a test (the deep-engine rules: `options stack`, `T?`-free helpers) |
| `skills/internal/dasllama_testing.md` | Planning a PR's test runs - an intermediate PR of an arc runs its scope report, the arc's final PR runs everything |
| `skills/writing_benchmarks.md` / `skills/internal/benchmarks_in_repo.md` | Anything under `benchmarks/` or `performance/` |
