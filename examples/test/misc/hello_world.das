require daslib/match

struct AB
    a, b : int

struct Bar
    a : int
    b : float

def or_match ( B:Bar )
    match B
        if [[Bar a=1, b=$v(b)]] || [[Bar a=2, b=$v(b)]]
            return b
        if _
            return 0.0

[sideeffects]
def guards_match ( ab : AB )
    match ab
        if [[AB a=$v(a), b=$v(b)]] && (b > a)
            return "{b} > {a}"
        if [[AB a=$v(a), b=$v(b)]]
            return "{b} <= {a}"


struct Foo
    a : int

def struct_match ( f : Foo )
    match f
        if [[Foo a=13]]
            return 0
        if [[Foo a=$v(anyA)]]
            return anyA

enum Color
    black
    red
    green
    blue

def enum_match ( color:Color )
    match color
        if Color black
            return 0
        if Color red
            return 1
        if _
            return -1

variant IF
    i : int
    f : float

def variant_as_match ( v : IF )
    match v
        if _ as i
            return "int"
        if _ as f
            return "float"
        if _
            return "anything"

[sideeffects]
def static_match_by_type ( what )
    static_match what
        if match_type<int> $v(expr)
            return "int {expr}"
        if _
            return "anything"

[export]
def main
    print("hello, world!\n")
    static_match_by_type(1)
    static_match_by_type(1.0)

options log
