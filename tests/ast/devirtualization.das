options gen2
options no_aot
options rtti

require dastest/testing_boost public
require daslib/ast
require daslib/ast_boost
require daslib/ast_print
require strings
require daslib/rtti
require _devirtualization_module

//! A call to a method a sealed class declares compiles to a direct
//! call of that method, and so does a call to a method of a private
//! class that nothing in its module overrides or writes. A
//! `def sealed` override, or a sealed class's override, stays a
//! method invoke: the base class's field can still be assigned, and
//! that write reaches the sealed object.

class sealed Counter {
    n : int
    def add(k : int) : int {
        n += k
        return twice()
    }
    def twice : int => n * 2
}

class Base {
    def name : string => "base"
}

class Mid : Base {
    def sealed name : string => "mid"
}

class Leaf : Mid {
    extra : int
}

class sealed SealedLeaf : Base {
    def override name : string => "sealed leaf"
}

[unused_argument(self)]
def patched(var self : Base) : string => "patched"

class private Walker {
    steps : int
    def step(n : int) : void {
        steps += n
    }
    def walk(n : int) : int {
        for (i in range(n)) {
            step(i)
        }
        return steps
    }
}

class private Shape {
    def kind : string => "shape"
}

class private Square : Shape {
    def override kind : string => "square"
}

class private Rewired {
    def tag : string => "rewired"
}

[unused_argument(self)]
def retagged(var self : Rewired) : string => "retagged"

def rewire(var r : Rewired?) {
    r.tag = @@retagged
}

class private Touched {
    def tag : string => "touched"
}

def touch(var f : auto&) {
}

class private UnderPublic : Base {
    def override name : string => "under public"
}

class private PrivateRoot {
    def tag : string => "private root"
}

class PublicChild : PrivateRoot {
    extra : int
}

def call_walker(var w : Walker?) : int => w->walk(4)

def call_shape(var s : Shape?) : string => s->kind()

def call_square(var s : Square?) : string => s->kind()

def call_rewired(var r : Rewired?) : string => r->tag()

def call_touched(var t : Touched?) : string {
    touch(t.tag)
    return t->tag()
}

def call_under_public(var u : UnderPublic?) : string => u->name()

def call_private_root(var r : PrivateRoot?) : string => r->tag()

def call_sealed_class(var c : Counter?) : int => c->add(5)

def call_sealed_method(var m : Mid?) : string => m->name()

def call_inherited_sealed_method(var l : Leaf?) : string => l->name()

def call_sealed_class_override(var l : SealedLeaf?) : string => l->name()

def call_virtual(var b : Base?) : string => b->name()

def call_module_sealed(var m : ModuleSealed?) : int => m->read()

def call_module_plain(var m : ModulePlain?) : int => m->read()

def private printed_body(var program : ProgramPtr; name : string) : string {
    var fn = find_unique_function(get_this_module(program), name)
    return "" if (fn == null)
    return build_string() $(var writer) {
        printExpr(fn.body, unsafe(addr(writer)))
    }
}

def private is_direct(body, callee : string) : bool =>
    find(body, "{callee}(") >= 0 && find(body, "invoke") < 0

def private compile_self(t : T?; auto_inline : bool;
                         blk : block<(var program : ProgramPtr) : void>) {
    let self_path = "{get_das_root()}/tests/ast/devirtualization.das"
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.disable_auto_inline = !auto_inline
            compile_file(self_path, access, unsafe(addr(mg)), cop) $(ok; var program; issues) {
                t |> success(ok, "the test did not compile itself: {issues}")
                return if (!ok || program == null)
                invoke(blk, program)
            }
        }
    }
}

[test(description = "a call to a method of a sealed class or a sealed method returns what the method returns, and a virtual call through the base still reaches the override")]
def test_sealed_calls_return_the_right_values(t : T?) {
    var c = new Counter()
    t |> equal(call_sealed_class(c), 10)
    t |> equal(c.n, 5)
    var leaf = new Leaf()
    t |> equal(call_sealed_method(leaf), "mid")
    t |> equal(call_inherited_sealed_method(leaf), "mid")
    t |> equal(call_virtual(leaf), "mid")
    t |> equal(call_virtual(new Base()), "base")
}

[test(description = "a call to a method of a private class returns what the method returns, whether it compiles to a direct call or stays an invoke")]
def test_private_calls_return_the_right_values(t : T?) {
    t |> equal(call_walker(new Walker()), 6)
    t |> equal(call_shape(new Square()), "square")
    t |> equal(call_square(new Square()), "square")
    var rewired = new Rewired()
    rewire(rewired)
    t |> equal(call_rewired(rewired), "retagged")
    t |> equal(call_touched(new Touched()), "touched")
    t |> equal(call_under_public(new UnderPublic()), "under public")
    t |> equal(call_private_root(new PublicChild()), "private root")
}

