# The release-split + coverage + arc-end re-mint arc

Branch `bbatkin/release-remint` off master 507362fd9 (PR #3905 merged). Three ruled jobs:
the version split (so the board survives routine changes), the m5-image arc's coverage debt,
and the arc-end board re-mint on the b10659 ref pin. One PR.

## Rulings (Boris, 2026-08-30)

- The gate candidates from the m5-image round are REVIEW.das-shaped, not daslib lints -
  assess scope first (step 0); update the CLAUDE.md routing wording so "lint opportunities"
  can land as folder-local REVIEW.das gates. No strong opinion on which land in this arc.
- Plane `long_length` (followup 57) rides this arc.
- M4 re-mints its canary rows + proves the Efficiency-tier gate. M1 gets NO runs
  ("we've measured multiple times") - its stale canary rows retire.
- The version split is approved in the minimal RELEASE form: "it gives us flexibility we
  were missing". Leaderboard must stop invalidating on every bump - a 'major release'
  scope, declared by Boris, not derived from diffs.

## Slice V - the release split (FIRST: the re-mint must land on the new scope)

Recon facts: `DASLLAMA_VERSION` (dasllama_version.das:12, v15) is consumed ONLY by the tune
scope (`dasllama_math_gen.das` `version_of=`), the sidecar exchange (`dasllama_exchange.das`
accept/lookup/submit), records stamping (`lcpp_bench.das` x3, `tuner_noise.das` provenance
note) and tests. Dlim identity never reads it - `IMAGE_VERSION` (dasllama_image.das) is a
separate per-change counter and stays exactly as it is.

- Rename `DASLLAMA_VERSION` -> `DASLLAMA_RELEASE`, value stays 15 (existing v15 artifacts
  remain valid; older sidecars refuse as before). The rename is deliberate: the bump duty
  changes from "ANY kernel work bumps it" to "bumped only when Boris declares comparability
  broken", and the old spelling breaking loudly is the sweep guarantee.
- Wire/schema field names (`dasllama_version` in exchange JSON, sidecar provenance, bench
  records) KEEP their spelling - they now carry the release number; no schema break.
- Docs sweep: the bump-duty rule in `REVIEW.md`, plus mentions in `ARCHITECTURE_ENGINE.md`,
  `ARCHITECTURE_MEASUREMENT.md`, `performance/REVIEW.md`, `BRINGUP.md`, ladder
  README/REVIEW, server README.
- Hazard accepted (ruled): within a release a changed kernel can keep a stale sidecar winner
  - perf-only drift (crowns pick twins; races verified the envelope at mint). The principled
  hardening - per-entry kernel identity hashes in llvm_tune, the JIT-dll-cache
  self-invalidation pattern - is ledgered as its own generic llvm_tune item, not blocking.

## Step 0 - gate-candidate scope assessment (DONE, 2026-08-30)

- **tgmem-less `@workgroup` class: not a gate - a LENS fix, rides this arc.** The
  `[metal_dispatch]` apply already reads `tgmem=` (dasllama_metal_lens.das:236) and collects
  `@workgroup` members (sec.292); `@workgroup` always rides the DYNAMIC shmem buffer
  (msl_emit wg_bytes/wg_off), so a dispatch without `tgmem=` on such a class is always the
  qk_rope bug. Fix: auto-default `tgmem=` to the generated `*_msl_tgmem` constant when the
  class has `@workgroup` members (explicit arg still overrides); omission becomes
  impossible at compile. Audit existing omission sites during implementation.
- **tests/msl fail-closed fixture sync (followup 45): REVIEW.das cell, rides this arc.**
  `modules/dasMetal/REVIEW.das` already exists (the RP law) - the followup's "creates it"
  clause is stale; add the list-A-equals-list-B cell (~30 lines).
- **`set_*`/`reset_*` pin-leak walk: parked, own PR.** Needs an exception list (tests whose
  subject IS the pin) - judgment-laden, a modules/dasLLAMA/tests REVIEW.das walk when done.
- **gemm_f32 into an ensure_length'd target: parked with spec, own PR.** Function-local
  dataflow heuristic (target allocated un-zeroed in the same function, no zero/fill before
  the accumulate) - highest false-positive risk of the four.

## Slice A - census tower rows (tests/test_kernel_coverage.das)

`cov_tower` exists (whisper-mixed, gemma4uv, gemma4v, gemma3v, qwen3a-conv). Missing rows
for the arc's kernel families: gemma4a Conformer, canary FastConformer (q8 decoder
artifact), qwen25v window ViT (win_attn + row_gather), qwen3v GPU stem, and a
tower-flash-eligible leg (hs 72, npos%64==0 -> flash + kv_hc). Reconcile against the census
NEVER-DISPATCHED report; blind-spot entries move onto real rows.

