#!/usr/bin/env python3
"""
Build the daslang.io blog + changelist + landing news feed from Markdown.

Input:
    posts_dir/  — long-form blog posts (Hexo-format Markdown)
    news_dir/   — short news entries (front-matter-only, body optional)
    template    — Forge HTML wrapper with {{title}} / {{body}} / {{root}} / {{description}} slots

Output (under out_dir):
    blog/index.html         — post listing
    blog/<slug>.html        — one per post
    blog/feed.xml           — Atom feed of posts
    news/<slug>.html        — one per news entry that has a body
    changelist.html         — full long-form news list (all entries by date desc)
    files/news.json         — top-N news for forge.js to render landing § 05

Hexo extensions translated:
    <!-- more -->            → excerpt boundary (excerpt above used on index)
    {% post_link <slug> %}   → <a href="<slug>.html">post title</a>

Usage:
    python3 build_blog.py --posts site/blog/_posts --news site/_news \\
        --template site/blog/template.html --out _site/

Local dev: pass --out site/ to write next to the sources so you can preview
from `cd site && python3 -m http.server`.
"""

from __future__ import annotations

import argparse
import html
import re
import sys
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path

try:
    import markdown
except ImportError as e:
    sys.exit(f"build_blog.py requires `markdown`: pip install markdown  ({e})")


# ─── Front matter ────────────────────────────────────────────────────

# Minimal YAML-ish parser for the small set of keys we use. Avoids the
# pyyaml dep so the CI step is just `pip install markdown pygments`.

KEY_RE = re.compile(r'^([a-zA-Z_]+):\s*(.*)$')

# Disqus shortname carried over from the legacy borisbat.github.io/dascf-blog
# install — the same account, but new threads under daslang.io URLs (Disqus
# resolves identifier first, then page.url). Old comments stay under the
# original blog URLs in the Disqus admin and can be migrated via the
# Disqus URL Mapper if desired.
DISQUS_SHORTNAME = 'https-borisbat-github-io-dascf-blog'


@dataclass
class Entry:
    slug: str
    title: str
    date: str           # YYYY-MM-DD
    tag: str = ''
    tags: list = field(default_factory=list)
    link: str = ''
    body_md: str = ''
    excerpt_md: str = ''
    has_body: bool = False
    kind: str = 'post'  # 'post' or 'news'
    source: Path = None


def parse_front_matter(text: str) -> tuple[dict, str]:
    if not text.startswith('---'):
        return {}, text
    parts = text.split('---', 2)
    if len(parts) < 3:
        return {}, text
    fm: dict = {}
    current_key = None
    for line in parts[1].splitlines():
        if not line.strip():
            continue
        # Continuation of a list (Hexo "tags:\n  - foo\n  - bar")
        m_list = re.match(r'^\s*-\s*(.*)$', line)
        if m_list and current_key:
            fm.setdefault(current_key + '__list', []).append(m_list.group(1).strip())
            continue
        m = KEY_RE.match(line)
        if m:
            key, val = m.group(1), m.group(2).strip()
            current_key = key
            if val:
                fm[key] = val
    # Collapse <key>__list into <key>
    for key in list(fm):
        if key.endswith('__list'):
            fm[key[:-6]] = fm.pop(key)
    return fm, parts[2].lstrip('\n')


# ─── Slug + date helpers ─────────────────────────────────────────────

def slug_of(path: Path) -> str:
    return path.stem


DATE_DT_RE = re.compile(r'^(\d{4})-(\d{2})-(\d{2})')

def normalize_date(s: str) -> str:
    """Accept YYYY-MM-DD or YYYY-MM-DD HH:MM:SS, return YYYY-MM-DD."""
    if not s:
        return '1970-01-01'
    m = DATE_DT_RE.match(s)
    if m:
        return f"{m.group(1)}-{m.group(2)}-{m.group(3)}"
    return s


# ─── Hexo extension preprocessing ────────────────────────────────────

POST_LINK_RE = re.compile(r'\{\%\s*post_link\s+([a-zA-Z0-9_\-]+)(?:\s+([^%]+?))?\s*\%\}')

