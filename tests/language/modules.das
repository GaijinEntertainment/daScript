options gen2
options no_unused_function_arguments = false

require UnitTest
require dastest/testing_boost public
require math as m                           // builtin C++ module
require daslib/strings_boost as sb          // daslib mount
require _require_as_fixture as fx           // same-directory bare require
require ./_require_as_fixture.das as fxp    // path form: second alias, same module
require _with_module_helper
require _with_module_helper2
require _finalize_collision_helper public
require _lambda_vis_mid
require _private_property_helper
require _struct_alias_helper
require _struct_alias_twin // nolint:STYLE030 - its SharedBox is the twin name
require _struct_alias_facade // nolint:STYLE030 - its typedefs are the use

// `require X as Y` registers the alias for every require form -
// builtin, daslib, same-directory, path
[test(description = "require ... as registers its alias for a builtin C++ module")]
def test_require_as_builtin(t : T?) {
    t |> success(m::sqrt(9.0) == 3.0)
}

[test(description = "require ... as registers its alias for a daslib module")]
def test_require_as_daslib(t : T?) {
    t |> equal(sb::join([1, 2, 3], ","), "1,2,3")
}

[test(description = "a same-directory bare require and its path spelling both register aliases, and the real module name keeps working beside them")]
def test_require_as_same_dir(t : T?) {
    t |> equal(fx::require_as_marker(), 45)
    t |> equal(_require_as_fixture::require_as_marker(), 45)
    t |> equal(fxp::require_as_marker(), 45)
}

[test(description = "with (module alias) resolves through a require alias while locals stay lexical inside it")]
def test_require_as_with_module(t : T?) {
    var joined : string
    with (module sb) {
        joined = join([4, 5], "-")
    }
    t |> equal(joined, "4-5")
}

// `with (module foo)` resolves names as if written in foo, privates
// included; `_::` resolves at the enclosing module
def local_probe() : int => 1000

[test(description = "inside with (module foo) the block reads and writes foo's private functions, globals, structs, enums and type aliases")]
def test_with_module_private_symbols(t : T?) {
    t |> run("private function") @(t : T?) {
        var r = 0
        with (module _with_module_helper) {
            r = secret_add(2, 3)
        }
        t |> equal(r, 5)
    }
    t |> run("private global read and write") @(t : T?) {
        var scale = 0
        var counter = 0
        with (module _with_module_helper) {
            scale = secret_scale
            secret_counter = 11
            counter = secret_counter
        }
        t |> equal(scale, 3)
        t |> equal(counter, 11)
    }
    t |> run("private struct and enum") @(t : T?) {
        var v = 0
        var e = 0
        with (module _with_module_helper) {
            let box = SecretBox(value = 42)
            v = box.value
            e = int(SecretEnum.Beta)
        }
        t |> equal(v, 42)
        t |> equal(e, 1)
    }
    t |> run("private type alias") @(t : T?) {
        var r = 0
        with (module _with_module_helper) {
            let v : SecretAlias = 21
            r = v * 2
        }
        t |> equal(r, 42)
    }
}

[test(description = "an unqualified name inside with (module foo) resolves in foo while _:: reaches the enclosing module, the innermost nested with wins, a local shadows foo's global, and a generic instantiates inside the block")]
def test_with_module_scoping(t : T?) {
    t |> run("unqualified name resolves in the with-module, _:: at the enclosing module") @(t : T?) {
        var a = 0
        var b = 0
        with (module _with_module_helper) {
            a = local_probe()
            b = _::local_probe()
        }
        t |> equal(a, 7)
        t |> equal(b, 1000)
    }
    t |> run("nested with-module - innermost wins") @(t : T?) {
        var a = 0
        var b = 0
        var c = 0
        with (module _with_module_helper) {
            a = who_am_i()
            with (module _with_module_helper2) {
                b = who_am_i()
            }
            c = who_am_i()
        }
        t |> equal(a, 1)
        t |> equal(b, 2)
        t |> equal(c, 1)
    }
    t |> run("local variable shadows the module's global") @(t : T?) {
        let secret_scale = 55
        var r = 0
        with (module _with_module_helper) {
            r = secret_scale
        }
        t |> equal(r, 55)
    }
    t |> run("generic instantiated inside the block") @(t : T?) {
        var r = 0
        with (module _with_module_helper) {
            r = secret_generic(21)
        }
        t |> equal(r, 42)
    }
}

