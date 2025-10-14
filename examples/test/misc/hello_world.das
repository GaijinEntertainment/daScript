require t_counter

typedef IntCounter = $counter(type<int>)

[export]
def main() {
    unsafe {
        var a = default<IntCounter>
        a.next_count()
        debug(a)
    }
}

options log

