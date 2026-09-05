options gen2
options persistent_heap
options gc
options rtti
options stack = 262144

require lp_render
require lp_dialogue
require lp_controls
require lp_audio
require lp_microphone
require lp_chatter
require live/glfw_live
require live/opengl_live
require live/live_commands
require live/live_api
require live/live_watch_boost
require opengl/opengl_ttf
require daslib/utf8_utils
require daslib/fio

var display_w = 1280
var display_h = 800
var framebuffer_w = 1280
var framebuffer_h = 800
var trace_gl_errors = false
var scene_gl_error = 0u
var hud_gl_error = 0u
var hud_font : Font?
var keys : bool[512]
var previous_mouse = float2(0.0)
var ui_cursor = float2(0.0)
var activation_char_pending = false

struct VisualPreferences {
    reduced_flashes : bool = false
    reflections : bool = true
    dust : bool = true
    music : float = 0.35
    effects : float = 0.65
    voice : float = 1.0
    logging : bool = true
    chatter : bool = true
    voice_processing : bool = true
}

def save_visual_preferences() {
    let prefs = VisualPreferences(reduced_flashes = reduced_flashes, reflections = render_ssr, dust = render_dust,
        music = music_volume, effects = effects_volume, voice = voice_volume, logging = runtime_logging, chatter = chatter_enabled,
        voice_processing = voice_processing)
    let path = path_join(get_das_root(), "examples/games/latchpoint/saves/visual-settings.json")
    if (!fwrite(path, sprint_json(prefs, false))) { to_log(LOG_WARNING, "Could not save visual preferences.") }
}

def load_visual_preferences() {
    let path = path_join(get_das_root(), "examples/games/latchpoint/saves/visual-settings.json")
    return if (!fexist(path))
    var prefs = VisualPreferences()
    if (sscan_json(fread(path), prefs)) {
        reduced_flashes = prefs.reduced_flashes
        render_ssr = prefs.reflections
        render_dust = prefs.dust
        music_volume = clamp(prefs.music, 0.0, 1.0)
        effects_volume = clamp(prefs.effects, 0.0, 1.0)
        voice_volume = clamp(prefs.voice, 0.0, 1.5)
        runtime_logging = prefs.logging
        playtest_logging = runtime_logging
        chatter_enabled = prefs.chatter
        voice_processing = prefs.voice_processing
    }
}

def activate_menu(selection : int) {
    return if (!empty(pending_job))
    if (selection == 0) {
        submit_interaction("new_game", "")
    } elif (selection == 1 && has_saved_game) {
        refresh_world()
        load_avatar()
        menu_open = false
    }
}

def send_typed() {
    if (!empty(input_text) && conversation_resident >= 0) {
        submit_interaction("talk", input_text, conversation_resident)
        if (!empty(pending_job) && conversation_stage == "thinking") {
            input_text = ""
            editing = false
        }
    }
}

def settings_key(key : int) : bool {
    return false if (editing)
    if (key == GLFW_KEY_P) { voice_processing = !voice_processing; save_visual_preferences(); notify_feedback(voice_processing ? "ROBOT SPEAKER COLORATION ON" : "DRY VOICES / COLORATION OFF"); return true }
    if (key == GLFW_KEY_F12) {
        chatter_enabled = !chatter_enabled
        player_turn_epoch++
        if (!chatter_enabled) {
            if (queued_voice_ambient) { queued_voice |> clear() }
            if (speech_is_ambient) { interrupt_voice() }
        }
        save_visual_preferences()
        return true
    }
    if (key >= GLFW_KEY_F8 && key <= GLFW_KEY_F11) {
        if (key == GLFW_KEY_F8) { music_volume = music_volume > 0.0 ? 0.0 : 0.35 }
        elif (key == GLFW_KEY_F9) { effects_volume = effects_volume > 0.0 ? 0.0 : 0.65 }
        elif (key == GLFW_KEY_F10) { voice_volume = voice_volume > 0.0 ? 0.0 : 1.0 }
        else { runtime_logging = !runtime_logging; playtest_logging = runtime_logging }
        save_visual_preferences()
        return true
    }
    if (key == GLFW_KEY_L || key == GLFW_KEY_F6 || key == GLFW_KEY_F7) {
        if (key == GLFW_KEY_L) { reduced_flashes = !reduced_flashes }
        elif (key == GLFW_KEY_F6) { render_ssr = !render_ssr }
        else { render_dust = !render_dust }
        save_visual_preferences()
        return true
    }
    return false
}

