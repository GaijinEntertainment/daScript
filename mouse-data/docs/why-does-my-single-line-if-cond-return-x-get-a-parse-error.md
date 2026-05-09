---
slug: why-does-my-single-line-if-cond-return-x-get-a-parse-error
title: Why does my single-line `if (cond) { return X }` get a parse error?
created: 2026-05-08
last_verified: 2026-05-08
links: []
---

gen2's grammar requires a statement separator (newline or `;`) before `}` in an `if`-body. A bare `if (cond) { return false }` on one line fails with `error[30151]: syntax error, unexpected '}', expecting if` — the `if` it's expecting is the postfix-conditional form (`return X if (cond)`), and `}` is neither that nor a newline.

Two fixes:

```
// Multiline (canonical):
if (cond) {
    return false
}

// Same-line with explicit terminator:
if (cond) { return false; }
```

The same caveat applies to `if`-`elif` chains: `} elif (cond)` works on a single line because `}` is followed by another keyword, but `if (cond) { stmt }\nelif (cond) { ... }` (newline before `elif`) does not — gen2 wants `}` and `elif` adjacent.

The `||` / `&&` line-continuation rule is related but separate: `return a\n   || b` doesn't parse at statement level (CLAUDE.md says newlines are only free inside `(...)` / `[...]` / `{...}`). Wrap the RHS in parens to break across lines.

## Questions
- Why does my single-line `if (cond) { return X }` get a parse error?
- gen2 single-line if return parse error
- error 30151 unexpected } expecting if
- why doesn't `if (x) { return false }` parse on one line
- gen2 if body needs newline before closing brace
