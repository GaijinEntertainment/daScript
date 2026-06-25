#!/usr/bin/env python3
"""Minimal glTF (separate .bin) -> OBJ converter (no deps). Extracts POSITION / TEXCOORD_0 / NORMAL
+ indices of the first mesh primitive and writes a triangulated OBJ that glsl/geom_gen load_obj_mesh
can read. Concrete Cat Statue (Poly Haven, CC0)."""
import json, struct, os

here = os.path.dirname(os.path.abspath(__file__))
gltf_path = os.path.join(here, "concrete_cat_statue_2k.gltf")
with open(gltf_path) as f:
    g = json.load(f)
with open(os.path.join(here, g["buffers"][0]["uri"]), "rb") as f:
    buf = f.read()
accs, bvs = g["accessors"], g["bufferViews"]

CT = {5126: ("f", 4), 5125: ("I", 4), 5123: ("H", 2), 5121: ("B", 1)}
NC = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4}

def read(idx):
    a = accs[idx]; bv = bvs[a["bufferView"]]
    off = bv.get("byteOffset", 0) + a.get("byteOffset", 0)
    fmt, sz = CT[a["componentType"]]; nc = NC[a["type"]]
    stride = bv.get("byteStride", sz * nc)
    return [struct.unpack_from("<" + fmt * nc, buf, off + i * stride) for i in range(a["count"])]

prim = g["meshes"][0]["primitives"][0]["attributes"], g["meshes"][0]["primitives"][0]
attrs, p = prim
pos = read(attrs["POSITION"]); uv = read(attrs["TEXCOORD_0"]); nrm = read(attrs["NORMAL"])
idx = [v[0] for v in read(p["indices"])]

out = os.path.join(here, "concrete_cat_statue.obj")
with open(out, "w", newline="\n") as f:
    f.write("# Concrete Cat Statue -- Poly Haven (CC0). Converted from concrete_cat_statue_2k.gltf\n")
    f.write("# textures: textures/concrete_cat_statue_{diff,nor_gl,arm}_2k.jpg\n")
    for x, y, z in pos: f.write("v {:.6f} {:.6f} {:.6f}\n".format(x, y, z))
    for u, v in uv:     f.write("vt {:.6f} {:.6f}\n".format(u, v))
    for x, y, z in nrm: f.write("vn {:.6f} {:.6f} {:.6f}\n".format(x, y, z))
    for i in range(0, len(idx), 3):
        a, b, c = idx[i] + 1, idx[i + 1] + 1, idx[i + 2] + 1
        f.write("f {0}/{0}/{0} {1}/{1}/{1} {2}/{2}/{2}\n".format(a, b, c))
print("wrote {} : {} verts, {} tris".format(os.path.basename(out), len(pos), len(idx) // 3))
