options gen2
options indenting = 4

module terminal shared

require terminal_core public
require math
require strings

let private MAX_CSI_SEQUENCE_BYTES = 4096
let private MAX_OSC_PAYLOAD_BYTES = 64 * 1024
let private MAX_UNKNOWN_SEQUENCE_BYTES = 4096
let private MAX_UNKNOWN_SEQUENCES = 256
let private MAX_SCROLLBACK_ROWS = 10000

let public TERMINAL_ATTR_BOLD = 1 << 0
let public TERMINAL_ATTR_FAINT = 1 << 1
let public TERMINAL_ATTR_ITALIC = 1 << 2
let public TERMINAL_ATTR_UNDERLINE = 1 << 3
let public TERMINAL_ATTR_BLINK = 1 << 4
let public TERMINAL_ATTR_INVERSE = 1 << 5
let public TERMINAL_ATTR_HIDDEN = 1 << 6
let public TERMINAL_ATTR_STRIKE = 1 << 7

enum public TerminalColorKind {
    default_color
    indexed
    rgb
}

enum public TerminalKey {
    text
    enter
    tab
    backspace
    escape
    up
    down
    right
    left
    home
    end
    insert
    delete_key
    page_up
    page_down
    f1
    f2
    f3
    f4
    f5
    f6
    f7
    f8
    f9
    f10
    f11
    f12
}

enum public TerminalPollStatus {
    data
    idle
    closed
    error
}

enum private TerminalParseState {
    ground
    escape
    csi
    csi_ignore
    osc
    osc_escape
    osc_ignore
    osc_ignore_escape
}

struct public TerminalKeyEvent {
    key : TerminalKey = TerminalKey.text
    text : string
    shift : bool
    alt : bool
    control : bool
}

struct public TerminalColor {
    kind : TerminalColorKind = TerminalColorKind.default_color
    index : int
    red : int
    green : int
    blue : int
}

struct public TerminalCell {
    row : int
    column : int
    grapheme : string = " "
    width : int = 1
    attributes : int
    foreground : TerminalColor = TerminalColor()
    background : TerminalColor = TerminalColor()
    hyperlink : string
}

struct public TerminalCursor {
    row : int
    column : int
    visible : bool = true
    blinking : bool = true
    style : int
}

struct public TerminalBufferSnapshot {
    cells : array<TerminalCell>
    scrollback : array<TerminalCell>
    scrollback_rows : int
    cursor : TerminalCursor = TerminalCursor()
}

struct public TerminalModes {
    auto_wrap : bool = true
    application_cursor_keys : bool
    bracketed_paste : bool
    focus_reporting : bool
    mouse_button_reporting : bool
    mouse_any_reporting : bool
    sgr_mouse_encoding : bool
}

struct public TerminalSnapshot {
    columns : int
    rows : int
    alternate_active : bool
    normal : TerminalBufferSnapshot = TerminalBufferSnapshot()
    alternate : TerminalBufferSnapshot = TerminalBufferSnapshot()
    modes : TerminalModes = TerminalModes()
    title : string
    current_directory : string
    unknown_sequences : array<string>
}

struct public TerminalTextRun {
    global_row : int
    column : int
    text : string
    color : TerminalColor = TerminalColor()
    default_is_background : bool
    glyph_cells : int
}

struct public TerminalViewportSnapshot {
    revision : int64
    columns : int
    rows : int
    alternate_active : bool
    buffer : TerminalBufferSnapshot = TerminalBufferSnapshot()
    text_runs : array<TerminalTextRun>
    screen_text : string
    cell_projection_us : int
    text_run_projection_us : int
}

struct private TerminalRow {
    cells : array<TerminalCell>
}

struct private TerminalBuffer {
    screen : array<TerminalRow>
    screen_start : int
    history : array<TerminalRow>
    history_start : int
    cursor : TerminalCursor = TerminalCursor()
    saved_cursor : TerminalCursor = TerminalCursor()
    pen : TerminalCell = TerminalCell()
    wrap_pending : bool
}

typedef public Pty = smart_ptr<pty_handle>

struct public Terminal {
    columns : int
    rows : int
    alternate_active : bool
    modes : TerminalModes = TerminalModes()
    title : string
    current_directory : string
    unknown_sequences : array<string>
    replies : array<array<uint8>>
    events : array<string>
    revision : int64

    normal : TerminalBuffer = TerminalBuffer()
    alternate : TerminalBuffer = TerminalBuffer()

    parse_state : TerminalParseState = TerminalParseState.ground
    sequence : array<uint8>
    csi_parameters : array<uint8>
    osc_data : array<uint8>
    utf8_codepoint : uint
    utf8_minimum : uint
    utf8_remaining : int
    ascii_graphemes : array<string>

    transport : Pty
    read_buffer : array<uint8>
}

def private blank_cell(row, column : int) : TerminalCell {
    return TerminalCell(row = row, column = column)
}

def private blank_row(columns, row : int) : TerminalRow {
    var result = TerminalRow()
    result.cells |> reserve(columns)
    for (column in range(columns)) {
        result.cells |> push(blank_cell(row, column))
    }
    return <- result
}

def private reset_buffer(var buffer : TerminalBuffer; columns, rows : int) {
    buffer.screen |> clear()
    buffer.screen |> reserve(rows)
    for (row in range(rows)) {
        var next <- blank_row(columns, row)
        buffer.screen |> emplace(next)
    }
    buffer.screen_start = 0
    buffer.history |> clear()
    buffer.history_start = 0
    buffer.cursor = TerminalCursor()
    buffer.saved_cursor = TerminalCursor()
    buffer.pen = TerminalCell()
    buffer.wrap_pending = false
}

def private screen_index(buffer : TerminalBuffer const; row : int) : int {
    return (buffer.screen_start + row) % length(buffer.screen)
}

def private history_index(buffer : TerminalBuffer const; row : int) : int {
    return (buffer.history_start + row) % length(buffer.history)
}

def private active_buffer(var terminal : Terminal) : TerminalBuffer? {
    unsafe {
        return terminal.alternate_active ? addr(terminal.alternate) : addr(terminal.normal)
    }
}

def private append_string_bytes(var bytes : array<uint8>; text : string) {
    let count = length(text)
    if (count == 0) return
    let offset = length(bytes)
    bytes |> resize_no_init(offset + count)
    peek_data(text) $(data) {
        for (index in range(count)) {
            bytes[offset + index] = data[index]
        }
    }
}

def private push_ascii(var bytes : array<uint8>; value : int) {
    bytes |> push(uint8(value & 255))
}

def private bytes_slice_string(bytes : array<uint8> const; first, past_last : int) : string {
    if (past_last <= first) return ""
    var slice : array<uint8>
    slice |> resize_no_init(past_last - first)
    unsafe {
        memcpy(addr(slice[0]), addr(bytes[first]), past_last - first)
    }
    return string(slice)
}

def private codepoint_utf8(codepoint : uint) : string {
    if (codepoint <= 0x7fu) return to_char(int(codepoint))
    if (codepoint <= 0x7ffu) {
        return (to_char(int(0xc0u | (codepoint >> 6u))) +
            to_char(int(0x80u | (codepoint & 0x3fu))))
    }
    if (codepoint <= 0xffffu) {
        return (to_char(int(0xe0u | (codepoint >> 12u))) +
            to_char(int(0x80u | ((codepoint >> 6u) & 0x3fu))) +
            to_char(int(0x80u | (codepoint & 0x3fu))))
    }
    return (to_char(int(0xf0u | (codepoint >> 18u))) +
        to_char(int(0x80u | ((codepoint >> 12u) & 0x3fu))) +
        to_char(int(0x80u | ((codepoint >> 6u) & 0x3fu))) +
        to_char(int(0x80u | (codepoint & 0x3fu))))
}

