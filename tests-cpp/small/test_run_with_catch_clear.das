options gen2

[export]
def panic_func() {
    panic("intentional panic from test")
}

[export]
def add_one(x : int) : int {
    return x + 1
}
