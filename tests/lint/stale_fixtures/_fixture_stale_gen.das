options gen2

def public gen_push(var dst : array<auto(TT)>; src : array<TT>) {
    for (s in src) { // nolint:PERF022
        dst |> push(s) // nolint:PERF006
    }
}
