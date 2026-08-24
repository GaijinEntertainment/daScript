#!/usr/bin/env python3
# Silero-VAD (v6, 16 kHz) weights -> dasLLAMA silero_vad.bin (parakeet-style container).
# Source = the official `silero_vad_16k_op15.onnx` initializers (shipped in the MIT `silero-vad`
# pip package, silero_vad/data/). NOTE: the sibling silero_vad_16k.safetensors holds the
# NON-reparameterized training weights and does NOT match the deployed graph — the op15
# `reparam_conv` initializers (+ the LSTM, which lives in an If subgraph) are the canonical ones.
#
#   python convert_silero_vad.py [op15.onnx] [out.bin]
#
# Container layout: history/dasLLAMA/silero_vad_plan.md.
import sys, os, struct
import numpy as np
import onnx
from onnx import numpy_helper

MAGIC = 0x67676D6C
HEADER = [1, 16000, 256, 128, 64, 576, 64, 258, 129, 128]  # see silero_vad_plan.md


def default_model():
    try:
        import silero_vad
        return os.path.join(os.path.dirname(silero_vad.__file__), "data", "silero_vad_16k_op15.onnx")
    except Exception:
        return "silero_vad_16k_op15.onnx"


def all_initializers(model):
    """Every initializer, walking into If/Loop subgraphs (the LSTM lives in a decoder If)."""
    out = {}

    def walk(g):
        for t in g.initializer:
            out[t.name] = numpy_helper.to_array(t)
        for n in g.node:
            for a in n.attribute:
                if a.HasField("g"):
                    walk(a.g)
                for sg in a.graphs:
                    walk(sg)
    walk(model.graph)
    return out


def main():
    model_path = sys.argv[1] if len(sys.argv) > 1 and sys.argv[1] else default_model()
    out_path = sys.argv[2] if len(sys.argv) > 2 and sys.argv[2] else "silero_vad.bin"
    w = all_initializers(onnx.load(model_path))

    def g(name):
        if name not in w:
            raise KeyError(f"{name} not found; have {sorted(w)[:20]}...")
        return w[name]

    tensors = {
        "stft_conv": g("model.stft.forward_basis_buffer").reshape(258, 256),
        "conv1.w": g("model.encoder.0.reparam_conv.weight"), "conv1.b": g("model.encoder.0.reparam_conv.bias"),
        "conv2.w": g("model.encoder.1.reparam_conv.weight"), "conv2.b": g("model.encoder.1.reparam_conv.bias"),
        "conv3.w": g("model.encoder.2.reparam_conv.weight"), "conv3.b": g("model.encoder.2.reparam_conv.bias"),
        "conv4.w": g("model.encoder.3.reparam_conv.weight"), "conv4.b": g("model.encoder.3.reparam_conv.bias"),
        "lstm.w_ih": g("model.decoder.rnn.weight_ih"), "lstm.w_hh": g("model.decoder.rnn.weight_hh"),
        "lstm.b": (g("model.decoder.rnn.bias_ih") + g("model.decoder.rnn.bias_hh")),
        "final.w": g("model.decoder.decoder.2.weight").reshape(128), "final.b": g("model.decoder.decoder.2.bias"),
    }

    with open(out_path, "wb") as f:
        f.write(struct.pack("<I", MAGIC))
        f.write(struct.pack("<%di" % len(HEADER), *HEADER))
        for name, arr in tensors.items():
            a = np.ascontiguousarray(arr, dtype=np.float32)
            nb = name.encode("utf-8")
            f.write(struct.pack("<i", a.ndim))
            f.write(struct.pack("<i", len(nb)))
            f.write(struct.pack("<i", 0))  # ttype 0 = F32
            f.write(struct.pack("<%di" % a.ndim, *a.shape))
            f.write(nb)
            f.write(a.tobytes())
    print(f"wrote {out_path}  ({len(tensors)} tensors, {os.path.getsize(out_path)} bytes)")


if __name__ == "__main__":
    main()
