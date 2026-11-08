options gen2
options no_unused_function_arguments = false
require UnitTest
require math
require dastest/testing_boost public
require daslib/rtti
require daslib/class_boost
require daslib/strings_boost
// the lowered `delete cast<Base>(self)` must carry source locations -
// a verify finding here is a compile error
require daslib/ast_verify
require _finalize_collision_helper

var class2_del_base = 0
var class2_del_derived = 0

class ClassBase {
    def operator delete {
        class2_del_base ++
    }
}

class ClassDerived : ClassBase {
    def operator delete {
        delete super.self
        class2_del_derived ++
    }
}

var class3_del_a = 0
var class3_del_b = 0
var class3_del_c = 0

class ClassA {
    def operator delete {
        class3_del_a ++
    }
}

class ClassB : ClassA {
    def operator delete {
        delete super.self
        class3_del_b ++
    }
}

class ClassC : ClassB {
    def operator delete {
        delete super.self
        class3_del_c ++
    }
}

var struct_del_base = 0
var struct_del_derived = 0

struct StructBase {
    a : int
}

[unused_argument(self)]
def operator delete(var self : StructBase) {
    struct_del_base ++
}

struct StructDerived : StructBase {
    b : int
}

def operator delete(var self : StructDerived) {
    delete super.self
    struct_del_derived ++
}

// Skip-level inheritance: empty SkipMid in the middle of the chain.
// `delete super.self` in SkipLeaf must walk past SkipMid and call
// SkipBase's finalizer.
var skip_class_base = 0
var skip_class_leaf = 0

class SkipBase {
    def operator delete {
        skip_class_base ++
    }
}

class SkipMid : SkipBase {
}

class SkipLeaf : SkipMid {
    def operator delete {
        delete super.self
        skip_class_leaf ++
    }
}

// 4-level skip with two empty intermediates.
var deep_class_base = 0
var deep_class_leaf = 0

class DeepBase {
    def operator delete {
        deep_class_base ++
    }
}

class DeepMid1 : DeepBase {
}

class DeepMid2 : DeepMid1 {
}

class DeepLeaf : DeepMid2 {
    def operator delete {
        delete super.self
        deep_class_leaf ++
    }
}

// Mid-chain with finalizer: only DeepLeaf and one ancestor have
// finalizers, the immediate parent does not. Ensures walk-up still
// chains transitively.
var mid_class_base = 0
var mid_class_b = 0
var mid_class_leaf = 0

class MidBase {
    def operator delete {
        mid_class_base ++
    }
}

class MidB : MidBase {
    def operator delete {
        delete super.self
        mid_class_b ++
    }
}

class MidEmpty : MidB {
}

class MidLeaf : MidEmpty {
    def operator delete {
        delete super.self
        mid_class_leaf ++
    }
}

// Finalizer inheritance: a derived class with NO finalizer of its own
// chains to the ancestor's automatically — the generated finalizer
// cleans the derived slice and runs `delete super.self` on the
// immediate parent, recursing past empty levels.
var inh_base = 0

class InhBase {
    def operator delete {
        inh_base ++
    }
}

class InhDerived : InhBase {
}

class InhMid : InhBase {
}

class InhLeaf : InhMid {
}

// Slice cleanup + ordering: TrB has no finalizer, but its generated
// chain finalizer still finalizes TrB's own fields (Tracer observes
// it), in dtor order — derived body first, then each slice on the way
// up, base finalizer last.
var fin_order = ""

struct Tracer {
    id : int
}

def operator delete(var self : Tracer) {
    fin_order += "tracer{self.id};"
}

class TrA {
    def operator delete {
        fin_order += "A;"
    }
}

class TrB : TrA {
    tr_b : Tracer = Tracer(id = 2)
}

class TrC : TrB {
    tr_c : Tracer = Tracer(id = 3)
    def operator delete {
        fin_order += "C;"
        delete tr_c
        delete super.self
    }
}

// Struct skip-level: SkipStructMid has no finalizer (struct case
// relies on substitution at the immediate parent — we still verify it
// works).
var skip_struct_base = 0
var skip_struct_leaf = 0

struct SkipStructBase {
    a : int
}

[unused_argument(self)]
def operator delete(var self : SkipStructBase) {
    skip_struct_base ++
}

struct SkipStructMid : SkipStructBase {
    m : int
}

struct SkipStructLeaf : SkipStructMid {
    z : int
}

def operator delete(var self : SkipStructLeaf) {
    delete super.self
    skip_struct_leaf ++
}

[test(description = "delete super.self in a class or struct finalizer runs every ancestor finalizer exactly once, across three-level chains and past empty intermediate classes and structs")]
def test_delete_super_self(t : T?) {
    t |> run("class derived operator delete chains to base via super.self") @(t : T?) {
        class2_del_base = 0
        class2_del_derived = 0
        var d = new ClassDerived()
        unsafe {
            delete d
        }
        t |> equal(class2_del_base, 1)
        t |> equal(class2_del_derived, 1)
    }
    t |> run("3-level class hierarchy chains all finalizers") @(t : T?) {
        class3_del_a = 0
        class3_del_b = 0
        class3_del_c = 0
        var c = new ClassC()
        unsafe {
            delete c
        }
        t |> equal(class3_del_a, 1)
        t |> equal(class3_del_b, 1)
        t |> equal(class3_del_c, 1)
    }
    t |> run("struct free-function finalizer chains via super.self") @(t : T?) {
        struct_del_base = 0
        struct_del_derived = 0
        var d = new StructDerived()
        unsafe {
            delete d
        }
        t |> equal(struct_del_base, 1)
        t |> equal(struct_del_derived, 1)
    }
    t |> run("delete super.self walks past empty intermediate class") @(t : T?) {
        skip_class_base = 0
        skip_class_leaf = 0
        var s = new SkipLeaf()
        unsafe {
            delete s
        }
        t |> equal(skip_class_base, 1)
        t |> equal(skip_class_leaf, 1)
    }
    t |> run("delete super.self walks past two empty intermediate classes") @(t : T?) {
        deep_class_base = 0
        deep_class_leaf = 0
        var d = new DeepLeaf()
        unsafe {
            delete d
        }
        t |> equal(deep_class_base, 1)
        t |> equal(deep_class_leaf, 1)
    }
    t |> run("delete super.self walks past empty mid-chain to nearest finalizer") @(t : T?) {
        mid_class_base = 0
        mid_class_b = 0
        mid_class_leaf = 0
        var m = new MidLeaf()
        unsafe {
            delete m
        }
        // MidLeaf -> (skip MidEmpty) -> MidB -> MidBase
        t |> equal(mid_class_base, 1)
        t |> equal(mid_class_b, 1)
        t |> equal(mid_class_leaf, 1)
    }
    t |> run("struct delete super.self walks past empty intermediate struct") @(t : T?) {
        skip_struct_base = 0
        skip_struct_leaf = 0
        var s = new SkipStructLeaf()
        unsafe {
            delete s
        }
        t |> equal(skip_struct_base, 1)
        t |> equal(skip_struct_leaf, 1)
    }
}

