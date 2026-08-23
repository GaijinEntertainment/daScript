# Builtin Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`include/daScript/simulate/ARCHITECTURE.md`.

- **Every plain value-returning extern bind in builtin (`$`), math, and strings uses
  `addExternInline`/`addExternInlineEx`; every other module binds member-flavor
  (`addExtern`)** — the core trio is on every interpreter hot path so its binds carry the
  NTTP node, while everything else shares per-signature nodes for size. cmres,
  ref-returning, and interop binds are exempt by construction; `ast_handle`'s generic
  container helpers are NTTP in whatever module they instantiate into.
  `src/builtin/REVIEW.das` enforces this; a violation the gate reports is fixed by
  switching the bind, not by editing the gate.
