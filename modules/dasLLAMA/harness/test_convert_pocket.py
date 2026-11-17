#!/usr/bin/env python3
"""convert_pocket.py's pure predicates - which tensor lands in which form - with no torch, no
safetensors and no network: the groups a `--fake` spec names, the `--kq` and head rules, the
encoder set `--no-cloning` leaves out, the records of a `tokenizers` file and the files of that
kind it refuses. The converted files' contents are the das cells' claim
(tests/test_tts_pocket.das); this file holds the rules that decide them."""
import json
import os
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import convert_pocket as cp  # noqa: E402

ATTN = "backbone.0.self_attn.in_proj.weight"
FFN = "backbone.3.linear1.weight"
NORM = "backbone.3.norm1.weight"
INPUT = "flow_lm.input_linear.weight"
SPEAKER = "flow_lm.speaker_proj_weight"   # the one matrix the bundle names without the ".weight" suffix
EMBED = "flow_lm.conditioner.embed.weight"
HEAD = "head.blocks.1.mlp.0.weight"
HEAD_IN = "head.input_proj.weight"
CODEC_TF = "mimi.dec_tf.2.self_attn.in_proj.weight"
CODEC_CONV = "mimi.decoder.model.4.conv.weight"
STRIDED = "mimi.encoder.model.3.conv.weight"
ENC_TF = "mimi.enc_tf.0.linear2.weight"
DOWNSAMPLE = "mimi.downsample.conv.conv.weight"
VOICE = "voice_latents.alba"


class FakeGroupTest(unittest.TestCase):
    def test_every_group_is_reached_by_the_tensor_it_names(self):
        self.assertEqual(cp.fake_group(ATTN, (3072, 1024)), "attn")
        self.assertEqual(cp.fake_group(FFN, (4096, 1024)), "ffn")
        self.assertEqual(cp.fake_group(INPUT, (1024, 32)), "input")
        self.assertEqual(cp.fake_group(SPEAKER, (1024, 512)), "speaker")
        self.assertEqual(cp.fake_group(EMBED, (4000, 1024)), "embed")
        self.assertEqual(cp.fake_group(HEAD, (1024, 1024)), "head")
        self.assertEqual(cp.fake_group(CODEC_TF, (1536, 512)), "codec")
        self.assertEqual(cp.fake_group(CODEC_CONV, (512, 512, 3), conv_served=True), "codecconv")
        self.assertEqual(cp.fake_group(STRIDED, (256, 128, 8)), "strided")
        for group in ("attn", "ffn", "input", "speaker", "embed", "head", "codec", "codecconv", "strided"):
            self.assertIn(group, cp.FAKE_GROUPS)

    def test_what_no_group_names(self):
        self.assertIsNone(cp.fake_group(NORM, (1024,)), "a norm is one-dimensional")
        self.assertIsNone(cp.fake_group("backbone.0.self_attn.in_proj.bias", (3072,)), "a bias is not a weight")
        self.assertIsNone(cp.fake_group("backbone.0.norm1.weight", (1024, 1)), "a backbone tensor outside attention and the FFN")
        self.assertIsNone(cp.fake_group(VOICE, (120, 32)), "a voice is never in a group")

    def test_a_spec_names_a_format_per_group(self):
        self.assertEqual(cp.parse_fake("attn=q4_k,head=q8_0"), {"attn": "Q4_K", "head": "Q8_0"})
        self.assertEqual(cp.parse_fake(""), {})
        with self.assertRaises(AssertionError):
            cp.parse_fake("norms=q4_k")


