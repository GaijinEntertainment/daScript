# dasLLAMA module instructions

dasLLAMA is the daslang LLM / ASR / vision engine, in-tree at `modules/dasLLAMA/`. **How it is
built and why is the `ARCHITECTURE*.md` set beside this file** (`ARCHITECTURE.md` routes to the
engine, image, GPU, Vulkan, Metal, measurement and media companions) - read the section you
are about to work in before writing code here. The rules binding a diff are `REVIEW*.md`;
`ENVIRONMENT.md` lists every knob; `followup_general.md` / `followup_vulkan.md` are the ledgers;
`PERF_LEDGER.md` is the measured record; `tests/CLAUDE.md` is the test discipline (run suites
ONLY through `tests/run.das`).

## Test workflow

After an edit: `./bin/daslang -jit modules/dasLLAMA/tests/run.das -- --changed` runs the tests
the changed files reach (areas `audio | vision | tts | llm | infra`; `--area <a,b>` names them
directly). A changed file in a model suite (decode | mtp | prefill | matrix | image) reaches no
area - `--changed` names it and runs nothing for it; run that suite with `--arm` scoped to the
change. Before a PR: `--suite model-free`, then `--suite stocked` on a box with models - a PR's
`stocked` run carries no `--exclude`; `--exclude test_ple_modes` is the between-PR iteration
form. `--no-tune` runs any suite but the image suites on the reference kernel bodies
(`DAS_TUNE_POLICY=reference`, the image rail off) - the portable tier's arm. The runner's `REPORT`
and `NEXT` lines say what ran, what the run skipped (a filtered arm, an absent device, or a
missing model), and what is still owed. The rest: `tests/CLAUDE.md`.

Follow the daslang **gen2** conventions - the root `CLAUDE.md` rules apply to every `.das` file
here.

## HOW_TO documents (REQUIRED for the task they name)

A HOW_TO is a procedure: imperative, ordered, each step citing the architecture section that
owns it, validated by execution, with a QUIRKS ledger of every place the pattern broke so a
follow-up arc can unquirk it. Read the one that matches your task before the first edit, and
fix it in the same session when a step turns out wrong.

| Document | Read BEFORE... |
|---|---|
| `HOW_TO_ADD_A_FORMAT.md` | Adding a weight format (a new `KqFmt`): GGUF type -> planes -> CPU kernels -> tune family -> Vulkan -> Metal -> tests |
| `BRINGUP.md` | Bringing a profiling box up from zero (the records rig; `METHODOLOGY.md` is the published method) |
| `HOW_TO_GET_SIDECAR.md` | Taking an untuned box to a committed CPU-class profile (`performance/defaults/<class>.tune-defaults.json`) |

## Skill files (REQUIRED)

| Skill file | Read BEFORE... |
|---|---|
| `skills/tune.md` | Touching any `[tune]` / `[tune_perm]` kernel family or the sidecar |
| `skills/internal/llvm_tune_internals.md` | Editing the tune framework itself |
| `skills/perf_lint.md` / `skills/style_lint.md` | Suppressing any lint finding here |
| `skills/internal/tests_in_repo.md` | Adding a test (the deep-engine rules: `options stack`, `T?`-free helpers) |
| `skills/writing_benchmarks.md` / `skills/internal/benchmarks_in_repo.md` | Anything under `benchmarks/` or `performance/` |
