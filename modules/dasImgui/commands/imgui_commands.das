options gen2
options _comment_hygiene = true
options indenting = 4
options _allow_imgui_legacy = true

module imgui_commands shared public

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
    //! Optional second chord (numpad twins, CUA alternates); 0 = none.
    @optional alt_key : int = 0
    @optional alt_primary : bool = false
    @optional alt_shift : bool = false
    @optional alt_alt : bool = false
    @optional alt_secondary : bool = false
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
    //! When true, the armed capture writes the binding's SECOND chord.
    capture_alt : bool = false
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

def public with_alt_chord(binding : CommandBinding; key : ImGuiKey;
                          primary : bool = false; shift : bool = false;
                          alt : bool = false;
                          secondary : bool = false) : CommandBinding {
    //! Attach the second chord to a binding (chainable at registration).
    var result := binding
    result.alt_key = int(key)
    result.alt_primary = primary
    result.alt_shift = shift
    result.alt_alt = alt
    result.alt_secondary = secondary
    return result
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
        binding.command_id = definition.id
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

[unused_argument(registry)]
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
    if (empty(registry.invocations)) return false
    invocation <- registry.invocations[0]
    registry.invocations |> erase(0)
    return true
}

def private command_uses_macos_behaviors() : bool {
    if (GetCurrentContext() != null) {
        return GetIO().ConfigMacOSXBehaviors
    }
    return get_platform_name() == "darwin"
}

def private make_chord(key : int; primary, shift, alt, secondary : bool) : int {
    var chord = key
    let macos = command_uses_macos_behaviors()
    if (primary) {
        chord = chord | (macos ? COMMAND_MOD_SUPER : COMMAND_MOD_CTRL)
    }
    if (shift) { chord = chord | COMMAND_MOD_SHIFT }
    if (alt) { chord = chord | COMMAND_MOD_ALT }
    if (secondary) {
        chord = chord | (macos ? COMMAND_MOD_CTRL : COMMAND_MOD_SUPER)
    }
    return chord
}

def public command_binding_chord(binding : CommandBinding const) : int {
    return make_chord(binding.key, binding.primary, binding.shift,
        binding.alt, binding.secondary)
}

def public command_binding_alt_chord(binding : CommandBinding const) : int {
    //! The second chord, or 0 when the binding has none.
    return 0 if (binding.alt_key == 0)
    return make_chord(binding.alt_key, binding.alt_primary, binding.alt_shift,
        binding.alt_alt, binding.alt_secondary)
}

def private primary_name() : string {
    let io & = unsafe(GetIO())
    return io.ConfigMacOSXBehaviors ? "Cmd" : "Ctrl"
}

def private secondary_name() : string {
    let io & = unsafe(GetIO())
    return io.ConfigMacOSXBehaviors ? "Ctrl" : "Super"
}

def private chord_text(key : int; primary, shift, alt, secondary : bool) : string {
    if (key == 0) return ""
    var value : string
    if (primary) { value += primary_name() + "+" }
    if (shift) { value += "Shift+" }
    if (alt) { value += "Alt+" }
    if (secondary) { value += secondary_name() + "+" }
    value += "{GetKeyName(unsafe(reinterpret<ImGuiKey>(key)))}"
    return value
}

def public command_binding_text(binding : CommandBinding const) : string {
    return chord_text(binding.key, binding.primary, binding.shift,
        binding.alt, binding.secondary)
}

def public command_binding_alt_text(binding : CommandBinding const) : string {
    return chord_text(binding.alt_key, binding.alt_primary, binding.alt_shift,
        binding.alt_alt, binding.alt_secondary)
}

def public command_shortcut_text(registry : CommandRegistry const;
                                 id : string) : string {
    let definition = definition_index(registry, id)
    if (definition < 0) return ""
    let index = binding_index(registry, id, registry.definitions[definition].scope)
    return index >= 0 ? command_binding_text(registry.bindings[index]) : ""
}

def private keypad_chord(chord : int) : bool {
    let key = chord & ((1 << 12) - 1)
    return key >= int(ImGuiKey.Keypad0) && key <= int(ImGuiKey.KeypadEqual)
}

def public command_poll_scope(var registry : CommandRegistry;
                              scope : CommandScope; context : string = "";
                              mute_keypad : bool = false) {
    //! Poll every binding in `scope` via ImGui `Shortcut` routing. Both
    //! chords are tested; a chord fires at most ONE command per frame.
    //! `mute_keypad` skips keypad chords (the NumLock disambiguation).
    if (!empty(registry.capture_command_id)) return
    var fired : array<int>
    for (binding in registry.bindings) {
        if (binding.scope != scope
                || !command_enabled(registry, binding.command_id)) continue
        let flags = (scope == CommandScope.application ? COMMAND_ROUTE_GLOBAL : 0) | (binding.repeat ? COMMAND_REPEAT : 0)
        for (chord_slot in range(2)) {
            let chord = (chord_slot == 0 ? command_binding_chord(binding)
                : command_binding_alt_chord(binding))
            continue if ((chord & ((1 << 12) - 1)) == 0
                || (mute_keypad && keypad_chord(chord))
                || find_index(fired, chord) >= 0)
            if (Shortcut(chord, flags)) {
                fired |> push(chord)
                let _ = command_invoke(registry, binding.command_id,
                                       CommandSource.keyboard, context)
            }
        }
    }
    delete fired
}

