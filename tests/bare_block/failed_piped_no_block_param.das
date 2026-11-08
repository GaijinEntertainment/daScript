//! A piped block on a function with no block parameter is rejected
//! because it has nowhere to land.
options gen2
expect 30341

// no block-like parameter at all - the piped block has nowhere to
// land
def scalar_only(a : int; b : float = 1.0) {
}

[export]
def main {
    scalar_only(1) $ {
    }
}
