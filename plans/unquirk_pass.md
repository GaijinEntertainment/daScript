# Unquirk pass - end of the iquant format ladder arc

The ladder is complete (9 formats x 4 tiers, ea1f41d2c). This plan covers everything between
that and the arc's PR: the tuner rework, the KqPlanes unquirk, the cache/one-off fixes, the
HOW_TO document split, and two PRs. Order: B + C1 + D1 -> **PR-1** (the ladder ships, the
parallel M5/M1 session forks) -> A + D2 + E -> **PR-2**; every phase ends green and
committed. Kernel catch-up runs on zen2 in parallel with the mac session after PR-1.

Evidence base (2026-08-31 sidecar audit): a full mint races 39 families / 637 seats. The 16
tile-gen families crown `dot_maddubs_width256_mr8` on every format on zen2 and mr8-shapes on
ARM; the 22 `[tuned]` vector-utility families have winner-vs-runner-up gaps of 0.0-1.4% on
zen2 AND on M5 (25/25 under 2%). Cross-sidecar winner agreement 34/39; all 5 diffs are noise
ties or a skipped-seat artifact. The race rediscovers ~2 facts for ~20 minutes per app per box.
M5 Max carries FEAT_I8MM + FEAT_SME2/2p1 + BF16 that our ARM seat space does not offer a
single seat for - its mint provably lands on M1's answers today.

## Phase B - tuner rework (kills QUIRK 11, 15, 17, 18)

Target: mint on a known box = zero races; full race on new silicon (zen4, M5) = a couple of
minutes. If that target holds, the dasllama.io tune service is removed entirely (Boris's call,
outside this PR).

**B1. Prune the seat space.**
- Tile gens (`dasllama_math_gen.das`, 16 families x 11 seats): keep `mr8` (generic/ARM
  baseline), `maddubs w256 mr8` (requires avx2 - the x86 baseline), `vpdpbusd w256 mr8`
  (requires vnni), `vpdpbusd w512 mr16` (requires avx512vnni+bw). Drop mr4 and every nrsplit2
  twin (never won beyond noise on any box). 11 -> 4 seats, only 2 raceable per box class.
- q8q8 (15 seats): keep the observed winners' axes only - `kstep2` (zen2 crown),
  `mr8_budget` (M5 crown), plus the maddubs/vpdpbusd kstep2 twins under their gates. ~15 -> 4.
- `[tuned]` utilities (`dasllama_tune.das::build_grid()`, one place): grid 20 -> the union of
  every winner ever recorded across zen2/m1/m4/m5 sidecars (~6: plain, u2, vec8_u2, vec16,
  vec16_u2, vec4_u2). With B2 they stop racing on known classes anyway; the pruned grid is
  what a new-silicon full race walks.

**B2. Arch default profiles (kernels-only - the sharper form of the CPU/GPU split).**
- Profiles carry KERNEL WINNERS ONLY. The runtime-knob section never travels: its knobs mix
  device knobs (metal_*, batch_grid_2d, lane caps) with box-shape knobs (core counts, cache
  budgets, thread receipts) - neither is class-portable, so nothing GPU- or box-side can
  mis-inherit by construction. Knobs stay code-defaults on a fresh box until a deliberate
  `--tune`; existing minted sidecars keep theirs.
- Checked-in profiles: `modules/dasLLAMA/performance/defaults/<class>.tune-defaults.json`
  (the extension dodges the `*.tune.json` gitignore), `x86-avx2` minted from zen2,
  `arm-neon` from m1. `x86-vnni512` comes from the c7a trial; `arm-i8mm`/`arm-sme` exist
  only once those kernels do. `harness/export_tune_profile.das` mints one from a sidecar;
  the framework fn is `tune_profile_export`.
- Adoption is FRAMEWORK-side, declared on the scope - `[tune_scope(defaults = "dir")]` -
  and runs in the auto/restart guards ahead of the scope resolver: walk `tune_class_chain()`
  down from `tune_cpu_class()`, adopt the first matching profile (version-pin checked) into
  the app sidecar as a normal local write. A covered scope races NOTHING.
- Race-on-unlock: an adopting start races ONLY families with a seat whose `requires=` the
  host passes but the profile's minting box did not (recorded as the `features` fingerprint
  every sidecar save now stamps) - plus families the profile predates. The residue rides the
  existing `--tune-only` machinery (the guard arms `DAS_TUNE_ONLY` for the tuner it spawns).
  No new flag: bare `--tune` already skips adoption entirely - it IS the full new-silicon
  walk. `--tune-only` unchanged.

