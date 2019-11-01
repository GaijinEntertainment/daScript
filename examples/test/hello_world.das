options log=true


[export]
def test
    var tab <- {{ "one" => 1; "two" => 2 }}
    var failed = false
    try
        find(tab,"one") <| $(t:int?)
            assert(t!=null)
            tab["three"] = 3
    recover
        failed = true
    failed = false
    try
        find(tab,"three") <| $(t:int?)
            assert(t==null)
            erase(tab,"three")
    recover
        failed = true
    assert(failed)
    return true
