# dasLLAMA Facade Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
docs: `ARCHITECTURE_ENGINE.md`, `ARCHITECTURE_RUNTIME.md`. Planned work: `followup_general.md`.

**A non-private, non-operator def of a file `REVIEW.das`'s `FACADE_FILES` lists, and each new
overload of one, is called in runnable code in a `tutorials/dasLLAMA/*.das` source - not a comment
or a passing mention - with that overload's argument types, and narrated on a
`doc/source/reference/tutorials/dasLLAMA_*.rst` page.**

**A diff that adds a `public` require, or makes an existing require `public`, on a chain from
`dasllama/dasllama.das` - so the required file's defs reach a consumer through
`require dasllama/dasllama` - adds that file to `REVIEW.das`'s `FACADE_FILES` in the same
change.**

**A NEW `[EnvConfig]` area struct is rendered by `env_markdown()` in the same change.** A struct
the renderer never emits is absent from `ENVIRONMENT.md` and every test; one it emits but the
registry does not, `tests/test_env_registry.das` catches.
