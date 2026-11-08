options gen2
options auto_inline_functions = false
// lint-skip-file: the const-stripped call is the LINT031 anchor

// program one of two sharing a callee name: fill_one writes its slot
def fill_one(var p : int?) {
    unsafe {
        *p = 1
    }
}

def route(dst : int?) {
    unsafe {
        fill_one(reinterpret<int?>(dst))
    }
}

[export]
def main() {
    var slot = 0
    unsafe {
        route(addr(slot))
    }
    print("{slot}\n")
}