def private in_range(value, first, last : uint) : bool {
    return value >= first && value <= last
}

def private is_combining(value : uint) : bool {
    return (in_range(value, 0x0300u, 0x036fu) || in_range(value, 0x0483u, 0x0489u) ||
        in_range(value, 0x0591u, 0x05bdu) || value == 0x05bfu ||
        in_range(value, 0x05c1u, 0x05c2u) || in_range(value, 0x0610u, 0x061au) ||
        in_range(value, 0x064bu, 0x065fu) || in_range(value, 0x1ab0u, 0x1affu) ||
        in_range(value, 0x1dc0u, 0x1dffu) || in_range(value, 0x20d0u, 0x20ffu) ||
        in_range(value, 0xfe00u, 0xfe0fu) || in_range(value, 0xfe20u, 0xfe2fu) ||
        in_range(value, 0xe0100u, 0xe01efu))
}

def private is_wide(value : uint) : bool {
    return (in_range(value, 0x1100u, 0x115fu) || value == 0x2329u || value == 0x232au ||
        in_range(value, 0x2e80u, 0xa4cfu) || in_range(value, 0xac00u, 0xd7a3u) ||
        in_range(value, 0xf900u, 0xfaffu) || in_range(value, 0xfe10u, 0xfe19u) ||
        in_range(value, 0xfe30u, 0xfe6fu) || in_range(value, 0xff00u, 0xff60u) ||
        in_range(value, 0xffe0u, 0xffe6u) || in_range(value, 0x1f300u, 0x1faffu) ||
        in_range(value, 0x20000u, 0x3fffdu))
}

def private indexed_color(index : int) : TerminalColor {
    return TerminalColor(kind = TerminalColorKind.indexed, index = clamp(index, 0, 255))
}

def private rgb_color(red, green, blue : int) : TerminalColor {
    return TerminalColor(kind = TerminalColorKind.rgb,
        red = clamp(red, 0, 255), green = clamp(green, 0, 255), blue = clamp(blue, 0, 255))
}

def private parameter_or(parameters : array<int> const; index, fallback : int) : int {
    return index >= length(parameters) || parameters[index] <= 0 ? fallback : parameters[index]
}

def private parse_bounded_decimal(bytes : array<uint8> const; first, past_last : int) : int {
    var index = first
    while (index < past_last && (bytes[index] == uint8(' ') || bytes[index] == uint8('\t') ||
            bytes[index] == uint8('\n') || bytes[index] == uint8('\r'))) {
        index++
    }
    var negative = false
    if (index < past_last && (bytes[index] == uint8('+') || bytes[index] == uint8('-'))) {
        negative = bytes[index] == uint8('-')
        index++
    }
    var value = 0l
    while (index < past_last) {
        let byte = int(bytes[index])
        if (!is_number(byte)) break
        value = min(2147483648l, value * 10l + int64(byte - '0'))
        index++
    }
    if (negative) return value >= 2147483648l ? -2147483647 - 1 : -int(value)
    return value >= 2147483647l ? 2147483647 : int(value)
}

def private parse_parameters(bytes : array<uint8> const;
                             var private_marker : int&; var result : array<int>) {
    result |> clear()
    private_marker = 0
    var begin = 0
    if (!empty(bytes) && (bytes[0] == uint8('?') || bytes[0] == uint8('>') ||
            bytes[0] == uint8('!'))) {
        if (bytes[0] == uint8('?')) { private_marker = 0x3f }
        elif (bytes[0] == uint8('>')) { private_marker = 0x3e }
        else { private_marker = 0x21 }
        begin = 1
    }
    var item = begin
    while (item <= length(bytes)) {
        var separator = item
        while (separator < length(bytes) && bytes[separator] != uint8(';')) {
            separator++
        }
        result |> push(separator == item ? -1 : parse_bounded_decimal(bytes, item, separator))
        if (separator == length(bytes)) break
        item = separator + 1
    }
    if (empty(result)) result |> push(-1)
}

def private copy_cell(cell : TerminalCell const; row, column : int) : TerminalCell {
    return TerminalCell(row = row, column = column, grapheme = cell.grapheme,
        width = cell.width, attributes = cell.attributes,
        foreground = cell.foreground, background = cell.background,
        hyperlink = cell.hyperlink)
}

def private clear_cell(terminal : Terminal const; var buffer : TerminalBuffer;
                       row, column : int) {
    if (row < 0 || row >= terminal.rows || column < 0 || column >= terminal.columns) return
    let physical_row = screen_index(buffer, row)
    var first = column
    while (first > 0 && buffer.screen[physical_row].cells[first].width == 0) {
        first--
    }
    if (buffer.screen[physical_row].cells[first].width == 2 && first + 1 >= column) {
        buffer.screen[physical_row].cells[first] = blank_cell(row, first)
        if (first + 1 < terminal.columns) {
            buffer.screen[physical_row].cells[first + 1] = blank_cell(row, first + 1)
        }
    } else {
        buffer.screen[physical_row].cells[column] = blank_cell(row, column)
    }
}

def private append_history_row(var buffer : TerminalBuffer; var row : TerminalRow) {
    if (length(buffer.history) < MAX_SCROLLBACK_ROWS) {
        buffer.history |> emplace(row)
    } else {
        buffer.history[buffer.history_start] <- row
        buffer.history_start = (buffer.history_start + 1) % MAX_SCROLLBACK_ROWS
    }
}

def private scroll_up(terminal : Terminal const; var buffer : TerminalBuffer; normal : bool) {
    let first = screen_index(buffer, 0)
    if (normal) append_history_row(buffer, buffer.screen[first])
    buffer.screen_start = (buffer.screen_start + 1) % terminal.rows
    let last = screen_index(buffer, terminal.rows - 1)
    buffer.screen[last] <- blank_row(terminal.columns, terminal.rows - 1)
}

def private carriage_return(var buffer : TerminalBuffer) {
    buffer.cursor.column = 0
    buffer.wrap_pending = false
}

def private line_feed(terminal : Terminal const; var buffer : TerminalBuffer; normal : bool) {
    buffer.wrap_pending = false
    if (buffer.cursor.row == terminal.rows - 1) scroll_up(terminal, buffer, normal)
    else buffer.cursor.row++
}

def private reverse_index(terminal : Terminal const; var buffer : TerminalBuffer) {
    buffer.wrap_pending = false
    if (buffer.cursor.row > 0) {
        buffer.cursor.row--
    } else {
        buffer.screen_start = (buffer.screen_start + terminal.rows - 1) % terminal.rows
        let first = screen_index(buffer, 0)
        buffer.screen[first] <- blank_row(terminal.columns, 0)
    }
}

def private set_cursor(terminal : Terminal const; var buffer : TerminalBuffer;
                       row, column : int) {
    buffer.cursor.row = clamp(row, 0, terminal.rows - 1)
    buffer.cursor.column = clamp(column, 0, terminal.columns - 1)
    buffer.wrap_pending = false
}