def is_confirm_key(key : int) : bool {
    return key == GLFW_KEY_ENTER || key == GLFW_KEY_KP_ENTER
}

def editing_key(key : int) {
    if (is_confirm_key(key)) { send_typed() }
    elif (key == GLFW_KEY_ESCAPE) { editing = false; set_robot_phase(conversation_resident, RobotPhase.idle) }
    elif (key == GLFW_KEY_BACKSPACE) {
        var codepoints <- utf8_decode(input_text)
        if (!empty(codepoints)) { codepoints |> resize(length(codepoints) - 1) }
        input_text = string(utf8_encode(codepoints))
    }
}

def press_key(key : int) {
    if (microphone_active) {
        if (key == GLFW_KEY_ESCAPE) { cancel_microphone(); reply_text = "Recording discarded." }
        return
    }
    if (key == GLFW_KEY_V && !editing) { begin_microphone(); return }
    return if (settings_key(key))
    if (menu_open) {
        if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) { menu_selection = 1 - menu_selection }
        elif (is_confirm_key(key)) { activate_menu(menu_selection) }
        elif (key == GLFW_KEY_N) { activate_menu(0) }
        elif (key == GLFW_KEY_C || key == GLFW_KEY_ESCAPE) { activate_menu(1) }
        return
    }
    if (editing) {
        editing_key(key)
        return
    }
    if (key == GLFW_KEY_E && empty(pending_job)) {
        player_turn_epoch++
        interrupt_voice()
        conversation_resident = nearest_resident()
        editing = conversation_resident >= 0
        if (editing) { set_robot_phase(conversation_resident, RobotPhase.listening) }
        activation_char_pending = editing
    }
    if (key == GLFW_KEY_F && empty(pending_job)) {
        let action = physical_at_avatar()
        if (!empty(action)) { submit_interaction("physical", action) }
    }
    if (key == GLFW_KEY_R && empty(pending_job)) { refresh_world() }
    if (key == GLFW_KEY_ESCAPE) {
        interrupt_voice()
        if (!empty(pending_job)) { cancel_dialogue() }
        save_avatar()
        menu_selection = has_saved_game ? 1 : 0
        menu_open = true
    }
}

var on_key <- @(window : GLFWwindow const?; var key, scan, action, mods : int) {
    if (key >= 0 && key < 512) { keys[key] = action != GLFW_RELEASE }
    if (key == GLFW_KEY_E && action == GLFW_RELEASE) { activation_char_pending = false }
    if (key == GLFW_KEY_V && action == GLFW_RELEASE) { finish_microphone() }
    if (action == GLFW_PRESS || (action == GLFW_REPEAT && key == GLFW_KEY_BACKSPACE)) { press_key(key) }
}

var on_char <- @(window : GLFWwindow const?; var codepoint : uint) {
    if (editing && !menu_open && codepoint >= 32u && length(input_text) < 1000) {
        if (activation_char_pending) {
            activation_char_pending = false
            return
        }
        input_text += string(utf8_encode(codepoint))
    }
}

var on_mouse <- @(_window : GLFWwindow const?; var button, action, _mods : int) {
    return if (!menu_open || button != GLFW_MOUSE_BUTTON_1 || action != GLFW_PRESS)
    var width, height : int
    glfwGetWindowSize(live_window, safe_addr(width), safe_addr(height))
    return if (width <= 0 || height <= 0)
    let p = ui_cursor * float2(float(display_w) / float(width), float(display_h) / float(height))
    let selection = menu_hit(p, float2(float(display_w), float(display_h)))
    if (selection >= 0) { activate_menu(selection) }
}

