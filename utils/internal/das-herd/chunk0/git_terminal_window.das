options gen2
options gc
options persistent_heap

require imgui/imgui_harness
require imgui/imgui_terminal
require imgui/imgui_fonts
require daslib/safe_addr
require git_terminal_probe
require math
require strings

var private g_terminal = Terminal()
var private g_terminal_view = ImGuiTerminalState()
var private g_terminal_cache = ImGuiTerminalCache()
var private g_terminal_font : ImFont?
var private g_font_sources : array<string>
var private g_log_file : FILE const? = null
var private g_log_path : string
var private g_command_name : string
var private g_command_line : string
var private g_visual_header : string
var private g_zoom_percent = 150
var private g_started = 0l
var private g_launch_us = -1l
var private g_first_output_us = -1l
var private g_exit_us = -1l
var private g_drained_us = -1l
var private g_last_data_us = 0l
var private g_output_bytes = 0
var private g_exit_code = -1l
var private g_done = false
var private g_error : string

def private elapsed_us() : int64 {
    return get_time_nsec(g_started) / 1000l
}

def private option_value(prefix : string; fallback : string) : string {
    for (arg in get_command_line_arguments()) {
        if (starts_with(arg, prefix)) return slice(arg, length(prefix))
    }
    return fallback
}

def private select_command(repository : string) {
    let requested = option_value("--probe=", "status")
    let commands <- make_git_probe_commands(repository)
    var index = 1
    if (requested == "version") {
        index = 0
    } elif (requested == "missing-ref") {
        index = 2
    }
    g_command_name = commands[index].name
    g_command_line = commands[index].command_line
    g_visual_header = "$ {g_command_line}"
}

def private zoom_by(delta : int) {
    g_zoom_percent = clamp(g_zoom_percent + delta, 50, 300)
}

def private poll_terminal() {
    return if (g_done)
    for (_ in range(64)) {
        let status = terminal_poll(g_terminal)
        let now_us = elapsed_us()
        if (status == TerminalPollStatus.data) {
            if (g_first_output_us < 0l) {
                g_first_output_us = now_us
            }
            g_last_data_us = now_us
            g_output_bytes += length(g_terminal.read_buffer)
            if (g_log_file != null) {
                let _written = fwrite(g_log_file, g_terminal.read_buffer)
                fflush(g_log_file)
            }
        } elif (status == TerminalPollStatus.error) {
            g_error = terminal_transport_error(g_terminal)
            g_done = true
            break
        }
        if (g_exit_us < 0l && terminal_process_exited(g_terminal)) {
            g_exit_us = now_us
            g_exit_code = terminal_exit_code(g_terminal)
            if (g_last_data_us == 0l) {
                g_last_data_us = now_us
            }
        }
        if (status == TerminalPollStatus.closed) {
            g_drained_us = now_us
            g_done = true
            break
        }
        if (g_exit_us >= 0l && now_us - g_last_data_us >= 250000l) {
            g_drained_us = now_us
            g_done = true
            break
        }
        break if (status == TerminalPollStatus.idle)
    }
}

[export]
def init() {
    harness_init("dasHerd Chunk 0 - Git terminal", 1100, 720)
    harness_maximize_window()
    g_terminal_font = load_unicode_ui_font(14.0f, g_font_sources)
    let repository = option_value("--repo=", get_das_root())
    select_command(repository)
    let log_dir = path_join(get_das_root(), "logs/dasHerd")
    var mkdir_error : string
    let _ = mkdir_rec(log_dir, mkdir_error)
    g_log_path = path_join(log_dir, "chunk0_{g_command_name}.raw")
    g_log_file = fopen(g_log_path, "wb")
    if (g_log_file == null) {
        g_error = "could not open raw log: {g_log_path}"
    }
    g_started = ref_time_ticks()
    var inscope launched <- terminal_launch(g_command_line, "", 100, 30)
    g_terminal := launched
    g_launch_us = elapsed_us()
    if (terminal_process_id(g_terminal) == 0) {
        g_error = terminal_transport_error(g_terminal)
        g_done = true
    }
}

[export]
def update() {
    if (!harness_begin_frame()) return
    poll_terminal()
    harness_new_frame()
    GetStyle().FontScaleMain = float(g_zoom_percent) / 100.0f
    let viewport = GetMainViewport()
    SetNextWindowPos(viewport.WorkPos, ImGuiCond.Always)
    SetNextWindowSize(viewport.WorkSize, ImGuiCond.Always)
    window(CHUNK0_WIN, (text = "dasHerd Chunk 0 - Git terminal",
            closable = false,
            flags = ImGuiWindowFlags.NoResize | ImGuiWindowFlags.NoMove)) {
        text("Git through ConPTY + terminal emulator + raw log")
        separator()
        let io & = unsafe(GetIO())
        if (IsWindowHovered(ImGuiHoveredFlags.RootAndChildWindows) && io.KeyCtrl) {
            if (io.MouseWheel > 0.0f) {
                zoom_by(5)
            } elif (io.MouseWheel < 0.0f) {
                zoom_by(-5)
            }
        }
        let terminal_style = ImGuiTerminalStyle(
            visual_header = g_visual_header,
            view_height = max(GetTextLineHeight(),
                GetContentRegionAvail().y - GetFrameHeightWithSpacing() * 2.0f
                    - GetStyle().ItemSpacing.y * 2.0f))
        with_font(g_terminal_font) {
            let _ = imgui_terminal("git", g_terminal, g_terminal_view,
                g_terminal_cache, terminal_style)
        }
        separator()
        text_show(CHUNK0_METRICS, ("exit={g_exit_code}  bytes={g_output_bytes}  " +
            "launch={g_launch_us}us  first={g_first_output_us}us  " +
            "exit_seen={g_exit_us}us  drained={g_drained_us}us"))
        text_disabled("raw log: {g_log_path}")
        if (!empty(g_error)) {
            text_colored(CHUNK0_ERROR,
                (color = float4(1.0f, 0.35f, 0.30f, 1.0f), text = g_error))
        }
        if (small_button(CHUNK0_ZOOM_OUT, (text = "-5%"))) {
            zoom_by(-5)
        }
        same_line()
        if (small_button(CHUNK0_ZOOM_IN, (text = "+5%"))) {
            zoom_by(5)
        }
        same_line()
        text("Zoom {g_zoom_percent}%")
    }
    harness_end_frame()
}

[export]
def shutdown() {
    if (!terminal_process_exited(g_terminal)) {
        let _ = terminal_terminate(g_terminal)
    }
    if (g_log_file != null) {
        fflush(g_log_file)
        fclose(g_log_file)
        g_log_file = null
    }
    terminal_viewport_snapshot_dispose(g_terminal_cache.snapshot)
    terminal_dispose(g_terminal)
    delete g_font_sources
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
