# dasLR1 Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**A diff under `lr1/` that adds a `require` of a module other than one under `lr1/`, one under `daslib/`,
or a builtin module the daslang executable registers itself (such as `strings` or `math`) is a defect** -
the module is reused by programs that load nothing under `modules/`; a caller-specific need is met at the
caller.

**A diff that lets `build_grammar` return a `Grammar` with an empty `err` for a grammar in which a rule the
root reaches derives no terminal string is a defect** - the acceptor would then accept a byte after which
no sentence can be completed (`ARCHITECTURE.md#viable-prefix`).

**A diff that resolves a table cell more than one action claims by keeping one action and dropping the
others is a defect** - keep the cell's first action in `action` and every other in `alt`, keyed by the
cell index, and let the stack set carry them (`ARCHITECTURE.md#stack-set`).

**A diff that makes a public `lr_*` function answer whether a byte may come next, or whether the input may
end, other than through the stacks' reduce chains to a shift (or to the accept action on the end-of-input
column) is a defect** - a sampler masks tokens through these answers and never undoes an emitted prefix
(`ARCHITECTURE.md#viable-prefix`).

**A diff that makes the acceptor drop a live stack when the set would pass `LR_MAX_STACKS` is a defect** -
refuse the byte and leave the set as it was (`ARCHITECTURE.md#stack-set`).

**A diff that weakens `test_schema_conversion_corpus` in `tests/test_json_schema.das` - a case skipped, the
comparison loosened past whitespace - is a defect** - that test is what holds `json_schema_to_gbnf` to the
reference converter's text (`ARCHITECTURE.md#ported-converter`).

**A diff that removes or narrows a refusal in `json_schema_to_gbnf` adds a case to
`tests/test_json_schema.das` with the text it now emits, in the same change.**

**A diff that changes `tests/_gbnf_corpus.json` or `tests/_schema_corpus.json` other than by rewriting it
with `tests/_lift_corpora.das` over a llama.cpp checkout, naming that checkout's commit in
`ARCHITECTURE.md` in the same change, is a defect** - a hand-edited corpus no longer checks the module
against llama.cpp's own cases; a case of this module's own goes in a test file beside them.

**A diff that changes a file under `tests/_gbnf/` other than by copying llama.cpp's `grammars/` file of the
same name whole is a defect.**

**A diff that drops `tests/_gbnf/LICENSE`, `tests/_gbnf_corpus.json.LICENSE` or
`tests/_schema_corpus.json.LICENSE` is a defect** - each carries the MIT notice of the files it covers.

**A diff that makes `gbnf_grammar` accept a token reference - `<[id]>`, `<name>`, `!<...>` - is a defect** -
the acceptor sees bytes; map a token to its bytes at the caller instead.

**A diff that raises `_cyclomatic_complexity` or `_function_length` in a file under `lr1/`, or adds code that
is not the ported converter's to `lr1/json_schema.das`, is a defect** - new code meets the default limits or
the function splits, in a file with no raise (the one ledgered raise: `ARCHITECTURE.md#ported-converter`).

**A diff that adds a construct `gbnf_grammar` reads, a schema keyword `json_schema_to_gbnf` handles, or a
public `lr_*` function ships, in the same change, a case under `tests/` with inputs the engine admits and
inputs it refuses** - acceptance alone passes on a grammar that accepts everything.
