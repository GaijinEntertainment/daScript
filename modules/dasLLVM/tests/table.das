require dastest/testing_boost

require llvm/llvm_jit

def make_table
    return <- {{
        1 => "1";
        2 => "2";
        3 => "3"
    }}

def test_table ( var tab:table<int; string> )
    return length(tab)==3 && tab[1]=="1" && tab[2]=="2" && tab[3]=="3"

[jit]
def make_table_jit
    return <- {{
        1 => "1";
        2 => "2";
        3 => "3"
    }}

[jit]
def test_table_jit ( var tab:table<int; string> )
    return length(tab)==3 && tab[1]=="1" && tab[2]=="2" && tab[3]=="3"

[jit]
def tab_erase ( var tab:table<int;string>; key:int )
    __builtin_table_erase(tab,key)

[jit]
def tab_set_insert ( var tab:table<int>; key:int )
    __builtin_table_set_insert(tab,key)

[test]
def test_table ( t:T? )
    t |> run("make_table") <| @ ( t : T? )
        t |> success ( is_jit_function(@@make_table_jit) )
        var tab <- make_table_jit()
        t |> success ( test_table(tab) )
    t |> run("[], size") <| @ ( t : T? )
        t |> success ( is_jit_function(@@test_table_jit) )
        var tab <- make_table()
        t |> success ( test_table_jit(tab) )
    t |> run("erase") <| @ ( t : T? )
        var tab <- {{
            1 => "1";
            2 => "2";
            3 => "3"
        }}
        tab_erase(tab, 2)
        t |> success ( key_exists(tab, 1) )
        t |> success ( !key_exists(tab, 2) )
        t |> success ( key_exists(tab, 3) )
    t |> run("set insert") <| @ ( t : T? )
        var tab <- {{ 1; 2; 4 }}
        t |> success ( key_exists(tab, 1) )
        t |> success ( key_exists(tab, 2) )
        t |> success ( !key_exists(tab, 3) )
        t |> success ( key_exists(tab, 4) )
        tab_set_insert(tab, 3)
        t |> success ( key_exists(tab, 1) )
        t |> success ( key_exists(tab, 2) )
        t |> success ( key_exists(tab, 3) )
        t |> success ( key_exists(tab, 4) )
