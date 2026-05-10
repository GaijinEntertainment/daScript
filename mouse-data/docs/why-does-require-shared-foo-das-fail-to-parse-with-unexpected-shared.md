---
slug: why-does-require-shared-foo-das-fail-to-parse-with-unexpected-shared
title: Why does `require ../path/shared/foo.das` fail with `syntax error, unexpected shared, expecting .. or name or '%' or '.'`?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

daslang's parser tokenizes `shared` as the `DAS_SHARED` keyword (used for shared smart pointers and `module foo shared`). When `shared` appears as a path component in a relative `require`, the parser sees the keyword instead of an identifier and reports:

```
error[30151]: syntax error, unexpected shared, expecting .. or name or '%' or '.'
```

The require grammar at this position accepts only `..`, an identifier, `%`, or `.`. `shared` matches none — it's a hard token, not a name.

**Fix**: rename the directory. Any non-keyword name works — `common/`, `_shared/`, `lib/`, `internal/`. To pre-check a candidate, grep `src/parser/ds_lexer.lpp` for `"<name>"`; if it's there, it's a reserved token. Same trap applies to any future cross-tool sharing under `utils/` or `tests/`.

Concrete: PR #2621 first tried `utils/shared/git_signature.das`, hit this error immediately, and the directory was renamed to `utils/common/`. The rename is purely cosmetic — the require path becomes `require ../../common/git_signature.das` and everything else is unchanged.

Worth knowing because the error message is cryptic if you don't already suspect the parser is treating your dir name as a keyword: the diagnostic says "unexpected shared" but doesn't hint that `shared` is reserved.

## Questions
- Why does `require ../path/shared/foo.das` fail to parse with "unexpected shared"?
- Can a daslang require path use any directory name?
- Which directory names break daslang's relative-require parser?

## Questions
- Why does `require ../path/shared/foo.das` fail with `syntax error, unexpected shared, expecting .. or name or '%' or '.'`?
