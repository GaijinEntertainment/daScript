# site-dasllama - the dasllama.io site

Static site for dasllama.io, served from dasweb-1 (own Caddy vhost, own root beside
daslang.io's). Design originated in the Claude Design forge (`consumers/dasllama-io/`,
project "daslang.io Forge") on the daslang.io Forge system. Arc plan + follow-up ledger:
`plans/dasllama_io_site.md`.

## Layout

- `index.html` - news-first home (identity masthead, feed, how-to-get, ladder teaser)
- `stories.html` - the stories index (title, date, tag, lede per story), generated from
  `_stories/` between the `stories:begin` markers by `build_news.py`; the index rows use
  `forge.css`'s shared blog-list language (`forge-blog-*`), so their styling is shared with
  daslang.io
- `stories/<slug>.html` - one page per story, generated from `_stories/<slug>.md` through
  `_stories/template.html` (`{{root}}` = `../`, so the shared chrome's links work one level
  down); the deploy copies the directory whole; plain "we", llama.cpp named as the yardstick
- `ladder.html` - ALL measurements (official + community); daslang.io/dasllama.html stays
  official-only
- `sidecars.html` - the sidecar exchange: finder, kernel-win viewer, downloads. The exchange is
  closed: the page is unlinked from every tab bar and footer and carries `noindex`; it stays so
  the service can reopen without a rebuild
- `files/dasllama-io.css` - site chrome (`dio-` prefix)
- `files/dasllama-io.js` - the one renderer for all three pages (page detected by mount
  points); community text is escaped before it reaches markup
- `files/dasllama-favicon.svg` - from the dasllama-mark brand set (amber)
- `_news/` - news entries as dated markdown, same pipeline shape as `site/_news`; a news
  item is a few lines, the detail is a story it links to
- `_stories/` - the stories as dated markdown (front matter: date, tag, title, lede; fenced
  code and tables render)
- `build_news.py` - regenerates the index.html news region (between the `news:begin`
  markers), the stories.html index (`stories:begin`), the `stories/*.html` pages (stale
  ones removed), `feed.xml` (Atom, news and stories) and `sitemap.xml` from `_news/` and
  `_stories/`; output is checked in so the preview matches production, and the deploy
  re-runs it anyway
- `robots.txt` - static

The public home URL is `https://dasllama.io/`. All pages link home with `/`, each page
declares an absolute canonical URL, and `utils/internal/dasllama-ladder/caddy.snippet` permanently
redirects `/index.html` to `/`. The sitemap and Atom feed also use the root URL so crawlers
never receive competing home-page identities.

Shared css is NOT duplicated here: `forge.css`, `nav-dropdown.css` and
`dasllama-table.css` live in `site/files/` (single source) and are staged into this
site's `files/` at deploy. `dasllama-table.css` is the shared measurement-table language
(`dl-` prefix) both sites render; dasllama.html's inline copy folds onto it as a
ledgered follow-up.

## Preview

    py -3 serve.py     # http://127.0.0.1:8932/ - stages shared css like the deploy does

`serve.py` also proxies `/api/*` to a locally running ladder service on 127.0.0.1:8201
(mirroring the Caddy vhost), so a preview against real data is: start
`utils/internal/dasllama-ladder/main.das` with `--official-dir modules/dasLLAMA/performance/records`,
then browse. Without the service the pages show their unreachable/zero states.

## Data

Tables, fact tiles, and the sidecar finder render client-side from the `dasllama-ladder`
service (`utils/internal/dasllama-ladder/README.md` sec.1.1 lists the routes; 127.0.0.1:8201 behind
Caddy). The ladder joins `/api/runs` with the verbatim `/api/submission/:id` documents -
measurements, receipts and command lines all come from the submission itself, and pairing
is within one submission only (a ratio appears when the methodology's reference flavor
rode along in the same document). The sidecar finder lists `/api/sidecars` and fetches the
sidecar document on expand. Pages must read sensibly with zero rows (launch state) and
with the service down.

## Copy rules (Boris)

Strictly factual, numbers-not-adjectives, no performance promo on the home page - the
promo scoreboard is daslang.io/dasllama.html. An empty ratio cell means "not raced",
never implied parity.
