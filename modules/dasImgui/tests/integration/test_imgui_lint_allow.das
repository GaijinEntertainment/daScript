options gen2
options _comment_hygiene = true

require imgui
require imgui/imgui_lint

//! Allow-list smoke — default-on lint AND only allow-listed raw calls
//! (`GetIO`, frame lifecycle, style queries). Must compile clean.

[export]
def main() {
    unsafe(GetStyle()).FontScaleMain = 1.0f    // imgui 1.92: FontGlobalScale moved to style.FontScaleMain
    // Read-only style allow-listed via GetStyleColorName in ALLOWED_IMGUI. Drawlist queries
    // (GetWindowDrawList / AddRect / AddLine / GetForegroundDrawList) were dropped from
    // ALLOWED_IMGUI by PR-B1 in favor of the with_*_drawlist rail.
    let _name = GetStyleColorName(ImGuiCol.Text)
    NewFrame()
    EndFrame()
}
