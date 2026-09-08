# Shader emitter follow-ups (dasSpirv, dasMetal, the kernels they compile)

The ledger for work that spans both emitters and the language surface they lower. Present-tense
mechanism lives in `modules/dasSpirv/ARCHITECTURE.md` and `modules/dasMetal/ARCHITECTURE.md`;
the binding rules in `modules/REVIEW_SHADER_EMITTERS.md`. This file holds what is owed.

1. **Eager operands as language surface; the emitter's purity heuristic goes.** Ruled 2026-09-06
   (Boris: operators, after the Vulkan arc). Today `?:`, `&&` and `||` are lazy by the language;
   dasMetal emits them natively (C short-circuit), and dasSpirv lowers an operand as a branch only
   when it indexes a global-rooted array, keeping `OpSelect` / `OpLogicalAnd` / `OpLogicalOr` for
   operands it takes as pure (`modules/dasSpirv/ARCHITECTURE.md`, "Operand laziness follows the
   language"). That eager-when-pure lowering is an optimization a kernel author cannot count on,
   and its trigger is syntactic: a helper call that indexes a buffer inside an operand still runs
   eagerly. The plan, in order:
   - `bool` gains `&`, `|` and `^` (both sides evaluated, as every C-family shading language reads
     them), scalar and `boolN`; the CPU tier, the JIT and AOT get them as plain builtins.
   - `select(cond, a, b)` lands in `daslib/shader_lingua_franca` as an ordinary function - a call
     evaluates its arguments first by the language, so it is eager on every tier by construction;
     the CPU body is `cond ? a : b` over the evaluated arguments; dasSpirv lowers it to `OpSelect`
     (component-wise for `boolN` conditions), dasMetal to MSL `select`. The name is HLSL's and
     Metal's.
   - dasSpirv drops the purity heuristic: `?:`, `&&`, `||` lower as branches always, no analysis,
     and the helper-call gap closes by construction. `SpirvGlobalIndexScan` / `indexes_global_array`
     / the `lazy_temps` allocation become the unconditional branch lowering.
   - The live kernel sites that want branchless code convert to `select` / `&` / `|` in the same
     change: the 12 sites the 2026-09-05 census listed (`dasllama_vulkan_classes.das`: the
     AtAttnTileT loaders, DnBaGemm's tail guards, DnStep's dq/dk picks, the RoPE store's cossin
     reads, DaAttnB's mask over the score tile, TopK's shared reads, the conv's `nsc[ch/ds]`, AR's
     `aa[..]`, `acc_rows`), re-measured on the 30B router bucket and the 9B window per
     `modules/dasLLAMA/REVIEW_MEASUREMENT.md`.
   - `modules/REVIEW_SHADER_EMITTERS.md`'s operand rule shrinks to "branchless where it matters is
     spelled `select` / `&` / `|`", and dasSpirv reports one note per `?:`/`&&`/`||` it branched
     inside a `[spirv_kernel]` or `[compute_shader]` body, naming the operator's line, so the rule
     retires to "weakening that note is a defect".
   Done = the operators and `select` documented in `skills/daslang/`, the emitter heuristic gone,
   the census sites converted with the two measurements beside them.

2. **`for [unroll_full]` unrolls at emission; a fixed-array local with constant indices becomes
   registers.** Found 2026-09-07 by the integer GEMM tile prototypes
   (`modules/dasLLAMA/harness/vk_gemm_probe.das -- mmqx`, `modules/dasLLAMA/followup_vulkan.md`
   item 42): a hinted loop leaves a `var acc : float[64]` local a Function-storage `OpVariable`
   indexed by the loop counter unless the driver both unrolls and scalarizes, and a 128-accumulator
   block - or a coopmat tile's sixteen accumulator fragments - is writable only as an array.
   LANDED 2026-09-08 (`modules/dasSpirv/ARCHITECTURE.md` section 3.6): `[unroll_full]` with
   literal bounds emits the body once per copy with the induction variable an `OpConstant`, folds
   the integer arithmetic on it so `acc[t * 16 + c]` chains a literal index, and refuses `break`,
   `continue` and a runtime bound; `[unroll]` stays the driver's `Unroll` hint. The spelling is the
   one the JIT (`llvm.loop.unroll.full`) and dasMetal (`#pragma clang loop unroll(full)`) already
   lower, so a kernel source reads the same on every tier, and no shipped SPIR-V kernel changed (the
   golden set is byte-identical). Fixture `ufor` in `tests/spirv` (zero `OpLoopMerge`, 23
   constant-index chains, the fold assertions) and two fail-closed fixtures. Still owed: the note
   channel - the unroll's copy count and item 1's per-operator note share one channel once item 1
   lands (a `to_log` at `LOG_DEBUG` prints on every kernel compile, so nothing is reported today);
   and the first kernel written on it, the KHR coopmat tile's accumulator block (item 42), whose
   device cells are the runtime gate a fixture cannot be.
   Done = the note, and the tile's cells green on the array form.
