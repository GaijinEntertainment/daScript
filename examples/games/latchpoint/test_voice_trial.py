import unittest
from voice_trial import word_error


class WordErrorTests(unittest.TestCase):
    def test_punctuation(self):
        self.assertEqual(word_error("Patch's leg, repaired!", "PATCH’S leg repaired")['wer'], 0)

    def test_substitution_insertion_deletion(self):
        self.assertEqual(word_error('a b c', 'a d c')['errors'], 1)
        self.assertEqual(word_error('a b c', 'a b')['errors'], 1)
        self.assertEqual(word_error('a b', 'a b c')['errors'], 1)

    def test_empty(self):
        self.assertEqual(word_error('', '')['wer'], 0)
        self.assertEqual(word_error('Patch', '')['wer'], 1)


if __name__ == '__main__':
    unittest.main()
