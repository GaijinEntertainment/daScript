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
    return first_character(s)
}

def first_piped(s : string) : int {
    return first_character(s)
}

def same_name(name : das_string) : bool {
    return name == "foo" || "bar" != name
}

def field_name(v : Variable?) : bool {
    return v.name == "x"
}

def null_check(prog : ProgramPtr) : bool {
    return prog == null
}

def field_read(prog : ProgramPtr) : bool {
    return prog.flags.failToCompile
}

def bump(var a : int; var f : float; var arr : array<int>) : int {
    a++
    a--
    f++
    arr[0]++
    let b = a
    a++ // keeps its comment
    return b
}

def is_digit(c : int) : bool {
    return is_number(c)
}

def not_digit(c : int) : bool {
    return !is_number(c)
}

def letter(c : int) : bool {
    return is_alpha(c)
}

def smaller(a, b : int) : int {
    return min(a, b)
}

def larger(a, b : float) : float {
    return max(a, b)
}

def magnitude(x : int) : int {
    return abs(x)
}

def has_items(s : string; arr : array<int>; tab : table<int; int>) : bool {
    return empty(s) || !empty(arr) || !empty(tab)
}

def flags(m : Mode) : int {
    return int(m | Mode.read)
}

def same_cast(a : int; f : float; s : string) : float {
    return float(a + 1) + f + float(length(s))
}

def hoist(c : bool; a, b : uint8) : int {
    return int(c ? a : b)
}

def interp(n : int; s : string; f : float) : string {
    return "{n} {s} {f}"
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
    return int(c ? (d ? a : b) : b)
}
