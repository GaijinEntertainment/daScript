options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require math

// Animation lerp functions (pure math, mirrored from main.das)

def chip_place_scale(t : float) : float {
    let tc = clamp(t, 0.0, 1.0)
    if (tc < 0.6) {
        return tc / 0.6 * 1.2
    } else {
        return 1.2 - (tc - 0.6) / 0.4 * 0.2
    }
}

def chip_remove_scale(t : float) : float {
    let tc = clamp(t, 0.0, 1.0)
    if (tc < 0.2) {
        return 1.0 + tc / 0.2 * 0.3
    } else {
        return 1.3 * (1.0 - (tc - 0.2) / 0.8)
    }
}

def chip_remove_alpha(t : float) : float {
    return 1.0 - clamp(t, 0.0, 1.0)
}

[test]
def test_chip_place_scale(t : T?) {
    t |> run("starts at zero") @(t : T?) {
        t |> success(abs(chip_place_scale(0.0)) < 0.001)
    }
    t |> run("overshoots to 1.2 at t=0.6") @(t : T?) {
        t |> success(abs(chip_place_scale(0.6) - 1.2) < 0.001)
    }
    t |> run("settles to 1.0 at t=1.0") @(t : T?) {
        t |> success(abs(chip_place_scale(1.0) - 1.0) < 0.001)
    }
    t |> run("monotonically increases in 0..0.6") @(t : T?) {
        var prev = chip_place_scale(0.0)
        var ok = true
        for (i in range(1, 7)) {
            let v = chip_place_scale(float(i) * 0.1)
            if (v < prev) {
                ok = false
            }
            prev = v
        }
        t |> success(ok, "should be monotonically increasing")
    }
    t |> run("decreases from overshoot in 0.6..1.0") @(t : T?) {
        let v06 = chip_place_scale(0.6)
        let v08 = chip_place_scale(0.8)
        let v10 = chip_place_scale(1.0)
        t |> success(v06 > v08, "0.6 > 0.8")
        t |> success(v08 > v10, "0.8 > 1.0")
    }
    t |> run("clamped beyond 1.0") @(t : T?) {
        t |> success(abs(chip_place_scale(1.5) - 1.0) < 0.001)
    }
}

[test]
def test_chip_remove_scale(t : T?) {
    t |> run("starts at 1.0") @(t : T?) {
        t |> success(abs(chip_remove_scale(0.0) - 1.0) < 0.001)
    }
    t |> run("flashes up to 1.3 at t=0.2") @(t : T?) {
        t |> success(abs(chip_remove_scale(0.2) - 1.3) < 0.001)
    }
    t |> run("shrinks to 0 at t=1.0") @(t : T?) {
        t |> success(abs(chip_remove_scale(1.0)) < 0.001)
    }
    t |> run("always non-negative") @(t : T?) {
        var ok = true
        for (i in range(11)) {
            if (chip_remove_scale(float(i) * 0.1) < -0.001) {
                ok = false
            }
        }
        t |> success(ok)
    }
}

[test]
def test_chip_remove_alpha(t : T?) {
    t |> run("starts opaque") @(t : T?) {
        t |> success(abs(chip_remove_alpha(0.0) - 1.0) < 0.001)
    }
    t |> run("ends transparent") @(t : T?) {
        t |> success(abs(chip_remove_alpha(1.0)) < 0.001)
    }
    t |> run("monotonically decreases") @(t : T?) {
        var prev = chip_remove_alpha(0.0)
        var ok = true
        for (i in range(1, 11)) {
            let v = chip_remove_alpha(float(i) * 0.1)
            if (v > prev) {
                ok = false
            }
            prev = v
        }
        t |> success(ok)
    }
    t |> run("clamped at boundaries") @(t : T?) {
        t |> success(abs(chip_remove_alpha(-0.5) - 1.0) < 0.001)
        t |> success(abs(chip_remove_alpha(1.5)) < 0.001)
    }
}
