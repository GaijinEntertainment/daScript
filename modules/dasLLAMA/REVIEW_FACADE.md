# dasLLAMA Facade Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_RUNTIME.md`, `ARCHITECTURE_TTS.md`. Planned work:
`followup_general.md`.

**Every non-private, non-operator def that a user program calls, in a file `REVIEW.das`'s
`FACADE_FILES` lists - each new overload, and each def a diff adds a parameter to, included - is
called with that overload's argument types in runnable code (not a comment or a passing mention)
in a `tutorials/dasLLAMA/*.das` source, and narrated on a
`doc/source/reference/tutorials/dasLLAMA_*.rst` page.** A def no user program calls - the runtime,
a loader chain, the scheduler, a server or a bench calls it - is a row of `REVIEW.das`'s
`FACADE_DEFS_WITHOUT_TUTORIAL`, the gate's ledger.

**A diff that adds a `public` require, or makes a require `public`, in `dasllama/dasllama.das`
or in a file `dasllama/dasllama.das` reaches through `public` requires only, adds the required
file to `REVIEW.das`'s `FACADE_FILES` in the same change.**

**A NEW `[EnvConfig]` area struct is rendered by `env_markdown()` in the same change.** A struct
the renderer never emits is absent from `ENVIRONMENT.md` and every test.
