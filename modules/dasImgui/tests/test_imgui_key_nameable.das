options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_live_core public
require daslib/enum_trait

[test]
def test_key_range_markers_are_not_nameable(t : T?) {
    // ImGuiKey carries three range markers alongside its keys, and ImGui's
    // GetKeyName asserts on all three. Iterating the enum walks straight into
    // them, so the chord lookup has to skip them by value, not by name.
    t |> success(!imgui_key_is_nameable(ImGuiKey.NamedKey_END),
                 "NamedKey_END is a range marker, not a key")
    t |> success(!imgui_key_is_nameable(ImGuiKey.NamedKey_COUNT),
                 "NamedKey_COUNT is a count, not a key")
    t |> success(!imgui_key_is_nameable(ImGuiKey.ImGuiMod_Mask_),
                 "ImGuiMod_Mask_ is a mask, not a key")
}

[test]
def test_real_keys_and_single_mods_are_nameable(t : T?) {
    t |> success(imgui_key_is_nameable(ImGuiKey.None), "None names itself")
    t |> success(imgui_key_is_nameable(ImGuiKey.Tab), "Tab is a key")
    t |> success(imgui_key_is_nameable(ImGuiKey.NamedKey_BEGIN),
                 "NamedKey_BEGIN aliases Tab and is a real key value")
    t |> success(imgui_key_is_nameable(ImGuiKey.F3), "F3 is a key")
    t |> success(imgui_key_is_nameable(ImGuiKey.Keypad4), "Keypad4 is a key")
    t |> success(imgui_key_is_nameable(ImGuiKey.ImGuiMod_Ctrl), "Ctrl is a single mod")
    t |> success(imgui_key_is_nameable(ImGuiKey.ImGuiMod_Super), "Super is a single mod")
}

[test]
def test_enum_walk_admits_only_nameable_values(t : T?) {
    // The whole enum, the way imgui_key_chord walks it: exactly three members
    // must be refused, and every one refused must be outside the named range.
    var refused = 0
    for (key in type<ImGuiKey>) {
        continue if (imgui_key_is_nameable(key))
        refused++
        let value = int(key)
        t |> success(value < int(ImGuiKey.NamedKey_BEGIN)
                     || value >= int(ImGuiKey.NamedKey_END),
                     "a refused value sits outside the named-key range")
    }
    t |> equal(refused, 3, "three range markers are refused")
}
