# dasLLAMA Tokenizer Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_ENGINE_FORMATS.md`. Planned work: `followup_general.md`.

**Routed from `REVIEW.md`: a diff that checklist routes here applies this list together with it.**

**A change to `encode`/`bpe_encode`, or to a function they call at encode time (not one that only
supplies a metadata default at load) in `dasllama/dasllama_spm.das`, `dasllama/dasllama_bpe.das`
or `dasllama/dasllama_pretok.das` - `encode` being `dasllama_spm.das`'s and `bpe_encode`
`dasllama_bpe.das`'s - ships before/after `--tok` rows (`benchmarks/lcpp_bench.das`) at
two or more input sizes on a model using that tokenizer; a time growing faster than linearly with
input size is a defect.**

**A change to code or data in `dasllama/dasllama_tokenizer.das`, `dasllama/dasllama_spm.das`,
`dasllama/dasllama_bpe.das`, or `dasllama/dasllama_pretok.das`, or to the special-token or
template strings any of them look up, names in the PR body a run of this folder's
`tests/test_tokenizer.das` with its cases EXECUTED, not skipped.**
