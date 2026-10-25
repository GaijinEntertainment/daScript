#pragma once
#ifndef __EMSCRIPTEN__
#include <algorithm>
#include <string>
#include <vector>
#include "imgui_backend_state.h"

// modules/dasImgui/ARCHITECTURE.md#native-composition
struct NativeIme {
    ImGuiContext * owner = nullptr;
    GLFWwindow * window = nullptr;
    GLFWwindow * active = nullptr;
    ImGuiID hook = 0;
    ImGuiID click_hook = 0;
    ImGuiID shutdown_hook = 0;
    bool enabled = true;
    int text_focus = -1;
    ImU32 owner_id = 0;
    ImVec4 cursor_rect = ImVec4(0, 0, 0, 0);
    ImVec4 overlay = ImVec4(0, 0, 0, 0);
    ImVec4 marks = ImVec4(0, 0, 0, 0);
    ImFont * overlay_font = nullptr;
    std::vector<unsigned int> preedit;
    std::vector<int> blocks;
    int focused_block = 0;
    int caret = 0;
    std::vector<ImGuiInputEvent> held;
    ImU32 finished_press = 0;
    double keys_up_since = -1.0;
};

static NativeIme native_ime;

static void native_ime_clear_preedit() {
    native_ime.preedit.clear();
    native_ime.blocks.clear();
    native_ime.focused_block = 0;
    native_ime.caret = 0;
}

// modules/dasGlfw/ARCHITECTURE.md#ime-patch
static void native_ime_cancel() {
    if (native_ime.active && !native_ime.preedit.empty())
        glfwResetPreeditText(native_ime.active);
    native_ime_clear_preedit();
}

static void native_ime_set_text_focus(bool focused) {
    if (native_ime.text_focus == (focused ? 1 : 0)) return;
    native_ime.text_focus = focused ? 1 : 0;
    if (native_ime.active) glfwSetTextInputFocus(native_ime.active, focused ? GLFW_TRUE : GLFW_FALSE);
}

static void native_ime_apply_preedit(int count, const unsigned int * text, int block_count, const int * block_sizes, int focused_block, int caret) {
    native_ime.preedit.assign(text, text + (count > 0 ? count : 0));
    native_ime.blocks.assign(block_sizes, block_sizes + (block_count > 0 ? block_count : 0));
    native_ime.focused_block = focused_block;
    native_ime.caret = ImClamp(caret, 0, (int)native_ime.preedit.size());
}

static void native_ime_preedit(GLFWwindow * window, int count, unsigned int * text, int block_count, int * block_sizes, int focused_block, int caret) {
    if (window != native_ime.active || !native_ime.enabled || native_ime.owner_id == 0) return;
    native_ime_apply_preedit(count, text, block_count, block_sizes, focused_block, caret);
}

static ImFont * native_ime_font(ImGuiContext & context) {
    if (ImFont * font = das::GetTextInputFont(native_ime.owner_id)) return font;
    return context.IO.FontDefault ? context.IO.FontDefault : context.IO.Fonts->Fonts[0];
}

static ImGuiViewport * native_ime_viewport(const ImGuiPlatformImeData & data) {
    ImGuiViewport * viewport = ImGui::FindViewportByID(data.ViewportId);
    return viewport ? viewport : ImGui::GetMainViewport();
}

