options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require imgui public
require imgui/imgui_playwright public
require daslib/json public
require daslib/json_boost public

//! Driver: record plot.apng against examples/tutorial/plot.das.
//! Four read-only sample plots, so there is nothing to click - the self-test instead asserts each
//! plot actually drew with its expected title (record_check_value on the title field; a missing or
//! renamed plot accumulates a miss and aborts at teardown). The subject advances g_phase per frame,
//! so the sine wave animates under the narration - read time naturally shows the motion.

[export]
def main {
    with_recording_app("examples/tutorial/plot.das",
                       "plot.apng", 40) $(app) {
        let T_LINES   = "PL_WIN/PL_LINES"
        let T_HIST    = "PL_WIN/PL_HIST"
        let T_LINES_G = "PL_WIN/PL_LINES_G"
        let T_HIST_G  = "PL_WIN/PL_HIST_G"

        var snap = wait_for_render(app, T_LINES, 10.0f)
        if (snap == null) {
            panic("{T_LINES} never rendered - wrong app running?")
        }

        // ---- Stage 1: plot_lines (array) - sine wave ----
        move_to(app, widget_click_point(snap, T_LINES), 500)
        hold_content(app, SETTLE_MS)
        let d1 = say_begin(app, "plot_lines - an array of samples", T_LINES,
            [voice = "A plot turns a row of numbers into a picture. This line plot reads a fresh array of samples every frame - here a sine wave - and draws it left to right."])
        record_check_value(app, T_LINES, "title", "Sine wave", 120)
        hold_content(app, d1)

        // ---- Stage 2: plot_histogram (array) - same data, bars ----
        move_to(app, widget_click_point(snap, T_HIST), 800)
        hold_content(app, SETTLE_MS)
        let d2 = say_begin(app, "plot_histogram - same data, bars", T_HIST,
            [voice = "The histogram takes the very same kind of array and draws it as bars instead of a line. Same data going in, a different shape coming out."])
        record_check_value(app, T_HIST, "title", "Histogram", 120)
        hold_content(app, d2)

        // ---- Stage 3: plot_lines_getter (lambda) - a callback, no array ----
        move_to(app, widget_click_point(snap, T_LINES_G), 800)
        hold_content(app, SETTLE_MS)
        let d3 = say_begin(app, "plot_lines_getter - a callback, no array", T_LINES_G,
            [voice = "You don't always need an array. The getter form calls a small function once per sample, so you can plot a signal computed on the fly without storing it anywhere."])
        record_check_value(app, T_LINES_G, "title", "sin(i*0.1)", 120)
        hold_content(app, d3)

        // ---- Stage 4: plot_histogram_getter (lambda) - callback bars ----
        move_to(app, widget_click_point(snap, T_HIST_G), 800)
        hold_content(app, SETTLE_MS)
        let d4 = say_begin(app, "plot_histogram_getter - callback bars", T_HIST_G,
            [voice = "And the same callback trick works for bars. Pick lines or bars purely by which picture reads best for your data."])
        record_check_value(app, T_HIST_G, "title", "abs(sin(i*0.1))", 120)
        hold_content(app, d4)
    }
}