[test(description = "a class with no finalizer of its own inherits the ancestor finalizer past empty levels, and the generated chain finalizer deletes each slice's fields in derived-to-base order")]
def test_finalizer_inheritance(t : T?) {
    t |> run("derived class without finalizer inherits the base finalizer") @(t : T?) {
        inh_base = 0
        var d = new InhDerived()
        unsafe {
            delete d
        }
        t |> equal(inh_base, 1)
    }
    t |> run("finalizer inheritance chains past an empty intermediate class") @(t : T?) {
        inh_base = 0
        var l = new InhLeaf()
        unsafe {
            delete l
        }
        t |> equal(inh_base, 1)
    }
    t |> run("generated chain finalizer cleans the intermediate slice in dtor order") @(t : T?) {
        fin_order = ""
        var c = new TrC()
        unsafe {
            delete c
        }
        // TrC body (logs C, deletes its own tracer), then TrB's generated
        // chain finalizer (deletes TrB's tracer), then TrA's finalizer.
        t |> equal(fin_order, "C;tracer3;tracer2;A;")
    }
}

var delFoo = 0
var delFoo3D = 0

class Foo {
    x, y : int = 0
    [export]
    def Foo {
        Foo`set(self, 1, 1)
    }
    def Foo(T : int) {
        self.set(T, T)
    }
    def Foo(X, Y : int) {
        Foo`set(self, X, Y)
    }
    def set(X, Y : int) {
        x = X
        y = Y
    }
    def ups() {
        x = -1
        y = -1
    }
    def operator delete {
        delFoo ++
    }
}

class Foo3D : Foo {
    z : int = 13
    def Foo3D {
        assert(x == 0 && y == 0)
        assert(z == 13)
        Foo`Foo(self)
        assert(x == 1 && y == 1)
        z = 3
    }
    def override set(X, Y : int) {
        Foo`set(self, X, Y)
        z = 0
    }
    def override ups() {
        x = -2
        y = -2
        z = -2
    }
    def operator delete {
        delete super.self
        delFoo3D ++
    }
}

class Goo {
    a : int
}

def test_foo(tt : T?) {
    unsafe {
        var f = Foo()
        tt |> equal(f.x, 1)
        tt |> equal(f.y, 1)
        f->set(1, 2)
        tt |> equal(f.x, 1)
        tt |> equal(f.y, 2)
        f = Foo(11, 12)
        tt |> equal(f.x, 11)
        tt |> equal(f.y, 12)
        f = Foo(13)
        tt |> equal(f.x, 13)
        tt |> equal(f.y, 13)
        var f3d = Foo3D()
        tt |> equal(f3d.x, 1)
        tt |> equal(f3d.y, 1)
        tt |> equal(f3d.z, 3)
        f3d->set(1, 2)
        tt |> equal(f3d.x, 1)
        tt |> equal(f3d.y, 2)
        tt |> equal(f3d.z, 0)
        f3d.z = 13
        f3d->set(3, 4)
        tt |> equal(f3d.x, 3)
        tt |> equal(f3d.y, 4)
        tt |> equal(f3d.z, 0)
        tt |> equal(delFoo, 0)
        tt |> equal(delFoo3D, 0)
        delete f
        tt |> equal(delFoo, 1)
        tt |> equal(delFoo3D, 0)
        delete f3d
        tt |> equal(delFoo, 2)
        tt |> equal(delFoo3D, 1)
        delFoo = 0
        delFoo3D = 0
    }
}

def test_foo_ptr(tt : T?) {
    var f = new Foo()
    f->ups()
    tt |> equal(f.x, -1)
    tt |> equal(f.y, -1)
    tt |> equal(delFoo, 0)
    tt |> equal(delFoo3D, 0)
    unsafe {
        delete f
    }
    tt |> equal(delFoo, 1)
    tt |> equal(delFoo3D, 0)
    f = cast<Foo?>(new Foo3D())
    f->ups()
    tt |> equal(f.x, -2)
    tt |> equal(f.y, -2)
    unsafe {
        delete f
    }
    tt |> equal(delFoo, 2)
    tt |> equal(delFoo3D, 1)
    delFoo = 0
    delFoo3D = 0
}

def test_rtti(tt : T?) {
    unsafe {
        let g = Goo()
        let f = Foo()
        let f2 = Foo()
        let f3d = Foo3D()
        let f3d2 = Foo3D()
        tt |> success(f.__rtti != g.__rtti)
        tt |> success(f3d.__rtti != g.__rtti)
        tt |> equal(f.__rtti, f2.__rtti)
        tt |> equal(f3d.__rtti, f3d2.__rtti)
        tt |> success(f.__rtti != f3d.__rtti)
        tt |> success(class_info(f) != class_info(g))
        tt |> success(class_info(f3d) != class_info(g))
        tt |> equal(class_info(f), class_info(f2))
        tt |> equal(class_info(f3d), class_info(f3d2))
        tt |> success(class_info(f) != class_info(f3d))
        tt |> success(is_compatible_cast(class_info(f), class_info(f)))
        tt |> success(is_compatible_cast(class_info(f), class_info(f3d)))
        tt |> success(!is_compatible_cast(class_info(f3d), class_info(f)))
        tt |> success(!is_compatible_cast(class_info(g), class_info(f)))
        tt |> success(!is_compatible_cast(class_info(f), class_info(g)))
        tt |> success(!is_compatible_cast(class_info(g), class_info(f3d)))
        tt |> success(!is_compatible_cast(class_info(f3d), class_info(g)))
        tt |> equal(class_info(f).name, "Foo")
        tt |> equal(class_info(f3d).name, "Foo3D")
        tt |> equal(class_info(g).name, "Goo")
    }
}