def public command_take_prefix(var registry : CommandRegistry; prefix : string;
                               var invocation : CommandInvocation) : bool {
    //! Drain the first queued invocation whose id starts with `prefix`,
    //! leaving the rest for the app's own dispatcher — how a component
    //! (the editor) consumes only its commands from a shared registry.
    for (index in range(length(registry.invocations))) {
        if (starts_with(registry.invocations[index].command_id, prefix)) {
            invocation <- registry.invocations[index]
            registry.invocations |> erase(index)
            return true
        }
    }
    return false
}

def public command_reset_bindings(var registry : CommandRegistry) {
    delete registry.bindings
    registry.bindings |> push_clone_from(registry.default_bindings)
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
        registry.bindings[index].alt_key = 0
        registry.bindings[index].alt_primary = false
        registry.bindings[index].alt_shift = false
        registry.bindings[index].alt_alt = false
        registry.bindings[index].alt_secondary = false
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
    let alt_chord = command_binding_alt_chord(binding)
    for (index in range(length(registry.bindings))) {
        if (index == binding_index_) continue
        let other = registry.bindings[index]
        continue if (other.scope != binding.scope)
        let other_chord = command_binding_chord(other)
        let other_alt = command_binding_alt_chord(other)
        if (other_chord == chord || (alt_chord != 0 && other_chord == alt_chord)
                || (other_alt != 0 && (other_alt == chord || other_alt == alt_chord))) {
            return other.command_id
        }
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
    config.bindings |> push_clone_from(registry.bindings)
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
    //!@nolint
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
        // nolint:STYLE014 — Skip the side-modifier keys themselves (LeftCtrl..RightSuper are
        // contiguous): a chord's modifiers press first, and binding on their
        // own down-frame would capture "Ctrl+LeftCtrl" before the real key
        // arrives. Modifiers are read from io.Key* when the non-modifier
        // key fires.
        if (int(key) >= int(ImGuiKey.LeftCtrl)
                && int(key) <= int(ImGuiKey.RightSuper)) continue
        let name = "{GetKeyName(key)}"
        // "None"/"Unknown" cover the enum's sentinel members — IsKeyPressed
        // on those is out-of-bounds (asserts in Debug, OOB read in Release).
        if (empty(name) || name == "None" || name == "Unknown"
                || starts_with(name, "Mod") || starts_with(name, "Mouse")
                || starts_with(name, "Gamepad") || !IsKeyPressed(key, false)) continue
        let primary_down = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl
        let secondary_down = io.ConfigMacOSXBehaviors ? io.KeyCtrl : io.KeySuper
        let id = registry.capture_command_id
        let scope = registry.definitions[definition].scope
        if (registry.capture_alt) {
            let index = binding_index(registry, id, scope)
            let binding := (index >= 0 ? registry.bindings[index]
                : CommandBinding(command_id = id, scope = scope))
            command_set_binding(registry, with_alt_chord(binding, key,
                primary_down, io.KeyShift, io.KeyAlt, secondary_down))
        } else {
            var updated = command_binding(id, key,
                primary_down, io.KeyShift, io.KeyAlt, secondary_down, scope)
            let index = binding_index(registry, id, scope)
            if (index >= 0) {
                // keep the existing second chord across a primary rebind
                updated.alt_key = registry.bindings[index].alt_key
                updated.alt_primary = registry.bindings[index].alt_primary
                updated.alt_shift = registry.bindings[index].alt_shift
                updated.alt_alt = registry.bindings[index].alt_alt
                updated.alt_secondary = registry.bindings[index].alt_secondary
                updated.repeat = registry.bindings[index].repeat
            }
            command_set_binding(registry, updated)
        }
        registry.capture_alt = false
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
                registry.capture_alt = false
                registry.status = "Press a chord for {definition.title}..."
            }
            SameLine()
            let alt_text = index >= 0 ? command_binding_alt_text(registry.bindings[index]) : ""
            let alt_label = empty(alt_text) ? "+ alt" : alt_text
            if (Button("{alt_label}##capture_alt", ImVec2(140.0f, 0.0f))) {
                registry.capture_command_id = definition.id
                registry.capture_alt = true
                registry.status = "Press a SECOND chord for {definition.title}..."
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
