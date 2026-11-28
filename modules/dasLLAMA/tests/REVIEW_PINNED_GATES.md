# dasLLAMA tests - Pinned Gates Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `../ARCHITECTURE.md`, `../ARCHITECTURE_CELL_BARS.md`. Planned work: `../followup_general.md`.

**A diff that touches a cell an entry of `REVIEW_PINNED_KERNEL_GATES.md` (beside this file) pins - the entries of the
`test_*_kernels*.das` files - applies every rule here to that entry as to an entry below.**

Terms this checklist does not define are as `REVIEW.md` (beside this file) defines them. A file entry opens on a file
alone (`` `<file>` must hold ``) and pins every cell of that file. A named-cell entry opens on a `[test]` function, or
on a cell function a block's `[test]` runs (`` `<file>`'s `<function>` cell must hold ``), and pins every cell that
function runs, every function of the block that computes a side of a compare on what that function kept, and every
verdict - a function run after the block returns - that asserts on it. The pinned set is the entries below and those of
`REVIEW_PINNED_KERNEL_GATES.md`. A pinned test cell is a cell an entry pins, in the set before the diff or in the set
after it; the pinned set alone decides whether a cell is one. A bar is the tolerance a compare accepts; a bar on a
compare whose two sides the cell computes is never a value an outside source can change, whatever model file it was
measured on. This file's part of the pinned set, one entry a bullet, each with what it must keep holding:

- `test_run_suites.das` must hold the per-PR split (`model-free`, `gpu-kernels`, `stocked`), the folder census (every
  test file in a suite but the census verdict file, `CENSUS_VERDICT`, which sits in none), the area tables (each
  standing area over the per-PR census and the `image` and `image-vulkan` suites, the on-demand `examples` area the
  `examples` suite's files and none of it, every image arm literal reached by the arms of exactly one area), the
  argument contracts (`--area` beside `--arm` runs, `--changed` beside `--arm` is refused), the `--exclude` and `--only`
  filters, the `--batch` defaults (`batch_files`), the batch dry run (a batch child's `START` line carrying
  `--together`, `--batch 1` one `--test` child a file), the test wall (`wall_seconds_in`, `log_wall_line`, a run's
  `LOST` stop at a batch without every verdict, wall or none, no file of it run alone, the `--skip` resume), the reach
  plan behind `--changed` (which test files each kind of changed path runs, an on-demand area's file for its own change
  alone), the children's module-cache folder, the model files a run names that its models directory lacks (the
  `MISSING` lines, the bare `.gguf` suffix no file's name), the image reap behind `--reap-images` and its gate, the K/V
  start `--poison-kv` hands the children, the `options persistent_heap` line of the `stocked` files, the model
  categories and families, the `--category` run, a child's `--run-env` filters, the forced tune file, the skip of what
  passed (`pass_stamp`'s inputs, the environment among them; the flag and stamp every run's children carry but a
  `--rerun`'s, the full suite's keyed apart; the `REPORT` count of files not run), a media run's heap setting, the
  children's scratch folder, the `--disable-module` flags each child of a `DASLLAMA_DISABLE_MODULES` run carries and the
  pass stamp they key apart, the crown race's gate (`box_race_possible`, `box_runtime_owed`, `box_manifest_pick`, the
  `BOXRACE` lines), every stocked model past the large tier a category's, and the fleet's Vulkan-free slice of the
  model-free suite (`VULKAN_FREE_ONLY`) and the CUDA-only files a run drops with dasCUDA left out (`CUDA_ONLY_EXCLUDE`).
- `test_run_suites.das`'s `test_child_chunks` cell must hold the `--batch` chunking (`child_chunks`: `per` files a
  child in order; an area run's image file, a `cant_` / `failed_` / `invalid_` file and a tier contract file alone, the
  files around it in one batch).