def expand_post_links(md_body: str, posts_by_slug: dict) -> str:
    """{% post_link slug %} → markdown link to the post."""
    def repl(m):
        slug = m.group(1)
        custom = (m.group(2) or '').strip()
        target = posts_by_slug.get(slug)
        title = custom or (target.title if target else slug)
        return f"[{title}]({slug}.html)"
    return POST_LINK_RE.sub(repl, md_body)


# Hexo posts use 4-space indents for code blocks; some have trailing :: lines
# (RST habit). Convert RST-style "::\n\n    code" into ```daslang fences.
RST_CODE_BLOCK_RE = re.compile(
    r'(::\s*\n)(\n(?:    [^\n]*\n?)+)',  # "::" line then indented block
    re.MULTILINE,
)

def normalize_rst_code_blocks(md_body: str) -> str:
    def repl(m):
        header, block = m.group(1), m.group(2)
        # Strip 4-space indent from the block
        lines = [ln[4:] if ln.startswith('    ') else ln for ln in block.lstrip('\n').splitlines()]
        return '\n```daslang\n' + '\n'.join(lines).rstrip() + '\n```\n'
    return RST_CODE_BLOCK_RE.sub(repl, md_body)


# Convert pure 4-space-indented blocks (no preceding ::) into daslang fences.
INDENT_BLOCK_RE = re.compile(
    r'(?:^\n)((?:^    [^\n]*\n?)+)',
    re.MULTILINE,
)

def normalize_indent_code_blocks(md_body: str) -> str:
    """Replace 4-space-indent code blocks not already inside a fence with
    fenced daslang blocks. Skips blocks following a list marker."""
    out_lines = []
    in_fence = False
    in_list_block = False
    indent_block: list[str] = []
    for line in md_body.splitlines(keepends=True):
        if line.startswith('```'):
            in_fence = not in_fence
            if indent_block:
                out_lines.extend(indent_block); indent_block = []
            out_lines.append(line)
            continue
        if in_fence:
            out_lines.append(line)
            continue
        # Detect list continuation: lines starting with "- ", "* ", "1. " then
        # subsequent 4-space-indented lines belong to that list item.
        bare = line.lstrip(' \t')
        if re.match(r'^(?:[-*+]\s|\d+\.\s)', bare):
            in_list_block = True
            if indent_block:
                out_lines.extend(indent_block); indent_block = []
            out_lines.append(line)
            continue
        if line.strip() == '':
            if indent_block:
                # Flush as a fenced block
                code = ''.join(ln[4:] if ln.startswith('    ') else ln for ln in indent_block)
                out_lines.append('\n```daslang\n' + code.rstrip() + '\n```\n')
                indent_block = []
            in_list_block = False
            out_lines.append(line)
            continue
        if line.startswith('    ') and not in_list_block:
            indent_block.append(line)
            continue
        if indent_block:
            out_lines.extend(indent_block); indent_block = []
        out_lines.append(line)
    if indent_block:
        code = ''.join(ln[4:] if ln.startswith('    ') else ln for ln in indent_block)
        out_lines.append('\n```daslang\n' + code.rstrip() + '\n```\n')
    return ''.join(out_lines)


def split_excerpt(md_body: str) -> tuple[str, str]:
    """Split on <!-- more -->. Returns (excerpt, full)."""
    if '<!-- more -->' in md_body:
        excerpt, _, _ = md_body.partition('<!-- more -->')
        return excerpt.strip(), md_body.replace('<!-- more -->', '').strip()
    return md_body.strip()[:240], md_body.strip()


# ─── Markdown renderer ───────────────────────────────────────────────

def make_md():
    # No codehilite — daslang isn't in pygments. We emit standard
    # CommonMark `<pre><code class="language-X">` and let
    # site/files/highlight.js tokenize daslang blocks client-side.
    return markdown.Markdown(
        extensions=['fenced_code', 'tables', 'def_list', 'attr_list'],
    )


# ─── Page rendering ──────────────────────────────────────────────────

