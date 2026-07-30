options gen2
options _comment_hygiene = true
options indenting = 4
options _allow_imgui_legacy = true

module imgui_commands public

require imgui public
require daslib/enum_trait
require daslib/fio public
require daslib/json public
require daslib/json_boost public
require daslib/safe_addr
require strings

//! Semantic application commands with editable ImGui keyboard bindings.
//!
//! Components expose operations; applications map stable command IDs onto
//! those operations. Keyboard chords, menus, toolbars, live APIs and command
//! palettes all enqueue the same CommandInvocation.

enum CommandScope {
    application
    document
    component
}

enum CommandSource {
    programmatic
    keyboard
    menu
    toolbar
    live_api
}

struct CommandDefinition {
    id : string
    title : string
    category : string
    description : string
    icon : string
    scope : CommandScope = CommandScope.application
    enabled : bool = true
    selected : bool = false
}

struct CommandBinding {
    command_id : string
    scope : CommandScope = CommandScope.application
    key : int
    primary : bool = false
    shift : bool = false
    alt : bool = false
    secondary : bool = false
    repeat : bool = false
}

struct CommandInvocation {
    command_id : string
    source : CommandSource = CommandSource.programmatic
    context : string
}

struct CommandBindingsFile {
    version : int = 1
    bindings : array<CommandBinding>
}

struct CommandRegistry {
    definitions : array<CommandDefinition>
    default_bindings : array<CommandBinding>
    bindings : array<CommandBinding>
    invocations : array<CommandInvocation>
    config_path : string
    status : string
    revision : int = 0
    saved_revision : int = 0
    capture_command_id : string
}

struct CommandBindingsEditorState {
    open : bool = false
}

let COMMAND_ROUTE_GLOBAL = 1 << 12
let COMMAND_REPEAT = 1 << 0
let COMMAND_MOD_CTRL = 1 << 12
let COMMAND_MOD_SHIFT = 1 << 13
let COMMAND_MOD_ALT = 1 << 14
let COMMAND_MOD_SUPER = 1 << 15

def public command_binding(command_id : string; key : ImGuiKey;
                           primary : bool = false; shift : bool = false;
                           alt : bool = false; secondary : bool = false;
                           scope : CommandScope = CommandScope.application;
                           repeat : bool = false) : CommandBinding {
    return CommandBinding(command_id = command_id, scope = scope, key = int(key),
                          primary = primary, shift = shift, alt = alt,
                          secondary = secondary, repeat = repeat)
}

def private definition_index(registry : CommandRegistry const; id : string) : int {
    return find_index_if(registry.definitions) $(definition) => definition.id == id
}

def private binding_index(registry : CommandRegistry const; id : string;
                          scope : CommandScope) : int {
    return find_index_if(registry.bindings) $(binding) => binding.command_id == id && binding.scope == scope
}

def public command_define(var registry : CommandRegistry;
                          definition : CommandDefinition const;
                          default_binding : CommandBinding = CommandBinding()) {
    if (definition_index(registry, definition.id) >= 0) return
    registry.definitions |> push_clone(definition)
    if (default_binding.key != 0) {
        var binding := default_binding
        binding.command_id := definition.id
        binding.scope = definition.scope
        registry.default_bindings |> push_clone(binding)
        registry.bindings |> push_clone(binding)
    }
}

def public command_set_enabled(var registry : CommandRegistry; id : string;
                               enabled : bool) {
    let index = definition_index(registry, id)
    if (index >= 0) {
        registry.definitions[index].enabled = enabled
    }
}

def public command_set_selected(var registry : CommandRegistry; id : string;
                                selected : bool) {
    let index = definition_index(registry, id)
    if (index >= 0) {
        registry.definitions[index].selected = selected
    }
}

def public command_enabled(registry : CommandRegistry const; id : string) : bool {
    let index = definition_index(registry, id)
    return index >= 0 && registry.definitions[index].enabled
}