- `test_run_suites.das`'s `test_census_whole_run`, `test_census_whole_dry_run`, `test_census_verdict_reads`,
  `test_reach_for_path` and `test_metal_witnesses_hold_metal_cells_files` cells must hold the census verdict's whole
  run (`census_whole`, the first two cells: every standing area, no narrowing flag, no family filter inherited from the
  environment, and the verdict child's `DASLLAMA_KERNEL_CENSUS_WHOLE=1` only after such a run reported no file from an
  earlier pass and failed none) and its read (the third: a skip where no census folder is named, a failure where a
  named folder holds no file of the home, the merged table and the whole flag reaching the verdict), every
  `MODULE_WITNESSES` row and `LLM_CORE_WITNESSES` naming only census files and every witness row's module existing
  with a `MODULE_AREAS` row that holds `llm` (the fourth), and every llm file that requires a Metal cells fixture in
  the witness row of each Metal driver module (the fifth).
- `test_facade_docs.das`'s `test_program_roots` cell must hold the `ROOT_DIRS` sweep, `options stack = 524288` and the
  prefill intent.
- `test_env_registry.das` must hold the `../ENVIRONMENT.md` knob contract.
- `test_model_specs.das` must hold `../performance/model_specs.das`'s model-set table, `spec_by_file` answering by the
  models-dir path and by the gguf's base name (a split-shard row sits in a subfolder), the fixture compare
  (`prefix_match_len` / `fixture_ok`: a flipped last id red under the strict form and held under the half-span one, a
  flip inside the first half red under both), `fetch_models --convert`'s route (`convert_route`) and quantize mint
  steps (`mint_steps`; a minted row names its fetched source, and a published one pins bytes and sha256, as
  `CLAUDE.md`'s entry states), the board fetch (`board_provenance`) holding every file the board sweeps read and no
  off-board row and no on-box parity oracle (`tts_oracle*`), and the companion contract: a companion two rows pin
  (each row's cell consumes it) carries one provenance - url, bytes, sha256 - on both, and the provenance view holds
  it once; every sha-pinned provenance url at a revision, never `resolve/main`;
  and every provenance entry past `MODEL_TIER_CEILING_BYTES` a category's (`test_large_rows_have_a_category`).
- `uncategorized_persistent_heap_stack_524288.das`'s `test_float_a_gate` cell must hold the MSL emitter's refusal of a
  float A operand (the activation input) to `tmm2d_q8u_f32` in a kernel that lacks `[metal_kernel(float_a_ok=true)]`,
  the error naming the FLOAT A operand, and the same kernel compiling under that annotation.
- `test_rope.das` must hold `../dasllama/dasllama_rope.das`'s table builders (`build_rope_tabs` and its packed, rows,
  imrope, mrope and vision forms) against their per-element references and against each other bit for bit,
  `mrope_span_positions`' map and its refusal of a grid that does not match the span, `rope_factor_tensor`'s pick, and
  `../dasllama/dasllama_math.das`'s `rope`, at the bar each cell sets.
- `test_rope_apply.das` must hold `../dasllama/dasllama_math.das`'s rope application (`rope_scaled_*`, `rope_apply`)
  and `yarn_rope_freqs` against their fp64 references, each tabled form against its un-tabled twin, and
  `build_rope_tabs` over 120000 rows and over none, at the bar each cell sets.
- `_metal_prefill_cells.das`'s `short_window_metal` cell and its verdict `short_verdict` must hold, on every
  `PrefillRow` whose `short_form` is `ShortForm.kq_tail` (the `kq-tail` rows), every stocked K-quant, grid and per-32
  short window's served form - the row-batched GEMV peel past the whole tiles and the split stamps of a window under
  one tile, the 16-row split stamps on a window of 16 rows or fewer - against the CPU control at its bar.
- `test_site_records.das` must hold the byte-compare of `site/files/dasllama/bench_records.json` and of
  `site/files/dasllama/bench_cells.json` (repo root) - the smaller copy the site page loads first - against a fresh
  `merge_site_records` run, and `bench_cells.json` keeping every field that page renders at load while dropping the
  fields it reads from `bench_records.json` only on demand; and `test_board_projection`'s filters - an older release's
  row, a retired board's row, a held row and an unpaired reference kept off the board, and the stored audio outlier
  voided on the projected row.
- `test_exchange_schema.das` must hold the exchange validator's corpus sweeps and the `[tune_scope]` wire-key pin read
  out of `../dasllama/dasllama_tune_scope.das`.
- `test_bench_records_schema.das` must hold the `write_bench_records` output, corpus sweeps included; the
  llama-batched-bench table parse - which cell at which `npl`, 0 on every refusal; the batched reference row's command
  line - the engine's extra flags follow `-npl N -fa on`, and a run that prints no row names why and reads 0; the
  audio outlier rule and the recorder's void pass (`audio_outlier_clips`, `void_outlier_clips`); the `void_clips` and
  `held` annotations reaching only the run they select; and the committed stores' batched receipts - the cpu legs' das
  child at `--npl-plen 128 --npl-reps 3` and reference at `-npp 128`, `--no-op-offload` on the stock cpu arm alone,
  every GPU leg's reference at `-npp 512` with neither cpu-only flag.
- `_scheduler_cells.das`'s `test_scheduler_media_splice` cell (on `test_gpu_resident_llama.das`'s SmolLM2 block) must
  hold a media span's text and rows as one prefill call, and a body cut at the chunk taking the rest a chunk a call;
  on a paged scheduler, the prefix cache matching across a span by its rows' content - the text ahead of a new span
  attaching and the hit stopping at it, the media stream donating its pages, the same span asked again attaching past
  it, a rowless request for it served off the cache, other rows at the same place sharing only the text ahead of them -
  and a rowless request on a cold cache finishing `media_lost` with nothing donated.