[test(description = "class constructor overloads and methods set the fields, overrides dispatch virtually through a base pointer, a derived finalizer chains to the base once, and class_info with is_compatible_cast tells the classes apart")]
def test_oop(t : T?) {
    t |> run("class constructors and methods") @(t : T?) { test_foo(t); }
    t |> run("virtual dispatch via pointer") @(t : T?) { test_foo_ptr(t); }
    t |> run("RTTI class_info and cast") @(t : T?) { test_rtti(t); }
}

class Base {
    x : int = 0
    def Base {
        x = 10
    }
    def Base(val : int) {
        x = val
    }
    def get_value() : int => x
    def add(a, b : int) : int => a + b
}

class Derived : Base {
    y : int = 0
    def Derived {
        super()
        y = 20
    }
    def Derived(val : int) {
        super(val)
        y = val * 2
    }
    def override get_value() : int => x + y
    def override add(a, b : int) : int => super.add(a, b) * 2
    def get_base_value() : int => super.get_value()
}

class Derived2 : Derived {
    z : int = 0
    def Derived2 {
        super()
        z = 30
    }
    def override get_value() : int => x + y + z
    def override add(a, b : int) : int => super.add(a, b) + z
    def get_derived_value() : int => super.get_value()
    def get_base_value_from_derived2() : int => super.get_base_value()
}

// Both if/else branches call super — CFG walker must accept
// (min=max=1 per path).
class CondSuper : Base {
    z : int = 0
    def CondSuper(flag : bool) {
        if (flag) {
            super(7)
        } else {
            super()
        }
        z = flag ? 100 : 200
    }
}

// super() before an early return — both paths have exactly one super
// call. CFG walker must accept (the if-true path's super count
// matches the fall-through path's count).
class EarlyReturnSuper : Base {
    z : int = 0
    def EarlyReturnSuper(early : bool) {
        super()
        if (early) return
        z = 999
    }
}

// super() inside a try block — runs on normal path. CFG walker must
// count it. recover is for diagnostics-before-exit (panic is fatal)
// and is ignored by the lint.
class TrySuper : Base {
    z : int = 0
    def TrySuper {
        try {
            super(5)
        } recover {
            z = -1
        }
        z = 500
    }
}

// Synthesized chain: ImplicitChain has no constructor; the compiler
// emits one that chains super() → Base default ctor. new
// ImplicitChain() must run Base, setting x=10.
class ImplicitChain : Base {
    z : int = 999
}

// Two-level synth: both MidImplicit and LeafImplicit have no
// constructor. The compiler must synthesize ctors for both, chaining
// MidImplicit → Base and LeafImplicit → MidImplicit.
class MidImplicit : Base {
    m : int = 50
}

class LeafImplicit : MidImplicit {
    leaf : int = 70
}

// Synth respects parent ctor with default arg: BaseWithDefault has
// def BaseWithDefault(v : int = 99). DerivedWithDefault has no ctor,
// must synth a chain that resolves to
// BaseWithDefault`BaseWithDefault(self).
class BaseWithDefault {
    bx : int = 0
    def BaseWithDefault(v : int = 99) {
        bx = v
    }
}

class DerivedWithDefault : BaseWithDefault {
    dx : int = 5
}

// Skip-level inheritance: SuperSkipMid is empty (inherits Base's
// constructor and methods). SuperSkipLeaf calls super() /
// super.method() — both must walk past SuperSkipMid up to Base.
class SuperSkipMid : Base {
}

class SuperSkipLeaf : SuperSkipMid {
    z : int = 0
    def SuperSkipLeaf {
        super()
        z = 100
    }
    def SuperSkipLeaf(val : int) {
        super(val)
        z = val + 100
    }
    def override get_value() : int => x + z
    def get_super_value() : int => super.get_value()
    def override add(a, b : int) : int => super.add(a, b) + z
}

