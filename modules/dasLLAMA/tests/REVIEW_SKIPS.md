# dasLLAMA tests - Skip and Pick Conditions Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../followup_general.md`, `../followup_vulkan.md`,
`../followup_metal.md`.

Terms this checklist does not define are as `REVIEW.md` (beside this file) defines them.

- A box fact is queried directly when the condition reads that fact from its source - the device,
  the build, the environment or runner flags, the run mode, a tune companion, the host toolchain, or
  the stocked file on disk - itself. A condition is judged by what the called function returns, not
  by its name: a value the engine's loader, a planner or a `set_*` call wrote during this run -
  other than a loaded model's `Config` field the loader copies from the file's header - is a value
  the run computed. A family's `*_serves_q8` accessor, read before the load with every lane pin it
  consults unset, queries the box fact of the family's unpinned default lane directly.
- A condition that decides which forms the form list of a block call holds is a skip condition of
  every cell run on a form it can leave out.
- A return guarded by the condition an assert just before it checks, so that it returns only after
  that assert failed, is neither a skip condition nor a condition that picks an assert.
- A model fact is a fact of the model file the cell loads (its architecture or a config field),
  read after the cell has loaded the file or from the file's own metadata in its header - never
  from its name or path.

**A box fact that leaves a form out of a block's form list registers a skipped cell naming the
file, the form and the reason (`skip_cell`, `_model_form.das`): a list of form names leaves it out
through `served_forms`, which registers it; a list of `ModelForm`s calls `skip_cell` where the form is
not pushed - never a form dropped with no cell.** A form dropped with no cell takes its
cells with it: they neither run nor skip.

**A cell's skip condition reads only a box fact queried directly or a model fact. The GPU home the
run armed through the environment (`DASLLAMA_GPU`, `DASLLAMA_CUDA`) is such a box fact: in a file no
cell of which calls `set_gpu_home` a skip reads it through `gpu_want_cuda()` or its negation, and in
a file whose cells pin the home with `set_gpu_home` through the knob's own `g_env_gpu` field
(`g_env_gpu.gpu`, `g_env_gpu.cuda`) - never a home a prior cell pinned with a `set_*` call; a skip on a model fact because the engine lacks
something states in its skip text what the engine lacks.**

**A condition that picks a cell's assert or bar by something other than an input the cell sets
itself (a format, a shape, a loop value), a constant table a file of this folder declares (a row
table among them - a table of per-row inputs the cell's file declares as a constant and the cell
loops over), or a model fact queries a box fact directly.**

**A cell's skip condition, or a condition that picks its assert or bar, never reads a value the run
computed (a margin, a gap, a measured difference, or a plan's or a planner's output - slots seated,
layers placed), never tests a registry a `register_*` call fills, and never tests the presence or
content of a file that the build, a mint or a test run writes (a minted `.dlim`, a generated binary,
a dump a test wrote); a bar computed from the assert's own operands (a tolerance scaled by a
measured magnitude) is not such a pick.** Once the producer is renamed, moved or removed, a
condition on its product skips the cell on every box, and the suite still passes.
