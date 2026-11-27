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
  `with_cpu_and_gpu_snapshot` call for a snapshot directory whose test load returns one, a
  `with_model_once` or `with_model_once_at` call (`_model_once.das`) for any other model file - a
  load returning a type that holds a chat `Model`, a `Translator` or an `AsrModel`, among them.
- A model file is a `.gguf` file, outside the llm area a `.bin` pack, or a Hugging Face snapshot
  directory a test names by `hf_snapshot_dir("<repo>")`, keyed by its repo string.
- A drafter a cell attaches to a block's model inside that block (`attach_block_drafter`, or a helper
  that calls it) is a knob the cell sets on that form's one load - not a load of a model file and not
  a form of its own.

**A diff that adds, moves or edits a cell loading a model file in the cell's own process loads it on
a form of the area's one block for the file - or of a pair's, as the next rule says - adding that
block where none exists, or in a function that calls `own_load("<why>")`.** A file the engine loads
on its own while loading the file a block names, as a voice's front-end packs, is no such load.
`check_models_load_once` (`REVIEW.das`) does not see three loads, so the reviewer checks them by
hand: a file the area loads once with no block, a load in an area `LOAD_ONCE_AREAS` does not list,
and a load in a test file's own helper that several cells call.

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

**A diff that adds or changes an `own_load` call states in its `why` only these reasons, each one
that no form of the area's one block for the file can serve: the cell reloads or shuts down the model;
drops the model before its last step, or drops a CPU-loaded model at all (a cell that drops the
device model as its last step runs as the `device_drop_cell` of a device form marked
`drops_device`); writes a `.dlim` image beside the file; changes the loaded model's config in a way
it cannot restore; loads it through a bench or tool; holds several models loaded at once; asserts on
the load itself - its time, its allocation, its refusal, or the lane, the image flavor or the staged
planes it lands on; or loads a copy of the file that the cell itself writes.**

**An `own_load` `why` a diff adds or changes spells each `;`-separated clause as one reason of the
list above in the list's words, optionally followed by `: <gloss>`; a clause in other words is a
defect.**

**A diff that puts into a `with_cpu_and_gpu_model` or `with_cpu_and_gpu_snapshot` call a cell whose
asserts read the output of one CPU load and of no other form gives that load as `cpu_cells_form` and
runs the cell only where `form.alone` is true.**

**A diff that compares two forms of a block - two loads of its file under different load-time
knobs or load mode - makes the compare after the call returns, on what the call's body collected.**

**A diff that adds or changes a TTS block's form loader (`styletts2_form`, `pocket_form`,
`_tts_parity.das`) or its form check (`styletts2_form_is`, `pocket_form_is`) keeps the check
asserting, on every form, how the form came by its image (`tts_image_origin`,
`../dasllama/dasllama_tts_blocks.das`) - an image form a `.dlim` the image rail mapped wherever the
run's rail maps one, a minted form an image built in memory - and, on a Pocket form, the lane tag
that image was minted under.** The model's lane flags cannot tell a minted form from a mapped one,
nor a pin to the file's own lane from no pin - only the image's origin and the mint's lane tag can.

**A cell asserting on a TTS or tower (a media encoder) model's CPU run - its weight layout, a
synthesis, an encode or a decode - and asserting nothing about which image a load picked, loads a
model holding its host weights: any load under `with_cpu_flavor`, a family's `stage_*` staging
minted in memory, or `pocket_minted(path, PkMint.cpu)` / `pocket_minted(path, PkMint.both)` - never
a load outside `with_cpu_flavor` that can map an image, and never a `stage_*_served` staging after
`set_serving_gpu_tower(false, family)` (`_gpu_knobs.das`).** The runner runs a batch's files in one
process, so an unpinned load maps the device image whenever another file of the batch registered a
GPU driver, and that image holds no host weights for the CPU run.

**A diff after which a `[test]` in a file of any `run.das` suite loads outside a block a stocked
model file of a family it did not load before - a `.gguf` carrier, its shards or an mmproj, or
outside the `llm` area a `.bin` pack of a family `CLAUDE.md`'s family filter list holds a token
for - calls in that `[test]` `family_on(t, name)` (`_model_tier.das`) once for each family it
loads, each call ahead of that family's first load and the test returning where any call returns
false.** An untagged test runs under every `--family` filter, each load of another family's file
skipping inside it by that file's row; a test that returns on a false call runs only under a filter
naming every family it loads.

**A `family_on(t, name)` call a diff adds or changes passes the token `CLAUDE.md`'s family filter
list holds for the family whose file the load the call gates opens.**

**A diff that weakens `check_family_tags` (`REVIEW.das`), which holds a block of a file no
`model_families()` row holds to a `family_on` call, or adds to its `FAMILY_FREE` list a file a
family's loader reads, is a defect - tag the block with `family_on` or give the file a
`model_families()` row.**

**A diff after which a `[test]` loads a `.gguf` carrier, its shards or an mmproj of a family
`CLAUDE.md`'s family filter list holds no token for adds the family's entry to that list in the same
change.**

**A diff that makes a test load a language-model `.gguf`, or a speech, voice-activity or
translation carrier, whose name no `model_families()` row (`_model_categories.das`) holds adds the
row in the same change, under the family's token in `CLAUDE.md`'s family filter list; a tower, a
fixture, and a file that cells tagged with another family also load get no row.**
`model_available` skips another family's file by that row; a file with no row loads under every
`--family` filter.
