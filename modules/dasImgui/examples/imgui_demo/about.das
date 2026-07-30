options gen2
options _comment_hygiene = true
options indenting = 4

module about

require imgui
require imgui/imgui_widgets_builtin
require imgui/imgui_containers_builtin
require daslib/safe_addr

// Module-scope state — replaces the C++ `static bool show_config_info`.
var ABOUT_SHOW_CONFIG_INFO = false
var private BUTTON_COPY : ClickState

// Mirrors imgui_demo.cpp:6460-6622 — ImGui::ShowAboutWindow.
// The ~50 C++ `#ifdef DEFINE_NAME` build-define Text() lines have no
// daslang-hosted equivalent and are elided; clipboard log path preserved.
def ShowAboutWindow() {
    window(ABOUT_WINDOW, (text = "About Dear ImGui", closable = false,
                          flags = ImGuiWindowFlags.AlwaysAutoResize)) {
        text("Dear ImGui {IMGUI_VERSION} ({IMGUI_VERSION_NUM})")
        separator(ABOUT_SEP_1)
        text("By Omar Cornut and all Dear ImGui contributors.")
        text((text = "Dear ImGui is licensed under the MIT License, " +
                     "see LICENSE for more information."))
        text("If your company uses this, please consider funding the project.")

        edit_checkbox(safe_addr(ABOUT_SHOW_CONFIG_INFO),
            (id = "ABOUT_SHOW_CONFIG_INFO_CB", text = "Config/Build Information"))
        if (ABOUT_SHOW_CONFIG_INFO) {
            let io & = unsafe(GetIO())
            let style & = unsafe(GetStyle())

            let copy_to_clipboard = button(BUTTON_COPY, (text = "Copy to clipboard"))
            let child_size = float2(0.0f, GetTextLineHeightWithSpacing() * 18.0f)
            child(ABOUT_CFG_INFOS_CHILD,
                (text = "cfg_infos", size = child_size,
                 child_flags = ImGuiChildFlags.FrameStyle,
                 window_flags = ImGuiWindowFlags.None)) {
                if (copy_to_clipboard) {
                    log_to_clipboard()
                    // Back-quotes wrap the dump so a GitHub-pasted version renders
                    // as a code block.
                    log_text("```\n")
                }

                text("Dear ImGui {IMGUI_VERSION} ({IMGUI_VERSION_NUM})")
                separator(ABOUT_SEP_2)
                text("define: __cplusplus elided (daslang port)")
                text("(C++ build-define block elided; see imgui_demo.cpp:6494-6565)")
                separator(ABOUT_SEP_3)
                text("io.BackendPlatformName: {io.BackendPlatformName}")
                text("io.BackendRendererName: {io.BackendRendererName}")
                text("io.ConfigFlags: {fmt_hex(uint(io.ConfigFlags))}")
                if (uint(io.ConfigFlags & ImGuiConfigFlags.NavEnableKeyboard) != 0u) {
                    text(" NavEnableKeyboard")
                }
                if (uint(io.ConfigFlags & ImGuiConfigFlags.NavEnableGamepad) != 0u) {
                    text(" NavEnableGamepad")
                }
                if (io.ConfigNavMoveSetMousePos) {
                    text(" ConfigNavMoveSetMousePos")
                }
                if (!io.ConfigNavCaptureKeyboard) {
                    text(" NavNoCaptureKeyboard")
                }
                if (uint(io.ConfigFlags & ImGuiConfigFlags.NoMouse) != 0u) {
                    text(" NoMouse")
                }
                if (uint(io.ConfigFlags & ImGuiConfigFlags.NoMouseCursorChange) != 0u) {
                    text(" NoMouseCursorChange")
                }
                if (uint(io.ConfigFlags & ImGuiConfigFlags.DockingEnable) != 0u) {
                    text(" DockingEnable")
                }
                if (uint(io.ConfigFlags & ImGuiConfigFlags.ViewportsEnable) != 0u) {
                    text(" ViewportsEnable")
                }
                if (io.MouseDrawCursor) {
                    text("io.MouseDrawCursor")
                }
                if (io.ConfigViewportsNoAutoMerge) {
                    text("io.ConfigViewportsNoAutoMerge")
                }
                if (io.ConfigViewportsNoTaskBarIcon) {
                    text("io.ConfigViewportsNoTaskBarIcon")
                }
                if (io.ConfigDockingNoSplit) {
                    text("io.ConfigDockingNoSplit")
                }
                if (io.ConfigDockingWithShift) {
                    text("io.ConfigDockingWithShift")
                }
                if (io.ConfigDockingAlwaysTabBar) {
                    text("io.ConfigDockingAlwaysTabBar")
                }
                if (io.ConfigMacOSXBehaviors) {
                    text("io.ConfigMacOSXBehaviors")
                }
                if (io.ConfigInputTextCursorBlink) {
                    text("io.ConfigInputTextCursorBlink")
                }
                text("io.BackendFlags: {fmt_hex(uint(io.BackendFlags))}")
                if (uint(io.BackendFlags & ImGuiBackendFlags.HasGamepad) != 0u) {
                    text(" HasGamepad")
                }
                if (uint(io.BackendFlags & ImGuiBackendFlags.HasMouseCursors) != 0u) {
                    text(" HasMouseCursors")
                }
                if (uint(io.BackendFlags & ImGuiBackendFlags.HasSetMousePos) != 0u) {
                    text(" HasSetMousePos")
                }
                if (uint(io.BackendFlags & ImGuiBackendFlags.PlatformHasViewports) != 0u) {
                    text(" PlatformHasViewports")
                }
                if (uint(io.BackendFlags & ImGuiBackendFlags.RendererHasVtxOffset) != 0u) {
                    text(" RendererHasVtxOffset")
                }
                if (uint(io.BackendFlags & ImGuiBackendFlags.RendererHasViewports) != 0u) {
                    text(" RendererHasViewports")
                }
                separator(ABOUT_SEP_4)
                text("io.DisplaySize: {io.DisplaySize.x},{io.DisplaySize.y}")
                text("io.DisplayFramebufferScale: {io.DisplayFramebufferScale.x},{io.DisplayFramebufferScale.y}")
                separator(ABOUT_SEP_5)
                text("style.WindowPadding: {style.WindowPadding.x},{style.WindowPadding.y}")
                text("style.WindowBorderSize: {style.WindowBorderSize}")
                text("style.FramePadding: {style.FramePadding.x},{style.FramePadding.y}")
                text("style.FrameRounding: {style.FrameRounding}")
                text("style.FrameBorderSize: {style.FrameBorderSize}")
                text("style.ItemSpacing: {style.ItemSpacing.x},{style.ItemSpacing.y}")
                text("style.ItemInnerSpacing: {style.ItemInnerSpacing.x},{style.ItemInnerSpacing.y}")

                if (copy_to_clipboard) {
                    log_text("\n```\n")
                    log_finish()
                }
            }
        }
    }
}

def private fmt_hex(v : uint) : string {
    return "0x{v}"
}
