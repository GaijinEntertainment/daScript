# daslib Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`daslib/ARCHITECTURE.md`.

**A lint report path checks suppression BEFORE its dedup insert, and the rule code stays part
of the dedup key.** A nolint'd finding that takes the (rule, location) slot silences a
co-located sibling rule; a key without the code collides across rules.

**PERF026-028 honor a `// nolint` at ANY frame of the reported chain.** The sink often
bottoms out in daslib while the actionable line is an intermediate call site — narrowing the
check to the sink line is a defect.

**Moving an `in_closure` / `in_deferred` guard inside a lint visitor method is a semantic
change, never cleanup.** The guard's position encodes which rules are meaningful inside a
lambda; the per-rule policy is in `daslib/ARCHITECTURE.md`.

**A lint warning anchors at the code its fix rewrites.** A remedy that deletes a statement
reports the statement; a remedy that edits one variable's initializer reports the variable —
an anchor chosen for implementation convenience instead is a defect.

**Per-loop visitor state is saved and restored per loop — a stack, never a bare scalar.**
A scalar survives into the sibling loop's exit path and unbalances its counter for the rest
of the walk.
