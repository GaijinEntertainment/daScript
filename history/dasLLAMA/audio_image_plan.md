# Audio `.dlim` — one load path, borrowed planes

The LLM side of the load rail was unified: one `build_image` / `parse_image`, cold builds stream
from the gguf mapping, the served struct is always image-backed. The audio side was left half
way. This is the symmetrical half.

## What is true today (measured)

Six weight carriers hold planes: `AudioTower`, `WhisperModel`, `Qwen3aTower`, `ParakeetModel`,
`CanaryEncoder`, `Gemma4aEncoder`.

**One struct serves two owners.** `load_X_` fills `array<T>` fields it owns; `parse_image` then
overwrites those same fields' headers with `_builtin_make_temp_array_i64`, pointing them into the
mapping. The field's type says "I own this"; at runtime it may or may not. Every finalizer has to
ask `lock_count(field)` to find out *what a field even is*, and that question leaked out of the
engine into `tests/test_model_image.das`.

**Two load modes.** `DASLLAMA_IMAGE=0`, or any cold path, serves owned RAM arrays. Otherwise the
struct is borrowed. Same type, two lifetimes.

**Cold peaks at two copies.** `load_X_` materializes the whole model, `cache_via_image` then writes
it out. The LLM rail stopped doing this; audio never started.

**Latent bug, independent of the above.** `AudioTower`, `Qwen3aTower`, `CanaryEncoder`, and
`Gemma4aEncoder` call `fmap_close` directly in their finalizers. `WhisperModel` and
`ParakeetModel` call `image_backing_release`, which is the one that also handles a memory chunk.
When `cache_via_image_` takes the chunk rail — `DASLLAMA_IMAGE_SAVE=0`, or a failed write — the
first four `munmap` a malloc'd pointer.

### Plane inventory

| carrier | planes | notes |
|---|---|---|
| `AudioTower` | `fblob` `qblob` `qscales` | 3 |
| `WhisperModel` | `filters` `dblob` `dqblob` `dqscales` | 4 + nested `enc` |
| `Qwen3aTower` | `cblob` | 1 + nested `core` |
| `ParakeetModel` | `filters` `window` `blob` `qblob` `qscales` + `durations` `lstm_ih` `lstm_hh` `lstm_b` | 9 |
| `CanaryEncoder` | `filters` `window` `twiddles` `blob` + `layers` | 5 |
| `Gemma4aEncoder` | `filters` `hann` `twiddles` `pos_emb` `ones` `blob` + `layers` | 7 |

**No plane is written after load.** Every write into a plane field sits inside a `load_*_` body;
the per-position tables that *are* written at runtime (`pos_emb` in `ParakeetState` /
`CanaryState`) live on state structs, not carriers. So a plane can be `const`.

**Consumer surface:** ~385 lines across the six files touch a plane field.

## Target

One representation, one lifetime, one rail:

- a plane is a **pointer and a count**, always borrowed, `const`;
- a carrier owns **exactly one** thing, its image backing, and its finalizer releases exactly that;
- there is no mode in which a carrier holds owned weights;
- cold mints the image first (the LLM behaviour), sized ahead of time, and serves from it.

## Representation (probe-verified)

A naked `float const?` field is the wrong trade: `p[i]` needs `unsafe` at every read, which is
~385 sites, and the length is gone, so bounds checks go with it. A two-field struct costs nothing
and keeps both.

```das
struct PlaneF  { p : float const? = null; n : int64 = 0l }
struct PlaneI8 { p : int8  const? = null; n : int64 = 0l }
```

with `operator []` (both `int` and `int64` — no implicit promotion), `length`, `empty`. Probed:
the accessor reads correctly, `pl.p` still hands a raw `float const?` to a pointer kernel with no
laundering, and a generic `apply` walk discriminates a plane by shape via
`typeinfo safe_has_field <p> (field)`.

**Two plane types is enough** if the small non-float arrays move into the meta blob, where they
belong: `durations`, `lstm_ih/hh/b` (offset lists), `layers` (POD offset structs). All are KB and
already have `serialize_pod_array`.

**The accessor is free under `-jit`, measured.** A cache-resident 4-accumulator sum over 67.1 M
elements, 7 interleaved reps: raw pointer 15.635 ms, plane accessor in the same module 15.619 ms,
plane accessor in a *different* module 15.627 ms, cv ≤ 1.24 %. That is ~1.3 elements per cycle in
all three arms, so the accessor inlines across the module boundary as well as within one — the
das auto-inline pass being same-module-only does not govern this, because under `-jit` the program
lowers to one LLVM module. Interpreted and AOT are untested and dasLLAMA runs neither.

**The bounds check is free too, so it ships.** Same rig: checked plane 15.611 ms against the raw
pointer's 15.608 ms, and against 20.856 ms for the `array<float>` indexing it replaces — the plane
is safer than a raw pointer and a quarter faster than the status quo, because array indexing pays
an indirection through the header that an inlined pointer does not. Removing the check is a
profiling-time question on real kernels, not a design one; it is banked in `PERF_LEDGER.md`.

## Staging is a separate type — this is the actual fix

The root cause is one struct serving an owner and a borrower. Split them:

```das
struct WhisperStaging {
    m : WhisperModel = WhisperModel()   // scalars and offsets land here; planes stay null
    filters : array<float>
    dblob : array<float>
    dqblob : array<int8>
    dqscales : array<float>
}
```

`load_X_` fills a staging struct. `build_image` walks it — every plane is an `array<T>` there, so
the **write side of `dasllama_image.das` needs no change at all**. `parse_image` binds the served
struct's `Plane` fields. Cold and warm converge on the same borrowed struct, and `Model` keeps
using the existing array-borrow path until it follows.

