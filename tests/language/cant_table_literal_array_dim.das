//! a table literal as an array dimension is diagnosed, not
//! asserted on
options gen2
expect 30109, 30194, 30310, 30915

def operator.(r; v) : int[{1 => 2}]{}

[export]
def table_literal_array_dim {
    var g <- generator<int>() <| ${ yield 1 }
}
