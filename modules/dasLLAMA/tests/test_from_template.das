options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_tune

def template twice_template(x : int) : int {
    return PH(x) + PH(x)
}

def private inc(x : int) : int => x + 1

def private dbl(x : int) : int => x * 2

[from_template(src = "twice_template", PH = "inc")]
def private twice_inc(x : int) : int {}

[from_template(src = "twice_template", PH = "dbl")]
def private twice_dbl(x : int) : int {}

def template scale_template(ws : float const ?; bi : int64) : float {
    return WS(ws, bi) * 2.0
}

def private ws_f32(ws : float const?; bi : int64) : float {
    unsafe {
        return ws[bi]
    }
}

def private ws_half(ws : uint16 const?; bi : int64) : float {
    unsafe {
        return float(ws[bi]) * 0.5
    }
}

[from_template(src = "scale_template", WS = "ws_f32")]
def private scaled(ws : float const?; bi : int64) : float {}

[from_template(src = "scale_template", WS = "ws_half")]
def private scaled(ws : uint16 const?; bi : int64) : float {}

[test]
def test_from_template(t : T?) {
    t |> run("a placeholder call renames to the annotation's target per stub") @(t : T?) {
        t |> equal(8, twice_inc(3), "PH = inc: (3 + 1) + (3 + 1)")
        t |> equal(12, twice_dbl(3), "PH = dbl: (3 * 2) + (3 * 2)")
    }
    t |> run("the stub's signature types the clone, so one template stamps both plane overloads") @(t : T?) {
        var f = [1.5, 2.5]
        var h = [uint16(3), uint16(4)]
        unsafe {
            t |> equal(5.0, scaled(addr(f[0]), 1l), "the float plane reads its scale")
            t |> equal(4.0, scaled(addr(h[0]), 1l), "the halfword plane widens through its own reader")
        }
    }
}
