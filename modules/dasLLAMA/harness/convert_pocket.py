"""Pocket TTS bundle -> dasLLAMA GGUF.

    ~/Work/tts-ab/pocket/.venv-pocket/bin/python convert_pocket.py --language english_2026-04 \\
        --hub ~/Work/tts-ab/pocket/hub --out ~/Work/llama.cpp/models [--llama-cpp ~/Work/llama.cpp]

Reads one language's pinned bundle (`<hub>/pocket-tts/languages/<language>/model.safetensors`, the
gated repo with the Mimi encoder populated), its tokenizer
(`<hub>/pocket-tts-without-voice-cloning/languages/<language>/tokenizer.model`), the config the
package ships for that language, and the bundled voice clips, and writes
`pocket-tts-<language>.gguf` under `general.architecture = "pocket-tts"`:

- every weight as f16 (the bundles are bf16; f16 keeps 3 more mantissa bits - the loss is the
  bf16 file's own), under the CANONICAL names below (GGML caps a name at 63 bytes; 16 of the
  bundle's names are longer), [out, in] like PyTorch;
- the unigram SentencePiece tokenizer as `tokenizer.ggml.model = "t5"` (upstream's name for a
  unigram model) with `tokenizer.ggml.tokens` / `scores` / `token_type` and the special ids;
- the model's scalars as `pocket.*` metadata (from the config, not guessed);
- each bundled voice as `voice_latents.<name>` [frames][latent_dim] f32 - the clip's frames
  through the package's own codec encoder, the form a voice state is built from (the reader
  also takes the older `voice.<name>` PCM form, which needs the encoder); `--voices` picks the
  roster, the CC BY-NC clips of the package's English roster are never in it (the sidecar names
  every clip's source and licence); `--no-cloning` leaves the codec encoder out and says so in
  `pocket.cloning`, so the file serves its roster and refuses to clone.

Canonical names: `flow_lm.transformer.layers.N.*` -> `backbone.N.*`; `flow_lm.flow_net.*` ->
`head.*`; `mimi.encoder_transformer.transformer.layers.N.*` -> `mimi.enc_tf.N.*`, the decoder
twin `mimi.dec_tf.N.*`; every other `flow_lm.` / `mimi.` name kept as is.

`--q8` writes the published form: every GEMM weight the engine serves as Q8_0 quants is stored
as Q8_0 in the layout the kernels read - a linear as [nout][nin] with the 32-blocks along nin,
a dense stride-1 conv on 32-wide channels as the tap-stacked slab [cout][k][cin] (the f16 form
keeps PyTorch's [cout][cin][k]) - and the reader takes the blocks straight into its int8 plane.
The rest of the file is unchanged. The engine's eligibility rule (`conv1d_q8_eligible`,
`linear_prepare`) is mirrored here in `q8_linear` / `q8_conv`; `tests/test_tts_pocket.das`
holds the two files to each other.
"""
import argparse
import json
import os
import struct
import sys

import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
ARCH = "pocket-tts"
GGML_MAX_NAME = 64
SAMPLE_RATE = 24000