def render_template(tpl: str, root: str, title: str, body: str, description: str = '') -> str:
    return (tpl
        .replace('{{root}}', root)
        .replace('{{title}}', html.escape(title))
        .replace('{{description}}', html.escape(description or title))
        .replace('{{body}}', body))


def render_post(entry: Entry, prev: Entry | None, next: Entry | None, md, posts_by_slug) -> str:
    body_md = expand_post_links(entry.body_md, posts_by_slug)
    body_md = normalize_rst_code_blocks(body_md)
    body_md = normalize_indent_code_blocks(body_md)
    md.reset()
    body_html = md.convert(body_md)
    tags_html = ''
    if entry.tags:
        tags_html = ' · '.join(html.escape(t) for t in entry.tags)
    elif entry.tag:
        tags_html = html.escape(entry.tag)
    nav_html = ''
    parts = []
    if prev:
        parts.append(f'<a href="{prev.slug}.html">← {html.escape(prev.title)}</a>')
    else:
        parts.append('<span></span>')
    if next:
        parts.append(f'<a href="{next.slug}.html">{html.escape(next.title)} →</a>')
    else:
        parts.append('<span></span>')
    nav_html = f'<div class="forge-post-nav">{parts[0]}{parts[1]}</div>'
    comments_html = render_comments(entry)
    return f"""    <article class="forge-post">
        <div class="forge-container">
            <div class="forge-post__meta">
                <span>{html.escape(entry.date)}</span>
                {('<span class="forge-news__tag">' + tags_html + '</span>') if tags_html else ''}
            </div>
            <h1 class="forge-post__title">{html.escape(entry.title)}</h1>
            <div class="forge-post__body">
                {body_html}
            </div>
            {nav_html}
        </div>
    </article>
{comments_html}"""


def render_comments(entry: Entry) -> str:
    """Disqus thread, pinned by post slug as identifier so threads survive
    URL changes (dev/prod, future renames). The embed script reads
    `disqus_config` for the URL + identifier."""
    slug = html.escape(entry.slug, quote=True)
    return f"""    <section class="forge-post__comments">
        <div class="forge-container">
            <div class="forge-section-label">
                <span class="forge-section-label__num">§</span>
                <span class="forge-section-label__rule"></span>
                <span>comments</span>
            </div>
            <div id="disqus_thread"></div>
            <script>
                var disqus_config = function () {{
                    this.page.url = window.location.href;
                    this.page.identifier = "{slug}";
                }};
                (function() {{
                    var d = document, s = d.createElement('script');
                    s.src = 'https://{DISQUS_SHORTNAME}.disqus.com/embed.js';
                    s.setAttribute('data-timestamp', +new Date());
                    (d.head || d.body).appendChild(s);
                }})();
            </script>
            <noscript>Comments require JavaScript — <a href="https://disqus.com/?ref_noscript">comments powered by Disqus</a>.</noscript>
        </div>
    </section>
"""


def render_index(posts: list[Entry], md) -> str:
    items = []
    for p in posts:
        md.reset()
        excerpt_html = md.convert(p.excerpt_md) if p.excerpt_md else ''
        tag = p.tags[0] if p.tags else p.tag
        items.append(f"""        <div class="forge-blog-item">
            <div class="forge-blog-item__date">{html.escape(p.date)}</div>
            <div class="forge-blog-item__tag">{html.escape(tag)}</div>
            <div>
                <div class="forge-blog-item__title"><a href="{p.slug}.html">{html.escape(p.title)}</a></div>
                <div class="forge-blog-item__excerpt">{excerpt_html}</div>
            </div>
        </div>""")
    return f"""    <section class="forge-section">
        <div class="forge-container">
            <div class="forge-section-label">
                <span class="forge-section-label__num">§</span>
                <span class="forge-section-label__rule"></span>
                <span>blog</span>
            </div>
            <h1 class="forge-h2" style="font-size:44px;">Notes from the language.</h1>
            <p class="forge-p" style="max-width:640px;">
                Design rationale, refactor stories, and the occasional shipping
                announcement. Newest first. <a href="feed.xml">RSS</a>.
            </p>
            <div class="forge-blog-list">
{chr(10).join(items)}
            </div>
        </div>
    </section>
"""


