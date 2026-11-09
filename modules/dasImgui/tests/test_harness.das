options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require imgui/imgui_playwright public
require daslib/fio
require daslib/json public
require daslib/json_boost public
require daslib/module_path
require strings

//! The test harness and the app foundation under it: host lifecycle,
//! the widget registry contract, the playwright API end to end, the
//! framebuffer a harness window gets, and the GLFW-free visual aids.

[test(description = "with_imgui_app spawns a daslang-live host that answers /status, then posts /shutdown and drains it, with no widget interaction", timeout = 1200)]
def test_spawn_and_shutdown(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/triggers.das") $(_d) {
        // Body intentionally empty. with_imgui_app's prelude already gated
        // on /status; the postlude POSTs /shutdown and drains. If we get
        // here, daslang-live is alive and answering HV.
    }
    t |> success(true, "harness spawned daslang-live, posted /shutdown, exited cleanly")
}

[test(description = "widgets auto-emit registry keys under bare identifiers despite dotted-suffix flags, serialize per-kind payloads, and answer imgui_click and imgui_force_set through one dispatch table", timeout = 1200)]
def test_foundation_smoke(t : T?) {
    with_imgui_app("modules/dasImgui/examples/features/foundation.das") $(d) {
        // Gate 1: auto-emit — SAVE_BTN exists in registry without any manual decl.
        var snap0 = wait_for_widget(d, "MAIN_WIN/SAVE_BTN", 15.0f)
        t |> success(snap0 != null, "SAVE_BTN auto-emitted (registry has it)")
        if (snap0 == null) return

        // Gate 2: dotted-suffix flags — bare identifiers as registry keys.
        // SPEED came from `slider_int(SPEED.PUBLIC, ...)`; key must be "MAIN_WIN/SPEED",
        // not "SPEED.PUBLIC". Same for VOLUME.NOTLIVE → "MAIN_WIN/VOLUME".
        t |> success(widget_exists(snap0, "MAIN_WIN/SPEED"), "SPEED.PUBLIC keys as 'SPEED'")
        t |> success(widget_exists(snap0, "MAIN_WIN/VOLUME"), "VOLUME.NOTLIVE keys as 'VOLUME'")
        t |> success(!widget_exists(snap0, "MAIN_WIN/SPEED.PUBLIC"), "no 'SPEED.PUBLIC' literal key")
        t |> success(!widget_exists(snap0, "MAIN_WIN/VOLUME.NOTLIVE"), "no 'VOLUME.NOTLIVE' literal key")

        // Gate 3: lazy serializer — payload shapes match per kind.
        var save = find_widget(snap0, "MAIN_WIN/SAVE_BTN")
        t |> equal(save?["kind"] ?? "", "button", "SAVE_BTN.kind")
        t |> success(save?["payload"]?["click_count"] != null, "ClickState payload.click_count")

        var speed = find_widget(snap0, "MAIN_WIN/SPEED")
        t |> equal(speed?["kind"] ?? "", "slider_int", "SPEED.kind")
        t |> success(speed?["payload"]?["value"] != null, "SliderStateInt payload.value")
        t |> success(speed?["payload"]?["bounds"] != null, "SliderStateInt payload.bounds")

        var volume = find_widget(snap0, "MAIN_WIN/VOLUME")
        t |> equal(volume?["kind"] ?? "", "slider_float", "VOLUME.kind")
        t |> success(volume?["payload"]?["value"] != null, "SliderStateFloat payload.value")

        // Gate 4: unified dispatch — imgui_click and imgui_force_set both work.
        let count_before = save?["payload"]?["click_count"] ?? 0
        click(d, "MAIN_WIN/SAVE_BTN")
        t |> success(wait_for_int_value(d, "MAIN_WIN/SAVE_BTN", "click_count", count_before + 1, 300),
                     "imgui_click bumps SAVE_BTN.click_count")

        force_set_value(d, "MAIN_WIN/SPEED", JV(7))
        t |> success(wait_for_int_value(d, "MAIN_WIN/SPEED", "value", 7, 300),
                     "imgui_force_set steers SPEED.value to 7")
    }
}