def private put_grapheme(var terminal : Terminal; grapheme : string;
                         width_ : int; combining : bool) {
    var buffer = active_buffer(terminal)
    if (combining) {
        var row = buffer.cursor.row
        var column = buffer.cursor.column - 1
        if (buffer.wrap_pending) { column = terminal.columns - 1 }
        if (column < 0 && row > 0) {
            row--
            column = terminal.columns - 1
        }
        if (column >= 0) {
            let physical_row = screen_index(*buffer, row)
            while (column > 0 && buffer.screen[physical_row].cells[column].width == 0) {
                column--
            }
            buffer.screen[physical_row].cells[column].grapheme += grapheme
        }
        return
    }
    if (buffer.wrap_pending && terminal.modes.auto_wrap) {
        carriage_return(*buffer)
        line_feed(terminal, *buffer, !terminal.alternate_active)
    }
    var width = width_
    if (width == 2 && terminal.columns > 1 &&
            buffer.cursor.column == terminal.columns - 1 && terminal.modes.auto_wrap) {
        carriage_return(*buffer)
        line_feed(terminal, *buffer, !terminal.alternate_active)
    }
    if (width == 2 && buffer.cursor.column + 1 >= terminal.columns) { width = 1 }
    let row = buffer.cursor.row
    let column = buffer.cursor.column
    clear_cell(terminal, *buffer, row, column)
    if (width == 2) clear_cell(terminal, *buffer, row, column + 1)
    let physical_row = screen_index(*buffer, row)
    var cell = copy_cell(buffer.pen, row, column)
    cell.grapheme = grapheme
    cell.width = width
    buffer.screen[physical_row].cells[column] <- cell
    if (width == 2) {
        var continuation = copy_cell(buffer.pen, row, column + 1)
        continuation.grapheme = ""
        continuation.width = 0
        buffer.screen[physical_row].cells[column + 1] <- continuation
    }
    let next = column + width
    if (next >= terminal.columns) {
        buffer.cursor.column = terminal.columns - 1
        buffer.wrap_pending = terminal.modes.auto_wrap
    } else {
        buffer.cursor.column = next
        buffer.wrap_pending = false
    }
}

def private process_codepoint(var terminal : Terminal; codepoint : uint) {
    let combining = is_combining(codepoint)
    put_grapheme(terminal, codepoint_utf8(codepoint),
        combining ? 0 : (is_wide(codepoint) ? 2 : 1), combining)
}

def private finish_utf8(var terminal : Terminal; valid_ : bool) {
    let codepoint = terminal.utf8_codepoint
    let valid = (valid_ && codepoint >= terminal.utf8_minimum && codepoint <= 0x10ffffu &&
        !in_range(codepoint, 0xd800u, 0xdfffu))
    terminal.utf8_codepoint = 0u
    terminal.utf8_minimum = 0u
    terminal.utf8_remaining = 0
    process_codepoint(terminal, valid ? codepoint : 0xfffdu)
}

def private record_unknown_text(var terminal : Terminal; value : string) {
    if (length(terminal.unknown_sequences) < MAX_UNKNOWN_SEQUENCES) {
        if (length(value) <= MAX_UNKNOWN_SEQUENCE_BYTES) {
            terminal.unknown_sequences |> push(value)
        } else {
            let suffix = "...[truncated]"
            let suffix_length = length(suffix)
            var bytes : array<uint8>
            bytes |> resize_no_init(MAX_UNKNOWN_SEQUENCE_BYTES)
            peek_data(value) $(data) {
                for (index in range(MAX_UNKNOWN_SEQUENCE_BYTES - suffix_length)) {
                    bytes[index] = data[index]
                }
            }
            peek_data(suffix) $(data) {
                for (index in range(suffix_length)) {
                    bytes[MAX_UNKNOWN_SEQUENCE_BYTES - suffix_length + index] = data[index]
                }
            }
            terminal.unknown_sequences |> push(string(bytes))
        }
    }
    terminal.parse_state = TerminalParseState.ground
}

def private record_unknown_bytes(var terminal : Terminal; bytes : array<uint8> const) {
    record_unknown_text(terminal, bytes_slice_string(bytes, 0, length(bytes)))
}

def private erase_line(terminal : Terminal const; var buffer : TerminalBuffer; mode : int) {
    var first = mode == 0 ? buffer.cursor.column : 0
    var last = mode == 1 ? buffer.cursor.column : terminal.columns - 1
    if (mode == 2) {
        first = 0
        last = terminal.columns - 1
    }
    for (column in range(first, last + 1)) {
        clear_cell(terminal, buffer, buffer.cursor.row, column)
    }
}

def private erase_display(terminal : Terminal const; var buffer : TerminalBuffer; mode : int) {
    if (mode == 2 || mode == 3) {
        for (row in range(terminal.rows)) {
            let physical = screen_index(buffer, row)
            buffer.screen[physical] <- blank_row(terminal.columns, row)
        }
        if (mode == 3) {
            buffer.history |> clear()
            buffer.history_start = 0
        }
    } elif (mode == 0) {
        erase_line(terminal, buffer, 0)
        for (row in range(buffer.cursor.row + 1, terminal.rows)) {
            let physical = screen_index(buffer, row)
            buffer.screen[physical] <- blank_row(terminal.columns, row)
        }
    } elif (mode == 1) {
        erase_line(terminal, buffer, 1)
        for (row in range(0, buffer.cursor.row)) {
            let physical = screen_index(buffer, row)
            buffer.screen[physical] <- blank_row(terminal.columns, row)
        }
    }
}

def private erase_cells(terminal : Terminal const; var buffer : TerminalBuffer; count : int) {
    let last = clamp(int64(buffer.cursor.column) + int64(max(1, count)), 0l, int64(terminal.columns))
    for (column in range(buffer.cursor.column, int(last))) {
        clear_cell(terminal, buffer, buffer.cursor.row, column)
    }
}

def private set_private_mode(var terminal : Terminal; mode : int; enabled : bool) {
    if (mode == 1) { terminal.modes.application_cursor_keys = enabled }
    elif (mode == 7) { terminal.modes.auto_wrap = enabled }
    elif (mode == 25) {
        terminal.normal.cursor.visible = enabled
        terminal.alternate.cursor.visible = enabled
    } elif (mode == 1000) { terminal.modes.mouse_button_reporting = enabled }
    elif (mode == 1003) { terminal.modes.mouse_any_reporting = enabled }
    elif (mode == 1004) { terminal.modes.focus_reporting = enabled }
    elif (mode == 1006) { terminal.modes.sgr_mouse_encoding = enabled }
    elif (mode == 1049) {
        if (enabled && !terminal.alternate_active) {
            terminal.normal.saved_cursor = terminal.normal.cursor
            reset_buffer(terminal.alternate, terminal.columns, terminal.rows)
            terminal.alternate.cursor.visible = terminal.normal.cursor.visible
            terminal.alternate.cursor.style = terminal.normal.cursor.style
            terminal.alternate.cursor.blinking = terminal.normal.cursor.blinking
            terminal.alternate_active = true
        } elif (!enabled && terminal.alternate_active) {
            terminal.alternate_active = false
            terminal.normal.cursor = terminal.normal.saved_cursor
        }
    } elif (mode == 2004) { terminal.modes.bracketed_paste = enabled }
    else {
        let action = enabled ? "set" : "reset"
        record_unknown_text(terminal, "DEC mode {action} {mode}")
    }
}

