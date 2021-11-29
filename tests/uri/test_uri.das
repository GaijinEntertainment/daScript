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
        t |> equal(get_full_path(uri),"one/two/three")
        var spath <- split_full_path(uri)
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