import unittest
import xml.etree.ElementTree as ET
from html.parser import HTMLParser
from pathlib import Path


ROOT = Path(__file__).resolve().parent
REPO_ROOT = ROOT.parent

# the one page census every test here iterates - a page missing from it is checked by nothing,
# and REVIEW.das reads this list to hold each served .html to it
PAGES = {
    "index.html": "https://dasllama.io/",
    "examples.html": "https://dasllama.io/examples.html",
    "stories.html": "https://dasllama.io/stories.html",
    "ladder.html": "https://dasllama.io/ladder.html",
    "sidecars.html": "https://dasllama.io/sidecars.html",
}

STORY_PAGES = {
    f"stories/{p.name}": f"https://dasllama.io/stories/{p.name}"
    for p in sorted((ROOT / "stories").glob("*.html"))
}
ALL_PAGES = {**PAGES, **STORY_PAGES}


class MetadataParser(HTMLParser):
    def __init__(self):
        super().__init__()
        self.canonical = None
        self.hrefs = []
        self.title = ""
        self.description = None
        self.og = {}
        self.atom = None
        self._in_title = False

    def handle_starttag(self, tag, attrs):
        values = dict(attrs)
        if tag == "a" and "href" in values:
            self.hrefs.append(values["href"])
        if tag == "link" and values.get("rel") == "canonical":
            self.canonical = values.get("href")
        if tag == "link" and values.get("rel") == "alternate" and values.get("type") == "application/atom+xml":
            self.atom = values.get("href")
        if tag == "meta" and values.get("name") == "description":
            self.description = values.get("content")
        if tag == "meta" and str(values.get("property", "")).startswith("og:"):
            self.og[values["property"]] = values.get("content")
        if tag == "title":
            self._in_title = True

    def handle_endtag(self, tag):
        if tag == "title":
            self._in_title = False

    def handle_data(self, data):
        if self._in_title:
            self.title += data


class SiteMetadataTest(unittest.TestCase):
    def test_pages_have_one_canonical_home_identity(self):
        for filename, expected in ALL_PAGES.items():
            with self.subTest(filename=filename):
                parser = MetadataParser()
                parser.feed((ROOT / filename).read_text(encoding="utf-8"))
                self.assertEqual(parser.canonical, expected)
                self.assertNotIn("index.html", parser.hrefs)

    def test_there_is_a_page_per_story(self):
        stories = sorted(p.stem for p in (ROOT / "_stories").glob("*.md"))
        pages = sorted(p.stem for p in (ROOT / "stories").glob("*.html"))
        self.assertTrue(stories, "no stories")
        self.assertEqual(pages, stories)

    def test_pages_carry_head_metadata(self):
        # the per-page metadata the site checklist requires: a title, a description, the
        # OpenGraph quartet, and the Atom link - a new or renamed page included
        for filename in ALL_PAGES:
            with self.subTest(filename=filename):
                parser = MetadataParser()
                parser.feed((ROOT / filename).read_text(encoding="utf-8"))
                self.assertTrue(parser.title.strip(), "empty <title>")
                self.assertTrue(parser.description, "no meta description")
                for key in ("og:url", "og:type", "og:title", "og:description"):
                    self.assertTrue(parser.og.get(key), f"missing {key}")
                self.assertEqual(parser.og["og:url"], parser.canonical)
                self.assertEqual(parser.atom, "/feed.xml")

    def test_sitemap_uses_the_canonical_urls(self):
        tree = ET.parse(ROOT / "sitemap.xml")
        namespace = {"sm": "http://www.sitemaps.org/schemas/sitemap/0.9"}
        urls = [node.text for node in tree.findall("sm:url/sm:loc", namespace)]
        self.assertEqual(urls[:len(PAGES)], list(PAGES.values()))
        self.assertEqual(sorted(urls[len(PAGES):]), sorted(STORY_PAGES.values()))

    def test_feed_links_to_home_page_anchors(self):
        feed = (ROOT / "feed.xml").read_text(encoding="utf-8")
        self.assertNotIn("https://dasllama.io/index.html", feed)
        self.assertIn("https://dasllama.io/#n-", feed)

    def test_caddy_redirects_explicit_index(self):
        snippet = (
            REPO_ROOT / "utils" / "internal" / "dasllama-ladder" / "caddy.snippet"
        ).read_text(encoding="utf-8")
        self.assertIn("redir /index.html / 308", snippet)


if __name__ == "__main__":
    unittest.main()
