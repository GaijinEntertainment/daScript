options gen2

// int/uint % and %= by a variable and by a literal

[never_inline]
def mod_var(a, b : int) : int => a % b
[never_inline]
def mod_lit(a : int) : int => a % 10
[never_inline]
def umod_var(a, b : uint) : uint => a % b
[never_inline]
def umod_lit(a : uint) : uint => a % 10u
[never_inline]
def set_var(var a : int&; b : int) {
    a %= b
}
[never_inline]
def set_lit(var a : int&) {
    a %= 10
}
[never_inline]
def uset_var(var a : uint&; b : uint) {
    a %= b
}
[never_inline]
def uset_lit(var a : uint&) {
    a %= 10u
}

[export]
def main {
    let k = length(get_command_line_arguments())
    var x = 17 + k
    var y = 17u + uint(k)
    set_var(x, 5 + k)
    set_lit(x)
    uset_var(y, 5u + uint(k))
    uset_lit(y)
    print("{mod_var(x, 5 + k)} {mod_lit(x)} {umod_var(y, 5u + uint(k))} {umod_lit(y)} {x} {y}\n")
}
