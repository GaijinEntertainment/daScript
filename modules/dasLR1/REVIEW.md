# dasLR1 Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**A diff under `lr1/` that adds a `require` of a module outside `daslib/` and the builtin set is a
defect** - the module is reused by programs that never load dasLLAMA; a caller-specific need is met at
the caller.

**A diff that lets `build_grammar` return a `Grammar` with an empty `err` for a grammar whose reachable
rule derives no terminal string is a defect** - the acceptor's refusal of a byte then no longer means
the grammar has no sentence past it (`ARCHITECTURE.md#viable-prefix`).

**A diff that resolves a table conflict by keeping one action and dropping the others is a defect** -
keep every action of the cell in `alt`, and let the stack set carry the alternatives
(`ARCHITECTURE.md#stack-set`).

**A diff that makes `lr_step` accept a byte the stacks cannot shift after their reduces, or `lr_at_end`
read acceptance other than through the reduce chains at the EOF column, is a defect** - a sampler masks
tokens through them and never undoes an emitted prefix.

**A diff that makes `json_schema_to_gbnf` emit text the conversion corpus does not expect is a
defect** unless the same diff lifts the newer reference corpus whose text it matches - the corpus is the
compiler's oracle (`ARCHITECTURE.md#ported-converter`). A schema the reference refuses and this
compiler serves adds a conversion case of its own, with the text it emits.

**A diff that edits a lifted corpus file - `tests/_gbnf_corpus.json`, `tests/_schema_corpus.json`, a
file under `tests/_gbnf/` - other than to lift a newer reference copy whole is a defect** - a hand-edited
corpus proves whatever its editor wanted; a case of this module's own goes in a test file beside them.
A corpus file's `.LICENSE` sibling stays with it.

**A diff that teaches `gbnf_grammar` a token reference (`<[id]>`, `<name>`, `!<...>`) is a defect** -
the acceptor sees bytes; map a token to its bytes at the caller instead.

**A diff that raises `_cyclomatic_complexity` or `_function_length` in a file under `lr1/` other than
`lr1/json_schema.das`, or raises them further there, is a defect** - the raise covers the ported
converter's shape alone; new code meets the default limits, or splits.

**A diff that adds a grammar feature, a schema keyword, or an acceptor verb ships a case in `tests/`
with strings the grammar accepts and strings it refuses, in the same change** - acceptance alone passes
on a grammar that accepts everything. The module's tests live under `tests/` here, never under the
repository's `tests/`.
