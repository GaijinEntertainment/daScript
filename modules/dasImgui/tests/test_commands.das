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
def test_command_primary_modifier_follows_platform_policy(t : T?) {
    let macos = get_platform_name() == "darwin"
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
               key | (macos ? super : ctrl) | shift | alt,
               "primary follows the current platform modifier policy")
    t |> equal(command_binding_chord(secondary),
               key | (macos ? ctrl : super),
               "secondary uses the complementary platform modifier")
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
