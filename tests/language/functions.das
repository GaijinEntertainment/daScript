options gen2
options no_unused_function_arguments = false
require dastest/testing_boost public
require strings

[sideeffects, no_jit]
def unused_index() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        var _unused = arr[10]
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_div(num : int; den : int) : bool {
    var ex = false
    try {
        var _unused = num / den
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_first_character(s : string) : bool {
    var ex = false
    try {
        var _unused = first_character(s)
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def dead_store() : bool {
    let arr <- [1, 2]
    var y = 0
    var ex = false
    try {
        y = arr[10]
        y = 5
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def same_expression() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        var _unused = arr[10] - arr[10]  // nolint:LINT007 -- deliberate: the fold under test
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def folded_operand() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        var _unused = arr[10] * 0
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def dropped_condition() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        if (arr[10] > 0) {
        }
    } recover {
        ex = true
    }
    return ex
}

def at_ten(a : array<int>) : int => a[10]

[sideeffects, no_jit]
def unused_user_call() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        var _unused = at_ten(arr)
    } recover {
        ex = true
    }
    return ex
}

struct Boxed {
    v : int
}

def operator +(a, b : Boxed) : Boxed {
    let arr <- [a.v, b.v]
    return Boxed(v = arr[10])
}

[sideeffects, no_jit]
def unused_user_operator() : bool {
    var ex = false
    try {
        var _unused = Boxed(v = 1) + Boxed(v = 2)
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_mod(num : int; den : int) : bool {
    var ex = false
    try {
        var _unused = num % den
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_string_to_int(s : string) : bool {
    var ex = false
    try {
        var _unused = int(s)
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_invoked_block() : bool {
    let arr <- [1, 2]
    var ex = false
    try {
        let blk = $() : int {
            return arr[10]
        }
        var _unused = invoke(blk)
    } recover {
        ex = true
    }
    return ex
}

[never_inline]
def asserted(x : int) : int {
    assert(x > 0, "not positive")
    return x
}

[never_inline]
def verified(x : int) : int {
    verify(x > 0, "not positive")
    return x
}

[sideeffects, no_jit]
def unused_asserted_call() : bool {
    var ex = false
    try {
        var _unused = asserted(-1)
    } recover {
        ex = true
    }
    return ex
}

[sideeffects, no_jit]
def unused_verified_call() : bool {
    var ex = false
    try {
        var _unused = verified(-1)
    } recover {
        ex = true
    }
    return ex
}

[test(description = "an expression whose value is never used still raises its runtime error - index, division, modulo, conversion, user call, user operator, block invoke, assert and verify - instead of being optimized away")]
def test_unused_may_raise(t : T?) {
    t |> run("unused out of range index still raises") @(t : T?) {
        t |> equal(unused_index(), true)
    }
    t |> run("unused division by zero still raises") @(t : T?) {
        t |> equal(unused_div(7, 0), true)
    }
    t |> run("unused first_character of an empty string still raises") @(t : T?) {
        t |> equal(unused_first_character(""), true)
    }
    t |> run("a dead store whose value raises still raises") @(t : T?) {
        t |> equal(dead_store(), true)
    }
    t |> run("E op E folding keeps the raise") @(t : T?) {
        t |> equal(same_expression(), true)
    }
    t |> run("an operand folded away by x * 0 still raises") @(t : T?) {
        t |> equal(folded_operand(), true)
    }
    t |> run("the condition of an empty if still raises") @(t : T?) {
        t |> equal(dropped_condition(), true)
    }
    t |> run("an unused call to a das function whose body raises still raises") @(t : T?) {
        t |> equal(unused_user_call(), true)
    }
    t |> run("an unused user operator whose body raises still raises") @(t : T?) {
        t |> equal(unused_user_operator(), true)
    }
    t |> run("unused modulo by zero still raises") @(t : T?) {
        t |> equal(unused_mod(7, 0), true)
    }
    t |> run("an unused failing string-to-int conversion still raises") @(t : T?) {
        t |> equal(unused_string_to_int("abc"), true)
    }
    t |> run("an unused invoke of a block whose body raises still raises") @(t : T?) {
        t |> equal(unused_invoked_block(), true)
    }
    t |> run("an unused call to a das function that asserts still raises") @(t : T?) {
        t |> equal(unused_asserted_call(), true)
    }
    t |> run("an unused call to a das function that verifies still raises") @(t : T?) {
        t |> equal(unused_verified_call(), true)
    }
}

// mixed positional + a trailing default; used for the flagship bare
// form
def three(a : int; hint : string; extra : int = 7) : string => "{a}/{hint}/{extra}"

// four positional floats + a named hint -- the tool_slider(...) shape
def slider4(name : string; val, lo, hi : float; hint : string) : string =>
    "{name}:{hint}:{int(val)}:{int(lo)}:{int(hi)}"

// overload set disambiguated by the named argument's name + type
def over(a : int; kind : string) : string => "{a}:S:{kind}"
def over(a : int; count : int) : string => "{a}:I:{count}"

// generic first argument + a named argument
def gen_named(a; hint : string) : string => "{a}:{hint}"

// a named call INSIDE a generic body: the body is cloned on
// instantiation before the named call demotes - the clone must carry
// the positional in nonNamedArguments, else matching fails "can't
// skip hint"
def in_generic(g) : string => three(g, hint = "gen")

// mutable-ref param: lets clone (:=) be observed vs by-ref (= / <-).
// _tag is a leading positional so the array can be bound as a named
// arg.
def drain(_tag : int; var a : array<int>) { a |> clear() }

// piped block as the trailing positional, with a named option after
// it
def section(_tag : int; body : block<() : int>; open : bool = false) : int =>
    invoke(body) + (open ? 1000 : 0)

// method-style (dot) call target
struct Widget { id : int }
def render(self : Widget; title : string; depth : int = 0) : string => "W{self.id}:{title}:{depth}"

// a name that is BOTH a struct and a function (struct construction
// must win)
struct SameName { p : int }
def SameName(p : int; q : int) : string => "fn:{p}:{q}"

// a class method is a struct field, unreachable by bare name, so a
// call to it goes through the member-call path, which must keep a
// middle positional after self beside a named/defaulted arg
class Gadget {
    id : int
    def paint(title : string; depth : int = 0) : string => "G{id}:{title}:{depth}"
    // calls its own method with implicit self
    def repaint(title : string) : string => paint(title, depth = 5)
}

[test(description = "a bracket-less call mixes positional and named arguments, several named ones in parameter order, after four positionals")]
def test_bare_mixed(t : T?) {
    t |> equal(three(1, hint = "bare"), "1/bare/7")
    t |> equal(three(2, hint = "multi", extra = 9), "2/multi/9")
    t |> equal(slider4("radius", 5.0, 0.1, 50.0, hint = "scatter"), "radius:scatter:5:0:50")
}

[test(description = "the bracketed [name = value] named-argument form still binds")]
def test_bracketed_still_works(t : T?) {
    t |> equal(three(3, [hint = "brk"]), "3/brk/7")
    t |> equal(three(4, [hint = "brk", extra = 5]), "4/brk/5")
}

[test(description = "a named argument binds by reference with =, clones with := and moves with <-")]
def test_named_move_clone(t : T?) {
    var s1 <- [1, 2, 3]
    drain(0, a = s1)
    t |> equal(length(s1), 0)
    var s2 <- [1, 2, 3]
    drain(0, a := s2)
    t |> equal(length(s2), 3)
    var s3 <- [1, 2, 3]
    drain(0, a <- s3)
    t |> equal(length(s3), 0)
}

[test(description = "the named argument's name and type pick between overloads")]
def test_overload_by_named(t : T?) {
    t |> equal(over(1, kind = "hi"), "1:S:hi")
    t |> equal(over(1, count = 9), "1:I:9")
}

[test(description = "a generic function takes a named argument beside its inferred positional for each instantiation")]
def test_generic_named(t : T?) {
    t |> equal(gen_named(42, hint = "g"), "42:g")
    t |> equal(gen_named("hey", hint = "s"), "hey:s")
}

[test(description = "a piped block fills the block slot of a named call, with a named option before it or defaulted")]
def test_piped_block_named(t : T?) {
    let r = section(5, open = true) {
        return 42
    }
    t |> equal(r, 1042)
    let r2 = section(6) {
        return 8
    }
    t |> equal(r2, 8)
}

[test(description = "a dot method call on a free function takes a positional and a named argument")]
def test_method_bare(t : T?) {
    let w = Widget(id = 7)
    t |> equal(w.render("hdr", depth = 2), "W7:hdr:2")
}

[test(description = "the -> method-call form takes named arguments, bare or bracketed, like the dot form")]
def test_method_arrow(t : T?) {
    let w = Widget(id = 7)
    t |> equal(w->render("hdr", depth = 2), "W7:hdr:2")
    t |> equal(w->render([title = "brk", depth = 3]), "W7:brk:3")
}

[test(description = "a method call with only named arguments after self resolves through both . and ->")]
def test_method_all_named(t : T?) {
    let w = Widget(id = 7)
    t |> equal(w.render(title = "hi"), "W7:hi:0")
    t |> equal(w->render(title = "hi"), "W7:hi:0")
    t |> equal(w.render(title = "yo", depth = 3), "W7:yo:3")
    t |> equal(w->render(title = "yo", depth = 3), "W7:yo:3")
}

[test(description = "a call with only named arguments resolves as a function call and still picks the overload by name and type")]
def test_all_named(t : T?) {
    t |> equal(three(a = 1, hint = "an"), "1/an/7")
    t |> equal(three(a = 2, hint = "an", extra = 9), "2/an/9")
    t |> equal(over(a = 5, kind = "z"), "5:S:z")
}

[test(description = "an all-named call on a name that is both a struct and a function constructs the struct")]
def test_struct_beats_function(t : T?) {
    let s = SameName(p = 7)
    t |> equal(s.p, 7)
}

[test(description = "a genuine class method keeps a middle positional after self beside a named argument, including an implicit-self call")]
def test_class_method_bare(t : T?) {
    var g = new Gadget(id = 3)
    t |> equal(g.paint("hdr", depth = 2), "G3:hdr:2")
    t |> equal(g.repaint("hi"), "G3:hi:5")
    unsafe { delete g }
}

[test(description = "-> on a genuine class method keeps a middle positional beside a named argument")]
def test_class_method_arrow(t : T?) {
    var g = new Gadget(id = 3)
    t |> equal(g->paint("hdr", depth = 2), "G3:hdr:2")
    unsafe { delete g }
}

[test(description = "a genuine class method called with only named arguments resolves through both . and ->")]
def test_class_method_all_named(t : T?) {
    var g = new Gadget(id = 3)
    t |> equal(g.paint(title = "hi"), "G3:hi:0")
    t |> equal(g->paint(title = "hi"), "G3:hi:0")
    unsafe { delete g }
}

[test(description = "a named call inside a generic body keeps its positional argument when the body is cloned for instantiation")]
def test_named_call_in_generic(t : T?) {
    t |> equal(in_generic(1), "1/gen/7")
}

def add(a, b : int) : int => a + b

def public greet(name : string) : string => "hello, {name}"

def private secret(x : int) : int => x * 7

def make_arr() : array<int> => <- [1, 2, 3]

[generic]
def dbl(x : auto(T)) : auto(T) => x + x

def no_ret(x : int) => x + 1

def next_line(x : int) : int =>
    x * 3

// `tag` carries a field annotation right after an arrow-bodied
// method: the lexer must not read the '@' as a trailing-block pipe
class ArrowWidget {
    w : int
    h : int
    def area() : int => w * h
    @do_not_delete tag : int
    def perimeter() : int => 2 * (w + h)
    def scaled(k : int) : int => area() * k
    def static unit() : int => 1
}

struct Vec2 {
    x : int
    y : int
    def dot(o : Vec2) : int => x * o.x + y * o.y
    def cloned() : Vec2 => <- Vec2(x = x, y = y)
}

[test(description = "def name(args) : T => expr and => <- expr parse as function bodies with visibility prefixes, generics, no return type, a body on the next line, and on class and struct methods")]
def test_function_arrow_body(t : T?) {
    t |> run("basic value arrow") @(t : T?) {
        t |> equal(add(2, 3), 5)
        t |> equal(add(-1, 1), 0)
    }
    t |> run("public visibility prefix") @(t : T?) {
        t |> equal(greet("world"), "hello, world")
    }
    t |> run("private visibility prefix") @(t : T?) {
        t |> equal(secret(3), 21)
    }
    t |> run("move-arrow returning array") @(t : T?) {
        let xs <- make_arr()
        t |> equal(length(xs), 3)
        t |> equal(xs[0], 1)
        t |> equal(xs[2], 3)
    }
    t |> run("generic with [generic] annotation") @(t : T?) {
        t |> equal(dbl(5), 10)
        t |> equal(dbl(2.5), 5.0)
    }
    t |> run("no explicit return type") @(t : T?) {
        t |> equal(no_ret(10), 11)
    }
    t |> run("body on the line after '=>'") @(t : T?) {
        t |> equal(next_line(2), 6)
    }
    t |> run("class method arrow bodies") @(t : T?) {
        var wp = new ArrowWidget(w = 3, h = 4)
        t |> equal(wp->area(), 12)
        t |> equal(wp->perimeter(), 14)
        t |> equal(wp->scaled(2), 24)
        t |> equal(ArrowWidget`unit(), 1)
        unsafe { delete wp; }
    }
    t |> run("struct method arrow bodies") @(t : T?) {
        var a = Vec2(x = 1, y = 2)
        var b = Vec2(x = 3, y = 4)
        t |> equal(a.dot(b), 11)
        let c <- a.cloned()
        t |> equal(c.x, 1)
        t |> equal(c.y, 2)
    }
}

typedef FaFn = function<(a, b : int) : int>

def fa_add(a, b : int) : int => a + b

def fa_mul(a, b : int) : int => a * b

struct FaSlot {
    handle : uint64
}

def fa_handle_of(f : FaFn) : uint64 => intptr(f)

def fa_inline_handle : uint64 => unsafe(reinterpret<uint64>(@@fa_add))

[test(description = "a function pointer invokes, compares to itself and null, and its uint64 handle is the same non-null value in every store position and casts back to a callable, while invoking a null one panics")]
def test_func_addr(t : T?) {
    t |> run("function pointer invoke") @(t : T?) {
        let fn = @@fa_add
        let val = invoke(fn, 1, 2)
        t |> equal(val, 3)
    }
    t |> run("function pointer comparison") @(t : T?) {
        let fn = @@fa_add
        t |> equal(fn != null, true)
        t |> equal(fn == @@fa_add, true)
        t |> equal(fn == null, false)
        t |> equal(fn != @@fa_add, false)
    }
    t |> run("null function pointer") @(t : T?) {
        let qq : function
        t |> equal(qq == null, true)
    }
    t |> run("a function address is a non-null uint64 handle in every store position") @(t : T?) {
        // not its own baseline - the direct spelling is what this block tests
        let handle_via_param = fa_handle_of(@@fa_add)
        t |> success(handle_via_param != 0ul, "the handle through a parameter is not null")
        unsafe {
            var assigned : uint64
            assigned = reinterpret<uint64>(@@fa_add)   // nolint:STYLE011 - the split IS the case under test: a store into a live local, not an initializer
            let declared = reinterpret<uint64>(@@fa_add)
            var fixarr : uint64[2]
            fixarr[1] = reinterpret<uint64>(@@fa_add)
            var slot : FaSlot
            slot.handle = reinterpret<uint64>(@@fa_add)
            let literal = FaSlot(handle = reinterpret<uint64>(@@fa_add))
            let arith = 0ul + reinterpret<uint64>(@@fa_add)
            let as_ptr = reinterpret<void?>(@@fa_add)
            t |> equal(assigned, handle_via_param, "assignment to a local")
            t |> equal(declared, handle_via_param, "declaration with initializer")
            t |> equal(fixarr[1], handle_via_param, "fixed-array element store")
            t |> equal(slot.handle, handle_via_param, "struct field assignment")
            t |> equal(literal.handle, handle_via_param, "struct literal field")
            t |> equal(arith, handle_via_param, "arithmetic operand")
            t |> equal(intptr(as_ptr), handle_via_param, "cast to void?")
            t |> equal(reinterpret<int64>(@@fa_add), int64(handle_via_param), "cast to int64")
            t |> equal(invoke(reinterpret<FaFn>(as_ptr), 20, 22), 42,
                       "a void? handle casts back to a callable")
        }
        t |> equal(fa_inline_handle(), handle_via_param, "the direct spelling inside a return")
    }
    t |> run("distinct functions get distinct handles") @(t : T?) {
        t |> success(fa_handle_of(@@fa_add) != fa_handle_of(@@fa_mul),
            "two functions do not share a handle")
    }
    t |> run("a uint64 handle casts back to a callable") @(t : T?) {
        unsafe {
            let h = reinterpret<uint64>(@@fa_add)
            t |> equal(invoke(reinterpret<FaFn>(h), 20, 22), 42)
        }
    }
    t |> run("invoke null function panics") @(t : T?) {
        var failed = false
        try {
            let fn2 : function<(a, b : int) : int>
            invoke(fn2, 1, 2)
        } recover {
            failed = true
        }
        t |> equal(failed, true)
    }
}

// a [nodiscard] result consumed by string interpolation is USED: the
// string builder formats it
[nodiscard]
def answer : int => 42

[test(description = "a [nodiscard] result formatted by string interpolation counts as used, in a call argument and in a let initializer")]
def test_nodiscard_in_interpolation(t : T?) {
    to_log(LOG_INFO, "nodiscard in interpolation: {answer()}\n")
    let s = "value is {answer()}"
    t |> equal(s, "value is 42")
}

[test(description = "a [nodiscard] result bound to a let or used as an operand compiles and carries its value")]
def test_nodiscard_consumed_forms(t : T?) {
    let x = answer()
    t |> equal(x, 42)
    t |> equal(answer() + 0, 42)
}
