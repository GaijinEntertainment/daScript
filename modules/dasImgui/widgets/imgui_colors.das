options gen2
options _comment_hygiene = true
options indenting = 4
options _allow_imgui_legacy = true

module imgui_colors shared

require imgui public

// Stand-alone packer for ImGui's IM_COL32 byte order, decoupled from
// the legacy v1 `daslib/imgui_boost.das`. Pulls only the imgui module
// (for the shift constants). dasIMGUI is compiled in the default RGBA
// mode (`IMGUI_USE_BGRA_PACKED_COLOR` off), so `R_SHIFT=0`, `B_SHIFT=16`;
// the renderer decodes the same word order, so colors round-trip.

def rgba(r, g, b, a : int) : uint {
    return ((uint(a) << uint(IM_COL32_A_SHIFT))
          | (uint(r) << uint(IM_COL32_R_SHIFT))
          | (uint(g) << uint(IM_COL32_G_SHIFT))
          | (uint(b) << uint(IM_COL32_B_SHIFT)))
}

def rgba(r, g, b, a : uint) : uint {
    return ((a << uint(IM_COL32_A_SHIFT))
          | (r << uint(IM_COL32_R_SHIFT))
          | (g << uint(IM_COL32_G_SHIFT))
          | (b << uint(IM_COL32_B_SHIFT)))
}

let IM_COL32_WHITE = rgba(255u, 255u, 255u, 255u)
let IM_COL32_BLACK = rgba(0u, 0u, 0u, 255u)
let IM_COL32_BLACK_TRANS = rgba(0u, 0u, 0u, 0u)

// HSV → RGB / RGB → HSV converters — wrap ImGui's float-out-param helpers so callers get an
// `(r,g,b)` / `(h,s,v)` triple in one shot. Saturation/Value stay in [0,1]; Hue wraps in [0,1)
// (1.0 folds to 0). Used by palette init in imgui_demo and any procedural color generation.

def hsv_to_rgb(h, s, v : float) : float3 {
    var r, g, b : float
    ColorConvertHSVtoRGB(h, s, v, r, g, b)
    return float3(r, g, b)
}

def rgb_to_hsv(r, g, b : float) : float3 {
    var h, s, v : float
    ColorConvertRGBtoHSV(r, g, b, h, s, v)
    return float3(h, s, v)
}
