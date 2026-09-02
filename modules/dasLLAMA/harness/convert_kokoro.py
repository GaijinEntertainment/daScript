"""Kokoro-82M checkpoint -> dasLLAMA GGUF.

    python convert_kokoro.py --out ~/Work/llama.cpp/models [--llama-cpp ~/Work/llama.cpp]

Reads the pinned snapshot (kitten_graph.KOKORO_SNAPSHOT: config.json, kokoro-v1_0.pth, the
54 voice packs), builds the reference model to fold weight-norm and read every convolution's
geometry from the module itself, and writes `kokoro-82m.gguf` under the same tensor names and
`styletts2.*` metadata the KittenTTS converter emits, so one loader serves both:

- weights f32, [out, in] like PyTorch; LSTM directions stacked [2, 4H, ...] in PyTorch's i,f,g,o
  gate order with the two bias halves summed; the ALBERT layer group as `bert.layer.*`;
- the STFT basis torch.stft / torch.istft implies (Hann 20, hop 5): forward cos / -sin windowed
  rows, inverse rows scaled by 1/N with the one-sided doubling, plus the window itself for the
  envelope normalization; `styletts2.stft_pad = reflect`, `styletts2.istft_norm = envelope`;
- every voice pack as `voice.<name>` [510, 256]; the vocabulary as `kokoro.symbol_cpts/ids`.
"""
import argparse
import glob
import json
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import kitten_graph as kg  # noqa: E402

ARCH = "kokoro"
GGML_MAX_NAME = 64
ALBERT_PREFIX = "bert.encoder.albert_layer_groups.0.albert_layers.0."
N_FFT, HOP = 20, 5


def fold_weight_norm(model):
    import torch
    for name, mod in list(model.named_modules()):
        if hasattr(mod, "weight_g") and hasattr(mod, "weight_v"):
            torch.nn.utils.remove_weight_norm(mod)


def canonical(name):
    if name.startswith(ALBERT_PREFIX):
        return "bert.layer." + name[len(ALBERT_PREFIX):]
    return name


def lstm_tensors(prefix, sd):
    """PyTorch's per-direction LSTM parameters -> the das layout."""
    import torch
    out = {}
    w_ih = torch.stack([sd[prefix + ".weight_ih_l0"], sd[prefix + ".weight_ih_l0_reverse"]])
    w_hh = torch.stack([sd[prefix + ".weight_hh_l0"], sd[prefix + ".weight_hh_l0_reverse"]])
    b = torch.stack([sd[prefix + ".bias_ih_l0"] + sd[prefix + ".bias_hh_l0"],
                     sd[prefix + ".bias_ih_l0_reverse"] + sd[prefix + ".bias_hh_l0_reverse"]])
    out[prefix + ".weight_ih"] = w_ih
    out[prefix + ".weight_hh"] = w_hh
    out[prefix + ".bias"] = b
    return out


