---
slug: css-important-beats-js-inline-style-debug-cascade-first
title: My JavaScript element.style.X assignment has no visible effect — why doesn't the element move/resize?
created: 2026-05-13
last_verified: 2026-05-13
links: []
---

**Suspect `!important` in the CSS cascade. Inline styles (set via `element.style.X = ...` or the `style="..."` attribute) lose to a stylesheet rule that has `!important`. Higher-specificity selectors without `!important` would normally win, but `!important` flips that order.**

CSS specificity order:
1. Author `!important` declarations
2. Inline style attribute (including JS-assigned `element.style.X`)
3. Author normal declarations
4. User agent styles

So `.main_col { flex: 1 1 0 !important; }` defeats `el.style.flex = '0 0 50%'`.

## How to spot it

1. Open DevTools → Elements → Styles panel. The "won" rule is the one not crossed out at the top, with the smaller specificity. Look for `!important` flags.
2. Inline `style="..."` shows at the top of the Styles panel — if it's crossed out and a stylesheet rule isn't, `!important` is the culprit.

## Two fixes

1. **Remove `!important` from the stylesheet rule, increase its specificity instead.** Use `.main_workspace > .main_col` rather than `.main_col` — same effect, no `!important` needed, JS inline overrides work normally.
2. **Set the JS-applied style as `!important` too.** `el.style.setProperty('flex', '0 0 50%', 'important')`. Use sparingly — `!important` arms races scale badly.

Option 1 is almost always right. `!important` in source CSS is usually a code smell; replacing it with selector specificity makes the cascade predictable.

## Where this bit me

PR #2648 (daslang playground splitter): drag handle wouldn't move the columns — `applyPct` was setting `left.style.flex` correctly, but the user-visible 50/50 layout never changed. `.main_col { flex: 1 1 0 !important; }` in `forge-skin.css` was beating the inline assignment. Fix: drop `!important`, use `.main_workspace > .main_col` selector specificity instead. The first-instinct debugging move (assume JS is broken) wastes time when the actual problem is CSS specificity.

## General lesson

When a JS-set inline style appears to have no effect, **inspect the Styles panel in DevTools first** before reaching for `console.log` or rewriting the JS — the cascade tells you exactly which rule won.

## Questions
- My JavaScript element.style.X assignment has no visible effect — why doesn't the element move/resize?
