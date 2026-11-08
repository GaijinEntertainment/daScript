options gen2
// the move-assign lowering emits one of these bindings per `left <-
// right`; the needs_container_finalize rows ride the same policy
options force_inscope_pod
require dastest/testing_boost public

// A local reference may bind to an ARGUMENT: the storage lives in a
// frame that outlives the binding, exactly like a local. The hard
// ref-to-temporary check must not claim otherwise - a
// struct/array/tuple argument carries type->ref == false (it is
// already by-reference at the ABI level), which is why that check
// tests the initializer's shape and not just its type.

struct Bag {
    xs : array<int>
}

struct Nest {
    bag : Bag
}

def private bind_arg(var container : Bag) {
    unsafe {                        // binding to a non-local stays policy-gated
        var r & = container
        r.xs |> push(1)
    }
}

def private bind_arg_field(var nest : Nest) {
    unsafe {
        var r & = nest.bag
        r.xs |> push(2)
    }
}

// the inscope-pod lowering rewrites `left <- right` into `var
// _pod_inscope_temp & = left` plus a collect call, so a move into an
// argument goes through the same check with no `&` in sight
def private fill_arg(var container : Bag) {
    var cpy : Bag
    cpy.xs |> push(42)
    container <- cpy
}

def private fill_arg_field(var nest : Nest) {
    var cpy : Bag
    cpy.xs |> push(43)
    nest.bag <- cpy
}

def private fill_arg_deref(var container : Bag?) {
    var cpy : Bag
    cpy.xs |> push(44)
    *container <- cpy
}

def private make_bag(n : int) : Bag {
    var b : Bag
    b.xs |> push(n)
    return <- b
}

[test(description = "a local reference bound to a var argument, or to a field of one, writes through to the caller's value")]
def test_local_ref_to_argument(t : T?) {
    t |> run("reference bound to an argument") @(t : T?) {
        var inscope b : Bag
        bind_arg(b)
        t |> equal(length(b.xs), 1)
        t |> equal(b.xs[0], 1)
    }
    t |> run("reference bound to an argument field") @(t : T?) {
        var inscope n : Nest
        bind_arg_field(n)
        t |> equal(n.bag.xs[0], 2)
    }
}

[test(description = "under force_inscope_pod a move into a var argument, its field, a dereferenced pointer argument or a caller temporary compiles and lands the value")]
def test_move_to_argument(t : T?) {
    t |> run("move into a var argument") @(t : T?) {
        var inscope b : Bag
        fill_arg(b)
        t |> equal(length(b.xs), 1)
        t |> equal(b.xs[0], 42)
    }
    t |> run("move into a var argument field") @(t : T?) {
        var inscope n : Nest
        fill_arg_field(n)
        t |> equal(length(n.bag.xs), 1)
        t |> equal(n.bag.xs[0], 43)
    }
    t |> run("move through a pointer argument") @(t : T?) {
        var inscope b : Bag
        fill_arg_deref(unsafe(addr(b)))
        t |> equal(length(b.xs), 1)
        t |> equal(b.xs[0], 44)
    }
    t |> run("move into an argument that is the caller's temporary") @(t : T?) {
        // the collect-and-zero lands on a temp-stack slot in this frame, live
        // for the whole call - the same guarantee a local gives
        fill_arg(make_bag(7))
        fill_arg(Bag())
        t |> success(true)
    }
}

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

struct DeepPod {
    p : PlainPod
}

[safe_when_uninitialized]
struct OptedOut {
    x : int = 7
}

variant PodVariant {
    i : int
    f : float
}

variant InitVariant {
    i : int
    w : WithInit
}

struct BagLike {
    xs : array<int>
}

struct UserFin {
    xs : array<int>
}

def finalize(var u : UserFin) {
    delete u.xs
}

struct HeapInFixed {
    a : array<int>[2]
}