class SmallFormTest(unittest.TestCase):
    def test_kq_takes_the_256_wide_matrices_of_the_backbone_the_codec_transformers_and_the_embedding(self):
        self.assertTrue(cp.kq_tensor(ATTN, (3072, 1024)))
        self.assertTrue(cp.kq_tensor(FFN, (4096, 1024)))
        self.assertTrue(cp.kq_tensor(CODEC_TF, (1536, 512)))
        self.assertTrue(cp.kq_tensor(ENC_TF, (512, 2048)))
        self.assertTrue(cp.kq_tensor(EMBED, (4000, 1024)))

    def test_kq_leaves_the_rest_alone(self):
        self.assertFalse(cp.kq_tensor(HEAD, (1024, 1024)), "the flow head stays Q8_0")
        self.assertFalse(cp.kq_tensor(INPUT, (1024, 32)), "the frame input projection is 32 wide")
        self.assertFalse(cp.kq_tensor(SPEAKER, (1024, 512)), "the speaker projection")
        self.assertFalse(cp.kq_tensor(ATTN, (3072, 1000)), "a width the 256-superblock does not divide")
        self.assertFalse(cp.kq_tensor(CODEC_CONV, (512, 512, 3)), "a conv is three-dimensional")

    def test_the_head_goes_to_q8_on_widths_that_quantize_per_32(self):
        self.assertTrue(cp.head_q8_linear(HEAD, (1024, 1024)))
        self.assertTrue(cp.head_q8_linear(HEAD_IN, (1024, 32)))
        self.assertFalse(cp.head_q8_linear("head.blocks.1.mlp.0.bias", (1024,)))
        self.assertFalse(cp.head_q8_linear(HEAD, (1024, 30)))
        self.assertFalse(cp.head_q8_linear(ATTN, (3072, 1024)), "the backbone is not the head")

    def test_no_cloning_leaves_out_exactly_the_clip_path(self):
        for name in (STRIDED, ENC_TF, DOWNSAMPLE, "mimi.encoder.model.0.conv.weight"):
            self.assertTrue(cp.encoder_tensor(name), name)
        for name in (CODEC_TF, CODEC_CONV, "mimi.upsample.convtr.convtr.weight", "mimi.quantizer.output_proj.weight", ATTN, HEAD, VOICE):
            self.assertFalse(cp.encoder_tensor(name), name)


class PublishedFormTest(unittest.TestCase):
    def test_q8_serves_the_rows_gemms_and_the_frame_input(self):
        self.assertTrue(cp.q8_linear(ATTN, (3072, 1024)))
        self.assertTrue(cp.q8_linear(ENC_TF, (512, 2048)))
        self.assertTrue(cp.q8_linear(INPUT, (1024, 32)))
        self.assertFalse(cp.q8_linear(HEAD, (1024, 1024)), "the head is the --kq form's alone")
        self.assertFalse(cp.q8_linear(SPEAKER, (1024, 512)))
        self.assertFalse(cp.q8_linear(ATTN, (3072, 1000)))

    def test_the_dense_codec_convs_are_the_forward_stride_one_ones(self):
        self.assertTrue(cp.dense_codec_conv(CODEC_CONV, (512, 512, 3), 1, False))
        self.assertTrue(cp.dense_codec_conv("mimi.quantizer.output_proj.weight", (512, 256, 1), 1, False))
        self.assertFalse(cp.dense_codec_conv(STRIDED, (256, 128, 8), 4, False), "a strided stage")
        self.assertFalse(cp.dense_codec_conv("mimi.decoder.model.5.convtr.convtr.weight", (256, 128, 8), 4, True), "a transposed stage")
        self.assertFalse(cp.dense_codec_conv("mimi.decoder.model.14.conv.weight", (1, 64, 7), 1, False), "the single-channel end")



def tokenizers_file(**changes):
    """A `tokenizers` file of the shape the 100M configs pin, with one part swapped per refusal."""
    j = {"model": {"type": "Unigram", "unk_id": 0, "byte_fallback": True,
                   "vocab": [["<unk>", 0.0], ["<s>", 0.0], ["</s>", 0.0], ["<0x41>", -9.0], ["\u2581the", -2.5], ["s", -3.25]]},
         "normalizer": {"type": "Prepend", "prepend": "\u2581"},
         "pre_tokenizer": {"type": "Metaspace", "replacement": "\u2581", "prepend_scheme": "always", "split": True},
         "post_processor": None,
         "added_tokens": [{"id": 0, "content": "<unk>", "special": True}, {"id": 1, "content": "<s>", "special": True},
                          {"id": 2, "content": "</s>", "special": True}]}
    for key, value in changes.items():
        if key == "vocab" or key == "model_type":
            j["model"]["vocab" if key == "vocab" else "type"] = value
        else:
            j[key] = value
    return j


