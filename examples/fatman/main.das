options gen2
options indenting = 4

// The fat man: statically links every external module into one host and requires them
// all. Exists purely to CI-gate the superbuild (static-embed) half of the external
// modules' both-worlds CMakeLists — the standalone .shared_module half each module's
// own CI covers. Build recipe: clone the externals into modules/ of a daslang checkout
// (no .daspkg_standalone marker), configure + build the daslang target, run this file.
// A `require` proves registration + boost-path resolution + the static link; nothing
// here opens a window or touches a GPU, so it runs on any CI runner.

// C++ binding modules (require-to-link is the whole test, hence the nolints)
require imgui                       // nolint:STYLE030
require imgui_app                   // nolint:STYLE030
require imgui_app_headless          // nolint:STYLE030
require implot                      // nolint:STYLE030
require imgui_node_editor           // nolint:STYLE030
require vulkan                      // nolint:STYLE030

// one boost path per external — proves the ADD_MODULE_DAS registrations resolve
require imgui/imgui_boost_v2
require imgui/imgui_theme_daslang   // nolint:STYLE030
require imgui/imgui_implot_boost_v2
require imgui/imgui_node_editor_boost_v2
require vulkan/vulkan_boost         // nolint:STYLE030
require vulkan/spirv_vulkan_shader

[export]
def main {
    print("the fat man stands: imgui + imgui_app(+headless) + implot + node_editor + vulkan all statically linked\n")
}
