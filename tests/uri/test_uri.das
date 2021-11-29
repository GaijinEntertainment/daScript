require dastest/testing_boost public

require uriparser
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

[test]
def test_uri ( t : T? )
    let uriStr = "scheme://userinfo@host:1234/one/two/three?quone=one&qtow=2#someframgnet"
    using(uriStr) <| $ ( var uri:Uri# )
        t |> equal(string(uri), uriStr)
        t |> equal(string(uri.uri.scheme),"scheme")
        t |> equal(string(uri.uri.userInfo),"userinfo")
        t |> equal(string(uri.uri.hostText),"host")
        t |> equal(string(uri.uri.portText),"1234")
        t |> equal(string(uri.uri.query),"quone=one&qtow=2")
        t |> equal(string(uri.uri.fragment),"someframgnet")
        t |> equal(get_full_path(uri),"one/two/three")
        var spath <- split_full_path(uri)
        t |> equal(spath[0], "one")
        t |> equal(spath[1], "two")
        t |> equal(spath[2], "three")

