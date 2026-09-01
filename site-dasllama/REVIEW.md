# site-dasllama Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `plans/dasllama_io_site.md`.

In this checklist "a page" means every served `.html` page in this directory, the generated
news region inside index.html included.

**A shell command, flag, or output line shown on a page is a defect unless the command runs
verbatim and produces the result the page shows.**

**A code sample shown on a page that does not compile and run with the current toolchain is
a defect.**

**A number shown on a page carries its source on the page itself - rendered from live data,
or named in a page comment or prose clause: the checked-in measurement record, or the box
and date and what was measured, or the word placeholder. A number with none of these is a
defect.**

**A comparative superiority claim made in words rather than as side-by-side figures, on any
page, is a defect.** Figures copied from a checked-in record may stand side by side, ours
and another engine's - the arithmetic a reader does is not the page's claim.

**Copy or rendering that lets a row with no reference engine's figure imply parity with a
reference engine is a defect - an empty ratio cell reads "not raced".**

**A diff that makes `REVIEW.das`'s cookie-and-host check read fewer files, or report less
than it did before the change with the same `ALLOWED_HOSTS`, is a defect** - the gate bans
`document.cookie` and reports any resource a page loads from a host the list does not
name.

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
visitor sends that host and whether the host sets cookies.**

**Weakening `REVIEW.das`'s `dl-*` census - the gate that reports a `dl-*` name used in a
page, script or stylesheet here that `site/files/dasllama-table.css` (repo root) does not
define - is a defect.**

**A statement on a page here about what a shipped tool does is a defect unless it matches
the current code.**

**A claim in a `_news/*.md` entry that is not true at publish time is a defect, including a
claim about what a page here renders - check that one against the page's markup and
`files/dasllama-io.js`.**

**A `_news/*.md` entry for something not yet shipped is a defect.**

**A value `files/dasllama-io.js` takes from `/api/*` and puts into markup without the
context-correct escape is a defect: `esc()` (which escapes `< > & " '`) for text and for
quoted-attribute values, `Number(...)` for anything numeric, and `safeApiHref()` (an
`^/api/...` allow-list) for anything landing in `href`/`src` or fed to `fetch`.**

**Weakening `test_metadata.py`'s per-page assertions - the test that fails a page without a
`<title>`, a meta description, the OpenGraph tags, or the Atom `<link>` - is a defect.**

**A diff that adds a file under `files/` also verifies the dasllama.io deploy step
(`.github/workflows/pages.yml`, "Stage dasllama.io for deployment") stages it, in the same
change.** Local preview serves the tree directly, so a staging miss is invisible until deploy.
