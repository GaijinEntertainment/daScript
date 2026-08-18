import unittest
import xml.etree.ElementTree as ET
from html.parser import HTMLParser
from pathlib import Path


ROOT = Path(__file__).resolve().parent
REPO_ROOT = ROOT.parent


class MetadataParser(HTMLParser):
    def __init__(self):
        super().__init__()
        self.canonical = None
        self.hrefs = []

    def handle_starttag(self, tag, attrs):
        values = dict(attrs)
        if tag == "a" and "href" in values:
            self.hrefs.append(values["href"])
        if tag == "link" and values.get("rel") == "canonical":
            self.canonical = values.get("href")


class SiteMetadataTest(unittest.TestCase):
    def test_pages_have_one_canonical_home_identity(self):
        pages = {
            "index.html": "https://dasllama.io/",
            "ladder.html": "https://dasllama.io/ladder.html",
            "sidecars.html": "https://dasllama.io/sidecars.html",
        }
        for filename, expected in pages.items():
            with self.subTest(filename=filename):
                parser = MetadataParser()
                parser.feed((ROOT / filename).read_text(encoding="utf-8"))
                self.assertEqual(parser.canonical, expected)
                self.assertNotIn("index.html", parser.hrefs)

    def test_sitemap_uses_the_canonical_urls(self):
        tree = ET.parse(ROOT / "sitemap.xml")
        namespace = {"sm": "http://www.sitemaps.org/schemas/sitemap/0.9"}
        urls = [node.text for node in tree.findall("sm:url/sm:loc", namespace)]
        self.assertEqual(
            urls,
            [
                "https://dasllama.io/",
                "https://dasllama.io/ladder.html",
                "https://dasllama.io/sidecars.html",
            ],
        )

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