var on_cursor <- @(_window : GLFWwindow const?; var x, y : double) {
    ui_cursor = float2(x, y)
}

def move_avatar(dt : float) {
    let mouse = glfwGetCursorPos(live_window)
    if (!editing && !menu_open && camera_dragging(glfwGetMouseButton(live_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS,
            glfwGetMouseButton(live_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)) {
        let delta = mouse - previous_mouse
        heading += delta.x * 0.003
        pitch = clamp(pitch - delta.y * 0.003, -1.1, 1.1)
    }
    previous_mouse = mouse
    return if (editing || menu_open)
    let input = movement_axes(keys)
    if (length(input) > 0.0) {
        let forward = float2(sin(heading), cos(heading))
        let right = float2(cos(heading), -sin(heading))
        let speed = (keys[GLFW_KEY_LEFT_SHIFT] || keys[GLFW_KEY_RIGHT_SHIFT]) ? 5.0 : 3.0
        avatar = float3(slide_step(avatar.xy, (forward * input.y + right * input.x) * dt * speed, 0.33, true), avatar.z)
    }
}

def panel(x, y, width, height : int; color : float3) {
    glEnable(GL_SCISSOR_TEST)
    let rect = ui_scissor_rect(x, y, width, height, int2(display_w, display_h), int2(framebuffer_w, framebuffer_h))
    glScissor(rect.x, rect.y, rect.z, rect.w)
    glClearColor(color.x, color.y, color.z, 1.0)
    glClear(GL_COLOR_BUFFER_BIT)
    glDisable(GL_SCISSOR_TEST)
}

def text_line(text : string; x, y, size : float; tint : float3 = float3(0.78, 0.86, 0.82)) {
    return if (hud_font == null)
    var quads <- (*hud_font) |> create_quads(text)
    let matrix = (ortho_rh(0.0, float(display_w), float(display_h), 0.0, -1.0, 1.0) *
        compose(float3(x, y, 0.0), float4(0.0, 0.0, 0.0, 1.0), float3(size * 2.0, size * 2.0, 1.0)))
    (*hud_font) |> draw_quads(quads, matrix, tint)
    delete quads
}

def paragraph(text : string; x, y : float; columns : int = 100; max_lines : int = 4) {
    let words <- split(text, " ")
    var inscope line_words : array<string>
    line_words |> reserve(length(words))
    var line_length = 0
    var row = 0
    for (word in words) {
        if (line_length + length(word) + 1 > columns) {
            text_line(join(line_words, " "), x, y + float(row) * 25.0, 0.42)
            row++
            return if (row >= max_lines)
            line_words |> clear()
            line_length = 0
        }
        line_words |> push(word)
        line_length += length(word) + 1
    }
    text_line(join(line_words, " "), x, y + float(row) * 25.0, 0.42)
}

def draw_menu() {
    let cx = display_w / 2
    let cy = display_h / 2
    panel(cx - 290, cy - 210, 580, 500, float3(0.018, 0.032, 0.035))
    text_line("L A T C H P O I N T", float(cx - 240), float(cy - 150), 0.72, TEAL)
    text_line("A SMALL ROBOT SANDBOX / LOCAL INFERENCE", float(cx - 240), float(cy - 112), 0.32)
    panel(cx - 240, cy - 56, 480, 64, menu_selection == 0 ? float3(0.08, 0.20, 0.18) : float3(0.04, 0.07, 0.075))
    text_line("NEW GAME", float(cx - 220), float(cy - 28), 0.48, TEAL)
    text_line("Start over / clears progress and robot memories", float(cx - 220), float(cy - 7), 0.27)
    panel(cx - 240, cy + 28, 480, 64, menu_selection == 1 ? float3(0.08, 0.20, 0.18) : float3(0.04, 0.07, 0.075))
    text_line("CONTINUE", float(cx - 220), float(cy + 57), 0.48, has_saved_game ? TEAL : float3(0.3))
    text_line(has_saved_game ? "Resume your saved world and conversations" : "No saved playthrough yet",
        float(cx - 220), float(cy + 79), 0.27)
    let status = (!empty(pending_job) ? "Waiting for the current interaction..." :
        "UP/DOWN SELECT / ENTER CONFIRM / OR CLICK")
    text_line(status, float(cx - 240), float(cy + 125), 0.29, AMBER)
    if (!bridge_online) { text_line("Bridge offline. Start start-game.ps1 and try again.", float(cx - 240), float(cy + 153), 0.28, AMBER) }
    elif (playtest_logging) { text_line("Conversation test logs stay on this machine.", float(cx - 240), float(cy + 153), 0.28) }
    text_line(reduced_flashes ? "L / STORM FLASHES OFF" : "L / STORM FLASHES ON", float(cx - 240), float(cy + 173), 0.27, AMBER)
    text_line("F6 / REFLECTIONS {render_ssr ? "ON" : "OFF"}    F7 / DUST {render_dust ? "ON" : "OFF"}", float(cx - 240), float(cy + 193), 0.27)
    text_line("F8 MUSIC {music_volume > 0.0 ? "ON" : "OFF"} / F9 SFX {effects_volume > 0.0 ? "ON" : "OFF"} / F10 VOICE {voice_volume > 0.0 ? "ON" : "OFF"}",
        float(cx - 240), float(cy + 215), 0.26)
    text_line("E TYPE / HOLD V FOR MICROPHONE / RELEASE TO TRANSCRIBE", float(cx - 240), float(cy + 235), 0.25)
    text_line("F11 / LOCAL CONVERSATION LOGS {runtime_logging ? "ON" : "OFF"}", float(cx - 240), float(cy + 255), 0.25)
    text_line("F12 / AMBIENT CHATTER {chatter_enabled ? "ON" : "OFF"}", float(cx + 30), float(cy + 255), 0.25)
    text_line("P / ROBOT VOICE COLORATION {voice_processing ? "ON" : "OFF"} (NEXT UTTERANCE)", float(cx - 240), float(cy + 275), 0.25)
}

def landmark_label(caption : string; position : float3; tint : float3 = TEAL) {
    let clip = v_projection * (v_view * float4(position, 1.0))
    return if (clip.w <= 0.0 || length(position - avatar) > 32.0)
    let ndc = clip.xy / clip.w
    return if (abs(ndc.x) > 0.9 || abs(ndc.y) > 0.7)
    let px = (ndc.x * 0.5 + 0.5) * float(display_w)
    let py = (0.5 - ndc.y * 0.5) * float(display_h)
    return if (hud_font == null)
    var quads <- (*hud_font) |> create_quads(caption)
    let bounds = quads_dim(quads)
    let width = (bounds.vmax.x - bounds.vmin.x) * 0.6
    delete quads
    let glow = tint * (0.45 + 0.55 * (1.0 - smoothstep(15.0, 32.0, length(position - avatar))))
    let left = int(px - width * 0.5 - 8.0)
    let right = int(px + width * 0.5 + 8.0)
    for (x in fixed_array(left, right)) {
        panel(x, int(py - 15.0), 1, 18, glow * 0.65)
        panel(x == left ? x : x - 5, int(py - 15.0), 6, 1, glow)
    }
    panel(int(px), int(py + 6.0), 1, 12, glow * 0.35)
    panel(int(px - 2.0), int(py + 19.0), 5, 2, glow)
    text_line(caption, px - width * 0.5 + 1.0, py + 1.0, 0.30, float3(0.008, 0.018, 0.018))
    text_line(caption, px - width * 0.5, py, 0.30, glow)
}

def draw_landmarks() {
    if (world.archive_open) { landmark_label(world.archive_inspected ? "SERVICE RECORD / RECOVERED" : "[F] INSPECT SERVICE RECORD", ARCHIVE_RECORD_POSITION + float3(0.0, 0.0, 0.55), AMBER) }
    landmark_label("WORKSHOP 01", float3(4.0, 0.3, 3.5))
    landmark_label("GENERATOR 02", GENERATOR_POSITION + float3(0.0, 0.0, 3.8))
    landmark_label("ARCHIVE 03", float3(-4.0, 10.3, 3.5))
    if (actuator_available()) { landmark_label("SALVAGE 04", SALVAGE_POSITION + float3(0.0, 0.0, 1.8), AMBER) }
    if (world.workshop_open && find_index(world.salvaged, "coil") < 0 && length(avatar.xy - COIL_POSITION.xy) < 5.0) {
        landmark_label("COPPER COIL", COIL_POSITION + float3(0.0, 0.0, 1.55), AMBER)
    }
}

def draw_conversation_phase(y : float) {
    if (!empty(pending_job)) {
        let seconds = int(get_uptime() - submitted_at)
        let phase = conversation_stage == "asr" ? "RECOGNIZING SPEECH" : (conversation_stage == "voice" ? "PREPARING VOICE" : "THINKING")
        text_line("{phase} / {seconds}s / YOU CAN KEEP EXPLORING", 300.0, y + 30.0, 0.28, AMBER)
    } elif (speech_is_ambient) {
        text_line("AMBIENT / E OR V TO INTERRUPT", 300.0, y + 30.0, 0.28, TEAL)
    } elif (microphone_active) {
        text_line("MIC LIVE / RELEASE V / ESC DISCARD", 300.0, y + 30.0, 0.28, AMBER)
    } elif (conversation_stage == "speaking") {
        text_line("SPEAKING / E TO REPLY / ESC TO INTERRUPT", 300.0, y + 30.0, 0.28, TEAL)
    } elif (editing) {
        text_line("LISTENING / TYPE YOUR MESSAGE", 300.0, y + 30.0, 0.28, TEAL)
    }
}

def draw_hud() {
    glDisable(GL_CULL_FACE)
    glDisable(GL_DEPTH_TEST)
    glEnable(GL_BLEND)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
    if (menu_open) {
        draw_menu()
        return
    }
    draw_landmarks()
    draw_action_feedback()
    panel(22, 22, 382, 92, float3(0.022, 0.04, 0.043))
    text_line("L A T C H P O I N T", 40.0, 55.0, 0.60, float3(0.85, 0.91, 0.80))
    text_line("EXTRACTION OUTPOST / SECTOR 07", 40.0, 83.0, 0.31, TEAL)
    text_line("REMOTE AVATAR LINK / LOCAL INFERENCE", 40.0, 102.0, 0.25)
    panel(display_w - 285, 22, 263, 87, float3(0.022, 0.04, 0.043))
    let power = world.power ? "ONLINE" : "OFFLINE"
    let leg = world.patch_repaired ? "REPAIRED" : "DAMAGED"
    text_line("POWER  {power}", float(display_w - 267), 49.0, 0.36, world.power ? TEAL : AMBER)
    text_line("PATCH  {leg}", float(display_w - 267), 72.0, 0.36)
    text_line(bridge_online ? "NATIVE RUNTIME / LOCAL SERVER" : "SAVE OR LINK ERROR / R RETRY", float(display_w - 267), 95.0, 0.25)
    let near = nearest_resident()
    let action = physical_at_avatar()
    let y = float(display_h - 230)
    if (near >= 0 && !editing) { text_line("[E] TYPE TO {resident_name(near)}  /  HOLD [V] TO SPEAK", 38.0, y - 18.0, 0.36, TEAL) }
    if (!empty(action) && !editing) { text_line("[F] {action}", 38.0, y - 44.0, 0.37, AMBER) }
    panel(22, display_h - 223, display_w - 44, 166, float3(0.024, 0.04, 0.043))
    let name = speech_is_ambient ? resident_name(speaking_resident) : (conversation_resident >= 0 ? resident_name(conversation_resident) : "OUTPOST TELEMETRY")
    text_line(name, 40.0, y + 30.0, 0.35, TEAL)
    draw_conversation_phase(y)
    paragraph(speech_is_ambient ? ambient_caption : reply_text, 40.0, y + 59.0, max(30, (display_w - 100) / 13), 3)
    if (editing) {
        text_line("> {input_text}_", 40.0, y + 142.0, 0.4, float3(0.95, 0.95, 0.84))
    } else {
        let items = empty(world.inventory) ? "empty" : join(world.inventory, ", ")
        text_line("CARGO / {items}", 40.0, y + 142.0, 0.33, AMBER)
    }
    text_line(editing ? "ENTER SEND    ESC CANCEL    FREE-FORM CONVERSATION" :
        "WASD / ARROWS MOVE   CLICK-DRAG LOOK   E TYPE   HOLD V SPEAK   F USE   ESC MENU", 38.0, float(display_h - 28), 0.29)
    if (world.archive_inspected) {
        text_line("RECORD RECOVERED / SHIFT COMPLETE", 430.0, 64.0, 0.36, TEAL)
    } elif (world.archive_open) {
        text_line("ARCHIVE RESTORED / THE SHIFT IS YOURS", 430.0, 64.0, 0.36, TEAL)
    } elif (world.power && world.patch_repaired) {
        text_line("OUTPOST BACK ONLINE", 480.0, 64.0, 0.40, TEAL)
    }
}

def draw_action_feedback() {
    if (get_uptime() < feedback_until) {
        panel(420, 117, display_w - 730, 3, feedback_positive ? TEAL : AMBER)
        paragraph(feedback_text, 425.0, 144.0, max(25, (display_w - 735) / 12), 2)
    }
    if (physical_at_avatar() == "start generator") {
        let mode = patch_can_assist() ? "PATCH HOLDING CLUTCH / HOLD F" : "MANUAL CRANK / HOLD F / PATCH CAN HELP"
        text_line(mode, 38.0, float(display_h - 300), 0.32, AMBER)
        panel(38, display_h - 287, 310, 5, float3(0.04, 0.08, 0.08))
        panel(38, display_h - 287, int(starter_progress * 310.0), 5, TEAL)
    }
}

def tick_generator(dt : float) {
    let before = starter_progress > 0.96 ? runtime_snapshot() : ""
    if (advance_starter(dt, keys[GLFW_KEY_F] && !editing && !menu_open && empty(pending_job))) {
        if (!runtime_save()) {
            if (!empty(before)) { runtime_load(before) }
            world.power = false
            notify_feedback("Could not save generator startup. Try again.", false)
        } else {
            notify_feedback(patch_can_assist() ? "GENERATOR ONLINE / PATCH'S CLUTCH ASSIST COMPLETE" : "GENERATOR ONLINE / MANUAL START COMPLETE")
            runtime_trace("generator_started", runtime_snapshot())
        }
    }
}

struct PoseArgs {
    x : float = 0.0
    y : float = -9.0
    z : float = 1.65
    yaw : float = 0.0
    tilt : float = -0.06
}

[live_command(description="Place the inspection camera. Args: x,y,z,yaw,tilt")]
def cmd_pose(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<PoseArgs>)
    avatar = float3(args.x, args.y, args.z)
    heading = args.yaw
    pitch = args.tilt
    return JV(avatar)
}

[live_command(description="Read player, resident, dialogue and authoritative world state")]
def cmd_scene_state(_input : JsonValue?) : JsonValue? {
    return JV((avatar = avatar, patch = patch_position, world = JV(world), pending = pending_job,
        editing = editing, input = input_text, reply = reply_text, nearest = nearest_resident(),
        menu = menu_open, has_save = has_saved_game, yaw = heading, interaction = physical_at_avatar(),
        patch_yaw = patch_heading, salvage_visible = actuator_available()))
}

struct RenderArgs {
    material : int = 0
    bloom : bool = true
    ao : bool = true
    time : float = -1.0
    reduced_flashes : bool = false
    ssr : bool = true
    dust : bool = true
    trace_errors : bool = false
}

[live_command(description="Inspect rendering. Args: material (0 lit,1 roughness,2 metalness,3 normals), bloom,ao,time (-1 realtime),reduced_flashes,ssr,dust")]
def cmd_render_settings(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<RenderArgs>)
    render_material_mode = clamp(args.material, 0, 3)
    render_bloom = args.bloom
    render_ao = args.ao
    render_time_override = args.time
    reduced_flashes = args.reduced_flashes
    render_ssr = args.ssr
    render_dust = args.dust
    trace_gl_errors = args.trace_errors
    trace_render_errors = args.trace_errors
    return JV(args)
}

