"""KittenTTS checkpoint -> dasLLAMA GGUF.

    python convert_kitten.py --size nano|mini --out ~/Work/llama.cpp/models [--llama-cpp ~/Work/llama.cpp]

Reads the pinned ONNX snapshot (kitten_graph.SNAPSHOTS), rewrites the mini's dynamic-uint8 ops
into f32 weights, and writes `kitten-<size>.gguf`:

- every weight as f32 under a stable name (`kmodel.` dropped; the predictor's submodules get
  their `predictor.` prefix back; the ALBERT layer group is `bert.layer.*`), [out, in] like
  PyTorch; the LSTM gates reordered from ONNX's i,o,f,c to i,f,g,o with the two bias halves
  summed - one cell serves every LSTM in the block home;
- the geometry of every convolution as metadata `styletts2.conv.<weight>` =
  [kernel, stride, pad_left, pad_right, dilation, groups, transposed, output_padding];
- the eight voices from voices.npz as `voice.<name>` [400, 256] tensors, `speed_priors` and
  `voice_aliases` from config.json as metadata.
"""
import argparse
import json
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import kitten_graph as kg  # noqa: E402

ARCH = "kitten-tts"
GGML_MAX_NAME = 64

# node-path prefixes the exporter wrote without their owning module
PREDICTOR_PREFIXES = ("text_encoder.lstms.", "lstm.", "shared.", "duration_proj.", "F0.", "N.", "F0_proj.", "N_proj.")
RENAMES = [("bert.encoder.albert_layer_groups.0.albert_layers.0.", "bert.layer.")]
GATE_REORDER = [0, 2, 3, 1]   # ONNX i,o,f,c -> i,f,g,o


def canonical(name):
    for p in PREDICTOR_PREFIXES:
        if name.startswith(p) and not name.startswith("predictor."):
            name = "predictor." + name
            break
    for old, new in RENAMES:
        if name.startswith(old):
            name = new + name[len(old):]
    return name


def lstm_tensors(name, arr):
    """One LSTM initializer (W [dir,4H,I], R [dir,4H,H] or B [dir,8H]) -> das layout."""
    base, kind = name.rsplit(".", 1)
    ndir = arr.shape[0]
    if kind == "B":
        h = arr.shape[1] // 8
        wb, rb = arr[:, : 4 * h], arr[:, 4 * h:]
        b = (wb + rb).reshape(ndir, 4, h)[:, GATE_REORDER, :].reshape(ndir, 4 * h)
        return {base + ".bias": b}
    h = arr.shape[1] // 4
    w = arr.reshape(ndir, 4, h, arr.shape[2])[:, GATE_REORDER, :, :].reshape(ndir, 4 * h, arr.shape[2])
    return {base + (".weight_ih" if kind == "W" else ".weight_hh"): w}


