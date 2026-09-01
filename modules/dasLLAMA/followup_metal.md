# Metal / mac-session follow-up - the M5 catch-up ledger

**Status: OPEN (2026-08-31, unquirk pass).** The ledger for the parallel mac session that
forks after PR-1 of `plans/unquirk_pass.md`. Metal-tier perf items move here from the
per-format notes during the Phase E doc split; the CPU items below are mac-session work too -
the M-series CPU tiers are minted and raced from that box.

## 1. The dark smmla (i8mm) leg - free race, never run

`q8q8_tile_gen` ships five `dot = "smmla"` seats gated `requires = "i8mm"` (mr4/mr8 x
kstep/nrsplit/gkstep), and NO box has ever raced them: `LLVMGetHostCPUFeatures()` returns an
EMPTY string on macOS (llvm_jit_common.das documents it beside `g_target_arm64_i8mm`), so
i8mm never detected on Apple Silicon at the EMITTER tier (`g_target_arm64_i8mm`), so the
generators declined and the seats never raced - even though `cpu_supports("i8mm")` answered
correctly via sysctl. M1 lacks i8mm; M2+ has it; the M5 Max additionally has FEAT_SME2p1 +
BF16/EBF16. FIXED in the unquirk pass (278b3c765): `g_target_arm64_i8mm` also consults
`cpu_supports`, and the target machine appends `+i8mm` when the host has it - an M5 `--tune`
now races smmla-vs-NEON with zero new kernel work. Mac-session order:

1. On the M5 (post PR-1): `--tune`, confirm the smmla seats EMIT (they have never been
   exercised - treat the emitter arms as unproven, gate with the gen probe TEST mode first)
   and report the crowns; export the `arm-i8mm` profile if they win.
2. If smmla wins q8q8: the kq tile families have NO ARM ISA seats at all (mr8 NEON is the
   whole grid) - an smmla kq tile emitter arm is the highest-leverage CPU kernel work on
   the mac, and it transfers to Graviton3+ (c8g) verbatim.
3. SME/SME2 is the tier after: no seats, no emitter, new kernel design (streaming mode +
   ZA tiles) - research first per the standing research-before-kernel-work rule.

## 2. Metal tier perf items

Placeholder - the per-format Metal notes (tg 0.78-0.93x tails, the IQ4_XS lane-map gap of
followup_general #58, the Q22 dispatch-loop probe method) consolidate here in Phase E of
`plans/unquirk_pass.md`; until then they live in `HOW_TO_ADD_A_FORMAT.md`'s per-format notes.