static void native_ime_draw(ImGuiContext & context, const ImGuiPlatformImeData & data) {
    native_ime.overlay = ImVec4(0, 0, 0, 0);
    native_ime.marks = ImVec4(0, 0, 0, 0);
    native_ime.overlay_font = nullptr;
    if (native_ime.preedit.empty() || !data.WantVisible || data.InputLineHeight <= 0.0f) return;
    ImGuiViewport * viewport = native_ime_viewport(data);
    ImFont * font = native_ime_font(context);
    const float size = data.InputLineHeight;
    std::string utf8;
    std::vector<int> offsets;
    for (unsigned int cp : native_ime.preedit) {
        offsets.push_back((int)utf8.size());
        char buf[5];
        utf8.append(buf, ImTextCharToUtf8(buf, cp));
    }
    offsets.push_back((int)utf8.size());
    const char * text = utf8.c_str();
    auto advance = [&](int from, int to) {
        return font->CalcTextSizeA(size, FLT_MAX, 0.0f, text + offsets[from], text + offsets[to]).x;
    };
    const ImVec2 origin(data.InputPos.x + 1.0f, data.InputPos.y);
    const float width = advance(0, (int)native_ime.preedit.size());
    const float limit = viewport->Pos.x + viewport->Size.x;
    const ImVec2 pos(ImMax(viewport->Pos.x, ImMin(origin.x, limit - width - 2.0f)), origin.y);
    const ImVec2 end(pos.x + width, pos.y + size);
    const ImGuiStyle & style = context.Style;
    ImDrawList * draw = ImGui::GetForegroundDrawList(viewport);
    draw->AddRectFilled(ImVec2(pos.x - 1.0f, pos.y), ImVec2(end.x + 2.0f, end.y), ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_FrameBg]) | IM_COL32_A_MASK);
    const ImU32 color = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
    draw->AddText(font, size, pos, color, text, text + utf8.size());
    int start = 0;
    int underlines = 0;
    float focused_thickness = 0.0f;
    const int total = (int)native_ime.preedit.size();
    const size_t block_total = native_ime.blocks.empty() ? 1 : native_ime.blocks.size();
    for (size_t block = 0; block < block_total && start < total; ++block) {
        const int length = native_ime.blocks.empty() ? total : native_ime.blocks[block];
        const int stop = ImClamp(start + length, start, total);
        const float x0 = pos.x + advance(0, start) + 1.0f;
        const float x1 = pos.x + advance(0, stop) - 1.0f;
        const float thickness = (int)block == native_ime.focused_block && block_total > 1 ? 2.0f : 1.0f;
        draw->AddLine(ImVec2(x0, end.y - thickness), ImVec2(x1, end.y - thickness), color, thickness);
        underlines++;
        if ((int)block == native_ime.focused_block) focused_thickness = thickness;
        start = stop;
    }
    const float caret_x = pos.x + advance(0, native_ime.caret);
    draw->AddLine(ImVec2(caret_x, pos.y), ImVec2(caret_x, end.y - 1.0f), ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_InputTextCursor]), 1.0f);
    native_ime.overlay = ImVec4(pos.x, pos.y, end.x, end.y);
    native_ime.marks = ImVec4(caret_x, (float)underlines, focused_thickness, 0.0f);
    native_ime.overlay_font = font;
}

static bool native_ime_keys_settled(const ImGuiContext & context) {
    for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; ++key) {
        if (ImGui::IsKeyDown((ImGuiKey)key)) {
            native_ime.keys_up_since = -1.0;
            return false;
        }
    }
    if (native_ime.keys_up_since < 0.0) native_ime.keys_up_since = context.Time;
    return context.Time - native_ime.keys_up_since >= 0.1;
}

static GLFWwindow * native_ime_viewport_window(const ImGuiPlatformImeData & data) {
    ImGuiViewport * viewport = native_ime_viewport(data);
    return viewport->PlatformHandle ? (GLFWwindow *)viewport->PlatformHandle : native_ime.window;
}

static void native_ime_switch_window(GLFWwindow * window) {
    native_ime.active = window;
    native_ime.text_focus = -1;
    native_ime.cursor_rect = ImVec4(0, 0, 0, 0);
    if (window) glfwSetPreeditCallback(window, native_ime_preedit);
}

static void native_ime_drop_closed_window() {
    if (native_ime.active == native_ime.window) return;
    for (ImGuiViewport * viewport : ImGui::GetPlatformIO().Viewports) {
        if (viewport->PlatformHandle == native_ime.active) return;
    }
    native_ime_clear_preedit();
    native_ime_switch_window(native_ime.window);
}

static void native_ime_follow(GLFWwindow * window) {
    if (!native_ime.window || window == native_ime.active) return;
    native_ime_cancel();
    native_ime_set_text_focus(false);
    glfwSetPreeditCallback(native_ime.active, nullptr);
    native_ime_switch_window(window);
}

static void native_ime_frame(ImGuiContext * context, ImGuiContextHook *) {
    if (context != native_ime.owner) return;
    const ImGuiPlatformImeData & data = context->PlatformImeData;
    const ImU32 owner = data.WantTextInput ? das::GetTextInputOwner() : 0;
    native_ime_drop_closed_window();
    if (owner != native_ime.owner_id) native_ime_cancel();
    if (owner != 0) native_ime_follow(native_ime_viewport_window(data));
    native_ime.owner_id = owner;
    const bool focused = owner != 0 && native_ime.enabled;
    const bool settled = native_ime_keys_settled(*context);
    if (focused || settled) native_ime_set_text_focus(focused);
    if (owner != 0) {
        ImGuiViewport * viewport = native_ime_viewport(data);
        const ImVec4 rect(IM_TRUNC(data.InputPos.x - viewport->Pos.x), IM_TRUNC(data.InputPos.y - viewport->Pos.y), 1.0f, IM_TRUNC(data.InputLineHeight));
        if (memcmp(&rect, &native_ime.cursor_rect, sizeof(rect)) != 0) {
            native_ime.cursor_rect = rect;
            if (native_ime.active) glfwSetPreeditCursorRectangle(native_ime.active, (int)rect.x, (int)rect.y, (int)rect.z, (int)rect.w);
        }
    }
    native_ime_draw(*context, data);
}

