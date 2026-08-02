options gen2
options _comment_hygiene = true
options indenting = 4

require dastest/testing_boost public
require imgui/imgui_commands public
require daslib/fio public

[test]
def test_command_invocation_is_input_source_agnostic(t : T?) {
    var registry = CommandRegistry()
    command_define(registry, CommandDefinition(
        id = "file.open", title = "Open...", category = "File"),
        command_binding("file.open", ImGuiKey.O, primary = true))

    t |> success(command_invoke(registry, "file.open", CommandSource.menu),
                 "enabled command accepts a menu invocation")
    var invocation = CommandInvocation()
    t |> success(command_take(registry, invocation), "queued command can be consumed")
    t |> equal(invocation.command_id, "file.open", "semantic command ID is retained")
    t |> equal(invocation.source, CommandSource.menu, "invocation source is telemetry only")

    command_set_enabled(registry, "file.open", false)
    t |> success(!command_invoke(registry, "file.open", CommandSource.live_api),
                 "disabled command rejects every invocation source")
    command_registry_dispose(registry)
}

[test]
def test_command_chords_speak_logical_ctrl(t : T?) {
    // Chords are logical on every platform: primary is the Ctrl bit, secondary
    // the Super bit. ImGui's ConfigMacOSXBehaviors swap inside AddKeyEvent owns
    // the Cmd mapping — a registry-side swap would double-swap on macOS.
    let key = int(ImGuiKey.C)
    let ctrl = 1 << 12
    let shift = 1 << 13
    let alt = 1 << 14
    let super = 1 << 15
    let primary = command_binding("edit.copy", ImGuiKey.C, primary = true,
                                  shift = true, alt = true)
    let secondary = command_binding("edit.copy.secondary", ImGuiKey.C,
                                    secondary = true)

    t |> equal(command_binding_chord(primary),
               key | ctrl | shift | alt,
               "primary is the logical Ctrl bit on every platform")
    t |> equal(command_binding_chord(secondary),
               key | super,
               "secondary is the logical Super bit on every platform")
}

[test]
def test_command_bindings_save_load_and_conflict_roundtrip(t : T?) {
    var temp_error : string
    let temp_dir = create_temp_directory("das_imgui_commands", temp_error)
    t |> success(empty(temp_error) && !empty(temp_dir),
                 "binding test receives a dedicated temporary directory")
    if (!empty(temp_error) || empty(temp_dir)) return

    let config_path = path_join(temp_dir, "bindings.json")
    var registry = CommandRegistry(config_path = config_path)
    command_define(registry, CommandDefinition(
        id = "file.open", title = "Open...", category = "File"),
        command_binding("file.open", ImGuiKey.O, primary = true))
    command_define(registry, CommandDefinition(
        id = "file.save", title = "Save", category = "File"),
        command_binding("file.save", ImGuiKey.S, primary = true))

    command_set_binding(registry,
        command_binding("file.save", ImGuiKey.O, primary = true))
    t |> equal(command_binding_conflict(registry, 1), "file.open",
               "same-scope duplicate chord reports its conflicting command")
    t |> success(command_save_bindings(registry), "edited bindings save")
    t |> success(fexist(config_path), "save creates the explicit config file")

    command_reset_bindings(registry)
    t |> equal(command_binding_chord(registry.bindings[1]),
               command_binding_chord(command_binding("", ImGuiKey.S, primary = true)),
               "reset restores the registered default")
    t |> success(command_load_bindings(registry), "saved bindings load")
    t |> equal(command_binding_chord(registry.bindings[1]),
               command_binding_chord(command_binding("", ImGuiKey.O, primary = true)),
               "load restores the edited chord exactly")

    command_clear_binding(registry, "file.open", CommandScope.application)
    t |> success(command_save_bindings(registry), "explicit unbound choice saves")
    command_reset_bindings(registry)
    t |> success(registry.bindings[0].key != 0, "reset temporarily restores bound default")
    t |> success(command_load_bindings(registry), "config with unbound command reloads")
    t |> equal(registry.bindings[0].key, 0,
               "explicitly cleared binding survives save/load")

    command_registry_dispose(registry)
    rmdir_rec_result(temp_dir)
}

