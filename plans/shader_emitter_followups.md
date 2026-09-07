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