def private select_graphic_rendition(var buffer : TerminalBuffer;
                                     parameters : array<int> const) {
    var index = 0
    while (index < length(parameters)) {
        let value = max(0, parameters[index])
        if (value == 0) {
            let hyperlink = buffer.pen.hyperlink
            buffer.pen = TerminalCell()
            buffer.pen.hyperlink = hyperlink
        } elif (value == 1) { buffer.pen.attributes |= TERMINAL_ATTR_BOLD }
        elif (value == 2) { buffer.pen.attributes |= TERMINAL_ATTR_FAINT }
        elif (value == 3) { buffer.pen.attributes |= TERMINAL_ATTR_ITALIC }
        elif (value == 4 || value == 21) { buffer.pen.attributes |= TERMINAL_ATTR_UNDERLINE }
        elif (value == 5 || value == 6) { buffer.pen.attributes |= TERMINAL_ATTR_BLINK }
        elif (value == 7) { buffer.pen.attributes |= TERMINAL_ATTR_INVERSE }
        elif (value == 8) { buffer.pen.attributes |= TERMINAL_ATTR_HIDDEN }
        elif (value == 9) { buffer.pen.attributes |= TERMINAL_ATTR_STRIKE }
        elif (value == 22) { buffer.pen.attributes &= ~(TERMINAL_ATTR_BOLD | TERMINAL_ATTR_FAINT) }
        elif (value == 23) { buffer.pen.attributes &= ~TERMINAL_ATTR_ITALIC }
        elif (value == 24) { buffer.pen.attributes &= ~TERMINAL_ATTR_UNDERLINE }
        elif (value == 25) { buffer.pen.attributes &= ~TERMINAL_ATTR_BLINK }
        elif (value == 27) { buffer.pen.attributes &= ~TERMINAL_ATTR_INVERSE }
        elif (value == 28) { buffer.pen.attributes &= ~TERMINAL_ATTR_HIDDEN }
        elif (value == 29) { buffer.pen.attributes &= ~TERMINAL_ATTR_STRIKE }
        elif (value == 39) { buffer.pen.foreground = TerminalColor() }
        elif (value == 49) { buffer.pen.background = TerminalColor() }
        elif (value >= 30 && value <= 37) { buffer.pen.foreground = indexed_color(value - 30) }
        elif (value >= 40 && value <= 47) { buffer.pen.background = indexed_color(value - 40) }
        elif (value >= 90 && value <= 97) { buffer.pen.foreground = indexed_color(value - 90 + 8) }
        elif (value >= 100 && value <= 107) { buffer.pen.background = indexed_color(value - 100 + 8) }
        elif ((value == 38 || value == 48) && index + 2 < length(parameters) &&
                parameters[index + 1] == 5) {
            if (value == 38) { buffer.pen.foreground = indexed_color(parameters[index + 2]) }
            else { buffer.pen.background = indexed_color(parameters[index + 2]) }
            index += 2
        } elif ((value == 38 || value == 48) && index + 4 < length(parameters) &&
                parameters[index + 1] == 2) {
            let color = rgb_color(parameters[index + 2], parameters[index + 3], parameters[index + 4])
            if (value == 38) { buffer.pen.foreground = color }
            else { buffer.pen.background = color }
            index += 4
        }
        index++
    }
}

def private append_reply_text(var terminal : Terminal; text : string) {
    var bytes : array<uint8>
    append_string_bytes(bytes, text)
    terminal.replies |> emplace(bytes)
}

def private process_csi(var terminal : Terminal; final_byte : int) {
    terminal.parse_state = TerminalParseState.ground
    var private_marker = 0
    var parameters : array<int>
    parse_parameters(terminal.csi_parameters, private_marker, parameters)
    var buffer = active_buffer(terminal)
    let first = parameter_or(parameters, 0, 1)
    if (private_marker == 0x3f && (final_byte == 'h' || final_byte == 'l')) {
        for (mode in parameters) {
            if (mode >= 0) set_private_mode(terminal, mode, final_byte == 'h')
        }
        return
    }
    if (final_byte == 'A') { buffer.cursor.row = int(clamp(int64(buffer.cursor.row) - int64(first), 0l, int64(terminal.rows - 1))) }
    elif (final_byte == 'B' || final_byte == 'e') { buffer.cursor.row = int(clamp(int64(buffer.cursor.row) + int64(first), 0l, int64(terminal.rows - 1))) }
    elif (final_byte == 'C' || final_byte == 'a') { buffer.cursor.column = int(clamp(int64(buffer.cursor.column) + int64(first), 0l, int64(terminal.columns - 1))) }
    elif (final_byte == 'D') { buffer.cursor.column = int(clamp(int64(buffer.cursor.column) - int64(first), 0l, int64(terminal.columns - 1))) }
    elif (final_byte == 'E') { set_cursor(terminal, *buffer, int(clamp(int64(buffer.cursor.row) + int64(first), 0l, int64(terminal.rows - 1))), 0) }
    elif (final_byte == 'F') { set_cursor(terminal, *buffer, int(clamp(int64(buffer.cursor.row) - int64(first), 0l, int64(terminal.rows - 1))), 0) }
    elif (final_byte == 'G') { set_cursor(terminal, *buffer, buffer.cursor.row, first - 1) }
    elif (final_byte == 'H' || final_byte == 'f') { set_cursor(terminal, *buffer, parameter_or(parameters, 0, 1) - 1, parameter_or(parameters, 1, 1) - 1) }
    elif (final_byte == 'J') { erase_display(terminal, *buffer, max(0, parameters[0])) }
    elif (final_byte == 'K') { erase_line(terminal, *buffer, max(0, parameters[0])) }
    elif (final_byte == 'X') { erase_cells(terminal, *buffer, first) }
    elif (final_byte == 'd') { set_cursor(terminal, *buffer, first - 1, buffer.cursor.column) }
    elif (final_byte == 'm') { select_graphic_rendition(*buffer, parameters) }
    elif (final_byte == 'n') {
        if (private_marker == 0 && parameters[0] == 5) { append_reply_text(terminal, "{to_char(27)}[0n") }
        elif (private_marker == 0 && parameters[0] == 6) { append_reply_text(terminal, "{to_char(27)}[{buffer.cursor.row + 1};{buffer.cursor.column + 1}R") }
        else { record_unknown_bytes(terminal, terminal.sequence) }
    } elif (final_byte == 'c') {
        if (private_marker == 0) { append_reply_text(terminal, "{to_char(27)}[?1;2c") }
        else { record_unknown_bytes(terminal, terminal.sequence) }
    } elif (final_byte == 's') { buffer.saved_cursor = buffer.cursor }
    elif (final_byte == 'u') { buffer.cursor = buffer.saved_cursor }
    elif (final_byte == 'q') {
        if (private_marker == 0) {
            buffer.cursor.style = max(0, parameters[0])
            let style = buffer.cursor.style
            buffer.cursor.blinking = style == 0 || style == 1 || style == 3 || style == 5
        } else { record_unknown_bytes(terminal, terminal.sequence) }
    } else { record_unknown_bytes(terminal, terminal.sequence) }
    buffer.wrap_pending = false
}

def private find_byte(bytes : array<uint8> const; value : int; first : int = 0) : int {
    for (index in range(first, length(bytes))) {
        if (bytes[index] == uint8(value)) return index
    }
    return -1
}

