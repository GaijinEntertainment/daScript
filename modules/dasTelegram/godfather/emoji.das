options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module emoji shared private

require daslib/regex_boost
require daslib/strings_boost
require daslib/utf8_utils

let replacing_emoji <- [{auto
// regular smiles
    ":)" => "🙂";
    ":)" => "🙂";
    ":(" => "☹️";
    ":D" => "😀";
    ";)" => "😉";
    ":-|" => "😐";
// *foo* format
    "*smirk*" => "😏";
    "*smile*" => "😊";
    "*wink*" => "😉";
    "*angry*" => "😠";
    "*neutral_face*" => "😐";
    "*grin*" => "😁";
    "*innocent*" => "😇";
    "*heart_eyes*" => "😍";
    "*kissing_heart*" => "😘";
// :foo: format
    ":smirk" => "😏";
    ":smile" => "😊";
    ":wink" => "😉";
    ":angry:" => "😠";
    ":neutral_face:" => "😐";
    ":grin:" => "😁";
    ":innocent:" => "😇";
    ":heart_eyes:" => "😍";
    ":kissing_heart:" => "😘";
// bad unicode?
    "ðŸ˜" => "😃";
    "ðŸ˜Ž" => "😄";
    "ðŸ§" => "👀";
    "ðŸ¤‘" => "😂";
    "ðŸ•¶ï¸" => "👍";
    "ðŸ˜’" => "😀"
}]

def public fix_emoji ( str:string )
    return str |> replace_multiple(replacing_emoji)

var broken_utf16 <- %regex~[uU][dD][89ab][0-9a-fA-F][0-9a-fA-F][uU][dD][c-f][0-9a-fA-F][0-9a-fA-F]?%%

def public fix_broken_utf16 ( str:string )
	return broken_utf16 |> regex_replace(str) <| $ ( codePoint )
		let hs = "0x{codePoint |> slice(1, 5)}"
		let ls = "0x{codePoint |> slice(6, 10)}"
		let utf32 = utf16_to_utf32(to_uint(hs, true), to_uint(ls, true))
		return string(utf8_encode(utf32))

def public fix_broken_utf32 ( str:string )
    //! fix all instances of uXXXX into the correct utf8
    return build_string <| $ ( writer )
        peek_data(str) <| $ ( p )
            var i = 0
            let llen = p |> length
            var bang : array<uint8>
            bang |> resize(4)
            while i < llen
                if p[i]=='u'u8 && (i+5<=llen)
                    if is_hex(int(p[i+1])) && is_hex(int(p[i+2])) && is_hex(int(p[i+3])) && is_hex(int(p[i+4]))
                        bang[0] = p[i+1]
                        bang[1] = p[i+2]
                        bang[2] = p[i+3]
                        bang[3] = p[i+4]
                        let utf32 = to_uint(string(bang), true)
                        writer |> write(string(utf8_encode(utf32)))
                        i += 5
                        continue
                writer |> write_char(int(p[i]))
                i += 1

var broken_html_hex <- %regex~&#x[0-9a-fA-F]+;%%

def public fix_broken_html_hex ( str:string )
	return broken_html_hex |> regex_replace(str) <| $ ( codePoint )
		let code = "0x{codePoint |> slice(3, -1)}"
		let utf32 = to_uint(code, true)
		return string(utf8_encode(utf32))

def public fix_broken_escaping ( prompt:string )
    return build_string <| $ ( writer )
        for Ch in prompt
            if Ch<=31
                writer |> write("\\u00{(Ch>>4)+'0'}{(Ch&15)+'0'}")
            else
                writer |> write_char(Ch)

