options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

require daslib/utf8_utils
require daslib/strings

module utf8 shared

// Array is asserted to be valid utf-8 at the beginning of the parsing.
def public get_next_character(utf8_string: array<uint8>; var index: int&): int
    var codepoint = 0u
    var state = 0u

    while true
        let byte = utf8_string[index++] |> uint()
        let type_ = s_utf8d[byte]

        if state == UTF8_ACCEPT
            codepoint = (0xFF >> type_) & byte
        else
            codepoint = (byte & 0x3F) | (codepoint << 6u)

        state = s_utf8d[256u + state + type_]

        if state == UTF8_ACCEPT
            return unsafe (reinterpret<int> codepoint)
        if state == 1u // UTF8_REJECT
            return -1

    return 0


def public get_utf8_byte_count(code_point: int): int
    if code_point == -1
        return 0

    if code_point <= 0x7F |> int()
        return 1
    elif code_point <= 0x7FF |> int()
        return 2
    elif code_point <= 0xFFFF |> int()
        return 3
    elif code_point <= 0x10FFFF |> int()
        return 4

    panic("Not a unicode symbol")
    return 0
