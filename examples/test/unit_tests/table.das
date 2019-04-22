[export]
def test : bool
    var tab:table<string,int>
    var i,del:int
    let total:int=32
    assert(!erase(tab,"0"))
    i = 0
    while i!=total
        tab[string(i)] = i++
    assert(length(tab)==total)
    // debug(tab)
    i = 0
    while i!=total
        assert(tab[string(i)]==i++)
    assert(length(tab)==total)
    // debug(tab)
    i = 0
    del = 0
    while i < total
        assert(erase(tab,string(i)))
        assert(!erase(tab,string(i)))
        del ++
        i += 7
    assert(length(tab)==total-del)
    // debug(tab)
    i = 0
    while i < total
        find(tab,string(i)) <| $(var pValue:int?)
            if pValue!=null
                assert(deref(pValue)==i)
            else
                assert(i%7==0)
        i ++
    if true
        var j,k:int=0
        i = 0
        while i < total
            find(tab,string(i)) <| $(var pVal:int?)
                j+= pVal ?? 0
            if i%7!=0
                k +=i
            else
                assert(i%7==0)
            i ++
        assert(j==k)
    if true
        var j,k,zero:int=0
        i = 0
        while i < total
            find(tab,string(i)) <| $(var pVal:int?)
                j += pVal ?? zero
            if i%7!=0
                k +=i
            i ++
        assert(j==k)
    // debug(tab)
    i = 0
    while i < total
        tab[string(i)] = i
        i += 7
    assert(length(tab)==total)
    // debug(tab)
    i = 0
    while i!=total
        assert(tab[string(i)]==i++)
    assert(length(tab)==total)
    // debug(tab)
    var cnt = 0
    for k,v in keys(tab),values(tab)
        // debug(k,"key=")
        // debug(v,"value=")
        assert(k==string(v))
        cnt ++
    assert(cnt==total)
    assert(erase(tab,"1"))
    assert(!erase(tab,"-1"))
    return true


