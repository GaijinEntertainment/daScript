# dasLLAMA Facade Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_RUNTIME.md`. Planned work: `followup_general.md`.

**A def of a facade file - one whose defs reach a consumer through `require dasllama/dasllama`
(`REVIEW.das`'s `FACADE_FILES`) - and each new overload of one is called in runnable code in a
`tutorials/dasLLAMA/*.das` source and narrated on a `doc/source/reference/tutorials/dasLLAMA_*.rst`
page; a comment or passing mention does not count, and a new overload's tutorial calls it with
that overload's argument types.** `REVIEW.das`'s `check_tutorial_floor` matches names only.

**A diff that makes another file's defs reach a consumer through `require dasllama/dasllama`
adds that file to `REVIEW.das`'s `FACADE_FILES` in the same change.**

**A NEW `[EnvConfig]` area struct is rendered by `env_markdown()` in the same change.** A struct
the renderer never emits is absent from `ENVIRONMENT.md` and every test; one it emits but the
registry does not, `tests/test_env_registry.das` catches.