[test(description = "super() and super(args) reach the parent constructor and super.method reaches the parent method across three levels, past an empty intermediate class, on every if branch, before an early return and inside try, and synthesized constructors chain to the base, through a default argument too")]
def test_super(t : T?) {  // nolint:STYLE038 - flat list of independent test arms
    t |> run("super() calls parent default constructor") @(t : T?) {
        unsafe {
            let d = Derived()
            t |> equal(d.x, 10)
            t |> equal(d.y, 20)
        }
    }
    t |> run("super(args) calls parent parameterized constructor") @(t : T?) {
        unsafe {
            let d = Derived(5)
            t |> equal(d.x, 5)
            t |> equal(d.y, 10)
        }
    }
    t |> run("super.method() calls parent method bypassing override") @(t : T?) {
        unsafe {
            var d = Derived()
            // virtual dispatch: calls Derived.get_value
            t |> equal(d->get_value(), 30)
            // super.get_value in get_base_value: calls Base.get_value
            t |> equal(d->get_base_value(), 10)
        }
    }
    t |> run("super.method(args) calls parent method with arguments") @(t : T?) {
        unsafe {
            var d = Derived()
            // virtual dispatch: calls Derived.add which doubles super.add
            t |> equal(d->add(3, 4), 14)
        }
    }
    t |> run("super in 3-level hierarchy") @(t : T?) {
        unsafe {
            var d2 = Derived2()
            // constructor chain: Base(x=10) -> Derived(y=20) -> Derived2(z=30)
            t |> equal(d2.x, 10)
            t |> equal(d2.y, 20)
            t |> equal(d2.z, 30)
            // Derived2.get_value: x+y+z
            t |> equal(d2->get_value(), 60)
            // Derived2.get_derived_value calls super.get_value =
            // Derived.get_value = x+y
            t |> equal(d2->get_derived_value(), 30)
            // Derived2.get_base_value_from_derived2 calls super.get_base_value =
            // Derived.get_base_value = super.get_value = Base.get_value = x
            t |> equal(d2->get_base_value_from_derived2(), 10)
        }
    }
    t |> run("super.method(args) in 3-level hierarchy") @(t : T?) {
        unsafe {
            var d2 = Derived2()
            // Derived2.add calls super.add(a,b) + z
            // super.add = Derived.add = super.add(a,b) * 2
            // Derived's super.add = Base.add = a + b
            // so: (3+4)*2 + 30 = 44
            t |> equal(d2->add(3, 4), 44)
        }
    }
    t |> run("super via pointer with virtual dispatch") @(t : T?) {
        var d = new Derived()
        // virtual dispatch on pointer
        t |> equal(d->get_value(), 30)
        t |> equal(d->get_base_value(), 10)
        t |> equal(d->add(3, 4), 14)
        // cast to base pointer — virtual dispatch still calls Derived methods
        var b = cast<Base?>(d)
        t |> equal(b->get_value(), 30)
        t |> equal(b->add(3, 4), 14)
        unsafe {
            delete d
        }
    }
    t |> run("super() walks past empty intermediate class") @(t : T?) {
        unsafe {
            // SuperSkipMid has no constructor of its own; super() in
            // SuperSkipLeaf must reach Base.
            let s = SuperSkipLeaf()
            t |> equal(s.x, 10)   // Base default ctor ran
            t |> equal(s.z, 100)
        }
    }
    t |> run("super(args) walks past empty intermediate class") @(t : T?) {
        unsafe {
            let s = SuperSkipLeaf(7)
            t |> equal(s.x, 7)
            t |> equal(s.z, 107)
        }
    }
    t |> run("super.method() walks past empty intermediate class") @(t : T?) {
        unsafe {
            var s = SuperSkipLeaf()
            // SuperSkipLeaf.get_super_value calls super.get_value; SuperSkipMid
            // has no own override, so the call must resolve to Base.get_value
            // (returns x).
            t |> equal(s->get_super_value(), 10)
        }
    }
    t |> run("super.method(args) walks past empty intermediate class") @(t : T?) {
        unsafe {
            var s = SuperSkipLeaf()
            // SuperSkipLeaf.add calls super.add(a,b) + z; SuperSkipMid has no
            // add, walks to Base: a+b
            t |> equal(s->add(3, 4), 107)
        }
    }
    t |> run("super called on every branch of if/else (CFG-aware lint)") @(t : T?) {
        var c1 = new CondSuper(true)
        // flag=true → super(7) → x=7, z=100
        t |> equal(c1.x, 7)
        t |> equal(c1.z, 100)
        var c2 = new CondSuper(false)
        // flag=false → super() → x=10, z=200
        t |> equal(c2.x, 10)
        t |> equal(c2.z, 200)
        unsafe {
            delete c1
            delete c2
        }
    }
    t |> run("super in try block (CFG-aware lint accepts)") @(t : T?) {
        var ts = new TrySuper()
        // normal path: try runs super(5), z=500
        t |> equal(ts.x, 5)
        t |> equal(ts.z, 500)
        unsafe { delete ts; }
    }
    t |> run("synth default ctor chains super to Base") @(t : T?) {
        unsafe {
            // No def ImplicitChain(); the compiler synthesizes one that calls
            // Base`Base(self).
            let ic = ImplicitChain()
            t |> equal(ic.x, 10)   // Base default ctor ran
            t |> equal(ic.z, 999)
        }
        var icp = new ImplicitChain()
        t |> equal(icp.x, 10)
        t |> equal(icp.z, 999)
        unsafe { delete icp; }
    }
    t |> run("synth default ctor chains through two levels") @(t : T?) {
        unsafe {
            // Neither MidImplicit nor LeafImplicit declare a ctor.
            // LeafImplicit's synth chains → MidImplicit's synth → Base.
            let li = LeafImplicit()
            t |> equal(li.x, 10)    // Base ran
            t |> equal(li.m, 50)
            t |> equal(li.leaf, 70)
        }
    }
    t |> run("synth chain resolves to parent ctor with default arg") @(t : T?) {
        unsafe {
            // BaseWithDefault has def BaseWithDefault(v : int = 99); synth chain
            // must resolve to a 0-arg-callable signature.
            let d = DerivedWithDefault()
            t |> equal(d.bx, 99)    // default arg used
            t |> equal(d.dx, 5)
        }
    }
    t |> run("super before early return (both paths have super=1)") @(t : T?) {
        var e1 = new EarlyReturnSuper(true)
        // early=true → super ran, then returned before z=999; z stays default
        // 0
        t |> equal(e1.x, 10)
        t |> equal(e1.z, 0)
        var e2 = new EarlyReturnSuper(false)
        // early=false → super, no return, z=999
        t |> equal(e2.x, 10)
        t |> equal(e2.z, 999)
        unsafe {
            delete e1
            delete e2
        }
    }
}

struct OvFoo {
    x, y : int = 0
    set = @@OvFoo_setXY
    set2 = @@OvFoo_setXY
}

def OvFoo_setXY(var that : OvFoo; x, y : int) {
    that.x = x
    that.y = y
}

struct OvFoo3D : OvFoo {
    z : int = 3
    override set = cast<auto>(@@OvFoo3D_setXY)
    override set2 = @@OvFoo3D_setXY2
}

def OvFoo3D_setXY(var that : OvFoo3D; x, y : int) {
    that.x = x
    that.y = y
    that.z = 0
}

def OvFoo3D_setXY2(var _that : OvFoo; x, y : int) {
    unsafe {
        var that : OvFoo3D& = upcast<OvFoo3D&>(_that)
        that.x = x
        that.y = y
        that.z = 0
    }
}

[test(description = "a struct function-pointer field called with -> runs the base function, and a derived struct's override field replaces it whether bound through cast or through upcast")]
def test_override(t : T?) {
    t |> run("base struct function pointer") @(t : T?) {
        var f = OvFoo()
        f->set(1, 2)  // nolint:STYLE036 - the `-const` is on the cast the `->` desugar synthesizes
        t |> equal(f.x, 1)
        t |> equal(f.y, 2)
    }
    t |> run("derived struct override with cast") @(t : T?) {
        var f3d = OvFoo3D()
        t |> equal(f3d.x, 0)
        t |> equal(f3d.y, 0)
        t |> equal(f3d.z, 3)
        f3d->set(1, 2)  // nolint:STYLE036 - the `-const` is on the cast the `->` desugar synthesizes
        t |> equal(f3d.x, 1)
        t |> equal(f3d.y, 2)
        t |> equal(f3d.z, 0)
    }
    t |> run("derived struct override with upcast") @(t : T?) {
        var f3d = OvFoo3D()
        f3d.z = 13
        f3d->set2(3, 4)  // nolint:STYLE036 - the `-const` is on the cast the `->` desugar synthesizes
        t |> equal(f3d.x, 3)
        t |> equal(f3d.y, 4)
        t |> equal(f3d.z, 0)
    }
}

