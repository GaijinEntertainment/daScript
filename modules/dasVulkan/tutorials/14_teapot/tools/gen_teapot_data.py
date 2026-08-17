#!/usr/bin/env python3
"""Parse teapotCGA.bpt (original Newell 32-patch Utah teapot) into a daslang data module.

BPT format: line 0 = patch count; then per patch a "3 3" degree line + 16 control-point lines
(row-major 4x4, "x y z"). Patch p -> control points [p*16 .. p*16+15]. We keep the raw Newell
coordinates (Z up); centering/scale/orientation is applied at render time via the MVP.
"""
import sys, os

def main():
    here = os.path.dirname(os.path.abspath(__file__))
    src = os.path.join(here, "..", "teapotCGA.bpt")
    with open(src) as f:
        lines = [l.strip() for l in f if l.strip() != ""]
    npatches = int(lines[0])
    assert npatches == 32, npatches
    idx = 1
    pts = []
    for p in range(npatches):
        du, dv = lines[idx].split()
        assert du == "3" and dv == "3", (du, dv)
        idx += 1
        for k in range(16):
            x, y, z = (float(v) for v in lines[idx].split())
            pts.append((x, y, z))
            idx += 1
    assert len(pts) == 512, len(pts)

    xs = [p[0] for p in pts]; ys = [p[1] for p in pts]; zs = [p[2] for p in pts]
    bb_min = (min(xs), min(ys), min(zs))
    bb_max = (max(xs), max(ys), max(zs))
    center = tuple((bb_min[i] + bb_max[i]) * 0.5 for i in range(3))
    extent = max(bb_max[i] - bb_min[i] for i in range(3))

    out = os.path.join(here, "..", "teapot_data.das")
    L = []
    L.append("options gen2")
    L.append("options _comment_hygiene = true")
    L.append("options indenting = 4")
    L.append("")
    L.append("// Generated from teapotCGA.bpt (original Newell 32-patch Utah teapot, public domain) by")
    L.append("// tools/gen_teapot_data.py -- DO NOT EDIT BY HAND. 32 bicubic Bezier patches x 16 control")
    L.append("// points = 512 points, row-major 4x4 per patch; patch p occupies [p*16 .. p*16+15]. Raw")
    L.append("// Newell units (Z up); centering/scale/orientation is applied at render time via the MVP.")
    L.append("")
    L.append("module teapot_data public")
    L.append("")
    L.append("let public TEAPOT_PATCH_COUNT = 32")
    L.append("let public TEAPOT_CONTROL_POINT_COUNT = 512")
    L.append("")
    L.append("// model-space bounds of the raw control net (for the MVP fit):")
    L.append("let public TEAPOT_CENTER = float3({:.5f}, {:.5f}, {:.5f})".format(*center))
    L.append("let public TEAPOT_EXTENT = {:.5f}".format(extent))
    L.append("")
    L.append("//! The 512 control points (32 patches x 16). Patch p = [p*16 .. p*16+15], row-major 4x4.")
    L.append("def public teapot_control_points : array<float3> {")
    L.append("    return <- [")
    for i, (x, y, z) in enumerate(pts):
        patch = i // 16
        tag = "  // patch {}".format(patch) if i % 16 == 0 else ""
        L.append("        float3({:.5f}, {:.5f}, {:.5f}),{}".format(x, y, z, tag))
    L.append("    ]")
    L.append("}")
    L.append("")
    with open(out, "w") as f:
        f.write("\n".join(L))
    print("wrote {} ({} control points)".format(out, len(pts)))
    print("center = {}  extent = {:.4f}  bb_min={} bb_max={}".format(
        tuple(round(c, 4) for c in center), extent,
        tuple(round(c, 3) for c in bb_min), tuple(round(c, 3) for c in bb_max)))

if __name__ == "__main__":
    main()