// the twin class arrives through a PUBLIC require: the generated
// __finalize must pin to this module's Doppel with the strict `__::`
// filter, since module-name lookup also walks public dependencies
class Doppel {
    y : int = 4
}

class DoppelChild : _finalize_collision_helper::Doppel {
    z : int = 5
}

[test(description = "a class named like a class in a publicly required module compiles, and a class derived from that twin gets its own finalizer")]
def test_class_name_collision_public(t : T?) {
    t |> run("same-named class across a public require compiles") @(t : T?) {
        var c = new DoppelChild()
        t |> equal(1, c.x)
        t |> equal(5, c.z)
        unsafe {
            delete c
        }
    }
}

// this file never requires _lambda_vis_inner: inner_dot is reachable
// only through _lambda_vis_mid's own require
[test(description = "a direct call, a lambda, a generator and a local function inside a generic instance all resolve names from the generic's home module")]
def test_lambda_in_generic_module_vis(t : T?) {
    t |> run("direct call in generic resolves from home module") @(t : T?) {
        t |> equal(9.0, vis_direct(4))
    }
    t |> run("lambda in generic resolves from home module") @(t : T?) {
        t |> equal(13.0, vis_lambda(4))
    }
    t |> run("generator in generic resolves from home module") @(t : T?) {
        t |> equal(17.0, vis_generator(4))
    }
    t |> run("local function in generic resolves from home module") @(t : T?) {
        t |> equal(21.0, vis_local_function(4))
    }
}

[test(description = "a class's private property setter and getter stay callable inside the declaring module, behind public helpers and methods")]
def test_private_property_in_module(t : T?) {
    var g = new Gate()
    t |> equal(false, g.flag)
    set_flag_from_helper(g, true)
    t |> equal(true, g.flag)
    t |> equal(7, g->read_hidden())
    t |> equal(true, read_hidden_flag(make_hidden()))
}

class PrivLambdaA {
    private onLog : lambda
    private y : int
    def PrivLambdaA {
        pass
    }
    def install {
        onLog <- @ {
            y--
        }
    }
    def bang {
        if (onLog != null) {
            onLog |> invoke()
        }
    }
    def operator delete {
        y++
    }
}

[test(description = "a lambda created inside a class method reads and writes the class's private fields")]
def test_access_private_from_lambda(t : T?) {
    t |> run("class lambda accesses private field") @(t : T?) {
        var a = new PrivLambdaA()
        a->install()
        a->bang()
        t |> success(true)
    }
}

[test(description = "a struct and a typedef of it visible from two modules construct through the bare name, the struct's module and the alias's module, with and without fields")]
def test_struct_alias_from_two_modules(t : T?) {
    let bare = AliasedBox()
    let fromStruct = _struct_alias_helper::AliasedBox()
    let fromAlias = _struct_alias_facade::AliasedBox()
    let bareFields = AliasedBox(x = 3)
    let aliasFields = _struct_alias_facade::AliasedBox(x = 4)
    t |> equal(bare.x, 0)
    t |> equal(fromStruct.x, 0)
    t |> equal(fromAlias.x, 0)
    t |> equal(bareFields.x, 3)
    t |> equal(aliasFields.x, 4)
}

[test(description = "a typedef naming a visible enum, a visible handled type, or the same type as another module's typedef is that one candidate, not a second")]
def test_alias_of_enum_handle_and_twin_alias(t : T?) {
    let hue = AliasedHue.green
    t |> equal(int(hue), 1)
    var mat : float4x4
    mat[0] = float4(1.0, 2.0, 3.0, 4.0)
    t |> equal(mat[0].y, 2.0)
    let n : AliasedNum = 7
    t |> equal(n, 7)
}

[test(description = "a typedef of a struct that shares its name with another visible struct constructs the aliased one, with and without arguments")]
def test_alias_of_struct_with_a_twin_name(t : T?) {
    let zero = SharedAl()
    t |> equal(zero.v, 0)
    let seven = SharedAl(7)
    t |> equal(seven.v, 7)
}
