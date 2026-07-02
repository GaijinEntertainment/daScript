// Shared pieces of the LSP subtools: LSP position/range shapes, the
// byte-offset <-> UTF-16 conversions, and argv plumbing. The das side owns
// both conversion directions because it has the file text (utils/lsp/ROADMAP.md).
//
// Coordinate conventions (verified against ds2_lexer.lpp: das_yycolumn = 0 at
// line start, last_column = column + yyleng):
//   daslang LineInfo — 1-based lines, 0-based BYTE columns, end-exclusive
//   LSP              — 0-based lines, 0-based UTF-16 characters, end-exclusive
options gen2
options indenting = 4

module lsp_common

require daslib/fio
require daslib/strings_boost
require strings
require math

struct public LspPosition {
    line : int
    character : int
}

struct public LspRange {
    start : LspPosition
    end : LspPosition
}

// UTF-16 code units in the first `byte_count` bytes of the line (LSP `character`)
def public utf16_units_in(line_text : string; byte_count : int) : int {
    let target = byte_count
    if (target <= 0) {
        return 0
    }
    var units = 0
    peek_data(line_text) $(arr) {
        var off = 0
        for (b in arr) {
            break if (off >= target)
            let c = int(b)
            if ((c & int(0xC0)) != int(0x80)) {  // not a continuation byte: one new code point
                units += ((c & int(0xF8)) == int(0xF0)) ? 2 : 1  // 4-byte UTF-8 = surrogate pair
            }
            off++
        }
    }
    return units
}

// LSP `character` (UTF-16 code units) -> 0-based byte offset; a character
// landing inside a surrogate pair snaps to the code point start
def public utf16_to_byte_offset(line_text : string; character : int) : int {
    if (character <= 0) {
        return 0
    }
    var result = 0
    peek_data(line_text) $(arr) {
        var off = 0
        var units = 0
        for (b in arr) {
            let c = int(b)
            if ((c & int(0xC0)) != int(0x80)) {
                break if (units >= character)
                units += ((c & int(0xF8)) == int(0xF0)) ? 2 : 1
            }
            off++
        }
        result = off  // loop end without break = past the last byte = end of line
    }
    return result
}

def public utf16_length(s : string) : int {
    var units = 0
    peek_data(s) $(arr) {
        for (b in arr) {
            let c = int(b)
            if ((c & int(0xC0)) != int(0x80)) {
                units += ((c & int(0xF8)) == int(0xF0)) ? 2 : 1
            }
        }
    }
    return units
}

def public make_position(lines : array<string>; line_1based, byte_col_0based : int) : LspPosition {
    let li = max(0, line_1based - 1)
    let text = li < length(lines) ? lines[li] : ""
    return LspPosition(line = li, character = utf16_units_in(text, byte_col_0based))
}

def public make_range(lines : array<string>; line, column, last_line, last_column : int) : LspRange {
    let start = make_position(lines, line, column)
    return LspRange(
        start = start,
        end = (last_line > 0 ?
            make_position(lines, last_line, last_column) :
            LspPosition(line = start.line, character = start.character + 1)))
}

def public load_file_lines(path : string) : array<string> {
    return <- split(fread(path), "\n")
}

// user args come after the `--` separator (CLI flags like -project_root go before)
def public lsp_user_args(raw : array<string>) : array<string> {
    var user : array<string>
    var seen_sep = false
    for (a in raw) {
        if (seen_sep) {
            user |> push(a)
        } elif (a == "--") {
            seen_sep = true
        }
    }
    return <- user
}
