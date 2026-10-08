options gen2

require smt/daslib/smt_macro
require strings

// Pack textures into the PROVABLY minimal square atlas: binary-search the side
// with incremental push()/pop(), then show side-1 is UNSAT. Sizes in 8px units.

let TEXES = [(w = 4, h = 4), (w = 4, h = 4), (w = 6, h = 2), (w = 2, h = 6), (w = 3, h = 3),
    (w = 4, h = 2), (w = 2, h = 2), (w = 2, h = 2), (w = 2, h = 2), (w = 5, h = 1)]

[export]
def main {
    let n = length(TEXES)
    with_solver() $(var s) {
        let side = s |> int_const("side")
        let xs <- [for (i in range(n)); s |> int_const("x{i}")]
        let ys <- [for (i in range(n)); s |> int_const("y{i}")]
        s |> constrain(all_of(n) $(i) => xs[i] >= 0 && ys[i] >= 0 && xs[i] + TEXES[i].w <= side && ys[i] + TEXES[i].h <= side)
        // no overlap: every pair separated on at least one axis
        s |> constrain(all_pairs(n) $(i, j) => xs[i] + TEXES[i].w <= xs[j] || xs[j] + TEXES[j].w <= xs[i] ||
            ys[i] + TEXES[i].h <= ys[j] || ys[j] + TEXES[j].h <= ys[i])
        print("--- formula (SMT-LIB2, before search) ---\n{s |> to_smtlib()}------------------------------------------\n")
        var lo = 1
        var hi = 0
        for (t in TEXES) {
            hi += t.w
        }
        var bx, by : array<int>
        while (lo < hi) {
            let mid = (lo + hi) / 2
            s |> push()
            s |> add(side <= mid)
            if ((s |> check()) == SmtResult.SAT) {
                delete bx
                delete by
                bx <- [for (x in xs); s |> model_int(x)]
                by <- [for (y in ys); s |> model_int(y)]
                hi = mid
            } else {
                lo = mid + 1
            }
            s |> pop()
        }
        s |> push()
        s |> add(side < lo)
        verify((s |> check()) == SmtResult.UNSAT)   // the minimality proof
        s |> pop()
        print("minimal atlas side = {lo}  ({lo - 1} is UNSAT - no smaller square fits)\n")
        assert(lo == 10)

        for (r in range(lo)) {
            var line = ""
            for (c in range(lo)) {
                var t = "."
                for (i in range(n)) {
                    if (c >= bx[i] && c < bx[i] + TEXES[i].w && r >= by[i] && r < by[i] + TEXES[i].h) {
                        t = to_char('A' + i)
                    }
                }
                line += t
            }
            print("{line}\n")
        }
    }
}
