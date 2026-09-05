# dasLLAMA tests

These module-owned suites are intentionally outside the core `tests/` tree. Run them through
the scoped runner - its environment is part of the suites' contract ([CLAUDE.md](CLAUDE.md)):

```text
bin/daslang -jit modules/dasLLAMA/tests/run.das -- --arm <filter> [--suite <name>]
bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite model-free          # every-change gate, no --arm
bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite stocked             # per-PR model coverage, no --exclude
bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite stocked --exclude test_ple_modes   # iteration form between PRs
bin/daslang -jit modules/dasLLAMA/tests/run.das -- --changed                   # after an edit: the areas the change reaches
```

A file of the `model-free` or `stocked` suite may be run via dastest directly -
`DASLLAMA_CPU_PREFILL=1 bin/daslang -jit dastest/dastest.das -- --test <file>` - always under
`-jit`: under the interpreter a model-gated cell skips, and a run of skips is not coverage.
Model-driven tests self-skip when their fixture models are absent; never launch the whole
directory unfiltered.

The suite covers architecture registration, audio and Whisper, batching and dispatch, the public
facade, forward and parity fixtures, quantization and KV codecs, CPU and Metal kernels, model images,
MTP, tokenization, sampling, Unicode, and the tune/tuned integration surfaces.