def private process_osc(var terminal : Terminal) {
    terminal.parse_state = TerminalParseState.ground
    let separator = find_byte(terminal.osc_data, ';')
    let command = bytes_slice_string(terminal.osc_data, 0,
        separator < 0 ? length(terminal.osc_data) : separator)
    let value = separator < 0 ? "" : bytes_slice_string(terminal.osc_data, separator + 1, length(terminal.osc_data))
    if (command == "0" || command == "2") {
        terminal.title = value
        terminal.events |> push("title:{value}")
    } elif (command == "7") {
        terminal.current_directory = value
        terminal.events |> push("cwd:{value}")
    } elif (command == "8") {
        let uri_separator = find_byte(terminal.osc_data, ';', separator + 1)
        var buffer = active_buffer(terminal)
        buffer.pen.hyperlink = (uri_separator < 0 ? "" :
            bytes_slice_string(terminal.osc_data, uri_separator + 1, length(terminal.osc_data)))
    } else {
        record_unknown_text(terminal, "OSC {string(terminal.osc_data)}")
    }
    terminal.osc_data |> clear()
}

def private reset_terminal(var terminal : Terminal) {
    reset_buffer(terminal.normal, terminal.columns, terminal.rows)
    reset_buffer(terminal.alternate, terminal.columns, terminal.rows)
    terminal.alternate_active = false
    terminal.modes = TerminalModes()
    terminal.title = ""
    terminal.current_directory = ""
    terminal.unknown_sequences |> clear()
    terminal.replies |> clear()
    terminal.events |> clear()
}

def private process_escape(var terminal : Terminal; byte : int) {
    terminal.sequence |> push(uint8(byte))
    terminal.parse_state = TerminalParseState.ground
    var buffer = active_buffer(terminal)
    if (byte == '[') {
        terminal.parse_state = TerminalParseState.csi
        terminal.csi_parameters |> clear()
    } elif (byte == ']') {
        terminal.parse_state = TerminalParseState.osc
        terminal.osc_data |> clear()
    } elif (byte == '7') { buffer.saved_cursor = buffer.cursor }
    elif (byte == '8') { buffer.cursor = buffer.saved_cursor }
    elif (byte == 'D') line_feed(terminal, *buffer, !terminal.alternate_active)
    elif (byte == 'E') {
        carriage_return(*buffer)
        line_feed(terminal, *buffer, !terminal.alternate_active)
    } elif (byte == 'M') reverse_index(terminal, *buffer)
    elif (byte == 'c') reset_terminal(terminal)
    elif (byte != '\\') record_unknown_bytes(terminal, terminal.sequence)
}

def private process_control(var terminal : Terminal; byte : int) {
    var buffer = active_buffer(terminal)
    if (byte == 0x07) terminal.events |> push("bell")
    elif (byte == 0x08) {
        buffer.cursor.column = max(0, buffer.cursor.column - 1)
        buffer.wrap_pending = false
    } elif (byte == 0x09) {
        buffer.cursor.column = min(terminal.columns - 1, ((buffer.cursor.column / 8) + 1) * 8)
        buffer.wrap_pending = false
    } elif (byte == 0x0a || byte == 0x0b || byte == 0x0c) {
        line_feed(terminal, *buffer, !terminal.alternate_active)
    } elif (byte == 0x0d) carriage_return(*buffer)
}

def private process_ground_byte(var terminal : Terminal; byte : int) {
    if (byte == 0x1b) {
        terminal.parse_state = TerminalParseState.escape
        terminal.sequence |> clear()
        terminal.sequence |> push(uint8(0x1b))
    } elif (byte < 0x20 || byte == 0x7f) process_control(terminal, byte)
    elif (byte < 0x80) put_grapheme(terminal, terminal.ascii_graphemes[byte], 1, false)
    elif ((byte & 0xe0) == 0xc0) {
        terminal.utf8_codepoint = uint(byte & 0x1f)
        terminal.utf8_minimum = 0x80u
        terminal.utf8_remaining = 1
    } elif ((byte & 0xf0) == 0xe0) {
        terminal.utf8_codepoint = uint(byte & 0x0f)
        terminal.utf8_minimum = 0x800u
        terminal.utf8_remaining = 2
    } elif ((byte & 0xf8) == 0xf0) {
        terminal.utf8_codepoint = uint(byte & 0x07)
        terminal.utf8_minimum = 0x10000u
        terminal.utf8_remaining = 3
    } else process_codepoint(terminal, 0xfffdu)
}

def private process_byte(var terminal : Terminal; byte : int) {
    if (terminal.utf8_remaining > 0) {
        if ((byte & 0xc0) == 0x80) {
            terminal.utf8_codepoint = (terminal.utf8_codepoint << 6u) | uint(byte & 0x3f)
            terminal.utf8_remaining--
            if (terminal.utf8_remaining == 0) finish_utf8(terminal, true)
            return
        }
        finish_utf8(terminal, false)
        process_byte(terminal, byte)
        return
    }
    if (terminal.parse_state == TerminalParseState.ground) process_ground_byte(terminal, byte)
    elif (terminal.parse_state == TerminalParseState.escape) process_escape(terminal, byte)
    elif (terminal.parse_state == TerminalParseState.csi) {
        if (byte == 0x1b) {
            terminal.parse_state = TerminalParseState.escape
            terminal.sequence |> clear()
            terminal.sequence |> push(uint8(0x1b))
            terminal.csi_parameters |> clear()
        } elif (length(terminal.sequence) == MAX_CSI_SEQUENCE_BYTES) {
            terminal.sequence |> clear()
            terminal.csi_parameters |> clear()
            terminal.parse_state = TerminalParseState.csi_ignore
        } else {
            terminal.sequence |> push(uint8(byte))
            if (byte >= 0x40 && byte <= 0x7e) { process_csi(terminal, byte) }
            elif (byte >= 0x20 && byte <= 0x3f) { terminal.csi_parameters |> push(uint8(byte)) }
            else { record_unknown_bytes(terminal, terminal.sequence) }
        }
    } elif (terminal.parse_state == TerminalParseState.csi_ignore) {
        if (byte >= 0x40 && byte <= 0x7e) { terminal.parse_state = TerminalParseState.ground }
        elif (byte == 0x1b) {
            terminal.parse_state = TerminalParseState.escape
            terminal.sequence |> clear()
            terminal.sequence |> push(uint8(0x1b))
        }
    } elif (terminal.parse_state == TerminalParseState.osc) {
        if (byte == 0x07) { process_osc(terminal) }
        elif (byte == 0x1b) { terminal.parse_state = TerminalParseState.osc_escape }
        elif (length(terminal.osc_data) == MAX_OSC_PAYLOAD_BYTES) {
            terminal.osc_data |> clear()
            terminal.parse_state = TerminalParseState.osc_ignore
        } else { terminal.osc_data |> push(uint8(byte)) }
    } elif (terminal.parse_state == TerminalParseState.osc_escape) {
        if (byte == '\\') { process_osc(terminal) }
        elif (length(terminal.osc_data) <= MAX_OSC_PAYLOAD_BYTES - 2) {
            terminal.osc_data |> push([uint8(0x1b), uint8(byte)])
            terminal.parse_state = TerminalParseState.osc
        } else {
            terminal.osc_data |> clear()
            terminal.parse_state = TerminalParseState.osc_ignore
        }
    } elif (terminal.parse_state == TerminalParseState.osc_ignore) {
        if (byte == 0x07) { terminal.parse_state = TerminalParseState.ground }
        elif (byte == 0x1b) { terminal.parse_state = TerminalParseState.osc_ignore_escape }
    } elif (terminal.parse_state == TerminalParseState.osc_ignore_escape) {
        if (byte == '\\') { terminal.parse_state = TerminalParseState.ground }
        elif (byte != 0x1b) { terminal.parse_state = TerminalParseState.osc_ignore }
    }
}

