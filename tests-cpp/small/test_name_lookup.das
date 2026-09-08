options gen2

var g_counter = 0
var g_label = "label"
var g_scale = 2.5

[export]
def foo(x : int) : int {
    return x + 1
}

[export]
def foo(x : float) : float {
    return x * 2.0
}

[export]
def foo(x : string) : string {
    return x
}

[export]
def bar : int {
    return 42
}

[export]
def main {
    g_counter++
    print("{g_label} {g_scale}\n")
}
