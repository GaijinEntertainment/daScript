options gen2
options stack = 32768
options heap_size_hint = 65536

// Tier C: the shapes that need the function table at runtime. A lambda, a
// function pointer and a generator are all values that name a function by its
// mangled-name hash and look it up when invoked, so this is what proves the
// lookup tables the generated constructor fills are real on nano.

[export]
def apply_twice(x : int) : int {
    let add3 = @(v : int) : int => v + 3
    return invoke(add3, invoke(add3, x))
}

def double_it(x : int) : int {
    return x + x
}

[export]
def call_through_pointer(x : int) : int {
    let fp = @@double_it
    return invoke(fp, x)
}

// A generator is a lambda with a resume point: each `yield` returns and the
// next call continues where it left off. The capture frame lives on the heap.
[export]
def sum_squares(n : int) : int {
    var gen <- generator<int> {
        for (i in range(n)) {
            yield i * i
        }
        return false
    }
    var total = 0
    for (v in gen) {
        total += v
    }
    return total
}

// A capturing closure. The capture frame is a heap allocation with a finalizer,
// so this is the shape that proves nano frees lambdas as well as calls them.
[export]
def count_up_to(limit : int) : int {
    var count = 0
    unsafe {
        let step <- @ capture(& count) {
            count++
        }
        while (count < limit) {
            invoke(step)
        }
    }
    return count
}
