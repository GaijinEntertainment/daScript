options gen2
options no_unused_block_arguments = false
options no_unused_function_arguments = false

require dastest/testing_boost public
require daslib/option

// ===========================
// constructors + predicates
// ===========================

[test(description = "some carries its value, none is empty for its named type, is_some and is_none agree and different payload types work")]
def test_constructors(t : T?) {
    t |> run("some carries the value") @(t : T?) {
        let a = some(42)
        t |> equal(unwrap(a), 42)
        t |> success(is_some(a))
    }

    t |> run("none is empty for the named type") @(t : T?) {
        let b = none(type<int>)
        t |> success(is_none(b))
    }

    t |> run("is_some / is_none") @(t : T?) {
        t |> success(some(1) |> is_some)
        t |> success(!(none(type<int>) |> is_some))
        t |> success(none(type<string>) |> is_none)
        t |> success(!(some("x") |> is_none))
    }

    t |> run("some / none work for different payload types") @(t : T?) {
        t |> success(is_some(some(1.5f)))
        t |> success(is_some(some("hi")))
        t |> success(is_none(none(type<float>)))
        t |> success(is_none(none(type<string>)))
    }
}

// ===========================
// map / filter / and_then / or_else / or_value
// ===========================

[test(description = "map transforms a some, leaves none as none and can change the payload type")]
def test_map(t : T?) {
    t |> run("map transforms some") @(t : T?) {
        let m = some(3) |> map($(x : int) => x * 2)
        t |> equal(unwrap(m), 6)
        t |> success(is_some(m))
    }

    t |> run("map leaves none as none") @(t : T?) {
        let m = none(type<int>) |> map($(x : int) => x * 2)
        t |> success(is_none(m))
    }

    t |> run("map can change the payload type") @(t : T?) {
        let m = some(7) |> map($(x : int) => "{x}")
        t |> equal(unwrap(m), "7")
    }
}

[test(description = "filter keeps a matching some, drops a non-matching one and passes none through")]
def test_filter(t : T?) {
    t |> run("filter keeps matching some") @(t : T?) {
        let f = some(10) |> filter($(x : int) => x > 5)
        t |> success(is_some(f))
    }

    t |> run("filter drops non-matching some") @(t : T?) {
        let f = some(3) |> filter($(x : int) => x > 5)
        t |> success(is_none(f))
    }

    t |> run("filter passes none through") @(t : T?) {
        let f = none(type<int>) |> filter($(x : int) => true)
        t |> success(is_none(f))
    }
}

[test(description = "and_then chains some to some, short-circuits on none and can return none")]
def test_and_then(t : T?) {
    t |> run("and_then chains some → some") @(t : T?) {
        let r = some(4) |> and_then($(x : int) => some(x + 10))
        t |> equal(unwrap(r), 14)
    }

    t |> run("and_then on none short-circuits") @(t : T?) {
        let r = none(type<int>) |> and_then($(x : int) => some(x + 10))
        t |> success(is_none(r))
    }

    t |> run("and_then can return none") @(t : T?) {
        let r = some(5) |> and_then($(x : int) => none(type<int>))
        t |> success(is_none(r))
    }
}

[test(description = "or_else passes a some through and runs the fallback on none")]
def test_or_else(t : T?) {
    t |> run("or_else passes some through") @(t : T?) {
        let r = some(1) |> or_else() { return some(99); }
        t |> equal(unwrap(r), 1)
    }

    t |> run("or_else invokes fallback on none") @(t : T?) {
        let r = none(type<int>) |> or_else() { return some(99); }
        t |> equal(unwrap(r), 99)
    }
}

[test(description = "or_value passes a some through and substitutes a value on none")]
def test_or_value(t : T?) {
    t |> run("or_value passes some through") @(t : T?) {
        let r = some(1) |> or_value(99)
        t |> equal(unwrap(r), 1)
    }

    t |> run("or_value substitutes on none") @(t : T?) {
        let r = none(type<int>) |> or_value(99)
        t |> equal(unwrap(r), 99)
    }
}

