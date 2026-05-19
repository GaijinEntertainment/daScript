---
slug: annotation-arg-name-cannot-shadow-keyword
title: Why does my annotation `[my_macro(abstract = true)]` fail to parse with "unexpected abstract, expecting type or in or name"?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

Annotation argument names cannot shadow daslang **reserved keywords**. Even though annotation args are parsed as identifier=value pairs, the identifier slot still goes through the regular tokenizer — `abstract`, `sealed`, `inherited` (NOT a keyword), `override`, `private`, `static`, `template`, `class`, `struct`, `enum`, `def`, `let`, `var`, `for`, `while`, `if`, `return`, `break`, `continue`, etc. all parse as their keyword token and the annotation-arg rule rejects them.

```das
// PARSE ERROR: "unexpected abstract"
[my_macro(abstract = true)]

// OK — prefix avoids the keyword
[my_macro(is_abstract = true)]
```

## Workarounds

1. **Prefix with `is_` / `has_` / `expected_`** — most idiomatic. Reads naturally for booleans (`is_abstract`, `has_body`).
2. **Use camelCase** if the keyword is a single lowercase word — e.g. `isAbstract`, `useSealed`. Daslang doesn't reserve camelCase forms.
3. **Read by string in the macro** — annotation args are accessed via `find_arg(args, "name") ?as tBool ?? false` where "name" is a plain string. The string literal IS the identifier as seen at parse time — so the *parser* must accept the identifier, but you read it from the runtime args list by its string spelling.

## Why this hits during macro test writing

`[structure_macro]` helpers that probe class-method flags naturally want arg names matching the flag names — `abstract`, `sealed`, `classMethod`. The bitfield bindings escape with `_abstract` / `_sealed` for the daslang-side read, but the bitfield-arg position uses string literals so the underscore is fine THERE. The annotation arg name position is a regular identifier token and the keyword-shadow rule fires.

## Reserved keyword list

`src/builtin/module_builtin_ast.cpp` around line 92 lists the daslang reserved keywords for tokenizer purposes. Check there if you're unsure whether a name will conflict.

## Questions
- Why does my annotation `[my_macro(abstract = true)]` fail to parse with "unexpected abstract, expecting type or in or name"?
