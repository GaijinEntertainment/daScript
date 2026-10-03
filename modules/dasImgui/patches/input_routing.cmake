# Keep local input fixes explicit and fail on upstream source drift.
FUNCTION(das_imgui_patch_input filename before after)
    SET(path "${DAS_IMGUI_DIR}/imgui/${filename}")
    FILE(READ "${path}" source)
    STRING(FIND "${source}" "${after}" applied)
    IF(NOT applied EQUAL -1)
        RETURN()
    ENDIF()
    STRING(FIND "${source}" "${before}" found)
    IF(found EQUAL -1)
        MESSAGE(FATAL_ERROR "ImGui ${filename} input routing changed; review the local input patch")
    ENDIF()
    STRING(REPLACE "${before}" "${after}" source "${source}")
    FILE(WRITE "${path}" "${source}")
ENDFUNCTION()

# Exact modifier routing otherwise ignores Shift+Enter and Shift+KeypadEnter.
das_imgui_patch_input(imgui_widgets.cpp
    "const bool is_enter = Shortcut(ImGuiKey_Enter, f_repeat, id) || Shortcut(ImGuiKey_KeypadEnter, f_repeat, id);"
    "const bool is_enter = Shortcut(ImGuiKey_Enter, f_repeat, id) || Shortcut(ImGuiKey_KeypadEnter, f_repeat, id) || Shortcut(ImGuiMod_Shift | ImGuiKey_Enter, f_repeat, id) || Shortcut(ImGuiMod_Shift | ImGuiKey_KeypadEnter, f_repeat, id);")

# Finish queued text in its current editor before a mouse button changes focus.
das_imgui_patch_input(imgui.cpp
    "if (trickle_fast_inputs && ((mouse_button_changed & (1 << button)) || mouse_wheeled))"
    "if (trickle_fast_inputs && ((mouse_button_changed & (1 << button)) || mouse_wheeled || text_inputted))")
