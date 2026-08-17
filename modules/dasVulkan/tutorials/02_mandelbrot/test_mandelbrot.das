options gen2
options _comment_hygiene = true
options indenting = 4

// Tutorial 02 pixel-oracle gate (lavapipe in CI, real GPU locally). Mandelbrot is analytic, so the
// asserts are exact properties of the math rather than a golden image: interior points never escape
// (black), exterior points escape (colored), and the set is mirror-symmetric about the real axis --
// pixel(x,y) == pixel(x, DIM-1-y), because c and its conjugate produce identical |z| sequences. The
// symmetry pairs are picked deep inside / outside the set so a 1-ULP asymmetry in the conjugate can
// never flip an escape count; that makes the equality exact on lavapipe and real hardware alike.

require dastest/testing_boost public
require mandelbrot_tut

[test]
def test_mandelbrot(t : T?) {
    var p <- render_mandelbrot()

    // interior -> black (n hits MAX_ITER, imageStore writes 0,0,0)
    let center = px(p, 256, 256)            // c ~ (-0.5, 0): inside the main cardioid
    t |> success(center.x == 0 && center.y == 0 && center.z == 0, "center is inside the set -> black")
    let bulb = px(p, 170, 256)              // c ~ (-1, 0): inside the period-2 bulb
    t |> success(bulb.x == 0 && bulb.y == 0 && bulb.z == 0, "c ~ -1 is inside the period-2 bulb -> black")

    // exterior -> escape-count ramp, never black
    let tl = px(p, 0, 0)                     // |c| > 2: escapes immediately, ramp is blue-dominant
    t |> success(tl.z > 200, "top-left corner is outside the set -> colored (blue-dominant)")
    let br = px(p, 511, 511)                 // outside on the other diagonal
    t |> success(!(br.x == 0 && br.y == 0 && br.z == 0), "bottom-right corner is outside -> not black")

    // reflection symmetry about the real axis: pixel(x,y) == pixel(x, DIM-1-y)
    let ext_top = px(p, 0, 100)             // far-left column escapes at n=1 for both mirror rows
    let ext_bot = px(p, 0, 411)
    t |> success(ext_top.x == ext_bot.x && ext_top.y == ext_bot.y && ext_top.z == ext_bot.z,
        "exterior mirror pair is identical (conjugate c escapes identically)")
    let in_top = px(p, 256, 250)            // center column, both rows deep inside -> both black
    let in_bot = px(p, 256, 261)
    t |> success(in_top.x == in_bot.x && in_top.y == in_bot.y && in_top.z == in_bot.z,
        "interior mirror pair is identical")

    delete p
}
