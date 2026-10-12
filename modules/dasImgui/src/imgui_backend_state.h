#pragma once
namespace das {
    DAS_MOD_API void InstallCoreClipboardBackend();
    DAS_MOD_API ImU32 GetActiveID();
    DAS_MOD_API ImU32 GetTextInputOwner();
    //! Call each frame for the focused editor inside its window, using its current ImGui item ID.
    DAS_MOD_API void SetTextInputCaret(ImU32 owner_id, const ImVec2 & position, float line_height);
}
