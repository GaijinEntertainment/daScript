options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // each test frees its model/scheduler before the next loads
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_scheduler   // the scheduler under test

// The continuous-batching scheduler's model-free tests: the stop scan,
// the idle chunk, the checkpoint stops' plan. The model cells of
// `_scheduler_cells.das` run on their models' blocks - the Metal n-gram
// capture cell on the Flash-Next trunk's (`test_gpu_resident_hc.das`).

// ===== stop strings: the hold-back scan on its own, then the stream it ends =====

[test(description = "`stop_scan` releases bytes that cannot begin a stop string, holds a growing partial match across pieces, cuts at a completed match, and with two stop strings cuts at the earliest")]
def test_scheduler_stop_scan(t : T?) {
    t |> run("stop_scan releases what cannot begin a stop string, holds what can, cuts at the first match") @(t : T?) {
        let stops <- ["</think", "STOP"]
        var hold = ""
        var emit = ""
        t |> success(!stop_scan(hold, "hello <", stops, emit), "no match in the first piece")
        t |> equal("hello ", emit, "the bytes that cannot begin a stop string are released")
        t |> equal("<", hold, "... the one that can is held")
        t |> success(!stop_scan(hold, "/thi", stops, emit), "still no match")
        t |> equal("", emit, "a hold that is all partial match releases nothing")
        t |> equal("</thi", hold, "the partial match grows")
        t |> success(stop_scan(hold, "nk> and more", stops, emit), "the match completes across three pieces")
        t |> equal("", emit, "nothing stood before it")
        t |> equal("", hold, "the hold is spent")
        t |> success(!stop_scan(hold, "a<b", stops, emit), "a false start")
        t |> equal("a<b", emit, "released whole: no suffix begins a stop string")
        t |> success(!stop_scan(hold, "xS", stops, emit), "the second stop string's first byte")
        t |> equal("x", emit)
        t |> equal("S", hold)
        t |> success(stop_scan(hold, "TOPPED", stops, emit), "completed by the next piece")
        t |> equal("", emit)
        t |> success(stop_scan(hold, "say STOP now", stops, emit), "a match inside one piece")
        t |> equal("say ", emit, "the text before the match")
        let two <- ["TOP", "ST"]
        t |> success(stop_scan(hold, "xSTOP", two, emit), "two stop strings match")
        t |> equal("x", emit, "the earliest match cuts")
    }
}

[test(description = "`idle_chunk_default` on a GPU is the larger of the 2048-token floor and the driver's prompt window, and on the CPU is the default chunk whatever the window")]
def test_idle_chunk_follows_the_window(t : T?) {
    t |> run("the idle chunk on a GPU is the wider of the 2048 floor and the driver's prompt window; the CPU's is one window") @(t : T?) {
        t |> equal(idle_chunk_default(true, 4096l), 4096l, "the wide window picks the chunk: a 3k prompt sweeps the expert stacks once")
        t |> equal(idle_chunk_default(true, 2048l), GPU_IDLE_CHUNK_TOKENS, "the narrow window is the floor")
        t |> equal(idle_chunk_default(true, 512l), GPU_IDLE_CHUNK_TOKENS, "a window under the floor never lowers it")
        t |> equal(idle_chunk_default(false, 4096l), DEFAULT_CHUNK_TOKENS, "the CPU's idle chunk is one window whatever a device would say")
    }
}

[test(description = "`take_within_stops` leaves a window whole while it holds no more checkpoint stops strictly inside it than a region keeps rows, ends it at the stop past that count, takes the stops in any order, and counts neither a stop on the window's first row nor one on its end")]
def test_take_within_stops(t : T?) {
    t |> run("a window holding more stops than a region keeps rows ends at the stop past the rows") @(t : T?) {
        let stops = fixed_array(48l, 96l, 160l, 239l)
        t |> equal(take_within_stops(stops, 0l, 240l, 4l), 240l, "four rows hold the four stops: the window stands whole")
        t |> equal(take_within_stops(stops, 0l, 240l, 2l), 160l, "two rows: the window ends at the third stop")
        t |> equal(take_within_stops(stops, 0l, 240l, 1l), 96l, "one row: at the second")
        t |> equal(take_within_stops(stops, 160l, 80l, 2l), 80l, "from the third stop the one stop left rides its window whole")
        t |> equal(take_within_stops(stops, 48l, 112l, 1l), 112l, "a stop on the window's first row and one on its end are not inside it: 96 alone is")
        t |> equal(take_within_stops(fixed_array(160l, 48l, 0l, 96l), 0l, 240l, 2l), 160l, "the stops out of order, an unset one among them: still the third by row")
    }
}

def private stops_are(got : int64[4]; a, b, c, d : int64) : bool => got[0] == a && got[1] == b && got[2] == c && got[3] == d

[test(description = "`checkpoint_stops_plan` answers a device-home stream's stops ascending, takes the request's markers in the order given until a region's rows are spent, and drops a marker at or under the matched rows, at or past the prompt's end, or equal to one already taken")]
def test_checkpoint_stops_plan(t : T?) {
    t |> run("a region of fewer rows than markers keeps the first markers in the order given, ascending") @(t : T?) {
        let marks = fixed_array(96l, 160l, 239l, 48l)   // the scheduler's order: the system opening, the stable stop, the messages' end, the opening's head
        let whole = checkpoint_stops_plan(marks, 0l, 240l, 4l)
        t |> success(stops_are(whole, 48l, 96l, 160l, 239l), "four rows: every marker a stop, ascending ({whole})")
        let two = checkpoint_stops_plan(marks, 0l, 240l, 2l)
        t |> success(stops_are(two, 96l, 160l, 0l, 0l), "two rows: the first two markers given, the lowest marker - the last given - not among them ({two})")
        let one = checkpoint_stops_plan(marks, 0l, 240l, 1l)
        t |> success(stops_are(one, 96l, 0l, 0l, 0l), "one row: the first marker alone ({one})")
        let past = checkpoint_stops_plan(marks, 96l, 240l, 4l)
        t |> success(stops_are(past, 160l, 239l, 0l, 0l), "a marker at or under the matched rows is no stop ({past})")
        let ended = checkpoint_stops_plan(marks, 0l, 239l, 4l)
        t |> success(stops_are(ended, 48l, 96l, 160l, 0l), "a marker at the prompt's end is no stop ({ended})")
        let twice = checkpoint_stops_plan(fixed_array(96l, 96l, 239l, 48l), 0l, 240l, 2l)
        t |> success(stops_are(twice, 96l, 239l, 0l, 0l), "a marker equal to one taken spends no row ({twice})")
    }
}
