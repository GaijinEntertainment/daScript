# bench-stand viewer Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`../README.md`. A diff here answers to `site/REVIEW.md` (repo root) too.

**A page, script, stylesheet or asset a diff adds to this folder is added to the `file(COPY ...)`
list in `../stand.cmake` in the same change** - a file missing from that list is never served at `/bench/`.

**A series a diff adds or recolors takes its color from its lane - the execution tier the
benchmark was measured in (`interp`, `jit`, `aot`) - never from its index in any list.**

**A chart a diff adds or changes that draws more than one lane (execution tier: `interp`, `jit`,
`aot`) must show a legend.**

**`app.js` takes a series' file id and group only from `data.json`'s `file` and `group` fields,
and never splits a path into directories to get them; it matches a failure to its series by
comparing the failure's `path`, with the trailing `.das` removed, against the series' `file`.**
The derivation from the path under `benchmarks/` is `utils/benchctl`'s, and one implementation
of it is enough.

**Every selector in a CSS rule a diff adds starts with a compound selector carrying the class
`.stand` or `.stand-page`; the only other allowed form is a `:root` block, at top level or inside
`@media`, that declares only custom properties `site/files/forge.css` (repo root) does not
define.** Anything else restyles the daslang.io page that embeds this stylesheet.

**A `var(--x)` a diff adds that reads a forge token - a CSS variable `site/files/forge.css` (repo
root) defines - must carry a literal fallback value after the comma, with no nested `var()`.** The
page served on its own at `/bench/` defines no forge token, and a bare read there is blank.

**Of the elements `app.js` queries by id or data attribute, a host page - a page that loads
`app.js` - carries only `#status` and an empty `#stand`; `mountSkeleton` builds the rest**, so
every host page gets the same markup.
