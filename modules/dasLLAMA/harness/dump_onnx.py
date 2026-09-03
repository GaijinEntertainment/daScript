"""Dump an ONNX graph as the op inventory a port needs: opset, I/O, op histogram, every
initializer with shape and byte size, every node with its attributes and (inferred) output
shapes, subgraph bodies recursed. Usage:
    python dump_onnx.py model.onnx [--nodes out.txt] [--voices voices.npz]
The summary goes to stdout; the full node listing goes to --nodes (default: stdout too)."""
import argparse
import collections
import sys

import numpy as np
import onnx
from onnx import numpy_helper, shape_inference

DT = {v: k for k, v in onnx.TensorProto.DataType.items()}


def shape_of(vi):
    t = vi.type.tensor_type
    if not t.HasField("shape"):
        return "?"
    dims = []
    for d in t.shape.dim:
        dims.append(str(d.dim_value) if d.HasField("dim_value") else (d.dim_param or "?"))
    return "x".join(dims) if dims else "scalar"


def attr_str(a):
    k = a.type
    if k == onnx.AttributeProto.INT:
        return str(a.i)
    if k == onnx.AttributeProto.FLOAT:
        return f"{a.f:g}"
    if k == onnx.AttributeProto.STRING:
        return repr(a.s.decode("utf-8", "replace"))
    if k == onnx.AttributeProto.INTS:
        return "[" + ",".join(map(str, a.ints)) + "]"
    if k == onnx.AttributeProto.FLOATS:
        return "[" + ",".join(f"{f:g}" for f in a.floats) + "]"
    if k == onnx.AttributeProto.TENSOR:
        t = numpy_helper.to_array(a.t)
        return f"tensor{list(t.shape)}:{t.dtype}" + (f"={t.tolist()}" if t.size <= 8 else "")
    if k == onnx.AttributeProto.GRAPH:
        return f"<graph {a.g.name} {len(a.g.node)} nodes>"
    return f"<{onnx.AttributeProto.AttributeType.Name(k)}>"


def walk(graph, depth, hist, inits, lines, shapes):
    ind = "  " * depth
    for init in graph.initializer:
        arr = numpy_helper.to_array(init)
        inits.append((init.name, DT[init.data_type], list(arr.shape), arr.nbytes, depth))
        shapes[init.name] = "x".join(map(str, arr.shape)) or "scalar"
    for vi in list(graph.value_info) + list(graph.input) + list(graph.output):
        shapes.setdefault(vi.name, shape_of(vi))
    for n in graph.node:
        hist[n.op_type] += 1
        attrs = " ".join(f"{a.name}={attr_str(a)}" for a in n.attribute)
        outs = ", ".join(f"{o}:{shapes.get(o, '?')}" for o in n.output)
        ins = ", ".join(f"{i}:{shapes.get(i, '?')}" if i else "''" for i in n.input)
        lines.append(f"{ind}{n.op_type:<20} {n.name or ''}\n{ind}    in  {ins}\n{ind}    out {outs}" + (f"\n{ind}    {attrs}" if attrs else ""))
        for a in n.attribute:
            if a.type == onnx.AttributeProto.GRAPH:
                lines.append(f"{ind}  -- body {a.name} of {n.name}:")
                walk(a.g, depth + 1, hist, inits, lines, shapes)
            elif a.type == onnx.AttributeProto.GRAPHS:
                for g in a.graphs:
                    lines.append(f"{ind}  -- body {a.name}/{g.name} of {n.name}:")
                    walk(g, depth + 1, hist, inits, lines, shapes)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("model")
    ap.add_argument("--nodes")
    ap.add_argument("--voices")
    args = ap.parse_args()
    m = onnx.load(args.model)
    try:
        m = shape_inference.infer_shapes(m, strict_mode=False, data_prop=True)
        inferred = "yes"
    except Exception as e:  # noqa: BLE001
        inferred = f"no ({e})"
    print(f"model {args.model}")
    print(f"ir_version {m.ir_version} producer {m.producer_name} {m.producer_version} opsets " + ", ".join(f"{o.domain or 'ai.onnx'}:{o.version}" for o in m.opset_import))
    print(f"shape inference: {inferred}")
    g = m.graph
    init_names = {i.name for i in g.initializer}
    print("inputs:")
    for vi in g.input:
        if vi.name not in init_names:
            print(f"  {vi.name}: {DT[vi.type.tensor_type.elem_type]} {shape_of(vi)}")
    print("outputs:")
    for vi in g.output:
        print(f"  {vi.name}: {DT[vi.type.tensor_type.elem_type]} {shape_of(vi)}")
    hist = collections.Counter()
    inits, lines, shapes = [], [], {}
    walk(g, 0, hist, inits, lines, shapes)
    print(f"nodes {sum(hist.values())} distinct ops {len(hist)}")
    for op, c in sorted(hist.items(), key=lambda kv: -kv[1]):
        print(f"  {op:<22} {c}")
    total = sum(b for _, _, _, b, _ in inits)
    big = [i for i in inits if i[3] >= 1024]
    print(f"initializers {len(inits)} ({total / 1e6:.2f} MB), >=1KB: {len(big)}")
    for name, dt, shape, nbytes, depth in sorted(big, key=lambda i: -i[3]):
        print(f"  {'  ' * depth}{name}: {dt} {shape} {nbytes / 1e6:.3f} MB")
    if args.nodes:
        with open(args.nodes, "w") as f:
            f.write("\n".join(lines) + "\n")
        print(f"node listing: {args.nodes} ({len(lines)} nodes)")
    else:
        print("\n".join(lines))
    if args.voices:
        z = np.load(args.voices)
        print(f"voices {args.voices}: {len(z.files)} keys")
        for k in z.files:
            a = z[k]
            print(f"  {k}: {a.dtype} {list(a.shape)} min {a.min():.4f} max {a.max():.4f} mean {a.mean():.4f}")


if __name__ == "__main__":
    sys.exit(main())
