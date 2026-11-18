//! a set that takes a temporary pointer as its key would keep that
//! pointer past the temporary's life, so the insert does not compile
expect 30932

options gen2

def keep(var seen : table<int?>; item : int?#) {
    seen |> insert(item)
}

[export]
def main {
    var seen : table<int?>
    var value = 1
    unsafe {
        keep(seen, reinterpret<int?#>(addr(value)))
    }
    print("{length(seen)}\n")
}