def render_changelist(news: list[Entry]) -> str:
    rows = []
    for n in news:
        href = n.link or (f"news/{n.slug}.html" if n.has_body else '')
        title = html.escape(n.title)
        title_html = f'<a href="{html.escape(href)}">{title}</a>' if href else title
        rows.append(f"""        <div class="forge-blog-item">
            <div class="forge-blog-item__date">{html.escape(n.date)}</div>
            <div class="forge-blog-item__tag">{html.escape(n.tag)}</div>
            <div class="forge-blog-item__title">{title_html}</div>
        </div>""")
    return f"""    <section class="forge-section">
        <div class="forge-container">
            <div class="forge-section-label">
                <span class="forge-section-label__num">§</span>
                <span class="forge-section-label__rule"></span>
                <span>change list</span>
            </div>
            <h1 class="forge-h2" style="font-size:44px;">Everything, in order.</h1>
            <p class="forge-p" style="max-width:640px;">
                Releases, tooling, ecosystem notes, and site updates. Newest
                first. The top 5 also appear in § 05 on the landing.
            </p>
            <div class="forge-changelist">
{chr(10).join(rows)}
            </div>
        </div>
    </section>
"""


def render_news_page(entry: Entry, md) -> str:
    md.reset()
    body_html = md.convert(entry.body_md)
    return f"""    <article class="forge-post">
        <div class="forge-container">
            <div class="forge-post__meta">
                <span>{html.escape(entry.date)}</span>
                <span class="forge-news__tag">{html.escape(entry.tag)}</span>
            </div>
            <h1 class="forge-post__title">{html.escape(entry.title)}</h1>
            <div class="forge-post__body">
                {body_html}
            </div>
        </div>
    </article>
"""


def render_atom_feed(posts: list[Entry], site_url: str) -> str:
    updated = posts[0].date if posts else '1970-01-01'
    entries = []
    for p in posts:
        url = f"{site_url}/blog/{p.slug}.html"
        entries.append(f"""<entry>
  <title>{html.escape(p.title)}</title>
  <link href="{url}"/>
  <id>{url}</id>
  <updated>{p.date}T00:00:00Z</updated>
  <summary type="html">{html.escape(p.excerpt_md)}</summary>
</entry>""")
    return f"""<?xml version="1.0" encoding="utf-8"?>
<feed xmlns="http://www.w3.org/2005/Atom">
<title>Daslang blog</title>
<link href="{site_url}/blog/feed.xml" rel="self"/>
<link href="{site_url}/blog/"/>
<updated>{updated}T00:00:00Z</updated>
<id>{site_url}/blog/</id>
{chr(10).join(entries)}
</feed>
"""


# ─── Main ─────────────────────────────────────────────────────────────

def load_posts(d: Path) -> list[Entry]:
    out = []
    for f in sorted(d.glob('*.md')):
        text = f.read_text(encoding='utf-8')
        fm, body = parse_front_matter(text)
        if not fm.get('title') or not fm.get('date'):
            print(f"warn: {f.name} missing title/date — skipping", file=sys.stderr)
            continue
        excerpt, full = split_excerpt(body)
        out.append(Entry(
            slug=slug_of(f),
            title=fm['title'],
            date=normalize_date(fm['date']),
            tag=fm.get('tag', ''),
            tags=fm.get('tags') if isinstance(fm.get('tags'), list) else (
                [fm['tags']] if fm.get('tags') else []),
            body_md=full,
            excerpt_md=excerpt,
            has_body=bool(full.strip()),
            kind='post',
            source=f,
        ))
    out.sort(key=lambda e: e.date, reverse=True)
    return out


