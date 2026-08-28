# site-dasllama Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `plans/dasllama_io_site.md`.

**A shell command, flag, or output line shown on a page is a defect unless the command runs
verbatim and produces the result the page shows.** Never show a command for an
implementation that does not exist yet.

**A code sample shown on a page that does not compile and run with the current toolchain is
a defect.**

**A number shown on a page carries checkable provenance: rendered from live data, copied from
a named checked-in measurement record, or an HTML comment naming the box, date, and what was
measured (or naming it a placeholder). A number with none of these is a defect.**

**A claim in words on index.html that one engine beats another ("beats", "Nx faster than",
"fastest") is a defect, the generated news region included.** Figures copied from a
checked-in record may stand side by side, ours and another engine's - the arithmetic a
reader does is not the page's claim.

**Copy or rendering that lets a row with no reference engine's figure imply parity with a
reference engine is a defect - an empty ratio cell reads "not raced".**

**A `dl-*` selector that `site/files/dasllama-table.css` (repo root) does not already
define, declared in any file under this directory, is a defect** - site chrome here uses the
`dio-` prefix in `files/dasllama-io.css`, and the `dl-*` measurement-table language is that
shared sheet's.

**A statement on a page here about what a shipped tool does is a defect unless it matches
the current code.**

**A claim in a `_news/*.md` entry that is not true at publish time is a defect, including a
claim about what a page here renders - check that one against the page's markup and
`files/dasllama-io.js`.** An entry for something not yet shipped is a defect.

**A hand-edit between the `news:begin` and `news:end` markers in index.html is a defect -
edit `_news/` and re-run `build_news.py` instead.** A diff that changes `_news/` also lands
the regenerated index.html, feed.xml and sitemap.xml, in the same change.

**A value `files/dasllama-io.js` takes from `/api/*` and puts into markup without the
context-correct escape is a defect: `esc()` (which escapes `< > & " '`) for text and for
quoted-attribute values, `Number(...)` for anything numeric, and `safeApiHref()` (an
`^/api/...` allow-list) for anything landing in `href`/`src` or fed to `fetch`.**

**A page that lacks `<title>`, a meta description, OpenGraph tags, or the Atom `<link>` is a
defect - a new or renamed page included.**

**A diff that adds a file under `files/` also verifies the dasllama.io deploy step
(`.github/workflows/pages.yml`, "Stage dasllama.io for deployment") stages it, in the same
change.** Local preview serves the tree directly, so a staging miss is invisible until deploy.