class AscFoo {
    a : int
    b : int
    def AscFoo {
        a = 13
    }
}

[sideeffects]
def test_ascend_ctor(t : T?) {
    let foo = new AscFoo(b = 42)
    t |> equal(13, foo.a)
    t |> equal(42, foo.b)
}

[test(description = "new with a named field initializer runs the class constructor and also applies the named field")]
def test_a_ctor(t : T?) {
    t |> run("ascend constructor") @(t : T?) {
        test_ascend_ctor(t)
    }
}

struct CtorFoo {
    i : int = 5
    f : float = 6.0
    s : string = "hello"
    n : int64
}

struct CtorBar {
    i : int
}

def CtorBar() : CtorBar {
    var b : CtorBar
    b.i = 13
    return b
}

[test(description = "a struct literal applies the field defaults, and a user def named after the struct serves as its constructor")]
def test_ctor(t : T?) {
    t |> run("struct default initializer") @(t : T?) {
        let tf = CtorFoo()
        t |> equal(tf.i, 5)
    }
    t |> run("struct custom constructor") @(t : T?) {
        let b = CtorBar()
        t |> equal(b.i, 13)
    }
}

class Node {
    def kind() : int => 1
}

class LeafNode : Node {
    def override kind() : int => 2
}

[test(description = "an array of base pointers built by an explicit-element-type constructor literal upcasts each derived element, so virtual calls reach the derived override")]
def test_ctor_literal_pointer_upcast(t : T?) {
    t |> run("derived instances land upcast in a base-pointer array") @(t : T?) {
        var arr <- array<Node?>(new LeafNode(), new Node())
        t |> equal(length(arr), 2)
        t |> equal(arr[0]->kind(), 2)
        t |> equal(arr[1]->kind(), 1)
        unsafe {
            delete arr
        }
    }
}

struct MethodPoint {
    x, y = 0.0f
    magnitude = @@method_point_magnitude
    dot = @@method_point_dot
}

def MethodPoint(X, Y) => MethodPoint(x = X, y = Y)

def method_point_magnitude(that : MethodPoint) => that.x * that.x + that.y * that.y

def method_point_dot(that, p : MethodPoint) => that.x * p.x + that.y * p.y

[test(description = "struct fields holding function pointers are callable as methods with ->, receiving the struct as their first argument")]
def test_method_semantic(t : T?) {
    t |> run("function pointer as method") @(t : T?) {
        let p = MethodPoint(2., 3.)
        let mag = p->magnitude()
        let d = p->dot(MethodPoint(1., 1.))
        t |> equal(mag, 13.)
        t |> equal(d, 5.)
    }
}

struct Box {
    value : int = 0

    [class_method]
    def static put(v : auto) {
        value = int(v)
    }

    [class_method]
    def static get : int {
        return value
    }

    [class_method]
    def static add(v : auto) : int {
        value += int(v)
        return value
    }
}

[test(description = "[class_method] on a generic static struct method injects self into every instantiation, so int and float calls both update the same value field")]
def test_generic_class_method(t : T?) {
    t |> run("instantiate generic put with int") @(it : T?) {
        var b = Box()
        b.put(42)
        it |> equal(b.get(), 42)
    }
    t |> run("instantiate generic put with float") @(it : T?) {
        var b = Box()
        b.put(7.5)
        it |> equal(b.get(), 7)
    }
    t |> run("multiple instantiations on the same value") @(it : T?) {
        var b = Box()
        b.put(10)
        let r1 = b.add(5)
        let r2 = b.add(2.0)
        it |> equal(r1, 15)
        it |> equal(r2, 17)
        it |> equal(b.get(), 17)
    }
}

var g_chain_ctor_count = 0
var g_chain_upper_count = 0
var g_chain_utf8_count = 0

class ChainString {
    def ChainString {
        g_chain_ctor_count++
    }
    def to_upper() : ChainString? {
        g_chain_upper_count++
        return addr(self)
    }
    def utf8() : ChainString? {
        g_chain_utf8_count++
        return addr(self)
    }
}

[test(description = "new Class()->method()->method() constructs the object once and calls each chained method exactly once")]
def test_chain_invoke_method(t : T?) {
    t |> run("new->method1->method2 chaining") @(t : T?) {
        g_chain_ctor_count = 0
        g_chain_upper_count = 0
        g_chain_utf8_count = 0
        let res = new ChainString()->to_upper()->utf8()
        t |> equal(g_chain_ctor_count, 1)
        t |> equal(g_chain_upper_count, 1)
        t |> equal(g_chain_utf8_count, 1)
    }
}

struct Dummy {}

struct Ball {
    position : float3
    radius : float
}

struct Balls {
    a, b : Ball
}

def get_ball_radius(b : Ball) : float => b.radius

def get_balls_radius(b : Balls) : float => b.a.radius + b.b.radius

def set_default_ball(var b : Ball) {
    b.position = float3(1.0, 2.0, 3.0)
    b.radius = 5.0
}

def make_default_ball : Ball? {
    var b : Ball? = new Ball
    set_default_ball(deref(b))
    return b
}

def verify_default_ball(t : T?; b : Ball) {
    t |> equal(b.position, float3(1.0, 2.0, 3.0))
    t |> equal(b.radius, 5.0)
}

def test_ref_ptr(t : T?; var a : Ball?&; var b : Ball?) {
    a = b
    t |> equal(a, b)
}

def test_ret_ptr(var a : Ball?) : Ball? => a

struct L3 {
    a : int
}

struct L2 {
    l3 : L3?
}

struct L1 {
    l2 : L2?
}

var glob : L1? = null

