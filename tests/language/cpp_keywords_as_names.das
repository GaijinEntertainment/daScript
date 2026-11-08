options gen2
require dastest/testing_boost public

// Coverage: daslang accepts C++ keywords as variable/struct/field/enum/class names.
// Only C++ keywords that are NOT also daslang reserved words can be tested here
// (the daslang parser rejects e.g. `for`, `if`, `class`, `new`, `delete`, ...).
// Safe choices: char, register, short, long, extern, friend, mutable, volatile,
// signed, unsigned, union, inline, virtual, namespace, do, catch, throw, ...

// --- struct fields named after C++ keywords ---
struct StFields {
    char : int
    register : float
    extern : bool
    mutable : string
}

// --- struct named after a C++ keyword ---
struct register {
    value : int
}

// --- class named after a C++ keyword ---
class extern {
    payload : int = 99
    def get_payload() : int => payload
}

// --- enum named after a C++ keyword ---
enum signed {
    plus
    minus
}

// --- enum values named after C++ keywords ---
enum Width {
    short
    long
    union
}

// --- type alias named after a C++ keyword ---
typedef short = int
typedef extern_ptr = int?

// --- function arg named after a C++ keyword ---
def double_it(char : int) : int => char * 2

// --- function type with a keyword arg name ---
def call_with_int(f : function<(char : int) : int>; v : int) : int => invoke(f, v)

// --- global var named after a C++ keyword ---
var g_char = 100
var register_g = 7

[test(description = "for-loop iterator variables named char and register iterate and accumulate like any other name")]
def test_for_loop_iter(t : T?) {
    t |> run("for-loop iter named char (user's bug repro)") @(t : T?) {
        var sum = 0
        for (char in [1, 2, 3]) {
            sum += char
        }
        t |> equal(sum, 6)
    }
    t |> run("for-loop iter named register") @(t : T?) {
        var product = 1
        for (register in [2, 3, 4]) {
            product *= register
        }
        t |> equal(product, 24)
    }
}

[test(description = "let and var locals named char, short, long and volatile hold and update their values")]
def test_let_var(t : T?) {
    t |> run("let var named char") @(t : T?) {
        let char = 42
        t |> equal(char, 42)
    }
    t |> run("let var named short with arithmetic") @(t : T?) {
        let short = 99
        let long = short * 2
        t |> equal(long, 198)
    }
    t |> run("var (mutable) named volatile") @(t : T?) {
        var volatile = 0
        volatile += 5
        volatile += 7
        t |> equal(volatile, 12)
    }
}

[test(description = "globals whose names start with the C++ keywords char and register keep their initial values")]
def test_global_var(t : T?) {
    t |> run("global var named g_char") @(t : T?) {
        t |> equal(g_char, 100)
    }
    t |> run("global var named register_g") @(t : T?) {
        t |> equal(register_g, 7)
    }
}

[test(description = "struct fields named char, register, extern and mutable initialize and read back by name")]
def test_struct_fields(t : T?) {
    t |> run("struct fields named after C++ keywords") @(t : T?) {
        let s = StFields(char = 5, register = 3.14, extern = true, mutable = "ok")
        t |> equal(s.char, 5)
        t |> equal(int(s.register * 100.0), 314)
        t |> equal(s.extern, true)
        t |> equal(s.mutable, "ok")
    }
}

[test(description = "a struct named register constructs and reads its field")]
def test_struct_name(t : T?) {
    t |> run("struct name 'register'") @(t : T?) {
        let r = register(value = 17)
        t |> equal(r.value, 17)
    }
}

[test(description = "a class named extern constructs with new and dispatches its method")]
def test_class_name(t : T?) {
    t |> run("class name 'extern'") @(t : T?) {
        var c = new extern()
        t |> equal(c->get_payload(), 99)
        unsafe { delete c; }
    }
}

[test(description = "an enum named signed declares values that compare by identity")]
def test_enum_name(t : T?) {
    t |> run("enum name 'signed'") @(t : T?) {
        let s = signed.plus
        t |> equal(s == signed.plus, true)
        t |> equal(s == signed.minus, false)
    }
}

[test(description = "enum values named short, long and union are distinct values of their enum")]
def test_enum_values(t : T?) {
    t |> run("enum values short/long/union") @(t : T?) {
        let w = Width.short
        t |> equal(w == Width.short, true)
        t |> equal(w == Width.long, false)
        t |> equal(w == Width.union, false)
    }
}

[test(description = "a type alias named short declares an int local")]
def test_typedef_alias(t : T?) {
    t |> run("typedef alias named short") @(t : T?) {
        let x : short = 5
        t |> equal(x, 5)
    }
}

[test(description = "a function argument named char works in a direct call and through a function type whose argument is named char")]
def test_function_arg(t : T?) {
    t |> run("function arg 'char'") @(t : T?) {
        t |> equal(double_it(5), 10)
    }
    t |> run("function type with keyword arg name") @(t : T?) {
        t |> equal(call_with_int(@@double_it, 7), 14)
    }
}

[test(description = "lambda arguments named char and register work, with and without a capture")]
def test_lambda_arg(t : T?) {
    t |> run("lambda arg 'char'") @(t : T?) {
        var f <- @(char : int) => char * 3
        t |> equal(invoke(f, 4), 12)
        unsafe { delete f; }
    }
    t |> run("lambda arg 'register' with capture") @(t : T?) {
        let bias = 100
        var g <- @(register : int) => register + bias
        t |> equal(invoke(g, 5), 105)
        unsafe { delete g; }
    }
}
