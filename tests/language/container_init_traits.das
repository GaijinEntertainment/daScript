options gen2
require dastest/testing_boost public

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

[test]
def test_needs_nontrivial_init(t : T?) {
    t |> run("trivially-zero types") @(t : T?) {
        t |> success(!typeinfo needs_nontrivial_init(type<int>), "int is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<float3>), "float3 is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<string>), "string is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<PlainPod>), "plain struct is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<DeepPod>), "nested plain struct is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<array<int>>), "array<int> is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<table<string; int>>), "table is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<WithInit?>), "pointer is trivial even to init-carrying pointee")
        t |> success(!typeinfo needs_nontrivial_init(type<int[4]>), "fixed array of int is trivial")
        t |> success(!typeinfo needs_nontrivial_init(type<tuple<int; float>>), "pod tuple is trivial")
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
    t |> run("needs_container_finalize composes the same policy") @(t : T?) {
        t |> success(typeinfo needs_container_finalize(type<array<int>>), "owned composite, policy on")
        t |> success(!typeinfo needs_container_finalize(type<PlainPod?>), "raw pointer never")
        t |> success(!typeinfo needs_container_finalize(type<int>), "nothing to finalize")
    }
    t |> run("is_safe_to_delete gates the finalize banner") @(t : T?) {
        t |> success(typeinfo is_safe_to_delete(type<array<int>>), "owned value composite")
        t |> success(!typeinfo is_safe_to_delete(type<PlainPod?>), "raw pointer")
        t |> success(!typeinfo is_safe_to_delete(type<tuple<PlainPod?; int>>), "pointer-carrying tuple")
        t |> success(!typeinfo is_safe_to_delete(type<lambda<(arg : int) : void>>), "lambda")
    }
}
