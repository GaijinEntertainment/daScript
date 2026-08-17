options gen2
options gc
options persistent_heap
options no_unused_function_arguments = false

module rich_commands

require imgui/imgui_harness
require imgui/imgui_commands public
require daslib/enum_trait
require daslib/strings_boost
require ./rich_state.das

//! dasHerd's named-command layer (note 23) over imgui/imgui_commands: one
//! registry drives the menus, ImGui-routed keyboard chords, the Settings >
//! Shortcuts editor, and the herder_command live rail.

var g_command_registry = CommandRegistry()
var g_command_actions : table<string; function<void>>

def register_herd_command(id, category, title : string;
                          binding : CommandBinding; action : function<void>) {
    command_define(g_command_registry, CommandDefinition(id = id, title = title,
        category = category), binding)
    g_command_actions[id] = action
}

def herd_commands_ready() {
    // After registration: overlay persisted bindings on the defaults. A load
    // marks the registry saved, so only user edits trigger the write-through.
    g_command_registry.config_path = command_default_config_path("dasHerd")
    let _ = command_load_bindings(g_command_registry)
    g_command_registry.status = ""
    g_command_registry.saved_revision = g_command_registry.revision
}

def herd_commands_poll() {
    command_capture_tick(g_command_registry)
    // The terminal owns the keyboard outright while focused (chords may be
    // PTY control bytes); text fields own it while typing.
    let io & = unsafe(GetIO())
    if (!g_terminal_view.focused && !io.WantTextInput) {
        command_poll_scope(g_command_registry, CommandScope.application)
    }
}

def herd_commands_drain() {
    var invocation = CommandInvocation()
    while (command_take(g_command_registry, invocation)) {
        if (key_exists(g_command_actions, invocation.command_id)) {
            invoke(g_command_actions[invocation.command_id])
        }
    }
    // Write-through (note 24): any binding edit persists immediately.
    if (g_command_registry.revision != g_command_registry.saved_revision) {
        let _ = command_save_bindings(g_command_registry)
    }
}

def herd_commands_dispose() {
    command_registry_dispose(g_command_registry)
    delete g_command_actions
}

def run_herd_command(id : string;
                     source : CommandSource = CommandSource.programmatic) : bool {
    return command_invoke(g_command_registry, id, source)
}

def herd_command_shortcut(id : string) : string {
    return command_shortcut_text(g_command_registry, id)
}

def herd_command_definition_scope(id : string; var scope : CommandScope&) : bool {
    for (definition in g_command_registry.definitions) {
        if (definition.id == id) {
            scope = definition.scope
            return true
        }
    }
    return false
}

def herd_command_conflict(id : string) : string {
    //! Command id of another binding sharing this command's chord, or "".
    for (index, binding in count(), g_command_registry.bindings) {
        if (binding.command_id == id) {
            return command_binding_conflict(g_command_registry, index)
        }
    }
    return ""
}

def herd_command_set_shortcut(id, chord : string) : tuple<ok : bool; message : string> {
    //! Rail-facing chord assignment. `chord` uses command_binding_text's own
    //! spelling ("Ctrl+Shift+N", key names per ImGui GetKeyName); "" unbinds.
    var scope = CommandScope.application
    if (!herd_command_definition_scope(id, scope)) {
        return (ok = false, message = "unknown command {id}")
    }
    if (empty(chord)) {
        command_clear_binding(g_command_registry, id, scope)
        return (ok = true, message = "")
    }
    var primary = false
    var shift = false
    var alt = false
    var secondary = false
    var key_name = ""
    for (part in split(chord, "+")) {
        if (part == "Ctrl" || part == "Cmd") { primary = true; }
        elif (part == "Shift") { shift = true; }
        elif (part == "Alt") { alt = true; }
        elif (part == "Super") { secondary = true; }
        elif (empty(key_name)) { key_name = part; }
        else { return (ok = false, message = "can't parse chord {chord}"); }
    }
    if (empty(key_name)) return (ok = false, message = "chord {chord} has no key")
    for (key in type<ImGuiKey>) {
        if ("{GetKeyName(key)}" == key_name) {
            command_set_binding(g_command_registry,
                command_binding(id, key, primary, shift, alt, secondary, scope))
            return (ok = true, message = "")
        }
    }
    return (ok = false, message = "unknown key {key_name}")
}
