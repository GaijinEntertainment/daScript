require dastest/testing_boost

require llvm/llvm_jit

def make_table
    return <- {{
        1 => "1";
        2 => "2";
        3 => "3"
    }}

[jit]
def make_table_jit
    return <- {{
        1 => "1";
        2 => "2";
        3 => "3"
    }}

[test]
def test_table ( t:T? )
    t |> run("make_table") <| @ ( t : T? )
        t |> success ( is_jit_function(@@make_table_jit) )
        var tab <- make_table_jit()
        t |> equal ( 3, length(tab) )
