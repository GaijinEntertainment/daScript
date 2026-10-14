# dasLLAMA Tokenizer Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_ENGINE_FORMATS.md`. Planned work: `followup_general.md`.

**A diff whose hunks change code in `dasllama/dasllama_spm.das`, `dasllama/dasllama_bpe.das` or
`dasllama/dasllama_pretok.das` that `dasllama_spm.das`'s `encode` or `dasllama_bpe.das`'s
`bpe_encode` runs, or that fills a lookup table either one reads, names in the PR body before/after
`--tok` rows (`benchmarks/lcpp_bench.das`) at two or more sizes of its ladder, on a model whose
tokenizer runs the changed code.**

**In a diff's before/after `--tok` rows (`benchmarks/lcpp_bench.das`), divide each size's time by
the next smaller size's time; a ratio that rises more than 10% from before to after, or an after
ratio of 16 or more, is a defect.** The ladder steps 4x, so a ratio of 16 is quadratic growth.

**Every diff this checklist reviews names in the PR body a run of this folder's
`tests/test_tokenizer.das` with no case skipped.** A case skips when its `ggml-vocab-*.gguf` fixture
is absent, so the run needs every fixture present.
