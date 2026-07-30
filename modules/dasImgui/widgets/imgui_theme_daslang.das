options gen2
options _comment_hygiene = true
options indenting = 4

module imgui_theme_daslang shared

require imgui public
require daslib/fio
require daslib/module_path

//! Drop-in Dear ImGui theme matching daslang.io — warm-dark background,
//! single amber accent (``#e8a13a``), tight 2px rounding. Call
//! :ref:`apply_daslang_theme <function-imgui_theme_daslang_apply_daslang_theme>`
//! once after ``CreateContext`` (or after :ref:`live_imgui_init
//! <function-imgui_live_live_imgui_init>`) and every window, button, slider,
//! tab and plot picks it up. Daslang port of the
//! ``daslang_imgui_style.h`` reference header.
//!
//! HDPI: ``apply_daslang_theme`` stores logical (1x) constants — paddings,
//! spacings, rounding. For high-DPI displays, follow up with
//! ``GetStyle() |> ScaleAllSizes(scale)`` and load the font at
//! ``14 * scale`` px. The :ref:`live_imgui_init
//! <function-imgui_live_live_imgui_init>` path does both automatically from
//! ``glfwGetWindowContentScale``; if you bring your own ImGui context, mirror
//! that pattern.

// ===== Palette =====
// Site colors precomputed as float4 RGBA. Tweak here to shift the whole
// theme without rewriting the per-slot assignments below.
let private bg          = float4(0.051f, 0.047f, 0.039f, 1.0f)  // #0d0c0a
let private bg2         = float4(0.082f, 0.075f, 0.059f, 1.0f)  // #15130f WindowBg
let private frame       = float4(0.067f, 0.055f, 0.031f, 1.0f)  // #110e08 FrameBg
let private frameHover  = float4(0.110f, 0.090f, 0.055f, 1.0f)
let private frameActive = float4(0.149f, 0.122f, 0.075f, 1.0f)
let private title       = float4(0.094f, 0.082f, 0.063f, 1.0f)
let private titleActive = float4(0.133f, 0.110f, 0.075f, 1.0f)  // #221c13
let private border      = float4(0.165f, 0.141f, 0.110f, 1.0f)  // #2a241c
let private btn         = float4(0.165f, 0.133f, 0.094f, 1.0f)  // #2a2218
let private btnHover    = float4(0.227f, 0.180f, 0.122f, 1.0f)  // #3a2e1f
let private amber       = float4(0.910f, 0.631f, 0.227f, 1.0f)  // #e8a13a
let private amberDim    = float4(0.659f, 0.455f, 0.125f, 1.0f)  // #a87420
let private amberSoft   = float4(0.910f, 0.631f, 0.227f, 0.27f)
let private fg          = float4(0.910f, 0.886f, 0.824f, 1.0f)  // #e8e2d2
let private fgFaint     = float4(0.357f, 0.333f, 0.278f, 1.0f)  // #5b5547