## Slice B - cell plumbing + followup 57

- `gen_bench_records.das:1015` `is_metal_served_family` keys on Whisper alone -> extend to
  the four new servable capabilities (canary Q8_0 decoder lane, canary Metal FastConformer,
  gemma4a/qwen3a Metal rail, qwen25v Metal tower) + whatever catalog/spec rows the canary
  Q8_0 artifact needs. Receipt/anti-sandbag arms follow the whisper shape.
- Followup 57: `long_length` overloads for the Plane types in `dasllama_plane`, buffer-sizing
  call sites moved onto them.
- Retire the stale `records/m1.json` canary rows (they pin the deleted
  `canary-qwen-2.5b-decoder-f16.gguf`; no M1 runs by ruling).

## Slice C - fresh tune manifests on M5 (quiet box)

`benchmarks/lcpp_bench.tune.json` (v14) + the rig sidecar `performance/m5.tune.json` (v9!)
re-mint under the new release scope; refresh `last_known_good_sidecar.json`. Sanity floor
first; never `--quick` (followup 32); no rebuilds mid-race.

## Slice C - DONE 2026-08-30: manifests minted under release scope

Predictions vs outcomes (the game): mint 6-12 min / noise ok - **held** (7.3 min release,
noise ok); >=36/40 winners identical to v14 - **held** (37/40; three near-tie elementwise
flips); provenance stamps release 15 under the wire key - **held**. The box-manifest mint
(m5.tune.json) agreed with the bench sidecar on all 40 winners; LKG refreshed. Ref-side
plumbing landed on the way: setup_asr_rig builds a build-metal mtmd twin on Apple (the CPU
build's -ngl is inert), setup_lcpp_ref builds llama-mtmd-cli beside llama-bench (the
sibling rule), stock + metal mtmd binaries built on this box.

## Slice D predictions (logged BEFORE the sweep, 2026-08-30)

- Text metal cells reproduce the committed m5 rows within cv (E2B ~8.7k pp / ~162 tg;
  every das/ref ratio holds green).
- CPU: E2B pp ~850 das vs ~840 clean-cpu@18; MoE Q4KM pp ahead post-split (26B-A4B ~1.4x).
- New ASR gpu cells: whisper turbo 1.0-1.4x vs whisper-cli metal; canary gb1 ~2.5-3x vs
  nemo-MPS; gemma4a jfk ~1.0-1.25x and qwen3a jfk ~1.4-1.7x vs mtmd-metal; omni ahead
  1.1-1.5x on the ref ladder.
- qwen25v image cell: enc ~106 ms vs mtmd ~117 (das ahead all three axes).

## Slice D - the full M5 board re-mint (ref pin b10659)

clean-cpu + stock ref builds already minted in `llama.cpp-ref-6fdd0ac`. das cpu vs
clean-cpu, das `--accel` vs stock, the Metal text/image/audio boards, the four new cells
from slice B minted here. Parity pregate per row; predictions logged before each cell; one
process per box, cv>3% void; `gen_site_records` closes it. Full board on one pin = one
provenance (the v14/fallback-stamp taint is not per-cell diagnosable).

## Post-sweep ruling (Boris, 2026-08-30)

The two-tier stop rule: a FAIL (crash/refusal/no-measure) stops the sweep for an immediate
fix - it poisons every cell after it; a PERFORMANCE red lets the sweep finish, then fix and
re-profile ONLY the red cells (`-o` narrowing) - no full re-sweep, the board is then good;
more sweeps follow after the coming arcs.
Red #1: gpt-oss-20b (mxfp4 MoE) das metal pp halved vs the pre-merge row (3989.4,
`records/m5.json` @ master; the sweep's first-pass board cell read ~2.0k - VOIDED, the
mis-bound race arm had crowned the wrong twin; the committed post-fix cell is 3995.1)
while the k-quant MoE cells moved up (committed rows vs their @ master twins); window = the
master merge (first post-merge perf pass); suspect = the ffn_moe_prefill_grouped seam x the
32-row split on the Metal mx4 path (per-expert-bias staging pays per region where k-quant's
cnt/basep walk does not).

