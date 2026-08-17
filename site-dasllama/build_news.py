#!/usr/bin/env python3
"""
Build the dasllama.io news feed from _news/*.md.

Rewrites index.html between the `<!-- news:begin -->` / `<!-- news:end -->` markers and
regenerates feed.xml (Atom) and sitemap.xml next to it. Entries live ON the home page —
there are no per-entry pages; feed entries link to the entry's anchor on the home page.

The generated output is CHECKED IN (unlike daslang.io's blog, which builds into _site
only): the preview rig serves the repo tree directly, and the site must preview exactly
as it deploys. The deploy step runs this same script so a forgotten local run cannot
ship a stale board — the deploy regenerates from _news/ either way.

Front matter keys: date (YYYY-MM-DD), tag, title. Body is Markdown.

Usage:
    python3 build_news.py [--root site-dasllama] [--site-url https://dasllama.io]
"""

from __future__ import annotations

import argparse
import html
import re
import sys
from pathlib import Path

try:
    import markdown
except ImportError as e:
    sys.exit(f"build_news.py requires markdown >= 3.0: pip install -U markdown  ({e})")

KEY_RE = re.compile(r'^([a-zA-Z_]+):\s*(.*)$')
DATE_RE = re.compile(r'^\d{4}-\d{2}-\d{2}$')
MARK_BEGIN = '<!-- news:begin'
MARK_END = '<!-- news:end -->'


def parse_front_matter(text: str) -> tuple[dict, str]:
    if not text.startswith('---'):
        return {}, text
    lines = text.split('\n')
    meta, i, closed = {}, 1, False
    while i < len(lines):
        if lines[i].strip() == '---':
            closed = True
            break
        m = KEY_RE.match(lines[i])
        if m:
            meta[m.group(1)] = m.group(2).strip().strip('"')
        i += 1
    if not closed:
        # an unclosed block would silently swallow the whole body (empty entry ships)
        raise ValueError("front matter is not closed with '---'")
    return meta, '\n'.join(lines[i + 1:])


def load_entries(news_dir: Path) -> list[dict]:
    out = []
    for f in sorted(news_dir.glob('*.md')):
        try:
            meta, body = parse_front_matter(f.read_text(encoding='utf-8'))
        except ValueError as e:
            sys.exit(f"{f}: {e}")
        if not meta.get('date') or not meta.get('title'):
            sys.exit(f"{f}: front matter needs at least date and title")
        if not DATE_RE.match(meta['date']):
            # a bad date otherwise ships an invalid Atom <updated> and sitemap <lastmod>, exit 0
            sys.exit(f"{f}: date '{meta['date']}' is not YYYY-MM-DD")
        out.append({
            'slug': f.stem,
            'date': meta['date'],
            'tag': meta.get('tag', ''),
            'title': meta['title'],
            'body_md': body.strip(),
        })
    out.sort(key=lambda e: (e['date'], e['slug']), reverse=True)
    return out


def entry_html(e: dict, lead: bool) -> str:
    md = markdown.Markdown(extensions=['fenced_code'])
    body = md.convert(e['body_md']) if e['body_md'] else ''
    cls = 'dio-item dio-item--lead is-open' if lead else 'dio-item'
    latest = '<span class="dio-latest">latest</span>' if lead else ''
    return (
        f'<article class="{cls}" id="n-{html.escape(e["slug"])}" onclick="this.classList.toggle(\'is-open\')">\n'
        f'<div class="dio-item__head"><span class="dio-item__date">{html.escape(e["date"])}</span>'
        f'<span class="dio-item__tag">{html.escape(e["tag"])}</span>'
        f'<h2 class="dio-item__title">{html.escape(e["title"])}{latest}</h2>'
        f'<span class="dio-item__chev">▾</span></div>\n'
        f'<div class="dio-item__body">{body}</div>\n'
        f'</article>'
    )


def rewrite_index(index: Path, entries: list[dict]) -> None:
    text = index.read_text(encoding='utf-8')
    if MARK_BEGIN not in text or MARK_END not in text:
        sys.exit(f"{index}: missing the news:begin/news:end markers")
    begin = text.index(MARK_BEGIN)
    begin = text.index('-->', begin) + len('-->')
    # LAST end marker is the structural closer: an entry body may legitimately contain the
    # literal '<!-- news:end -->', and it always sits BEFORE the real closer (inside the region),
    # so rfind picks the real one and a build cannot append an orphan </article> fragment
    end = text.rfind(MARK_END)
    if end < begin:
        sys.exit(f"{index}: news:end precedes news:begin")
    items = '\n'.join(entry_html(e, i == 0) for i, e in enumerate(entries))
    index.write_text(text[:begin] + '\n' + items + '\n' + text[end:],
                     encoding='utf-8', newline='\n')


def write_feed(root: Path, entries: list[dict], site_url: str) -> None:
    def iso(date: str) -> str:
        return f'{date}T00:00:00Z'
    md = markdown.Markdown(extensions=['fenced_code'])
    items = []
    for e in entries:
        link = f'{site_url}/#n-{e["slug"]}'
        body = md.convert(e['body_md']) if e['body_md'] else ''
        md.reset()
        items.append(
            f'<entry>\n'
            f'<title>{html.escape(e["title"])}</title>\n'
            f'<link href="{link}"/>\n'
            f'<id>{link}</id>\n'
            f'<updated>{iso(e["date"])}</updated>\n'
            f'<content type="html">{html.escape(body)}</content>\n'
            f'</entry>'
        )
    newest = iso(entries[0]['date']) if entries else '1970-01-01T00:00:00Z'
    feed = (
        '<?xml version="1.0" encoding="utf-8"?>\n'
        '<feed xmlns="http://www.w3.org/2005/Atom">\n'
        '<title>dasllama.io news</title>\n'
        '<author><name>dasllama.io</name></author>\n'   # RFC 4287 §4.1.1 requires a feed-level author
        f'<link href="{site_url}/"/>\n'
        f'<link href="{site_url}/feed.xml" rel="self"/>\n'
        f'<updated>{newest}</updated>\n'
        f'<id>{site_url}/</id>\n'
        + '\n'.join(items) +
        '\n</feed>\n'
    )
    (root / 'feed.xml').write_text(feed, encoding='utf-8', newline='\n')


def write_sitemap(root: Path, entries: list[dict], site_url: str) -> None:
    newest = entries[0]['date'] if entries else None
    urls = [('', newest), ('ladder.html', None), ('sidecars.html', None)]
    body = '\n'.join(
        f'<url><loc>{site_url}/{page}</loc>'
        + (f'<lastmod>{lastmod}</lastmod>' if lastmod else '')
        + '</url>'
        for page, lastmod in urls)
    (root / 'sitemap.xml').write_text(
        '<?xml version="1.0" encoding="UTF-8"?>\n'
        '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">\n'
        f'{body}\n'
        '</urlset>\n', encoding='utf-8', newline='\n')


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument('--root', default=str(Path(__file__).parent),
                    help='site-dasllama directory (index.html + _news/ inside)')
    ap.add_argument('--site-url', default='https://dasllama.io')
    args = ap.parse_args()

    root = Path(args.root)
    entries = load_entries(root / '_news')
    rewrite_index(root / 'index.html', entries)
    write_feed(root, entries, args.site_url)
    write_sitemap(root, entries, args.site_url)
    print(f'news: {len(entries)} entries -> index.html, feed.xml, sitemap.xml')


if __name__ == '__main__':
    main()
