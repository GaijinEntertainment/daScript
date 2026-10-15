options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/option

def private some_int(v : int) : Option<int> => some(v)

def private some_int_dollar(v : int) : $Option<int> => some(v)

def private halve(v : uint64) : uint64 => v >> 1ul

def private rotate_right(v : uint) : uint => v >>> 1u

[test]
def test_shift_after_type_macro(t : T?) {
    t |> run("type macros parse") @(t : T?) {
        t |> equal(some_int(3) ?? 0, 3)
        t |> equal(some_int_dollar(4) ?? 0, 4)
    }
    t |> run("a shift after them is still a shift") @(t : T?) {
        t |> equal(halve(8ul), 4ul)
        t |> equal(rotate_right(2u), 1u)
    }
}
