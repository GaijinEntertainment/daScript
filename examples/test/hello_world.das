options log=true

[export,unsafe]
def test
    let const_tab <- {{ "one"=>1; "two"=>2 }}
    find(const_tab,"one") <| $(num:int?)
        assert(deref(num)==1)
    assert(deref(find(const_tab,"one"))==1)

    // find(const_tab,"one") <| $(var num:int?)    // TODO: top level?
    //     assert(deref(num)==1)
    //     deref(num) = 3

    var tab <- {{ "one"=>1; "two"=>2 }}
    find(tab,"one") <| $(num:int?)
        assert(deref(num)==1)
    find_for_edit(tab,"one") <| $(var num:int?)
        assert(deref(num)==1)
        deref(num) = 3
    assert(deref(find(tab,"one"))==3)
    deref(find_for_edit(tab,"one"))=1
    assert(deref(find_for_edit(tab,"one"))==1)

	return true