def load_news(d: Path) -> list[Entry]:
    out = []
    if not d.exists():
        return out
    for f in sorted(d.glob('*.md')):
        text = f.read_text(encoding='utf-8')
        fm, body = parse_front_matter(text)
        if not fm.get('title') or not fm.get('date'):
            print(f"warn: {f.name} missing title/date — skipping", file=sys.stderr)
            continue
        body = body.strip()
        out.append(Entry(
            slug=slug_of(f),
            title=fm['title'],
            date=normalize_date(fm['date']),
            tag=fm.get('tag', 'news'),
            link=fm.get('link', ''),
            body_md=body,
            has_body=bool(body),
            kind='news',
            source=f,
        ))
    out.sort(key=lambda e: e.date, reverse=True)
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--posts', required=True, type=Path)
    ap.add_argument('--news', type=Path)
    ap.add_argument('--template', required=True, type=Path)
    ap.add_argument('--out', required=True, type=Path)
    ap.add_argument('--site-url', default='https://daslang.io')
    args = ap.parse_args()

    tpl = args.template.read_text(encoding='utf-8')
    out = args.out
    out.mkdir(parents=True, exist_ok=True)
    (out / 'blog').mkdir(parents=True, exist_ok=True)

    md = make_md()

    posts = load_posts(args.posts)
    posts_by_slug = {p.slug: p for p in posts}

    # 1. Per-post pages
    for i, p in enumerate(posts):
        prev_ = posts[i - 1] if i > 0 else None
        next_ = posts[i + 1] if i + 1 < len(posts) else None
        body = render_post(p, prev_, next_, md, posts_by_slug)
        html_out = render_template(tpl, root='../', title=p.title, body=body,
                                   description=(p.excerpt_md[:160] if p.excerpt_md else p.title))
        (out / 'blog' / f'{p.slug}.html').write_text(html_out, encoding='utf-8')

    # 2. Blog index
    index_body = render_index(posts, md)
    (out / 'blog' / 'index.html').write_text(
        render_template(tpl, root='../', title='Blog', body=index_body,
                        description='Daslang blog — design notes, refactor stories, releases.'),
        encoding='utf-8')

    # 3. Atom feed
    (out / 'blog' / 'feed.xml').write_text(
        render_atom_feed(posts, args.site_url), encoding='utf-8')

    # 4. News + changelist
    news: list[Entry] = []
    if args.news:
        news = load_news(args.news)
        (out / 'news').mkdir(parents=True, exist_ok=True)
        for n in news:
            if n.has_body:
                body = render_news_page(n, md)
                html_out = render_template(tpl, root='../', title=n.title, body=body,
                                           description=n.title)
                (out / 'news' / f'{n.slug}.html').write_text(html_out, encoding='utf-8')

    # 5. changelist.html — full long-form news list
    changelist_body = render_changelist(news)
    (out / 'changelist.html').write_text(
        render_template(tpl, root='', title='Change list',
                        body=changelist_body,
                        description='Releases, tooling, and ecosystem updates.'),
        encoding='utf-8')

    # 6. news.json for landing § 05 (top 5)
    (out / 'files').mkdir(parents=True, exist_ok=True)
    import json
    top_news = [
        {
            'date': n.date, 'tag': n.tag, 'title': n.title,
            'link': n.link or (f"news/{n.slug}.html" if n.has_body else ''),
        }
        for n in news[:8]
    ]
    (out / 'files' / 'news.json').write_text(
        json.dumps(top_news, indent=2), encoding='utf-8')

    # 7. blog.json — drives the "N NEW" nav chip. baseline_date is the
    #    date of the second-newest post: a first-time visitor (no
    #    localStorage) sees exactly one post counted as new (the newest).
    newest_date = posts[0].date if posts else '1970-01-01'
    baseline_date = posts[1].date if len(posts) >= 2 else '1970-01-01'
    blog_data = {
        'newest_date': newest_date,
        'baseline_date': baseline_date,
        'posts': [{'slug': p.slug, 'date': p.date} for p in posts],
    }
    (out / 'files' / 'blog.json').write_text(
        json.dumps(blog_data, indent=2), encoding='utf-8')

    print(f"built {len(posts)} posts, {len(news)} news entries → {out}/")


if __name__ == '__main__':
    main()
