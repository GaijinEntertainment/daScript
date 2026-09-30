# Inliner conditional lowering

This document follows `ARCHITECTURE_COMMON.md`.

## Conditional call lowering in the inliner {#inline-conditional-lowering}

When a call inside a conditional expression needs statements for inlining,
`InlinePatch::tryLowerCallPosition` splits the condition into arm stores in the
current pass. For a user declaration it first hoists the expression into a generated
temporary, then splits that temporary immediately. Generator lowering can move a
local into a capture field before the next inlining round; postponing the split
would rediscover the same conditional call and repeat the hoist.