def stft_basis():
    """torch.stft / torch.istft with a periodic Hann window as conv bases (freq_bins, 1, n_fft)."""
    n = np.arange(N_FFT)
    k = np.arange(N_FFT // 2 + 1)
    window = 0.5 - 0.5 * np.cos(2 * np.pi * n / N_FFT)   # torch.hann_window(periodic=True)
    angle = 2 * np.pi * np.outer(k, n) / N_FFT
    cos_a, sin_a = np.cos(angle), np.sin(angle)
    # the DC and Nyquist rows are exactly +-1 / 0 in an FFT; a rounded sin(pi n) here would let the
    # das conv land on the other side of atan2's branch cut
    for row in (0, len(k) - 1):
        sin_a[row] = 0.0
        cos_a[row] = np.round(cos_a[row])
    fwd_re = cos_a * window
    fwd_im = -sin_a * window
    c = np.full(len(k), 2.0)
    c[0] = 1.0
    c[-1] = 1.0
    bwd_re = (c[:, None] * cos_a / N_FFT) * window
    bwd_im = (c[:, None] * sin_a / N_FFT) * window
    f32 = lambda a: np.ascontiguousarray(a.astype(np.float32))
    return {
        "decoder.generator.stft.weight_forward_real": f32(fwd_re[:, None, :]),
        "decoder.generator.stft.weight_forward_imag": f32(fwd_im[:, None, :]),
        "decoder.generator.stft.weight_backward_real": f32(bwd_re[:, None, :]),
        "decoder.generator.stft.weight_backward_imag": f32(bwd_im[:, None, :]),
        "decoder.generator.stft.window": f32(window),
    }


def conv_geometry(model):
    import torch
    out = {}
    for name, mod in model.named_modules():
        if isinstance(mod, torch.nn.Conv1d):
            out[canonical(name) + ".weight"] = [mod.kernel_size[0], mod.stride[0], mod.padding[0], mod.padding[0],
                                                mod.dilation[0], mod.groups, 0, 0]
        elif isinstance(mod, torch.nn.ConvTranspose1d):
            out[canonical(name) + ".weight"] = [mod.kernel_size[0], mod.stride[0], mod.padding[0], mod.padding[0],
                                                mod.dilation[0], mod.groups, 1, mod.output_padding[0]]
    for nm in ("weight_forward_real", "weight_forward_imag"):
        out[f"decoder.generator.stft.{nm}"] = [N_FFT, HOP, 0, 0, 1, 1, 0, 0]
    for nm in ("weight_backward_real", "weight_backward_imag"):
        out[f"decoder.generator.stft.{nm}"] = [N_FFT, HOP, 0, 0, 1, 1, 1, 0]
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", required=True, help="directory for kokoro-82m.gguf")
    ap.add_argument("--llama-cpp", default=os.path.expanduser("~/Work/llama.cpp"), help="for gguf-py")
    a = ap.parse_args()
    sys.path.insert(0, os.path.join(a.llama_cpp, "gguf-py"))
    import gguf
    import torch
    from kokoro.model import KModel

    snap = kg.KOKORO_SNAPSHOT
    cfg = json.load(open(snap + "config.json", encoding="utf8"))
    model = KModel(repo_id="hexgrad/Kokoro-82M", config=snap + "config.json", model=snap + "kokoro-v1_0.pth")
    model.eval()
    fold_weight_norm(model)
    geometry = conv_geometry(model)
    sd = {k: v.detach().cpu() for k, v in model.state_dict().items()}

    tensors = {}
    lstm_prefixes = sorted({k.rsplit(".", 1)[0] for k in sd if k.endswith("weight_ih_l0")})
    for p in lstm_prefixes:
        tensors.update({canonical(k): v for k, v in lstm_tensors(p, sd).items()})
    for k, v in sd.items():
        if any(k.startswith(p + ".") for p in lstm_prefixes):
            continue
        if k.startswith("bert.pooler") or k.endswith("position_ids"):
            continue
        tensors[canonical(k)] = v
    tensors = {k: np.ascontiguousarray(v.numpy().astype(np.float32)) if hasattr(v, "numpy") else v for k, v in tensors.items()}
    tensors.update(stft_basis())
    for w in geometry:
        assert w in tensors, w

    voices = {}
    for f in sorted(glob.glob(snap + "voices/*.pt")):
        name = os.path.basename(f)[:-3]
        pack = torch.load(f, map_location="cpu", weights_only=True)
        voices[name] = np.ascontiguousarray(pack.squeeze(1).numpy().astype(np.float32))
        tensors["voice." + name] = voices[name]
    rows, style_dim = next(iter(voices.values())).shape
    vocab = cfg["vocab"]
    symbols = sorted(vocab.items(), key=lambda kv: kv[1])

    long = [k for k in tensors if len(k) >= GGML_MAX_NAME]
    assert not long, long
    path = os.path.join(a.out, "kokoro-82m.gguf")
    w = gguf.GGUFWriter(path, ARCH)
    w.add_name("Kokoro-82M v1.0")
    w.add_string("kokoro.revision", kg.KOKORO_REVISION)
    w.add_string("kokoro.source", "hexgrad/Kokoro-82M kokoro-v1_0.pth + voices (Apache-2.0)")
    w.add_uint32("styletts2.sample_rate", 24000)
    w.add_uint32("styletts2.n_symbols", int(cfg["n_token"]))
    w.add_uint32("styletts2.style_dim", int(style_dim))
    w.add_uint32("styletts2.style_rows", int(rows))
    w.add_uint32("styletts2.n_harmonics", int(tensors["decoder.generator.m_source.l_linear.weight"].shape[1]))
    w.add_array("styletts2.voices", list(voices.keys()))
    w.add_string("styletts2.stft_pad", "reflect")
    w.add_string("styletts2.istft_norm", "envelope")
    w.add_float32("styletts2.stft_eps", 0.0)
    w.add_string("styletts2.resample", "torch")
    w.add_array("kokoro.symbol_cpts", [ord(s) for s, _ in symbols])
    w.add_array("kokoro.symbol_ids", [int(i) for _, i in symbols])
    for name, geo in sorted(geometry.items()):
        w.add_array(f"styletts2.conv.{name}", [int(x) for x in geo])
    for name in sorted(tensors):
        w.add_tensor(name, tensors[name])
    w.write_header_to_file()
    w.write_kv_data_to_file()
    w.write_tensors_to_file()
    w.close()
    with open(path + ".LICENSE", "w", encoding="utf8") as f:
        f.write(f"kokoro-82m.gguf - Kokoro-82M v1.0 weights and voice packs (hexgrad, Apache License 2.0), converted from "
                f"hexgrad/Kokoro-82M @ {kg.KOKORO_REVISION} by modules/dasLLAMA/harness/convert_kokoro.py; "
                "see the dasLLAMA THIRD_PARTY_NOTICES.md\n")
    total = sum(t.nbytes for t in tensors.values())
    print(f"wrote {path}: {len(tensors)} tensors, {total / 1e6:.1f} MB f32, {len(geometry)} conv geometries, "
          f"{len(voices)} voices, {len(symbols)} symbols; {os.path.getsize(path)} bytes on disk")


if __name__ == "__main__":
    main()
