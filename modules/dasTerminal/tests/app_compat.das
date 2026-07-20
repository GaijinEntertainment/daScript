options gen2
options rtti

require terminal/terminal
require daslib/json_boost
require daslib/fio
require strings

struct CompatResize {
    columns : int
    rows : int
}

struct CompatStep {
    feed : array<uint8>
    resize : CompatResize?
    checkpoint : string
}

struct CompatScenario {
    id : string
    source : string
    columns : int
    rows : int
    steps : array<CompatStep>
}

struct CompatCorpus {
    version : int
    scenarios : array<CompatScenario>
}

struct CompatExpectedCheckpoint {
    checkpoint : string
    screen : array<string>
    history : array<string>
    cursor : TerminalCursor = TerminalCursor()
    alternate_active : bool
    title : string
    replies : array<uint8>
    cells : array<TerminalCell>
}

struct CompatExpectedScenario {
    id : string
    checkpoints : array<CompatExpectedCheckpoint>
}

struct CompatOracle {
    oracle : string
    version : int
    scenarios : array<CompatExpectedScenario>
}

def row_text(buffer : TerminalBufferSnapshot const; row, columns : int) : string {
    let text = build_string() $(var writer) {
        for (column in range(columns)) {
            let cell = buffer.cells[row * columns + column]
            if (cell.width != 0) writer |> write(cell.grapheme)
        }
    }
    return rtrim(text)
}

def history_text(buffer : TerminalBufferSnapshot const; row, columns : int) : string {
    let text = build_string() $(var writer) {
        for (column in range(columns)) {
            let cell = buffer.scrollback[row * columns + column]
            if (cell.width != 0) writer |> write(cell.grapheme)
        }
    }
    return rtrim(text)
}

def same_color(left, right : TerminalColor const) : bool {
    return (left.kind == right.kind && left.index == right.index &&
        left.red == right.red && left.green == right.green && left.blue == right.blue)
}

def default_color(color : TerminalColor const) : bool {
    return color.kind == TerminalColorKind.default_color
}

def relevant_cell(cell : TerminalCell const) : bool {
    return (cell.grapheme != " " || cell.width != 1 || cell.attributes != 0 ||
        !default_color(cell.foreground) || !default_color(cell.background))
}

def check(condition : bool; message : string) {
    if (!condition) panic(message)
}

def assert_cell(actual, expected : TerminalCell const; context : string) {
    check(actual.row == expected.row && actual.column == expected.column,
        "{context}: cell position")
    check(actual.grapheme == expected.grapheme, "{context}: cell grapheme")
    check(actual.width == expected.width, "{context}: cell width")
    check(actual.attributes == expected.attributes, "{context}: cell attributes")
    check(same_color(actual.foreground, expected.foreground), "{context}: cell foreground")
    check(same_color(actual.background, expected.background), "{context}: cell background")
}

def assert_buffer(buffer : TerminalBufferSnapshot const;
                  expected : CompatExpectedCheckpoint const;
                  columns, rows : int; context : string) {
    check(length(expected.screen) == rows, "{context}: screen row count")
    for (row in range(rows)) {
        let actual_text = row_text(buffer, row, columns)
        check(actual_text == expected.screen[row],
            "{context}: screen row {row}: got '{actual_text}', expected '{expected.screen[row]}'")
    }
    check(buffer.scrollback_rows == length(expected.history),
        "{context}: history row count {buffer.scrollback_rows}, expected {length(expected.history)}")
    for (row in range(buffer.scrollback_rows)) {
        check(history_text(buffer, row, columns) == expected.history[row],
            "{context}: history row {row}")
    }
    check(buffer.cursor.row == expected.cursor.row,
        "{context}: cursor row {buffer.cursor.row}, expected {expected.cursor.row}")
    check(buffer.cursor.column == expected.cursor.column,
        "{context}: cursor column {buffer.cursor.column}, expected {expected.cursor.column}")

    var expected_index = 0
    for (cell in buffer.cells) {
        if (!relevant_cell(cell)) continue
        check(expected_index < length(expected.cells), "{context}: extra terminal cell")
        assert_cell(cell, expected.cells[expected_index], context)
        expected_index++
    }
    check(expected_index == length(expected.cells), "{context}: missing terminal cell")
}

def assert_checkpoint(var terminal : Terminal; expected : CompatExpectedCheckpoint const;
                      scenario_id : string) {
    var snapshot = TerminalSnapshot()
    terminal_snapshot(terminal, snapshot)
    let context = "{scenario_id}/{expected.checkpoint}"
    check(snapshot.alternate_active == expected.alternate_active,
        "{context}: active buffer")
    check(snapshot.title == expected.title, "{context}: title")

    var reply_chunks : array<array<uint8>>
    terminal_drain_replies_bytes(terminal, reply_chunks)
    var replies : array<uint8>
    for (chunk in reply_chunks) {
        replies |> push_from(chunk)
    }
    check(length(replies) == length(expected.replies),
        "{context}: reply byte count {length(replies)}, expected {length(expected.replies)}")
    for (index in range(length(replies))) {
        check(replies[index] == expected.replies[index],
            "{context}: reply byte {index}: got {replies[index]}, expected {expected.replies[index]}")
    }
    if (expected.alternate_active) {
        assert_buffer(snapshot.alternate, expected, snapshot.columns, snapshot.rows, context)
    } else {
        assert_buffer(snapshot.normal, expected, snapshot.columns, snapshot.rows, context)
    }
}

def find_expected(oracle : CompatOracle const; id : string) : CompatExpectedScenario const? {
    for (scenario in oracle.scenarios) {
        if (scenario.id == id) {
            unsafe { return addr(scenario) }
        }
    }
    return null
}

[export]
def main {
    let fixture_root = "{get_das_root()}/modules/dasTerminal/tests/app_compat"
    var corpus : CompatCorpus
    var oracle : CompatOracle
    check(sscan_json(fread("{fixture_root}/scenarios.json"), corpus),
        "parse app compatibility scenarios")
    check(sscan_json(fread("{fixture_root}/oracle-snapshots.json"), oracle),
        "parse xterm oracle snapshots")
    assert(corpus.version == oracle.version, "fixture version")
    assert(oracle.oracle == "@xterm/headless@5.5.0 + @xterm/addon-unicode11@0.8.0",
        "pinned oracle")
    assert(length(corpus.scenarios) == length(oracle.scenarios), "scenario count")

    for (scenario in corpus.scenarios) {
        var inscope terminal <- terminal_create(scenario.columns, scenario.rows)
        let expected = find_expected(oracle, scenario.id)
        check(expected != null, "missing oracle scenario {scenario.id}")
        var checkpoint_index = 0
        for (step in scenario.steps) {
            if (!empty(step.feed)) terminal_feed_bytes(terminal, step.feed)
            if (step.resize != null) {
                unsafe {
                    terminal_resize(terminal, step.resize.columns, step.resize.rows)
                }
            }
            if (!empty(step.checkpoint)) {
                unsafe {
                    check(checkpoint_index < length(expected.checkpoints),
                        "extra checkpoint {scenario.id}/{step.checkpoint}")
                    check(expected.checkpoints[checkpoint_index].checkpoint == step.checkpoint,
                        "checkpoint order {scenario.id}/{step.checkpoint}")
                    assert_checkpoint(terminal, expected.checkpoints[checkpoint_index], scenario.id)
                    checkpoint_index++
                }
            }
        }
        unsafe {
            check(checkpoint_index == length(expected.checkpoints),
                "missing checkpoint {scenario.id}")
        }
    }
}
