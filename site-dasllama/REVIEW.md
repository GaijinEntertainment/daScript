# site-dasllama Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `plans/dasllama_io_site.md`.

**Every shell command shown on a page runs verbatim and produces the result the page shows.**
A command, flag, or output line invented for illustration is a defect; if the implementation
does not exist yet, the page does not show the command.

**Every code sample shown on a page compiles and runs with the current toolchain.** Same
standard as shell commands: no pseudo-code presented as code.

**Every number shown is either rendered from live data or copied from a checked-in
measurement record; anything else is a placeholder and carries an HTML comment naming it as
one.** A placeholder that could be mistaken for a fact is a defect.

**No performance claims on index.html.** The promotional page is daslang.io/dasllama.html;
this site states inventory and renders data.

**An empty ratio cell always reads "not raced".** Copy or rendering that lets a das-only row
imply parity with a reference is a defect.

**The `dl-*` measurement-table language lives only in `site/files/dasllama-table.css`.**
New `dl-*` rules in any file under this directory are a defect; site chrome uses the `dio-`
prefix in `files/dasllama-io.css`.

**News entries state real, shipped events.** An entry for something not yet true at publish
time is a defect; entries live in `_news/*.md` and the page carries only what the generator
emits.

**The region between the `news:begin`/`news:end` markers in index.html is `build_news.py`
output.** A hand-edit inside the markers is a defect — edit `_news/` and re-run the
generator; the regenerated index.html, feed.xml and sitemap.xml land in the same change.

**Every value `files/dasllama-io.js` takes from `/api/*` reaches markup only through the
context-correct escape: `esc()` (which escapes `< > & " '`) for text AND for quoted-attribute
values, `Number(...)` for anything numeric, and `safeApiHref()` (an `^/api/…` allow-list) for
anything landing in `href`/`src` or fed to `fetch`.** A submission-derived value concatenated
into markup with the wrong escape — a bare `esc()`-less number, an unfiltered URL, or (before
`esc()` escaped quotes) a value inside `"…"` — is a defect. The rule names the CONTEXT because
"passed through `esc()`" alone does not catch a quote breaking out of an attribute.

**Every page keeps `<title>`, meta description, OpenGraph tags, and the Atom `<link>`.**
A new or renamed page without them is a defect.