[live_command(description="Read lighting, reflection and atmosphere state without changing it")]
def cmd_lighting_state(_input : JsonValue?) : JsonValue? {
    return JV((lights = YARD_LIGHTS, shadowed_local_lights = 1, probe_ready = lp_environment_ready > 0.5,
        reduced_flashes = reduced_flashes, flash = lp_flash, lamp = hanging_lamp(lp_clock),
        ssr = render_ssr, dust = render_dust, static_parts = length(scene_parts), dynamic_parts = length(frame_parts)))
}

[live_command(description="Read native dialogue, audio and gate animation status")]
def cmd_native_status(_input : JsonValue?) : JsonValue? {
    return JV((runtime = "daslang", client = "dasOPENAI", phase = conversation_stage, pending = inference_busy(),
        audio = audio_ready, speech = speech_sid != INVALID_SID, speaker = speaking_resident,
        microphone = microphone_active, microphone_level = microphone_level,
        robot_phases = JV(robot_phase), indicators = JV(robot_voice_level),
        gates = float2(workshop_aperture, archive_aperture)))
}

[live_command(description="Read ambient activity and chatter state")]
def cmd_ambient_state(_input : JsonValue?) : JsonValue? {
    let time = get_uptime()
    return JV((enabled = chatter_enabled, pending = chatter_job.done != 0ul, stage = chatter_stage,
        count = chatter_count, line = chatter_last_line, allowed = ambient_activity_allowed,
        drill = drill_speed(time), contact = drill_contact(time), lamp = inspection_lamp(time), speaking = speech_is_ambient))
}