[test(description = "the needs_nontrivial_init, needs_container_init, needs_container_finalize, is_pod_delete and is_safe_to_delete type traits answer true only for types with field inits or owned heap, honoring safe_when_uninitialized and user finalizers")]
def test_needs_nontrivial_init(t : T?) {
    t |> run("trivially-zero types") @(t : T?) {
        t |> success(!typeinfo needs_nontrivial_init(type<int>), "int is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<float3>), "float3 is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<string>), "string is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<PlainPod>), "plain struct is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<DeepPod>),
                     "nested plain struct is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<array<int>>), "array<int> is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<table<string; int>>), "table is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<WithInit?>),
                     "pointer is trivial even to init-carrying pointee")
        t |> success(!typeinfo needs_nontrivial_init(type<int[4]>), "fixed array of int is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<tuple<int; float>>),
                     "pod tuple is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<PodVariant>), "pod variant is trivial")
    }
    t |> run("init-carrying types") @(t : T?) {
        t |> success(typeinfo needs_nontrivial_init(type<WithInit>), "field init counts")
        t |> success(typeinfo needs_nontrivial_init(type<Nested>), "nested field init counts")
        t |> success(typeinfo needs_nontrivial_init(type<WithInit[3]>), "fixed array recurses")
        t |> success(typeinfo needs_nontrivial_init(type<tuple<int; WithInit>>), "tuple recurses")
        t |> success(typeinfo needs_nontrivial_init(type<InitVariant>), "variant recurses")
    }
    t |> run("safe_when_uninitialized opts out") @(t : T?) {
        t |> success(!typeinfo needs_nontrivial_init(type<OptedOut>), "opt-out honored")
    }
    t |> run("needs_container_init composes the policy") @(t : T?) {
        t |> success(typeinfo needs_container_init(type<WithInit>), "policy defaults on")
        t |> success(!typeinfo needs_container_init(type<int>), "trivial stays off")
    }
    t |> run("needs_container_finalize composes the inscope-pod policy") @(t : T?) {
        t |> success(typeinfo needs_container_finalize(type<array<int>>),
                     "owned composite, policy on")
        t |> success(typeinfo needs_container_finalize(type<BagLike>),
                     "generated finalizer with heap")
        t |> success(!typeinfo needs_container_finalize(type<PlainPod>),
                     "POD struct - nothing to free, zero cost")
        t |> success(!typeinfo needs_container_finalize(type<WithInit>),
                     "field inits alone own no heap")
        t |> success(!typeinfo needs_container_finalize(type<UserFin>),
                     "user finalizers never run implicitly")
        t |> success(!typeinfo needs_container_finalize(type<PlainPod?>), "raw pointer never")
        t |> success(!typeinfo needs_container_finalize(type<int>), "nothing to finalize")
        t |> success(!typeinfo needs_container_finalize(type<tuple<int; PlainPod?>>),
                     "pointer-carrying tuple")
        t |> success(typeinfo needs_container_finalize(type<tuple<int; array<int>>>),
                     "heap-carrying tuple")
    }
    t |> run("is_pod_delete is the pure predicate behind it") @(t : T?) {
        t |> success(typeinfo is_pod_delete(type<array<int>>), "owned heap")
        t |> success(typeinfo is_pod_delete(type<BagLike>),
                     "struct owning heap, generated finalizer")
        t |> success(typeinfo is_pod_delete(type<HeapInFixed>), "heap through a fixed array")
        t |> success(!typeinfo is_pod_delete(type<PlainPod>), "no heap to free")
        t |> success(!typeinfo is_pod_delete(type<UserFin>), "user finalizer disqualifies")
        t |> success(!typeinfo is_pod_delete(type<PlainPod?>), "raw pointer")
        t |> success(!typeinfo is_pod_delete(type<lambda<(arg : int) : void>>), "lambda")
    }
    t |> run("is_safe_to_delete stays the unsafe-delete predicate") @(t : T?) {
        t |> success(typeinfo is_safe_to_delete(type<array<int>>), "owned value composite")
        t |> success(!typeinfo is_safe_to_delete(type<PlainPod?>), "raw pointer")
        t |> success(!typeinfo is_safe_to_delete(type<tuple<PlainPod?; int>>),
                     "pointer-carrying tuple")
        t |> success(!typeinfo is_safe_to_delete(type<lambda<(arg : int) : void>>), "lambda")
    }
}
