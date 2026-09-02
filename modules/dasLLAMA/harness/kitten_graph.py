"""The KittenTTS ONNX graph as the converter and the oracle both see it.

Both checkpoints come from one PyTorch module tree; the mini is a dynamic-uint8 export of it
(MatMulInteger / ConvInteger / DynamicQuantizeLSTM, f16 upsamplers). This module turns either
graph into the same f32 picture:

- `dequantize(model)` rewrites the integer ops back into MatMul / Conv / LSTM over exactly
  dequantized weights ((q - zero_point) * scale), widens the f16 initializers and neutralizes
  the f16 casts. A nano graph passes through unchanged.
- `noise_as_inputs(model)` replaces the two RNG nodes inside the sine source with graph inputs,
  so a run is a function of its inputs.
- `named_tensors(model)` returns every weight under a stable name: the `kmodel.` prefix
  dropped, anonymous `onnx::MatMul_N` / `onnx::LSTM_N` initializers named after the node that
  consumes them.
"""
import numpy as np
import onnx
from onnx import helper, numpy_helper

HUB = "/Users/borisbatkin/Work/tts-ab/g2p/.hf/hub/"
SNAPSHOTS = {
    "nano": HUB + "models--KittenML--kitten-tts-nano-0.8/snapshots/7a1db645b1f3ab9420761d87428e042b9cec3f26/",
    "mini": HUB + "models--KittenML--kitten-tts-mini-0.8/snapshots/c02725660cea441db4c383af69f1f26f5cd00947/",
}
MODEL_FILES = {"nano": "kitten_tts_nano_v0_8.onnx", "mini": "kitten_tts_mini_v0_8.onnx"}
KOKORO_REVISION = "f3ff3571791e39611d31c381e3a41a3af07b4987"
KOKORO_SNAPSHOT = HUB + "models--hexgrad--Kokoro-82M/snapshots/" + KOKORO_REVISION + "/"
REVISIONS = {"nano": "7a1db645b1f3ab9420761d87428e042b9cec3f26", "mini": "c02725660cea441db4c383af69f1f26f5cd00947"}


def load(size):
    return onnx.load(SNAPSHOTS[size] + MODEL_FILES[size])


def initializer_map(graph):
    return {t.name: t for t in graph.initializer}


def to_array(t):
    return numpy_helper.to_array(t)


def _replace_initializer(graph, name, arr):
    inits = initializer_map(graph)
    new = numpy_helper.from_array(np.ascontiguousarray(arr), name)
    if name in inits:
        inits[name].CopyFrom(new)
    else:
        graph.initializer.append(new)


def _consumers(graph):
    out = {}
    for n in graph.node:
        for i in n.input:
            out.setdefault(i, []).append(n)
    return out


def _producer(graph):
    return {o: n for n in graph.node for o in n.output}


def dequant_weight(inits, qname):
    """(q - zero_point) * scale for the `<x>_quantized` initializer, per-direction when the scale
    carries a leading axis (the LSTM case)."""
    base = qname[: -len("_quantized")]
    q = to_array(inits[qname]).astype(np.float32)
    scale = to_array(inits[base + "_scale"]).astype(np.float32)
    zp = to_array(inits[base + "_zero_point"]).astype(np.float32)
    if scale.ndim == 0:
        return (q - zp) * scale
    shape = (scale.shape[0],) + (1,) * (q.ndim - 1)
    return (q - zp.reshape(shape)) * scale.reshape(shape)


def dequantize(model):
    """Rewrite the dynamic-uint8 graph into its f32 twin; a float graph passes through."""
    g = model.graph
    inits = initializer_map(g)
    prod = _producer(g)
    cons = _consumers(g)
    remove = set()
    new_nodes = []
    for n in list(g.node):
        if n.op_type in ("MatMulInteger", "ConvInteger"):
            x_q, w_q = n.input[0], n.input[1]
            dql = prod[x_q]
            assert dql.op_type == "DynamicQuantizeLinear", dql.op_type
            x = dql.input[0]
            cast = cons[n.output[0]][0]
            assert cast.op_type == "Cast", cast.op_type
            mul = cons[cast.output[0]][0]
            assert mul.op_type == "Mul", mul.op_type
            scales_mul = prod[mul.input[1]] if mul.input[1] in prod else prod[mul.input[0]]
            assert scales_mul.op_type == "Mul" and scales_mul.name.endswith("_quant_scales_mul"), scales_mul.name
            w_name = w_q[: -len("_quantized")]
            _replace_initializer(g, w_name, dequant_weight(inits, w_q))
            op = "MatMul" if n.op_type == "MatMulInteger" else "Conv"
            inputs = [x, w_name]
            node = helper.make_node(op, inputs, [mul.output[0]], name=n.name.replace("_quant", ""))
            node.attribute.extend(n.attribute)
            new_nodes.append((n, node))
            remove.update(id(k) for k in (n, cast, mul, scales_mul))
            if all(id(c) in remove or c.op_type in ("MatMulInteger", "ConvInteger") for c in cons[x_q]):
                remove.add(id(dql))
        elif n.op_type == "DynamicQuantizeLSTM":
            x, w_q, r_q, b, seq, h0, c0 = n.input[:7]
            w = dequant_weight(inits, w_q).transpose(0, 2, 1)  # [dir, I, 4H] -> [dir, 4H, I]
            r = dequant_weight(inits, r_q).transpose(0, 2, 1)
            w_name, r_name = w_q[: -len("_quantized")], r_q[: -len("_quantized")]
            _replace_initializer(g, w_name, w)
            _replace_initializer(g, r_name, r)
            node = helper.make_node("LSTM", [x, w_name, r_name, b, seq, h0, c0], list(n.output),
                                    name=n.name.replace("_quant", ""))
            node.attribute.extend(a for a in n.attribute)
            new_nodes.append((n, node))
            remove.add(id(n))
    # every DynamicQuantizeLinear whose consumers all went is dead
    for n in g.node:
        if n.op_type == "DynamicQuantizeLinear" and all(id(c) in remove for c in cons.get(n.output[0], [])):
            remove.add(id(n))
    kept = []
    replacement = {id(old): new for old, new in new_nodes}
    for n in g.node:
        if id(n) in replacement:
            kept.append(replacement[id(n)])
        elif id(n) not in remove:
            kept.append(n)
    del g.node[:]
    g.node.extend(kept)
    for sub in _all_graphs(g):
        _widen_f16(sub)
    # the quantizer's leftovers: scale / zero-point / _quantized initializers nobody reads
    used = {i for n in g.node for i in n.input}
    for t in list(g.initializer):
        if t.name not in used and (t.name.endswith("_scale") or t.name.endswith("_zero_point") or t.name.endswith("_quantized")):
            g.initializer.remove(t)
    return model


