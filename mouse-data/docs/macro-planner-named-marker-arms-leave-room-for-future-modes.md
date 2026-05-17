---
slug: macro-planner-named-marker-arms-leave-room-for-future-modes
title: How do I structure a chain-recognition macro planner to leave room for future emission modes (e.g. buffer-required ops like order_by/distinct/group_by) without painting over them with a generic fallback?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---


Use **named marker arms** in the chain-recognition switch: each operator that's currently unsupported BUT will need its own emission mode later gets a distinct `elif` arm with `return null` and a `// TODO: <FutureMode>` comment. The generic "unknown operator → fallback" catch-all stays at the bottom for genuinely unrecognized ops.

```das
} elif (opName == "where_" || opName == "select") {
    // ... splice-mode emission ...
} elif (opName == "take" || opName == "skip") {
    // ... bounded-loop splice ...
} elif (is_buffer_required_op(opName)) {    // nolint:LINT009
    // TODO Phase 2X: order_by/distinct/reverse/group_by/zip/join → buffered emission modes
    // (BufferTopN, BufferDistinct, BufferReverse, BufferGroupBy, MultiSourceZip, BufferedJoin).
    return null
} else {
    // genuinely unknown operator — fallback to plain pipeline
    return null
}

[macro_function]
def private is_buffer_required_op(name : string) : bool {
    return name == "order_by" || name == "order_descending"
        || name == "distinct" || name == "distinct_by"
        || name == "reverse"
        || name == "group_by" || name == "group_by_lazy"
        || name == "zip"
        || name == "join" || name == "left_join" || name == "group_join"
}
```

**Why this matters**: when the future mode lands, it grafts in at the existing named arm — no re-walking the chain-recognition logic, no risk of accidentally enabling broken splice for an op that needs full-source materialization. A single `else: return null` catch-all would force the future PR to identify which name belongs in which mode, re-implement the recognition, and risk shadowing the wrong arm.

**Concrete location**: `daslib/linq_fold.das` `plan_loop_or_count`. Buffer-required marker arm landed in Phase 2C Ring 3 (2026-05-17, PR …); the actual emission modes are deferred to Phase 3+. The marker also doubles as documentation: a reader sees exactly which operators have planned-but-unimplemented support, vs. which are forever-out-of-scope.

**Bonus**: when the project moves to a fail-loudly contract (macro errors instead of silent fallback), the named arms can emit specific error messages ("cannot splice `order_by` + take — needs BufferTopN mode (Phase 3+)"), while the catch-all becomes a generic "unknown chain operator".

Related: see [[chained-select-splice-bind-via-clone-assign-universal]] for the chained-bind shape itself.

## Questions
- How do I structure a chain-recognition macro planner to leave room for future emission modes (e.g. buffer-required ops like order_by/distinct/group_by) without painting over them with a generic fallback?
