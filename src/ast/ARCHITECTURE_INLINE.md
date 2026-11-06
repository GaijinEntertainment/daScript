# Inliner conditional lowering

This document follows `ARCHITECTURE_COMMON.md`.

## Conditional call lowering in the inliner {#inline-conditional-lowering}

When a call inside a conditional expression needs statements for inlining,
`InlinePatch::tryLowerCallPosition` splits the condition into arm stores in the
current pass. For a user declaration it first hoists the expression into a generated
temporary, then splits that temporary immediately: the split declaration keeps the
temporary's `Variable`, so every read the hoist bound stays bound, and no later
round has to rediscover the call.