def public command_begin_frame(var registry : CommandRegistry) {
    // Deliberately retain queued invocations. Live/API producers may enqueue
    // between rendered frames; command_take drains them after UI submission.
}

def public command_invoke(var registry : CommandRegistry; id : string;
                          source : CommandSource = CommandSource.programmatic;
                          context : string = "") : bool {
    if (!command_enabled(registry, id)) return false
    registry.invocations |> push(CommandInvocation(
        command_id = id, source = source, context = context))
    return true
}

def public command_take(var registry : CommandRegistry;
                        var invocation : CommandInvocation) : bool {
    if (length(registry.invocations) == 0) return false
    invocation <- registry.invocations[0]
    registry.invocations |> erase(0)
    return true
}

def private command_uses_macos_behaviors() : bool {
    if (GetCurrentContext() != null) {
        return unsafe(GetIO()).ConfigMacOSXBehaviors
    }
    return get_platform_name() == "darwin"
}

def public command_binding_chord(binding : CommandBinding const) : int {
    var chord = binding.key
    let macos = command_uses_macos_behaviors()
    if (binding.primary) {
        chord = chord | (macos ? COMMAND_MOD_SUPER : COMMAND_MOD_CTRL)
    }
    if (binding.shift) { chord = chord | COMMAND_MOD_SHIFT }
    if (binding.alt) { chord = chord | COMMAND_MOD_ALT }
    if (binding.secondary) {
        chord = chord | (macos ? COMMAND_MOD_CTRL : COMMAND_MOD_SUPER)
    }
    return chord
}

def private primary_name() : string {
    let io & = unsafe(GetIO())
    return io.ConfigMacOSXBehaviors ? "Cmd" : "Ctrl"
}

def private secondary_name() : string {
    let io & = unsafe(GetIO())
    return io.ConfigMacOSXBehaviors ? "Ctrl" : "Super"
}

def public command_binding_text(binding : CommandBinding const) : string {
    if (binding.key == 0) return ""
    var value : string
    if (binding.primary) { value += primary_name() + "+" }
    if (binding.shift) { value += "Shift+" }
    if (binding.alt) { value += "Alt+" }
    if (binding.secondary) { value += secondary_name() + "+" }
    value += "{GetKeyName(unsafe(reinterpret<ImGuiKey>(binding.key)))}"
    return value
}

def public command_shortcut_text(registry : CommandRegistry const;
                                 id : string) : string {
    let definition = definition_index(registry, id)
    if (definition < 0) return ""
    let index = binding_index(registry, id, registry.definitions[definition].scope)
    return index >= 0 ? command_binding_text(registry.bindings[index]) : ""
}

def public command_poll_scope(var registry : CommandRegistry;
                              scope : CommandScope; context : string = "") {
    if (!empty(registry.capture_command_id)) return
    for (binding in registry.bindings) {
        if (binding.scope != scope || binding.key == 0
                || !command_enabled(registry, binding.command_id)) continue
        let flags = (scope == CommandScope.application ? COMMAND_ROUTE_GLOBAL : 0) | (binding.repeat ? COMMAND_REPEAT : 0)
        if (Shortcut(command_binding_chord(binding), flags)) {
            let _ = command_invoke(registry, binding.command_id,
                                   CommandSource.keyboard, context)
        }
    }
}

def public command_reset_bindings(var registry : CommandRegistry) {
    delete registry.bindings
    for (binding in registry.default_bindings) {
        registry.bindings |> push_clone(binding)
    }
    registry.revision++
    registry.status = "Restored default bindings."
}

def public command_set_binding(var registry : CommandRegistry;
                               binding : CommandBinding const) {
    let index = binding_index(registry, binding.command_id, binding.scope)
    if (index >= 0) {
        registry.bindings[index] = binding
    } else {
        registry.bindings |> push_clone(binding)
    }
    registry.revision++
}

