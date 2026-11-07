# dasLLAMA Hot-Path Annotation Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_RUNTIME.md`. Planned work: `followup_general.md`.

The terms below are short forms; each one's full boundary is in
`ARCHITECTURE_RUNTIME.md#hot-path-terms`, and where the annotations sit and why is
`ARCHITECTURE_RUNTIME.md#the-hot-path-coverage-model`.

A function calls another when its body, a block or lambda literal in it included, names that
function in a call; an `invoke` calls none. A function reaches another when it calls it or calls a
function that reaches it, and runs under any function that reaches it or reaches the `invoke` that
runs it.

A serving step is one unit of work the runtime repeats inside one served request: a token, a
prefill batch, one encoded image, video frame or audio chunk, or one synthesized speech chunk.

A seat is a module-level variable holding a function, filled through a public setter. A region
entry is the outermost function the runtime re-enters once per serving step, or a function a
serving step runs through a seat or a kernel-backend override where no `[hot_path]`, contract or
`[cold_path]` function reaches the `invoke`. An interior function is a function, not a region
entry, with a caller under `dasllama/` a serving step reaches, every such caller re-entered once
per serving step.

**A diff that adds a kernel dispatch, a loop or a call that the runtime re-enters once per serving
step makes sure the function that records the dispatch or holds the loop or call is, or runs
under, a region entry that carries `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]`
contract.**

**A function that a serving step `invoke`s from a seat filled in a module other than the reaching
annotated function's carries `[hot_path]`, a `[no_alloc]` / `[no_env]` / `[no_io]` contract, or
`[cold_path]`.** The allocation lint does not follow an `invoke` into a function another module
filled the seat with, so nothing else covers that path into the function.

**A `[cold_path]` function that a serving step calls, or runs through a function value, with no
guard around the call or the `invoke` - a guard that skips it on most steps of every request
that runs it at least once - is a defect: split the rarely-taken part (a rebuild, a first-use
allocation, a log) into its own `[cold_path]` function behind the guard that keeps it rare, and
leave the function every serving step reaches unmarked.** The annotation is a promise about how
often the function runs, and the allocation lint stops walking at it.

**A diff that renames a function carrying `[hot_path]`, `[cold_path]` or a `[no_alloc]` /
`[no_env]` / `[no_io]` contract, or inserts a def between that annotation and the def it
annotated, keeps the annotation on the function it annotated while that function is still the
region entry or the rarely-taken branch - on the new name after a rename.**

**A `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract on an interior function is a
defect - drop it, and when no function the interior function runs under still carries `[hot_path]`
or that contract, put it on the region entry the interior function runs under; an interior function
carries a `[cold_path]` only on a rarely-taken branch.**

**Never put `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract on a function no
serving step reaches through a call or an `invoke` - it is no region entry; it carries
`[cold_path]` or nothing.**

**A non-`[test]` function a diff adds or changes under this module's `tests/`, `harness/`,
`benchmarks/` or `performance/` that reaches a `[hot_path]`, `[no_alloc]`, `[no_env]` or `[no_io]`
function puts one `[cold_path]` on the outermost non-`[test]` function of those four folders on
each chain that reaches it, from a `[test]` or from a function nothing in those folders calls.**

**A diff that adds or changes a non-`[test]` function under this module's `tests/`, `harness/`,
`benchmarks/` or `performance/` that reaches a `[hot_path]`, `[no_alloc]`, `[no_env]` or `[no_io]`
function leaves no `[cold_path]` on a function of those four folders that is the outermost
non-`[test]` one on no chain reaching that function from a `[test]` or from a function nothing in
those folders calls - a helper shared by two chains included, whether the diff edits it or not;
each chain's outermost one carries it.**
