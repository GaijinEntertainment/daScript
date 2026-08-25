options gen2
require dastest/testing_boost public

var g_hits = 0

def bump() {
    g_hits++
}

def bump_by(n : int) {
    g_hits += n
}

def call_void(a : block<(x : int) : void>) {
    a(3)
}

def ret_void_explicit(n : int) : void {
    return bump_by(n)
}

def ret_void_auto(n : int) {
    return bump_by(n)
}

def ret_void_early(n : int) {
    if (n > 0) {
        return bump_by(n)
    }
    bump_by(-1)
}

def ret_void_pipe(n : int) {
    return n |> bump_by()
}

def ret_void_fn_arrow(n : int) => bump_by(n)

def forward_void(blk : block<() : void>) {
    return blk()
}

def forward_generic(blk) {
    return blk()
}

def mixed_result(n : int) : int {
    call_void() $(x) => bump_by(x)
    return g_hits + n
}

[test]
def test_return_void_expression(t : T?) {
    t |> run("block arrow with void body") @(t : T?) {
        g_hits = 0
        call_void() $(x) => bump_by(x)
        t |> equal(g_hits, 3)
    }

    t |> run("block arrow with explicit void result") @(t : T?) {
        g_hits = 0
        call_void() $(x : int) : void => bump_by(x)
        t |> equal(g_hits, 3)
    }

    t |> run("lambda arrow with void body") @(t : T?) {
        g_hits = 0
        let f = @(x : int) => bump_by(x)
        invoke(f, 5)
        t |> equal(g_hits, 5)
    }

    t |> run("local function arrow with void body") @(t : T?) {
        g_hits = 0
        let f = @@(x : int) => bump_by(x)
        invoke(f, 7)
        t |> equal(g_hits, 7)
    }

    t |> run("explicit return of a void call") @(t : T?) {
        g_hits = 0
        ret_void_explicit(11)
        t |> equal(g_hits, 11)
    }

    t |> run("auto result forced to void") @(t : T?) {
        g_hits = 0
        ret_void_auto(13)
        t |> equal(g_hits, 13)
        let fp : function<(n : int) : void> = @@ret_void_auto
        g_hits = 0
        invoke(fp, 2)
        t |> equal(g_hits, 2)
    }

    t |> run("early-out return of a void call") @(t : T?) {
        g_hits = 0
        ret_void_early(17)
        t |> equal(g_hits, 17)
        g_hits = 0
        ret_void_early(0)
        t |> equal(g_hits, -1)
    }

    t |> run("pipe into void call in return position") @(t : T?) {
        g_hits = 0
        ret_void_pipe(19)
        t |> equal(g_hits, 19)
    }

    t |> run("named function arrow body with void call") @(t : T?) {
        g_hits = 0
        ret_void_fn_arrow(23)
        t |> equal(g_hits, 23)
    }
}

[test]
def test_return_void_forwarding(t : T?) {
    t |> run("forwarding a void block result") @(t : T?) {
        g_hits = 0
        forward_void() $ => bump()
        t |> equal(g_hits, 1)
    }

    t |> run("generic forwarding of a void block") @(t : T?) {
        g_hits = 0
        forward_generic() $ => bump()
        t |> equal(g_hits, 1)
    }

    t |> run("void block inside a non-void function") @(t : T?) {
        g_hits = 0
        t |> equal(mixed_result(10), 13)
    }
}
