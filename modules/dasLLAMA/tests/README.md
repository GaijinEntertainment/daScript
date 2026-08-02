# dasLLAMA tests

These module-owned suites are intentionally outside the core `tests/` tree. Run them through
the scoped runner — its environment is part of the suites' contract ([CLAUDE.md](CLAUDE.md)):

```text
bin/daslang -jit modules/dasLLAMA/tests/run.das -- --arm <filter> [--suite <name>]
```

Standalone suites (`test_chat`, `test_whisper`, `test_audio`, ...) may be run via dastest
directly, with `DASLLAMA_CPU_PREFILL=1` exported. Model-driven tests self-skip when their
fixture models are absent; never launch the whole directory unfiltered.

The suite covers architecture registration, audio and Whisper, batching and dispatch, the public
facade, forward and parity fixtures, quantization and KV codecs, CPU and Metal kernels, model images,
MTP, tokenization, sampling, Unicode, and the tune/tuned integration surfaces.
