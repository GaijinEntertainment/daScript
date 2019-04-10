[export]
def test : bool
    let tab:table<string,int>
    let i,del:int
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
        find(tab,string(i)) <| $(pValue:int?)
            if pValue!=null
                assert(deref(pValue)==i)
            else
                assert(i%7==0)
        i ++
    if true
        let j,k:int=0
        i = 0
        while i < total
            find(tab,string(i)) <| $(pVal:int?)
                j+= pVal ?? 0
            if i%7!=0
                k +=i
            else
                assert(i%7==0)
            i ++
        assert(j==k)
    if true
        let j,k,zero:int=0
        i = 0
        while i < total
            find(tab,string(i)) <| $(pVal:int?)
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
    let cnt = 0
    for k,v in keys(tab),values(tab)
        // debug(k,"key=")
        // debug(v,"value=")
        assert(k==string(v))
        cnt ++
    assert(cnt==total)
    assert(erase(tab,"1"))
    assert(!erase(tab,"-1"))
    return true


