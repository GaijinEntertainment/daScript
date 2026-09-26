# dasLLAMA Hot-Path Annotation Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_RUNTIME.md`. Planned work: `followup_general.md`.

A function calls another when its body names that function in a call, a call written inside a
block or lambda literal included; the function that encloses the literal is the caller.

**Every kernel dispatch (the host function that records it), loop or call path a diff adds that
the runtime re-enters once per serving step - a token, a prefill quantum (one batch of prompt
tokens the prefill path processes in a single pass), a media encode (an image, a video frame, an
audio chunk) - is reached by an annotated region entry: `[hot_path]`, any of the `[no_alloc]` /
`[no_env]` / `[no_io]` contracts, or `[cold_path]` on the guarded, rarely-taken function that is
the path's only entry.** The region entry is the OUTERMOST such function - interior means every
caller is itself re-entered that way, so a function reached only through a registered function
value is an entry (`ARCHITECTURE_RUNTIME.md#the-hot-path-coverage-model`).

**A `[cold_path]` on a function that each serving step reaches unconditionally is a defect - split
the rarely-taken part (a rebuild, a first-use allocation, a log) into its own `[cold_path]`
function behind the guard that keeps it rare, and leave the function every serving step reaches
unmarked.** The annotation is a promise about how often the function runs, and the allocation
lint stops walking at it.

**A diff that renames a function carrying `[hot_path]`, `[cold_path]` or a `[no_alloc]` /
`[no_env]` / `[no_io]` contract, or inserts a def textually above it in the file, keeps that
annotation on the function it annotated while that function is still the region entry or the
rarely-taken branch - on the new name after a rename, never on the def the diff inserted above
it** - it is no new entry.

**A `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract on a function below the
region entry is a defect - move it to the entry; an interior function carries only a
`[cold_path]` on a rarely-taken branch.**

**Never put `[hot_path]` or a `[no_alloc]` / `[no_env]` / `[no_io]` contract on a loop reached
only from a load, stage, bake, or convert path - it is no region entry; it carries `[cold_path]`
or nothing.**

**A function a diff adds or changes under this module's `tests/`, `harness/`, `benchmarks/` or
`performance/` that is not a `[test]` and calls into a region entry carries `[cold_path]` if no
other such non-`[test]` function calls it, and no annotation otherwise.**

**A diff that adds a non-`[test]` caller above a `[cold_path]` function under this module's
`tests/`, `harness/`, `benchmarks/` or `performance/` removes that function's `[cold_path]` in the
same change.**
