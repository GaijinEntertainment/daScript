options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_aot = true
options strict_smart_pointers = true

module peg

require peg/parse_macro public
require strings public

def public matches(var parser; template_: string; strlen: int): bool
    var mark = parser.index

    // Not sufficient supply of charachters in the input
    return false if parser.input |> length < mark + strlen

    unsafe
        if memcmp(addr(parser.input[mark]), reinterpret<uint8?> template_, strlen) == 0
            parser.index += strlen
            return true

    return false
