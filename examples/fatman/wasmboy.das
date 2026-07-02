options gen2
options indenting = 4

// The little wasm boy: the fat man's web-side sibling. Statically links the imgui
// family (dasImgui + dasImguiImplot + dasImguiNodeEditor — the externals the
// playground interpreter binds; no dasVulkan on wasm) into daslang_static and
// requires them all under node. Exists purely to CI-gate the emscripten arm of the
// externals' both-worlds CMakeLists + the web/CMakeLists .das embeds — the wiring
// the playground's furier/path_tracer_lab cards run on. Build recipe: clone the
// three externals into modules/ of a daslang checkout, emcmake-configure web/,
// build daslang_static, run this file via web/test/wasmboy_node.js. A `require`
// proves registration + boost-path resolution (the --embed-file mounts) + the
// static wasm link; nothing here opens a canvas, so it runs under plain node.

// C++ binding modules (require-to-link is the whole test, hence the nolints)
require imgui                       // nolint:STYLE030
require imgui_app                   // nolint:STYLE030
require imgui_app_headless          // nolint:STYLE030
require implot                      // nolint:STYLE030
require imgui_node_editor           // nolint:STYLE030

// one boost path per external — proves the ADD_MODULE_DAS registrations + embeds resolve
require imgui/imgui_boost_v2
require imgui/imgui_theme_daslang   // nolint:STYLE030
require imgui/imgui_implot_boost_v2
require imgui/imgui_node_editor_boost_v2

[export]
def main {
    print("the little wasm boy stands: imgui + imgui_app(+headless) + implot + node_editor statically linked in wasm\n")
}
