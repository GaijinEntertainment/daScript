require UnitTest

//options log_nodes = true
options log = true
options print_ref = true

let
    Eid0 : EntityId = make_invalid_id()

def get_int ( i : EntityId; name : string )
    print("get_int({i},{name})\n")
    return 0

[export]
def get_es ( i = Eid0 )
    unsafe
        let pi = addr(i)
        debug(pi, "pi is")
    debug(i, "i is")
    var t = get_int(i,"foo")
    t --
    t ++
    print("returned {t}\n")

[export]
def test
    debug(Eid0, "Eid0 is")
    unsafe
        let pi = addr(Eid0)
        debug(pi, "pEid0 is ")
    get_es()
    return true

