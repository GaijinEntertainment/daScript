---
slug: cpp-profiler-macos-samply-instruments
title: What C++ sampling profiler should I use on macOS for daslang (and how do I run it)?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

# C++ sampling profiler on macOS (Apple Silicon)

VS Code has **no first-class C++ profiler integration on macOS** — the "Performance Profiler" / similar extensions wrap Linux `perf` and don't help here. Skip them. Run a sampler from the integrated terminal and view results in browser/Instruments.

## samply (default choice)

Rust-built, Firefox-Profiler frontend, zero config.

```bash
cargo install samply
samply record ./build/daslang script.das
```

- Opens flamegraph in browser automatically.
- Symbolicates Mach-O cleanly if you build `-DCMAKE_BUILD_TYPE=RelWithDebInfo` (do NOT use plain `Release` — symbols are stripped).
- Works without sudo on Apple Silicon.
- Good for "where does the CPU go" questions.

## Xcode Instruments — Time Profiler (second opinion)

Native macOS sampler, kernel-assisted, best symbolication on Apple Silicon. Use when samply's view is ambiguous or you want call-tree + timeline together.

```bash
xcrun xctrace record --template 'Time Profiler' --launch -- ./build/daslang script.das
```

Then open the resulting `.trace` bundle (Instruments launches). UI is outside VS Code.

## daslang-specific recipe

Pair the sampler with the per-module compile-time logging (`-log-compile-time` CLI flag, added on branch `bbatkin/log-compile-time-cli`):

```bash
cmake --build build --config RelWithDebInfo -j 64
samply record ./build/daslang -log-compile-time path/to/script.das
```

- `-log-compile-time` tells you which module is slow.
- Sampling tells you which function inside that module is hot.
- Together they narrow "compile is slow" to a specific phase + symbol.

## What NOT to use

- `perf` — Linux only, doesn't exist on Darwin.
- Intel VTune — x86-mostly, ignore on Apple Silicon.
- `gprof` — instrumenting, not sampling; ancient.
- VS Code C++ profiler extensions — see above, all are Linux/perf wrappers or toys.
- `hyperfine` / `poop` — benchmarking (whole-program timing), not profiling (per-function hotspots). Different question.

## Build flag reminder

Both samply and Instruments need symbols. The two viable build types on this repo:

- `RelWithDebInfo` — fast code + symbols. Use this for profiling.
- `Debug` — slow code; profile reflects debug overhead, not real hotspots. Avoid.

Plain `Release` strips symbols and you'll get `???` everywhere in the flamegraph.

## Questions
- What C++ sampling profiler should I use on macOS for daslang (and how do I run it)?