- `test_gpu_tier.das`'s `test_home_words` cell must hold `rdec_fallback_words` (the text naming what serves a model
  the whole-model driver declined) in two cases. It must read as the per-op rails with no CUDA ask, and also under the
  CUDA ask on a build without dasCUDA. It must read as the CPU rails under `DASLLAMA_CUDA=1` on a dasCUDA build. The
  cell must also hold that each discrete home's dry bake (the lane's plan built without calling the device) refuses a
  config whose home is the other one, with a message naming the config's home.
- `test_gpu_tier.das`'s `test_override_announces` cell must hold every override line the GPU homes announce - its
  setter and env spellings - in step with the knob list (`../ENVIRONMENT.md`), and no line where the override takes no
  effect.
- `test_tts_pocket.das`'s `test_pocket_seat_stats` cell must hold `pocket_gpu_seats()`'s stage names in dispatch
  order, and `pocket_gpu_stats` panicking on a name it does not list.
- `test_tts_pocket.das`'s `test_pocket_q8_file` cell must hold the published file's tensor formats against the f16
  file's load-time quants.
- `test_tts_pocket.das`'s `test_pocket_parity` cell (on the file's pocket-tts-en block) must hold, on the f32 CPU lane
  against the English oracle's dumps (`oracle_dir()`), the codec decoder at `CODEC_BAR`, and on the first two voiced
  cases the prompt trim at `TRIM_BAR` - a clip with one kept sample moved landing outside it - the codec encoder at
  `ENCODER_BAR` with the trimmed clip's frame count, and the voice state's K/V rows at `FRAME_BAR` with the prompt the
  BOS row plus the frames.
- `test_tts_pocket.das`'s `test_pocket_kq_file` cell must hold the small file's load-time tensor formats on each lane
  (unpinned: the backbone's and the codec transformers' matrices as Q4_K planes, the flow head as Q8_0 blocks, the
  speaker projection f32, the embedding table its own Q4_K planes, a prompt's rows off them the dequantized table's
  bit for bit, the codec's f16 convs held as half floats where the build's CPU converts half floats in hardware (the
  answer `reset_rows_f32` returns to), as floats elsewhere, never both copies; pinned q8: Q8_0 blocks, the codec convs
  floats; pinned f32: f32 planes, the head included); the kq lane against the q8 lane of the same file under
  `Q8_FILE_BAR`; the kq lane against the exact lane under `KQ_EXACT_BAR`; the flow head on its Q8_0 route against the
  oracle's frames under `KQ_HEAD_BAR`; the stored roster voice against the reference encoder; and a clone over a
  roster name keeping the roster at 19 voices and speaking.