struct AmbientRequestArgs {
    npc : int = 2
    topic : string = "Distant lightning illuminates the cloud bank beyond the yard."
}

[live_command(description="Request one test observation when idle. Args: npc,topic. No gameplay actions.")]
def cmd_chatter_request(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<AmbientRequestArgs>)
    if (args.npc < 0 || args.npc > 2 || length(args.topic) > 300) { return JV((error = "Invalid observation")) }
    if (chatter_job.done != 0ul) { return JV((accepted = false, pending = true, stage = chatter_stage)) }
    chatter_next = 0.0
    begin_chatter(args.topic, args.npc, get_uptime())
    return JV((accepted = chatter_job.done != 0ul, pending = chatter_job.done != 0ul, stage = chatter_stage))
}

[live_command(description="Read back the HDR target and report invalid pixels and GL errors; diagnostic, not a frame-time benchmark")]
def cmd_hdr_check(_input : JsonValue?) : JsonValue? {
    if (scene_fbo == 0u || fx_width <= 0 || fx_height <= 0) return JV((error = "No scene framebuffer"))
    var inscope pixels : array<float4>
    pixels |> reserve(fx_width * fx_height)
    pixels |> resize(fx_width * fx_height)
    glBindFramebuffer(GL_FRAMEBUFFER, scene_fbo)
    glReadBuffer(GL_COLOR_ATTACHMENT0)
    glReadPixels(0, 0, fx_width, fx_height, GL_RGBA, GL_FLOAT, unsafe(addr(pixels[0])))
    glBindFramebuffer(GL_FRAMEBUFFER, 0u)
    var invalid = 0
    var peak = 0.0
    for (pixel in pixels) {
        if (!hdr_pixel_valid(pixel)) { invalid++ }
        else { peak = max(peak, max(pixel.x, max(pixel.y, pixel.z))) }
    }
    return JV((invalid = invalid, peak = peak, hdr = fx_hdr, gl_error = glGetError(),
        scene_error = scene_gl_error, hud_error = hud_gl_error, stages = JV(render_errors)))
}

