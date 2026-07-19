options gen2

require terminal/terminal
require strings

def row_text(buffer : TerminalBufferSnapshot const; row, columns : int) : string {
    let text = build_string() $(var writer) {
        for (column in range(columns)) {
            let cell = buffer.cells[row * columns + column]
            if (cell.width != 0) writer |> write(cell.grapheme)
        }
    }
    return rtrim(text)
}

def scrollback_row_text(buffer : TerminalBufferSnapshot const; row, columns : int) : string {
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

def assert_cells_equal(left, right : array<TerminalCell> const) {
    assert(length(left) == length(right), "cell count")
    for (a, b in left, right) {
        assert(a.row == b.row && a.column == b.column, "cell position")
        assert(a.grapheme == b.grapheme, "cell grapheme")
        assert(a.width == b.width, "cell width")
        assert(a.attributes == b.attributes, "cell attributes")
        assert(same_color(a.foreground, b.foreground), "cell foreground")
        assert(same_color(a.background, b.background), "cell background")
        assert(a.hyperlink == b.hyperlink, "cell hyperlink")
    }
}

def assert_snapshots_equal(left, right : TerminalSnapshot const) {
    assert(left.columns == right.columns && left.rows == right.rows, "geometry")
    assert(left.alternate_active == right.alternate_active, "active buffer")
    assert_cells_equal(left.normal.cells, right.normal.cells)
    assert_cells_equal(left.normal.scrollback, right.normal.scrollback)
    assert_cells_equal(left.alternate.cells, right.alternate.cells)
    assert_cells_equal(left.alternate.scrollback, right.alternate.scrollback)
    assert(left.normal.cursor.row == right.normal.cursor.row, "cursor row")
    assert(left.normal.cursor.column == right.normal.cursor.column, "cursor column")
    assert(left.normal.cursor.visible == right.normal.cursor.visible, "cursor visible")
    assert(left.normal.cursor.blinking == right.normal.cursor.blinking, "cursor blinking")
    assert(left.modes.auto_wrap == right.modes.auto_wrap, "auto wrap")
    assert(left.modes.application_cursor_keys == right.modes.application_cursor_keys, "cursor mode")
    assert(left.modes.bracketed_paste == right.modes.bracketed_paste, "paste mode")
    assert(left.title == right.title, "title")
    assert(left.current_directory == right.current_directory, "cwd")
}

[export]
def main {
    var inscope terminal <- terminal_create(8, 2)
    assert(terminal.columns == 8)
    assert(terminal.rows == 2)
    terminal_feed(terminal, "hello")
    var snapshot = TerminalSnapshot()
    terminal_snapshot(terminal, snapshot)
    assert(length(snapshot.normal.cells) == 16)
    assert(snapshot.normal.cells[0].grapheme == "h")
    assert(snapshot.normal.cells[4].grapheme == "o")
    assert(snapshot.normal.cursor.row == 0)
    assert(snapshot.normal.cursor.column == 5)

    var inscope scrolled <- terminal_create(4, 2)
    terminal_feed(scrolled, "a\r\nb\r\nc\r\nd")
    terminal_snapshot(scrolled, snapshot)
    assert(snapshot.normal.scrollback_rows == 2)
    assert(snapshot.normal.scrollback[0].grapheme == "a")
    assert(snapshot.normal.scrollback[4].grapheme == "b")
    assert(snapshot.normal.cells[0].grapheme == "c")
    assert(snapshot.normal.cells[4].grapheme == "d")
    var viewport = TerminalViewportSnapshot()
    terminal_viewport_snapshot(scrolled, 0, viewport)
    assert(viewport.buffer.scrollback_rows == 2)
    assert(viewport.screen_text == "c\nd")
    assert(length(viewport.text_runs) == 2)
    assert(viewport.text_runs[0].text == "c")
    assert(viewport.text_runs[1].text == "d")
    terminal_viewport_snapshot(scrolled, 2, viewport)
    assert(length(viewport.text_runs) == 4)
    assert(viewport.text_runs[0].global_row == 0)
    assert(viewport.text_runs[0].text == "a")

    var inscope styled <- terminal_create(8, 2)
    let esc = to_char(27)
    terminal_feed(styled, "{esc}[31mA{esc}[1mB{esc}[0mC")
    terminal_snapshot(styled, snapshot)
    assert(snapshot.normal.cells[0].foreground.kind == TerminalColorKind.indexed)
    assert(snapshot.normal.cells[0].foreground.index == 1)
    assert((snapshot.normal.cells[1].attributes & TERMINAL_ATTR_BOLD) != 0)
    assert(snapshot.normal.cells[2].foreground.kind == TerminalColorKind.default_color)
    terminal_viewport_snapshot(styled, 0, viewport)
    assert(length(viewport.buffer.cells) == 1)
    assert(viewport.buffer.cells[0].column == 1)

    let bytes <- [uint8(0xe4), uint8(0xb8), uint8(0xad)]
    terminal_feed_bytes(styled, bytes)
    terminal_snapshot(styled, snapshot)
    assert(snapshot.normal.cells[3].width == 2)

    var key_bytes : array<uint8>
    terminal_encode_key_bytes(terminal, TerminalKeyEvent(key = TerminalKey.up), key_bytes)
    assert(string(key_bytes) == "{esc}[A")
    terminal_feed(terminal, "{esc}[?2004h")
    terminal_encode_paste_bytes(terminal, "hello", key_bytes)
    assert(string(key_bytes) == "{esc}[200~hello{esc}[201~")

    var inscope erased <- terminal_create(6, 3)
    terminal_feed(erased, "abcdef\r\nghijkl\r\nmnopqr{esc}[2;3H{esc}[1J")
    terminal_snapshot(erased, snapshot)
    let erased_row_0 = row_text(snapshot.normal, 0, 6)
    let erased_row_1 = row_text(snapshot.normal, 1, 6)
    let erased_row_2 = row_text(snapshot.normal, 2, 6)
    assert(erased_row_0 == "")
    assert(erased_row_1 == "   jkl")
    assert(erased_row_2 == "mnopqr")
    assert(snapshot.normal.cursor.row == 1 && snapshot.normal.cursor.column == 2)

    var inscope resized <- terminal_create(4, 1)
    let wide_bytes <- [uint8('A'), uint8(0xe7), uint8(0x95), uint8(0x8c),
        uint8('\r'), uint8('\n')]
    terminal_feed_bytes(resized, wide_bytes)
    terminal_resize(resized, 2, 1)
    terminal_snapshot(resized, snapshot)
    assert(snapshot.normal.scrollback_rows == 1)
    assert(snapshot.normal.scrollback[0].grapheme == "A")
    assert(snapshot.normal.scrollback[1].grapheme == " ")
    assert(snapshot.normal.scrollback[1].width == 1)

    var inscope protocol <- terminal_create(10, 3)
    let bel = to_char(7)
    terminal_feed(protocol, "main{esc}]2;PowerShell{bel}{esc}]7;file:///repo{esc}\\{esc}[?25l{esc}[?1049halt{esc}[6n")
    terminal_snapshot(protocol, snapshot)
    assert(snapshot.title == "PowerShell")
    assert(snapshot.current_directory == "file:///repo")
    assert(snapshot.alternate_active)
    assert(!snapshot.alternate.cursor.visible)
    let alternate_row = row_text(snapshot.alternate, 0, 10)
    let normal_row = row_text(snapshot.normal, 0, 10)
    assert(alternate_row == "alt")
    assert(normal_row == "main")
    var replies : array<string>
    terminal_drain_replies(protocol, replies)
    assert(length(replies) == 1 && replies[0] == "{esc}[1;4R")
    terminal_drain_replies(protocol, replies)
    assert(empty(replies))
    var events : array<string>
    terminal_take_events(protocol, events)
    assert(length(events) == 2)
    assert(events[0] == "title:PowerShell")
    assert(events[1] == "cwd:file:///repo")
    terminal_feed(protocol, "{bel}")
    terminal_take_events(protocol, events)
    assert(length(events) == 1 && events[0] == "bell")
    terminal_take_events(protocol, events)
    assert(empty(events))

    terminal_feed(protocol, "{esc}[?1049l{esc}[5n")
    var byte_replies : array<array<uint8>>
    terminal_drain_replies_bytes(protocol, byte_replies)
    assert(length(byte_replies) == 1 && string(byte_replies[0]) == "{esc}[0n")
    terminal_drain_replies_bytes(protocol, byte_replies)
    assert(empty(byte_replies))

    var inscope chunked <- terminal_create(12, 4)
    var inscope complete <- terminal_create(12, 4)
    let chunk_stream = "hello\r\n{esc}[1;34mblue{esc}[0m " + string(wide_bytes) + "\r\n{esc}]2;chunk test{esc}\\{esc}[?2004h{esc}[2;5H!"
    terminal_feed(complete, chunk_stream)
    for (byte in chunk_stream) {
        var one : array<uint8>
        one |> push(uint8(byte))
        terminal_feed_bytes(chunked, one)
    }
    var complete_snapshot = TerminalSnapshot()
    var chunked_snapshot = TerminalSnapshot()
    terminal_snapshot(complete, complete_snapshot)
    terminal_snapshot(chunked, chunked_snapshot)
    assert_snapshots_equal(complete_snapshot, chunked_snapshot)

    var inscope bounded <- terminal_create(8, 2)
    terminal_feed(bounded, "{esc}[" + repeat(";", 5000) + "mX")
    terminal_feed(bounded, "{esc}]0;" + repeat("a", 70 * 1024) + "{esc}\\Y")
    terminal_snapshot(bounded, snapshot)
    let bounded_row = row_text(snapshot.normal, 0, 8)
    assert(bounded_row == "XY")
    assert(empty(snapshot.title))

    let shift_left = terminal_encode_key(terminal, TerminalKeyEvent(
        key = TerminalKey.left, shift = true))
    let control_alt_right = terminal_encode_key(terminal, TerminalKeyEvent(
        key = TerminalKey.right, alt = true, control = true))
    let control_delete = terminal_encode_key(terminal, TerminalKeyEvent(
        key = TerminalKey.delete_key, control = true))
    let alt_f12 = terminal_encode_key(terminal, TerminalKeyEvent(
        key = TerminalKey.f12, alt = true))
    assert(shift_left == "{esc}[1;2D")
    assert(control_alt_right == "{esc}[1;7C")
    assert(control_delete == "{esc}[3;5~")
    assert(alt_f12 == "{esc}[24;3~")

    var inscope capped <- terminal_create(8, 1)
    let many_lines = build_string() $(var writer) {
        for (line in range(10005)) {
            writer |> write("L{line}\r\n")
        }
    }
    terminal_feed(capped, many_lines)
    terminal_snapshot(capped, snapshot)
    assert(snapshot.normal.scrollback_rows == 10000)
    let oldest_row = scrollback_row_text(snapshot.normal, 0, 8)
    let newest_row = scrollback_row_text(snapshot.normal, 9999, 8)
    assert(oldest_row == "L5")
    assert(newest_row == "L10004")

    var inscope reset <- terminal_create(8, 2)
    terminal_feed(reset, "before{bel}{esc}]2;title{bel}{esc}[5n{esc}c")
    terminal_snapshot(reset, snapshot)
    let reset_row = row_text(snapshot.normal, 0, 8)
    assert(reset_row == "")
    assert(empty(snapshot.title))
    terminal_drain_replies(reset, replies)
    terminal_take_events(reset, events)
    assert(empty(replies) && empty(events))

    var inscope application_keys <- terminal_create(8, 2)
    terminal_feed(application_keys, "{esc}[?1h")
    let application_up = terminal_encode_key(application_keys,
        TerminalKeyEvent(key = TerminalKey.up))
    assert(application_up == "{esc}OA")
    terminal_feed(application_keys, "{esc}[?1l")
    let normal_home = terminal_encode_key(application_keys,
        TerminalKeyEvent(key = TerminalKey.home))
    assert(normal_home == "{esc}[H")

    terminal_dispose(application_keys)
    terminal_dispose(reset)
    terminal_dispose(capped)
    terminal_dispose(bounded)
    terminal_dispose(complete)
    terminal_dispose(chunked)
    terminal_dispose(protocol)
    terminal_dispose(resized)
    terminal_dispose(erased)
    terminal_dispose(styled)
    terminal_dispose(scrolled)
    terminal_dispose(terminal)
}
