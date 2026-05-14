---
slug: css-import-must-precede-all-other-rules-or-silently-dropped
title: My CSS @import isn't loading any fonts — what's wrong?
created: 2026-05-13
last_verified: 2026-05-13
links: []
---

**`@import` rules must precede every other style rule in the stylesheet (except `@charset` / `@layer`). A browser silently drops any `@import` placed after a normal rule.**

Per CSS 2.1 §6.3:
> Any @import rules must precede all other rules (except the @charset rule if present).

Modern browsers honor this strictly — there is no console warning, the imported stylesheet just never loads. Symptoms: web fonts fall back to system serif, retoken colors revert to defaults, etc.

## The trap

```css
:root {
    --bg: #0d0c0a;
    --fg: #e8e2d2;
}

/* SILENTLY IGNORED — comes after :root */
@import url('https://fonts.googleapis.com/css2?family=Inter+Tight&display=swap');

body {
    background: var(--bg);  /* works */
    font-family: 'Inter Tight', sans-serif;  /* falls back to system */
}
```

The whole file looks "right" — the rules work, just no Inter Tight. Easy to miss because the fallback chain renders something reasonable.

## Fix

Move the `@import` to the very top, before any rule (including `:root`):

```css
@import url('https://fonts.googleapis.com/css2?family=Inter+Tight&display=swap');

:root { ... }
body { ... }
```

You can also avoid `@import` entirely by linking the stylesheet via `<link rel="stylesheet">` in HTML — same fonts, no ordering trap.

## Where this bit me

PR #2648 (daslang.io docs theme): `doc/source/_static/custom.css` had the Google Fonts `@import` after the `:root` block defining design tokens. Browsers dropped the import, Sphinx HTML rendered in system serif despite the file looking correct. Fix was hoisting the `@import` to line 11 of the file, before `:root` on line 13.

Spec reference: https://www.w3.org/TR/CSS21/cascade.html#at-import

## Questions
- My CSS @import isn't loading any fonts — what's wrong?
