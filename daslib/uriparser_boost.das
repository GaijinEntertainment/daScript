options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module uriparser_boost shared public

require uriparser public

require daslib/strings_boost

def uri_get_full_path ( uri:Uri implicit )
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

def uri_split_full_path ( uri:Uri implicit )
    var paths : array<string>
    var pcur = uri.uri.pathHead
    while pcur != null
        paths |> push ( string(pcur.text) )
        pcur = pcur.next
    return <- paths

def uri_compose_query ( query:table<string;string> )
    return build_string <| $ ( var tw )
        var first = true
        for key,value in keys(query),values(query)
            if first
                first = false
            else
                tw |> write("&")
            tw |> write(key)
            if !empty(value)
                tw |> write("=")
                tw |> write(value)

def uri_compose_query_in_order ( query:array<tuple<key:string;value:string>> )
    return build_string <| $ ( var tw )
        var first = true
        for kv in query
            if first
                first = false
            else
                tw |> write("&")
            tw |> write(kv.key)
            if !empty(kv.value)
                tw |> write("=")
                tw |> write(kv.value)
