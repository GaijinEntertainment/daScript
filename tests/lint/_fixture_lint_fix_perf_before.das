//fmt:ignore-file
options gen2
options auto_inline_functions = false

require strings
require math
require daslib/ast_boost

bitfield Mode {
    read
    write
}

def first(s : string) : int {
    return character_at(s, 0)
}

def first_piped(s : string) : int {
    return s |> character_at(0)
}

def same_name(name : das_string) : bool {
    return string(name) == "foo" || "bar" != string(name)
}

def field_name(v : Variable?) : bool {
    return string(v.name) == "x"
}

def null_check(prog : ProgramPtr) : bool {
    return get_ptr(prog) == null
}

def field_read(prog : ProgramPtr) : bool {
    return get_ptr(prog).flags.failToCompile
}

def bump(var a : int; var f : float; var arr : array<int>) : int {
    a += 1
    a -= 1
    f += 1.0
    arr[0] += 1
    let b = a
    a += 1 // keeps its comment
    return b
}

def is_digit(c : int) : bool {
    return c >= '0' && c <= '9'
}

def not_digit(c : int) : bool {
    return c < '0' || c > '9'
}

def letter(c : int) : bool {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
}

def smaller(a, b : int) : int {
    return a < b ? a : b
}

def larger(a, b : float) : float {
    return a > b ? a : b
}

def magnitude(x : int) : int {
    return x < 0 ? -x : x
}

def has_items(s : string; arr : array<int>; tab : table<int; int>) : bool {
    return length(s) == 0 || length(arr) > 0 || 0 != length(tab)
}

def flags(m : Mode) : int {
    return int(m) | int(Mode.read)
}

def same_cast(a : int; f : float; s : string) : float {
    return float(int(a) + 1) + float(f) + float(length(string(s)))
}

def hoist(c : bool; a, b : uint8) : int {
    return c ? int(a) : int(b)
}

def interp(n : int; s : string; f : float) : string {
    return "{string(n)} {string(s)} {string(f)}"
}

def second(s : string) : int {
    return character_at(s, 1)
}

def generic_first(s) : int {
    return character_at(s, 0)
}

def use_generic() : int {
    return generic_first("x")
}

def unsigned_interp(u : uint) : string {
    return "{string(u)}"
}

def smaller_commented(a, b : int) : int {
    return (a < b ? a // the left one
        : b)
}

def hoist_nested(c, d : bool; a, b : uint8) : int {
    return c ? int(d ? a : b) : int(b)
}
