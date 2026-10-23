"""stamp_assets.py over a temp staged tree: python3 -I site-dasllama/test_stamp_assets.py"""
import contextlib
import hashlib
import io
import importlib.util
import re
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parent

# -I keeps the script's own directory off sys.path, so the module loads by its path
_spec = importlib.util.spec_from_file_location("stamp_assets", ROOT / "stamp_assets.py")
stamp_assets = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(stamp_assets)


def sha10(data):
    return hashlib.sha1(data).hexdigest()[:10]


class StampTree(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        files = self.root / "files"
        files.mkdir()
        (files / "x.js").write_bytes(b"console.log('x');\n")
        (files / "y.css").write_bytes(b"body { color: red; }\n")
        (files / "q.js").write_bytes(b"// already versioned\n")
        (self.root / "sub").mkdir()
        (self.root / "index.html").write_text(
            '<script src="files/x.js"></script>\n'
            '<script src="files/missing.js"></script>\n'
            '<script src="files/q.js?v=keep"></script>\n')
        (self.root / "sub" / "page.html").write_text('<link rel="stylesheet" href="../files/y.css">\n')
        (self.root / "doc").mkdir()
        (self.root / "doc" / "files").mkdir()
        (self.root / "doc" / "files" / "x.js").write_bytes(b"doc's own\n")
        (self.root / "doc" / "index.html").write_text('<script src="files/x.js"></script>\n')

    def tearDown(self):
        self.tmp.cleanup()

    def page(self, rel):
        return (self.root / rel).read_text()

    def test_stamps_resolvable_refs_only(self):
        pages, refs = stamp_assets.stamp_tree(str(self.root))
        self.assertEqual((pages, refs), (2, 2))
        index = self.page("index.html")
        x = sha10((self.root / "files" / "x.js").read_bytes())
        self.assertIn(f'src="files/x.js?v={x}"', index)
        self.assertRegex(index, r'src="files/x\.js\?v=[0-9a-f]{10}"')
        self.assertIn('src="files/missing.js"', index, "a ref that resolves to no file is untouched")
        self.assertIn('src="files/q.js?v=keep"', index, "a ref that carries a query is untouched")
        y = sha10((self.root / "files" / "y.css").read_bytes())
        self.assertIn(f'href="../files/y.css?v={y}"', self.page("sub/page.html"))
        self.assertEqual(self.page("doc/index.html"), '<script src="files/x.js"></script>\n',
                         "the doc/ subtree is skipped")

    def test_second_run_stamps_nothing(self):
        stamp_assets.stamp_tree(str(self.root))
        before = self.page("index.html")
        self.assertEqual(stamp_assets.stamp_tree(str(self.root)), (0, 0))
        self.assertEqual(self.page("index.html"), before)

    def test_hash_follows_content(self):
        fresh = '<script src="files/x.js"></script>\n'
        stamp_assets.stamp_tree(str(self.root))
        first = re.search(r"x\.js\?v=([0-9a-f]{10})", self.page("index.html")).group(1)
        (self.root / "files" / "x.js").write_bytes(b"console.log('changed');\n")
        (self.root / "index.html").write_text(fresh)
        stamp_assets.stamp_tree(str(self.root))
        second = re.search(r"x\.js\?v=([0-9a-f]{10})", self.page("index.html")).group(1)
        self.assertNotEqual(first, second)
        self.assertEqual(second, sha10(b"console.log('changed');\n"))

    def test_main_refuses_a_non_dir_root(self):
        with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
            self.assertEqual(stamp_assets.main([str(self.root / "index.html")]), 1)
            self.assertEqual(stamp_assets.main([str(self.root)]), 0)


if __name__ == "__main__":
    unittest.main()
