options gen2

require dastest/testing_boost
require llvm/daslib/llvm_tune
require strings

// The progress protocol, driven through its own front-end seam (tune_progress_feed +
// tune_progress_line) so none of it depends on the test process owning a terminal.

def private feed_plan(total : int) {
    tune_progress_reset()
    tune_progress_feed("@tune plan scope=probe total={total}")
}

[test]
def test_non_event_lines_are_not_events(t : T?) {
    tune_progress_reset()
    t |> success(!tune_progress_feed("kernel tuner: default, N=4096"))
    t |> success(!tune_progress_feed("  vec8_u2: 1234 us  (0.0% vs vec8_u2)"))
    t |> success(!tune_progress_feed("CONFIRM_PP: 41.5"))
    t |> success(!tune_progress_active(), "chatter must not start a display")
}

[test]
def test_plan_starts_a_display(t : T?) {
    feed_plan(25)
    t |> success(tune_progress_active())
    let line = tune_progress_line(100)
    t |> success(line |> find("0/25") >= 0, "outer counter present")
    t |> success(line |> find("[") == 0, "the bar leads the line")
}

[test]
def test_kernel_counters_and_phase_render(t : T?) {
    feed_plan(25)
    t |> success(tune_progress_feed("@tune begin name=dot_q8q8 total=80"))
    t |> success(tune_progress_feed("@tune step i=47 phase=finalists live=4"))
    let line = tune_progress_line(100)
    t |> success(line |> find("dot_q8q8") >= 0, "kernel name")
    t |> success(line |> find("finalists 47/80") >= 0, "pass and inner counter")
    t |> success(line |> find("4 live") >= 0, "live-candidate count")
}

[test]
def test_end_advances_the_outer_counter(t : T?) {
    feed_plan(3)
    tune_progress_feed("@tune begin name=a total=10")
    tune_progress_feed("@tune end name=a winner=v1 verdict=beats")
    t |> success(tune_progress_line(100) |> find("1/3") >= 0)
    tune_progress_feed("@tune begin name=b total=10")
    tune_progress_feed("@tune end name=b winner=v2 verdict=holds")
    t |> success(tune_progress_line(100) |> find("2/3") >= 0)
}

[test]
def test_inner_counter_drops_out_between_kernels(t : T?) {
    feed_plan(3)
    tune_progress_feed("@tune begin name=a total=10")
    tune_progress_feed("@tune step i=5 phase=screening live=9")
    t |> success(tune_progress_line(100) |> find("screening 5/10") >= 0)
    tune_progress_feed("@tune end name=a winner=v1 verdict=beats")
    let line = tune_progress_line(100)
    t |> success(line |> find("/10") < 0, "a finished kernel leaves no stale inner counter")
    t |> success(line |> find("live") < 0, "nor a stale live count")
}

[test]
def test_bar_fills_with_the_outer_counter(t : T?) {
    feed_plan(4)
    let empty_bar = tune_progress_line(100)
    for (i in range(4)) {
        tune_progress_feed("@tune begin name=k{i} total=2")
        tune_progress_feed("@tune end name=k{i} winner=w verdict=holds")
    }
    let full_bar = tune_progress_line(100)
    t |> success(empty_bar |> find("#") < 0, "nothing filled at 0/4")
    t |> success(full_bar |> find("-") < 0, "nothing unfilled at 4/4")
    t |> success(full_bar |> find("4/4") >= 0)
}

[test]
def test_line_respects_a_narrow_terminal(t : T?) {
    feed_plan(25)
    tune_progress_feed("@tune begin name=dot_q8q8 total=80")
    // the bar floor is 8 cells, so a cramped width shrinks the bar rather than the counters
    let narrow = tune_progress_line(20)
    t |> success(narrow |> find("0/25") >= 0, "counters survive a narrow terminal")
    t |> success(narrow |> find("[--------]") >= 0, "the bar clamps to its 8-cell floor")
}

[test]
def test_malformed_events_do_not_break_the_display(t : T?) {
    feed_plan(5)
    t |> success(tune_progress_feed("@tune "), "a bare prefix is still an event")
    t |> success(tune_progress_feed("@tune step i=notanumber phase= live="))
    t |> success(tune_progress_feed("@tune wat name=x"))
    t |> success(tune_progress_active(), "the display survives garbage")
    t |> success(tune_progress_line(100) |> find("0/5") >= 0)
}

[test]
def test_verbosity_parses_and_defaults(t : T?) {
    tune_set_verbosity("silent")
    t |> equal(tune_verbosity(), "silent")
    tune_set_verbosity("verbose")
    t |> equal(tune_verbosity(), "verbose")
    tune_set_verbosity("normal")
    t |> equal(tune_verbosity(), "normal")
    // a typo must never silence the tune
    tune_set_verbosity("quiet")
    t |> equal(tune_verbosity(), "normal")
    tune_set_verbosity("")
    t |> equal(tune_verbosity(), "normal")
}