def public terminal_create(columns : int = 80; rows : int = 25) : Terminal {
    var inscope terminal = Terminal(columns = max(1, columns), rows = max(1, rows))
    reset_buffer(terminal.normal, terminal.columns, terminal.rows)
    reset_buffer(terminal.alternate, terminal.columns, terminal.rows)
    terminal.sequence |> reserve(MAX_CSI_SEQUENCE_BYTES)
    terminal.csi_parameters |> reserve(64)
    terminal.osc_data |> reserve(1024)
    terminal.read_buffer |> reserve(64 * 1024)
    terminal.ascii_graphemes |> reserve(128)
    for (value in range(128)) {
        terminal.ascii_graphemes |> push(value >= 32 && value < 127 ? to_char(value) : "")
    }
    return <- terminal
}

def public terminal_dispose(var terminal : Terminal&) {
    delete terminal.normal.screen
    delete terminal.normal.history
    delete terminal.alternate.screen
    delete terminal.alternate.history
    delete terminal.unknown_sequences
    delete terminal.replies
    delete terminal.events
    delete terminal.sequence
    delete terminal.csi_parameters
    delete terminal.osc_data
    delete terminal.read_buffer
    delete terminal.ascii_graphemes
    terminal := default<Terminal>
}

def public terminal_launch(command_line : string;
                           working_directory : string = "";
                           columns : int = 80; rows : int = 25) : Terminal {
    var inscope terminal <- terminal_create(columns, rows)
    var inscope pty <- _pty_launch(command_line, length(command_line),
        working_directory, length(working_directory), columns, rows)
    move(terminal.transport) <| pty
    return <- terminal
}

def public terminal_feed_bytes(var terminal : Terminal; bytes : array<uint8> const) {
    if (empty(bytes)) return
    terminal.revision++
    for (byte in bytes) {
        process_byte(terminal, int(byte))
    }
}

def public terminal_feed(var terminal : Terminal; bytes : string) {
    if (empty(bytes)) return
    terminal.revision++
    peek_data(bytes) $(data) {
        for (index in range(length(bytes))) {
            process_byte(terminal, int(data[index]))
        }
    }
}

def public terminal_poll(var terminal : Terminal;
                         maximum_bytes : int = 64 * 1024) : TerminalPollStatus {
    if (terminal.transport == null) return TerminalPollStatus.error
    let status = unsafe(reinterpret<TerminalPollStatus>(
        _pty_read(terminal.transport, terminal.read_buffer, maximum_bytes)))
    if (status != TerminalPollStatus.data) return status
    terminal_feed_bytes(terminal, terminal.read_buffer)
    for (reply in terminal.replies) {
        if (!_pty_write(terminal.transport, reply)) return TerminalPollStatus.error
    }
    terminal.replies |> clear()
    return status
}

def public terminal_write_bytes(terminal : Terminal const; bytes : array<uint8> const) : bool {
    return terminal.transport != null && _pty_write(terminal.transport, bytes)
}

def public terminal_process_id(terminal : Terminal const) : int {
    return terminal.transport != null ? _pty_process_id(terminal.transport) : 0
}

def public terminal_process_exited(terminal : Terminal const) : bool {
    return terminal.transport != null && _pty_process_exited(terminal.transport)
}

def public terminal_exit_code(terminal : Terminal const) : int64 {
    return terminal.transport != null ? _pty_exit_code(terminal.transport) : -1l
}

def public terminal_terminate(terminal : Terminal const; exit_code : int = 1) : bool {
    return terminal.transport != null && _pty_terminate(terminal.transport, exit_code)
}

def public terminal_transport_error(terminal : Terminal const) : string {
    return terminal.transport != null ? _pty_error(terminal.transport) : ""
}

def public terminal_revision(terminal : Terminal const) : int64 {
    return terminal.revision
}

def private resize_buffer(var buffer : TerminalBuffer; old_columns, old_rows,
                          columns, rows : int) {
    var replacement : array<TerminalRow>
    replacement |> reserve(rows)
    for (row in range(rows)) {
        var next = blank_row(columns, row)
        if (row < old_rows) {
            let old_index = screen_index(buffer, row)
            for (column in range(min(old_columns, columns))) {
                next.cells[column] = copy_cell(buffer.screen[old_index].cells[column], row, column)
            }
            if (!empty(next.cells) && next.cells[length(next.cells) - 1].width == 2) {
                next.cells[length(next.cells) - 1] = blank_cell(row, columns - 1)
            }
        }
        replacement |> emplace(next)
    }
    buffer.screen <- replacement
    buffer.screen_start = 0
    for (logical in range(length(buffer.history))) {
        let index = history_index(buffer, logical)
        let old_count = length(buffer.history[index].cells)
        unsafe {
            buffer.history[index].cells |> resize(columns)
        }
        for (column in range(old_count, columns)) {
            buffer.history[index].cells[column] = blank_cell(logical, column)
        }
        if (!empty(buffer.history[index].cells) && buffer.history[index].cells[columns - 1].width == 2) {
            buffer.history[index].cells[columns - 1] = blank_cell(logical, columns - 1)
        }
    }
    buffer.cursor.row = min(buffer.cursor.row, rows - 1)
    buffer.cursor.column = min(buffer.cursor.column, columns - 1)
    buffer.saved_cursor.row = min(buffer.saved_cursor.row, rows - 1)
    buffer.saved_cursor.column = min(buffer.saved_cursor.column, columns - 1)
    buffer.wrap_pending = false
}

def public terminal_resize(var terminal : Terminal; columns_, rows_ : int) {
    let columns = max(1, columns_)
    let rows = max(1, rows_)
    if ((columns == terminal.columns && rows == terminal.rows) ||
            (terminal.transport != null && !_pty_resize(terminal.transport, columns, rows))) return
    let old_columns = terminal.columns
    let old_rows = terminal.rows
    resize_buffer(terminal.normal, old_columns, old_rows, columns, rows)
    resize_buffer(terminal.alternate, old_columns, old_rows, columns, rows)
    terminal.columns = columns
    terminal.rows = rows
    terminal.revision++
}

def private collect_buffer(buffer : TerminalBuffer const; columns, rows : int;
                           var snapshot : TerminalBufferSnapshot) {
    snapshot.cells |> clear()
    snapshot.cells |> reserve(columns * rows)
    for (row in range(rows)) {
        let physical = screen_index(buffer, row)
        for (column in range(columns)) {
            snapshot.cells |> push(copy_cell(buffer.screen[physical].cells[column], row, column))
        }
    }
    snapshot.scrollback |> clear()
    snapshot.scrollback |> reserve(columns * length(buffer.history))
    for (row in range(length(buffer.history))) {
        let physical = history_index(buffer, row)
        for (column in range(columns)) {
            snapshot.scrollback |> push(copy_cell(buffer.history[physical].cells[column], row, column))
        }
    }
    snapshot.scrollback_rows = length(buffer.history)
    snapshot.cursor = buffer.cursor
}