struct TalkArgs {
    npc : int = 1
    text : string
}

[live_command(description="Submit a resident conversation. Args: npc (0 Brack,1 Patch,2 Vesper), text")]
def cmd_talk(input : JsonValue?) : JsonValue? {
    let args = from_JV(input, type<TalkArgs>)
    if (args.npc < 0 || args.npc > 2) return JV((error = "Resident out of range"))
    submit_interaction("talk", args.text, args.npc)
    return JV((pending = pending_job))
}

[live_command(description="Apply the physical interaction available at the avatar position")]
def cmd_interact(_input : JsonValue?) : JsonValue? {
    let action = physical_at_avatar()
    if (!empty(action)) { submit_interaction("physical", action) }
    return JV((action = action, pending = pending_job))
}

[export]
def init() {
    live_create_window("Latchpoint / Local robot sandbox", 1280, 800)
    glfwSetWindowSizeLimits(live_window, 1000, 700, -1, -1)
    init_barriers()
    load_visual_preferences()
    init_renderer()
    init_yard_audio()
    cache_ttf_objects()
    hud_font = cache_font(path_join(get_das_root(), "modules/dasStbImage/fonts/droidsansmono.ttf"))
    glfw_chain_add_key(live_window, on_key)
    glfw_chain_add_char(live_window, on_char)
    glfw_chain_add_mouse_button(live_window, on_mouse)
    glfw_chain_add_cursor_pos(live_window, on_cursor)
    previous_mouse = glfwGetCursorPos(live_window)
    ui_cursor = previous_mouse
    refresh_world()
    menu_selection = has_saved_game ? 1 : 0
}

