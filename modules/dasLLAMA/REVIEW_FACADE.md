# dasLLAMA Facade Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_RUNTIME.md`. Planned work: `followup_general.md`.

**A non-private, non-operator def of a file `REVIEW.das`'s `FACADE_FILES` lists, and each new
overload of one, is called in runnable code in a `tutorials/dasLLAMA/*.das` source - not a comment
or a passing mention - with that overload's argument types, and narrated on a
`doc/source/reference/tutorials/dasLLAMA_*.rst` page.**

**A diff that adds a `public` require, or makes a require `public`, in `dasllama/dasllama.das`
or in a file `dasllama/dasllama.das` reaches through `public` requires only, adds the required
file to `REVIEW.das`'s `FACADE_FILES` in the same change.**

**A NEW `[EnvConfig]` area struct is rendered by `env_markdown()` in the same change.** A struct
the renderer never emits is absent from `ENVIRONMENT.md` and every test.
