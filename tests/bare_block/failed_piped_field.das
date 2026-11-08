//! A field access cannot take a piped block; it is a parse error
//! instead of a silently dropped block.
options gen2
expect 30347

// field access can't take a piped block (parse error, was a silent
// drop)
struct Holder {
    v : int
}

[export]
def main {
    var h = Holder(v = 1)
    let q = h.v $ {
    }
}