# the package's predefined voices (pocket_tts/utils/utils.py) with their tts-voices folder and
# licence; a clip under a CC BY-NC folder is not shipped
ENGLISH_VOICES = {
    "alba": ("alba-mackenna/casual.wav", "CC BY 4.0 (Alba Mackenna)"),
    "marius": ("voice-donations/Selfie.wav", "CC0 (Unmute voice donation)"),
    "javert": ("voice-donations/Butter.wav", "CC0 (Unmute voice donation)"),
    "anna": ("vctk/p228_023_enhanced.wav", "CC BY 4.0 (VCTK, CSTR University of Edinburgh)"),
    "vera": ("vctk/p229_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "fantine": ("vctk/p244_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "charles": ("vctk/p254_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "paul": ("vctk/p259_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "eponine": ("vctk/p262_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "azelma": ("vctk/p303_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "george": ("vctk/p315_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "mary": ("vctk/p333_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "jane": ("vctk/p339_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "michael": ("vctk/p360_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "eve": ("vctk/p361_023_enhanced.wav", "CC BY 4.0 (VCTK)"),
    "bill_boerst": ("voice-zero/bill_boerst.wav", "CC0 (LibriVox, voice-zero)"),
    "peter_yearsley": ("voice-zero/peter_yearsley.wav", "CC0 (LibriVox, voice-zero)"),
    "stuart_bell": ("voice-zero/stuart_bell.wav", "CC0 (LibriVox, voice-zero)"),
    "caro_davy": ("voice-zero/caro_davy.wav", "CC0 (LibriVox, voice-zero)"),
}
EXCLUDED_VOICES = {"cosette": "expresso (CC BY-NC 4.0)", "jean": "ears (CC BY-NC 4.0)"}
# the non-English defaults live in the kyutai/pocket-tts repo itself (`<hub>/pocket-tts/<file>`)
LANGUAGE_VOICES = {
    "italian": {"giovanni": ("common_voice_it_36520747-enhanced-v2.mp3", "CC0 (Common Voice)")},
    "spanish": {"lola": ("common_voice_es_19762977-enhanced-v2.mp3", "CC0 (Common Voice)")},
    "german": {"juergen": ("de-DE-juergen.mp3", "kyutai/pocket-tts repo clip")},
    "portuguese": {"rafael": ("g-Vi8PgmSY0-enhanced-v2.wav", "kyutai/pocket-tts repo clip")},
    "french_24l": {"estelle": ("tts-voices:unmute-prod-website/developpeuse-3.wav", "CC0 (unmute-prod-website)")},
}
DEFAULT_VOICE = {"english_2026-04": "alba", "english": "alba", "italian": "giovanni", "spanish": "lola",
                 "german": "juergen", "portuguese": "rafael", "french_24l": "estelle"}
DEFAULT_TEMPERATURE = 0.7   # pocket_tts/default_parameters.py when the config carries none


def q8_linear(name, shape):
    """The linears the engine serves through the rows GEMM as Q8_0: the transformer layers'
    four matrices and the frame input projection, on widths that quantize per 32. The head's
    GEMVs, the EOS head, the speaker projection and the norms stay f16."""
    rows_served = (".self_attn." in name or ".linear1." in name or ".linear2." in name) and name.endswith(".weight") \
        and (name.startswith("backbone.") or name.startswith("mimi.enc_tf.") or name.startswith("mimi.dec_tf."))
    rows_served = rows_served or name == "flow_lm.input_linear.weight"
    return rows_served and len(shape) == 2 and shape[0] % 32 == 0 and shape[1] % 32 == 0


def q8_conv(name, shape, stride, transposed):
    """The convs the engine serves q8: dense, forward, stride 1, both channel counts on 32 -
    every codec conv (the latent projection included) but the strided encoder stages and the
    downsampler, the transposed decoder stages, the depthwise resampler and the two
    single-channel ends."""
    is_conv = name.startswith("mimi.") and (name.endswith(".conv.weight") or name == "mimi.quantizer.output_proj.weight")
    return is_conv and len(shape) == 3 and not transposed and stride == 1 and shape[0] % 32 == 0 and shape[1] % 32 == 0


FAKE_GROUPS = ("attn", "ffn", "input", "speaker", "embed", "head", "codec", "codecconv", "strided")


def fake_group(name, shape, conv_served=False):
    """The tensor group a `--fake` spec names: the backbone's attention projections, its two FFN
    matrices, the frame input projection, the speaker projection, the text embedding table, the
    flow head's matrices, the codec transformers' GEMMs (`codec`), the convs the engine serves
    q8 (`codecconv`), and the codec's strided, transposed and resampling convs the file keeps
    f16 (`strided`). Norms, biases and the voices are never in a group."""
    if not name.endswith(".weight") or len(shape) < 2:
        return None
    if name.startswith("backbone."):
        if ".self_attn." in name:
            return "attn"
        if ".linear1." in name or ".linear2." in name:
            return "ffn"
        return None
    if name == "flow_lm.input_linear.weight":
        return "input"
    if name == "flow_lm.speaker_proj.weight":
        return "speaker"
    if name == "flow_lm.conditioner.embed.weight":
        return "embed"
    if name.startswith("head."):
        return "head"
    if name.startswith("mimi."):
        if len(shape) == 3:
            return "codecconv" if conv_served else "strided"
        return "codec"
    return None


def parse_fake(spec):
    """`group=fmt,group=fmt` - a format per group, ggml's names (q4_0, q4_k, q6_k, iq4_nl, ...)."""
    out = {}
    for item in filter(None, spec.split(",")):
        group, fmt = item.split("=")
        assert group in FAKE_GROUPS, (group, FAKE_GROUPS)
        out[group] = fmt.upper()
    return out


class FakeQuant:
    """Round a float matrix through a ggml quant format and back: ggml's own quantizer (the
    built llama.cpp's libggml-base) writes the blocks, gguf-py reads them back to f32. The rows
    then store in the file's usual form, so the engine's lanes measure the format's loss with no
    new kernel; a width the format's block does not divide is left as it is and reported."""

    def __init__(self, llama_cpp, gguf_mod):
        import ctypes
        import glob
        libs = glob.glob(os.path.join(llama_cpp, "build", "bin", "libggml-base.dylib")) + \
            glob.glob(os.path.join(llama_cpp, "build", "bin", "libggml-base.so"))
        assert libs, "no built libggml-base beside llama.cpp/build/bin - build llama.cpp first"
        self.lib = ctypes.CDLL(libs[0])
        self.lib.ggml_quantize_chunk.restype = ctypes.c_size_t
        self.lib.ggml_quantize_chunk.argtypes = [ctypes.c_int, ctypes.POINTER(ctypes.c_float), ctypes.c_void_p,
                                                 ctypes.c_int64, ctypes.c_int64, ctypes.c_int64, ctypes.POINTER(ctypes.c_float)]
        self.ctypes = ctypes
        self.gguf = gguf_mod
        self.skipped = []
        self.done = {}

    def quantize_bytes(self, name, rows, fmt):
        """`rows` [nrows][n_per_row] f32 -> the format's blocks, [nrows][bytes per row] u8 (None
        where the width is not a whole number of blocks)."""
        t = self.gguf.GGMLQuantizationType[fmt]
        block, type_size = self.gguf.GGML_QUANT_SIZES[t]
        nrows, n_per_row = rows.shape
        if n_per_row % block != 0:
            self.skipped.append((name, fmt, n_per_row))
            return None
        out = np.empty((nrows, (n_per_row // block) * type_size), dtype=np.uint8)
        c = self.ctypes
        n = self.lib.ggml_quantize_chunk(int(t), rows.ctypes.data_as(c.POINTER(c.c_float)), out.ctypes.data_as(c.c_void_p),
                                         0, nrows, n_per_row, None)
        assert n == out.nbytes, (name, fmt, n, out.nbytes)
        return out

    def apply(self, name, w32, fmt):
        from gguf.quants import dequantize
        t = self.gguf.GGMLQuantizationType[fmt]
        # a conv [cout][cin][k] rounds per output channel over its cin*k taps; a matrix per row
        rows = np.ascontiguousarray(w32.reshape(w32.shape[0], -1) if w32.ndim == 3 else w32.reshape(-1, w32.shape[-1]), dtype=np.float32)
        out = self.quantize_bytes(name, rows, fmt)
        if out is None:
            return w32
        back = dequantize(out, t).reshape(w32.shape).astype(np.float32)
        err = float(np.sqrt(((back - w32) ** 2).mean()) / max(np.sqrt((w32 ** 2).mean()), 1e-12))
        self.done[name] = (fmt, out.nbytes, err)
        return back


def encoder_tensor(name):
    """The codec encoder: the SEANet stages, the encoder transformer and the frame downsampler -
    everything a clip goes through on its way to latents, and nothing a synthesis reads."""
    return name.startswith("mimi.encoder.") or name.startswith("mimi.enc_tf.") or name.startswith("mimi.downsample.")


def clip_encoder(language):
    """The package's own codec encoder over a 24 kHz clip -> its latent frames [frames][latent_dim],
    the form the roster is stored in: a voice is the backbone's memory of those frames, so the
    file needs no clip samples and, with --no-cloning, no encoder."""
    import torch
    from pocket_tts import TTSModel
    model = TTSModel.load_model(language=language)

    def encode(pcm):
        with torch.no_grad():
            lat = model.mimi.encode_to_latent(torch.from_numpy(np.ascontiguousarray(pcm, dtype=np.float32))[None, None])
        return np.ascontiguousarray(lat[0].numpy(), dtype=np.float32)
    return encode


def kq_tensor(name, shape):
    """The tensors `--kq` stores as Q4_K: the backbone's and the codec transformers' matrices and
    the text embedding table - every one 256-wide along its rows, the K-quant rule."""
    return fake_group(name, shape) in ("attn", "ffn", "codec", "embed") and len(shape) == 2 and shape[1] % 256 == 0


def head_q8_linear(name, shape):
    """The flow head's matrices, which `--kq` stores as Q8_0: the vector layers' GEMVs run on the
    q8 lane where the file holds the blocks."""
    return name.startswith("head.") and name.endswith(".weight") and len(shape) == 2 and shape[0] % 32 == 0 and shape[1] % 32 == 0


def canonical(name):
    if name.startswith("flow_lm.transformer.layers."):
        return "backbone." + name[len("flow_lm.transformer.layers."):]
    if name.startswith("flow_lm.flow_net."):
        return "head." + name[len("flow_lm.flow_net."):]
    for src, dst in (("mimi.encoder_transformer.transformer.layers.", "mimi.enc_tf."),
                     ("mimi.decoder_transformer.transformer.layers.", "mimi.dec_tf.")):
        if name.startswith(src):
            return dst + name[len(src):]
    return name


def read_safetensors(path):
    with open(path, "rb") as f:
        n = struct.unpack("<Q", f.read(8))[0]
        hdr = json.loads(f.read(n))
        base = 8 + n
        out = {}
        for k, v in hdr.items():
            if k == "__metadata__":
                continue
            assert v["dtype"] == "BF16", (k, v["dtype"])
            a, b = v["data_offsets"]
            f.seek(base + a)
            raw = np.frombuffer(f.read(b - a), dtype=np.uint16)
            out[k] = (raw.astype(np.uint32) << 16).view(np.float32).reshape(v["shape"])
        return out


def load_config(language):
    import yaml
    from pocket_tts.utils.config import CONFIGS_DIR
    return yaml.safe_load(open(os.path.join(CONFIGS_DIR, f"{language}.yaml"), encoding="utf8"))


def read_clip(path):
    """A voice clip as f32 mono at 24 kHz, the package's own read + resample path."""
    import torch
    from pocket_tts.data.audio import audio_read
    from pocket_tts.data.audio_utils import convert_audio
    audio, sr = audio_read(path)
    audio = convert_audio(audio, sr, SAMPLE_RATE, 1)
    return np.ascontiguousarray(audio[0].numpy().astype(np.float32))


def tokenizer_records(path):
    from sentencepiece import sentencepiece_model_pb2 as pb
    m = pb.ModelProto()
    m.ParseFromString(open(path, "rb").read())
    assert m.trainer_spec.model_type == 1, "not a unigram model"
    pieces = [p.piece for p in m.pieces]
    scores = [float(p.score) for p in m.pieces]
    # gguf token_type: NORMAL 1, UNKNOWN 2, CONTROL 3, USER_DEFINED 4, UNUSED 5, BYTE 6
    types = {1: 1, 2: 2, 3: 3, 4: 4, 5: 5, 6: 6}
    ttype = [types[p.type] for p in m.pieces]
    spec = dict(bos=m.trainer_spec.bos_id, eos=m.trainer_spec.eos_id, unk=m.trainer_spec.unk_id, pad=m.trainer_spec.pad_id,
                byte_fallback=bool(m.trainer_spec.byte_fallback), add_dummy_prefix=bool(m.normalizer_spec.add_dummy_prefix),
                remove_extra_whitespaces=bool(m.normalizer_spec.remove_extra_whitespaces))
    return pieces, scores, ttype, spec


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--language", default="english_2026-04")
    ap.add_argument("--hub", default=os.path.expanduser("~/Work/tts-ab/pocket/hub"))
    ap.add_argument("--out", required=True, help="directory for pocket-tts-<language>.gguf")
    ap.add_argument("--llama-cpp", default=os.path.expanduser("~/Work/llama.cpp"), help="for gguf-py")
    ap.add_argument("--name", default=None, help="output file stem (default pocket-tts-<language>)")
    ap.add_argument("--q8", action="store_true", help="the published form: the served GEMM weights as Q8_0 in the kernels' layout")
    ap.add_argument("--fake", default="", help="quality experiment, never published: group=fmt[,group=fmt] (attn, ffn, input, speaker, head, "
                    "codec; ggml format names) - the group's weights round through that format before they are stored; needs --name")
    ap.add_argument("--kq", action="store_true", help="the small form: the backbone's and the codec transformers' matrices and the text embedding "
                    "as Q4_K, the flow head as Q8_0, the rest as --q8 writes it (implies --q8)")
    ap.add_argument("--voices", default="", help="the roster as a comma list of the language's voice names (default: every voice the language ships)")
    ap.add_argument("--no-cloning", action="store_true", help="leave the codec encoder out: the roster speaks from its stored latents, "
                    "tts_register_voice refuses, and the file is smaller by the encoder")
    a = ap.parse_args()
    sys.path.insert(0, os.path.join(a.llama_cpp, "gguf-py"))
    import gguf
    from gguf.quants import quantize as gguf_quantize
    fake = parse_fake(a.fake)
    assert not fake or a.name, "--fake files are local experiments: give them a --name"
    if a.kq:
        a.q8 = True
    fq = FakeQuant(a.llama_cpp, gguf) if (fake or a.kq) else None

    lang = a.language
    cfg = load_config(lang)
    weights_rev = cfg["weights_path"].split("@")[1]
    tok_rev = cfg["flow_lm"]["lookup_table"]["tokenizer_path"].split("@")[1]
    st = read_safetensors(os.path.join(a.hub, "pocket-tts", "languages", lang, "model.safetensors"))
    enc0 = st["mimi.encoder.model.0.conv.weight"]
    assert np.abs(enc0).max() > 0, "the Mimi encoder is zero: this is the no-cloning bundle, not the gated one"
    pieces, scores, ttype, spec = tokenizer_records(os.path.join(a.hub, "pocket-tts-without-voice-cloning", "languages", lang, "tokenizer.model"))
    assert len(pieces) == cfg["flow_lm"]["lookup_table"]["n_bins"], (len(pieces), cfg["flow_lm"]["lookup_table"]["n_bins"])

    # the codec's strided stages carry the ratios as their strides; a transposed conv's name says so
    ratios = [int(r) for r in cfg["mimi"]["seanet"]["ratios"]]
    conv_stride = {}
    for i, r in enumerate(reversed(ratios)):
        conv_stride[f"mimi.encoder.model.{3 + 3 * i}.conv.weight"] = r
    conv_stride["mimi.downsample.conv.conv.weight"] = st["mimi.downsample.conv.conv.weight"].shape[2] // 2   # kernel 2 x stride
    tensors = {}
    quantized = []
    quantized_k4 = []
    dropped = []
    for k, v in st.items():
        name = canonical(k)
        assert len(name) < GGML_MAX_NAME, name
        assert name not in tensors, name
        if a.no_cloning and encoder_tensor(name):
            dropped.append(name)
            continue
        group = fake_group(name, v.shape, q8_conv(name, v.shape, conv_stride.get(name, 1), ".convtr." in name)) if fake else None
        if group in fake:
            v = fq.apply(name, np.ascontiguousarray(v.astype(np.float32)), fake[group])
        if a.kq and kq_tensor(name, v.shape):
            tensors[name] = ("k4", np.ascontiguousarray(v.astype(np.float32)))
            quantized_k4.append(name)
        elif a.q8 and (q8_linear(name, v.shape) or (a.kq and head_q8_linear(name, v.shape))):
            tensors[name] = ("q8", np.ascontiguousarray(v.astype(np.float32)))
            quantized.append(name)
        elif a.q8 and q8_conv(name, v.shape, conv_stride.get(name, 1), ".convtr." in name):
            slab = np.ascontiguousarray(np.transpose(v, (0, 2, 1)).astype(np.float32))   # [cout][cin][k] -> [cout][k][cin]
            tensors[name] = ("q8", slab)
            quantized.append(name)
        else:
            tensors[name] = np.ascontiguousarray(v.astype(np.float16))

    voices = {}
    sources = {}
    roster = dict(ENGLISH_VOICES) if lang.startswith("english") else LANGUAGE_VOICES[lang]
    if a.voices:
        picked = [v.strip() for v in a.voices.split(",") if v.strip()]
        unknown = [v for v in picked if v not in roster]
        assert not unknown, f"--voices names {unknown}; the {lang} roster is {sorted(roster)}"
        roster = {v: roster[v] for v in picked}
    encoder = clip_encoder(lang)
    for vname, (rel, licence) in roster.items():
        if rel.startswith("tts-voices:"):
            path = os.path.join(a.hub, "tts-voices", rel[len("tts-voices:"):])
        elif lang.startswith("english"):
            path = os.path.join(a.hub, "tts-voices", rel)
        else:
            path = os.path.join(a.hub, "pocket-tts", rel)
        if not os.path.exists(path):
            print(f"  voice {vname}: {path} missing - skipped", flush=True)
            continue
        pcm = read_clip(path)
        latents = encoder(pcm)
        voices[vname] = latents
        sources[vname] = (rel, licence, len(pcm))
        tensors["voice_latents." + vname] = latents
    default_voice = DEFAULT_VOICE.get(lang, next(iter(voices)))
    if default_voice not in voices:
        default_voice = next(iter(voices))
    assert default_voice in voices, (default_voice, list(voices))

    fl = cfg["flow_lm"]
    mm = cfg["mimi"]
    stem = a.name or (f"pocket-tts-{lang}-q8" if a.q8 else f"pocket-tts-{lang}")
    path = os.path.join(a.out, stem + ".gguf")
    w = gguf.GGUFWriter(path, ARCH)
    w.add_name(f"Pocket TTS {lang}" + (" Q8_0" if a.q8 else ""))
    w.add_string("pocket.weights", "q8" if a.q8 else "f16")
    if a.kq:
        w.add_string("pocket.kq", "q4_k")
    w.add_bool("pocket.cloning", not a.no_cloning)
    if fake:
        w.add_string("pocket.fake", a.fake)
    w.add_string("pocket.language", lang)
    w.add_string("pocket.revision", weights_rev)
    w.add_string("pocket.tokenizer_revision", tok_rev)
    w.add_string("pocket.source", f"kyutai/pocket-tts languages/{lang}/model.safetensors")
    w.add_float32("pocket.default_temperature", float(cfg.get("default_temperature", DEFAULT_TEMPERATURE)))
    # the package's per-config text rules (pocket_tts/utils/config.py Config defaults)
    w.add_bool("pocket.pad_short_inputs", bool(cfg.get("pad_with_spaces_for_short_inputs", False)))
    w.add_bool("pocket.remove_semicolons", bool(cfg.get("remove_semicolons", False)))
    w.add_bool("pocket.append_terminal_punctuation", bool(cfg.get("append_terminal_punctuation", True)))
    w.add_int32("pocket.frames_after_eos", int(cfg.get("model_recommended_frames_after_eos") or -1))
    w.add_uint32("pocket.sample_rate", int(mm["sample_rate"]))
    w.add_float32("pocket.frame_rate", float(mm["frame_rate"]))
    w.add_uint32("pocket.latent_dim", int(mm["quantizer"]["dimension"]))
    w.add_uint32("pocket.backbone.layers", int(fl["transformer"]["num_layers"]))
    w.add_uint32("pocket.backbone.d_model", int(fl["transformer"]["d_model"]))
    w.add_uint32("pocket.backbone.heads", int(fl["transformer"]["num_heads"]))
    w.add_uint32("pocket.backbone.hidden_scale", int(fl["transformer"]["hidden_scale"]))
    w.add_float32("pocket.backbone.rope_max_period", float(fl["transformer"]["max_period"]))
    w.add_bool("pocket.bos_before_voice", bool(fl["insert_bos_before_voice"]))
    w.add_uint32("pocket.head.depth", int(fl["flow"]["depth"]))
    w.add_uint32("pocket.head.dim", int(fl["flow"]["dim"]))
    w.add_string("pocket.head.flow_type", str(fl["flow"].get("type", "lsd")))
    w.add_uint32("pocket.mimi.d_model", int(mm["transformer"]["d_model"]))
    w.add_uint32("pocket.mimi.heads", int(mm["transformer"]["num_heads"]))
    w.add_uint32("pocket.mimi.layers", int(mm["transformer"]["num_layers"]))
    w.add_uint32("pocket.mimi.context", int(mm["transformer"]["context"]))
    w.add_uint32("pocket.mimi.dim_feedforward", int(mm["transformer"]["dim_feedforward"]))
    w.add_float32("pocket.mimi.layer_scale", float(mm["transformer"]["layer_scale"]))
    w.add_float32("pocket.mimi.rope_max_period", float(mm["transformer"].get("max_period", 10000.0)))
    w.add_uint32("pocket.mimi.n_filters", int(mm["seanet"]["n_filters"]))
    w.add_array("pocket.mimi.ratios", [int(r) for r in mm["seanet"]["ratios"]])
    w.add_uint32("pocket.mimi.kernel_size", int(mm["seanet"]["kernel_size"]))
    w.add_uint32("pocket.mimi.residual_kernel_size", int(mm["seanet"]["residual_kernel_size"]))
    w.add_uint32("pocket.mimi.last_kernel_size", int(mm["seanet"]["last_kernel_size"]))
    w.add_uint32("pocket.mimi.compress", int(mm["seanet"]["compress"]))
    w.add_string("pocket.default_voice", default_voice)
    w.add_array("pocket.voices", list(voices.keys()))
    w.add_tokenizer_model("t5")
    w.add_token_list(pieces)
    w.add_token_scores(scores)
    w.add_token_types(ttype)
    w.add_bos_token_id(spec["bos"])
    w.add_eos_token_id(spec["eos"])
    w.add_unk_token_id(spec["unk"])
    w.add_pad_token_id(spec["pad"])
    w.add_add_space_prefix(spec["add_dummy_prefix"])
    w.add_remove_extra_whitespaces(spec["remove_extra_whitespaces"])
    w.add_bool("tokenizer.ggml.byte_fallback", spec["byte_fallback"])
    for name in sorted(tensors):
        t = tensors[name]
        if isinstance(t, tuple) and t[0] == "k4":
            data = fq.quantize_bytes(name, t[1], "Q4_K")   # ggml's own quantizer; the writer derives the element shape from the byte shape
            w.add_tensor(name, data, raw_dtype=gguf.GGMLQuantizationType.Q4_K)
        elif isinstance(t, tuple):
            data = gguf_quantize(t[1], gguf.GGMLQuantizationType.Q8_0)   # the writer derives the element shape from the byte shape
            w.add_tensor(name, data, raw_dtype=gguf.GGMLQuantizationType.Q8_0)
        else:
            w.add_tensor(name, t)
    w.write_header_to_file()
    w.write_kv_data_to_file()
    w.write_tensors_to_file()
    w.close()
    with open(path + ".LICENSE", "w", encoding="utf8") as f:
        f.write(f"{stem}.gguf - Kyutai Pocket TTS ({lang}) weights{' (the backbone and codec transformers as Q4_K, the head as Q8_0)' if a.kq else (' (the served GEMMs as Q8_0)' if a.q8 else '')}"
                f"{' without the codec encoder' if a.no_cloning else ''}, CC BY 4.0 (Kyutai), converted from kyutai/pocket-tts "
                f"languages/{lang}/model.safetensors @ {weights_rev} and the unigram SentencePiece tokenizer @ {tok_rev} by "
                "modules/dasLLAMA/harness/convert_pocket.py; the reference implementation is MIT (github.com/kyutai-labs/pocket-tts). "
                "Bundled voices (each the codec encoder's latent frames of the clip named):\n")
        for vname, (rel, licence, n) in sources.items():
            f.write(f"  voice_latents.{vname}: {rel} ({n / SAMPLE_RATE:.1f} s) - {licence}\n")
        for vname, why in EXCLUDED_VOICES.items():
            if lang.startswith("english"):
                f.write(f"  not shipped: {vname} - {why}\n")
        f.write("see the dasLLAMA THIRD_PARTY_NOTICES.md\n")
    print(f"wrote {path}: {len(tensors)} tensors ({len(quantized)} as Q8_0{f', {len(quantized_k4)} as Q4_K' if quantized_k4 else ''}"
          f"{f', {len(dropped)} encoder tensors left out' if dropped else ''}), "
          f"{len(pieces)} pieces, {len(voices)} voices (default {default_voice}); {os.path.getsize(path)} bytes on disk")
    if fq:
        by_group = {}
        for name, (fmt, nbytes, err) in fq.done.items():
            g = by_group.setdefault((fake_group(name, (1, 1)) or "strided", fmt), [0, 0, 0.0])
            g[0] += 1
            g[1] += nbytes
            g[2] = max(g[2], err)
        for (group, fmt), (n, nbytes, err) in sorted(by_group.items()):
            print(f"  fake {group}={fmt}: {n} tensors, {nbytes / 1e6:.1f} MB as {fmt}, worst rms rel err {err:.4f}")
        for name, fmt, width in fq.skipped:
            print(f"  fake skipped {name}: width {width} is not a whole number of {fmt} blocks")


if __name__ == "__main__":
    main()