## Slice E - M4 over ssh

Canary rows re-mint (Q8_0 decoder artifact) + the Efficiency-tier gate proof: live-lane
print shows the fast-tier-only pool, pp/tg spot-check against existing rows within cv.

## Gate-phase evidence ledger (for the PR body)

- **Preflight --full ran ONCE** (the make-pr chain; every other gate green first pass). Its
  three reds, each fixed and validated with the TARGETED lane per policy: lint = LINT027
  (module REVIEW.md hit 317 lines after the exchange-rule move - split into the routed
  `REVIEW_EXCHANGE.md` companion, 294 left) + two off-Apple LINT012s
  (`[unused_argument(t, samples)]`); tests-interp / tests-jit = the same three files
  (jobque affinity, math_misc, crt_twins) erroring on missing builtins under the chained
  full run only - solo-green with the same binary before the fix, both lanes PASS clean on
  targeted rerun, so the failure was chained-load pressure, not code. CI validates the tip.
- **TDD audit (worktree, tip 013a151f4)**: 47 branches - 19 CONTROLLED (all 14 restore
  guards via `check_exe_fn_global_restore` incl. a base-source reverse run reporting exactly
  the 14; the lens tgmem refusal via an inverted control; both fail-closed-sync arms; the two
  serve rows), 12/12 test edits JUSTIFIED with reverse controls where runnable. Three of its
  named gaps closed in-PR with must-red controls: the `metal_served` roster pin
  (test_asr_catalog_invariants), serve-card presence pins (test_model_catalog), and the
  `version_key = "dasllama_version"` textual pin (test_exchange_schema - llvm_tune exposes no
  runtime reader). Remaining measurement-orchestrator branches (gen_bench_records x7,
  canary pybench x4, rig plumbing) ride the standing coverage-followup PR; the mx4-race
  bind-number gate is proposed to Boris as a REVIEW.das rule.
