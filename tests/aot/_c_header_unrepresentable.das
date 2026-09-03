options gen2

require daslib/export_c


class Klass {
    v : int
}

[export_c]
def takes_array(a : array<int>) : int {
    return length(a)
}

[export_c]
def returns_tuple() : tuple<int; int> {
    return (1, 2)
}

[export_c]
def returns_dim() : int[3] {
    return fixed_array<int>(1, 2, 3)
}