[test(description = "an empty struct has size 0, struct values and pointers copy and compare by value and by address, and a null-safe field chain with ?? writes the fallback until every link exists")]
def test_struct(t : T?) {
    t |> run("empty struct sizeof") @(t : T?) {
        t |> equal(typeinfo sizeof(type<Dummy>), 0)
    }
    t |> run("struct pointer and copy") @(t : T?) {
        var b : Ball? = new Ball  // nolint:STYLE013 - field writes through a Ball? pointer are the tested construct
        b.position = float3(1.0, 2.0, 3.0)
        b.radius = 5.0
        verify_default_ball(t, deref(b))
        var xb, qb : Ball
        var qc : Ball? = b
        var qd : Ball? = new Ball
        qb = deref(qc)
        verify_default_ball(t, qb)
        verify_default_ball(t, deref(qc))
        qc = null
        t |> equal(qc, null)
        t |> success(qd != null)
        test_ref_ptr(t, qc, qd)
        t |> equal(qc, qd)
        qd = test_ret_ptr(null)
        t |> equal(qd, null)
        t |> success(qd != qc)
        xb = qb
        verify_default_ball(t, xb)
    }
    t |> run("make_default_ball via ptr") @(t : T?) {
        let b : Ball? = make_default_ball()
        verify_default_ball(t, deref(b))
    }
    t |> run("null-safe field chain") @(t : T?) {
        var dummy : int
        glob?.l2?.l3?.a ?? dummy = 1
        t |> equal(dummy, 1)
        glob = new L1
        glob?.l2?.l3?.a ?? dummy = 2
        t |> equal(dummy, 2)
        glob.l2 = new L2
        glob?.l2?.l3?.a ?? dummy = 3
        t |> equal(dummy, 3)
        glob.l2.l3 = new L3
        glob?.l2?.l3?.a ?? dummy = 4
        t |> equal(dummy, 3)
        t |> equal(glob?.l2?.l3?.a ?? 5, 4)
    }
}

enum MdEnum {
    foo = 0
    bar
}

struct MdStruct {
    x : int
    y : float
}

[test(description = "a type-name call with no arguments yields the zero value of int, string, tuple, variant, struct, enum and array, and array struct syntax sets the named fields and zeroes the rest")]
def test_make_default(t : T?) {
    t |> run("default int") @(t : T?) {
        let t_int = int()
        t |> equal(t_int, 0)
    }
    t |> run("default string") @(t : T?) {
        let t_string = string("")  // nolint:PERF020 - the ctor form is the subject; string() has no zero-arg form
        t |> equal(t_string, "")
    }
    t |> run("default tuple") @(t : T?) {
        let t_tuple = tuple<int; float>()
        t |> equal(t_tuple._0, 0)
        t |> equal(t_tuple._1, 0.0)
    }
    t |> run("default variant") @(t : T?) {
        let t_variant = variant<a : int; b : float>()
        t |> equal(t_variant is a, true)
        t |> equal(t_variant as a, 0)
    }
    t |> run("default struct") @(t : T?) {
        let t_struct = MdStruct()
        t |> equal(t_struct.x, 0)
        t |> equal(t_struct.y, 0.0)
    }
    t |> run("default enum") @(t : T?) {
        let t_enum = default<MdEnum>
        t |> equal(t_enum, MdEnum.foo)
    }
    t |> run("default array") @(t : T?) {
        let t_array <- array<int>()
        t |> equal(length(t_array), 0)
    }
    t |> run("array struct syntax") @(t : T?) {
        let t_array_struct = array struct<MdStruct>((x = 1), (x = 2))
        t |> equal(length(t_array_struct), 2)
        t |> equal(t_array_struct[0].x, 1)
        t |> equal(t_array_struct[1].x, 2)
        t |> equal(t_array_struct[0].y, 0.0)
        t |> equal(t_array_struct[1].y, 0.0)
    }
}

[safe_when_uninitialized]
struct MlBar {
    x : int = 1
    y : int = 2
    z : int = 3
    w : int = 4
}

[safe_when_uninitialized]
struct MlFoo {
    i : int = 5
    f : float = 6.0
    s : string = "hello"
}

let {
    ml_testFoo = MlFoo()
}

let {
    ml_jestFoo = MlFoo(i = 3, f = 4., s = "blah")
}

def ml_test_bar(t : T?) {
    let tf = MlBar()
    t |> equal(tf.x, 1)
    t |> equal(tf.y, 2)
    t |> equal(tf.z, 3)
    t |> equal(tf.w, 4)
    let t0 = MlBar(uninitialized x = 13)
    t |> equal(t0.x, 13)
    t |> equal(t0.y, 0)
    t |> equal(t0.z, 0)
    t |> equal(t0.w, 0)
    let t1 = MlBar(x = 13)
    t |> equal(t1.x, 13)
    t |> equal(t1.y, 2)
    t |> equal(t1.z, 3)
    t |> equal(t1.w, 4)
}

[test(description = "struct literals apply the field defaults in locals, globals and fixed arrays, named fields override them, and uninitialized leaves every unnamed field zero")]
def test_make_local(t : T?) {
    t |> run("bar default and uninitialized") @(t : T?) {
        ml_test_bar(t)
    }
    t |> run("global struct defaults") @(t : T?) {
        t |> equal(ml_testFoo.i, 5)
        t |> equal(ml_testFoo.f, 6.0)
        t |> equal(ml_testFoo.s, "hello")
    }
    t |> run("global struct custom init") @(t : T?) {
        t |> equal(ml_jestFoo.i, 3)
        t |> equal(ml_jestFoo.f, 4.0)
        t |> equal(ml_jestFoo.s, "blah")
    }
    t |> run("local struct defaults") @(t : T?) {
        let tf = MlFoo()
        t |> equal(tf.i, 5)
        t |> equal(tf.f, 6.0)
        t |> equal(tf.s, "hello")
    }
    t |> run("local struct uninitialized fields") @(t : T?) {
        let q = MlFoo(uninitialized i = 6, s = "world")
        t |> equal(q.i, 6)
        t |> equal(q.f, 0.0)
        t |> equal(q.s, "world")
    }
    t |> run("fixed_array of structs with uninitialized") @(t : T?) {
        let qq3 = [MlFoo(uninitialized i = 1), MlFoo(uninitialized i = 2),
                   MlFoo(uninitialized i = 3)]
        t |> equal(qq3[0].i, 1)
        t |> equal(qq3[1].i, 2)
        t |> equal(qq3[2].i, 3)
    }
    t |> run("fixed_array of ints") @(t : T?) {
        let ar = fixed_array<int>(1, 2, 3, 4)
        t |> equal(ar[0], 1)
        t |> equal(ar[1], 2)
        t |> equal(ar[2], 3)
        t |> equal(ar[3], 4)
    }
    t |> run("fixed_array of structs") @(t : T?) {
        let arf = fixed_array<MlFoo>(MlFoo(), MlFoo())
        t |> equal(arf[0].i, 5)
        t |> equal(arf[1].i, 5)
    }
}

