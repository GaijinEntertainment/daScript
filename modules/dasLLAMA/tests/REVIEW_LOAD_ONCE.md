# dasLLAMA tests - Load Once Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once`. Planned work: `../followup_general.md`.

- A cell is one `t |> run` subtest, or a `[test]` function that runs no subtest, and owns the
  asserts of every helper it calls.
- An area is a name in `run.das`'s `AREAS` list, standing for the files `run.das`'s `area_tests`
  returns for it.
- A form is one load of a model file under one set of load-time knobs and one load mode.
- A home is a backend a model's weights are served from - the CPU, or a GPU backend
  (`DASLLAMA_GPU`, `DASLLAMA_CUDA`).

**A diff that adds or moves a cell loading a `.gguf` model file loads it on a form of the area's
one `with_cpu_and_gpu_model` call for the file (`_cpu_gpu_model.das`) - adding that call where none
exists - or in a function that calls `own_load("<why>")`.** `check_models_load_once` (`REVIEW.das`)
fails, in an area `LOAD_ONCE_AREAS` lists, a load of a `.gguf` file a string literal names when a
call loads that file or a second load names it; the reviewer checks the loads it cannot read - a
file loaded once in the area with no call, any load in another area, a file name built at run time,
and a load in one helper that several cells call. A load in a child process the cell spawns is
outside this rule.

**A diff that removes an area from `LOAD_ONCE_AREAS` (`REVIEW.das`), removes or disarms
`check_models_load_once`, adds a file to `UNFOLDED`, or adds or moves a model load into a file
`UNFOLDED` lists, is a defect - the load goes on a form of the area's one `with_cpu_and_gpu_model`
call for its file, or into a function that calls `own_load("<why>")`.**

**A diff that adds an `own_load` call states in its `why` one of these reasons, and no other, that
no form of the area's one `with_cpu_and_gpu_model` call for the file can serve: the cell reloads or
shuts down the model; drops the model before its last step, or drops a CPU-loaded model at all (a
cell that drops the device model as its last step runs as the `device_drop_cell` of a device form
marked `drops_device`); writes a `.dlim` image beside the file; changes the loaded model's config in
a way it cannot restore; loads it through a bench or tool; holds several models loaded at once; or
runs on a home the call does not load.**

**A diff that puts into a `with_cpu_and_gpu_model` call a cell whose asserts read the output of one
CPU load and of no other form gives that load as `cpu_cells_form` and runs the cell only where
`form.alone` is true.** Other CPU forms load only on a box where a device form ran; the body runs on
a CPU load once per KV-cache codec the device forms armed, and `form.alone` is true on exactly one
of those runs.

**A diff that compares two forms of a `with_cpu_and_gpu_model` call - two loads of its file under
different load-time knobs or load mode - makes the compare after the call returns, on what the
call's body collected.**