[test]
def test_command_alt_only_binding_reports_its_conflict(t : T?) {
    var registry = CommandRegistry()
    command_define(registry, CommandDefinition(
        id = "view.zoom_in", title = "Zoom in", category = "View"))
    command_define(registry, CommandDefinition(
        id = "edit.increment", title = "Increment", category = "Edit"))
    // the binding editor's "+ alt" on an unbound command: a second chord and
    // no primary at all
    command_set_binding(registry, with_alt_chord(
        CommandBinding(command_id = "view.zoom_in"), ImGuiKey.KeypadAdd))
    command_set_binding(registry, with_alt_chord(
        CommandBinding(command_id = "edit.increment"), ImGuiKey.KeypadAdd))

    t |> equal(registry.bindings[0].key, 0, "an alt-only binding has no primary")
    t |> equal(command_binding_chord(registry.bindings[0]), 0,
               "modifiers without a key are not a chord")
    t |> equal(command_binding_conflict(registry, 0), "edit.increment",
               "an alt-only binding sees the collision from its own row")
    t |> equal(command_binding_conflict(registry, 1), "view.zoom_in",
               "and the colliding command reports it back")

    command_set_binding(registry, with_alt_chord(
        CommandBinding(command_id = "edit.increment"), ImGuiKey.KeypadSubtract))
    t |> equal(command_binding_conflict(registry, 0), "",
               "distinct second chords do not conflict")

    // an alt-only DEFAULT binding must register too, not just captured ones
    command_define(registry, CommandDefinition(
        id = "view.zoom_reset", title = "Zoom reset", category = "View"),
        with_alt_chord(CommandBinding(), ImGuiKey.KeypadMultiply))
    let zoom_reset_row = find_index_if(registry.bindings) $(binding) => binding.command_id == "view.zoom_reset"
    t |> success(zoom_reset_row >= 0,
                 "a default binding with only an alt chord registers")
    command_registry_dispose(registry)
}

[test]
def test_command_load_keeps_defaults_for_absent_alt_chord(t : T?) {
    var temp_error : string
    let temp_dir = create_temp_directory("das_imgui_alt_chord", temp_error)
    t |> success(empty(temp_error) && !empty(temp_dir),
                 "alt-chord test receives a dedicated temporary directory")
    if (!empty(temp_error) || empty(temp_dir)) return

    let config_path = path_join(temp_dir, "bindings.json")
    var registry = CommandRegistry(config_path = config_path)
    command_define(registry, CommandDefinition(
        id = "edit.paste", title = "Paste", category = "Edit"),
        with_alt_chord(command_binding("edit.paste", ImGuiKey.V, primary = true),
                       ImGuiKey.Insert, shift = true))

    // json_boost omits zero-valued @optional fields, so a record without a
    // second chord IS the pre-alt-chord file format
    var legacy = CommandBindingsFile()
    legacy.bindings |> push(command_binding("edit.paste", ImGuiKey.F5))
    t |> success(fwrite(config_path, write_json(JV(legacy))),
                 "a pre-alt-chord config is written")
    delete legacy.bindings

    t |> success(command_load_bindings(registry), "old-format config loads")
    t |> equal(registry.bindings[0].key, int(ImGuiKey.F5),
               "the saved primary chord still wins")
    t |> equal(registry.bindings[0].alt_key, int(ImGuiKey.Insert),
               "an absent second chord keeps the default instead of wiping it")
    t |> success(registry.bindings[0].alt_shift,
                 "the kept default second chord keeps its modifiers")

    command_set_binding(registry, with_alt_chord(
        command_binding("edit.paste", ImGuiKey.V, primary = true),
        ImGuiKey.KeypadEnter))
    t |> success(command_save_bindings(registry), "a rebound second chord saves")
    command_reset_bindings(registry)
    t |> success(command_load_bindings(registry), "the rewritten config loads")
    t |> equal(registry.bindings[0].alt_key, int(ImGuiKey.KeypadEnter),
               "a saved second chord still overrides the default")

    command_registry_dispose(registry)
    rmdir_rec_result(temp_dir)
}