// ===========================
// unwrap family
// ===========================

[test(description = "unwrap, unwrap_or, unwrap_or_else, unwrap_or_default and move_unwrap return the payload or the fallback as specified")]
def test_unwrap(t : T?) {
    t |> run("unwrap on some returns value") @(t : T?) {
        t |> equal(some(42) |> unwrap, 42)
    }

    t |> run("unwrap_or returns default when none") @(t : T?) {
        t |> equal(none(type<int>) |> unwrap_or(17), 17)
        t |> equal(some(5) |> unwrap_or(17), 5)
    }

    t |> run("unwrap_or_else calls block when none") @(t : T?) {
        t |> equal(none(type<int>) |> unwrap_or_else() { return 11; }, 11)
        t |> equal(some(5) |> unwrap_or_else() { return 11; }, 5)
    }

    t |> run("unwrap_or_default uses zero value") @(t : T?) {
        t |> equal(none(type<int>) |> unwrap_or_default, 0)
        t |> equal(none(type<string>) |> unwrap_or_default, "")
        t |> equal(some(5) |> unwrap_or_default, 5)
    }

    t |> run("move_unwrap on copyable T returns value, leaves source none") @(t : T?) {
        var o = some(42)
        t |> equal(o |> move_unwrap, 42)
        t |> success(is_none(o))
    }
}

// ===========================
// operators
// ===========================

[test(description = "the ?? operator unwraps or defaults and == and != compare options with options and with payload values")]
def test_operators(t : T?) {
    t |> run("?? unwraps some") @(t : T?) {
        t |> equal(some(7) ?? 0, 7)
    }

    t |> run("?? returns default on none") @(t : T?) {
        t |> equal(none(type<int>) ?? 99, 99)
    }

    t |> run("== compares structurally") @(t : T?) {
        t |> success(some(5) == some(5))
        t |> success(!(some(5) == some(6)))
        t |> success(none(type<int>) == none(type<int>))
        t |> success(!(some(5) == none(type<int>)))
    }

    t |> run("== compares option with payload value") @(t : T?) {
        t |> success(some(5) == 5)
        t |> success(5 == some(5))
        t |> success(!(some(5) == 6))
        t |> success(!(6 == some(5)))
        t |> success(!(none(type<int>) == 0))
        t |> success(!(0 == none(type<int>)))
    }

    t |> run("!= compares option with payload value") @(t : T?) {
        t |> success(some(5) != 6)
        t |> success(6 != some(5))
        t |> success(!(some(5) != 5))
        t |> success(!(5 != some(5)))
        t |> success(none(type<int>) != 0)
        t |> success(0 != none(type<int>))
    }
}

// ===========================
// side-effect combinators
// ===========================

[test(description = "if_some and if_none run their block only on the matching state")]
def test_if_some_if_none(t : T?) {
    t |> run("if_some fires on some") @(t : T?) {
        var fired = 0
        some(3) |> if_some() $(x : int) { fired = x; }
        t |> equal(fired, 3)
    }

    t |> run("if_some is a no-op on none") @(t : T?) {
        var fired = 0
        none(type<int>) |> if_some() $(x : int) { fired = x; }
        t |> equal(fired, 0)
    }

    t |> run("if_none fires on none") @(t : T?) {
        var fired = 0
        none(type<int>) |> if_none() { fired = 42; }
        t |> equal(fired, 42)
    }

    t |> run("if_none is a no-op on some") @(t : T?) {
        var fired = 0
        some(1) |> if_none() { fired = 42; }
        t |> equal(fired, 0)
    }
}

// ===========================
// zip
// ===========================

