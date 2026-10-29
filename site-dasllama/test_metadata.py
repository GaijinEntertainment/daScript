import re
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
    "features.html": "https://dasllama.io/features.html",
    "examples.html": "https://dasllama.io/examples.html",
    "articles.html": "https://dasllama.io/articles.html",
    "stories.html": "https://dasllama.io/stories.html",
    "ladder.html": "https://dasllama.io/ladder.html",
    "sidecars.html": "https://dasllama.io/sidecars.html",
}

STORY_PAGES = {
    f"stories/{p.name}": f"https://dasllama.io/stories/{p.name}"
    for p in sorted((ROOT / "stories").glob("*.html"))
}
ARTICLE_PAGES = {
    f"articles/{p.name}": f"https://dasllama.io/articles/{p.name}"
    for p in sorted((ROOT / "articles").glob("*.html"))
}
ALL_PAGES = {**PAGES, **STORY_PAGES, **ARTICLE_PAGES}


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

    def test_there_is_a_page_per_article(self):
        articles = sorted(p.stem for p in (ROOT / "_articles").glob("*.md"))
        pages = sorted(p.stem for p in (ROOT / "articles").glob("*.html"))
        self.assertTrue(articles, "no articles")
        self.assertEqual(pages, articles)
        for page in pages:
            # a signed article names its author on the page, an unsigned one shows no byline; the
            # template's slot is filled, never left raw
            text = (ROOT / "articles" / f"{page}.html").read_text(encoding="utf-8")
            self.assertNotIn("{{author}}", text)
            source = (ROOT / "_articles" / f"{page}.md").read_text(encoding="utf-8")
            if re.search(r"^author:\s*\S", source, re.M):
                self.assertRegex(text, r'<span class="dio-story__author">[^<]+</span>')
            else:
                self.assertNotIn('dio-story__author', text)
            # a figure's "Source:" line is set small: every such paragraph carries the class, none is bare
            self.assertNotRegex(text, r"<p>Source:")
            self.assertEqual(text.count('<p class="dio-story__source">Source:'),
                             len(re.findall(r"^Source:", source, re.M)))

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
        self.assertEqual(sorted(urls[len(PAGES):]), sorted({**STORY_PAGES, **ARTICLE_PAGES}.values()))

    def test_feed_links_to_home_page_anchors(self):
        feed = (ROOT / "feed.xml").read_text(encoding="utf-8")
        self.assertNotIn("https://dasllama.io/index.html", feed)
        self.assertIn("https://dasllama.io/#n-", feed)

    def test_ladder_sections_carry_the_view_mounts(self):
        """Every ladder section - the paired ones (LLM, image chat) and the three audio ones - mounts
        the same bars/table toggle the renderer wires (`wireViews` in files/dasllama-io.js): a view
        segment with the two buttons, a bars box, a table wrap, the table, the caption and the
        filters; the renderer reads them by these ids. The bars are the default view everywhere."""
        page = (ROOT / "ladder.html").read_text(encoding="utf-8")
        script = (ROOT / "files" / "dasllama-io.js").read_text(encoding="utf-8")
        # the paired sections name their ids in PAIR_SECTIONS, and mountPairs wires the toggle from it
        pair = re.compile(r"(\w+): \{ table: '([^']+)', filters: '([^']+)', caption: '([^']+)', view: '([^']+)',\s*"
                          r"metric: '([^']+)', bars: '([^']+)', wrap: '([^']+)'")
        sections = {}
        for m in pair.finditer(script):
            name, table, filters, caption, view, metric, bars, wrap = m.groups()
            sections[name] = {"table": table, "filters": filters, "caption": caption, "view": view,
                              "metric": metric, "bars": bars, "wrap": wrap}
        self.assertEqual(sorted(sections), ["image", "llm"], "PAIR_SECTIONS names the LLM and image-chat sections")
        self.assertIn("mountPairs(PAIR_SECTIONS.llm, llm)", script)
        self.assertIn("mountPairs(PAIR_SECTIONS.image, image)", script)
        self.assertIn("wireViews({ view: sec.view, metric: sec.metric, bars: sec.bars, wrap: sec.wrap, filters: sec.filters }", script)
        # the audio sections' ids are derived from the workload in the renderer
        self.assertIn("wireViews({ view: workload + '-view', bars: workload + '-bars', wrap: workload + '-wrap', filters: workload + '-filters' }", script)
        for w in ("asr", "tts", "audio-chat"):
            sections[w] = {"table": f"{w}-table", "caption": f"{w}-caption", "view": f"{w}-view",
                           "bars": f"{w}-bars", "wrap": f"{w}-wrap", "filters": f"{w}-filters"}
        for name, ids in sections.items():
            with self.subTest(section=name):
                for role, elem in ids.items():
                    self.assertIn(f'id="{elem}"', page, f"{name}: the {role} mount {elem!r} is on the page")
                start = page.index(f'id="{ids["view"]}"')
                seg = page[start:page.index("</div>", start)]
                self.assertIn('class="is-on" data-view="bars"', seg, f"{name}: the bars are the default view")
                self.assertIn('<button data-view="table">', seg, f"{name}: a table button, off")
                self.assertRegex(page, rf'<div class="dl-bars" id="{re.escape(ids["bars"])}">',
                                 f"{name}: the bars box is shown")
                self.assertRegex(page, rf'<div class="dl-table-wrap" id="{re.escape(ids["wrap"])}" hidden>',
                                 f"{name}: the table wrap starts hidden")

    def test_articles_reach_the_index_the_feed_and_the_sitemap(self):
        # every article: an index row under articles/ (naming its author when signed), a feed entry, a lastmod on
        # the index's sitemap row, and section ids on its page (the markdown 'toc' extension)
        index = (ROOT / "articles.html").read_text(encoding="utf-8")
        feed = (ROOT / "feed.xml").read_text(encoding="utf-8")
        sitemap = (ROOT / "sitemap.xml").read_text(encoding="utf-8")
        articles = sorted((ROOT / "_articles").glob("*.md"))
        newest = None
        for md in articles:
            slug = md.stem
            with self.subTest(article=slug):
                text = md.read_text(encoding="utf-8")
                author = re.search(r"^author:\s*(.+)$", text, re.M)
                date = re.search(r"^date:\s*(\S+)$", text, re.M)
                self.assertTrue(date, "front matter names a date")
                newest = max(newest or "", date.group(1))
                row = re.search(rf'<a class="forge-blog-item" href="articles/{re.escape(slug)}\.html">.*?</a>', index, re.S)
                self.assertTrue(row, "the article has an index row under articles/")
                if author:
                    self.assertIn(f" · {author.group(1).strip()}</span>", row.group(0), "the row names the author")
                else:
                    self.assertNotIn(" · ", row.group(0), "an unsigned article's row carries no byline")
                self.assertIn(f"https://dasllama.io/articles/{slug}.html", feed)
                page = (ROOT / "articles" / f"{slug}.html").read_text(encoding="utf-8")
                self.assertRegex(page, r'<h2 id="[a-z0-9-]+">', "section headings carry ids")
        self.assertIn(f"<loc>https://dasllama.io/articles.html</loc><lastmod>{newest}</lastmod>", sitemap)

    def test_serve_maps_the_shared_files_onto_the_daslang_io_tree(self):
        # serve.py serves the files the deploy stages from site/files - flat ones and the one under
        # a subdirectory - from that tree, and everything else under files/ from this one
        import serve
        handler = serve.Handler.__new__(serve.Handler)
        handler.directory = str(ROOT)
        site_files = Path(serve.SITE_FILES)
        self.assertEqual(Path(handler.translate_path("/files/forge.css")), site_files / "forge.css")
        self.assertEqual(Path(handler.translate_path("/files/cm/daslang-keywords.js?v=1")), site_files / "cm" / "daslang-keywords.js")
        self.assertEqual(Path(handler.translate_path("/files/dasllama-io.css")), ROOT / "files" / "dasllama-io.css")

    def test_every_files_reference_on_a_page_is_served(self):
        # a page's files/<path> reference resolves here, or to a file the deploy stages from
        # site/files (the shared list in pages.yml); stamp_assets leaves an unresolvable one as a
        # dead tag, so nothing else catches it
        yml = (REPO_ROOT / ".github" / "workflows" / "pages.yml").read_text(encoding="utf-8")
        shared_line = re.search(r"for f in (forge\.css[^;]*); do", yml)
        self.assertTrue(shared_line, "pages.yml lists the shared files the dasllama.io deploy stages")
        shared = set(shared_line.group(1).split())
        pages = list(ALL_PAGES) + ["_stories/template.html", "_articles/template.html"]
        for filename in pages:
            text = (ROOT / filename).read_text(encoding="utf-8")
            for rel in re.findall(r'(?:src|href)="(?:\.\./|\{\{root\}\})?files/([A-Za-z0-9_./-]+)', text):
                with self.subTest(page=filename, ref=rel):
                    self.assertTrue((ROOT / "files" / rel).exists() or rel in shared,
                                    f"files/{rel} is neither here nor in the deploy's shared list")

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
