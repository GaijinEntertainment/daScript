# dasLLAMA Hot-Path Annotation Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_RUNTIME.md`. Planned work: `followup_general.md`.

A function calls another when its body names that function in a call, a call written inside a
block or lambda literal included; the function that encloses the literal is the caller. A
function reaches another when it calls it, or calls a function that reaches it. A serving step
is one unit of served work the runtime re-enters a path for: a token, a prefill quantum (one
batch of prompt tokens the prefill path processes in a single pass), one encoded media input (an
image, a video frame, an audio chunk), or one synthesized speech chunk or frame. A region entry is
the outermost function the runtime re-enters once per serving step. An interior function is a
function, not itself a region entry, that has at least one caller and whose every caller the
runtime re-enters once per serving step (`ARCHITECTURE_RUNTIME.md#the-hot-path-coverage-model`).

**A diff that adds a kernel dispatch (the host function that records it), a loop or a call path
that the runtime re-enters once per serving step makes sure it is, or is reached by, a region
entry that carries `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract - or that its
only way in is one `[cold_path]` function a serving step calls behind a guard that skips the call
on most steps.**

**A function a serving step reaches only through a registered function value - a function stored,
after its declaration, in a table or variable that the runtime calls through - is a region entry,
and carries `[hot_path]`, a `[no_alloc]` / `[no_env]` / `[no_io]` contract, or `[cold_path]`.** No
caller in the call graph re-enters it, so nothing above it can carry the annotation for it. The
default value a table or variable is declared with is not a registered function value.

**A `[cold_path]` function that a serving step calls with no guard around the call - a guard
that skips the call on most steps of every generation that calls it at least once - is a
defect: split the rarely-taken part (a rebuild, a first-use allocation, a log) into its own
`[cold_path]` function behind the guard that keeps it rare, and leave the function every serving
step reaches unmarked.** The annotation is a promise about how often the function runs, and the
allocation lint stops walking at it.

**A diff that renames a function carrying `[hot_path]`, `[cold_path]` or a `[no_alloc]` /
`[no_env]` / `[no_io]` contract, or inserts a def textually above it in the file, keeps that
annotation on the function it annotated while that function is still the region entry or the
rarely-taken branch - on the new name after a rename, never on the def the diff inserted above
it.**

**A `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract on an interior function is a
defect - move it to the region entry; an interior function carries only a `[cold_path]` on a
rarely-taken branch.**

**Never put `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract on a function
reached only from a load, stage, bake, or convert path - it is no region entry; it carries
`[cold_path]` or nothing.**

**A diff that adds or changes a non-`[test]` function under this module's `tests/`, `harness/`,
`benchmarks/` or `performance/` leaves two things true in the same change: that function, where
it reaches a region entry, carries `[cold_path]` if no other non-`[test]` function under those
folders reaches it, and no annotation otherwise; and no function under those folders that it
reaches carries `[cold_path]`.**