[test(description = "zip of two somes is a some pair and either none makes the result none")]
def test_zip(t : T?) {
    t |> run("zip some + some -> some") @(t : T?) {
        let z = zip(some(1), some(2.5f))
        t |> success(is_some(z))
        t |> equal(unwrap(z)._0, 1)
        t |> equal(unwrap(z)._1, 2.5f)
    }

    t |> run("zip with either none -> none") @(t : T?) {
        t |> success(is_none(zip(none(type<int>), some(1.5f))))
        t |> success(is_none(zip(some(1), none(type<float>))))
        t |> success(is_none(zip(none(type<int>), none(type<float>))))
    }
}

// ===========================
// chaining smoke test — the whole point of the library
// ===========================

[test(description = "a some flows through map, filter and unwrap_or while a none short-circuits the whole chain")]
def test_chaining(t : T?) {
    t |> run("some → map → filter → unwrap_or") @(t : T?) {
        let result = (some(3)
            |> map($(x : int) => x * 2)
            |> filter($(x : int) => x > 0)
            |> unwrap_or(-1))
        t |> equal(result, 6)
    }

    t |> run("none short-circuits the whole chain") @(t : T?) {
        let result = (none(type<int>)
            |> map($(x : int) => x * 2)
            |> filter($(x : int) => x > 0)
            |> unwrap_or(-1))
        t |> equal(result, -1)
    }
}

// Regression for typemacro+tuple Option carrying a payload that's
// "unsafe-when-uninitialized" — i.e. a struct with hasInitFields = true.
// Without TypeDeclFlags.safeWhenUninitialized on the value field,
// default<Option<Brittle>> would trip ast_infer_type.cpp:5032's
// "Uninitialized variable" check.

struct Brittle {
    must_init : int = 42
    name : string = "default"
}

[test(description = "an Option of a struct with must-init fields compiles as none, as a bare var and as a some that round-trips")]
def option_brittle_constructors(t : T?) {
    t |> run("default<Option<Brittle>> compiles and reads as none") @(t : T?) {
        let o = none(type<Brittle>)
        t |> success(o |> is_none)
        t |> success(!(o |> is_some))
    }
    t |> run("bare var Option<Brittle> compiles via @safe_when_uninitialized on _value") @(t : T?) {
        // Without @safe_when_uninitialized on the _value field of the tuple
        // produced by [template_tuple(T)] in daslib/option.das, this would
        // trigger "Uninitialized variable is unsafe" at ast_infer_type.cpp:5040.
        // The bare var (no init) is the only form that exercises this check —
        // ``let`` requires an initializer.
        var bare : Option<Brittle> // nolint:LINT003
        t |> success(bare |> is_none)
    }
    t |> run("some(Brittle(...)) round-trips") @(t : T?) {
        let o = some(Brittle(must_init = 7, name = "x"))
        t |> success(o |> is_some)
        let v = o |> unwrap
        t |> equal(v.must_init, 7)
        t |> equal(v.name, "x")
    }
}

[test(description = "unwrap_or_default of a none Option of a must-init struct returns the struct's defaults")]
def option_brittle_unwrap_or_default(t : T?) {
    t |> run("unwrap_or_default returns default<Brittle> on none") @(t : T?) {
        let o = none(type<Brittle>)
        let v = o |> unwrap_or_default
        t |> equal(v.must_init, 42)
        t |> equal(v.name, "default")
    }
}

let OPTION_PICK_DIM = 2

def option_pick_own_alias(_o : $Option<auto(OptionPickAlias)>; _v : OptionPickAlias) : string =>
    typeinfo typename(type<OptionPickAlias>)

def option_pick_dim(_o : tuple<int[OPTION_PICK_DIM]; $Option<auto(TT)>>; _v : TT) : int => 1

def option_pick_dim(_o : auto; _v : auto) : int => 2

[test(description = "a generic whose type variable is bound inside an Option argument keeps that variable when a module typedef has the same name, and a named dimension beside an Option in one argument still matches")]
def option_generic_argument_fold(t : T?) {
    t |> equal(option_pick_own_alias(some(1.0), 2.0), "float const")
    let row : tuple<int[2]; $Option<float>>
    t |> equal(option_pick_dim(row, 2.0), 1)
}

typedef OptionPickAlias = int
