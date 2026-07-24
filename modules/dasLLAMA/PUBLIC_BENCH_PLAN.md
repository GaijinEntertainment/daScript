# dasLLAMA public benchmarks — records, tool, site

One measurement rail feeding a GGUF-keyed record store, rendered as a per-model leaderboard on
daslang.io, open to community submissions. Replaces the bottom (2D table) portion of
`site/dasllama.html`; the top hand-picked scoreboard bars stay as-is. ASR/audio-chat sections are
out of scope (separate arc). Internal ladder drivers keep their rails until migrated.

## Metrics

Community-standard pair, exactly llama-bench semantics and naming: **pp512** (prompt processing,
tok/s) and **tg128** (token generation, tok/s), mean ± stdev over `-r` reps after one untimed
warmup. The test key set is open-ended (`pp512`, `tg128`, later `pp512@d4096`, `tg128@b4`, ...);
depth/batch variants are internal-only until deliberately published.

## Record schema

One record per run — `(gguf, box, engine, backend, flavor)` is the identity. References are
first-class runs, not fields nested inside das records; pairing and ratios are derived at render
time and never stored.

```json
{ "gguf": "Qwen3-30B-A3B-Q4_K_M.gguf", "arch": "qwen3moe", "quant": "Q4_K_M",
  "params_b": 30.5, "active_b": 3.3, "size_bytes": 18600000000,
  "runs": [
    { "engine": "das", "backend": "metal", "flavor": "tuned",
      "box": "m1max", "threads": 8, "date": "2026-07-21", "source": "official",
      "cmd": "bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m ... -ngl 99 -o json",
      "sha": "6832e6dcd", "version": "0.6.3",
      "tune": "k4q8_tile_gen=mr4; ...",
      "hardware": { "cpu": "Apple M1 Max", "arch": "arm64", "perf_cores": 8, "total_cores": 10,
                    "ram_gb": 64, "gpu": "Apple M1 Max (32-core)", "os": "macOS 26.4.1" },
      "tests": { "pp512": { "tok_s": 612.4, "stddev": 3.1 },
                 "tg128": { "tok_s": 48.2, "stddev": 0.4 } } },
    { "engine": "llama.cpp", "backend": "metal", "flavor": "stock", "box": "m1max",
      "cmd": "llama-bench -m ... -ngl 99 -t 8 -p 512 -n 128 -o json", "sha": "ebd048f",
      "...": "same shape" } ] }
```

- `flavor` — das: `tuned` (the only sanctioned mode; the tool auto-tunes first) or `accel`
  (`--accel`, the +AMX Accelerate tier over the tuned stack). llama.cpp: `clean-cpu` (no
  Accelerate/BLAS/Metal — kernel-vs-kernel), `stock` (release defaults, what users actually
  get), `metal`. The stock CPU pass runs `-ngl 0 -nopo 1` — without `-nopo` a stock build
  op-offloads big-batch host-weight work to Metal, which is not a CPU number.
- `tune` — the applied kernel winners/fallbacks. Required on das runs: `[tune]` makes two
  identical machines legitimately differ, so a record without the fingerprint is not reproducible.
- `hardware` — auto-gathered, never typed by the submitter (community submissions depend on this
  block being trustworthy and complete).
- `source` — `official` | `community`; community rows render tagged and are accepted via reviewed
  PR (v1) or the hosted submission server (v2).
- Upsert key = `(gguf, box, engine, backend, flavor)`. Re-measures replace in place with a fresh
  date/sha; git history is the archive. No superseded copies, ever.

Store: `modules/dasLLAMA/performance/records/<box>.json` (official) +
`records/community/*.json`; site build merges into one `site/files/dasllama/bench_records.json`.

## Tool — `benchmarks/lcpp_bench.das` (name kept)

Already a llama-bench mirror (`-m -p -n -r`, same test shapes, no-sampling tg, warmup+reps).
Grows into the one user-facing bench; everything else becomes a driver of it.

