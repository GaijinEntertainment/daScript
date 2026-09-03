#!/usr/bin/env python3
"""
Build the dasllama.io news feed from _news/*.md and the stories page from _stories/*.md.

Rewrites index.html between the `<!-- news:begin -->` / `<!-- news:end -->` markers,
stories.html between `<!-- stories:begin -->` / `<!-- stories:end -->`, and regenerates
feed.xml (Atom, news and stories together) and sitemap.xml next to them. Entries live ON
their page — there are no per-entry pages; feed entries link to the entry's anchor.

The generated output is CHECKED IN (unlike daslang.io's blog, which builds into _site
only): the preview rig serves the repo tree directly, and the site must preview exactly
as it deploys. The deploy step runs this same script so a forgotten local run cannot
ship a stale board — the deploy regenerates from _news/ and _stories/ either way.

Front matter keys: date (YYYY-MM-DD), tag, title; stories add lede. Body is Markdown
(fenced code and tables).

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
STORY_BEGIN = '<!-- stories:begin'
STORY_END = '<!-- stories:end -->'
MD_EXTENSIONS = ['fenced_code', 'tables']


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
            'lede': meta.get('lede', ''),
            'body_md': body.strip(),
        })
    out.sort(key=lambda e: (e['date'], e['slug']), reverse=True)
    return out


def render_md(body_md: str) -> str:
    return markdown.Markdown(extensions=MD_EXTENSIONS).convert(body_md) if body_md else ''


def entry_html(e: dict, lead: bool) -> str:
    body = render_md(e['body_md'])
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


def story_index_html(stories: list[dict]) -> str:
    # the contents list at the top of the page: the shared blog-list language from forge.css
    rows = ''.join(
        f'<a class="forge-blog-item" href="#s-{html.escape(s["slug"])}">'
        f'<span class="forge-blog-item__date">{html.escape(s["date"])}</span>'
        f'<span class="forge-blog-item__tag">{html.escape(s["tag"])}</span>'
        f'<span class="forge-blog-item__title">{html.escape(s["title"])}</span>'
        f'<span class="forge-blog-item__excerpt">{html.escape(s["lede"])}</span></a>\n'
        for s in stories)
    return f'<div class="forge-blog-list dio-story-list">\n{rows}</div>'


def story_html(s: dict) -> str:
    body = render_md(s['body_md'])
    lede = f'<p class="dio-story__lede">{html.escape(s["lede"])}</p>\n' if s['lede'] else ''
    return (
        f'<article class="dio-story" id="s-{html.escape(s["slug"])}">\n'
        f'<div class="dio-story__eyebrow"><span class="dio-item__date">{html.escape(s["date"])}</span>'
        f'<span class="dio-item__tag">{html.escape(s["tag"])}</span></div>\n'
        f'<h2 class="dio-story__title">{html.escape(s["title"])}</h2>\n'
        f'{lede}'
        f'<div class="dio-story__body">{body}</div>\n'
        f'</article>'
    )


def write_lf(path: Path, text: str) -> None:
    # not Path.write_text(newline=...): that keyword needs Python 3.10+, and the checklist
    # tells authors to re-run this script on the stock macOS interpreter (3.9)
    with open(path, 'w', encoding='utf-8', newline='\n') as f:
        f.write(text)


def rewrite_region(page: Path, mark_begin: str, mark_end: str, items: str) -> None:
    text = page.read_text(encoding='utf-8')
    if mark_begin not in text or mark_end not in text:
        sys.exit(f"{page}: missing the {mark_begin[5:]}/{mark_end[5:-4]} markers")
    begin = text.index(mark_begin)
    begin = text.index('-->', begin) + len('-->')
    # LAST end marker is the structural closer: an entry body may legitimately contain the
    # literal end marker, and it always sits BEFORE the real closer (inside the region), so
    # rfind picks the real one and a build cannot append an orphan </article> fragment
    end = text.rfind(mark_end)
    if end < begin:
        sys.exit(f"{page}: the end marker precedes the begin marker")
    write_lf(page, text[:begin] + '\n' + items + '\n' + text[end:])


def rewrite_index(index: Path, entries: list[dict]) -> None:
    rewrite_region(index, MARK_BEGIN, MARK_END,
                   '\n'.join(entry_html(e, i == 0) for i, e in enumerate(entries)))


def rewrite_stories(page: Path, stories: list[dict]) -> None:
    rewrite_region(page, STORY_BEGIN, STORY_END,
                   story_index_html(stories) + '\n' + '\n'.join(story_html(s) for s in stories))


def write_feed(root: Path, entries: list[dict], stories: list[dict], site_url: str) -> None:
    def iso(date: str) -> str:
        return f'{date}T00:00:00Z'
    items = []
    feed_entries = ([dict(e, link=f'{site_url}/#n-{e["slug"]}') for e in entries]
                    + [dict(s, link=f'{site_url}/stories.html#s-{s["slug"]}') for s in stories])
    feed_entries.sort(key=lambda e: (e['date'], e['slug']), reverse=True)
    for e in feed_entries:
        link = e['link']
        body = render_md(e['body_md'])
        items.append(
            f'<entry>\n'
            f'<title>{html.escape(e["title"])}</title>\n'
            f'<link href="{link}"/>\n'
            f'<id>{link}</id>\n'
            f'<updated>{iso(e["date"])}</updated>\n'
            f'<content type="html">{html.escape(body)}</content>\n'
            f'</entry>'
        )
    newest = iso(feed_entries[0]['date']) if feed_entries else '1970-01-01T00:00:00Z'
    feed = (
        '<?xml version="1.0" encoding="utf-8"?>\n'
        '<feed xmlns="http://www.w3.org/2005/Atom">\n'
        '<title>dasllama.io news and stories</title>\n'
        '<author><name>dasllama.io</name></author>\n'   # RFC 4287 §4.1.1 requires a feed-level author
        f'<link href="{site_url}/"/>\n'
        f'<link href="{site_url}/feed.xml" rel="self"/>\n'
        f'<updated>{newest}</updated>\n'
        f'<id>{site_url}/</id>\n'
        + '\n'.join(items) +
        '\n</feed>\n'
    )
    write_lf(root / 'feed.xml', feed)


def write_sitemap(root: Path, entries: list[dict], stories: list[dict], site_url: str) -> None:
    newest = entries[0]['date'] if entries else None
    newest_story = stories[0]['date'] if stories else None
    # ONE line: site-dasllama/REVIEW.das reads it as the page census
    urls = [('', newest), ('stories.html', newest_story), ('ladder.html', None), ('sidecars.html', None)]
    body = '\n'.join(
        f'<url><loc>{site_url}/{page}</loc>'
        + (f'<lastmod>{lastmod}</lastmod>' if lastmod else '')
        + '</url>'
        for page, lastmod in urls)
    write_lf(root / 'sitemap.xml',
             '<?xml version="1.0" encoding="UTF-8"?>\n'
             '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">\n'
             f'{body}\n'
             '</urlset>\n')


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument('--root', default=str(Path(__file__).parent),
                    help='site-dasllama directory (index.html + _news/ inside)')
    ap.add_argument('--site-url', default='https://dasllama.io')
    args = ap.parse_args()

    root = Path(args.root)
    entries = load_entries(root / '_news')
    stories = load_entries(root / '_stories')
    rewrite_index(root / 'index.html', entries)
    rewrite_stories(root / 'stories.html', stories)
    write_feed(root, entries, stories, args.site_url)
    write_sitemap(root, entries, stories, args.site_url)
    print(f'news: {len(entries)} entries, {len(stories)} stories -> index.html, stories.html, feed.xml, sitemap.xml')


if __name__ == '__main__':
    main()