def public command_clear_binding(var registry : CommandRegistry; id : string;
                                 scope : CommandScope) {
    let index = binding_index(registry, id, scope)
    if (index >= 0) {
        registry.bindings[index].key = 0
        registry.bindings[index].primary = false
        registry.bindings[index].shift = false
        registry.bindings[index].alt = false
        registry.bindings[index].secondary = false
    } else {
        registry.bindings |> push(CommandBinding(command_id = id, scope = scope))
    }
    registry.revision++
}

def public command_binding_conflict(registry : CommandRegistry const;
                                    binding_index_ : int) : string {
    if (binding_index_ < 0 || binding_index_ >= length(registry.bindings)) return ""
    let binding = registry.bindings[binding_index_]
    if (binding.key == 0) return ""
    let chord = command_binding_chord(binding)
    for (index in range(length(registry.bindings))) {
        if (index == binding_index_) continue
        let other = registry.bindings[index]
        if (other.scope == binding.scope
                && command_binding_chord(other) == chord) return other.command_id
    }
    return ""
}

def public command_save_bindings(var registry : CommandRegistry) : bool {
    if (empty(registry.config_path)) {
        registry.status = "No bindings config path is set."
        return false
    }
    var error : string
    if (!mkdir_rec(dir_name(registry.config_path), error)) {
        registry.status = "Could not create bindings directory: {error}"
        return false
    }
    var config = CommandBindingsFile()
    for (binding in registry.bindings) {
        config.bindings |> push_clone(binding)
    }
    let ok = fwrite(registry.config_path, write_json(JV(config)))
    delete config.bindings
    if (!ok) {
        registry.status = "Could not save bindings: {registry.config_path}"
        return false
    }
    registry.saved_revision = registry.revision
    registry.status = "Saved bindings to {registry.config_path}"
    return true
}

def public command_load_bindings(var registry : CommandRegistry) : bool {
    if (empty(registry.config_path) || !fexist(registry.config_path)) {
        registry.status = "Bindings config does not exist; using defaults."
        return false
    }
    var error : string
    let root = read_json(fread(registry.config_path), error)
    if (root == null || !empty(error)) {
        registry.status = "Could not parse bindings config: {error}"
        return false
    }
    var config <- from_JV(root, type<CommandBindingsFile>)
    if (config.version != 1) {
        registry.status = "Unsupported bindings config version: {config.version}"
        delete config.bindings
        return false
    }
    // Overlay saved choices on current defaults. This retains defaults for
    // commands introduced after the config was written, while key=0 records
    // an explicit user choice to leave an existing command unbound.
    command_reset_bindings(registry)
    for (binding in config.bindings) {
        if (definition_index(registry, binding.command_id) >= 0) {
            command_set_binding(registry, binding)
        }
    }
    delete config.bindings
    registry.revision++
    registry.saved_revision = registry.revision
    registry.status = "Loaded bindings from {registry.config_path}"
    return true
}

def public command_default_config_path(app_name : string) : string {
    var base : string
    if (has_env_variable("APPDATA")) {
        base = get_env_variable("APPDATA")
    } elif (has_env_variable("XDG_CONFIG_HOME")) {
        base = get_env_variable("XDG_CONFIG_HOME")
    } elif (has_env_variable("HOME")) {
        base = path_join(get_env_variable("HOME"), ".config")
    } else {
        base = "."
    }
    return path_join(path_join(base, "daScript"), "{app_name}-bindings.json")
}