[test(description = "against the save_demo app, expect_render, click, type_text with auto-focus and the panicking expect_value form work end to end", timeout = 1200)]
def test_save_flow(t : T?) {
    with_imgui_app("modules/dasImgui/examples/save_demo/main.das") $(app) {
        // Subprocess up + first frame painted. expect_render's failure
        // message would list every registered widget — useful sanity.
        expect_render(app, "MAIN_WIN/SAVE_BTN", 15.0f)

        // Click → STATUS_TEXT flips to "saved".
        click(app, "MAIN_WIN/SAVE_BTN")
        expect_value(app, "MAIN_WIN/STATUS_TEXT", "value", "saved", 300)

        // type_text auto-focuses NAME_INPUT, settles, then sends chars.
        // expect_value confirms the chars landed on state.value.
        type_text(app, "MAIN_WIN/NAME_INPUT", "hello")
        expect_value(app, "MAIN_WIN/NAME_INPUT", "value", "hello", 300)

        t |> success(true, "save_demo end-to-end: click + type_text auto-focus + expect_value")
    }
}

[test(description = "a harness window created with zero samples gets a single-sample OpenGL framebuffer, skipping when headless has no framebuffer", timeout = 1200)]
def zero_samples_creates_single_sample_framebuffer(t : T?) {
    with_imgui_app(get_das_root() + "/modules/dasImgui/tests/harness_samples_app.das") $(d) {
        var snap = wait_for_widget(d, "SAMPLES_WINDOW/SAMPLES_VALUE", 15.0)
        t |> success(snap != null, "framebuffer probe starts")
        if (snap == null) {
            return
        }
        let value = widget_payload_field(snap, "SAMPLES_WINDOW/SAMPLES_VALUE", "value") ?? ""
        if (value == "-1") {
            delete_json(snap)
            t |> skip("a headless harness has no OpenGL framebuffer")
            return
        }
        t |> equal(value, "0", "explicit zero disables multisampling on a real framebuffer")
        delete_json(snap)
    }
}

def private starts_with_require(line : string) : bool {
    let trimmed = chop(line, 0, length(line))
    return false if (length(trimmed) < 8)
    return slice(trimmed, 0, 8) == "require "
}

def private require_token(line : string) : string {
    // Strip `require ` and any trailing ` public`/`private`, return the module path.
    var tok = slice(line, 8, length(line))
    // Drop trailing modifiers.
    for (suffix in [" public", " private"]) {
        if (length(tok) > length(suffix) && ends_with(tok, suffix)) {
            tok = chop(tok, 0, length(tok) - length(suffix)) // nolint:PERF031 - one require line, at most two bounded strips
        }
    }
    return tok
}

def private file_lines(path : string; var lines : array<string>) : bool {
    var ok = false
    fopen(path, "r") $(f) {
        if (f == null) return
        ok = true
        while (true) {
            let line = f |> fgets()
            break if (empty(line))
            lines |> push(chop(line, 0, length(line)))   // keep trailing newline if present; pattern checks are length-bounded
        }
    }
    return ok
}

//! A source grep, not an RTTI walk: the imgui_app C++ binding
//! requires glfw unconditionally, so a transitive walk always
//! finds it.
[test(description = "widgets/imgui_visual_aids.das names no glfw/ or live/glfw_ module in its requires, so the visual aids load without GLFW")]
def test_imgui_visual_aids_has_no_direct_glfw_require(t : T?) {
    // Resolve the target via the das root, not cwd: the module is in-tree at
    // modules/dasImgui since the merge (the test itself lives in modules/dasImgui/tests).
    // Must be inside a function (-exe top-level let baking).
    let target = "{get_das_root()}/modules/dasImgui/widgets/imgui_visual_aids.das"
    var lines : array<string>
    let ok = file_lines(target, lines)
    t |> success(ok, "open {target}")
    return if (!ok)

    var offenders : array<string>
    for (line in lines) {
        if (!starts_with_require(line)) continue
        let mod = require_token(line)
        // Direct glfw deps live under `glfw/...` or `live/glfw_*`.
        if (length(mod) >= 5 && slice(mod, 0, 5) == "glfw/") {
            offenders |> push(line)
        }
        if (length(mod) >= 10 && slice(mod, 0, 10) == "live/glfw_") {
            offenders |> push(line)
        }
    }
    t |> equal(length(offenders), 0,
               "imgui_visual_aids must not directly require any GLFW module - see project_dasimgui_cpp_glfw_split_deferred")
    for (off in offenders) {
        to_log(LOG_ERROR, "test_harness: offending require in {target}: {off}")
    }
}