def _all_graphs(g):
    yield g
    for n in g.node:
        for a in n.attribute:
            if a.HasField("g"):
                yield from _all_graphs(a.g)
            for sg in a.graphs:
                yield from _all_graphs(sg)


def _widen_f16(g):
    """f16 -> f32 in one graph: widen the initializers, make every f16 cast a no-op, retype the
    f16 value_info / inputs / outputs the exporter recorded."""
    for t in g.initializer:
        if t.data_type == onnx.TensorProto.FLOAT16:
            t.CopyFrom(numpy_helper.from_array(to_array(t).astype(np.float32), t.name))
    for n in g.node:
        if n.op_type == "Cast":
            for a in n.attribute:
                if a.name == "to" and a.i == onnx.TensorProto.FLOAT16:
                    a.i = onnx.TensorProto.FLOAT
    for vi in list(g.value_info):
        if vi.type.tensor_type.elem_type == onnx.TensorProto.FLOAT16:
            g.value_info.remove(vi)
    for vi in list(g.input) + list(g.output):
        if vi.type.tensor_type.elem_type == onnx.TensorProto.FLOAT16:
            vi.type.tensor_type.elem_type = onnx.TensorProto.FLOAT


NOISE_INPUTS = {"RandomUniformLike": "noise_uniform", "RandomNormalLike": "noise_normal"}


def noise_as_inputs(model):
    """The two RNG nodes become graph inputs of the same shape; returns {input_name: the
    tensor whose shape the noise must take}."""
    g = model.graph
    shapes = {}
    for n in list(g.node):
        if n.op_type in NOISE_INPUTS:
            name = NOISE_INPUTS[n.op_type]
            shapes[name] = n.input[0]
            for m in g.node:
                for k, i in enumerate(m.input):
                    if i == n.output[0]:
                        m.input[k] = name
            dims = ["B", "H"] if n.op_type == "RandomUniformLike" else ["B", "N", "H"]
            g.input.append(helper.make_tensor_value_info(name, onnx.TensorProto.FLOAT, dims))
            g.node.remove(n)
    assert len(shapes) == 2, shapes
    return shapes


def expose(model, names):
    """Add intermediate tensors as graph outputs (for the per-stage dumps); shape left open,
    which onnxruntime accepts and the strict checker does not."""
    have = {o for n in model.graph.node for o in n.output}
    for nm in names:
        if nm not in have:
            raise KeyError(nm)
        vi = model.graph.output.add()
        vi.name = nm
        vi.type.tensor_type.elem_type = onnx.TensorProto.FLOAT


def named_tensors(model):
    """{stable name: f32 array} for every weight the forward reads. Call on a dequantized
    graph."""
    g = model.graph
    out = {}
    cons = _consumers(g)
    for t in g.initializer:
        arr = to_array(t)
        if arr.dtype != np.float32 or arr.size < 2:
            if not (arr.dtype == np.float32 and t.name.startswith("kmodel.")):
                continue
        if t.name.startswith("kmodel."):
            name = t.name[len("kmodel."):]
            users = cons.get(t.name, [])
            # a MatMul reads its weight as [in, out] (the quantizer rewrote the mini's Gemms so);
            # a Gemm with transB, a Conv, an LSTM keep [out, in]
            if users and users[0].op_type == "MatMul" and users[0].input[1] == t.name:
                arr = arr.T
        elif t.name.startswith("onnx::"):
            users = cons.get(t.name, [])
            assert users, t.name
            n = users[0]
            path = n.name.strip("/").replace("/", ".")
            if n.op_type in ("MatMul",):
                # ".../query/MatMul" -> ".../query.weight" (stored [in, out], transposed below)
                name = path.rsplit(".", 1)[0] + ".weight"
                arr = arr.T
            elif n.op_type == "LSTM":
                slot = list(n.input).index(t.name)
                name = path.rsplit(".", 1)[0] + "." + {1: "W", 2: "R", 3: "B"}[slot]
            else:
                raise ValueError(f"anonymous {t.name} feeds {n.op_type} {n.name}")
        else:
            continue
        assert name not in out, name
        out[name] = np.ascontiguousarray(arr.astype(np.float32))
    return out