- `test_tts_pocket.das`'s `test_pocket_quiet_floor` cell (on the file's pocket-tts-en-kq block) must hold the served
  lane's quiet floor against the f32 lane's.
- `test_tts_pocket.das`'s `test_pocket_small_models` cell must hold, for each `SMALL` row's published file, the
  19-voice English roster and the four character replacements its config carries, as the reference build's config
  lists them, each seat serving, or declining with the `shape` reason, as that row's seat columns say, and a low-rank
  text table kept low-rank.
- `_audio_asr_cells.das`'s `test_canary_qwen_oracle` and `canary_oracle_device_walk` cells (run under
  `test_audio.das`'s Canary blocks), an external-reference compare, must hold the forced walk over
  `../harness/canary_qwen_expected.tsv`'s ids within `CN_TIE_BAR` of our argmax at every step of its three clips, on
  the CPU chain and on the GPU tower that serves canary's default lane (the encoder minted on that lane in that
  tower's flavor, each walk counted as a device encode), its control the same walk fed each id one step early with the
  oracle id outside the bar at every step that input moved, and the greedy ids equal to the oracle's up to the first
  tie.
- `_audio_asr_cells.das`'s `test_parakeet_oracle`, `test_parakeet_v3_oracle` and `test_gemma4a_audio_oracle` cells
  (the first two on the `image` forms of `test_audio.das`'s parakeet v2 and v3 blocks, the third on the `image_asr`
  form of its E2B mmproj's block), external-reference compares, must hold the greedy ids against the tool's: jfk.wav's
  against parakeet-cli's - the token count, each token id the cell names and the text - and `gemma4a_test2.wav`'s
  first 40 against llama-mtmd-cli's, id for id.
- `test_nmt.das`'s `test_nmt_spm` cell, an external-reference compare, must hold the model-file tokenizer id for id
  against `nmt_oracle/<dir>/`'s `tokens.json` source ids and `spm_edge.json` on every stocked student, its control the
  charsmap dropped spelling the normalizer fixture apart.
- `test_nmt.das`'s `test_nmt_exact_lane` cell (on each student's block, its f32 form), an external-reference compare,
  must hold every tapped stage of the exact lane under the oracle's own tokens within `STAGE_BAR` (1e-4 of the stage's
  largest magnitude) of the `.npy` dumps, the element counts equal, a displaced source token landing the last encoder
  layer outside the bar; and the free run at `EXACT_LINES_FLOOR` (28 of 30) lines of `tokens.json`'s ids, the lines
  fed the next line's source under it.
- `test_nmt.das`'s `test_nmt_q8_lane` cell (on each ru/en base-memory student's block: its q8 form, the
  classifier-zeroed form's `q8_cls_zeroed_lane`, and their compare `q8_against_zeroed` after the block), an
  external-reference compare, must hold the q8 lane at `Q8_LINES_FLOOR` (25 of 30) lines of `tokens.json`'s ids on the
  ru/en base-memory pairs, the same file with its classifier's Q8_0 copy zeroed moving every line and falling under
  the floor.
- `test_tts_g2p_ru.das`'s `test_ru_stress_encoder` cell (on the file's ru-stress-rubert-base block, a form each), an
  external-reference compare, must hold, on the 20 sentences of `tts_oracle/ru_stress/`, the piece ids the dumps' own,
  the last hidden state within `F32_BAR` on the f32 lane and `Q8_BAR` on the q8 lane, the head's logits within the
  lane's logit bar and the f32 lane's every pick the reference's, each lane with the word's pieces as `[UNK]` landing
  outside both bars.