[test(description = "assigning a method field through the base class reaches a def sealed override and a sealed class override, so neither call may be devirtualized")]
def test_a_write_through_the_base_reaches_sealed_objects(t : T?) {
    var mid = new Mid()
    var sealed_leaf = new SealedLeaf()
    var through_mid : Base? = mid
    var through_leaf : Base? = sealed_leaf
    through_mid.name = @@patched
    through_leaf.name = @@patched
    t |> equal(call_sealed_method(mid), "patched")
    t |> equal(call_sealed_class_override(sealed_leaf), "patched")
}

[test(description = "with auto-inline off, a method call compiles to a direct call only when the method is sealed up to its family root or belongs to a closed private family nothing overrides or writes")]
def test_calls_compile_to_direct_calls_only_when_fixed(t : T?) {
    compile_self(t, false) $(var program) {
        let sealed_class = printed_body(program, "call_sealed_class")
        t |> success(is_direct(sealed_class, "Counter`add"), sealed_class)
        let inside = printed_body(program, "Counter`add")
        t |> success(is_direct(inside, "Counter`twice"), inside)
        let walker = printed_body(program, "Walker`walk")
        t |> success(is_direct(walker, "Walker`step"), walker)
        let square = printed_body(program, "call_square")
        t |> success(is_direct(square, "Square`kind"), square)
        let invokes = ["call_sealed_method", "call_inherited_sealed_method",
                       "call_sealed_class_override", "call_virtual", "call_shape",
                       "call_rewired", "call_touched", "call_under_public",
                       "call_private_root"]
        for (name in invokes) {
            let body = printed_body(program, name)
            t |> success(find(body, "invoke") >= 0, "{name}: {body}")
        }
    }
}

[test(description = "with auto-inline on, a directly called sealed method is spliced into its caller while a virtual call stays an invoke")]
def test_sealed_calls_inline(t : T?) {
    compile_self(t, true) $(var program) {
        let sealed_class = printed_body(program, "call_sealed_class")
        let spliced = (find(sealed_class, "Counter`add(") < 0
                       && find(sealed_class, "invoke") < 0)
        t |> success(spliced, sealed_class)
        let virtual_call = printed_body(program, "call_virtual")
        t |> success(find(virtual_call, "invoke") >= 0, virtual_call)
    }
}

[test(description = "a call to a method of a sealed class another module declares returns its value and compiles to a direct call in the calling module, while a plain class from that module stays an invoke")]
def test_sealed_class_from_another_module(t : T?) {
    t |> equal(call_module_sealed(new ModuleSealed()), 6)
    t |> equal(call_module_plain(new ModulePlain()), 8)
    compile_self(t, false) $(var program) {
        let sealed_call = printed_body(program, "call_module_sealed")
        t |> success(is_direct(sealed_call, "ModuleSealed`read"), sealed_call)
        let plain_call = printed_body(program, "call_module_plain")
        t |> success(find(plain_call, "invoke") >= 0, plain_call)
    }
}

[unused_argument(self)]
def patched_read(var self : Base) : int => 100

def private patch_method(var obj; name : string; fn) {
    let info = class_info(*obj)
    for (i in range(int(info.count))) {
        let field = unsafe(info.fields[i])
        if (field.name == name) {
            unsafe {
                memcpy(reinterpret<uint8?>(obj) + int(field.offset), addr(fn), typeinfo sizeof(fn))
            }
        }
    }
}

def whole_program_private_root(var r : PrivateRoot?) : string => r->tag()

def whole_program_module_plain(var m : ModulePlain?) : int => m->read()

def whole_program_virtual(var b : Base?) : string => b->name()

[test(description = "the interpreter and the JIT run a call to a method nothing in the program overrides or writes as a direct call, so a raw write to the method slot - which no analysis sees - does not reach it, while a call to an overridden method still does")]
def test_whole_program_calls_are_direct_calls(t : T?) {
    var root = new PrivateRoot()
    patch_method(root, "tag", @@patched)
    t |> equal(whole_program_private_root(root), "private root")
    var module_plain = new ModulePlain()
    patch_method(module_plain, "read", @@patched_read)
    t |> equal(whole_program_module_plain(module_plain), 8)
    var base = new Base()
    patch_method(base, "name", @@patched)
    t |> equal(whole_program_virtual(base), "patched")
}
