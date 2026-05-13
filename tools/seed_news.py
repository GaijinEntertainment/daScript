#!/usr/bin/env python3
"""
Seed `site/_news/<slug>.md` files from the legacy `page-news` div in the
pre-redesign `site/index.html`. One-time tool — re-runnable but should only
be needed once during the website-forge migration.

Usage:
    python3 tools/seed_news.py <old_index.html> <out_dir>

Defaults:
    old_index.html = /tmp/old_site_index.html
    out_dir        = site/_news
"""

import re
import sys
from html import unescape
from pathlib import Path

MONTHS = {
    'jan': 1, 'january': 1,
    'feb': 2, 'february': 2,
    'mar': 3, 'march': 3,
    'apr': 4, 'april': 4,
    'may': 5,
    'jun': 6, 'june': 6,
    'jul': 7, 'july': 7,
    'aug': 8, 'august': 8,
    'sep': 9, 'sept': 9, 'september': 9,
    'oct': 10, 'october': 10,
    'nov': 11, 'november': 11,
    'dec': 12, 'december': 12,
}

# Examples this needs to parse:
#   May 9th, 2026 / May 9, 2026
#   1st Jan 2023 / 1 Jan 2023
#   24 August 2020
#   November 18th, 2021
DATE_PATTERNS = [
    # "May 9th, 2026" / "May 9, 2026"
    re.compile(r'\b([A-Za-z]+)\s+(\d{1,2})(?:st|nd|rd|th)?,?\s*(\d{4})\b'),
    # "9 May 2026" / "1st Jan 2023"
    re.compile(r'\b(\d{1,2})(?:st|nd|rd|th)?\s+([A-Za-z]+)\s+(\d{4})\b'),
]

def parse_date(text):
    for pat in DATE_PATTERNS:
        m = pat.search(text)
        if not m:
            continue
        groups = m.groups()
        # Identify which group is the month name
        if groups[0].lower() in MONTHS:
            month, day, year = groups[0], int(groups[1]), int(groups[2])
        elif groups[1].lower() in MONTHS:
            day, month, year = int(groups[0]), groups[1], int(groups[2])
        else:
            continue
        month_num = MONTHS.get(month.lower())
        if not month_num:
            continue
        return f"{year:04d}-{month_num:02d}-{day:02d}"
    return None


VERSION_RE = re.compile(r'\b(\d+\.\d+(?:\.\d+)?(?:-RC\d+)?)\b')

def infer_tag(body, title):
    """Pick a short tag — prefer a version mention, fall back to a generic."""
    for src in (body, title):
        m = VERSION_RE.search(src)
        if m:
            return m.group(1).lower()
    text = (body + ' ' + title).lower()
    if 'strudel' in text:
        return 'strudel'
    if 'jit' in text:
        return 'jit'
    if 'site' in text or 'website' in text or 'documentation' in text:
        return 'docs'
    if 'blog' in text:
        return 'blog'
    return 'news'


SLUG_BAD = re.compile(r'[^a-z0-9]+')

def slugify(date, title):
    body = SLUG_BAD.sub('-', title.lower()).strip('-')[:40].strip('-')
    return f"{date}-{body or 'entry'}"


HREF_RE = re.compile(r'<a[^>]*href="([^"]*)"[^>]*>', re.IGNORECASE)
TAG_RE = re.compile(r'<[^>]+>')
WS_RE = re.compile(r'\s+')

def extract_link(html):
    """First <a> with an http(s) href, or any non-anchor (e.g. /doc/) link."""
    external = None
    internal = None
    for m in HREF_RE.finditer(html):
        href = m.group(1)
        if not href or href.startswith('#'):
            continue
        if href.startswith('http'):
            if not external:
                external = href
        elif not internal:
            internal = href
    return external or internal


def strip_html(s):
    s = unescape(s)
    s = TAG_RE.sub('', s)
    return WS_RE.sub(' ', s).strip()


# Entry start: <li>...<b>DATE</b>: BODY</li>
ENTRY_RE = re.compile(
    r'<li>\s*<b>([^<]+)</b>\s*:?\s*(.*?)</li>',
    re.DOTALL,
)


def seed(input_path, out_dir):
    html = Path(input_path).read_text(encoding='utf-8')
    # Limit to the page-news div
    start = html.find('id="page-news"')
    end = html.find('end page-news', start)
    if start == -1:
        sys.exit("page-news div not found")
    section = html[start:end] if end != -1 else html[start:]

    out = Path(out_dir)
    out.mkdir(parents=True, exist_ok=True)

    seeded = 0
    skipped = 0
    for m in ENTRY_RE.finditer(section):
        bold_text, body_html = m.group(1), m.group(2)
        date = parse_date(bold_text)
        if not date:
            skipped += 1
            print(f"skip (no date): {bold_text!r}", file=sys.stderr)
            continue
        link = extract_link(body_html)
        title = strip_html(body_html)
        # Trim trailing "See the Change List for details" cruft
        title = re.sub(r'\s+See the Change List.*', '', title)
        title = re.sub(r'\s+See plans .*', '', title)
        # Strip leading punctuation
        title = title.lstrip(' .;,')
        if not title:
            skipped += 1
            continue
        tag = infer_tag(body_html + ' ' + title, '')
        slug = slugify(date, title)
        path = out / f"{slug}.md"

        fm_lines = [
            "---",
            f"date: {date}",
            f"tag: {tag}",
            f"title: {title.replace(chr(10), ' ')}",
        ]
        if link:
            fm_lines.append(f"link: {link}")
        fm_lines.append("---")
        path.write_text("\n".join(fm_lines) + "\n", encoding='utf-8')
        seeded += 1

    print(f"seeded {seeded} entries to {out}/ (skipped {skipped})")


if __name__ == "__main__":
    src = sys.argv[1] if len(sys.argv) > 1 else "/tmp/old_site_index.html"
    dst = sys.argv[2] if len(sys.argv) > 2 else "site/_news"
    seed(src, dst)
