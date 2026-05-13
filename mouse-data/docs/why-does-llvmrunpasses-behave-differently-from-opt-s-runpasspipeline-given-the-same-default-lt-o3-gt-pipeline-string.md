---
slug: why-does-llvmrunpasses-behave-differently-from-opt-s-runpasspipeline-given-the-same-default-lt-o3-gt-pipeline-string
title: Why does LLVMRunPasses behave differently from opt's runPassPipeline given the same `default&lt;O3&gt;` pipeline string?
created: 2026-05-12
last_verified: 2026-05-12
links: []
---

**The C-API entry point `LLVMRunPasses` is significantly less set-up than opt's `runPassPipeline`.** Same pipeline string, different analysis-manager registration. Empirically (LLVM 22.1.5, /tmp/queen-ir/ session):

- Daslang's `optimize_llvm_module` calls `LLVMRunPasses(M, "default<O3>", TM, opts)` → outer `for c=0..8` loop in addqueen stays rolled, no TRE.
- Running `/opt/homebrew/opt/llvm/bin/opt -passes='default<O3>'` on the same IR → outer loop fully unrolled, TRE fires (`tailrecurse` block).

**What opt's `runPassPipeline` does that `LLVMRunPasses` skips** (verified against LLVM 22.1.5 source — `lib/Passes/PassBuilderBindings.cpp::runPasses` vs `tools/opt/NewPMDriver.cpp::runPassPipeline`):

1. `FAM.registerPass([&] { return TargetLibraryAnalysis(*TLII); })` with a **platform-aware** `TargetLibraryInfoImpl(Mod->getTargetTriple())`. The C-API's default `registerFunctionAnalyses` registers a default TLA with no platform info.
2. `MAM.registerPass([&] { return RuntimeLibraryAnalysis(...); })` and `LibcallLoweringModuleAnalysis()` (LLVM 22 only, headers `llvm/Analysis/RuntimeLibcallInfo.h` + `llvm/CodeGen/LibcallLoweringInfo.h`).
3. `TM->setPGOOption(P)` — even for NoPGO (sets to `std::nullopt` explicitly).
4. `registerEPCallbacks(PB)` — extension-point pipelines (PipelineStartEP, OptimizerLastEP, etc.). All gated on cl::opt strings — no-op in our case.
5. `cl::ParseCommandLineOptions` having parsed argv may set various `-unroll-*` / `-tail-call-elim-*` cl::opt globals to non-default values, even with no user flags.

**The LLVM-C API has no entry point to register a platform TLI on a `LLVMPassBuilderOptions`.** `LLVMAddAnalysisPasses` exists but is legacy-PM only.

**Tried fix — C++ shim** `modules/dasLLVM/src/das_llvm_passes.cpp` (branch `experiment/jit-shim`) mirroring opt's setup. Partial result:
- Got the **inner**-loop unroll to fire (addqueen body 64→173 IR lines).
- Did NOT get outer-loop unroll or TRE.
- Timing improvement: ~5%, within noise. Did not close the JIT-vs-C++ gap.

Suspected remaining cause: two LLVM instances loaded (daslang loads `<sdk>/lib/LLVM.dll` 147MB, shim links against `/opt/homebrew/opt/llvm/lib/libLLVM.dylib` 164MB — same `install_name` but different file content). cl::opt globals would diverge across instances.

**To make a shim viable long-term: bundle it into the aleksisch/llvm-shared-builds prebuilt** so it lives inside the same LLVM.dll daslang loads. Otherwise the two-instance issue undermines the experiment.

**Cheaper alternative** to try first: `LLVMParseCommandLineOptions` at JIT init to set the cl::opt defaults (`-unroll-threshold`, etc.) explicitly. May achieve opt-like behavior without a shim.

Where: session 2026-05-12, exploratory branch `experiment/jit-shim`, PR #2634 lands the host-CPU/features correctness fix only.

## Questions
- Why does LLVMRunPasses behave differently from opt's runPassPipeline given the same `default&lt;O3&gt;` pipeline string?
