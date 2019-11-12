options log = true
// options log_infer_passes = true

[unsafe]
def sort ( var a : auto[] )
    builtin_sort ( addr(a[0]), length(a) )

[unsafe]
def sort ( var a : auto(TT)[]; cmp : block<(x,y:TT):bool> )
    builtin_sort_cblock ( addr(a[0]), length(a), cmp )

[unsafe]
def sort ( var a : array<auto> )
    builtin_sort ( addr(a[0]), length(a) )

[unsafe]
def sort ( var a : array<auto(TT)>; cmp : block<(x,y:TT):bool> )
    builtin_sort_cblock ( addr(a[0]), length(a), cmp )

def tn ( var a : auto(TT)[] )
    print ( "{typeinfo(typename type TT)}\n" )

[sideeffects]
def qq ( a : auto(T1); b : block<(a1:T1;a2:T2):bool>; c : auto(T2) )
    print("QQ\n")
    return 13

[export]
def test
    qq(1,$(x,y) => true,2.0)
    // default sort
    var dim = [[int 3; 2; 4; 1]]
    sort ( dim )
    print ( "dim = {dim}\n" )
    var arr <- [{int 3; 2; 4; 1 }]
    sort ( arr )
    print ( "arr = {arr}\n" )
    // sort with block
    sort(dim, $(x,y) => x > y)
    print ( "dim = {dim}\n" )
    sort(arr) <| $(x,y:int) : bool
        return x > y
    print ( "arr = {arr}\n" )
    return true
