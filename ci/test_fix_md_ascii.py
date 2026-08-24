#!/usr/bin/env python3
import tempfile
import unittest
from pathlib import Path

import fix_md_ascii


class MarkdownAsciiTest(unittest.TestCase):
    def test_mixed_utf8_and_cp1252_are_repaired_without_mojibake(self):
        raw = (
            b"UTF-8 dash \xe2\x80\x94 CP-1252 dash \x97 "
            b"UTF-8 arrow \xe2\x86\x92 table\n"
        )
        with tempfile.TemporaryDirectory() as temp_dir:
            path = Path(temp_dir) / "mixed.md"
            path.write_bytes(raw)

            repaired, was_broken = fix_md_ascii.read_repaired(path)

        self.assertTrue(was_broken)
        self.assertEqual(
            repaired,
            "UTF-8 dash \N{EM DASH} CP-1252 dash \N{EM DASH} "
            "UTF-8 arrow \N{RIGHTWARDS ARROW} table\n",
        )
        fixed, residual = fix_md_ascii.transliterate(repaired)
        self.assertEqual(residual, set())
        self.assertEqual(
            fixed,
            "UTF-8 dash - CP-1252 dash - UTF-8 arrow -> table\n",
        )


if __name__ == "__main__":
    unittest.main()
