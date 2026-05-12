options gen2
options no_aot

require dastest/testing_boost public

require live/mouse_events

[test]
def test_sort_by_tms_stable(t : T?) {
    var events <- [
        MouseEventWire(t_ms = 300, kind = "move", x = 3.0),
        MouseEventWire(t_ms = 100, kind = "move", x = 1.0),
        MouseEventWire(t_ms = 200, kind = "move", x = 2.0),
        MouseEventWire(t_ms = 100, kind = "move", x = 1.5) // ties with previous
    ]
    sort_play_events(events)
    t |> equal(events[0].t_ms, 100)
    t |> equal(events[1].t_ms, 100)
    t |> equal(events[2].t_ms, 200)
    t |> equal(events[3].t_ms, 300)
    // Stable: 1.0 came before 1.5 in input, must stay so on equal t_ms.
    t |> equal(events[0].x, 1.0)
    t |> equal(events[1].x, 1.5)
}

[test]
def test_sort_empty(t : T?) {
    var events : array<MouseEventWire>
    sort_play_events(events)
    t |> equal(length(events), 0)
}

[test]
def test_sort_already_sorted(t : T?) {
    var events <- [
        MouseEventWire(t_ms = 100),
        MouseEventWire(t_ms = 200),
        MouseEventWire(t_ms = 300)
    ]
    sort_play_events(events)
    t |> equal(events[0].t_ms, 100)
    t |> equal(events[1].t_ms, 200)
    t |> equal(events[2].t_ms, 300)
}