**B3. `DAS_TUNE_POLICY=reference`** - a policy value that serves the original bodies outright
(QUIRK 18's missing spelling). Small, framework-side.

**B4. Validation + the measurement Boris's service decision needs.** MEASURED 2026-08-31:
zen2 fresh full mint under the pruned grids 488 s (gen probe 414 + tuned/knobs 66; noise cv
<= 0.33%); zen2 parity cold start against the shipped x86-avx2 profile = 0 races (the 285 s
wall is two monolith JIT compiles, pre-adopt + re-exec), stream identical to the recorded
stamped one; c7a.4xlarge (EPYC 9R14 zen4, 16 vCPU) full new-silicon mint **316 s** (gen probe
253 + tuned 58, noise cv <= 0.21%) - `dot_vpdpbusd_width512_mr16` crowns EVERY tile family
and q8q8 takes the bias128/gkstep2 vnni512 stamp; x86-vnni512 profile exported + checked in.
m1 gen half 734 s (M1, 8 threads; mr8 everywhere) - its `tune_kernels` Metal arm had a
macOS-only compile error (two require paths for MetalTensorRaceResult), fixed in-pass.
- dasLLVM tune tests (test_tuned/test_grid/llvm_tune_*) extended for profile resolution +
  the split sidecar; gen probe test+tune modes; parity 40/40; one bench row vs current
  numbers (winners are unchanged by construction - prove it, don't assume it).
- zen2: delete sidecar, cold start -> 0 races, numbers match. Wall time recorded.
- M5 (ssh): fresh mint under arm-neon profile -> 0 races. Wall time recorded.
- zen4 (ssh, off-hours - it hosts dasweb): `--tune-full` new-silicon race, wall time
  recorded (target <= ~2-3 min), its x86-vnni512 profile checked in. First box where
  vpdpbusd can actually win - report the crowns.
- skills/tune.md + tune_for_this_box.md + HOW_TO QUIRK 11/15/17/18 text updated in-phase.

## Phase A - the KqPlanes table (kills QUIRK 1, 2, 3, 5, 7, 8, 13, 19, 25, 26)

One per-format record on `Model` - quant plane, scale plane, mr, qsb/ssb strides, stream
code, schema id, device-form flag - indexed by `KqFmt`; every `fmt ==` ladder becomes a
lookup. Staged so each commit is green; the fast dev loop (`-module-cache` +
`--jit-split-modules=-1`) keeps the cycle ~1 min; after EVERY stage that touches layout or
pack, delete `*.dlim` and e2e the MIXED vehicle (the iq2xs requant carrying q2_K - QUIRK
26's lesson), not a pure one.

- **A1.** The table + accessors (`kq_plane_q/s`, `kq_mr`, ...) + the id-derivation table
  (schema id + stream code from `KqFmt` - QUIRK 5/25 become one row each) + `test_kqformat`
  pins table<->enum. No consumers move yet.
- **A2.** Collapse the load/common/layout ladders (12 + 13 + 3 arms-per-format sites).
- **A3.** Collapse math_gen/math_default/math (8 + 3 + 4); the repack-mr freeze ladder
  (QUIRK 8) becomes a table write; `serialize_image_meta` serializes the table and
  `dlim_identity` formats from it (QUIRK 3/7 - the hand list and the count tripwire go).
  Image meta layout changes -> images re-bake once; C1 lands before this so stale images
  die loudly.
- **A4.** `tests/_kq_fixtures.das`: one per-format dispatch (`kq_transcode_sb`,
  `kq_dequant_sb`, `kq_dot`, `kq_repack`, `kq_gemv_gen`, `kq_tile_gen`) shared by
  `test_kquant` and `gen_tune_probe`; ONE family array walks both probe modes (QUIRK 2; the
  stub-family rule of QUIRK 16 gets a `stub` flag in that array instead of a hand list).
- **A5.** Metal test records (QUIRK 13): per-format `{src, entry, fastmath, tgmem}` table
  replaces the four parallel ternary chains. m1 sync + metal gemv/gemm gates.
- **A6.** The device gather's two branches key on the table's device-form flag (QUIRK 19).

## Phase C - cache invalidation

- **C1** (lands FIRST, before A3): a `PACK_VERSION` folded into the `.dlim` filename hash
  (QUIRK 24) - a layout/pack edit re-bakes instead of serving stale planes. Gate: bump ->
  next run logs "baked", not "mapped".
- QUIRK 21 (generator bodies into the JIT DLL hash) STAYS on `plans/jit_compile_time.md` -
  cache-keying work, not this pass.

## Phase D - one-offs

- **D1**: posix arm for `stage_jit_backend` in `utils/mcp/setup.das` (QUIRK 12).
- **D2**: `static_if ... elif` in the MSL emitter (QUIRK 14) - timeboxed to ~2h, dropped to
  the metal followup doc if the emitter fights.
- Ledgered, not done here: QUIRK 6 resolves as an INTRINSIC builtin (Boris's ruling: daslang
  has no u8/u16 operator math and will not start - a `byte_or(...)`-shaped builtin, never
  `|=` on uint8), SPIR-V constant-storage lowering for literal fixed_arrays (QUIRK 20,
  emitter). Both proposed as lint rules too:
  module-global read reachable from a worker/team-lane kernel (QUIRK 9/27, bitten twice),
  dynamically-indexed fixed_array local in a kernel class (QUIRK 20's cheap half).

## Phase E - the document split (the detour)

`HOW_TO_ADD_A_FORMAT.md` (today: recipe + 27 quirks + 9 phase diaries) becomes:

1. **`HOW_TO_ADD_A_FORMAT.md`** - an actual how-to: steps 0-8 rewritten post-unquirk (the
   table shrinks the step count), the E-cluster rules folded in as steps/warnings (stubs
   first Q4; worker-table rule Q9/27; stale-binder Q10; stub-out-of-test-list Q16;
   kernel-probe-not-e2e Q22; census-the-vehicle Q23), and the quirk ledger cut to the
   survivors (whatever A/B/C/D did not kill).
2. **`performance/PERF_LEDGER.md`** - the nine per-format four-tier tables, the three gap
   classes, each format's vehicle GGUF, and the box inventory: which model files sit on
   zen2 for testing, the m1/m5 paths, the ref llama.cpp builds per box.
3. **`followup_metal.md`** - twin of followup_vulkan.md: the Metal tg items (0.78-0.93x),
   the Q22 dispatch-loop probe method, D2 if dropped.
4. The per-format phase diaries -> `/history` (doc_archiving skill), linked from the ledger.

LAWS.md (modules/dasLLAMA) gets the rulings from this session: prune the families, saved
zen2/M1 defaults, race only on feature unlock, CPU/GPU tune split, the doc split.

## Phase F - TWO PRs (the mid-plan cut that starts the M5 session)

- **PR-1** (after B): the 9-format ladder the branch already carries + the tuner rework +
  C1 + D1. Full make_pr chain. On merge the parallel M5/M1 session forks from master -
  free mints (arm-neon profile), fast dev loop, pack-versioned images.
- **PR-2** (fresh branch off master): A (KqPlanes) + D2 + E (the doc split - it depends on
  A's survivor-quirk ledger anyway). Full make_pr chain again.
- Parallel-window rule: the mac session owns kernel BODIES and additive arms (i8mm/SME
  seats, Metal levers); the zen2 session's A owns dispatch/tables. A5 (the Metal test
  records) runs EARLY in A's sequence to shrink the shared-file window.

## Open questions (Boris)

1. The new-silicon trial runs on AWS (us-west-2, CLI configured on this box), not the dasweb
   zen4. No UMA + built-in-GPU x86 box exists on AWS; per goal the candidates are
   c7a.2xlarge (zen4, avx512-vnni - the vnni512 mint), c7i.2xlarge (Sapphire Rapids,
   vnni + AMX), c8g.2xlarge (Graviton4: i8mm + SVE2 - the arm-i8mm class once those
   kernels exist), g4ad.xlarge (zen2 + Radeon V520 dGPU - Vulkan-on-AMD-driver, not UMA).
   True UMA + Vulkan is Strix Halo territory, outside any cloud. Boris picks the box(es).
2. RESOLVED: profile classes key by the feature set that gates seats (winners follow
   instruction sets, not box identity). Host resolves to the highest class it satisfies
   that has a shipped profile, falling back down the chain (M5 today -> arm-neon).
3. RESOLVED yes: `performance/PERF_LEDGER.md` + `followup_metal.md`.
4. RESOLVED yes: D2 timeboxed, dropped to followup_metal.md if the emitter fights.

## Rough wall-clock

B ~1 day incl. the m5/zen4 trials; A ~1-2 days (the wide mechanical stages, each gated);
C/D hours; E ~half day; F the usual chain. Everything on `bbatkin/iquant-formats`.
