# dasLLVM Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE.md` (beside this file), `../ARCHITECTURE.md`.

**A `[test]` file that carries a `require dasllama/...` line of its own answers to
`modules/dasLLAMA/REVIEW.md` as well.**

**A diff that adds a `[test]` anywhere under `modules/dasLLVM/` puts it in an existing file of
this folder that holds tests of the same topic, or in `uncategorized.das`, never in a new file.**

**A `[test]` a diff adds or changes that builds an executable or a library, spawns a child
daslang, or runs the tuner carries `timeout = 1200`.**

**Weakening the `check_test_shape` call in `REVIEW.das` (beside this file) - removing it, raising
its `cap` argument, or passing `tests_only = true` - is a defect.**

**A test never creates, overwrites, or deletes a git-tracked path - it works on a temp copy it
makes itself.**

**A test that writes - through its own filesystem calls, not a child process's and not the JIT
cache an in-process JIT run writes - writes only to paths it created uniquely for this process (a
temp file or a temp directory), and removes them** -
two runs at once collide on a shared path under `build/` (repo root).

**A test that deletes a directory recursively deletes only a directory it created itself, or the
folder `round_cache_folder` (`_llvm_cache_common.das`) returns for a script it copied into such a
directory.** A failed in-process JIT run returns an empty `output_path`, and `dir_name` of an
empty path can be the working directory.

**A test that spawns a daslang child keeps every path the child writes inside a directory `<dir>`
the test created uniquely for this process.** A child writes its two caches - the front-end
module cache and the JIT cache - under the cwd unless these move or stop them: `-output
<dir>/...` (an `-exe` build, the JIT cache), `-module-cache <dir>/...` or `-no-module-cache`
(the front-end cache), `-jit-no-cache` (the JIT cache), a working directory inside `<dir>`
(both caches). A child the test hands `DAS_JIT_MANIFEST=<dir>/...` also writes the record of
the cached DLLs it used into that directory.

**A test whose child compiles through the front-end module cache - any child but a `-exe`,
`-compile-only`, `-documentation`, debugger or AOT run - and asserts a line a macro prints at
compile time, or its absence, spawns that child with `-no-module-cache`; a test that checks the
cache itself spawns it with `-module-cache` naming its own temp file instead, never with
`-no-module-cache`.** A cache hit runs no macro,
so that line prints only on a cold cache (`../ARCHITECTURE.md#macro-line-cache-replay`).

**Weakening `llvm_env_registry.das` (beside this file) is a defect** - dropping a scan, narrowing
its `STRICT_ROOTS`, or widening its `SKIP_FILES` weakens it.

**Weakening `llvm_tune_manifest.das` (beside this file) is a defect** - deleting a `[test]`, or
dropping or loosening an assertion weakens it.

**A test that searches the text of the LLVM IR the JIT emits matches an instruction by its opcode
and flags (`fmul reassoc`), never by a numbered register (`%12`).** The numbering shifts with any
unrelated change to the emitted function.

**A `[test]` comparing a float emitter's emitted and interpreted results also asserts both answer
NaN in the same lanes.** A clamp or a conversion written with ordered compares turns a NaN lane
into a number, and an accuracy bound reads that as success.
