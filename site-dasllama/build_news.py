#!/usr/bin/env python3
"""
Build the dasllama.io news feed from _news/*.md, the stories from _stories/*.md and the
articles from _articles/*.md.

Rewrites index.html between the `<!-- news:begin -->` / `<!-- news:end -->` markers (the
news live ON the home page, feed entries link to their anchor), writes the stories index
into stories.html between `<!-- stories:begin -->` / `<!-- stories:end -->` and one page per
story into stories/<slug>.html from _stories/template.html (stale pages there are removed),
the same for articles.html / articles/<slug>.html / _articles/template.html (an article's
front matter adds an author, shown on its index row and its page), and regenerates feed.xml
(Atom, news, stories and articles together) and sitemap.xml next to them.

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
ARTICLE_BEGIN = '<!-- articles:begin'
ARTICLE_END = '<!-- articles:end -->'
MD_EXTENSIONS = ['fenced_code', 'tables', 'toc']   # toc: headings get ids, so a page can link a section


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
            'author': meta.get('author', ''),
            'body_md': body.strip(),
        })
    out.sort(key=lambda e: (e['date'], e['slug']), reverse=True)
    return out


def render_md(body_md: str) -> str:
    if not body_md:
        return ''
    out = markdown.Markdown(extensions=MD_EXTENSIONS).convert(body_md)
    # a paragraph opening with "Source:" is a figure's provenance line, set small by the stylesheet
    return re.sub(r'<p>(?=Source:)', '<p class="dio-story__source">', out)


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


def page_index_html(items: list[dict], kind: str) -> str:
    rows = ''.join(
        f'<a class="forge-blog-item" href="{kind}/{html.escape(s["slug"])}.html">'
        f'<span class="forge-blog-item__date">{html.escape(s["date"])}</span>'
        f'<span class="forge-blog-item__tag">{html.escape(s["tag"])}'
        f'{" · " + html.escape(s["author"]) if s["author"] else ""}</span>'
        f'<span class="forge-blog-item__title">{html.escape(s["title"])}</span>'
        f'<span class="forge-blog-item__excerpt">{html.escape(s["lede"])}</span></a>\n'
        for s in items)
    return f'<div class="forge-blog-list dio-story-list">\n{rows}</div>'


def page_html(s: dict, template: str, site_url: str, kind: str) -> str:
    lede = f'<p class="dio-story__lede">{html.escape(s["lede"])}</p>' if s['lede'] else ''
    slots = {
        '{{title}}': html.escape(s['title']),
        '{{description}}': html.escape(s['lede'] or s['title']),
        '{{canonical}}': f'{site_url}/{kind}/{html.escape(s["slug"])}.html',
        '{{root}}': '../',
        '{{date}}': html.escape(s['date']),
        '{{tag}}': html.escape(s['tag']),
        '{{author}}': html.escape(s['author']),
        '{{lede}}': lede,
        '{{body}}': render_md(s['body_md']),
    }
    out = template
    for slot, value in slots.items():
        out = out.replace(slot, value)
    if not s['author']:
        out = out.replace('<span class="dio-story__author"></span>', '')
    return out


def write_pages(root: Path, items: list[dict], site_url: str, kind: str) -> None:
    template_path = root / f'_{kind}' / 'template.html'
    template = template_path.read_text(encoding='utf-8')
    for slot in ('{{title}}', '{{description}}', '{{canonical}}', '{{root}}', '{{body}}'):
        if slot not in template:
            sys.exit(f"{template_path}: missing the {slot} slot")
    out_dir = root / kind
    out_dir.mkdir(exist_ok=True)
    keep = {f'{s["slug"]}.html' for s in items}
    for stale in out_dir.glob('*.html'):
        if stale.name not in keep:
            stale.unlink()   # a renamed or removed entry must not leave its old page deployed
    for s in items:
        write_lf(out_dir / f'{s["slug"]}.html', page_html(s, template, site_url, kind))


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


INDEX_MARKERS = {'stories': (STORY_BEGIN, STORY_END), 'articles': (ARTICLE_BEGIN, ARTICLE_END)}


def rewrite_kind_index(page: Path, items: list[dict], kind: str) -> None:
    begin, end = INDEX_MARKERS[kind]
    rewrite_region(page, begin, end, page_index_html(items, kind))


def write_feed(root: Path, entries: list[dict], stories: list[dict], articles: list[dict], site_url: str) -> None:
    def iso(date: str) -> str:
        return f'{date}T00:00:00Z'
    items = []
    feed_entries = ([dict(e, link=f'{site_url}/#n-{e["slug"]}') for e in entries]
                    + [dict(s, link=f'{site_url}/stories/{s["slug"]}.html') for s in stories]
                    + [dict(a, link=f'{site_url}/articles/{a["slug"]}.html') for a in articles])
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


def write_sitemap(root: Path, entries: list[dict], stories: list[dict], articles: list[dict], site_url: str) -> None:
    newest = entries[0]['date'] if entries else None
    newest_story = stories[0]['date'] if stories else None
    newest_article = articles[0]['date'] if articles else None
    # ONE line: site-dasllama/REVIEW.das reads it as the top-level page census
    urls = [('', newest), ('features.html', None), ('examples.html', None), ('articles.html', newest_article), ('stories.html', newest_story), ('ladder.html', None), ('sidecars.html', None)]
    urls += [(f'articles/{a["slug"]}.html', a['date']) for a in articles]
    urls += [(f'stories/{s["slug"]}.html', s['date']) for s in stories]
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
    articles = load_entries(root / '_articles')
    rewrite_index(root / 'index.html', entries)
    rewrite_kind_index(root / 'stories.html', stories, 'stories')
    rewrite_kind_index(root / 'articles.html', articles, 'articles')
    write_pages(root, stories, args.site_url, 'stories')
    write_pages(root, articles, args.site_url, 'articles')
    write_feed(root, entries, stories, articles, args.site_url)
    write_sitemap(root, entries, stories, articles, args.site_url)
    print(f'news: {len(entries)} entries, {len(stories)} stories, {len(articles)} articles -> index.html, '
          f'stories.html, stories/*.html, articles.html, articles/*.html, feed.xml, sitemap.xml')


if __name__ == '__main__':
    main()