Staging embeds the served struct rather than restating its scalars, so the meta list and its
field-count tripwire stay in one place.

**Open:** nested carriers (`WhisperModel.enc`, `Qwen3aTower.core`) get dotted section names today
via `typeinfo safe_has_field <image_map>`. Staging structs have no `image_map`, so the nested walk
needs a different marker. Decide before Phase 1 starts.

## Status

**Phase 1 LANDED, Phase 2 LANDED, Phase 3 LANDED FOR WHISPER** (5.29 → 2.12 GB cold-mint peak,
streamed==eager gated plane-for-plane, fp32 and q8 both). Gemma4a/canary mints (+1.15/+1.33 GB)
stay staged for now — both sit under serve-side peaks their mints do not dominate, and the
long-clip encoder scratch in PERF_LEDGER.md is the bigger memory item. Parakeet (+0.15 GB) is
not worth streaming.

**Phase 1 LANDED, Phase 2 LANDED** (2026-08-01, branch bbatkin/dasllama-reorg). All six carriers
serve borrowed planes; no eager form survives; the ownership hack remains only under `Model` and
the synthetic test probe. The gemma4a teardown panic closed with it — it was the generated
`AsrModel` field-walk delete, fixed by an explicit finalizer. Phase 3 is open, and it is the
binding half of CODEREVIEW.md's "a mint never holds the whole model": today every audio mint
stages the full model in owned arrays before the writer runs.

## Phases

**Phase 1 — representation.** Add `PlaneF`/`PlaneI8` and accessors. Move the small arrays to meta.
Convert the six carriers field by field, family at a time, `AudioTower` first (`WhisperModel` and
`Qwen3aTower` nest it). Introduce staging structs; `load_X_` fills staging. Finalizers lose the
`lock_count` branch and release only the backing. `_builtin_make_temp_array` /
`_builtin_forget_temp_array` leave the audio side entirely.

**Phase 2 — always map.** Delete the eager-serve arm: `DASLLAMA_IMAGE=0` routes to the in-memory
chunk rail instead of returning owned arrays, matching `DASLLAMA_IMAGE_SAVE` on the LLM side. Fix
the four `fmap_close` finalizers to `image_backing_release` — a prerequisite, since Phase 2 makes
the chunk rail reachable in the default configuration. This is a user-visible knob change and
belongs in `ENVIRONMENT.md`.

**Phase 3 — stream the cold build.** Baselines measured (2026-08-02, m1, max-RSS warm vs cold):
whisper-turbo 1.48→5.29 GB is THE case — mint 3.6× serve; gemma4a +1.15 GB, canary +1.33 GB on
top of serve peaks their mints do not dominate; parakeet +0.15 GB, not worth streaming. Order:
whisper, then gemma4a/canary if their serve-side scratch (the larger finding — see
PERF_LEDGER.md) has not restructured them first. Compute the layout and total size ahead of the first byte
(the loaders already compute every offset before filling), then transcode plane bytes from the
source mapping straight into the writer instead of materializing the blob. Peak drops from
whole-model to one band. This is where the memory win is; Phases 1–2 are correctness.

Phase 1 is the large one and splits per family. Phase 3 can land per family too, and does not have
to land for all six.

## Tests

Existing coverage to lean on: `tests/test_model_image.das` (image round-trip),
`tests/test_whisper.das` (the whole ASR suite, 35 tests), `tests/test_parity.das`.

New, and they land *with* the phase they cover:

**Phase 1**
- *Plane accessor unit test* — bind a `PlaneF`/`PlaneI8` over a known byte pattern; check
  `length`, `empty`, `operator []` at both index types, and that `pl.p` reaches a pointer kernel.
  Feed the function, check the bytes.
- *Staging/served name agreement* — per family, assert every staging plane field name has a
  matching served `Plane` field and vice versa. A rename that hits one side silently drops a plane
  to empty today; this is the test that makes that loud.
- *Round-trip per family* — stage from source, build to a chunk, parse back, compare every plane
  byte-for-byte and every meta scalar against the staged values. Six cells, small models only.
- *Finalizer owns one thing* — load a carrier from an image, drop it, and assert the backing is
  released once and no plane is freed. `test_model_image.das` loses its `lock_count` reach-in.

**Phase 2**
- *Chunk rail per family* — force `DASLLAMA_IMAGE_SAVE=0` and load each family; this is the test
  the four `fmap_close` finalizers fail today.
- *`DASLLAMA_IMAGE=0` still serves* — the knob changes meaning, so it needs a cell proving a model
  still loads and transcribes under it.

**Phase 3**
- *Streamed == eager* — build a family's image both ways and compare the files byte-for-byte,
  the same shape the LLM streaming rail is gated by.
- *Peak footprint* — measure peak RSS for a cold audio mint before and after, per family. Report
  the pair (footprint, wall-clock); a load-time trade ships both numbers, not an assumption.

**Every phase**
- The full ASR suite green under `DASLLAMA_CPU_PREFILL=1 DASLLAMA_METAL_HAZARD_STRICT=1`, via
  `tests/run.das`, before the phase is called done.
- ASR transcripts unchanged: the ASR board through `performance/gen_bench_records.das --asr-tier`
  compares text, and text is the acceptance gate for a representation change.

## Gated on this

`test_gemma4a_audio_oracle` — `can't delete locked array` during teardown after real token
generation. It is a bug in exactly the mechanism this arc deletes, so it is not patched
separately; it is re-checked after Phase 1.
