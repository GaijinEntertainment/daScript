options gen2
options _comment_hygiene = true
options gc

require imgui/imgui_harness
require imgui/imgui_commands
require daslib/json public
require daslib/json_boost public
require live/live_commands

// =============================================================================
// FEATURE: macos_chords — the command registry under the macOS input model.
// SHOWS:   io.ConfigMacOSXBehaviors forced ON so every platform exercises the
//          Mac path: ImGui swaps Ctrl<->Super inside AddKeyEvent, bindings stay
//          declared in logical Ctrl-speak, and Shortcut() matching still fires.
// DRIVE:   curl -X POST -d '{"name":"imgui_key_chord","args":{"mods":["Ctrl"],"key":"K"}}' \
//              localhost:9090/command
//          curl -X POST -d '{"name":"macos_chords_state"}' localhost:9090/command
// STANDALONE: daslang.exe modules/dasImgui/examples/features/macos_chords.das
// HEADLESS:   daslang.exe modules/dasImgui/examples/features/macos_chords.das -- --headless --headless-frames=60
// =============================================================================

var private g_commands = CommandRegistry()
var private g_primary_fired = 0
var private g_secondary_fired = 0

[export]
def init() {
    harness_init("macos chords probe", 720, 320)
    // Force the macOS input model regardless of host OS: ImGui swaps
    // Ctrl<->Super inside AddKeyEvent when this is on, same as a real Mac.
    var io & = unsafe(GetIO())
    io.ConfigMacOSXBehaviors = true
    command_define(g_commands, CommandDefinition(
        id = "probe.primary", title = "Primary chord probe", category = "Probe"),
        command_binding("probe.primary", ImGuiKey.K, primary = true))
    command_define(g_commands, CommandDefinition(
        id = "probe.secondary", title = "Secondary chord probe", category = "Probe"),
        command_binding("probe.secondary", ImGuiKey.J, secondary = true))
}

[export]
def update() {
    if (!harness_begin_frame()) return
    harness_new_frame()
    command_begin_frame(g_commands)
    SetNextWindowPos(ImVec2(40.0, 40.0), ImGuiCond.Always)
    SetNextWindowSize(ImVec2(640.0, 240.0), ImGuiCond.Always)
    window(MAIN_WIN, (text = "macOS chord probe", closable = false,
                      flags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove)) {
        command_poll_scope(g_commands, CommandScope.application)
        command_capture_tick(g_commands)
        var invocation = CommandInvocation()
        while (command_take(g_commands, invocation)) {
            if (invocation.command_id == "probe.primary") {
                g_primary_fired++
            } elif (invocation.command_id == "probe.secondary") {
                g_secondary_fired++
            }
        }
        text("Cmd+K fires the primary probe, Ctrl+J the secondary one.")
        text("primary fired: {g_primary_fired}")
        text("secondary fired: {g_secondary_fired}")
    }
    harness_end_frame()
}

def private binding_of(id : string) : CommandBinding {
    for (binding in g_commands.bindings) {
        if (binding.command_id == id) return binding
    }
    return CommandBinding()
}

[live_command(description = "Chord ints, fire counters, io modifiers and the probe.primary binding under forced macOS behaviors.")]
def macos_chords_state(_input : JsonValue?) : JsonValue? {
    let io & = unsafe(GetIO())
    let primary = binding_of("probe.primary")
    let secondary = binding_of("probe.secondary")
    return JV((
        ok = true,
        macos_behaviors = io.ConfigMacOSXBehaviors,
        key_ctrl = io.KeyCtrl,
        key_super = io.KeySuper,
        primary_fired = g_primary_fired,
        secondary_fired = g_secondary_fired,
        primary_chord = command_binding_chord(primary),
        secondary_chord = command_binding_chord(secondary),
        primary_text = command_binding_text(primary),
        primary_key = primary.key,
        primary_is_primary = primary.primary,
        primary_is_secondary = primary.secondary,
        capture_armed = !empty(g_commands.capture_command_id)))
}

[live_command(description = "Arm binding capture on probe.primary — the next non-modifier key press rebinds it.")]
def macos_chords_arm_capture(_input : JsonValue?) : JsonValue? {
    g_commands.capture_command_id = "probe.primary"
    g_commands.capture_alt = false
    return JV((ok = true))
}

[export]
def shutdown() {
    command_registry_dispose(g_commands)
    harness_shutdown()
}

[export]
def main() {
    init()
    while (!exit_requested()) {
        update()
        harness_maybe_collect_gc()
    }
    shutdown()
}
