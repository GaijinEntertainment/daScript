options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module peg

require detail/utf8

require peg/parse_macro public
require peg/detail/colors public

require strings public
require math public

def public matches(var parser; template_: string; strlen: int): bool
    var mark = parser.index

    // Not sufficient supply of charachters in the input
    return false if parser.input |> length < mark + strlen

    unsafe
        if memcmp(addr(parser.input[mark]), reinterpret<uint8?> template_, strlen) == 0
            parser |> move(strlen)
            return true

    return false


def at_around(text: string; position, window: int): string
    return text |> chop(position - window / 2, window)


def public move(var parser; offset)
    parser.index += offset
    parser.longest_prefix = parser.longest_prefix |> max <| parser.index


def public tabulate(var parser)
    print("{repeat("|   ", parser.tabs)}")


def public log_plain(var parser, message)
    parser |> tabulate
    print("[-] {message}\n")


def public log_fail(var parser, message)
    parser |> tabulate
    print("[!] {message}\n" |> red_str <| parser.color_output)


def public log_success(var parser, message)
    parser |> tabulate
    print("[!] {message}\n"  |> green_str <| parser.color_output)


def public log_info(var parser, message)
    parser |> tabulate
    print("[.] {message}\n" |> blue_str <| parser.color_output)


def public reached_EOF(var parser): bool
    return parser.input |> length <= parser.index


def public skip_whitespace(var parser)
    while parser |> get_current_char |> is_white_space
        parser |> move(1)


def public get_current_char(var parser): int
    return -1 if parser.input |> length <= parser.index
    return parser.input[parser.index] |> int()


// def public move_to_next_char(var parser)
//     if parser.input |> length <= parser.index
//         parser.current = -1 // EOF
//     else
//         parser.current = get_next_character(parser.input, parser.index)


// def public current_index(var parser): int
//     var backtrack_len = parser.current |> get_utf8_byte_count
//     return parser.index - backtrack_len


// def public current_buffer_pos(var parser): string
//     var backtrack_len = parser.current |> get_utf8_byte_count
//     return parser.input[parser.index - backtrack_len] |> string()


// def public get_current_char(var parser): int
//     return parser.current


def public match_decimal_literal(var parser): tuple<success:bool; value:int; endpos:int>
    //! Simple lexing of decimal integers, doesn't check for overflow

    if !parser |> get_current_char |> is_number
        // Can't use typedef in this context, sadly
        return [[tuple<success:bool; value:int; endpos:int>]]

    var result = 0

    while parser |> get_current_char |> is_number
        result *= 10
        result +=  parser |> get_current_char - '0'
        parser |> move(1)

    return [[auto true, result, parser.index]]


def public match_string_literal(var parser): tuple<success:bool; string>
    //! Tries to match everything inside ""

    // Define buffer to store string literal characters
    var inscope buffer: array<uint8>

    var current_char = parser |> get_current_char

    // If the current character is not a double quote, the rule is not a string
    return [[auto false, ""]] if current_char != '"'

    parser |> move(1)
    current_char = parser |> get_current_char

    while current_char != '"' && !parser |> reached_EOF
        buffer |> push(current_char |> uint8())
        parser |> move(1)
        current_char = parser |> get_current_char

    // If we've reached EOF file without finding a closing quote
    return [[auto false, ""]] if parser |> reached_EOF

    parser |> move(1)
    return [[auto true, buffer |> string()]]


def public match_double_literal(var parser): tuple<success:bool; double>
    //! Matches doubles in the form of [-+]? [0-9]* .? [0-9]+ ([eE] [-+]? [0-9]+)?
    //! The number is not checked to be representable as defined in IEEE-754

    var current_char = parser |> get_current_char

    if !current_char |> is_number && current_char != '+' && current_char != '-'
        return <- [[auto false, 0.0 |> double()]]

    var inscope buffer: array<uint8>

    if current_char == '-' || current_char == '+'
        buffer |> push(current_char |> uint8())
        parser |> move(1)
        current_char = parser |> get_current_char

    // Add everything up to '.' to the buffer
    while current_char |> is_number || current_char == '.'
        buffer |> push(current_char |> uint8())
        parser |> move(1)
        current_char = parser |> get_current_char


    // Match exponent part
    if current_char == 'e' || current_char == 'E'
        buffer |> push(current_char |> uint8())
        parser |> move(1)
        current_char = parser |> get_current_char

        // Check for '-' or '+' after 'e' or 'E'
        if current_char == '-' || current_char == '+'
            buffer |> push(current_char |> uint8())
            parser |> move(1)
            current_char = parser |> get_current_char

        // Continue appending digits after 'e' or 'E'
        while current_char |> is_number
            buffer |> push(current_char |> uint8())
            parser |> move(1)
            current_char = parser |> get_current_char

    return [[auto true, buffer |> string() |> double()]]
