---
slug: dasimgui-registry-path-brackets-vs-slash-indexed-vs-container
title: dasImgui registry path convention — indexed widgets use IDENT[N] (brackets) vs containers IDENT/N (slash) — why test/record wait targets fail with /0
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Registry paths use two different separators depending on what's indexed**:

| Shape | Path format | Example |
|---|---|---|
| Indexed leaf/text widget (`text(DT_NAME[i], ...)`) | `IDENT[N]` — square brackets | `DT_WIN/DT_TABLE/DT_NAME[0]` |
| Container hierarchy nesting | `IDENT/N` — forward slash | `DT_WIN/DT_TABLE` |
| Mixed | brackets at leaf, slashes between containers | `DT_WIN/DT_TABLE/DT_NAME[0]` |

The brackets-vs-slash split is set by two different code paths:
- **Indexed widget wrapper** (per `[widget]` / `[container]` annotation, emitted lazily per IDENT) builds the bare key as `"{IDENT}[{k}]"` — see `imgui_boost_runtime.widget_path_key` and the per-IDENT macro-emitted wrapper in [how-do-indexed-table-keyed-dasimgui-widgets-work-slider-float-channel-vol-i](how-do-indexed-table-keyed-dasimgui-widgets-work-slider-float-channel-vol-i.md).
- **Container path stack** maintains `g_container_path_str` as `"/"`-joined chain of currently-open container idents — see [dasimgui-container-annotation-and-hierarchical-path-keys](dasimgui-container-annotation-and-hierarchical-path-keys.md).

**The gotcha that bit me**: writing `wait_for_widget(d, "SORT_WIN/SORT_TABLE/SORT_ROW_ID/0", 15.0f)` for an indexed text widget. The widget never registers under that path — the actual path is `SORT_ROW_ID[0]`. The wait times out at 15s, looks like a CI flake or layout bug, actually a 1-char typo. Caught on dasImgui PR #52 (CI failure `bfcb48b`, fixed in `57ade70`).

**Authoritative example** of both forms in one driver: `tests/integration/record_data_table.das:37-39`:

```daslang
let T_TABLE = "DT_WIN/DT_TABLE"           // container/container — slashes
let T_NAME0 = "DT_WIN/DT_TABLE/DT_NAME[0]"  // ../indexed-leaf — brackets at leaf
let T_VAL5  = "DT_WIN/DT_TABLE/DT_VAL[5]"
```

Reach for this whenever writing `wait_for_widget` / `wait_for_render` / snapshot lookups in a record_*.das / test_*.das driver. If the target is a `text(IDENT[i], ...)` / `slider_float(STATE[k], ...)` etc. — it's brackets at that segment. If it's a `window(W, ...)` / `data_table(T, ...)` / `child(C, ...)` — it's slash between containers.

## Questions
- dasImgui registry path convention — indexed widgets use IDENT[N] (brackets) vs containers IDENT/N (slash) — why test/record wait targets fail with /0
