# site-dasllama Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`README.md`. Planned work: `modules/dasLLAMA/followup_general.md` (repo root).

In this checklist "a page" means a served `.html` file under this directory, the generated news
region inside index.html, `feed.xml`, and `sitemap.xml`. Publish time is the deploy that
follows the merge of the change under review.

**A program name, flag, command line, or output line shown on a page must match what the
download that page links accepts or emits, verbatim, for every platform that page offers.**

**A code sample shown on a page that does not compile and run with the current toolchain is
a defect.**

**A measured number shown on a page with no source named on the page is a defect** - a
measured number is one a run produced, and the named source is whatever lets a reader find
that run: the live endpoint the page renders it from, the checked-in measurement record a page
comment or prose clause names, the box and date and what was measured, or the word
placeholder.

**A claim on any page that one engine, build, or way of running beats another on anything a run
measures - speed, error rate, footprint, size - made in words rather than as side-by-side
figures in the same sentence or in the table the sentence points at, is a defect** - the
arithmetic a reader does over two figures is not the page's claim.

**Copy or rendering that lets a row with no reference engine's figure imply parity is a
defect - an empty ratio cell means the comparison was never run.**

**A diff that makes `REVIEW.das`'s cookie-and-host check read fewer files, or report less
than it did before the change with the same `ALLOWED_HOSTS`, is a defect** - the gate bans
`document.cookie` and reports any resource a page loads from a host the list does not
name.

**A diff that adds a host to `REVIEW.das`'s `ALLOWED_HOSTS` states, in the PR body, what a
visitor sends that host and whether the host sets cookies.**

**Weakening `REVIEW.das`'s `dl-*` census - the gate that reports a `dl-*` name used in a
top-level `.html` page here, or in a script or stylesheet under `files/`, that no selector of
`site/files/dasllama-table.css` (repo root) defines and no `id="..."` in those files declares -
is a defect.**

**A claim in any text under this folder a reader or an operator reads as fact - a page, a
`_news/*.md` or `_stories/*.md` entry, `README.md`, a tool's docstring - that is not true at
publish time is a defect.** A dated `_news` or `_stories` entry is read as a claim about its own
date; standing page copy - a masthead, section prose, a meta tag - is read as a claim about
now. A claim is checked against the thing it describes as it stands at publish time: the
tool's current code, the page's markup and `files/dasllama-io.js`, the artifact at the link.

**A diff that falsifies standing page copy - a masthead, section prose, a meta tag - or a
`_news/*.md` / `_stories/*.md` entry dated on or after the publish day of this change, fixes it
in the same change.**

**A PR whose copy describes what a linked download contains refreshes that artifact at its
published location - a GitHub release, a Hugging Face repository - before the PR merges, or
holds the copy until it does** - the copy is judged against the artifact reachable at that
link at publish time, and an artifact republishes on its own trigger, never on a page's merge.

**A `_news/*.md` or `_stories/*.md` entry for something not yet shipped is a defect.** Shipped
means a reader can have it at publish time: the code the entry describes is merged to this
repository's `master`, and any artifact the entry points a reader at is downloadable from where
it points.

**A PR that adds or changes copy describing what a downloadable artifact contains names, in
the PR body, the URL it fetched for each artifact the copy points at and what the fetch
returned.**

**A figure in a `_news/*.md` or `_stories/*.md` entry that does not name both the date and the
build sha of the run it came from - in the entry's own text or its `<!-- figures: ... -->`
comment - is a defect, and a sha the entry marks as unconfirmed does not count as named** - a
number without its date reads as a standing claim.

**An author's query or marker - `[NAME: ...]`, `TODO`, `FIXME` - in a page, or in a
`_news/*.md` or `_stories/*.md` entry, its `<!-- figures: ... -->` comment included, is a
defect** - the entry's text, comments and all, ships in the page source and in `feed.xml`.

**A value `files/dasllama-io.js` takes from `/api/*` and puts into markup without the
context-correct escape is a defect: `esc()` (which escapes `< > & " '`) for text and for
quoted-attribute values, `Number(...)` for anything numeric, and `safeApiHref()` (an
`^/api/...` allow-list) for anything landing in `href`/`src` or fed to `fetch`.**

**Weakening `test_metadata.py`'s per-page assertions - the test that fails an `.html` page
without a `<title>`, a meta description, the OpenGraph tags, or the Atom `<link>` - is a
defect.**

**Weakening `REVIEW.das`'s page census - the gate that reports a served file, or the `stories/`
directory, missing from the dasllama.io deploy step (`.github/workflows/pages.yml`, repo root),
or an `.html` page missing from `build_news.py`'s sitemap list or `sitemap.xml` or from
`test_metadata.py`'s page list - is a defect.** Local preview serves the tree directly, so no
miss shows there: the page deploys unlisted, uncrawled, or unchecked.
