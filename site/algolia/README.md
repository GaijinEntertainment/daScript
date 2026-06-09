# Algolia DocSearch — crawler config reference

`crawler-config.js` is a **version-controlled mirror** of the Algolia Crawler
configuration that indexes daslang.io for DocSearch. It is **not** consumed by
any build and **not** deployed — the live source of truth is the Algolia
dashboard (*Crawler → "daslang.io crawler" → Editor*). Update both together.

The front-end widget (the search box itself) is wired separately — see
`site/files/docsearch.js` / `docsearch.css` and `doc/source/conf.py`.

## Why the config looks the way it does

The docs are Sphinx + the Read-the-Docs theme, which fought the default
DocSearch extractor in three ways. The fixes:

1. **Discovery — the sitemap.** Link-following alone topped out at ~52 URLs
   (the main site + blog) and never reached the deep `/doc/` tree, so the docs
   were absent from the index. `sphinx-sitemap` (enabled in `doc/source/conf.py`)
   emits `https://daslang.io/doc/sitemap.xml` listing all ~435 doc pages, which
   the crawler reads via the `sitemaps` field.

2. **Records too big — scope + per-function levels.** The RTD theme renders its
   full nav sidebar (~1635 links) on every page, and stdlib pages document
   dozens of functions as `<dl><dt><dd>` blocks under just a handful of headings.
   The default extractor aggregated all of that into one >97 KB record (a
   58 KB-of-text page produced a 349 KB record, because the syntax-highlighted
   signatures are hundreds of `<span>`s of markup). Action 1 fixes it by:
   - scoping every selector to `.rst-content` (the nav lives outside it);
   - promoting each signature `<dt class="sig">` to a hierarchy **level** (`lvl4`)
     so each function becomes its own small record, read as clean text;
   - keeping `content` to `p, li` only — never `dt` (no markup blob);
   - `indexHeadings: true` so signature-only functions (empty `<dd>`) still index.

3. **Two actions, mutually exclusive.** Docs need the `.rst-content` scoping;
   the hand-authored landing pages have no `<article>`/`<main>` and need the bare
   `p, li` catch-all. A URL matching multiple actions produces a record from
   *each*, so Action 2 negates `/doc/**` (`"!https://daslang.io/doc/**"`) to avoid
   double-processing.

## After editing the config
Save in the dashboard → run the **URL Tester** on a stdlib page
(`…/doc/stdlib/generated/algorithm.html`), a reference page, and a blog post to
confirm small, structured records → **Restart crawling**.