def conv_geometry(model, name_of):
    """{canonical weight name: [k, stride, pad_l, pad_r, dilation, groups, transposed, output_padding]}."""
    out = {}
    for n in model.graph.node:
        if n.op_type not in ("Conv", "ConvTranspose"):
            continue
        attrs = {a.name: list(a.ints) for a in n.attribute if a.ints}
        attrs.update({a.name: a.i for a in n.attribute if a.type == 2})
        w = name_of(n.input[1])
        k = attrs["kernel_shape"][0]
        pads = attrs.get("pads", [0, 0])
        out[w] = [k, attrs.get("strides", [1])[0], pads[0], pads[1], attrs.get("dilations", [1])[0],
                  attrs.get("group", 1), 1 if n.op_type == "ConvTranspose" else 0, attrs.get("output_padding", [0])[0]]
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--size", required=True, choices=("nano", "mini"))
    ap.add_argument("--out", required=True, help="directory for kitten-<size>.gguf")
    ap.add_argument("--llama-cpp", default=os.path.expanduser("~/Work/llama.cpp"), help="for gguf-py")
    a = ap.parse_args()
    sys.path.insert(0, os.path.join(a.llama_cpp, "gguf-py"))
    import gguf

    model = kg.dequantize(kg.load(a.size))
    raw = kg.named_tensors(model)
    tensors = {}
    for name, arr in raw.items():
        cname = canonical(name)
        if cname.endswith((".W", ".R", ".B")):
            tensors.update(lstm_tensors(cname, arr))
        else:
            tensors[cname] = arr
    # weight-name lookup for the conv geometry: initializer name -> canonical
    init_to_canon = {}
    for t in model.graph.initializer:
        n = t.name
        c = n[len("kmodel."):] if n.startswith("kmodel.") else n
        init_to_canon[n] = canonical(c)
    geometry = conv_geometry(model, lambda n: init_to_canon[n])
    for w in geometry:
        assert w in tensors, w

    cfg = json.load(open(kg.SNAPSHOTS[a.size] + "config.json", encoding="utf8"))
    voices = np.load(kg.SNAPSHOTS[a.size] + "voices.npz")
    for v in voices.files:
        tensors["voice." + v] = np.ascontiguousarray(voices[v].astype(np.float32))

    long = [k for k in tensors if len(k) >= GGML_MAX_NAME]
    assert not long, long
    path = os.path.join(a.out, f"kitten-{a.size}.gguf")
    w = gguf.GGUFWriter(path, ARCH)
    w.add_name(cfg["name"])
    w.add_string("kitten.size", a.size)
    w.add_string("kitten.revision", kg.REVISIONS[a.size])
    w.add_string("kitten.source", f"KittenML/kitten-tts-{a.size}-0.8 {kg.MODEL_FILES[a.size]}")
    w.add_uint32("styletts2.sample_rate", 24000)
    w.add_uint32("styletts2.n_symbols", int(tensors["text_encoder.embedding.weight"].shape[0]))
    w.add_uint32("styletts2.style_dim", int(voices[voices.files[0]].shape[1]))
    w.add_uint32("styletts2.style_rows", int(voices[voices.files[0]].shape[0]))
    w.add_uint32("styletts2.n_harmonics", int(tensors["decoder.generator.m_source.l_linear.weight"].shape[1]))
    w.add_array("styletts2.voices", list(voices.files))
    # the checkpoint's conv-based STFT: replicate padding, no inverse normalization, eps inside the sqrt
    w.add_string("styletts2.stft_pad", "edge")
    w.add_string("styletts2.istft_norm", "basis")
    w.add_float32("styletts2.stft_eps", 1e-14)
    w.add_string("styletts2.resample", "onnxruntime")
    for v, s in cfg.get("speed_priors", {}).items():
        w.add_float32(f"kitten.speed_prior.{v}", float(s))
    aliases = cfg.get("voice_aliases", {})
    w.add_array("kitten.alias_names", list(aliases.keys()))
    w.add_array("kitten.alias_voices", list(aliases.values()))
    for name, geo in sorted(geometry.items()):
        w.add_array(f"styletts2.conv.{name}", [int(x) for x in geo])
    for name in sorted(tensors):
        w.add_tensor(name, tensors[name])
    w.write_header_to_file()
    w.write_kv_data_to_file()
    w.write_tensors_to_file()
    w.close()
    with open(path + ".LICENSE", "w", encoding="utf8") as f:
        f.write(f"kitten-{a.size}.gguf - KittenTTS {a.size} 0.8 weights (KittenML, Apache License 2.0), "
                f"converted from KittenML/kitten-tts-{a.size}-0.8 @ {kg.REVISIONS[a.size]} by "
                "modules/dasLLAMA/harness/convert_kitten.py; see the dasLLAMA THIRD_PARTY_NOTICES.md\n")
    total = sum(t.nbytes for t in tensors.values())
    print(f"wrote {path}: {len(tensors)} tensors, {total / 1e6:.1f} MB f32, {len(geometry)} conv geometries, "
          f"{len(voices.files)} voices; {os.path.getsize(path)} bytes on disk")


if __name__ == "__main__":
    main()
