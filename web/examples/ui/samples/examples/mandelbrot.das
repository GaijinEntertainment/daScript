// Mandelbrot benchmark — ported from dasProfile/tests/mandelbrot.das.
// 64×64 grid, 256-iteration escape time. Prints `profile` timings.
options gen2

require math

def level(c : float2) : int {
    var l = 0
    var z = c
    while (length(z) < 2.0 && l < 255) {
        z = float2(z.x * z.x - z.y * z.y, z.x * z.y + z.y * z.x) + c
        l ++
    }
    return l - 1
}

[sideeffects]
def test {
    let xmin = -2.0
    let xmax = 2.0
    let ymin = -2.0
    let ymax = 2.0
    let N = 64
    let dx = (xmax - xmin) / float(N)
    let dy = (ymax - ymin) / float(N)
    var S = 0
    var xy = float2(xmin, ymin)
    for (_i in range(N)) {
        xy.y = ymin
        for (_j in range(N)) {
            S += level(xy)
            xy.y += dy
        }
        xy.x += dx
    }
    return S
}

var S = 0

[export]
def main {
    profile(10, "mandelbrot") {
        S = test()
    }
}