[export]
def update() {
    return if (!live_begin_frame())
    live_get_framebuffer_size(framebuffer_w, framebuffer_h)
    glfwGetWindowSize(live_window, safe_addr(display_w), safe_addr(display_h))
    return if (display_w <= 0 || display_h <= 0 || framebuffer_w <= 0 || framebuffer_h <= 0)
    let dt = min(get_dt(), 0.05)
    ambient_activity_allowed = !menu_open && !editing && !microphone_active && empty(pending_job) && speech_sid == INVALID_SID
    move_avatar(dt)
    if (!menu_open) { tick_follow(dt) }
    poll_dialogue()
    tick_behavior(dt)
    tick_generator(dt)
    tick_yard_audio(get_uptime())
    tick_microphone()
    tick_chatter(get_uptime())
    if (trace_gl_errors) { glGetError() }
    render_yard(framebuffer_w, framebuffer_h, get_uptime())
    if (trace_gl_errors) { scene_gl_error = glGetError() }
    draw_hud()
    if (trace_gl_errors) { hud_gl_error = glGetError() }
    live_end_frame()
}

[export]
def shutdown() {
    cancel_microphone()
    shutdown_yard_audio()
    save_avatar()
    glfw_chain_clear(live_window)
    finalize(cube)
    finalize(sphere)
    finalize(cylinder)
    finalize(rock)
    finalize(rock_alternate)
    finalize(beveled)
    glDeleteFramebuffers(1, safe_addr(lamp_shadow_fbo))
    glDeleteTextures(1, safe_addr(lamp_shadow_tex))
    glDeleteFramebuffers(1, safe_addr(probe_fbo))
    glDeleteTextures(1, safe_addr(probe_color))
    glDeleteTextures(1, safe_addr(probe_cube))
    glDeleteRenderbuffers(1, safe_addr(probe_depth))
    glDeleteFramebuffers(1, safe_addr(atmosphere_fbo))
    glDeleteTextures(1, safe_addr(atmosphere_color))
    live_destroy_window()
}

[export]
def main() {
    init()
    eval_main_loop() {
        update()
        maybe_collect_gc()
        return !exit_requested()
    }
    shutdown()
}
