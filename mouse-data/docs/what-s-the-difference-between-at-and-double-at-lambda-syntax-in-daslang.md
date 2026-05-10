---
slug: what-s-the-difference-between-at-and-double-at-lambda-syntax-in-daslang
title: What's the difference between `@(args) => expr` and `@@(args) => expr` in daslang, and when does the distinction matter?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

`@(args) { body }` / `@(args) => expr` is a **lambda** — a closure with captures. Its type is `lambda<(arg_types) : ret_type>`.

`@@(args) { body }` / `@@(args) => expr` is a **function pointer** — no captures allowed (it's a code-pointer + signature, with nothing to close over). Its type is `function<(arg_types) : ret_type>`.

`lambda<...>` and `function<...>` are NOT interchangeable. Passing a function pointer where a lambda is expected fails type-check:

```
no matching functions or generics: compute_signature(string const&,
    function<(p:string const):bool const>, array<string>)
... invalid argument 'predicate' (1). expecting
    'lambda<(repo_rel_path:string const):bool> const',
    passing 'function<(p:string const):bool const>'
```

**Rule of thumb:** default to `@(...)`. Use `@@(...)` only when the API explicitly takes a `function<...>` (FFI callbacks, certain dispatch tables, places where the runtime needs a no-allocation pointer). Going from `@` to `@@` is a silent narrowing — looks fine until you later need a capture and hit "no closures allowed".

If you DO need to capture into a lambda but get `error 31003: implicit capture by move requires unsafe` (or `... by reference requires unsafe`), the explicit form is:

```
unsafe(@ capture(& var) (args) => expr)   // by reference
unsafe(@ capture(:= var) (args) => expr)  // by move
```

Live examples:
- `utils/mcp/tools/cpp_common.das:ensure_cpp_index` — `unsafe(@ capture(& prune_dirs) (p : string) => ...)`
- `utils/find-dupe/main.das:432` — `@ capture(ref(jobs), ref(sources_by_id)) () { ... }`
- `utils/das-fmt/dasfmt.das:145` — `@ capture(& verified, := args) { ... }`
- `utils/benchctl/main.das:267` — `@capture(:= re, := parts) (x : string) : string { ... }`

The `unsafe(...)` wrap is required because both reference and move capture touch lifetime invariants the type-checker can't otherwise prove safe.

## Questions
- What's the difference between `@(args) => expr` and `@@(args) => expr` in daslang?
- When should I use `@@(...)` instead of `@(...)`?
- Why does my predicate fail with "expecting lambda, passing function"?
- How do I capture a non-copyable variable into a daslang lambda?

## Questions
- What's the difference between `@(args) => expr` and `@@(args) => expr` in daslang, and when does the distinction matter?
