
require UnitTest
require daslib/contracts

[expect_any_vector(blah)]
def print_vec ( blah )
    for i in range(length(blah))
        print("{blah[i]}\n")

[expect_any_vector(blah)]
def print_vec_length ( blah : auto(DummyName) )
    print("length(blah) is {length(blah)}\n")

def print_vec_length ( blah )
    print("blah is not a vector\n")

[expect_any_array(blah)]
def print_arr ( blah )
    for i in range(length(blah))
        print("{blah[i]}\n")

[export]
def main
    testPoint3Array() <| $(arr)
        print_vec(arr)
        print_vec_length(arr)
        print_arr(arr)
    // print_vec(13)
    // 30304: no matching functions or generics print_vec ( int const )
    //  candidate function:
    //        print_vec ( blah : auto const ) : auto at hello_world.das:6:4
    //                argument blah is expected to be a vector template
    var i : int[3]
    print_arr(i)
    print_vec_length(13)
    return true
