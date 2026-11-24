# dasLLAMA tests - Load Once Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE_ENGINE_SUPPORT.md#test-load-once`. Planned work: `../followup_general.md`.

Terms this checklist does not define are as `REVIEW.md` (beside this file) defines them.

- A home is a backend a model's weights are served from - the CPU, a discrete GPU backend
  (`DASLLAMA_GPU`, `DASLLAMA_CUDA`), or the Metal home of a das_metal build (`metal_form`).
- A form is one model built from a model file - by its own load, by a facade's own load of the file
  with a second file it pairs it with, or minted from its own copy of a staged read the block
  keeps - under one set of load-time knobs, one load mode and one home.
- A block is the area's one call that loads a model file: a `with_cpu_and_gpu_model` call
  (`_cpu_gpu_model.das`) for a `.gguf` file whose test load returns a chat `Model`, a
  `with_model_once` or `with_model_once_at` call (`_model_once.das`) for any other model file - a
  load returning a type that holds a chat `Model`, a `Translator` or an `AsrModel`, among them.
- A model file is a `.gguf` file, or - outside the llm area - a `.bin` pack.

**A diff that adds or moves a cell loading a model file in the cell's own process loads it on a
form of the area's one block for the file - or of a pair's, as the next rule says - adding that block
where none exists, or in a function that calls `own_load("<why>")`.** A file the engine loads on its
own while loading the file a block names, as a voice's front-end packs, is no such load.
`check_models_load_once` (`REVIEW.das`) cannot read a file loaded once in the area with no block, a
load in an area `LOAD_ONCE_AREAS` does not list, a file name built at run time, or a load in one
helper that several cells call; the reviewer checks those by hand.

**A cell whose asserts read the loaded models of two model files at once runs in the inner of two
nested blocks, or - where the pairing of the two files is the cell's subject - on a form of one
file's block that is a facade's own load of the pair.** A compare made after both blocks return
reads what their bodies collected, not two loaded models.

**A diff that weakens `check_block_rows` (`REVIEW.das`), which holds a block to its own file's row of
a table literal, is a defect.**

**A diff that removes an area from `LOAD_ONCE_AREAS` (`REVIEW.das`), removes or disarms
`check_models_load_once`, or gives the check a list of test files whose findings it holds back, is
a defect - the load goes on a form of the area's one block for its file, or into a function that
calls `own_load("<why>")`.**

**A diff that adds an `own_load` call states in its `why` one of these reasons, and no other, that
no form of the area's one block for the file can serve: the cell reloads or
shuts down the model; drops the model before its last step, or drops a CPU-loaded model at all (a
cell that drops the device model as its last step runs as the `device_drop_cell` of a device form
marked `drops_device`); writes a `.dlim` image beside the file; changes the loaded model's config in
a way it cannot restore; loads it through a bench or tool; holds several models loaded at once;
asserts on the load itself - its time, its allocation, its refusal, or the lane, the image flavor
or the staged planes it lands on; or loads a copy of the file that the cell itself writes.**

**A diff that puts into a `with_cpu_and_gpu_model` call a cell whose asserts read the output of one
CPU load and of no other form gives that load as `cpu_cells_form` and runs the cell only where
`form.alone` is true.**

**A diff that compares two forms of a block - two loads of its file under different load-time
knobs or load mode - makes the compare after the call returns, on what the call's body collected.**
