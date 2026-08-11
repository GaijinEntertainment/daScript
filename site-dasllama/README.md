# site-dasllama — the dasllama.io site

Static site for dasllama.io, served from dasweb-1 (own Caddy vhost, own root beside
daslang.io's). Design originated in the Claude Design forge (`consumers/dasllama-io/`,
project "daslang.io Forge") on the daslang.io Forge system. Arc plan + follow-up ledger:
`plans/dasllama_io_site.md`.

## Layout

- `index.html` — news-first home (identity masthead, feed, how-to-get, ladder teaser)
- `ladder.html` — ALL measurements (official + community); daslang.io/dasllama.html stays
  official-only
- `sidecars.html` — the sidecar exchange: finder, kernel-win viewer, downloads
- `files/dasllama-io.css` — site chrome (`dio-` prefix)
- `files/dasllama-favicon.svg` — from the dasllama-mark brand set (amber)
- `_news/` — news entries as dated markdown, same pipeline shape as `site/_news`;
  generator emits the home feed + `feed.xml` (Atom)

Shared css is NOT duplicated here: `forge.css`, `nav-dropdown.css` and
`dasllama-table.css` live in `site/files/` (single source) and are staged into this
site's `files/` at deploy. `dasllama-table.css` is the shared measurement-table language
(`dl-` prefix) both sites render; dasllama.html's inline copy folds onto it as a
ledgered follow-up.

## Preview

    py -3 serve.py     # http://127.0.0.1:8932/ — stages shared css like the deploy does

## Data

Tables, fact tiles, and the sidecar finder render client-side from the `dasllama-ladder`
service (`/api/ladder/*`, `/api/sidecars/*`; 127.0.0.1:8201 behind Caddy). The rows and
entries currently in the HTML are design placeholders and are replaced by the live
renderers; pages must read sensibly with zero rows (launch state).

## Copy rules (Boris)

Strictly factual, numbers-not-adjectives, no performance promo on the home page — the
promo scoreboard is daslang.io/dasllama.html. An empty ratio cell means "not raced",
never implied parity.