- **Woodpecker round 2** (tip 013a151f4): zero findings ("no discrete, actionable
  correctness issues"); round 1 at the pre-fix tip was also zero.
- **Suite evidence (the tests-checklist's owed runs, final tree, 2026-08-30)**: `run.das --
  --suite model-free` rc=0 (75 files); `--arm kernels --suite kernels` rc=0; `--arm coverage
  --suite coverage` rc=0 (small tier; the large-tier census rows stay
  `DASLLAMA_PARITY_FULL`-gated as designed). All under `-jit` with the box manifest armed -
  the first suite pass on this box with live crowns (see the crowned-lane entry above).
- **Figure provenance line (goes in the PR body verbatim)**: all figures quoted in this PR:
  `performance/records/m5.json` / `m4.json`, minted 2026-08-30 (m4 canary rows stamp
  2026-08-31 UTC) on the b10659 ref pin; `img:enc` / `asr:` figures are `lcpp_bench
  --image` / `--asr` board cells, das metal vs `llama-mtmd-cli`/`whisper-cli`/NeMo torch-MPS
  reference legs.

- **Commit-stamp re-stamp** (per `performance/REVIEW.md`'s reachability rule): four orphaned
  pre-rebase stamps re-stamped to byte-identical reachable twins (`git diff --stat <old> <new>
  -- modules/dasLLAMA/` = 0 lines each): fa67fe165->345dcd667, 82e45e0a3->bbdcf8da1,
  366669a53->f5417ba8f, 54595cfea->77dfc242d. The three tune-generation archives re-hashed and
  renamed (144fd7914ced->506843b9bcb2, 3865c28dd577->61f4ee8c1a4b, 05da62524756->17d503d8574d),
  every `tune_sha` repointed; LKG + `benchmarks/lcpp_bench.tune.json` re-stamped in place.
  `gen_site_records --verify` OK; bench-records/site/exchange schema gates green post-restamp.
- **M1 canary rows withdrawn, not re-minted**: the box is retired from runs by ruling ("we've
  measured multiple times") and the rows pinned the deleted f16 decoder artifact.
- **Post-fix mx4 race**: the rebound `race_moe_mulmm_mx4` re-race re-crowned honestly and the
  gpt-oss metal cell was re-profiled with the renumber in the built exe - the committed
  3995.1 row (`records/m5.json`, 2026-08-30) IS the post-fix reading; the fix and the record
  land in one commit, which is why the row's stamp predates the commit carrying both.
- **GPU parity for the crown/forwarder changes**: every das Metal row of the m5/m4 re-mint ran
  behind the per-row parity pregate with `--ngl` armed - q8 (E2B Q8_0, canary Q8_0) and kq
  (12B/30B Q4_K_M) both covered. The 14 boot-restore `[init]`s are `if (== null)` guards - no-ops
  on the `-jit` rail, exercised on the `-exe` rail by `test_exe_smoke`. The Vulkan-armed proof is
  deferred: the vulkan hooks arrived from the parallel vulkan arc and no Vulkan box runs in this
  arc.
- **exe-smoke knockout scope**: the smoke's SmolLM2 CPU generate reaches the decode claim path
  (`g_moe_gpu_attn_dec*` trio via the prefill-tail claim mirrors); the rdec/ffn_xf seams are
  unreached by it and covered statically by `REVIEW.das`'s `check_exe_fn_global_restore`.
- **Census numeric fixture claims**: assert-free by ledgered design (tests/CLAUDE.md, coverage
  suite: dispatch diversity, parity suites own truth) - "yes, ledgered".
- **Twin binding divergence** (mx4 cnt@6/basep@7 vs k-quant cnt@7/basep@8): ledgered in
  `ARCHITECTURE_GPU_PREFILL.md` sec.2.2g - "yes, ledgered".
- **url re-pins**: both new serve models AND their mmproj companions commit-pinned; each pinned
  url resolved live (HTTP 200 through the CDN redirect) at pin time.
- **m4 canary run dates read 2026-08-31**: the box stamps UTC; minted the evening of 08-30 local.
- **Exchange roster drift**: the download gate keys on (release, box); the serve-roster growth
  rides release 15 unchanged - older clients see the new rows only after their own catalog pull.
- **Server fixtures**: `catalog_idle.json` re-captured from a live boot (7 cards);
  `catalog_downloading/done` keep their captures - the download-state shape they pin is
  unchanged, and a mid-flight re-capture needs a live download (playwright suite 106/106).
- **Efficiency-tier gate, verified live**: M5 `pool 18 lanes` spanning Super+Performance; M4
  `live: 10 dispatch lanes` (efficiency cores excluded).
- **Suite-red forensics (post-audit fix pass)**: the re-minted sidecar armed the box's
  crowns in the test suites for the first time (the pre-remint sidecar was binary-stale, so
  crown-consuming gates had silently measured fallback). Two strict cells crossed bars under
  crowned kernels - whisper cross-lane token-identity (attn_qkmm) and gemma4uv tier-1 rung
  (mulmm_bf16, 6e-05 past the bar on one canvas). Both now pin the simdgroup lane via the
  new `pin_metal_tensor_crowns` latch (a bare setter cannot hold a lane - every load
  re-applies profile crowns); the crowned lane's serving quality remains the board parity
  pregate's claim. Ledgered as followup 60 (should the twins' race envelope tighten until
  strict gates pass crowned?). Also fixed on the way: the two role fixtures gained their
  `tgmem=` (the new lens refusal fired first, masking the role-error the test expects), and
  `test_exe_smoke` reads the env through `env_value_of` (the registry gate).
- **Census dlim purge (caught by the final suite pass)**: the pinned-lane census loads baked
  wrong-lane `.dlim` flavors to disk, and the bake's sibling GC purged the serving lane's
  image - `test_audio_embedder` then panicked on the wrong identity. All four new tower rows
  now mint in memory (`stage_*` + `mint_*`, a new `mint_gemma4a_encoder` filling the one
  missing wrapper); the stray flavors were removed and rebake on demand; the rule is now in
  `tests/REVIEW.md` (pinned-lane loads go through the in-memory rail).
- **Tower-gate cheap path** (`REVIEW_TOWER.md`): the driver diff is confined to the
  `get_metal_tower_f16` / `get_metal_tower_flash` getters, functions no other file under
  `dasllama/` names - their only callers are the census rows in
  `tests/test_kernel_coverage.das`. The coverage suite run additionally drives every tower
  family encode (gemma4uv/4v/3v, qwen3v/25v, whisper, audio legs).

## Done

- Tune sidecars and board rows survive routine version-less changes; only a declared
  release invalidates them.
- Census NEVER-DISPATCHED list holds only entries with no stockable carrier.
- Board fully re-minted on b10659 under release scope; site regenerated; M4 canary rows
  fresh; M1 canary rows retired.
- Ledger updates: memory `test_coverage_debt.md` items closed; llvm_tune per-kernel-hash
  hardening ledgered.
