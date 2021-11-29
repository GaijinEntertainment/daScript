options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module uriparser_boost shared public

require uriparser public

require daslib/strings_boost

def get_full_path ( uri:Uri implicit )
    return build_string() <| $ ( var tw )
        var pcur = uri.uri.pathHead
        var first = true
        while pcur != null
            if first
                first = false
            else
                tw |> write("/")
            tw |> write ( string(pcur.text) )
            pcur = pcur.next

def split_full_path ( uri:Uri implicit )
    var paths : array<string>
    var pcur = uri.uri.pathHead
    while pcur != null
        paths |> push ( string(pcur.text) )
        pcur = pcur.next
    return <- paths
