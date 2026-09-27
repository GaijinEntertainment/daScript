# dasLLAMA Architecture - a fat exe's first start

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor. This document
carries one section, `ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start`: how a fat exe,
which ships no tuner, mints its sidecar's runtime section on its first start, and where that
sidecar lives. The benchmark rig, the tune gate and the instrumentation rails stay in
`ARCHITECTURE_MEASUREMENT.md`.

### A fat exe races its runtime section at first start {#fat-first-start}

A fat exe (`DAS_TUNE_MODE=fat`, `modules/dasLLVM/ARCHITECTURE_TARGET_FEATURES.md` sec.11) ships
its kernels baked per CPU class and carries no tuner and no policy rail, so nothing would ever
mint the sidecar's `"runtime"` section - the Metal twin crowns among its knobs, a 2-4x
kernel-form gain of a tensor twin over its simdgroup kernel per twin-race row on the M5 Max
(`harness/tune_kernels.das`, the metal_crowns family) - and a shipped Mac exe would run
uncrowned forever. The section needs no rebuild, so the exe mints it itself:
`dasllama_fat_start` registers `dasllama_fat_first_start` with the box-profile apply
(`set_runtime_race_hook`) from its `[init]`, and the engine umbrella (`dasllama_transformer`)
requires the module so every engine program carries the registration - the shipped bench
requires the umbrella, never the facade; `apply_box_profile_runtime_checked` fires the hook when
the sidecar is absent, another box's, or carries no runtime section, then reads the file the
hook wrote. The hook answers false outside a fat exe (`tune_fat_built()`); inside one it runs
`dasllama_race_runtime_section`: the Metal twin races (`dasllama_metal_crown_race` - both halves,
synthetic, no model) under the tune progress display, then `dasllama_runtime_snapshot` - the same
writer the mint's kernel half ends with - merged into the app sidecar with the kernels section
untouched. The next start reads it and races nothing. A box without a Metal device records the
knob defaults, so the file still documents the box. An unwritable location keeps the crowns for
the process and says so.

The sidecar of a fat exe lives per user, `~/.dasllama/tune/<exe>.tune.json` (`USERPROFILE` on
Windows), never beside the exe: a packaged exe sits in a directory its user cannot write or an
upgrade replaces whole (`/opt`, a Homebrew keg, a scoop version directory, site-packages, a
`.app` in `/Applications`). The same `[init]` registers a second hook
(`set_sidecar_place_hook`), which the default apply runs before it reads the path: it points the
tune framework there (`set_tune_manifest_runtime_path`), so the exchange files beside the sidecar
follow it, unless the location was chosen already - `DAS_TUNE_MANIFEST` still moves the file. It
runs at the apply rather than in the `[init]` because the exe's own `[init]` is what makes
`tune_fat_built()` true, and two `[init]`s run in no fixed order. A sidecar an
earlier version wrote beside the exe is copied over once, so an upgrade does not race again.

What a first start never does: load a model, spawn a child, or race a kernel. The tuner's
confirms - the generator half's end-to-end prefill A/B, the kernel half's serving and MTP depth
confirms - each spawn a daslang child on a harness script and a vehicle model, and they are the
harness's alone; under `harness/dasllama_tuner.das` on the M5 Max the confirms take 147 s of the
metal_crowns family's 161 s, the twin race itself 14 s.
