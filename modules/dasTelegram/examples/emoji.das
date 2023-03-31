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

var broken_utf16 <- %regex~ud[89ab][0-9a-fA-F][0-9a-fA-F]ud[c-f][0-9a-fA-F][0-9a-fA-F]?%%

def public fix_broken_utf16 ( str:string )
	return broken_utf16 |> regex_replace(str) <| $ ( codePoint )
		let hs = "0x{codePoint |> slice(1, 5)}"
		let ls = "0x{codePoint |> slice(6, 10)}"
		let utf32 = utf16_to_utf32(to_uint(hs, true), to_uint(ls, true))
		return string(utf8_encode(utf32))

var broken_utf32 <- %regex~u[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]u[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]u[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]%%

def public fix_broken_utf32 ( str:string )
	return broken_utf32 |> regex_replace(str) <| $ ( codePoint )
		let l1 = "0x{codePoint |> slice(1, 5)}"
		let l2 = "0x{codePoint |> slice(6, 10)}"
		let l3 = "0x{codePoint |> slice(11, 15)}"
		let enc <- [{uint
			to_uint(l1, true);
			to_uint(l2, true);
			to_uint(l3, true)
		}]
		return string(utf8_encode(enc))

var broken_html_hex <- %regex~&#x[0-9a-fA-F]+;%%

def public fix_broken_html_hex ( str:string )
	return broken_html_hex |> regex_replace(str) <| $ ( codePoint )
		let code = "0x{codePoint |> slice(3, -1)}"
		let utf32 = to_uint(code, true)
		return string(utf8_encode(utf32))


