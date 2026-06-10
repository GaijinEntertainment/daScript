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

2. **Records too big — scope, canonical levels, and `nosearch`.** The RTD theme
   renders its full nav sidebar (~1635 links) on every page, and stdlib pages
   document dozens of symbols as `<dl><dt><dd>` blocks under a handful of
   headings. The default extractor aggregated all of that into one >97 KB record
   (a 58 KB-of-text page produced a 349 KB record, because the syntax-highlighted
   signatures are hundreds of `<span>`s of markup). Action 1 fixes it by:
   - scoping every selector to `.rst-content` (the nav lives outside it);
   - promoting each **canonical** signature (`dt.sig[id]`) to a hierarchy
     **level** (`lvl4`) — overloads carry no `id`, so they don't each spawn a
     record;
   - keeping `content` to `p, li` (minus the toctree `li`, which index pages
     embed inside `.rst-content`) — never `dt` (no markup blob);
   - `indexHeadings: true` so signature-only symbols (empty `<dd>`) still index;
   - `$(".nosearch").remove()` — das2rst wraps oversized value/field tables in
     `.. container:: nosearch` (by symbol name, e.g. rtti's 580-value
     `CompilationError` enum); the symbol + its description stay indexed, the
     table is dropped. Toggle the list in `doc/reflections/das2rst.das`.

3. **Too many records — the 750/page cap.** Independently of size, the crawler
   caps a page at 750 records, and a page over the cap is dropped **entirely**
   (0 records). `ast.html` documents 546 symbols → 612 signature levels + ~541
   description runs = 1153. Action 1b indexes `ast.html` **signatures-only**: the
   same `dt.sig[id]` levels (search by symbol name still works), but it
   physically removes the content nodes (`$('.rst-content p').remove()` etc.) so
   the per-symbol descriptions don't each spawn a record → 612. (A non-matching
   content *selector* doesn't work — docsearch falls back to its defaults when a
   selector matches nothing — so strip the DOM instead.) If ast ever crosses 750
   on signatures alone, split the page into per-group sub-pages.

4. **Three actions, mutually exclusive.** A URL matching multiple actions
   produces a record from *each*, so the path patterns partition the site:
   Action 1 = `/doc/**` except `ast.html`; Action 1b = `ast.html` only;
   Action 2 = everything else (`!/doc/**` — the hand-authored landing pages have
   no `<article>`/`<main>` and need the bare `p, li` catch-all).

## After editing the config
Save in the dashboard → run the **URL Tester** on a stdlib page
(`…/doc/stdlib/generated/algorithm.html`), `ast.html` (expect ~612
signature-only records, no "too many" error), `rtti.html` (no "too big"), a
reference page, and a blog post → **Restart crawling**.
