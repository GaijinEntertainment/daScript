---
slug: daslib-json-optional-bool-omitted-when-false-snapshot-test-impact
title: Why doesn't `wait_for_bool_value(target, "opened", false)` see the `opened=false` transition for tree_node / collapsing_header in dasImgui integration tests?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Because `@optional bool : opened` field is OMITTED from the JSON envelope when its value is the default (false).** `JV(state)` reflects struct fields with @optional skipping zero-valued ones — for bool, that's false. The test sees `payload.opened` as ABSENT, not as `false`, and `wait_for_bool_value` returns null → predicate never matches → timeout.

**dasImgui Phase 0b.3 specifics**: TreeNodeState / CollapsingHeaderState have:
```das
@optional opened : bool                   // observed per-frame: TreeNode returned true
```

When the tree closes (state.opened=false), JSON omits the field. So:
```das
wait_for_bool_value(d, ".../PHYSICS_TREE", "opened", false, 5.0f)   // ❌ never converges
```

**Three fixes**:

1. **Test by leaf disappearance (preferred)** — closed tree_node hides its leaves; assert leaves disappear instead. More meaningful UI test anyway:
   ```das
   close_widget(d, ".../PHYSICS_TREE")
   var snap_closed = wait_until(d, 5.0f) $(var snap) {
       return !widget_exists(snap, ".../PHYSICS_TREE/GRAVITY")
   }
   t |> success(snap_closed != null, "imgui_close hides leaves")
   ```

2. **Test by absence**: `widget_payload_field(snap, target, "opened") == null` polls for the field disappearing.

3. **Drop `@optional` on `opened`** — make it always-serialized. Cost: every snapshot carries `"opened":false` for every closed tree_node. Acceptable noise floor for snapshot tests, but a regression in JSON brevity for production telemetry.

**General rule for dasImgui state struct design**: `@optional bool` means "presence ↔ true". If you need to observe the false-transition explicitly via the snapshot, either drop @optional or design the test to detect absence.

`wait_for_bool_value(..., true, ...)` is unaffected — true is non-default, the field is present, polling sees it.

## Questions
- Why doesn't `wait_for_bool_value(target, "opened", false)` see the `opened=false` transition for tree_node / collapsing_header in dasImgui integration tests?
