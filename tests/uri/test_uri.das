require dastest/testing_boost public

require daslib/uriparser_boost
require daslib/strings_boost
require daslib/defer

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
        t |> equal(uri_get_full_path(uri),"one/two/three")
        var spath <- uri_split_full_path(uri)
        t |> equal(spath[0], "one")
        t |> equal(spath[1], "two")
        t |> equal(spath[2], "three")

[test]
def test_normalize_and_rebase ( t : T? )
    t |> run("normalize") <| @@ ( t : T? )
        let uriStr = "file://somepath/./deadend/.././subpath/file.txt#somefrag"
        using(uriStr) <| $ ( var uri:Uri# )
            uri |> normalize
            t |> equal(string(uri), "file://somepath/subpath/file.txt#somefrag")
    t |> run("add base uri") <| @@ ( t : T? )
        unsafe
            var relUri <- Uri("images/any")
            defer_delete(relUri)
            var baseUri <- Uri("http://www.google.com")
            defer_delete(baseUri)
            var uri <- relUri |> add_base_uri(baseUri)
            t |> equal(string(uri),"http://www.google.com/images/any")
    t |> run("remove base uri") <| @@ ( t : T? )
        unsafe
            var absUri <- Uri("http://www.google.com/images/any")
            defer_delete(absUri)
            var baseUri <- Uri("http://www.google.com/images")
            defer_delete(baseUri)
            var uri <- absUri |> remove_base_uri(baseUri)
            t |> equal(string(uri),"any")
    t |> run("strip") <| @@ ( t : T? )
        let uriStr = "file://somepath/file.txt?a=1&b#somefrag"
        using(uriStr) <| $ ( var uri:Uri# )
            unsafe
                var uriS <- uri |> strip_uri(true,true)
                defer_delete(uriS)
                t |> equal(string(uriS), "file://somepath/file.txt")

[test]
def test_uri_file_name ( t : T? )
    t |> run("to_file_name") <| @@ ( t : T? )
        let uriStr = "file:///somedir/somefile.txt"
        using(uriStr) <| $ ( var uri:Uri# )
            let wfn = uri |> to_windows_file_name
            let ufn = uri |> to_unix_file_name
            let afn = uri |> to_file_name
            t |> equal(wfn, "somedir\\somefile.txt")
            t |> equal(ufn, "/somedir/somefile.txt")
            t |> success(wfn==afn || ufn==afn)
    t |> run("from_windows_file_name") <| @@ ( t : T? )
        unsafe
            var uri <- uri_from_windows_file_name("C:\\somedir\\somefile.txt")
            defer_delete(uri)
            t |> equal(string(uri), "file:///C:/somedir/somefile.txt")
    t |> run("from_unix_file_name") <| @@ ( t : T? )
        unsafe
            var uri <- uri_from_unix_file_name("/somedir/somefile.txt")
            defer_delete(uri)
            t |> equal(string(uri), "file:///somedir/somefile.txt")

[test]
def test_uri_query ( t : T? )
    t |> run("split query") <| @@ ( t : T? )
        let uriStr = "something?one=1&two=2&three=3&nothing"
        using(uriStr) <| $ ( var uri:Uri# )
            var count = 0
            uri |> uri_for_each_query_kv <| $ ( k, v )
                count ++
                if k=="one"
                    t |> equal(v, "1")
                elif k=="two"
                    t |> equal(v, "2")
                elif k=="three"
                    t |> equal(v, "3")
                elif k=="nothing"
                    t |> equal(v, "")
                else
                    t->fatal("not expecting {k}={v}")
            t |> equal(count, 4)
    t |> run("compose query") <| @@ ( t : T? )
        var qq <- {{
            "one" => "1";
            "two" => "2";
            "three" => "3";
            "nothing" => ""
        }}
        let qstr = uri_compose_query(qq)
        unsafe
            var quri <- Uri("http://boo.hoo/?" + qstr)
            print("{string(quri)} QURI\n")





