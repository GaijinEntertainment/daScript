//! a generic instance takes argument types of up to 4096 type
//! nodes: SizeProbe10 is a tuple tree of 4095, so one more pair
//! around it is 4097; SizeProbe11 is 8191, under an array's first
//! type and under a table's second
expect 30516:3

options gen2

typedef SizeProbe0 = tuple<int; int>
typedef SizeProbe1 = tuple<SizeProbe0; SizeProbe0>
typedef SizeProbe2 = tuple<SizeProbe1; SizeProbe1>
typedef SizeProbe3 = tuple<SizeProbe2; SizeProbe2>
typedef SizeProbe4 = tuple<SizeProbe3; SizeProbe3>
typedef SizeProbe5 = tuple<SizeProbe4; SizeProbe4>
typedef SizeProbe6 = tuple<SizeProbe5; SizeProbe5>
typedef SizeProbe7 = tuple<SizeProbe6; SizeProbe6>
typedef SizeProbe8 = tuple<SizeProbe7; SizeProbe7>
typedef SizeProbe9 = tuple<SizeProbe8; SizeProbe8>
typedef SizeProbe10 = tuple<SizeProbe9; SizeProbe9>
typedef SizeProbe11 = tuple<SizeProbe10; SizeProbe10>

def size_of_argument(a) : int => typeinfo sizeof(a)

[export]
def main {
    let one_past : tuple<SizeProbe10; int>
    var in_array : array<SizeProbe11>
    var in_table : table<int; SizeProbe11>
    print("{size_of_argument(one_past)}\n")
    print("{size_of_argument(in_array)}\n")
    print("{size_of_argument(in_table)}\n")
}
