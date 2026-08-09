options gen2
require daslib/strings_boost

// the write(writer, fresh-string-chain) shape json::write_value carries — the exact
// call-site the temp-string wrapper pass rewrites
[export]
def write_trimmed(var writer : StringBuilderWriter; x : double) {
    write(writer, fmt(":.17f", x) |> rtrim("0") |> rtrim("."))
}