def public command_capture_tick(var registry : CommandRegistry) {
    //! Advance an armed binding capture (``registry.capture_command_id``):
    //! Escape cancels, the first pressed non-modifier key plus current
    //! modifiers becomes the binding. Call once per frame; the built-in
    //! editor calls it for you, custom binding UIs call it directly.
    if (empty(registry.capture_command_id)) return
    if (IsKeyPressed(ImGuiKey.Escape, false)) {
        registry.capture_command_id = ""
        registry.status = "Binding capture cancelled."
        return
    }
    let definition = definition_index(registry, registry.capture_command_id)
    if (definition < 0) {
        registry.capture_command_id = ""
        return
    }
    let io & = unsafe(GetIO())
    for (key in type<ImGuiKey>) {
        // Skip the side-modifier keys themselves (LeftCtrl..RightSuper are
        // contiguous): a chord's modifiers press first, and binding on their
        // own down-frame would capture "Ctrl+LeftCtrl" before the real key
        // arrives. Modifiers are read from io.Key* when the non-modifier
        // key fires.
        if (int(key) >= int(ImGuiKey.LeftCtrl)
                && int(key) <= int(ImGuiKey.RightSuper)) continue
        let name = "{GetKeyName(key)}"
        if (empty(name) || starts_with(name, "Mod") || starts_with(name, "Mouse")
                || starts_with(name, "Gamepad") || !IsKeyPressed(key, false)) continue
        let primary_down = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl
        let secondary_down = io.ConfigMacOSXBehaviors ? io.KeyCtrl : io.KeySuper
        let id = registry.capture_command_id
        command_set_binding(registry, command_binding(id, key,
            primary_down, io.KeyShift, io.KeyAlt, secondary_down,
            registry.definitions[definition].scope))
        registry.capture_command_id = ""
        registry.status = "Bound {id} to {command_shortcut_text(registry, id)}"
        return
    }
}

def public command_bindings_editor(var registry : CommandRegistry;
                                   var state : CommandBindingsEditorState) {
    if (!state.open) return
    var open = state.open
    var p_open : bool?
    unsafe { p_open = addr(open) }
    let visible = Begin("Edit bindings###dasimgui_command_bindings", p_open,
                        ImGuiWindowFlags.NoCollapse)
    state.open = open
    if (visible) {
        TextUnformatted("Config: {registry.config_path}")
        if (Button("Load##command_bindings")) {
            let _ = command_load_bindings(registry)
        }
        SameLine()
        if (Button("Save##command_bindings")) {
            let _ = command_save_bindings(registry)
        }
        SameLine()
        if (Button("Reset defaults##command_bindings")) {
            command_reset_bindings(registry)
        }
        if (!empty(registry.status)) TextDisabled(registry.status)
        Separator()
        TextDisabled("Click a binding, then press a key chord. Escape cancels.")
        BeginChild("command_binding_rows", ImVec2(0.0f, 0.0f), ImGuiChildFlags.None,
                   ImGuiWindowFlags.HorizontalScrollbar)
        for (definition_index_ in range(length(registry.definitions))) {
            let definition = registry.definitions[definition_index_]
            let index = binding_index(registry, definition.id, definition.scope)
            let current_text = index >= 0 ? command_binding_text(registry.bindings[index]) : ""
            let binding_text = empty(current_text) ? "Unbound" : current_text
            PushID(definition_index_)
            TextUnformatted("{definition.category} / {definition.title}")
            SameLine(360.0f)
            if (Button("{binding_text}##capture", ImVec2(180.0f, 0.0f))) {
                registry.capture_command_id = definition.id
                registry.status = "Press a chord for {definition.title}..."
            }
            SameLine()
            if (SmallButton("Clear")) {
                command_clear_binding(registry, definition.id, definition.scope)
            }
            if (index >= 0) {
                let conflict = command_binding_conflict(registry, index)
                if (!empty(conflict)) {
                    SameLine()
                    TextColored(float4(1.0f, 0.45f, 0.25f, 1.0f),
                                "conflicts with {conflict}")
                }
            }
            PopID()
        }
        EndChild()
        command_capture_tick(registry)
    }
    End()
}

def public command_registry_dispose(var registry : CommandRegistry) {
    delete registry.definitions
    delete registry.default_bindings
    delete registry.bindings
    delete registry.invocations
    registry = CommandRegistry()
}