struct MscFoo {
    a : array<int>
    b : string
}

[test(description = "a struct literal field initialized with := clones the source array and leaves the source intact, inside an array literal too")]
def test_make_struct_with_clone(t : T?) {
    t |> run("struct with cloned array field") @(t : T?) {
        let sa <- [1, 2, 3, 4]
        let sb <- [1, 2, 3]
        let fa <- MscFoo(a := sa)
        let fa2 <- [MscFoo(a := sa), MscFoo(a := sb)]
        t |> equal(length(sa), 4)
        t |> equal(length(fa.a), 4)
        t |> equal(length(fa2[0].a), 4)
        t |> equal(length(fa2[1].a), 3)
    }
    t |> run("struct clone with string builder") @(t : T?) {
        let sa <- [1, 2, 3, 4]
        var faw <- MscFoo(a := sa, b = "a is ")
        faw.b += join(faw.a, "-")
        t |> equal(length(faw.a), 4)
        t |> equal(faw.b, "a is 1-2-3-4")
    }
}

[safe_when_uninitialized]
struct NwiBall {
    center : float3 = float3(7, 8, 9)
    radius : float = 1.234f
}

def NwiBall(center : float3; radius : float) : NwiBall {
    // This helper IS the user-defined `def TypeName(args)` constructor
    // that the test below exercises via `new NwiBall(float3(1,2,3),
    // 4.0f)`. Rewriting to `NwiBall(center=center, radius=radius)` would
    // recurse — named-arg lookup resolves to this same function, not to
    // the makestruct.
    var b : NwiBall  // nolint:STYLE013
    b.center = center
    b.radius = radius
    return b
}

[test(description = "new T zeroes the fields, new T() applies the field defaults, and new with constructor arguments runs the user constructor for one value and for every element of a fixed array")]
def test_new_with_init(t : T?) {
    t |> run("new without parens — zeroed") @(t : T?) {
        var b0 = new NwiBall
        t |> equal(b0.center, float3())
        t |> equal(b0.radius, 0.0f)
        unsafe { delete b0; }
    }
    t |> run("new with parens — defaults") @(t : T?) {
        var b1 = new NwiBall()
        t |> equal(b1.center, float3(7, 8, 9))
        t |> equal(b1.radius, 1.234f)
        unsafe { delete b1; }
    }
    t |> run("new with custom constructor") @(t : T?) {
        var b2 = new NwiBall(float3(1, 2, 3), 4.0f)
        t |> equal(b2.center, float3(1, 2, 3))
        t |> equal(b2.radius, 4.0f)
        unsafe { delete b2; }
    }
    t |> run("new array with constructor") @(t : T?) {
        var b3 = new <NwiBall[3]>(float3(3, 2, 1), 0.5f)
        for (b in b3) {
            t |> equal(b.center, float3(3, 2, 1))
            t |> equal(b.radius, 0.5f)
        }
        unsafe { delete b3; }
    }
}

[no_default_initializer]
struct NdiFoo {
    a : int = 5
    b : float = 6.
}

[test(description = "a [no_default_initializer] struct literal still applies the declared field defaults to every field it does not name")]
def test_no_default_initialize(t : T?) {
    var f = NdiFoo(a = 10)
    t |> equal(f.a, 10)
    t |> equal(f.b, 6.)
    f = NdiFoo()
    t |> equal(f.a, 5)
    t |> equal(f.b, 6.)
}

// The structs, globals and helpers below are referenced ONLY from
// field and default-argument initializers - no function uses them -
// so unused-symbol removal must null or keep those references, and
// dastest's --ser sweep over tests/ serializes them without a
// dangling pointer.

struct PlainPod {
    a : int
    b : float
}

struct WithInit {
    x : int = 7
}

struct Nested {
    p : PlainPod
    w : WithInit = WithInit()
}

[safe_when_uninitialized]
struct OptedOut {
    x : int = 7
}

// mutable so const folding can't erase the ExprVar - the global's
// only anchor is the field init
var g_seed = 13

struct UsesGlobal {
    s : int = g_seed
}

// builtin-module global (folded addConstant, index=-2): builtin
// modules are never pruned, so the cleaner must LEAVE this ref alone
// (only thisModule's freed globals get nulled)
struct UsesBuiltinGlobal {
    p : float = PI
}

// default-argument initializers are the same story: MarkSymbolUse
// skips them too (canVisitArgumentInit is false there), so a helper
// whose only anchor is `x = arg_helper()` is freed while the
// surviving function keeps the raw pointer in arg->init. arg_helper
// writes a global so const folding can't erase the call, and
// g_arg_state rides along as the Variable half of the same pin
var private g_arg_state = 1

def private arg_helper() : int {
    g_arg_state ++
    return g_arg_state
}

[export]
def default_arg_anchor(x : int = arg_helper()) : int => x

variant PodVariant {
    i : int
    f : float
}

variant InitVariant {
    i : int
    w : WithInit
}

[test(description = "structs, globals and a helper referenced only from field and default-argument initializers compile and survive unused-symbol removal, which the serialization sweep then reads back")]
def test_field_init_ctor_survives_marking(t : T?) {
    t |> run("the structs above exist and stay otherwise unused") @(t : T?) {
        t |> success(true, "the pin is the file itself under the --ser sweep")
    }
}

// Custom property operator: `.res :=` dispatches to a setter
struct PropFoo {
    resz : string
}

def operator . res :=(var f : PropFoo; st : string#) {
    f.resz := st
}

// Class with getter/setter property pair
class sealed UIFrame {
    _size : float2
    def const operator . sizezz() : float2 => _size
    def operator . sizezz :=(value : float2) {
        _size = value
    }
}

