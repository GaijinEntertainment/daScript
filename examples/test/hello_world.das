options log = true
// options log_infer_passes = true

[unsafe]
def lock ( Tab : table<auto(keyT);auto(valT)>; blk : block<(t : table<keyT;valT>#):void >  )
    __builtin_table_lock(Tab)
    invoke(blk,reinterpret<table<auto(keyT);auto(valT)> const!>(Tab))
    __builtin_table_unlock(Tab)

[export]
def test
    var t <- {{ "one" => 1; "two" => 2 }}
    print("t = {t}\n")
    var tClone : table<string;int>
    lock ( t ) <| $ ( localT )
        debug(localT,"localT = ")
        let l = length(localT)
        print("length(localT) = {l}\n")
        tClone := localT
    return true
