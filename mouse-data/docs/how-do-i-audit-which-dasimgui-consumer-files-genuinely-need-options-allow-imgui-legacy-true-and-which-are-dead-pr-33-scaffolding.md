---
slug: how-do-i-audit-which-dasimgui-consumer-files-genuinely-need-options-allow-imgui-legacy-true-and-which-are-dead-pr-33-scaffolding
title: How do I audit which dasImgui consumer files genuinely need `options _allow_imgui_legacy = true` and which are dead PR #33 scaffolding I can drop?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

PR #33 (2026-05-15) bulk-added `options _allow_imgui_legacy = true` defensively to every imgui_demo file as part of flipping the lint default-on. Many of those opt-outs are now dead scaffolding — the file genuinely has no raw `imgui::*` calls, the option is suppressing nothing.

**Audit pattern (per-file strip + dry-run + IMGUI002 count):**

```sh
# For each candidate file: cp, strip, dry-run, count, restore.
for f in examples/imgui_demo/*.das; do
  if ! grep -q "^options _allow_imgui_legacy" "$f"; then echo "= $f  (no opt-out)"; continue; fi
  cp "$f" "$f.bak"
  sed -i '/^options _allow_imgui_legacy/d' "$f"
  errs=$(daslang.exe -dry-run -project_root <dasImgui_root> "$f" 2>&1 | grep -c "IMGUI002")
  mv "$f.bak" "$f"
  echo "$errs $f"
done | sort -n
```

**Found in PR #40 audit (2026-05-16):**
- 9 files report `0` IMGUI002 errors → dead scaffolding, drop the opt-out: `app_console`, `app_custom_rendering`, `app_dockspace`, `app_documents`, `app_small`, `main`, `popups`, `tables`, `user_guide`.
- 7 files report `2..133` IMGUI002 errors → genuine raw survivors, keep the opt-out until individually swept: `app_main_menu` (2), `about` (5), `imgui_demo` (6), `widgets` (6), `style_editor` (9), `inputs` (13), `layout` (133).

**Why this is safe:** the lint pass is conservative — IMGUI002 is the only error class gated by `_allow_imgui_legacy`. Zero IMGUI002 errors with the option removed means the option is suppressing nothing.

**Don't blindly drop in widget-builtin files** (`widgets/imgui_*.das`). Those carry the opt-out structurally — their wrappers ARE the bottom-of-stack and legitimately raw-call `Columns/InvisibleButton/Begin/End/Render` etc. The lint visits transitive module bodies (see card `why-does-my-lint-macro-fire-on-the-wrapper-module-that-legitimately-uses-the-forbidden-symbols-even-though-i-scope-visit-module`), so an audit on a widget builtin will return non-zero. That's correct — the opt-out stays.

**Related cards:**
- `why-does-my-lint-macro-fire-on-the-wrapper-module-that-legitimately-uses-the-forbidden-symbols-even-though-i-scope-visit-module` — explains the per-module pass-macro scope that makes the opt-out structurally required in wrapper modules.
- `port-v1-imgui-boost-example-to-boost-v2-checklist` — the per-file sweep that converts the 7 genuine-survivor files (kills their need for the opt-out).

## Questions
- How do I audit which dasImgui consumer files genuinely need `options _allow_imgui_legacy = true` and which are dead PR #33 scaffolding I can drop?
