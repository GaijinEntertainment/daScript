# dasLLAMA Architecture - a fat exe's first start

Companion to `ARCHITECTURE_MEASUREMENT.md`; a section is cited by its anchor. This document
carries two sections: `ARCHITECTURE_MEASUREMENT_FAT_START.md#fat-first-start`, how a fat exe,
which ships no tuner, mints its sidecar's runtime section on its first start, and where that
sidecar lives; and `ARCHITECTURE_MEASUREMENT_FAT_START.md#box-runtime-file`, the per-box file a
run with no sidecar of its own races the same section into. The benchmark rig, the tune gate and
the instrumentation rails stay in `ARCHITECTURE_MEASUREMENT.md`.

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

### A run with no sidecar of its own races into the box's file {#box-runtime-file}

A `-jit` script, the server run from the tree, a probe, a test child: each keeps its sidecar
beside its own root script (`<script>.tune.json`), so a program that was never minted has no
runtime section and, before this file existed, served Metal uncrowned with a warning nobody read.
Such a run now reads the box's file instead: `~/.dasllama/tune/box-<key>.tune.json`
(`dasllama_box_runtime_path`), the key this box's identity folded as the sidecar staleness rule
folds it (`box_match_key`), one file for every program on the box. The default apply
(`apply_box_profile_runtime_checked` with `box_fallback` on, the form `apply_box_profile_runtime`
runs for the program's own manifest) asks for it through the box-runtime hook
(`set_box_runtime_hook`, registered by `dasllama_fat_start`'s `[init]`) wherever that manifest
holds no section of this box's own - absent, another box's, unreadable, or a kernels-only mint;
an apply a caller points at a path of its own never falls back, since the path is the caller's
choice. The hook (`dasllama_box_runtime_file`) answers "" for a process that keeps its manifest - a
fat exe (its per-user sidecar above), an explicit `DAS_TUNE_MANIFEST`, a standalone exe with its
sidecar beside it - and for a run on the `reference` or `fallback` kernel policy
(`tune_policy_env`), the portable tier's arm, which runs crownless by its own contract. The rig and
the records cells set the manifest, and the suite runner sets it on a box whose
`performance/<box>.tune.json` is committed; a runner on a box with no committed manifest reaches
the file like any other script.

The file holds the runtime section alone - the Metal twin crowns and the knob snapshot
(`dasllama_runtime_snapshot`) - beside the box identity and `kernel_library`, the digest of the
Metal driver sources this process compiles its kernels from (`dasllama_kernel_sources_digest`: the
name, size and mtime of every `dasllama_metal*.das`, `dasllama_gpu_kernels_common.das` and
`dasllama_gpu_math.das` beside the engine's own source, located through `get_this_module_dir`,
and of the MSL emitter under `modules/dasMetal/metal/`, whose edits move every kernel's text). A file whose identity or digest
differs is not valid for this run (`dasllama_box_runtime_valid`), and the hook races again
(`dasllama_box_runtime_race`: the same synthetic twin race the fat exe runs, no model, no child)
and overwrites it; a shared binary's mtime is no key here, since every script on a box shares one
binary and a kernel edit moves the verdict without rebuilding it. A race runs at most once per
process, so an unwritable home keeps the crowns for the process and says so. No kernels section is
ever written to this file, so the tune framework's stale-sidecar merge cannot drop one from it.