- `test_tts_g2p_ru.das`'s `test_ru_stress_api` cell (on the file's ru-stress-rubert-base block, its f32 form) must
  hold the word finder on the manifest's span and the catalog's mask the manifest's for every case, the batch picks
  bit for bit the single picks, and a maskless pick on the largest logit.
- `test_tts_kokoro.das`'s `test_kokoro_ru_mixed_script` cell (on the file's kokoro-ru block) must hold the facade's
  phonemes of a text with Latin runs around two homographs to the context pass's marking of the normalized text, the
  Latin runs spliced after, the pack's marking another string; and with no context model, the pack's marking, the same
  whether the splice runs first or last.

**A diff that changes the contract a pinned test cell holds fixed - what its entry says it must keep holding, an axis
gained or lost - updates that cell's entry in the pinned set in the same change; a diff adding a cell to a file with a
file entry adds to that entry each axis the cell asserts that the entry does not name.** An axis is one distinct
behaviour the cell asserts - an output form, a refusal path, an argument's order; a new input row is an axis gained
only where it runs a kernel stamp, an output form or a branch of the kernel or its router that no earlier row of the
cell ran. An assert added inside a gate of an entry that holds every kernel gate the cell calls is covered by that
clause, and needs no item of its own.

**A diff that widens a pinned test cell's tolerance, swaps its comparison for a looser one, rewrites an expected value
its entry ties to an outside source (a document, a checked-in table, a knob list) to a value that source does not state,
drops its input, or drops a `run.das` suite or area listing that reaches it is a defect - fix the code that fails the
cell instead; a bar shown mis-derived is re-derived and ledgered in `../ARCHITECTURE_CELL_BARS.md#rederived-cell-bars`.
A comparison that changes because the contract it held changed on purpose - a rule document or the data model now says
so - is not a loosening where the same change rewrites the cell's entry to the new contract and names the rule or
document that states it.**

**A diff that removes a pinned test cell's assert while the code it checks stays adds to the same cell, in the same
change, an assert that holds the same values to an equal or tighter bound, with its control (an extra assert in the
same cell proving the compare can fail).**

**A diff that deletes the kernel or function a pinned test cell's assert checks and routes its callers to other code -
the replacement - adds, in the same change, an assert holding the replacement's output to the deleted assert's bound
or tighter, with its control, to a pinned cell testing the replacement, or names in the old cell's pinned-set entry
the pinned cell that already asserts the replacement's output to that bound.**

**A diff that adds a cell or an assert - a cell moved or re-hosted (run from another `[test]`) whole, its asserts
unchanged, is not added - whose expected value the cell does not compute from its own input, and that an outside
source can change while no sha, row or tag in this repository changes, adds the cell to the pinned set in the same
change.** Such a value is a literal or a copied condition restating that source, or a read of a file under
`models_dir()` that the cell does not build and that no `../performance/model_specs.das` row pins by sha256 (an oracle
dump, a truth cache). A new entry in an existing cell's expectation list is not a new assert, and a constant this
repository declares, compared by name, is not such a value.

**A diff that adds a cell or an assert that a checked-in table names as its evidence - `../README.md`'s model table, a
kernel list whose coverage note names a cell as each kernel's evidence - adds the cell to the pinned set in the same
change.**

**A diff that adds a cell to the pinned set adds it as a named-cell entry - in `REVIEW_PINNED_KERNEL_GATES.md` where
the cell's file is a `test_*_kernels*.das` file, here otherwise - or, when the new cell leaves every cell of its file
pinned, replaces the file's named cells with one file entry naming what it pins.**

**A diff that moves, renames or re-hosts a pinned test cell, in the same change, rewrites the cell's named-cell entry
to its new file and function, or, when the cell's old file has a file entry, adds a named-cell entry for the cell at
its new file and function.**
