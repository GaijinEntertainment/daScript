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

    def test_examples_are_isolated_with_require_corp(self):
        """The -pthread browser examples need a cross-origin-isolated page on every engine, and
        WebKit implements only `require-corp`; the local preview server mirrors the vhost."""
        snippet = (
            REPO_ROOT / "utils" / "internal" / "dasllama-ladder" / "caddy.snippet"
        ).read_text(encoding="utf-8")
        serve = (ROOT / "serve.py").read_text(encoding="utf-8")
        for text, where in ((snippet, "caddy.snippet"), (serve, "serve.py")):
            self.assertIn("Cross-Origin-Embedder-Policy", text, f"{where} sets COEP on /examples/")
            self.assertNotIn("credentialless", text, f"{where} sends a COEP value WebKit implements")
        self.assertIn("Cross-Origin-Embedder-Policy require-corp", snippet)
        self.assertIn('"Cross-Origin-Embedder-Policy", "require-corp"', serve)


EXAMPLE_SHELLS = {
    "storyteller": (REPO_ROOT / "examples" / "dasLLAMA" / "storyteller" / "web_shell.html", "runStoryteller"),
    "storywish": (REPO_ROOT / "examples" / "dasLLAMA" / "storywish" / "web_shell.html", "runStorywish"),
    "parrot": (REPO_ROOT / "examples" / "dasLLAMA" / "parrot" / "web_shell.html", "runParrot"),
}


class ExampleShellTest(unittest.TestCase):
    """Each browser example's page (examples/dasLLAMA/<id>/web_shell.html, served at
    /examples/<id>/) guards the browser before it loads the program: emcc's script tag lands in
    an inert template and only a cross-origin-isolated page gets a live copy. A shell that moved
    the placeholder out of the template would run emcc's own user-agent check before the note and
    leave the download line up forever. Each shell also reads the
    deploy's models/manifest.json and carries the IMAGE_VERSION slot the deploy stamps
    (examples/dasLLAMA/wasm/mint_models.py --stamp-page), so a set minted for another version is
    refused before it is fetched, and routes a program abort back onto the page."""

    def shells(self):
        return [(name, path.read_text(encoding="utf-8"), runner) for name, (path, runner) in EXAMPLE_SHELLS.items()]

    def test_program_tag_is_inert_until_the_guard_runs(self):
        for name, text, runner in self.shells():
            placeholder = "{{{ SCRIPT }}}"
            self.assertEqual(text.count(placeholder), 1, "emcc substitutes exactly one placeholder")
            start = text.index('<template id="loader">')
            end = text.index("</template>", start)
            self.assertIn(placeholder, text[start:end], "the placeholder sits inside the loader template")
            # the guard is the only path to a live program tag: it copies the template's src
            self.assertIn("document.getElementById('loader').content.querySelector('script')", text)
            self.assertLess(text.index("self.crossOriginIsolated"), text.index(f"function {runner}()"),
                            "the isolation probe is decided before the program path")

    def test_the_build_stamp_sits_in_the_head_and_draws_nothing(self):
        # daspkg release wasm replaces the placeholder wherever the shell puts it; the shells put it
        # in a das-build meta tag, so a released page carries its build without showing it
        for name, text, runner in self.shells():
            self.assertEqual(text.count("__DAS_BUILD_STAMP__"), 1, f"{name}: one placeholder")
            head = text[:text.index("<body")]
            self.assertIn('<meta name="das-build" content="__DAS_BUILD_STAMP__">', head, f"{name}: the stamp is a meta tag in the head")
            self.assertNotIn('id="build"', text, f"{name}: no element draws the stamp")

    def test_the_only_live_scripts_are_the_site_files(self):
        for name, text, runner in self.shells():
            parser = MetadataParser()
            scripts = []
            parser.handle_starttag_orig = parser.handle_starttag

            subresources = []

            def handle_starttag(tag, attrs):
                a = dict(attrs)
                if tag == "script" and a.get("src"):
                    scripts.append(a)
                url = a.get("src") if tag in ("script", "img") else a.get("href") if tag == "link" else None
                if url:
                    subresources.append((tag, url, a))
                parser.handle_starttag_orig(tag, attrs)

            parser.handle_starttag = handle_starttag
            parser.feed(text)
            for tag in scripts:
                src = tag["src"]
                self.assertTrue(src.startswith("/files/") or src.startswith("//gc.zgo.at/"),
                                f"a live script tag the guard does not control: {src}")
            for tag, url, a in subresources:
                if url.startswith("//") or "://" in url:
                    # the page is served under COEP require-corp, which silently drops a
                    # cross-origin load that does not ask for CORS
                    self.assertEqual(a.get("crossorigin"), "anonymous",
                                     f"{name}: cross-origin <{tag}> {url} loads without crossorigin")

    def test_the_notes_name_what_the_browser_lacks(self):
        for name, text, runner in self.shells():
            for needle in ("SharedArrayBuffer", "back to the examples", "force=unsupported"):
                self.assertIn(needle, text, f"the shell names {needle!r}")
            for stale in ("memory64", "Safari", "iPhone"):
                self.assertNotIn(stale, text, f"the shell no longer gates on {stale!r}: the lowered build runs on every engine")

    def test_the_model_set_is_read_from_the_manifest_and_version_checked(self):
        for name, text, runner in self.shells():
            with self.subTest(shell=name):
                self.assertEqual(text.count("/* @image-version */ 0"), 1, "exactly one version slot for the deploy to stamp")
                self.assertIn("manifest.json", text, "the file list comes from the deploy's manifest, never a list in the page")
                self.assertIn("m.image_version !== PAGE_IMAGE_VERSION", text, "a set minted for another version is refused before the fetch")
                self.assertIn("onAbort: function (what) { showFailure(", text, "a program abort lands on the page")
                self.assertIn("m.image_version !== PAGE_IMAGE_VERSION) {\n      throw new Error(", text, "the version check throws, it does not log")
                self.assertNotIn("MODEL_FILES", text, "no hard-coded model list survives beside the manifest")


if __name__ == "__main__":
    unittest.main()
