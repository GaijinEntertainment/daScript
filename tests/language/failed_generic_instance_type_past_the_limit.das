//! a generic instance takes argument types of up to 4096 type
//! nodes; SizeProbe11 is a tuple tree of 8191
expect 30516:1

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

def size_of_argument(a) : int {
    return typeinfo sizeof(a)
}

[export]
def main {
    var probe : SizeProbe11
    print("{size_of_argument(probe)}\n")
}
