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