def public terminal_snapshot(terminal : Terminal const; var snapshot : TerminalSnapshot) {
    snapshot.columns = terminal.columns
    snapshot.rows = terminal.rows
    snapshot.alternate_active = terminal.alternate_active
    collect_buffer(terminal.normal, terminal.columns, terminal.rows, snapshot.normal)
    collect_buffer(terminal.alternate, terminal.columns, terminal.rows, snapshot.alternate)
    snapshot.modes = terminal.modes
    snapshot.title = terminal.title
    snapshot.current_directory = terminal.current_directory
    snapshot.unknown_sequences := terminal.unknown_sequences
}

def public terminal_drain_replies_bytes(var terminal : Terminal;
                                        var replies : array<array<uint8>>) {
    replies <- terminal.replies
}

//! Compatibility boundary for callers which still consume terminal replies as
//! strings. Parser and transport storage remain raw bytes end to end.
def public terminal_drain_replies(var terminal : Terminal; var replies : array<string>) {
    var raw_replies : array<array<uint8>>
    terminal_drain_replies_bytes(terminal, raw_replies)
    replies |> clear()
    replies |> reserve(length(raw_replies))
    for (reply in raw_replies) {
        replies |> push(string(reply))
    }
}

def public terminal_take_events(var terminal : Terminal; var events : array<string>) {
    events <- terminal.events
}

def private string_to_bytes(text : string; var bytes : array<uint8>) {
    bytes |> clear()
    append_string_bytes(bytes, text)
}

def public terminal_write(terminal : Terminal const; text : string) : bool {
    var bytes : array<uint8>
    string_to_bytes(text, bytes)
    return terminal_write_bytes(terminal, bytes)
}

def private key_modifier(event : TerminalKeyEvent const) : int {
    return 1 + (event.shift ? 1 : 0) + (event.alt ? 2 : 0) + (event.control ? 4 : 0)
}

def private append_alt_prefix(var output : array<uint8>; alt : bool) {
    if (alt) output |> push(uint8(0x1b))
}

def private append_csi_key(var output : array<uint8>; final_byte, modifier : int) {
    append_string_bytes(output, "{to_char(27)}[1;{modifier}{to_char(final_byte)}")
}

def private append_tilde_key(var output : array<uint8>; code, modifier : int) {
    append_string_bytes(output, "{to_char(27)}[{code}")
    if (modifier != 1) append_string_bytes(output, ";{modifier}")
    output |> push(uint8('~'))
}

def private append_control_text(var output : array<uint8>; text : string) {
    if (length(text) != 1) {
        append_string_bytes(output, text)
        return
    }
    let value = first_character(text)
    if (value >= 'a' && value <= 'z') output |> push(uint8(value - 'a' + 1))
    elif (value >= 'A' && value <= 'Z') output |> push(uint8(value - 'A' + 1))
    elif (value == ' ' || value == '@') output |> push(uint8(0))
    elif (value == '[') output |> push(uint8(0x1b))
    elif (value == '\\') output |> push(uint8(0x1c))
    elif (value == ']') output |> push(uint8(0x1d))
    elif (value == '^') output |> push(uint8(0x1e))
    elif (value == '_') output |> push(uint8(0x1f))
    elif (value == '?') output |> push(uint8(0x7f))
    else append_string_bytes(output, text)
}

def public terminal_encode_key_bytes(terminal : Terminal const;
                                     event : TerminalKeyEvent const;
                                     var output : array<uint8>) {
    output |> clear()
    let modifier = key_modifier(event)
    if (event.key == TerminalKey.text) {
        append_alt_prefix(output, event.alt)
        if (event.control) append_control_text(output, event.text)
        else append_string_bytes(output, event.text)
    } elif (event.key == TerminalKey.enter) {
        append_alt_prefix(output, event.alt)
        output |> push(uint8('\r'))
    } elif (event.key == TerminalKey.tab) {
        append_alt_prefix(output, event.alt)
        if (event.shift) append_string_bytes(output, "{to_char(27)}[Z")
        else output |> push(uint8('\t'))
    } elif (event.key == TerminalKey.backspace) {
        append_alt_prefix(output, event.alt)
        output |> push(uint8(0x7f))
    } elif (event.key == TerminalKey.escape) {
        append_alt_prefix(output, event.alt)
        output |> push(uint8(0x1b))
    } elif (int(event.key) >= int(TerminalKey.up) && int(event.key) <= int(TerminalKey.end)) {
        let finals = ['A', 'B', 'C', 'D', 'H', 'F']
        let final_byte = finals[int(event.key) - int(TerminalKey.up)]
        if (modifier != 1) append_csi_key(output, final_byte, modifier)
        else {
            append_string_bytes(output,
                terminal.modes.application_cursor_keys ? "{to_char(27)}O" : "{to_char(27)}[")
            output |> push(uint8(final_byte))
        }
    } elif (event.key == TerminalKey.insert) append_tilde_key(output, 2, modifier)
    elif (event.key == TerminalKey.delete_key) append_tilde_key(output, 3, modifier)
    elif (event.key == TerminalKey.page_up) append_tilde_key(output, 5, modifier)
    elif (event.key == TerminalKey.page_down) append_tilde_key(output, 6, modifier)
    elif (int(event.key) >= int(TerminalKey.f1) && int(event.key) <= int(TerminalKey.f4)) {
        let final_byte = 'P' + int(event.key) - int(TerminalKey.f1)
        if (modifier != 1) append_csi_key(output, final_byte, modifier)
        else {
            append_string_bytes(output, "{to_char(27)}O")
            output |> push(uint8(final_byte))
        }
    } elif (event.key == TerminalKey.f5) append_tilde_key(output, 15, modifier)
    elif (event.key == TerminalKey.f6) append_tilde_key(output, 17, modifier)
    elif (event.key == TerminalKey.f7) append_tilde_key(output, 18, modifier)
    elif (event.key == TerminalKey.f8) append_tilde_key(output, 19, modifier)
    elif (event.key == TerminalKey.f9) append_tilde_key(output, 20, modifier)
    elif (event.key == TerminalKey.f10) append_tilde_key(output, 21, modifier)
    elif (event.key == TerminalKey.f11) append_tilde_key(output, 23, modifier)
    elif (event.key == TerminalKey.f12) append_tilde_key(output, 24, modifier)
}

def public terminal_encode_key(terminal : Terminal const;
                               event : TerminalKeyEvent const) : string {
    var bytes : array<uint8>
    terminal_encode_key_bytes(terminal, event, bytes)
    return string(bytes)
}

def public terminal_encode_paste_bytes(terminal : Terminal const; text : string;
                                       var output : array<uint8>) {
    output |> clear()
    if (terminal.modes.bracketed_paste) append_string_bytes(output, "{to_char(27)}[200~")
    append_string_bytes(output, text)
    if (terminal.modes.bracketed_paste) append_string_bytes(output, "{to_char(27)}[201~")
}

def public terminal_encode_paste(terminal : Terminal const; text : string) : string {
    var bytes : array<uint8>
    terminal_encode_paste_bytes(terminal, text, bytes)
    return string(bytes)
}

def public terminal_send_key(terminal : Terminal const; event : TerminalKeyEvent const) : bool {
    var bytes : array<uint8>
    terminal_encode_key_bytes(terminal, event, bytes)
    return terminal_write_bytes(terminal, bytes)
}

