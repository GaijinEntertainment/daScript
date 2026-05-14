---
slug: daslang-external-module-docs-crosslink-convention
title: How do I cross-link daslang docs to an external module's gh-pages site (and back) — what's the convention?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

## The two-PR shape

External module (e.g. dasImgui) ships its own Sphinx site on `<owner>.github.io/<repo>/`. Cross-linking is a **paired-PR convention** — one PR on each side, landed together so neither dangles.

## daslang side (outbound)

1. New file `doc/source/external_modules/<name>.rst` — short page covering: what the module is, how to install (`daspkg install …`), what's inside, link to gh-pages URL.
2. New `doc/source/external_modules/index.rst` — single `.. toctree::` listing the per-module RSTs.
3. Add `external_modules/index` to the root `doc/source/index.rst` toctree (after `stdlib/`).

This is one outbound link sitting under a dedicated "External modules" section in the daslang docs nav. Pattern repeats per module added.

Reference: daslang commit `e49e6f35c` (PR #2649, 2026-05-13) — first instance, dasImgui.

## External module side (inbound + visual parity)

1. Vendor daslang.io's Forge CSS (`doc/source/_static/custom.css` — Inter Tight + JetBrains Mono, amber-on-dark, retoken'd Pygments). Without this the link feels like leaving the site; with it the two docs read as one.
2. Wire via `conf.py`:
   ```python
   html_static_path = ["_static"]
   html_css_files = ["custom.css"]
   ```
3. One-line "Part of the daslang ecosystem" pointer at the top of `doc/source/index.rst` linking back to the daslang docs root.

Reference: dasImgui commit `a18f195` (PR #24, 2026-05-13).

## Pinned decisions

- **Visual theme parity is intentional.** Cross-navigation should feel like one site, not two. Vendor the CSS, don't try to re-derive a theme.
- **Outbound = dedicated page, not a one-line link in `index.rst`.** A module deserves a blurb + install snippet + "what's inside" — readers landing on the cross-link need that context.
- **Inbound = one-line ecosystem pointer.** The external module's own index is the destination; don't bury the link.
- **Paired PRs ship together.** Either side alone leaves a half-built bridge.

## Questions
- How do I cross-link daslang docs to an external module's gh-pages site (and back) — what's the convention?