class TokenizerJsonTest(unittest.TestCase):
    def records(self, **changes):
        with tempfile.TemporaryDirectory() as d:
            path = os.path.join(d, "tokenizer.json")
            with open(path, "w", encoding="utf8") as f:
                json.dump(tokenizers_file(**changes), f)
            return cp.tokenizer_json_records(path)

    def test_the_records_are_the_sentencepiece_ones(self):
        pieces, scores, ttype, spec = self.records()
        self.assertEqual(pieces, ["<unk>", "<s>", "</s>", "<0x41>", "\u2581the", "s"])
        self.assertEqual(scores, [0.0, 0.0, 0.0, -9.0, -2.5, -3.25])
        self.assertEqual(ttype, [2, 3, 3, 6, 1, 1])   # unknown, control, control, byte, normal, normal
        self.assertEqual(spec, dict(bos=1, eos=2, unk=0, pad=-1, byte_fallback=True, add_dummy_prefix=True, remove_extra_whitespaces=False))

    def test_a_prepend_inside_a_sequence_reads_the_same(self):
        seq = {"type": "Sequence", "normalizers": [{"type": "Prepend", "prepend": "\u2581"}]}
        self.assertEqual(self.records(normalizer=seq), self.records())

    def test_a_file_the_unigram_path_cannot_serve_is_refused(self):
        refused = [dict(model_type="BPE"),
                   dict(normalizer={"type": "NFKC"}),
                   dict(normalizer={"type": "Sequence", "normalizers": [{"type": "Prepend", "prepend": "\u2581"}, {"type": "Lowercase"}]}),
                   dict(pre_tokenizer={"type": "Metaspace", "replacement": "\u2581", "prepend_scheme": "first", "split": True}),
                   dict(pre_tokenizer={"type": "Metaspace", "replacement": "\u2581", "prepend_scheme": "always", "split": False}),
                   dict(post_processor={"type": "TemplateProcessing"}),
                   dict(normalizer={"type": "Prepend", "prepend": "_"}),
                   dict(pre_tokenizer={"type": "Whitespace"}),
                   dict(pre_tokenizer={"type": "Metaspace", "replacement": "_", "prepend_scheme": "always", "split": True}),
                   dict(vocab=[["<unk>", 0.0], ["\u2581of\u2581the", -4.0]])]
        for change in refused:
            with self.assertRaises(AssertionError, msg=str(change)):
                self.records(**change)

    def test_absent_keys_read_as_the_library_defaults(self):
        split_default = {"type": "Metaspace", "replacement": "\u2581", "prepend_scheme": "always"}
        self.assertEqual(self.records(pre_tokenizer=split_default), self.records())   # `split` absent is true
        plain = tokenizers_file()
        del plain["model"]["byte_fallback"]
        with tempfile.TemporaryDirectory() as d:
            path = os.path.join(d, "tokenizer.json")
            with open(path, "w", encoding="utf8") as f:
                json.dump(plain, f)
            self.assertFalse(cp.tokenizer_json_records(path)[3]["byte_fallback"])   # `byte_fallback` absent is false
        self.assertTrue(self.records()[3]["byte_fallback"])

    def test_the_lookup_table_names_the_reader(self):
        self.assertIs(cp.tokenizer_reader({"tokenizer": "tokenizers"}), cp.tokenizer_json_records)
        self.assertIs(cp.tokenizer_reader({"tokenizer": "sentencepiece"}), cp.tokenizer_records)
        self.assertIs(cp.tokenizer_reader({}), cp.tokenizer_records)
        with self.assertRaises(AssertionError):
            cp.tokenizer_reader({"tokenizer": "wordpiece"})


if __name__ == "__main__":
    unittest.main()