def public terminal_send_paste(terminal : Terminal const; text : string) : bool {
    var bytes : array<uint8>
    terminal_encode_paste_bytes(terminal, text, bytes)
    return terminal_write_bytes(terminal, bytes)
}

def private same_color(left, right : TerminalColor const) : bool {
    return (left.kind == right.kind && left.index == right.index &&
        left.red == right.red && left.green == right.green && left.blue == right.blue)
}

def private batchable_ascii(cell : TerminalCell const) : bool {
    if (cell.width != 1 || cell.attributes != 0 || cell.hyperlink != "" ||
            length(cell.grapheme) != 1) return false
    let value = first_character(cell.grapheme)
    return value >= 32 && value <= 126
}

def private needs_paint_cell(cell : TerminalCell const) : bool {
    if (cell.width == 0) return false
    let chrome = (cell.background.kind != TerminalColorKind.default_color ||
        (cell.attributes & (TERMINAL_ATTR_INVERSE | TERMINAL_ATTR_UNDERLINE |
            TERMINAL_ATTR_STRIKE)) != 0)
    let fallback = (!batchable_ascii(cell) && cell.grapheme != " " &&
        cell.grapheme != "" && (cell.attributes & TERMINAL_ATTR_HIDDEN) == 0)
    return chrome || fallback
}

struct private TerminalTextRunGroup {
    color : TerminalColor = TerminalColor()
    default_is_background : bool
    first_column : int
    last_column : int
    glyph_cells : int
    text : array<uint8>
}

def private emit_group(group : TerminalTextRunGroup const; global_row : int;
                       var runs : array<TerminalTextRun>) {
    runs |> push(TerminalTextRun(global_row = global_row, column = group.first_column,
        text = bytes_slice_string(group.text, group.first_column, group.last_column + 1),
        color = group.color, default_is_background = group.default_is_background,
        glyph_cells = group.glyph_cells))
}

def private append_text_runs_for_row(row : TerminalRow const; global_row : int;
                                     var runs : array<TerminalTextRun>) {
    var groups : array<TerminalTextRunGroup>
    groups |> reserve(17)
    var too_many_colors = false
    for (column in range(length(row.cells))) {
        let cell = row.cells[column]
        if (!batchable_ascii(cell) || cell.grapheme == " ") continue
        let use_background = (cell.attributes & TERMINAL_ATTR_INVERSE) != 0
        let color = use_background ? cell.background : cell.foreground
        var found = length(groups)
        for (index in range(length(groups))) {
            if (groups[index].default_is_background == use_background &&
                    same_color(groups[index].color, color)) {
                found = index
                break
            }
        }
        if (found == length(groups)) {
            if (length(groups) == 16) {
                too_many_colors = true
                break
            }
            var group = TerminalTextRunGroup(color = color,
                default_is_background = use_background,
                first_column = column, last_column = column)
            group.text |> resize(length(row.cells))
            for (item in range(length(group.text))) {
                group.text[item] = uint8(' ')
            }
            groups |> emplace(group)
            found = length(groups) - 1
        }
        groups[found].text[column] = uint8(first_character(cell.grapheme))
        groups[found].last_column = column
        groups[found].glyph_cells++
    }
    if (!too_many_colors) {
        for (group in groups) {
            emit_group(group, global_row, runs)
        }
        return
    }
    var column = 0
    while (column < length(row.cells)) {
        let first = row.cells[column]
        if (!batchable_ascii(first) || first.grapheme == " ") {
            column++
            continue
        }
        let use_background = (first.attributes & TERMINAL_ATTR_INVERSE) != 0
        let color = use_background ? first.background : first.foreground
        let run_start = column
        var ink_end = column
        var glyph_cells = 0
        var text : array<uint8>
        while (column < length(row.cells)) {
            let cell = row.cells[column]
            let cell_background = (cell.attributes & TERMINAL_ATTR_INVERSE) != 0
            let cell_color = cell_background ? cell.background : cell.foreground
            if (!batchable_ascii(cell) || cell_background != use_background ||
                    !same_color(cell_color, color)) break
            text |> push(uint8(first_character(cell.grapheme)))
            if (cell.grapheme != " ") {
                ink_end = length(text)
                glyph_cells++
            }
            column++
        }
        runs |> push(TerminalTextRun(global_row = global_row, column = run_start,
            text = bytes_slice_string(text, 0, ink_end), color = color,
            default_is_background = use_background, glyph_cells = glyph_cells))
    }
}

def private append_paint_cells_for_row(row : TerminalRow const; logical_row : int;
                                       var cells : array<TerminalCell>) {
    for (column in range(length(row.cells))) {
        if (needs_paint_cell(row.cells[column])) {
            cells |> push(copy_cell(row.cells[column], logical_row, column))
        }
    }
}

def private screen_text(buffer : TerminalBuffer const; rows : int) : string {
    var bytes : array<uint8>
    var last_content_end = 0
    for (row in range(rows)) {
        if (row != 0) bytes |> push(uint8('\n'))
        let row_begin = length(bytes)
        let physical = screen_index(buffer, row)
        for (cell in buffer.screen[physical].cells) {
            if (cell.width != 0) append_string_bytes(bytes, cell.grapheme)
        }
        while (length(bytes) > row_begin && bytes[length(bytes) - 1] == uint8(' ')) {
            bytes |> pop()
        }
        if (length(bytes) != row_begin) { last_content_end = length(bytes) }
    }
    bytes |> resize(last_content_end)
    return string(bytes)
}

def public terminal_viewport_snapshot(terminal : Terminal const; scroll_offset : int;
                                      var snapshot : TerminalViewportSnapshot) {
    snapshot.revision = terminal.revision
    snapshot.columns = terminal.columns
    snapshot.rows = terminal.rows
    snapshot.alternate_active = terminal.alternate_active
    let buffer & = terminal.alternate_active ? terminal.alternate : terminal.normal
    let history_rows = length(buffer.history)
    let offset = clamp(scroll_offset, 0, history_rows)
    let first_history = history_rows - offset
    let past_history = min(history_rows, first_history + terminal.rows)

    let cells_started = ref_time_ticks()
    snapshot.buffer.cells |> clear()
    snapshot.buffer.scrollback |> clear()
    for (row in range(first_history, past_history)) {
        let physical = history_index(buffer, row)
        append_paint_cells_for_row(buffer.history[physical], row, snapshot.buffer.scrollback)
    }
    for (row in range(terminal.rows)) {
        let physical = screen_index(buffer, row)
        append_paint_cells_for_row(buffer.screen[physical], row, snapshot.buffer.cells)
    }
    snapshot.cell_projection_us = get_time_usec(cells_started)

    let runs_started = ref_time_ticks()
    snapshot.text_runs |> clear()
    for (row in range(first_history, past_history)) {
        let physical = history_index(buffer, row)
        append_text_runs_for_row(buffer.history[physical], row, snapshot.text_runs)
    }
    for (row in range(terminal.rows)) {
        let physical = screen_index(buffer, row)
        append_text_runs_for_row(buffer.screen[physical], history_rows + row, snapshot.text_runs)
    }
    snapshot.screen_text = screen_text(buffer, terminal.rows)
    snapshot.text_run_projection_us = get_time_usec(runs_started)
    snapshot.buffer.scrollback_rows = history_rows
    snapshot.buffer.cursor = buffer.cursor
}
