def test : bool
    let(tab:table<string,int>;i,del:int;total:int=1000)
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
        i = del = 0
        while i < total
            assert(erase(tab,string(i)))
            assert(!erase(tab,string(i)))
            del ++
            i += 7
        assert(length(tab)==total-del)
        // debug(tab)
        i = 0
        while i < total
            let ( pKey:int* = find(tab,string(i)))
                if pKey!=null
                    assert(pKey->==i)
                else
                    assert(i%7==0)
            i ++
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
        for k,v in keys(tab),values(tab)
            // debug(k)
            // debug(v)
            assert(k==string(v))
        assert(erase(tab,"1"))
        assert(!erase(tab,"-1"))
    return true


