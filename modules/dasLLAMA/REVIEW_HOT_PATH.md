# dasLLAMA Hot-Path Annotation Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_RUNTIME.md`. Planned work: `followup_general.md`.

A function calls another when its body names that function in a call, a call written inside a
block or lambda literal included; the function that encloses the literal is the caller. A
function reaches another when it calls it, or calls a function that reaches it. A serving step
is one unit of served work the runtime re-enters a path for: a token, a prefill quantum (one
batch of prompt tokens the prefill path processes in a single pass), one encoded media input (an
image, a video frame, an audio chunk), or one synthesized speech chunk or frame. A region entry is
the outermost function the runtime re-enters once per serving step, and any function a serving
step runs through a function value - one a seat (a module-level variable holding a function,
filled through a public setter) or a kernel-backend override registration stores, after its
declaration, for the runtime to call through - where no `[hot_path]`, contract or `[cold_path]`
function reaches the `invoke`. An interior function is a function, not itself a region entry, that
has at least one caller and whose every caller the runtime re-enters once per serving step
(`ARCHITECTURE_RUNTIME.md#the-hot-path-coverage-model`).

**A diff that adds a kernel dispatch, a loop or a call that the runtime re-enters once per serving
step makes sure the function that records the dispatch or holds the loop or call is, or is reached
by, a region entry that carries `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]`
contract.**

**A function that a serving step runs through a function value, where no `[hot_path]`, contract or
`[cold_path]` function reaches the `invoke`, carries `[hot_path]`, a `[no_alloc]` / `[no_env]` /
`[no_io]` contract, or `[cold_path]`.** The allocation lint follows an `invoke` only from inside an
annotated region, so nothing else covers that path into the function.

**A `[cold_path]` function that a serving step calls, or runs through a function value, with no
guard around the call or the `invoke` - a guard that skips it on most steps of every generation
that runs it at least once - is a defect: split the rarely-taken part (a rebuild, a first-use
allocation, a log) into its own `[cold_path]` function behind the guard that keeps it rare, and leave the function every serving
step reaches unmarked.** The annotation is a promise about how often the function runs, and the
allocation lint stops walking at it.

**A diff that renames a function carrying `[hot_path]`, `[cold_path]` or a `[no_alloc]` /
`[no_env]` / `[no_io]` contract, or inserts a def between that annotation and the def it
annotated, keeps the annotation on the function it annotated while that function is still the
region entry or the rarely-taken branch - on the new name after a rename.**

**A `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract on an interior function is a
defect - move it to the region entry; an interior function carries only a `[cold_path]` on a
rarely-taken branch.**

**Never put `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract on a function no
serving step reaches - it is no region entry; it carries `[cold_path]` or nothing.**

**A diff that adds or changes a non-`[test]` function under this module's `tests/`, `harness/`,
`benchmarks/` or `performance/` that reaches a region entry gives it `[cold_path]` where no other
non-`[test]` function under those four folders reaches it, and no annotation otherwise.**

**A diff that adds or changes a non-`[test]` function under `tests/`, `harness/`, `benchmarks/` or
`performance/` that reaches a region entry leaves no `[cold_path]` on any function under those
four folders that it reaches.**