- Add: `-t` (threads), `--ngl` (0 = CPU, else Metal `required` — llama.cpp's spelling), `-o json|md`
  (`md` = the community-legible llama-bench-style table; `json` = complete records).
- `--json-path <file>` is the machine rail: engine `[I]`/tune logs share stdout, and an untuned
  box's tune-then-relaunch merges stderr into stdout, so record consumers (the driver, the server
  button) read the file, never the pipe. `-o json` still prints the records last on stdout.
- Auto-tune-first, then relaunch with winners (the existing server-bench-child behavior).
- `--ref <llama-bench>`: also runs llama-bench on the same GGUF (parses its `-o json`, which
  self-reports `build_commit` — provenance is automatic whatever binary is supplied) and emits
  the sibling llama.cpp record in the same output.
- Advanced/internal flags (the `--*-ab` levers; later `-d`, `-b` — also llama-bench spellings)
  remain but are not part of the user-facing story.
- No shipped das binaries — `[tune]` rules them out. Users run the `.das` under `-jit` from a
  repo build or `daspkg release -jit`; 0.7 (brew/pip, ~September) widens the audience.

## Reference supply chain

- `benchmarks/setup_lcpp_ref` (new, executable — not prose): checks out llama.cpp at a pinned
  sha and builds per flavor with pinned flags. `clean-cpu` = `-DGGML_ACCELERATE=OFF
  -DGGML_BLAS=OFF -DGGML_METAL=OFF`; `metal`/`stock` = release defaults. Any box reproduces the
  identical ref binary.
- Community users skip building: official llama.cpp GitHub release binaries include llama-bench;
  `build_commit` in its JSON output pins what they ran.
- A run with no ref binary is still valid — das-only record, missing cells render `-`.

## Site — leaderboard per (GGUF × box)

The self-defending presentation: every configuration is a row, sorted by speed. No editorial
"which ref is fair" — stock/accelerated rows sit next to ours.

- GGUF list; each model a card; within, one group per box.
- Rows = all runs: engine + backend + tags (`metal` / `cpu` / `accel` / `raw` / `tuned` /
  `community`), pp512, tg128, horizontal bar scaled to the group's fastest.
- Sort by tg128 default (perceived speed), toggle pp512.
- das rows amber, llama.cpp rows gray, group winner bold.
- Ratio column on das rows: each das configuration vs its MATCHED llama.cpp build — three
  like-for-like pairs: `generic neon-64` (das cpu vs clean-cpu, kernel vs kernel), `cpu + AMX`
  (das `--accel` vs stock `-ngl 0 -nopo 1`, Accelerate both sides / no GPU), `as shipped`
  (das metal vs stock `-ngl 99`). Never "best on the backend" — a stock `-ngl 0` row with
  Metal op-offload masquerading as CPU poisoned that derivation. >1.0 amber-bold, <1.0 dim.
- Row expands to the receipt: full command lines, shas, date, tune fingerprint, hardware.
- Filter chips: platform, backend.

## Server unification

`dasllama-server` control page's Test button (`POST /bench`) becomes a thin wrapper over the
tool: one child invocation with `--ref -o json`, no more two-child orchestration or markdown
parsing in `openai_server.das`. Result panel gains a "copy record" affordance — the v1
submission funnel (paste into a prefilled GitHub issue).

## Fleet & catalog

Boxes: M1 Max, M3 (ssh), M5 (incoming), zen2 (3990X), rented Zen 3/4, rented Intel.
Catalog: community-popular GGUFs, one popular quant each (not the internal quant matrix) —
Qwen3-30B-A3B (MoE), gemma-3 sizes, a Llama-3.x-8B, Qwen3-4B/8B, a small-end model; final list
fixed at first sweep. Measurement discipline per the standing protocol: one model process at a
time, quiesced box, batch cells into one invocation, interleave engines per cell.

## Build order

1. Tool: `-t` / `-ngl` / `-o json|md`, hardware + tune block, `--ref` sibling records.
2. Ref supply chain: `setup_lcpp_ref` + flavor pinning.
3. Record store + catalog driver (official upserts); server Test button rebased onto the tool.
4. Site leaderboard section (community tag from day 1).
5. Clean-window sweeps per box, clean + stock ref passes.

v2: hosted submission server (also hosts/auto-fetches pinned ref binaries), select-two compare
widget (maybe), publishing depth/batch variants.
