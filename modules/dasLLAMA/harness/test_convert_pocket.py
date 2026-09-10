#!/usr/bin/env python3
"""convert_pocket.py's pure predicates - which tensor lands in which form - with no torch, no
safetensors and no network: the groups a `--fake` spec names, the `--kq` and head rules, the
encoder set `--no-cloning` leaves out. The converted files' contents are the das cells' claim
(tests/test_tts_pocket.das); this file holds the rules that decide them."""
import sys
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

    def test_q8_convs_are_the_dense_forward_stride_one_ones(self):
        self.assertTrue(cp.q8_conv(CODEC_CONV, (512, 512, 3), 1, False))
        self.assertTrue(cp.q8_conv("mimi.quantizer.output_proj.weight", (512, 256, 1), 1, False))
        self.assertFalse(cp.q8_conv(STRIDED, (256, 128, 8), 4, False), "a strided stage")
        self.assertFalse(cp.q8_conv("mimi.decoder.model.5.convtr.convtr.weight", (256, 128, 8), 4, True), "a transposed stage")
        self.assertFalse(cp.q8_conv("mimi.decoder.model.14.conv.weight", (1, 64, 7), 1, False), "the single-channel end")


if __name__ == "__main__":
    unittest.main()
