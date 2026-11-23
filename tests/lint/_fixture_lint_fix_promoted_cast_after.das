//fmt:ignore-file
options gen2
options auto_inline_functions = false
options infer_time_folding = false

def scaled(x : float) : float => x * 2

def shifted(x : float) : float {
    var f = x
    f += 1
    return f
}

[export]
def main {
    print("{scaled(1.0)} {shifted(1.0)}\n")
}