def public apply_daslang_theme(var style : ImGuiStyle) {
    //! Apply the daslang.io theme to ``style`` in-place — sets every
    //! ``ImGuiCol_*`` slot, every rounding/border value, and the tighter
    //! padding/spacing that matches the site's information density.

    // ===== Shape =====
    style.WindowRounding    = 2.0f
    style.ChildRounding     = 2.0f
    style.FrameRounding     = 2.0f
    style.PopupRounding     = 2.0f
    style.GrabRounding      = 1.0f
    style.TabRounding       = 2.0f
    style.ScrollbarRounding = 2.0f

    style.WindowBorderSize  = 1.0f
    style.ChildBorderSize   = 1.0f
    style.PopupBorderSize   = 1.0f
    style.FrameBorderSize   = 1.0f
    style.TabBorderSize     = 0.0f

    // ===== Spacing =====
    style.WindowPadding     = float2(8.0f, 8.0f)
    style.FramePadding      = float2(6.0f, 3.0f)
    style.ItemSpacing       = float2(8.0f, 5.0f)
    style.ItemInnerSpacing  = float2(6.0f, 4.0f)
    style.IndentSpacing     = 16.0f
    style.ScrollbarSize     = 10.0f
    style.GrabMinSize       = 10.0f

    style.WindowTitleAlign  = float2(0.0f, 0.5f)
    style.ButtonTextAlign   = float2(0.5f, 0.5f)

    // ===== Colors =====
    style.Colors[int(ImGuiCol.Text)]                  = fg
    style.Colors[int(ImGuiCol.TextDisabled)]          = fgFaint
    style.Colors[int(ImGuiCol.WindowBg)]              = bg2
    style.Colors[int(ImGuiCol.ChildBg)]               = bg
    style.Colors[int(ImGuiCol.PopupBg)]               = float4(0.094f, 0.082f, 0.063f, 0.98f)
    style.Colors[int(ImGuiCol.Border)]                = border
    style.Colors[int(ImGuiCol.BorderShadow)]          = float4(0.0f, 0.0f, 0.0f, 0.0f)

    style.Colors[int(ImGuiCol.FrameBg)]               = frame
    style.Colors[int(ImGuiCol.FrameBgHovered)]        = frameHover
    style.Colors[int(ImGuiCol.FrameBgActive)]         = frameActive

    style.Colors[int(ImGuiCol.TitleBg)]               = title
    style.Colors[int(ImGuiCol.TitleBgActive)]         = titleActive
    style.Colors[int(ImGuiCol.TitleBgCollapsed)]      = title

    style.Colors[int(ImGuiCol.MenuBarBg)]             = title
    style.Colors[int(ImGuiCol.ScrollbarBg)]           = frame
    style.Colors[int(ImGuiCol.ScrollbarGrab)]         = btn
    style.Colors[int(ImGuiCol.ScrollbarGrabHovered)]  = btnHover
    style.Colors[int(ImGuiCol.ScrollbarGrabActive)]   = amberDim

    style.Colors[int(ImGuiCol.CheckMark)]             = amber
    style.Colors[int(ImGuiCol.SliderGrab)]            = amberDim
    style.Colors[int(ImGuiCol.SliderGrabActive)]      = amber

    style.Colors[int(ImGuiCol.Button)]                = btn
    style.Colors[int(ImGuiCol.ButtonHovered)]         = btnHover
    style.Colors[int(ImGuiCol.ButtonActive)]          = amberDim

    style.Colors[int(ImGuiCol.Header)]                = btnHover
    style.Colors[int(ImGuiCol.HeaderHovered)]         = float4(0.290f, 0.227f, 0.149f, 1.0f)
    style.Colors[int(ImGuiCol.HeaderActive)]          = amberDim

    style.Colors[int(ImGuiCol.Separator)]             = border
    style.Colors[int(ImGuiCol.SeparatorHovered)]      = amberDim
    style.Colors[int(ImGuiCol.SeparatorActive)]       = amber

    style.Colors[int(ImGuiCol.ResizeGrip)]            = amberSoft
    style.Colors[int(ImGuiCol.ResizeGripHovered)]     = amberDim
    style.Colors[int(ImGuiCol.ResizeGripActive)]      = amber

    style.Colors[int(ImGuiCol.Tab)]                   = float4(0.122f, 0.102f, 0.071f, 1.0f)
    style.Colors[int(ImGuiCol.TabHovered)]            = btnHover
    style.Colors[int(ImGuiCol.TabSelected)]           = bg2
    style.Colors[int(ImGuiCol.TabDimmed)]             = title
    style.Colors[int(ImGuiCol.TabDimmedSelected)]     = titleActive

    style.Colors[int(ImGuiCol.PlotLines)]             = amber
    style.Colors[int(ImGuiCol.PlotLinesHovered)]      = fg
    style.Colors[int(ImGuiCol.PlotHistogram)]         = amberDim
    style.Colors[int(ImGuiCol.PlotHistogramHovered)]  = amber

    style.Colors[int(ImGuiCol.TableHeaderBg)]         = title
    style.Colors[int(ImGuiCol.TableBorderStrong)]     = border
    style.Colors[int(ImGuiCol.TableBorderLight)]      = float4(0.118f, 0.102f, 0.082f, 1.0f)
    style.Colors[int(ImGuiCol.TableRowBg)]            = float4(0.0f, 0.0f, 0.0f, 0.0f)
    style.Colors[int(ImGuiCol.TableRowBgAlt)]         = float4(1.0f, 1.0f, 1.0f, 0.02f)

    style.Colors[int(ImGuiCol.TextSelectedBg)]        = amberSoft
    style.Colors[int(ImGuiCol.DragDropTarget)]        = amber
    style.Colors[int(ImGuiCol.NavCursor)]          = amber
    style.Colors[int(ImGuiCol.NavWindowingHighlight)] = float4(0.910f, 0.631f, 0.227f, 0.70f)
    style.Colors[int(ImGuiCol.NavWindowingDimBg)]     = float4(0.0f, 0.0f, 0.0f, 0.50f)
    style.Colors[int(ImGuiCol.ModalWindowDimBg)]      = float4(0.0f, 0.0f, 0.0f, 0.55f)
}

def public apply_daslang_theme() {
    //! Convenience overload — applies the theme to ``ImGui::GetStyle()``.
    //! Call once from your script's ``init()`` after ``live_imgui_init``.
    apply_daslang_theme(GetStyle())
}

// ===== Font =====

// JetBrains Mono lives at modules/dasImgui/font/; the .das is one level deeper (widgets/), so step up
// one dir then join "font/...". `get_this_module_dir()` returns the relocated dir for daspkg release
// bundles (PR #2579 resolver), so this works in dev + bundle.

def public daslang_font_ttf_path() : string {
    //! Absolute path to the bundled ``JetBrainsMono-Regular.ttf`` — resolved via ``get_this_module_dir``
    //! so daspkg-release standalone bundles pick up the relocated path. Read it directly to feed your own
    //! ``AddFontFromFileTTF`` (custom size / config / glyph ranges).

    // On web the TTF isn't on a real filesystem: `daspkg release wasm` embeds it into MEMFS at the fixed
    // path from this module's .das_package release_embed_file. Uses get_running_platform_name (not
    // get_platform_name, which const-folds to the compile host) so a cross-compiled in-browser exe resolves it.
    if (get_running_platform_name() == "emscripten") {
        return "/modules/dasImgui/font/JetBrainsMono-Regular.ttf"
    }
    return path_join(dir_name(get_this_module_dir()), "font/JetBrainsMono-Regular.ttf")
}

def public load_daslang_font(size_px : float = 14.0f) : ImFont? {
    //! Load JetBrains Mono into the current ImGui font atlas at ``size_px`` (pass ``14 * content_scale``
    //! for HDPI to keep it crisp instead of upscaled by ``ImGuiIO.FontGlobalScale``). Call once from
    //! ``init()`` *before* ``apply_daslang_theme``. Returns the ``ImFont?`` — null if the TTF is missing (ImGui falls back to ProggyClean).
    let ttf = daslang_font_ttf_path()
    var atlas & = unsafe(*GetIO().Fonts)
    return atlas |> AddFontFromFileTTF(ttf, size_px, null, null)
}
