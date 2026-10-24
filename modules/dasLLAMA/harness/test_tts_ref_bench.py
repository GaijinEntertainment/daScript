#!/usr/bin/env python3
"""tts_ref_bench.py's pure voice rule - which pack voice and speed prior a Kitten voice name
selects - with no onnxruntime, no mlx and no network. The table is the shape of the pack's
`config.json` (`voice_aliases`, `speed_priors`), which the GGUF carries for the das side; a pair on
the TTS board compares the two engines on the same voice only while both resolve a name through it."""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import tts_ref_bench as rb  # noqa: E402

ALIASES = {"Bella": "expr-voice-2-f", "Jasper": "expr-voice-2-m", "Luna": "expr-voice-3-f", "Bruno": "expr-voice-3-m",
           "Rosie": "expr-voice-4-f", "Hugo": "expr-voice-4-m", "Kiki": "expr-voice-5-f", "Leo": "expr-voice-5-m"}
PRIORS = {v: 0.8 for v in ALIASES.values()}


class KittenVoiceTest(unittest.TestCase):
    def test_an_alias_resolves_through_the_pack_table(self):
        self.assertEqual(rb.kitten_voice(ALIASES, PRIORS, "Bella"), ("expr-voice-2-f", 0.8))
        self.assertEqual(rb.kitten_voice(ALIASES, PRIORS, "Kiki"), ("expr-voice-5-f", 0.8))
        self.assertNotEqual(rb.kitten_voice(ALIASES, PRIORS, "Bella")[0], rb.kitten_voice(ALIASES, PRIORS, "Kiki")[0])

    def test_a_pack_voice_names_itself(self):
        self.assertEqual(rb.kitten_voice(ALIASES, PRIORS, "expr-voice-5-f"), ("expr-voice-5-f", 0.8))

    def test_a_name_the_pack_lacks_passes_through_at_unit_speed(self):
        self.assertEqual(rb.kitten_voice(ALIASES, PRIORS, "af_heart"), ("af_heart", 1.0))
        self.assertEqual(rb.kitten_voice({}, {}, "Bella"), ("Bella", 1.0))


if __name__ == "__main__":
    unittest.main(verbosity=1)
