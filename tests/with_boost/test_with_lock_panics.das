options gen2
options indenting = 4

require dastest/testing_boost public
require daslib/with_boost

//! When the with_ body panics, the lock leaks (daslang issue #2532).
//! These tests deliberately don't try to ``delete`` the leaked-locked
//! containers afterwards — that would compound the failure. We only
//! verify the panic is caught at the with_ boundary.

struct A {
    f1 : int
}

[test]
def test_push_inside_panics(t : T?) {
    var arr <- [A(f1=1), A(f1=2)]
    var panicked = false
    try {
        with_(arr[0]) $(a) {
            arr |> push(A(f1 = 99))
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "push during with_ body must panic")
}

[test]
def test_erase_inside_panics(t : T?) {
    var arr <- [A(f1=1), A(f1=2), A(f1=3)]
    var panicked = false
    try {
        with_(arr[1]) $(a) {
            arr |> erase(0)
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "erase during with_ body must panic")
}

[test]
def test_resize_inside_panics(t : T?) {
    var arr <- [A(f1=1)]
    var panicked = false
    try {
        with_(arr[0]) $(a) {
            arr |> resize(100)
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "resize during with_ body must panic")
}

[test]
def test_clear_inside_panics(t : T?) {
    var arr <- [A(f1=1)]
    var panicked = false
    try {
        with_(arr[0]) $(a) {
            arr |> clear
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "clear during with_ body must panic")
}

[test]
def test_table_insert_inside_panics(t : T?) {
    var tab : table<string; A>
    tab |> insert("a", A(f1 = 1))
    var panicked = false
    try {
        with_(tab["a"]) $(v) {
            tab |> insert("b", A(f1 = 2))
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "table insert during with_ body must panic")
}

[test]
def test_table_erase_inside_panics(t : T?) {
    var tab : table<string; A>
    tab |> insert("a", A(f1 = 1))
    tab |> insert("b", A(f1 = 2))
    var panicked = false
    try {
        with_(tab["a"]) $(v) {
            tab |> erase("b")
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "table erase during with_ body must panic")
}

[test]
def test_multi_arg_inner_push_panics(t : T?) {
    var a <- [A(f1=1)]
    var b <- [A(f1=2)]
    var panicked = false
    try {
        with_(a[0], b[0]) $(va, vb) {
            b |> push(A(f1 = 99))
        }
    } recover {
        panicked = true
    }
    t |> success(panicked, "multi-arg: mutation to either locked array must panic")
}