[test(description = "clone-assignment of a temp string lands on a local, on a struct field, and through a user .res := property operator")]
def test_property_clone_assign(t : T?) {
    t |> run("clone-assign string via block") @(t : T?) {
        var res = ""
        res := unsafe(reinterpret<string#>("foo"))
        t |> equal(res, "foo")
    }
    t |> run("clone-assign to struct field directly") @(t : T?) {
        var f : PropFoo
        f.resz := unsafe(reinterpret<string#>("bar"))
        t |> equal(f.resz, "bar")
    }
    t |> run("clone-assign via property operator") @(t : T?) {
        var f : PropFoo
        f.res := unsafe(reinterpret<string#>("baz"))
        t |> equal(f.resz, "baz")
    }
}

[test(description = "a class getter and setter property pair stores and returns the value, and the last of repeated assignments wins")]
def test_class_property_operators(t : T?) {
    t |> run("setter assigns value") @(t : T?) {
        var frame = new UIFrame()
        frame.sizezz := float2(100.0, 200.0)
        t |> equal(frame.sizezz, float2(100.0, 200.0))
        unsafe { delete frame; }
    }
    t |> run("getter returns current value") @(t : T?) {
        var frame = new UIFrame()
        frame.sizezz := float2(3.0, 4.0)
        let sz = frame.sizezz
        t |> equal(sz.x, 3.0)
        t |> equal(sz.y, 4.0)
        unsafe { delete frame; }
    }
    t |> run("multiple assignments overwrite") @(t : T?) {
        var frame = new UIFrame()
        frame.sizezz := float2(1.0, 1.0)
        frame.sizezz := float2(5.0, 6.0)
        t |> equal(frame.sizezz, float2(5.0, 6.0))
        unsafe { delete frame; }
    }
}

struct Cell {
    raw : int
}

def operator . value(c : Cell) : int => c . . raw * 2

def operator . value :=(var c : Cell; v : int) {
    c . . raw = v / 2
}

def operator . value +=(var c : Cell; v : int) {
    c . . raw += v / 2
}

def operator .(c : Cell; name : string) : int => name == "twice" ? c . . raw * 2 : -1

[test(description = "named and by-name property getters, the := setter and the += operator apply on a struct value, through a pointer, and inside with blocks")]
def test_property_promotion_paths(t : T?) {
    t |> run("named getter") @(t : T?) {
        var c = Cell(raw = 21)
        var pc = unsafe(addr(c))
        t |> equal(c.value, 42)
        t |> equal(pc.value, 42)
    }
    t |> run("with-name getter") @(t : T?) {
        var c = Cell(raw = 4)
        var pc = unsafe(addr(c))
        t |> equal(c.twice, 8)
        t |> equal(pc.twice, 8)
        t |> equal(c.anything_else, -1)
    }
    t |> run("setter and op-assign") @(t : T?) {
        var c = Cell(raw = 0)
        var pc = unsafe(addr(c))
        c.value := 10
        t |> equal(c . . raw, 5)
        pc.value := 30
        t |> equal(c . . raw, 15)
        c.value += 4
        t |> equal(c . . raw, 17)
        pc.value += 6
        t |> equal(c . . raw, 20)
    }
    t |> run("getters in a with block") @(t : T?) {
        var c = Cell(raw = 3)
        var pc = unsafe(addr(c))
        with (c) {
            t |> equal(value, 6)
            t |> equal(twice, 6)
        }
        with (pc) {
            t |> equal(value, 6)
            t |> equal(twice, 6)
        }
    }
    t |> run("setter in a with block") @(t : T?) {
        var c = Cell(raw = 0)
        with (c) {
            value := 8
        }
        t |> equal(c . . raw, 4)
    }
}

struct U {
    x : int
}

def operator .s(u : U) : int => u.x

def operator .s :=(var u : U; v : int) {
    u.x = v
}

struct S {
    u : U
}

[test(description = "a += through a property under deref of a pointer sum writes the field of the element at that offset")]
def test_compound_assignment_through_a_property_under_deref_of_a_pointer_sum(t : T?) {
    var arr : S[4]
    unsafe {
        var p : S? = addr(arr[0])
        deref(p + 2).u.s += 1
        let k = 3
        deref(p + k).u.s += 5
    }
    t |> equal(arr[2].u.x, 1)
    t |> equal(arr[3].u.x, 5)
}

[test(description = "a += on a property inside a with over deref of a pointer sum writes the field of that element")]
def test_compound_assignment_through_a_property_of_a_with_over_deref_of_a_pointer_sum(t : T?) {
    var arr : S[4]
    unsafe {
        var p : S? = addr(arr[0])
        with (deref(p + 1).u) {
            s += 4
        }
    }
    t |> equal(arr[1].u.x, 4)
}

[test(description = "a += on a property inside a with over a pointer writes through the pointer into the pointee")]
def test_compound_assignment_through_a_property_of_a_with_over_a_pointer(t : T?) {
    var arr : S[4]
    var p : U? = unsafe(addr(arr[2].u))
    with (p) {
        s += 6
    }
    t |> equal(arr[2].u.x, 6)
}

[sideeffects]
def takeFoo(t : T?; var foo : TestObjectFoo; fooRO : TestObjectFoo) {
    t |> success(!foo.isReadOnly)
    t |> success(fooRO.isReadOnly)
}

[test(description = "the isReadOnly extension property reads true on a let handle and false on a var handle, and keeps that answer through by-value arguments")]
def test_add_property_ext_const(t : T?) {
    var foo : TestObjectFoo
    let fooRO : TestObjectFoo
    t |> success(!foo.isReadOnly)
    t |> success(fooRO.isReadOnly)
    takeFoo(t, foo, fooRO)
}

// A class with the same name as a class in a required module. The
// generated __finalize field init must pin to this module's
// Doppel'__finalize.
class Doppel {
    y : int = 2
    def value : int => y
}

// deriving from the required module's twin while the local Doppel
// exists - the derived class regenerates __finalize in THIS module
// and must pin there too
class DoppelChild : _finalize_collision_helper::Doppel {
    z : int = 3
}

[test(description = "a class named like a class of a required module compiles, and a class derived from the module's twin constructs and finalizes with both its own and the inherited field initialized")]
def test_class_name_collision(t : T?) {
    t |> run("same-named class across modules compiles") @(t : T?) {
        t |> success(true)
    }
    t |> run("derive from the required module's twin") @(t : T?) {
        var c = new DoppelChild()
        t |> equal(1, c.x)
        t |> equal(3, c.z)
        unsafe {
            delete c
        }
    }
}
