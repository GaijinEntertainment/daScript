options log=true

[export,unsafe]
def test
    let const_tab <- {{ "one"=>1; "two"=>2 }}
    find(const_tab,"one") <| $(num:int?)
        assert(*num==1)
    assert(*find(const_tab,"one")==1)

    // find(const_tab,"one") <| $(var num:int?)
    //     assert(*num==1)
    //     *num = 3

    var tab <- {{ "one"=>1; "two"=>2 }}
    find(tab,"one") <| $(num:int?)
        assert(*num==1)
    find_for_edit(tab,"one") <| $(var num:int?)
        assert(*num==1)
        *num = 3
    assert(*find(tab,"one")==3)
    *find_for_edit(tab,"one")=1
    assert(*find_for_edit(tab,"one")==1)

	return true
