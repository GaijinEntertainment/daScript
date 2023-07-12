options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module peg_utf8

// Array is asserted to be valid utf-8 at the beginning of the parsing.
def public get_next_character(utf8_string: array<uint8>; var index: int&): int
    var codepoint = 0
    var state = 0u

    while true // Guaranteed to terminate
        let byte = utf8_string[index] |> int()
        index += 1
        let type_ = s_utf8d[byte]
        codepoint = (byte & int(0x3F)) | (codepoint << 6)
        state = s_utf8d[256u + state + type_]
        return codepoint if state == UTF8_ACCEPT

    return 0


def public decode_utf8_codepoint(utf8_data: array<uint8>; var index: int): tuple<res:int; len:int>
    var byte1 = utf8_data[index] |> uint()

    if (byte1 & 0x80u) == 0u
        // Single-byte codepoint
        return [[auto byte1 |> int(), 1]]
    elif (byte1 & 0xE0) == 0xC0
        // Two-byte codepoint
        var byte2 = utf8_data[index + 1] |> uint()
        var val = ((byte1 & 0x1F) << 6u) | (byte2 & 0x3F)
        return [[auto val |> int(), 2]]
    elif (byte1 & 0xF0) == 0xE0
        // Three-byte codepoint
        var byte2 = utf8_data[index + 1] |> uint()
        var byte3 = utf8_data[index + 2] |> uint()
        var val = ((byte1 & 0x0F) << 12u) | ((byte2 & 0x3F) << 6u) | (byte3 & 0x3F)
        return [[auto val |> int(), 3]]
    elif (byte1 & 0xF8) == 0xF0
        // Four-byte codepoint
        var byte2 = utf8_data[index + 1] |> uint()
        var byte3 = utf8_data[index + 2] |> uint()
        var byte4 = utf8_data[index + 3] |> uint()
        var val = ((byte1 & 0x07) << 18u) | ((byte2 & 0x3F) << 12u) | ((byte3 & 0x3F) << 6u) | (byte4 & 0x3F)
        return [[auto val |> int(), 4]]
    else
        // Invalid UTF-8 sequence
        return [[auto -1, 0]]
