options gen2

require smt/daslib/smt_macro

// Two questions about integer triples, two SMT verdicts — each an ordinary das
// bool function lifted by [smt_fn], then handed to Z3:
//   * a Pythagorean triple with perimeter 1000 (Project Euler #9) -> SOLVED (SAT)
//   * Fermat's Last Theorem, exponent 3                           -> UNDECIDED (UNKNOWN)
// Same technique — the difference is entirely what Z3 can decide.

[smt_fn]
def is_right(a, b, c : int) : bool => a * a + b * b == c * c

[smt_fn]
def is_fermat3(a, b, c : int) : bool => a * a * a + b * b * b == c * c * c

def set_timeout(s : Solver; ms : uint) {
    let p = Z3_mk_params(s.ctx)
    Z3_params_inc_ref(s.ctx, p)
    Z3_params_set_uint(s.ctx, p, Z3_mk_string_symbol(s.ctx, "timeout"), ms)
    Z3_solver_set_params(s.ctx, s.solver, p)
    Z3_params_dec_ref(s.ctx, p)
}

// SOLVE: the Pythagorean triple with a + b + c == 1000 (Project Euler #9).
def solve_euler9() {
    with_solver() $(var s) {
        let a = s |> int_const("a")
        let b = s |> int_const("b")
        let c = s |> int_const("c")
        s |> add(is_right(a, b, c))
        s |> constrain(a > 0 && b > 0 && c > 0)
        s |> add(a + b + c == 1000)
        if ((s |> check()) == SmtResult.SAT) {
            let av = s |> model_int(a)
            let bv = s |> model_int(b)
            let cv = s |> model_int(c)
            print("SOLVED   : {av}^2+{bv}^2={cv}^2, perimeter 1000 (product {av * bv * cv}).\n")
        } else {
            print("SOLVED   : no such triple.\n")
        }
    }
}

// DECIDE: Fermat's Last Theorem, exponent 3 — a^3+b^3=c^3 has no positive
// solution (Euler, 1770). Unbounded nonlinear integer arithmetic is
// undecidable, so with a timeout Z3 returns UNKNOWN — it cannot settle this.
def decide_fermat() {
    with_solver() $(var s) {
        s |> set_timeout(3000u)                          // 3s, then give up
        let a = s |> int_const("a")
        let b = s |> int_const("b")
        let c = s |> int_const("c")
        s |> add(is_fermat3(a, b, c))
        s |> constrain(a > 0 && b > 0 && c > 0)
        let r = s |> check()
        if (r == SmtResult.UNKNOWN) {
            print("UNDECIDED: Z3 can neither find a^3+b^3=c^3 nor rule it out - FLT is beyond SMT.\n")
        } elif (r == SmtResult.SAT) {
            print("UNDECIDED: solution found - this would DISPROVE Fermat: {s |> model_int(a)}^3+{s |> model_int(b)}^3={s |> model_int(c)}^3\n")
        } else {
            print("UNDECIDED: proven impossible - Z3 just did what took mathematics 350 years.\n")
        }
    }
}

[export]
def main {
    solve_euler9()
    decide_fermat()
    print("theorems ok\n")
}