static int native_ime_first_press(const ImVector<ImGuiInputEvent> & queue) {
    for (int i = 0; i < queue.Size; ++i) {
        if (queue[i].Type == ImGuiInputEventType_MouseButton && queue[i].MouseButton.Down) return i;
    }
    return -1;
}

static void native_ime_release_held(ImGuiContext & context) {
    ImVector<ImGuiInputEvent> arrived;
    arrived.swap(context.InputEventsQueue);
    auto & queue = context.InputEventsQueue;
    for (const auto & event : arrived) {
        if (event.Type == ImGuiInputEventType_Text) queue.push_back(event);
    }
    for (const auto & event : native_ime.held) queue.push_back(event);
    for (const auto & event : arrived) {
        if (event.Type != ImGuiInputEventType_Text) queue.push_back(event);
    }
    native_ime.held.clear();
}

static void native_ime_commit_before_click(ImGuiContext * context, ImGuiContextHook *) {
    if (context != native_ime.owner) return;
    if (!native_ime.held.empty()) {
        native_ime_release_held(*context);
        return;
    }
    if (!native_ime.enabled || native_ime.owner_id == 0) return;
    auto & queue = context->InputEventsQueue;
    const int press = native_ime_first_press(queue);
    if (press < 0 || queue[press].EventId == native_ime.finished_press) return;
    native_ime.finished_press = queue[press].EventId;
    if (!native_ime.preedit.empty()) {
        const int start = queue.Size;
        for (unsigned int cp : native_ime.preedit) context->IO.AddInputCharacter(cp);
        native_ime_cancel();
        std::rotate(queue.begin() + press, queue.begin() + start, queue.end());
    } else if (native_ime.active && glfwGetPlatform() == GLFW_PLATFORM_X11) {
        glfwResetPreeditText(native_ime.active);
        native_ime.held.assign(queue.begin() + press, queue.end());
        queue.resize(press);
    }
}

static void native_ime_detach() {
    if (!native_ime.owner) return;
    native_ime_cancel();
    if (native_ime.active) {
        native_ime_set_text_focus(true);
        glfwSetPreeditCallback(native_ime.active, nullptr);
    }
    if (native_ime.hook) ImGui::RemoveContextHook(native_ime.owner, native_ime.hook);
    if (native_ime.click_hook) ImGui::RemoveContextHook(native_ime.owner, native_ime.click_hook);
    if (native_ime.shutdown_hook) ImGui::RemoveContextHook(native_ime.owner, native_ime.shutdown_hook);
    native_ime = NativeIme();
}

static void native_ime_context_shutdown(ImGuiContext * context, ImGuiContextHook *) {
    if (context == native_ime.owner) native_ime = NativeIme();
}

static void native_ime_attach(GLFWwindow * window) {
    ImGuiContext * context = ImGui::GetCurrentContext();
    if (!context) return;
    if (native_ime.owner == context && native_ime.window == window) return;
    native_ime_detach();
    native_ime.owner = context;
    native_ime.window = window;
    native_ime.active = window;
    ImGuiContextHook frame;
    frame.Type = ImGuiContextHookType_EndFramePre;
    frame.Callback = native_ime_frame;
    native_ime.hook = ImGui::AddContextHook(context, &frame);
    ImGuiContextHook click;
    click.Type = ImGuiContextHookType_NewFramePre;
    click.Callback = native_ime_commit_before_click;
    native_ime.click_hook = ImGui::AddContextHook(context, &click);
    ImGuiContextHook shutdown;
    shutdown.Type = ImGuiContextHookType_Shutdown;
    shutdown.Callback = native_ime_context_shutdown;
    native_ime.shutdown_hook = ImGui::AddContextHook(context, &shutdown);
    if (window) {
        ImGui::GetPlatformIO().Platform_SetImeDataFn = nullptr;
        glfwSetPreeditCallback(window, native_ime_preedit);
        native_ime_set_text_focus(false);
    }
}

static void native_ime_set_enabled(bool enabled) {
    if (ImGui::GetCurrentContext() != native_ime.owner || native_ime.enabled == enabled) return;
    native_ime.enabled = enabled;
    if (!enabled) native_ime_cancel();
    native_ime_set_text_focus(enabled && native_ime.owner_id != 0);
}
#endif
